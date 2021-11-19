#include <iostream>
#include <chrono>

#include "Entra.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

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

class TransformationSystem : public Entra::System<Transform, Rotation> {
    private:
        float gravity;
    public:
        explicit TransformationSystem(Entra::Registry* pRegistry, float g): Entra::System<Transform, Rotation>(pRegistry), gravity(g) {}
        
        void update(double deltaTime) {
            for (auto component : components) {
                for (int i=0; i<20; i++)
                    std::get<0>(component)->y *= gravity * deltaTime;
            }
        }
};

int main() {
    constexpr size_t N_ENTITIES = 10000;
    constexpr size_t N_CYCLES = 100;
    
    auto start = high_resolution_clock::now();
    Entra::Registry* registry = new Entra::Registry();
    auto end = high_resolution_clock::now();
    std::cout << "registry creation: " << duration_cast<milliseconds>(end - start).count() << "ms" << std::endl;

    start = high_resolution_clock::now();
    registry->addSystem<TransformationSystem>(9.81f);
    end = high_resolution_clock::now();
    std::cout << "system creation: " << duration_cast<milliseconds>(end - start).count() << "ms" << std::endl;

    start = high_resolution_clock::now();
    for (int i=0; i<N_ENTITIES; i++) {
        Entra::EntityId id = registry->addEntity();
        registry->addComponent<Transform>(id, 0, 0, 0);
        registry->addComponent<Rotation>(id);
    }
    end = high_resolution_clock::now();
    std::cout << N_ENTITIES << " entities creation: " << duration_cast<milliseconds>(end - start).count() << "ms" << std::endl;

    start = high_resolution_clock::now();
    for (int i=0; i<N_CYCLES; i++) {
        registry->update(0.1f);
    }
    end = high_resolution_clock::now();
    std::cout << N_CYCLES << " updates: " << duration_cast<milliseconds>(end - start).count() << "ms" << std::endl;
    
    delete registry;
    return 0;
}
