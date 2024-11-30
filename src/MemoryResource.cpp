#include "../include/MemoryResource.h"
#include <stdexcept>

MemoryResource::MemoryResource(size_t blockSize, size_t blockCount)
        : blockSize(blockSize), blockCount(blockCount), memory(blockSize * blockCount), freeBlocks(blockCount) {
    for (size_t i = 0; i < blockCount; ++i) {
        freeBlocks[i] = memory.data() + i * blockSize;
    }
}

void* MemoryResource::do_allocate(size_t bytes, size_t alignment) {
    if (bytes > blockSize) {
        throw std::invalid_argument("Запрошенный размер больше размера блока");
    }

    if (freeBlocks.empty()) {
        throw std::bad_alloc();
    }

    void* ptr = freeBlocks.back();
    freeBlocks.pop_back();
    return ptr;
}

void MemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    freeBlocks.push_back(static_cast<char*>(p));
}

bool MemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}