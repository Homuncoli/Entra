#include <gtest/gtest.h>

#include "Entra.h"

using namespace Entra;

TEST(Entity, Creation) {
    Registry registry;
    EntityId id = registry.addEntity();
    ASSERT_TRUE(registry.hasEntity(id));
}

TEST(Entity, Delete) {
    Registry registry;
    
    EntityId id = registry.addEntity();
    registry.removeEntity(id);
    ASSERT_FALSE(registry.hasEntity(id));
}