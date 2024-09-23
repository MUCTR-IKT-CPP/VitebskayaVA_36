//  Вариант 1
// Требуется написать программу которая будет генерировать массив состоящий из 10000 элементов. 
// Массив содержит числа с плавающей запятой двойной точности. 
// Требуется отсортировать массив при помощи алгоритма расческой. 
// По итогу, нужно проверить сортировку массива и вывести ее результат.

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cmath>

using namespace std;

const int ARR_SIZE = 10000;//размер массива
const int LIMIT = 100;//диапазон


/*  Функция заполнение массива рандомными числами с плавающей запятой двойной точности
*
*   @param range_min минимально возможное число
*   @param range_max максимально возможное число
*   @return возврашает рандомное числами с заданной точностью
*/
double rrand(int range_min, int range_max) {
    double value = range_min + (range_max - range_min) * rand() / (double)RAND_MAX; //генерируем рандомное вещественное число
    return round(value * 100.0) / 100.0; // записываем до двойной точности
}


/*  Функция для реализации алгоритма расчесткой
*
*    @param arr[] неотсортированный массив
*/
void combSort(double arr[]) {
    const double factor = 1.2;
    int len = ARR_SIZE;
    int step = len / factor; 
   
    while (step >= 1) {
        for (int i = 0; i + step < len; ++i) {
            if (arr[i] > arr[i + step]) {
                swap(arr[i], arr[i + step]);
            }
        }
    step = step / factor;
    }
}

/*  Функция для проверки отсортированности массива
*
*   @param arr[] массив для проверки
*   @return возврашает true, если массив отсортирован и false, если не отсортирован
*/
bool isSorted(double arr[]) {
    int k = 0;
    for (int i = 1; i < ARR_SIZE; ++i) {
        k+=1;
        if (arr[i - 1] > arr[i]) {
            cout << k;
            return false;
        }
    }
    return true;
}

/*  Функция для вывода массива
*
*    @param arr[] массив, который нужно вывести
*/
void arrayOutput(double arr[]){
    for (int i = 0; i < ARR_SIZE; i++) {
        cout << arr[i] << ", " ;
    }
}


int main() {

    double ary[ARR_SIZE];

    srand(time(NULL));

    // инициализация массива случайными значениями из диапазона [-ABSLIMIT;ABSLIMIT]
    for (int i = 0; i < ARR_SIZE; i++) {
        ary[i] = rrand(-LIMIT, LIMIT);
    }
    cout << "Initial array:" << endl;
    arrayOutput(ary);

    // сортировка массива
    combSort(ary);
    
    // проверка сортировки и вывод отсортированного массива
    if (isSorted(ary)) {
        cout << endl << endl << "The array has been successfully sorted!)" << endl;
        arrayOutput(ary);
    } else {
        cout << endl << endl << "Error: The array is not sorted.(" << endl;
    }

    return 0;
}
