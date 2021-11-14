#include <iostream>

#include "Entra.h"

struct Transform {
    float x = 1,y,z;

    Transform(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Rotation {
    float pitch, yaw, roll;
};

// ToDo: Systems need to to be able to have multiple components they care about
int main() {
    Entra::Registry registry;

    Entra::EntityId id1; 
    Entra::EntityId id2; 
    Entra::EntityId id3; 

    std::cout << "Create new entity: " << (id1 = registry.createEntity()) << std::endl;
    std::cout << "Create new entity: " << (id2 = registry.createEntity()) << std::endl;
    std::cout << "Create new entity: " << (id3 = registry.createEntity()) << std::endl;
    std::cout << "====================" << std::endl;

    registry.deleteEntity(id1);
    registry.deleteEntity(id2);
    std::cout << "Delete entity" << std::endl;
    std::cout << "Delete entity" << std::endl;
    std::cout << "Create new entity: " << (id1 = registry.createEntity()) << std::endl;
    std::cout << "Create new entity: " << (id2 = registry.createEntity()) << std::endl;
    std::cout << "====================" << std::endl;

    registry.addComponent<Transform>(id1, 1, 2, 3);
    registry.addComponent<Transform>(id2, 4, 5, 6);
    registry.addComponent<Rotation>(id2);
    registry.addComponent<Rotation>(id3);
    std::cout << "====================" << std::endl;

    std::cout << registry.getComponent<Transform>(id1)->x << std::endl;
    std::cout << registry.getComponent<Transform>(id2)->x << std::endl;
    std::cout << "====================" << std::endl;

    return 0;
}
