#include <gtest/gtest.h>

#include "Entra.h"

using namespace Entra;

static Registry registry;

struct Transform {
    float x,y,z;
};

struct Rotation {
    float pitch,yaw,roll;
};

class ArgumentSystem : public Entra::System<Transform> {
    private:
        std::string a;
    public:
        explicit ArgumentSystem(Entra::Registry* pRegistry): Entra::System<Transform>(pRegistry) {}
        explicit ArgumentSystem(Entra::Registry* pRegistry, std::string a): Entra::System<Transform>(pRegistry), a(a) {}
        void update(double deltaTime) {}
        std::string getA() { return a; }
};

class SingleSignitureSystem : public Entra::System<Transform> {
    public:
        explicit SingleSignitureSystem(Entra::Registry* pRegistry): Entra::System<Transform>(pRegistry) {}
        void update(double deltaTime) {}
};
class MutliSignitureSystem : public Entra::System<Transform, Rotation> {
    public:
        explicit MutliSignitureSystem(Entra::Registry* pRegistry): Entra::System<Transform, Rotation>(pRegistry) {}
        void update(double deltaTime) {}
};

TEST(System, CreationWithoutArgs) {
    ArgumentSystem* system = registry.addSystem<ArgumentSystem>();
    ASSERT_EQ(system->getA(), std::string());
}
TEST(System, CreationWithArgs) {
    ArgumentSystem* system = registry.addSystem<ArgumentSystem>("AAA");
    ASSERT_EQ(system->getA(), "AAA");
}

TEST(System, SingleSigniture) {
    SingleSignitureSystem* system = registry.addSystem<SingleSignitureSystem>();
    ASSERT_EQ(system->getSigniture().count(), 1);
}

TEST(System, MultipleSigniture) {
    MutliSignitureSystem* system = registry.addSystem<MutliSignitureSystem>();
    ASSERT_EQ(system->getSigniture().count(), 2);
}

TEST(System, Get) {
    ArgumentSystem* init = registry.addSystem<ArgumentSystem>("AAA");
    ArgumentSystem* get = registry.getSystem<ArgumentSystem>();
    ASSERT_EQ(init, get);
}

TEST(System, Delete) {
    ArgumentSystem* init = registry.addSystem<ArgumentSystem>("BBB");
    ASSERT_TRUE(registry.hasSystem<ArgumentSystem>());

    registry.removeSystem<ArgumentSystem>();
    ASSERT_FALSE(registry.hasSystem<ArgumentSystem>());
}