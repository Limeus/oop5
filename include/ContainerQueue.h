#ifndef CONTAINERQUEUE_H
#define CONTAINERQUEUE_H

#include <memory_resource>
#include <variant>
#include <iostream>
#include <stdexcept>
#include <iterator>

#include "ComplexType.h"

using QueueElement = std::variant<int, ComplexType>;

template<typename T>
class ContainerQueue {
public:
    using allocator_type = std::pmr::polymorphic_allocator<T>;

    ContainerQueue(allocator_type allocator = {})
            : alloc(allocator), data(nullptr), head(0), tail(0), size(0), capacity(8) {
        data = alloc.allocate(capacity);
    }

    ~ContainerQueue() {
        clear();
        alloc.deallocate(data, capacity);
    }

    void add(const T& value) {
        if (size == capacity) {
            resize(capacity * 2);
        }
        std::allocator_traits<allocator_type>::construct(alloc, data + tail, value);
        tail = (tail + 1) % capacity;
        ++size;
    }

    void remove() {
        if (size == 0) {
            throw std::out_of_range("Очередь пуста");
        }
        std::allocator_traits<allocator_type>::destroy(alloc, data + head);
        head = (head + 1) % capacity;
        --size;
    }

    T& front() {
        if (size == 0) {
            throw std::out_of_range("Очередь пуста");
        }
        return data[head];
    }

    bool isEmpty() const {
        return size == 0;
    }

    size_t getSize() const {
        return size;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        Iterator(pointer ptr, size_t idx, size_t cap)
                : ptr(ptr), index(idx), capacity(cap) {}

        reference operator*() const { return ptr[index]; }
        pointer operator->() { return &ptr[index]; }

        Iterator& operator++() {
            index = (index + 1) % capacity;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.ptr == b.ptr && a.index == b.index; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return !(a == b); }

    private:
        pointer ptr;
        size_t index;
        size_t capacity;
    };

    Iterator begin() { return Iterator(data, head, capacity); }
    Iterator end() { return Iterator(data, tail, capacity); }

private:
    void resize(size_t newCapacity) {
        T* newData = alloc.allocate(newCapacity);
        for (size_t i = 0; i < size; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc, newData + i, std::move(data[(head + i) % capacity]));
            std::allocator_traits<allocator_type>::destroy(alloc, data + (head + i) % capacity);
        }
        alloc.deallocate(data, capacity);
        data = newData;
        capacity = newCapacity;
        head = 0;
        tail = size;
    }

    void clear() {
        while (!isEmpty()) {
            remove();
        }
    }

    allocator_type alloc;
    T* data;
    size_t head;
    size_t tail;
    size_t size;
    size_t capacity;
};

#endif // CONTAINERQUEUE_H