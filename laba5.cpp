#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <string>

// Класс исключений CustomException
// Используется для генерации пользовательских исключений с сообщением.
class CustomException : public std::exception {
private:
    std::string message; // Сообщение исключения

public:
    // Конструктор принимает строку-сообщение
    explicit CustomException(const std::string& msg) : message(msg) {}

    // Метод what() возвращает сообщение исключения
    const char* what() const noexcept override {
        return message.c_str();
    }
};

namespace containers {

    // Класс Queue - реализация очереди
    template <typename T>
    class Queue {
    private:
        // Узел очереди, содержащий значение и указатель на следующий узел
        struct Node {
            T value;    // Значение узла
            Node* next; // Указатель на следующий узел

            // Конструкторы узла
            Node(const T& val) : value(val), next(nullptr) {}
            Node(T&& val) : value(std::move(val)), next(nullptr) {}

            // Объявляем класс Queue другом для доступа к private-членам
            friend class Queue;
        };

        Node* front; // Указатель на первый элемент очереди
        Node* back;  // Указатель на последний элемент очереди
        size_t count; // Количество элементов в очереди

    public:
        // Конструктор по умолчанию
        Queue() : front(nullptr), back(nullptr), count(0) {}

        // Конструктор с инициализатором списка
        Queue(std::initializer_list<T> init_list) : Queue() {
            for (const auto& elem : init_list) {
                push(elem); // Добавляем элементы в очередь
            }
        }

        // Конструктор копирования
        Queue(const Queue& other) : Queue() {
            Node* current = other.front;
            while (current) {
                push(current->value); // Копируем элементы очереди
                current = current->next;
            }
        }

        // Конструктор перемещения
        Queue(Queue&& other) noexcept
            : front(other.front), back(other.back), count(other.count) {
            // Обнуляем элементы у исходного объекта
            other.front = other.back = nullptr;
            other.count = 0;
        }

        // Оператор присваивания копированием
        Queue& operator=(const Queue& other) {
            if (this != &other) { // Защита от самоприсваивания
                clear(); // Очищаем текущую очередь
                Node* current = other.front;
                while (current) {
                    push(current->value); // Копируем элементы
                    current = current->next;
                }
            }
            return *this;
        }

        // Оператор присваивания перемещением
        Queue& operator=(Queue&& other) noexcept {
            if (this != &other) {
                clear(); // Очищаем текущую очередь
                front = other.front;
                back = other.back;
                count = other.count;
                // Обнуляем исходный объект
                other.front = other.back = nullptr;
                other.count = 0;
            }
            return *this;
        }

        // Деструктор
        ~Queue() {
            clear(); // Удаляем все элементы
        }

        // Добавление элемента в конец очереди
        void push(const T& value) {
            Node* newNode = new Node(value); // Создаем новый узел
            if (empty()) {
                front = back = newNode; // Если очередь пуста, новый узел становится первым и последним
            } else {
                back->next = newNode; // Присоединяем новый узел к концу
                back = newNode;       // Обновляем указатель на последний элемент
            }
            ++count; // Увеличиваем счетчик элементов
        }

        // Удаление элемента из начала очереди
        void pop() {
            if (empty()) {
                throw CustomException("Queue is empty"); // Исключение, если очередь пуста
            }
            Node* oldFront = front; // Сохраняем указатель на текущий первый элемент
            front = front->next;   // Смещаем указатель на следующий элемент
            delete oldFront;       // Удаляем старый первый элемент
            --count; // Уменьшаем счетчик элементов
            if (empty()) {
                back = nullptr; // Если очередь пуста, обнуляем указатель на последний элемент
            }
        }

        // Доступ к элементу по индексу (без удаления)
        T& operator[](size_t index) {
            if (index >= count) {
                throw CustomException("Index out of range"); // Исключение, если индекс вне диапазона
            }
            Node* current = front;
            for (size_t i = 0; i < index; ++i) {
                current = current->next; // Переход к нужному элементу
            }
            return current->value;
        }

        // Возвращает количество элементов в очереди
        size_t size() const {
            return count;
        }

        // Проверяет, пуста ли очередь
        bool empty() const {
            return count == 0;
        }

        // Удаляет все элементы из очереди
        void clear() {
            while (!empty()) {
                pop(); // Удаляем элементы по одному
            }
        }

        // Отображение всех элементов очереди
        void display() const {
            Node* current = front;
            while (current) {
                std::cout << current->value << " ";
                current = current->next;
            }
            std::cout << std::endl;
        }

        // Интерфейс для итераторов
        class Iterator {
        public:
            virtual ~Iterator() = default;
            virtual bool hasNext() = 0; // Проверка наличия следующего элемента
            virtual T& next() = 0;      // Возвращает следующий элемент
        };

        // Итератор для прямого обхода
        class ForwardIterator : public Iterator {
        private:
            Node* current;

        public:
            explicit ForwardIterator(Node* start) : current(start) {}

            bool hasNext() override {
                return current != nullptr; // Проверяем, есть ли текущий элемент
            }

            T& next() override {
                if (!current) {
                    throw CustomException("No more elements in forward iterator");
                }
                T& value = current->value; // Сохраняем значение текущего элемента
                current = current->next;  // Переходим к следующему элементу
                return value;
            }
        };

        // Итератор для обратного обхода
        class ReverseIterator : public Iterator {
        private:
            Node** nodes; // Массив указателей на узлы
            int index;    // Текущий индекс для обратного обхода

        public:
            ReverseIterator(Node* front, size_t count) : nodes(new Node*[count]), index(count - 1) {
                Node* current = front;
                for (size_t i = 0; i < count; ++i) {
                    nodes[i] = current; // Заполняем массив узлов
                    current = current->next;
                }
            }

            ~ReverseIterator() {
                delete[] nodes; // Освобождаем память
            }

            bool hasNext() override {
                return index >= 0; // Проверяем, есть ли элементы для обратного обхода
            }

            T& next() override {
                if (index < 0) {
                    throw CustomException("No more elements in reverse iterator");
                }
                return nodes[index--]->value; // Возвращаем значение и уменьшаем индекс
            }
        };

        // Получение итератора для прямого обхода
        ForwardIterator getForwardIterator() {
            return ForwardIterator(front);
        }

        // Получение итератора для обратного обхода
        ReverseIterator getReverseIterator() {
            return ReverseIterator(front, count);
        }
    };
} // namespace containers

int main() {
    using namespace containers;

    // Создаем очередь с элементами
    Queue<int> queue{1, 2, 3, 4, 5};

    // Прямой обход
    auto forwardIterator = queue.getForwardIterator();
    std::cout << "Forward iteration: ";
    while (forwardIterator.hasNext()) {
        std::cout << forwardIterator.next() << " ";
    }
    std::cout << std::endl;

    // Обратный обход
    auto reverseIterator = queue.getReverseIterator();
    std::cout << "Reverse iteration: ";
    while (reverseIterator.hasNext()) {
        std::cout << reverseIterator.next() << " ";
    }
    std::cout << std::endl;

    return 0;
}
