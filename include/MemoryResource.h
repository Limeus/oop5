#ifndef MEMORYRESOURCE_H
#define MEMORYRESOURCE_H

#include <memory_resource>
#include <vector>

class MemoryResource : public std::pmr::memory_resource {
public:
    MemoryResource(size_t blockSize, size_t blockCount);

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
    size_t blockSize;
    size_t blockCount;
    std::vector<char> memory;
    std::vector<void*> freeBlocks;

};

#endif // MEMORYRESOURCE_H