#include "Registry.h"

Entra::Registry::Registry() {

}

Entra::Registry::~Registry() {
    for (auto* system : systems) {
        delete system;
    }
}

Entra::EntityId Entra::Registry::addEntity() {
    EntityId id = INVALID_ENTITY_ID;
    if (freeEntityIds.size() > 0) {
        id = freeEntityIds.front();
        freeEntityIds.pop_front();
    } else {
        id = nextEntityId++;
    }

    entitySignitures.emplace(id, Entra::Signiture());

    return id;
}

bool Entra::Registry::hasEntity(EntityId id) {
    auto find = std::find(freeEntityIds.begin(), freeEntityIds.end(), id);
    return id <= nextEntityId && find == freeEntityIds.end();
}

void Entra::Registry::removeEntity(EntityId id) {
    // ToDo: Remove
}

void Entra::Registry::update(double deltaTime) {
    for (int i=0; i<systems.size(); i++) {
        systems[i]->update(deltaTime);
    }
}