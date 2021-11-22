#pragma once

#include <memory>
#include <limits>

// http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/#usage-and-testing
// Note: a different kind of allocator is need for arrays, vectors, etc...
//       this one only works for single objects
namespace Entra
{
    class PoolAllocator
    {
    private:
        struct Chunk
        {
            // next is a Chunk if free, else contains user data
            Chunk *next{nullptr};
        };

        size_t chunksPerBlock;
        Chunk *pAlloc = nullptr;

        Chunk *allocateBlock(size_t chunkSize)
        {
            size_t blockSize = chunksPerBlock * chunkSize;

            Chunk *blockBegin = reinterpret_cast<Chunk *>(malloc(blockSize));
            Chunk *chunk = blockBegin;

            for (int i = 0; i < chunksPerBlock - 1; ++i)
            {
                chunk->next = reinterpret_cast<Chunk *>(reinterpret_cast<char *>(chunk) + chunkSize);
                chunk = chunk->next;
            }

            chunk->next = nullptr;

            return blockBegin;
        }

    public:
        PoolAllocator(size_t chunksPerBlock = 20) : chunksPerBlock(chunksPerBlock) {}

        void* allocate(std::size_t size)
        {
            if (pAlloc == nullptr)
            {
                pAlloc = allocateBlock(size);
            }

            Chunk *freeChunk = pAlloc;
            pAlloc = pAlloc->next;

            return freeChunk;
        }

        void deallocate(void *chunk, std::size_t size) noexcept
        {
            reinterpret_cast<Chunk*>(chunk)->next = pAlloc;
            pAlloc = reinterpret_cast<Chunk*>(chunk);
        }
    };
}