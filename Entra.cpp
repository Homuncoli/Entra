#include <iostream>
#include <chrono>

#include "Entra.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

struct Transform {
    float x,y,z,w;

    Transform(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 0;
    }

    static Entra::PoolAllocator allocator;

    static void* operator new(size_t size) {
        return allocator.allocate(size);
    }
 
    static void operator delete(void* ptr, size_t size) {
        return allocator.deallocate(ptr, size);
    }
};
Entra::PoolAllocator Transform::allocator{8};

struct Rotation {
    float pitch, yaw, roll;
};

class TransformationSystem : public Entra::System<Transform> {
    private:
        float gravity;
    public:
        explicit TransformationSystem(Entra::Registry* pRegistry, float g): Entra::System<Transform>(pRegistry), gravity(g) {}
        ~TransformationSystem() = default;
        
        void update(double deltaTime) {
            for (auto component : components) {
                std::get<0>(component)->x *= gravity * deltaTime;
            }
        }
};

int main(int argc, char **argv) {
    size_t N_ENTITIES = 10000;
    size_t N_CYCLES = 10000;

    if (argc >= 2) {
        N_ENTITIES = std::atoi(argv[1]);
    }
    if (argc >= 3) {
        N_CYCLES = std::atoi(argv[2]);
    }
    
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
        registry->addComponent<Transform>(id, i, 0, 0);
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
