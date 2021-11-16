#pragma once

#include "Entity.h"
#include "Component.h"
#include "BaseSystem.h"

#include <any>
#include <vector>
#include <map>
#include <deque>
#include <bitset>
#include <type_traits>
#include <memory>
#include <any>
#include <algorithm>

#include <iostream>

namespace Entra {
    class Registry {
        private:
            EntityId nextEntityId = 1;

            std::deque<EntityId> freeEntityIds;
            // which entity HAS which components
            std::map<EntityId, Signiture> entitySignitures;
            // which entity owns which index in the components array
            std::map<EntityId, std::map<ComponentId, size_t>> entityToIndex;
            std::map<size_t, std::map<ComponentId, EntityId>> indexToEntity;

            // store all instances of one component continously
            std::map<ComponentId, std::vector<std::any>> components;
            
            std::vector<BaseSystem*> systems;
            std::map<Entra::SystemId, size_t> systemToIndexMapping;
            std::map<size_t, Entra::SystemId> indexToSystemMapping;

        public:
            explicit Registry();
            ~Registry();

            EntityId addEntity();
            bool hasEntity(EntityId id);
            void removeEntity(EntityId id);
            void update(double deltaTime);

            template<class System, class...Args>
            System* addSystem(Args...args) {
                static_assert(std::is_base_of<Entra::BaseSystem, System>::value, "System must derive from Entra::System");
                
                SystemId systemId = getSystemId<System>();
                auto findSystem = systemToIndexMapping.find(systemId);
                size_t index = -1;
                
                // if this type of system is already added, replace it
                if (findSystem != systemToIndexMapping.end()) {
                    index = findSystem->second;
                    delete systems[index];
                    systems[index] = new System(this, std::forward<Args>(args)...);
                } else {
                    index = systems.size();
                    systemToIndexMapping.emplace(systemId, index);
                    indexToSystemMapping.emplace(index, systemId);
                    systems.push_back(new System(this, std::forward<Args>(args)...));
                }

                return static_cast<System*>(systems[index]);
            }

            template<class System>
            System* getSystem() {
                size_t index = systemToIndexMapping[getSystemId<System>()];
                return static_cast<System*>(systems[index]);
            }

            template<class System>
            bool hasSystem() {
                SystemId id = getSystemId<System>();
                return systemToIndexMapping.find(id) != systemToIndexMapping.end();
            }

            template<class System>
            void removeSystem() {
                SystemId currentId = getSystemId<System>();
                size_t currentIndex = systemToIndexMapping[currentId];
                size_t lastIndex = systems.size()-1;
                SystemId lastId = indexToSystemMapping[lastIndex];
                
                delete systems[currentIndex];
                systems[currentIndex] = systems[lastIndex];
                systems.pop_back();

                systemToIndexMapping.erase(currentId);
                indexToSystemMapping.erase(currentIndex);
                systemToIndexMapping[lastId] = currentIndex;
                indexToSystemMapping[currentIndex] = lastId;
            }

            template<class T, class...Args>
            void addComponent(EntityId id, Args... args) {
                const ComponentId cId = getComponentId<T>();
                const Signiture cSigniture = getSigniture<T>();
                const size_t index = components[cId].size();

                auto find = entityToIndex[id].find(cId);
                if (find != entityToIndex[id].end()) {
                    std::cerr << id << " tries to have multiple " << cId << std::endl;
                }

                entitySignitures[id] |= cSigniture;
                entityToIndex[id][cId] = index;
                indexToEntity[index][cId] = id;
                components[cId].push_back(T(std::forward<Args>(args)...));

                for (int i=0; i<systems.size(); i++) {
                    systems[i]->processEntity(id, entitySignitures[id]);
                }
            }

            template<class Component>
            bool hasComponent(EntityId eId) {
                const ComponentId cId = getComponentId<Component>();
                return entityToIndex[eId].find(cId) != entityToIndex[eId].end();
            }

            template<class Component>
            Component* getComponent(EntityId eId) {
                const ComponentId cId = getComponentId<Component>();
                const size_t index = entityToIndex[eId][cId];
                return std::any_cast<Component>(&(components[cId][index]));     
            }

            template<class Component>
            std::tuple<Component*> getComponents(EntityId eId) {
                return std::make_tuple(getComponent<Component>(eId));
            }

            template<class Component, class Other, class...Rest>
            std::tuple<Component*, Other*, std::add_pointer_t<Rest>...>  getComponents(EntityId eId) {
                return std::tuple_cat(std::make_tuple(getComponent<Component>(eId)), getComponents<Other, Rest...>(eId));
            }

            template<class Component>
            void removeComponent(EntityId currentId) {
                const ComponentId componentId = getComponentId<Component>();

                const size_t currentIndex = entityToIndex[currentId][componentId];
                const size_t lastIndex = components[componentId].size()-1;
                const EntityId lastId = indexToEntity[currentIndex][componentId];

                components[componentId][currentIndex] = components[componentId][lastId];
                components[componentId].pop_back();

                entityToIndex[currentId].erase(componentId);
                indexToEntity[lastIndex].erase(componentId);

                if (currentIndex != lastIndex) {
                    entityToIndex[lastId][componentId] = currentIndex;
                    indexToEntity[currentIndex][componentId] = lastId;
                }
            }
    };
}