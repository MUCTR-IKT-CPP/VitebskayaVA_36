#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <initializer_list>

template <typename T>
class SortedVector : public std::vector<T> {
public:
    /**
     * @brief Конструктор, принимающий список инициализации и сортирующий вектор.
     * @param initList Список инициализации для создания отсортированного вектора.
     */
    SortedVector(std::initializer_list<T> initList) : std::vector<T>(initList) {
        std::sort(this->begin(), this->end());
    }

    /**
     * @brief Подсчитывает количество элементов, делящихся нацело на заданное число.
     * @param divisor Делитель.
     * @return Количество элементов, делящихся нацело на divisor.
     */
    int countDivisibleBy(T divisor) {
        return std::count_if(this->begin(), this->end(), [divisor](T& elem) {
            return elem % divisor == 0;
        });
    }

    /**
     * @brief Перегрузка оператора вывода для SortedVector.
     * @param os Поток вывода.
     * @param vec Вектор, который нужно вывести.
     * @return Поток вывода с элементами вектора.
     */
    friend std::ostream& operator<<(std::ostream& os, const SortedVector<T>& vec) {
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(os, " "));
        return os;
    }

    /**
     * @brief Вставляет элемент в первую подходящую позицию, чтобы сохранить сортировку.
     * @param value Значение, которое нужно вставить.
     */
    void insert(T value) {
        auto pos = std::upper_bound(this->begin(), this->end(), value);
        std::vector<T>::insert(pos, value);
    }

    /**
     * @brief Выводит все перестановки элементов вектора без повторений.
     */
    void printPermutations() {
        std::vector<T> vecCopy = *this; 
        do {
            std::copy(vecCopy.begin(), vecCopy.end(), std::ostream_iterator<T>(std::cout, " "));
            std::cout << std::endl;
        } while (std::next_permutation(vecCopy.begin(), vecCopy.end()));
    }
};

int main() {
    // Пример использования конструктора с initializer_list
    SortedVector<int> vec = {20, 10, 30};

    // Выводим вектор
    std::cout << "Sorted vector: " << vec << std::endl;

    // Подсчитываем элементы, делящиеся нацело на 2
    double del;
    std::cout << "Enter value:" ;
    std::cin >> del;
    int count = vec.countDivisibleBy(del);
    std::cout << "Number of element that are divisible by " << del << ": " << count << std::endl;

    // Вставляем элемент в правильную позицию
    double new_el;
    std::cout << "Enter value:" ;
    std::cin >> new_el;
    vec.insert(new_el);
    std::cout << "After insertion " << new_el << ": \n" << vec << std::endl;

    // Выводим все перестановки
    std::cout << "Permutations without repetition:" << std::endl;
    vec.printPermutations();

    return 0;
}
