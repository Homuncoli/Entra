#include <gtest/gtest.h>

#include "Entra.h"

TEST(Registry, Creation) {
    Entra::Registry* registry;
    ASSERT_NO_THROW(registry = new Entra::Registry());
    delete registry;
}

TEST(Registry, Deletion) {
    Entra::Registry* registry = new Entra::Registry();
    ASSERT_NO_THROW(delete registry);
}