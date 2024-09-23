// Вариант 1
// Требуется написать программу которая выполняет следующие действия.

// Получить от пользователя значение N.
// Выделить память под 3 двумерных массива N * N. Тип массива double. Для удобства будем называть их A,B,C, в программе они могут называться любым образом.
// Заполнить массивы случайными числами в диапазоне от 0 до 10 (включительно).
// Предложить пользователю возможность выбора операции с массивом:
// Сложение матрицы A и B, с записью результата в C.
// Умножение матрицы A и B, с записью результата в C.
// Транспонирование матрицы A или B(на выбор пользователя), с записью результата в C.
// Поиск определителя матрицы.

#include<iostream>
#include <cstdlib>
#include <time.h>
#include <cmath>

using namespace std;

const int MIN = 0;//минимум диапазона
const int MAX = 10;//мксимум диапазона

/** Функция для выделения памяти под двумерный массив N * N
*
*   @param N размерность массива
*   @return возврашает двумерный массив N * N
*/
double** createArray(int N) {
    double** array = new double*[N];
    for (int i = 0; i < N; ++i) {
        array[i] = new double[N];
    }
    return array;
}

/**  Функция заполнение массива рандомными числами
*
*   @param range_min минимально возможное число
*   @param range_max максимально возможное число
*   @param array массив, который нужно заполнить
*   @param N размерность массива
*/
void fillArray(int range_min, int range_max, double** array, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            array[i][j] = round(range_min + (range_max - range_min) * rand() / (double)RAND_MAX * 100.0) / 100.0; //генерируем рандомное вещественное число
        }
    }
}

/**  Функция для вывода массива
*
*   @param array массив, который нужно вывести
*   @param N размерность массива
*/
void printArray(double** array, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << array[i][j] << "   " ;
        }
    cout << endl;
    }
}

/**  Функция для освобождения памяти двумерного массива
*
*   @param array массив, который нужно удалить
*   @param N размерность массива
*/
void deleteArray(double** array, int N) {
    for (int i = 0; i < N; ++i) {
        delete[] array[i];
    }
    delete[] array;
}

/**  Функция для сложения матриц
*
*   @param A первое слогаемое
*   @param B второе слогаемое
*   @param C сумма
*   @param N размерность массивов
*/
void addMatrices(double** A, double** B, double** C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

/**  Функция для умножения матриц
*
*   @param A первый множитель
*   @param B второй множитель
*   @param C произведение
*   @param N размерность массивов
*/
void multiplyMatrices(double** A, double** B, double** C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/**  Функция для транспонирования матрицы
*
*   @param array матрица,которую нужно транспонировать
*   @param С транспонированная матрица array
*   @param N размерность массивов
*/
void transposeMatrix(double** array, double** C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[j][i] = array[i][j];
        }
    }
}

/** Функция для получения подматрици без i-й строки и j-го столбца.
 *
 * @param mas Указатель на исходную матрицу.
 * @param p Указатель на подматрицу, которую нужно заполнить.
 * @param i Индекс строки, которую нужно исключить.
 * @param j Индекс столбца, который нужно исключить.
 * @param m Размерность исходной матрицы.
 */
void GetMatr(double** mas, double** p, int i, int j, int m) { 
    int ki, kj, di, dj; 
    di = 0; 
    for (ki = 0; ki < m - 1; ki++) { // проверка индекса строки 
        if (ki == i)  
            di = 1; 
        dj = 0; 
        for (kj = 0; kj < m - 1; kj++) { // проверка индекса столбца 
            if (kj == j)  
                dj = 1; 
            p[ki][kj] = mas[ki + di][kj + dj]; 
        } 
    } 
} 

/** Функция для вычисления определителя матрицы.
 *
 * @param mas Указатель на исходную матрицу.
 * @param m Размерность матрицы.
 * @return Определитель матрицы.
 */
double determinant(double** mas, int m) {
    int i, j, k, n; 
    double d;
    double** p = createArray(m);
    k = 1; 
    n = m - 1; 

    if (m == 1) { 
        d = mas[0][0]; 
        deleteArray(p, m);
        return d; 
    } 

    if (m == 2) { 
        d = mas[0][0] * mas[1][1] - (mas[1][0] * mas[0][1]); 
        deleteArray(p, m);
        return d; 
    } 

    if (m > 2) { 
        for (i = 0; i < m; i++) { 
            GetMatr(mas, p, i, 0, m); 
            d += k * mas[i][0] * determinant(p, n); 
            k = -k; 
        } 
    } 
    
    deleteArray(p, m);
    
    return d; 
}



int main(){

    int N;
    cout << "Enter the value N:" << endl ;
    cin >> N;

    // Выделение памяти под 3 двумерных массива N x N
    double** A = createArray(N);
    double** B = createArray(N);
    double** C = createArray(N);

    // Заполнение массива случейными числами
    srand((time(NULL)));

    fillArray(MIN, MAX, A, N);
    cout << "Array A:" << endl ;
    printArray(A, N);
    fillArray(MIN, MAX, B, N);
    cout << "Array B:" << endl ;
    printArray(B, N);
    fillArray(MIN, MAX, C, N);
    cout << "Array C:" << endl ;
    printArray(C, N);


    int choice;
    do {
        cout << "\nSelect operation:\n";
        cout << "1. Matrix addition (A + B)\n";
        cout << "2. Matrix multiplication(A * B)\n";
        cout << "3. Matrix Transpose A\n";
        cout << "4. Matrix Transpose B\n";
        cout << "5. Finding the determinant of a matrix A\n";
        cout << "6. Finding the determinant of a matrix B\n";
        cout << "7. Finding the determinant of a matrix C\n";
        cout << "0. Exit\n";

        cin >> choice;

        switch (choice) {
            case 1:
                addMatrices(A, B, C, N);
                cout << "A + B = C. \n Array C:" << endl;
                printArray(C, N);
                break;
            case 2:
                multiplyMatrices(A, B, C, N);
                cout << "A * B = C. \n Array C:" << endl;
                printArray(C, N);
                break;
            case 3:
                transposeMatrix(A, C, N);
                cout << "Matrix A is transposed and written to C.\n Array C:" << endl;
                printArray(C, N);
                break;
            case 4:
                transposeMatrix(B, C, N);
                cout << "Matrix B is transposed and written to C.\n Array C:" << endl;
                printArray(C, N);
                break;
            case 5:
                cout << "Determinant of matrix A: " << determinant(A, N) << endl;
                break;
            case 6:
                cout << "Determinant of matrix B: " << determinant(B, N) << endl;
                break;
            case 7:
                cout << "Determinant of matrix C: " << determinant(C, N) << endl;
                break;
            case 0:
                cout << "Bye.\n";
                break;
            default:
                cout << "Error. Please try again.\n";
        }
    } while (choice != 0);

    // Освобождение памяти
    deleteArray(A, N);
    deleteArray(B, N);
    deleteArray(C, N);

    return 0;
}