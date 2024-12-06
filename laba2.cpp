> Roma:
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <string>

namespace containers {

    template <typename T>
    class Queue {
    private:
        // Вложенная структура Node
        struct Node {
            T value;
            Node* next;

            Node(const T& val) : value(val), next(nullptr) {}  // Приватный конструктор
            Node(T&& val) : value(std::move(val)), next(nullptr) {} // Для перемещения

            friend class Queue;  // Queue может создавать узлы
        };

        Node* front;  // Указатель на первый элемент
        Node* back;   // Указатель на последний элемент
        size_t count; // Количество элементов в очереди

    public:
        // Конструктор по умолчанию
        Queue() : front(nullptr), back(nullptr), count(0) {}

        // Конструктор от std::initializer_list<T>
        Queue(std::initializer_list<T> init_list) : Queue() {
            for (const auto& elem : init_list) {
                push(elem);
            }
        }

        // Конструктор копирования
        Queue(const Queue& other) : Queue() {
            Node* current = other.front;
            while (current) {
                push(current->value);
                current = current->next;
            }
        }

        // Конструктор перемещения
        Queue(Queue&& other) noexcept 
            : front(other.front), back(other.back), count(other.count) {
            other.front = other.back = nullptr;
            other.count = 0;
        }

        // Оператор присваивания копированием
        Queue& operator=(const Queue& other) {
            if (this != &other) {
                clear();
                Node* current = other.front;
                while (current) {
                    push(current->value);
                    current = current->next;
                }
            }
            return *this;
        }

        // Оператор присваивания перемещением
        Queue& operator=(Queue&& other) noexcept {
            if (this != &other) {
                clear();
                front = other.front;
                back = other.back;
                count = other.count;
                other.front = other.back = nullptr;
                other.count = 0;
            }
            return *this;
        }

        // Деструктор
        ~Queue() {
            clear();
        }

        // Добавление элемента в конец очереди
        void push(const T& value) {
            Node* newNode = new Node(value);
            if (empty()) {
                front = back = newNode;
            } else {
                back->next = newNode;
                back = newNode;
            }
            ++count;
        }

        // Удаление элемента из начала очереди
        void pop() {
            if (empty()) {
                throw std::out_of_range("Queue is empty");
            }
            Node* oldFront = front;
            front = front->next;
            delete oldFront;
            --count;
            if (empty()) {
                back = nullptr;
            }
        }

        // Доступ к элементу по индексу (возвращает ссылку)
        T& operator[](size_t index) {
            if (index >= count) {
                throw std::out_of_range("Index out of range");
            }
            Node* current = front;
            for (size_t i = 0; i < index; ++i) {
                current = current->next;
            }
            return current->value;
        }

        // Размер очереди
        size_t size() const {
            return count;
        }

        // Проверка на пустоту
        bool empty() const {
            return count == 0;
        }

        // Очистка очереди
        void clear() {
            while (!empty()) {
                pop();
            }
        }

        // Вывод всех элементов (для тестирования)
        void display() const {
            Node* current = front;
            while (current) {
                std::cout << current->value << " ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    };

> Roma:
} // namespace containers

// Тестирование в main()
int main() {
    using namespace containers;

    // Очередь с int
    Queue<int> intQueue{1, 2, 3, 4};
    intQueue.push(5);
    intQueue.display();

    // Очередь с double
    Queue<double> doubleQueue{1.1, 2.2, 3.3};
    doubleQueue.push(4.4);
    doubleQueue.display();

    // Очередь с std::string
    Queue<std::string> stringQueue{"hello", "world"};
    stringQueue.push("!");
    stringQueue.display();

    return 0;
}
