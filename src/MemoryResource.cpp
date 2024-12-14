#include "../include/MemoryResource.h"
#include <vector>
#include <stdexcept>
#include <cstddef> // for std::align
#include <string>

MemoryResource::MemoryResource(size_t blockSize, size_t blockCount)
        : blockSize(blockSize), blockCount(blockCount), memory(blockSize * blockCount), freeBlocks(blockCount) {
    for (size_t i = 0; i < blockCount; ++i) {
        freeBlocks[i] = memory.data() + i * blockSize;
    }
}

void* MemoryResource::do_allocate(size_t bytes, size_t alignment) {
    if (bytes > blockSize) {
        throw std::invalid_argument("Запрошенный размер (" + std::to_string(bytes) +
                                    ") больше размера блока (" + std::to_string(blockSize) + ")");
    }

    if (freeBlocks.empty()) {
        throw std::bad_alloc();
    }

    // Выделение блока
    void* ptr = freeBlocks.back();
    freeBlocks.pop_back();

    // Проверяем, соответствует ли указатель выравниванию
    void* alignedPtr = std::align(alignment, bytes, ptr, blockSize);
    if (!alignedPtr) {
        throw std::bad_alloc(); // Не удалось выделить с нужным выравниванием
    }

    return alignedPtr;
}

void MemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    if (!is_aligned(p, alignment)) {
        throw std::invalid_argument("Указатель не соответствует выравниванию");
    }

    freeBlocks.push_back(static_cast<char*>(p));
}

bool MemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

bool MemoryResource::is_aligned(void* ptr, size_t alignment) const noexcept {
    return reinterpret_cast<uintptr_t>(ptr) % alignment == 0;
}
