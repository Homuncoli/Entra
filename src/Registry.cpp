#include "Registry.h"

Entra::Registry::Registry() {

}

Entra::Registry::~Registry() {

}

Entra::EntityId Entra::Registry::createEntity() {
    EntityId id = INVALID_ENTITY_ID;
    if (freeEntityIds.size() > 0) {
        id = freeEntityIds.front();
        freeEntityIds.pop();
    } else {
        id = nextEntityId++;
    }

    entitySignitures.emplace(id, Entra::Signiture());

    return id;
}

void Entra::Registry::deleteEntity(EntityId id) {
    entitySignitures.erase(id);
    entityComponentIndex.erase(id);
    // ToDo: Delete Components, be mindful of the index in entityComponentIndex, since the indicies of other entites should not all be changed
    freeEntityIds.push(id);
}