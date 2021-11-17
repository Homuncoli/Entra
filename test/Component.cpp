#include <gtest/gtest.h>

#include "Entra.h"

using namespace Entra;

struct Argument {
    std::string arg;

    Argument(std::string arg) : arg(arg) {}
};

struct Transform {
    float x,y,z;
};

struct Rotation {
    float pitch,yaw,roll;
};

class ComponentFixture : public ::testing::Test {
  protected:
    Entra::Registry* registry;
    EntityId entity;

    virtual void SetUp() {
      registry = new Entra::Registry();
      entity = registry->addEntity();
    }

    virtual void TearDown() {
      delete registry;
    }
};

TEST_F(ComponentFixture, AddSingle) {
    bool has = registry->hasComponent<Transform>(entity);
    ASSERT_FALSE(has);

    registry->addComponent<Transform>(entity);
    has = registry->hasComponent<Transform>(entity);
    ASSERT_TRUE(has);
}

TEST_F(ComponentFixture, AddWithArgument) {
    std::string arg = "AddWithArgument";
    registry->addComponent<Argument>(entity, arg);

    ASSERT_TRUE(registry->hasComponent<Argument>(entity)) << "should have Argument component";
    ASSERT_EQ(registry->getComponent<Argument>(entity)->arg, arg) << "should have arg set";
}

TEST_F(ComponentFixture, AddMultiple) {
    registry->addComponent<Transform>(entity);
    registry->addComponent<Rotation>(entity);

    ASSERT_TRUE(registry->hasComponent<Transform>(entity)) << "should have Transfrom component";
    ASSERT_TRUE(registry->hasComponent<Rotation>(entity)) << "should have Rotation component";
}

TEST_F(ComponentFixture, RemoveSingle) {
    registry->addComponent<Transform>(entity);
    ASSERT_TRUE(registry->hasComponent<Transform>(entity));

    registry->removeComponent<Transform>(entity);
    ASSERT_FALSE(registry->hasComponent<Transform>(entity));
}

TEST_F(ComponentFixture, RemoveSingleFromMultiple) {
    registry->addComponent<Transform>(entity);
    registry->addComponent<Rotation>(entity);
    ASSERT_TRUE(registry->hasComponent<Transform>(entity)) << "expected to have Transfrom component";
    ASSERT_TRUE(registry->hasComponent<Rotation>(entity)) << "expected have Rotation component";

    registry->removeComponent<Transform>(entity);
    ASSERT_FALSE(registry->hasComponent<Transform>(entity)) << "should NOT have Transfrom component";
    ASSERT_TRUE(registry->hasComponent<Rotation>(entity)) << "should have Rotation component";
}

TEST_F(ComponentFixture, RemoveMultiple) {
    registry->addComponent<Transform>(entity);
    registry->addComponent<Rotation>(entity);
    ASSERT_TRUE(registry->hasComponent<Transform>(entity)) << "expected to have Transfrom component";
    ASSERT_TRUE(registry->hasComponent<Rotation>(entity)) << "expected have Rotation component";

    registry->removeComponent<Transform>(entity);
    registry->removeComponent<Rotation>(entity);
    ASSERT_FALSE(registry->hasComponent<Transform>(entity)) << "should NOT have Transfrom component";
    ASSERT_FALSE(registry->hasComponent<Rotation>(entity)) << "should NOT have Rotation component";
}

TEST_F(ComponentFixture, Get) {
    auto* transform = registry->addComponent<Argument>(entity, "Test");
    ASSERT_TRUE(registry->hasComponent<Argument>(entity)) << "expected to have Argument component";

    auto* get = registry->getComponent<Argument>(entity);
    ASSERT_EQ(transform->arg, get->arg);
}

TEST_F(ComponentFixture, GetModifing) {
    std::string newText = "New";
    auto* transform = registry->addComponent<Argument>(entity, "Test");
    ASSERT_TRUE(registry->hasComponent<Argument>(entity)) << "expected to have Argument component";

    transform->arg = newText;

    auto* get = registry->getComponent<Argument>(entity);
    ASSERT_EQ(transform->arg, get->arg);
    ASSERT_EQ(transform->arg, newText);
}