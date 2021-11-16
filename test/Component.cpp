#include <gtest/gtest.h>

#include "Entra.h"

using namespace Entra;

struct Transform {
    float x,y,z;
};

struct Rotation {
    float pitch,yaw,roll;
};

TEST(Component, Creation) {
    Registry registry;
    EntityId entity = registry.addEntity();
    ASSERT_FALSE(registry.hasComponent<Transform>(entity));

    registry.addComponent<Transform>(entity);
    ASSERT_TRUE(registry.hasComponent<Transform>(entity));
}

TEST(Component, Delete) {
    Registry registry;
    EntityId entity = registry.addEntity();

    registry.addComponent<Transform>(entity);
    ASSERT_TRUE(registry.hasComponent<Transform>(entity));

    registry.removeComponent<Transform>(entity);
    ASSERT_FALSE(registry.hasComponent<Transform>(entity));
}