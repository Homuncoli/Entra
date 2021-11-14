#pragma once

#include "Entity.h"
#include "Component.h"

#include <any>
#include <vector>
#include <map>
#include <queue>
#include <bitset>

#include <iostream>

namespace Entra {
    class Registry {
        private:
            EntityId nextEntityId = 1;
            Signiture nextComponentSigniture = 1;

            std::queue<EntityId> freeEntityIds;
            // which entity HAS which components
            std::map<EntityId, Signiture> entitySignitures;
            // which entity owns which index in the components array
            std::map<EntityId, std::map<ComponentId, size_t>> entityComponentIndex;

            // which signiture belongs to this component
            std::map<ComponentId, Signiture> componentSignitures;
            // store all instances of one component continously
            std::map<ComponentId, std::vector<std::any>> components;

            template<class T>
            ComponentId getComponentId() {
                return typeid(T).hash_code();
            }

            template<class T>
            Signiture getComponentSigniture() {
                const ComponentId cId = getComponentId<T>();
                Signiture signiture = INVALID_SIGNITURE;

                if (componentSignitures.count(cId) > 0) {
                    signiture = componentSignitures[cId];
                } else {
                    signiture = nextComponentSigniture;
                    nextComponentSigniture = (nextComponentSigniture << 1);
                    componentSignitures.emplace(cId, signiture);
                }

                return signiture;
            }

        public:
            explicit Registry();
            ~Registry();

            EntityId createEntity();
            void deleteEntity(EntityId id);

            template<class T, class...Args>
            void addComponent(EntityId id, Args... args) {
                const ComponentId cId = getComponentId<T>();
                const Signiture cSigniture = getComponentSigniture<T>();

                entitySignitures[id] = entitySignitures[id] | cSigniture;
                entityComponentIndex[id][cId] = components[cId].size();
                components[cId].push_back(T(std::forward<Args>(args)...));

                std::cout << "cId: " << cId << std::endl;
                std::cout << "cSigniture: " << cSigniture << std::endl;
            }

            template<class T>
            T* getComponent(EntityId eId) {
                const ComponentId cId = getComponentId<T>();
                const size_t index = entityComponentIndex[eId][cId];
                return std::any_cast<T>(&(components[cId][index]));
            }
    };
}