#ifndef MEMORYRESOURCE_H
#define MEMORYRESOURCE_H

#include <memory_resource>
#include <vector>
#include <stdexcept>
#include <cstddef> // for std::align
#include <string>

class MemoryResource : public std::pmr::memory_resource {
public:
    // Конструктор
    MemoryResource(size_t blockSize, size_t blockCount);

    // Деструктор
    ~MemoryResource() = default;

protected:
    // Переопределенные методы базового класса
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
    size_t blockSize;               // Размер одного блока памяти
    size_t blockCount;              // Количество блоков памяти
    std::vector<char> memory;       // Основное хранилище памяти
    std::vector<void*> freeBlocks;  // Указатели на свободные блоки

    // Проверка выравнивания
    bool is_aligned(void* ptr, size_t alignment) const noexcept;
};

#endif // MEMORYRESOURCE_H
