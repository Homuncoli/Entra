#pragma once

#include <typeinfo>
#include <cstdint>
#include <bitset>
#include <map>

namespace Entra {
    constexpr int MAX_COMPONENTS = 32;

    typedef std::size_t ComponentId;
    typedef std::bitset<MAX_COMPONENTS> Signiture;

    constexpr ComponentId INVALID_COMPONENT_ID = 0;
    constexpr Signiture INVALID_SIGNITURE = 0;

    // ToDo: make them static in getSigniture
    // does not work so easily because static variables are redefined for each template class
    extern std::map<ComponentId, Signiture> componentSignitures;
    extern size_t nComponentTypes;

    template<class T>
    ComponentId getComponentId() {
        return typeid(T).hash_code();
    }

    template<class Component>
    Signiture getSigniture() {
        const ComponentId cId = getComponentId<Component>();
        Signiture signiture = INVALID_SIGNITURE;

        if (componentSignitures.count(cId) > 0) {
            signiture = componentSignitures[cId];
        } else {
            signiture = 1 << (nComponentTypes++);
            componentSignitures.emplace(cId, signiture);
        }

        return signiture;
    }

    template<class Component, class Other, class...Rest>
    Signiture getSigniture() {
        return getSigniture<Component>() | getSigniture<Other, Rest...>();
    }
}