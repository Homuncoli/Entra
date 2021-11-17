#include <gtest/gtest.h>

#include "Entra.h"

using namespace Entra;

class EntityFixture : public ::testing::Test {
  protected:
    Entra::Registry* registry;

    void SetUp() override {
      registry = new Entra::Registry();
    }

    void TearDown() override {
      delete registry;
    }
};

TEST_F(EntityFixture, Add) {
    EntityId id = registry->addEntity();
    ASSERT_TRUE(registry->hasEntity(id)) << "should have created the entity";
}

TEST_F(EntityFixture, Remove) {
    EntityId id = registry->addEntity();
    ASSERT_TRUE(registry->hasEntity(id)) << "expected entity to have been created";

    registry->removeEntity(id);
    ASSERT_FALSE(registry->hasEntity(id)) << "should NOT have entity any longer";
}