#include "gtest/gtest.h"
#include "../include/MemoryResource.h"
#include "../include/ContainerQueue.h"
#include "../include/ComplexType.h"

TEST(MemoryResource, AllocateAndDeallocate) {
    FixedBlockMemoryResource memoryResource{sizeof(QueueElement) * 100, 100};
    std::pmr::polymorphic_allocator<QueueElement> alloc(&memoryResource);

    QueueElement *element = alloc.allocate(1);
    EXPECT_NO_THROW(alloc.deallocate(element, 1));
}

TEST(MemoryResource, ExhaustMemory) {
    FixedBlockMemoryResource memoryResource{sizeof(QueueElement) * 100, 100};
    std::pmr::polymorphic_allocator<QueueElement> alloc(&memoryResource);
    std::vector<QueueElement *> allocatedBlocks;

    for (int i = 0; i < 100; ++i) {
        allocatedBlocks.push_back(alloc.allocate(1));
    }

    EXPECT_THROW({
                     auto ptr = alloc.allocate(1);
                     alloc.deallocate(ptr, 1);
                 }, std::bad_alloc);

    for (auto ptr: allocatedBlocks) {
        alloc.deallocate(ptr, 1);
    }
}

TEST(ContainerQueue, AddAndRemove) {
    FixedBlockMemoryResource memoryResource{sizeof(QueueElement) * 100, 100};
    ContainerQueue<QueueElement> queue{&memoryResource};

    QueueElement element = 42;

    queue.add(element);
    QueueElement frontElement = queue.front();

    EXPECT_EQ(std::get<int>(frontElement), 42);

    queue.remove();

    EXPECT_TRUE(queue.isEmpty());
}

TEST(ContainerQueue, AddComplexType) {
    FixedBlockMemoryResource memoryResource{sizeof(QueueElement) * 100, 100};
    ContainerQueue<QueueElement> queue{&memoryResource};

    ComplexType complex(1, 2.5, "test");

    queue.add(complex);
    QueueElement frontElement = queue.front();

    ComplexType retrieved = std::get<ComplexType>(frontElement);
    EXPECT_EQ(retrieved.a, 1);
    EXPECT_EQ(retrieved.b, 2.5);
    EXPECT_EQ(retrieved.c, "test");

    queue.remove();

    EXPECT_TRUE(queue.isEmpty());
}

TEST(ContainerQueue, IteratorTest) {
    FixedBlockMemoryResource memoryResource{sizeof(QueueElement) * 100, 100};
    ContainerQueue<QueueElement> queue{&memoryResource};

    queue.add(10);
    queue.add(ComplexType(1, 2.5, "test"));

    auto it = queue.begin();
    int firstValue = std::get<int>(*it);
    ++it;
    ComplexType secondValue = std::get<ComplexType>(*it);

    EXPECT_EQ(firstValue, 10);
    EXPECT_EQ(secondValue.a, 1);
    EXPECT_EQ(secondValue.b, 2.5);
    EXPECT_EQ(secondValue.c, "test");
}

TEST(ContainerQueue, RemoveFromEmptyQueue) {
    FixedBlockMemoryResource memoryResource{sizeof(QueueElement) * 100, 100};
    ContainerQueue<QueueElement> queue{&memoryResource};

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_THROW(queue.remove(), std::out_of_range);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}