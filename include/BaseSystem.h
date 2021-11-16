#pragma once

#include "Registry.h"
#include "Entity.h"
#include "Component.h"

#include <vector>
#include <map>
#include <tuple>

#include <iostream>

namespace Entra {
    class BaseSystem {
        public:
            BaseSystem() = default;
            virtual ~BaseSystem() = default;
            virtual void update(double deltaTime) = 0;
            virtual void processEntity(EntityId id, Signiture signiture) = 0;
    };
}