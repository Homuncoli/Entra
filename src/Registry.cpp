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
    return id < nextEntityId && find == freeEntityIds.end();
}

void Entra::Registry::removeEntity(EntityId id) {
    for (auto system : systems) {
        system->processEntity(id, INVALID_SIGNITURE, entitySignitures[id]);
    }

    for (auto index : entityToIndex[id]) {
        indexToEntity[index.second].erase(index.first);
    }
     entityToIndex.erase(id);
     entitySignitures.erase(id);

     freeEntityIds.push_back(id);
}

void Entra::Registry::update(double deltaTime) {
    for (int i=0; i<systems.size(); i++) {
        systems[i]->update(deltaTime);
    }
}