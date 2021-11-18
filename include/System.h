#pragma once

#include "Registry.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "Component.h"

#include <vector>
#include <map>
#include <tuple>
#include <type_traits>

#include <iostream>

namespace Entra {
    template<class...Components>
    class System : public BaseSystem {
        protected:
            Registry* pRegistry;
            // signiture that the system requires
            const Signiture signiture;
            std::vector<std::tuple<std::add_pointer_t<Components>...>> components;
            std::map<EntityId, size_t> entityIndex;

        public:
            explicit System(Registry* pRegistry): Entra::BaseSystem(), pRegistry(pRegistry), signiture(Entra::getSigniture<Components...>()) {}
            const Signiture& getSigniture() { return signiture; }
            
            void processEntity(EntityId id, Signiture newSigniture, Signiture oldSigniture) override {
                if ((newSigniture & signiture) == signiture && (oldSigniture & signiture) != signiture) {
                    components.push_back(pRegistry->getComponents<Components...>(id));
                    entityIndex[id] = components.size() - 1;
                }
                if ((oldSigniture & signiture) == signiture && (newSigniture & signiture) != signiture) {
                    components.erase(components.begin() + entityIndex[id]);
                    entityIndex.erase(id);
                }

            }
    };
}