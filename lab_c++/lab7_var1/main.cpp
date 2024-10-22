#include <iostream>


template<typename T>
class Queue {
public:
    struct Element {
        T data;  // Данные элемента.
        Element* next;  // Указатель на следующий элемент.

        Element(const T& value) : data(value), next(nullptr) {}
    };

    Element* start_element;  // Указатель на начало очереди.
    Element* end_element;    // Указатель на конец очереди.
    int size;  // Текущий размер очереди.

    Queue() : start_element(nullptr), end_element(nullptr), size(0) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    /**
     * @brief Добавление элемента в конец очереди.
     * @param value Значение, которое нужно добавить в очередь.
     */
    void enqueue(const T& value) {
        Element* newElement = new Element(value);
        if (isEmpty()) {
            start_element = end_element = newElement;
        } else {
            end_element->next = newElement;
            end_element = newElement;
        }
        ++size;
    }

    /**
     * @brief Удаление элемента из начала очереди.
     * 
     * Если очередь пуста, выводится сообщение об ошибке.
     */
    void dequeue() {
        if (isEmpty()) {
            std::cerr << "Queue is empty!\n";
        }else{
            Element* temp = start_element;
            start_element = start_element->next;
            delete temp;
            --size;
            if (isEmpty()) {
                end_element = nullptr;
            }  
        }
        
    }

    /**
     * @brief Получение элемента из начала очереди без его удаления.
     * @return Указатель на данные элемента в начале очереди.
     * Возвращает nullptr, если очередь пуста.
     */
    T* peek() const {
        if (isEmpty()) {
            std::cerr << "Queue is empty!\n";
            return nullptr;
        }
        return &start_element->data;
    }

    /**
     * @brief Вывод всех элементов очереди.
     * 
     * Если очередь пуста, выводится соответствующее сообщение.
     */
    void printQueue() const {
        if (isEmpty()) {
            std::cout << "Queue is empty!" << std::endl;
            return;
        }

        std::cout << "Queue elements: ";

        // как оно работает
        // for (Queue<int>::Iterator i = q.begin(); i != q.end(); ++i) {
        //     int el = *i;  // Получаем значение текущего элемента
        //     std::cout << el << " ";
        // }

        for (auto el : *this) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }

    /**
     * @brief Проверка, пуста ли очередь.
     * @return true, если очередь пуста, иначе false.
     */
    bool isEmpty() const {
        return start_element == nullptr;
    }

    /**
     * @brief Получение текущего размера очереди.
     * @return Количество элементов в очереди.
     */
    int getSize() const {
        return size;
    }

    /**
     * @brief Класс итератора для обхода элементов очереди.
     */
    class Iterator {
    public:
        Element* current;  ///< Указатель на текущий элемент.

        Iterator(Element* start_element) : current(start_element) {}

        // объяснение умных действий:
        // Это перегрузка оператора, простыми словами его переопределение
        // то есть теперь эти операторы для данного класса работают не как нормальные, а как тут написано
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        Iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        T& operator*() const {
            return current->data;
        }
    };

    /**
     * @brief Получение итератора, указывающего на первый элемент очереди.
     * @return Итератор начала очереди.
     */
    Iterator begin() const {
        return Iterator(start_element);
    }

    /**
     * @brief Получение итератора, указывающего на конец очереди (nullptr).
     * @return Итератор конца очереди.
     */
    Iterator end() const {
        return Iterator(nullptr);
    }
};

int main() {
    Queue<int> q;

    int choice;
    do {
        std::cout << "\nSelect operation:\n";
        std::cout << "1. Get queue size\n";
        std::cout << "2. Add element to end\n";
        std::cout << "3. Get element from start\n";
        std::cout << "4. Remove element from beginning\n";
        std::cout << "0. Exit\n";

        std::cin >> choice;

        switch (choice) {
            case 1:
                q.printQueue();
                std::cout << "Queue size: " << q.size << "\n";
                break;
            case 2:
                q.printQueue();
                std::cout << "Enter value: ";
                int value;
                std::cin >> value;
                q.enqueue(value);
                std::cout << std::endl;
                q.printQueue();
                break;
            case 3:
                q.printQueue();
                std::cout << "Element from start: " << *q.peek() << std::endl;
                break;
            case 4:
            
                if (q.peek() == nullptr){
                    break;
                }
                q.printQueue();
                std::cout << "Element from start: " << *q.peek() << std::endl;
                std::cout << "Remove element...\n";
                q.dequeue();
                std::cout << "Now: ";
                q.printQueue();
                break;
            case 0:
                std::cout << "Bye.\n";
                break;
            default:
                std::cout << "Error. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
