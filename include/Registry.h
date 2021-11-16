#pragma once

#include "Entity.h"
#include "Component.h"
#include "BaseSystem.h"

#include <any>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
#include <type_traits>
#include <memory>

#include <iostream>

namespace Entra {
    class Registry {
        private:
            EntityId nextEntityId = 1;

            std::queue<EntityId> freeEntityIds;
            // which entity HAS which components
            std::map<EntityId, Signiture> entitySignitures;
            // which entity owns which index in the components array
            std::map<EntityId, std::map<ComponentId, size_t>> entityComponentIndex;

            // store all instances of one component continously
            std::map<ComponentId, std::vector<std::any>> components;
            
            std::vector<std::unique_ptr<BaseSystem>> systems;

        public:
            explicit Registry();
            ~Registry();

            EntityId createEntity();
            void deleteEntity(EntityId id);
            void update(double deltaTime);

            template<class System, class...Args>
            void addSystem(Args...args) {
                static_assert(std::is_base_of<Entra::BaseSystem, System>::value, "System must derive from Entra::System");

                systems.push_back(std::make_unique<System>(this, std::forward<Args>(args)...));
            }

            template<class T, class...Args>
            void addComponent(EntityId id, Args... args) {
                const ComponentId cId = getComponentId<T>();
                const Signiture cSigniture = getSigniture<T>();

                entitySignitures[id] |= cSigniture;
                entityComponentIndex[id][cId] = components[cId].size();
                components[cId].push_back(T(std::forward<Args>(args)...));

                for (int i=0; i<systems.size(); i++) {
                    systems[i]->processEntity(id, entitySignitures[id]);
                }
            }

            template<class Component>
            Component* getComponent(EntityId eId) {
                const ComponentId cId = getComponentId<Component>();
                const size_t index = entityComponentIndex[eId][cId];
                return std::any_cast<Component>(&(components[cId][index]));     
            }

            template<class Component>
            std::tuple<Component*> getComponents(EntityId eId) {
                return std::make_tuple(getComponent<Component>(eId));
            }

            template<class Component, class Other, class...Rest>
            auto getComponents(EntityId eId) {
                return std::tuple_cat(std::make_tuple(getComponent<Component>(eId)), getComponents<Other, Rest...>(eId));
            }
    };
}