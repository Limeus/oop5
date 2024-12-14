#include "include/MemoryResource.h"
#include "include/ContainerQueue.h"
#include "include/ComplexType.h"

#include <locale>
#include <iostream>
#include <string>


void printQueue(ContainerQueue<QueueElement>& queue) {
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        std::visit([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
            std::cout << arg << " ";
        } else if constexpr (std::is_same_v<T, ComplexType>) {
            std::cout << "{" << arg.a << ", " << arg.b << ", " << arg.c << "} ";
        }
        }, *it);
    }
    std::cout << std::endl;
}

int main() {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    MemoryResource memoryResource(sizeof(QueueElement) * 100, 100);

    std::pmr::polymorphic_allocator<QueueElement> alloc(&memoryResource);
    ContainerQueue<QueueElement> mixedQueue(&memoryResource);

    std::string command;
    while (true) {
        std::cout << "Введите команду (add_int, add_complex, remove, print, size, exit): ";
        std::cin >> command;

        if (command == "add_int") {
            int value;
            std::cout << "Введите целое число: ";
            std::cin >> value;
            mixedQueue.add(value);
        } else if (command == "add_complex") {
            int a;
            double b;
            std::string c;
            std::cout << "Введите значения <int>, <double> и <string> для ComplexType: ";
            std::cin >> a >> b >> c;
            mixedQueue.add(ComplexType(a, b, c));
        } else if (command == "remove") {
            try {
                mixedQueue.remove();
                std::cout << "Элемент удален из очереди.\n";
            } catch (const std::out_of_range& e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command == "print") {
            printQueue(mixedQueue);
        } else if (command == "size") {
            std::cout << "Размер очереди: " << mixedQueue.getSize() << std::endl;
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Неизвестная команда. Попробуйте снова.\n";
        }
    }

    return 0;
}