// Вариант 2
// Требуется написать программу, в которой:
// Будет реализована структура "Банкнота" с полями:
//      Наименование
//      Номинал (из списка заданного перечислением 1, 5, 10, 20, 50, 100, 1000)
//      Курс к рублю
// Пользователь вводит число N.
// Создан массив хранящий N банкнот, который будем называть кошельком, кол-во считывается с клавиатуры.
// Сгенерировано N банкнот различного наминала и валюты.
// Написаны функции которые:
//   Подсчитывают общую сумму в рублях хранимую в кошельке.
//   Формирует срез банкнот с номиналом больше выбранного пользователем и выводит их количество разбивая их по валютам.
//   Выводит состав кошелька отсортированный по валютам (по алфавиту) и номиналу с количеством каждой существующей банкноты.
//   Конвертирует все банкноты в рубль через курс набирая итог банкнотами с максимальным номиналом, 
//      в случае неровной суммы округляем в большую сторону.

#include<iostream>
#include <vector>
#include <time.h>
#include <cstdlib>  
#include <map>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

enum Denomination{
    ONE = 1,
    FIVE = 5,
    TEN = 10,
    TWENTY = 20,
    FIFTY = 50,
    HUNDRED = 100,
    THOUSAND = 1000,
    DENOMINATION_COUNT = 7
};

struct Banknote{
    string name;
    Denomination denomination;
    double rate;
};

/**
 * Возвращает случайный номинал банкноты из предопределенного набора номиналов.
 *
 * @return Случайный номинал из перечисления Denomination.
 */
Denomination getRandomDenomination() {
    int randomIndex = rand() % DENOMINATION_COUNT; 

    switch (randomIndex) {
        case 0: return ONE;
        case 1: return FIVE;
        case 2: return TEN;
        case 3: return TWENTY;
        case 4: return FIFTY;
        case 5: return HUNDRED;
        case 6: return THOUSAND;
        default: return ONE; // На всякий случай
    }
}

/**
 * Рассчитывает общую сумму всех банкнот в кошельке в рублях.
 *
 * @param wallet Вектор объектов типа Banknote, представляющий кошелек.
 * @param N Количество банкнот в кошельке.
 * @return Общая сумма всех банкнот в рублях.
 */
double sumVallet(vector<Banknote>& wallet, int N){
    double result = 0;
    for(int i = 0; i < N; i++){
        result += (wallet[i].denomination * wallet[i].rate);
    }
    return result;
}


/**
 * Выводит количество банкнот с номиналом больше указанного значения.
 * 
 * @param wallet Вектор объектов типа Banknote, представляющий кошелек.
 * @param denom Минимальный номинал для фильтрации банкнот.
 * @param N Количество банкнот в кошельке.
 */
void minDenom(vector<Banknote>& wallet, int denom, int N){
    map<string, int> result;  // это просто словарь, как в питоне, ну почти
    for (int i = 0; i < N; i++){
        if (wallet[i].denomination > denom){
            result[wallet[i].name]++;
        }
    }
    for (auto note : result) {
        cout << "Denomination " << note.first << ": " << note.second << endl;
    }
}

/**
 * Сортирует банкноты в кошельке по валюте (по алфавиту) и номиналу (по возрастанию).
 * 
 * @param wallet Вектор объектов типа Banknote, представляющий кошелек.
 */
void sortDenomAndVal(vector<Banknote>& wallet){
    // Сортировка по валюте (по алфавиту) и номиналу (по возрастанию)
    sort(wallet.begin(), wallet.end(), [](const Banknote& a, const Banknote& b) {
        if (a.name == b.name) {
            return a.denomination < b.denomination;  
        }
        return a.name < b.name; 
    });

    map<pair<string, int>, int> banknoteCount;

    for (auto note : wallet) {
        banknoteCount[{note.name, note.denomination}]++;  
    }

    cout << "\nSorted wallet composition:" << endl;
    for (auto note : banknoteCount) {
        cout << "Currency " << note.first.first << ", denominaion " << note.first.second << ": " << note.second << endl;
    }
}

/**
 * Конвертирует все банкноты в рубли и округляет итоговую сумму до ближайшего большего целого числа.
 *
 * @param wallet Вектор объектов типа Banknote, представляющий кошелек.
 * @param N Количество банкнот в кошельке.
 */

void convertAll(vector<Banknote>& wallet, int N){
    double all_sum = sumVallet(wallet, N);
    int finalSum = ceil(all_sum); 

    int nominals[] = {1, 5, 10, 20, 50, 100, 1000};

    cout << "\nConverting total to banknotes:\n";
    for (int i = size(nominals) - 1; i >= 0; --i) {
        int nominal = nominals[i];
        int count = finalSum / nominal;  
        if (count > 0) {
            cout << "Nominal " << nominal << " : " << count << endl;
            finalSum -= count * nominal; 
        }
    }

}

int main(){

    int N;
    cout << "Enter the value N:" << endl ;
    cin >> N;   
    
    const int quan_varios = 10;
    string name_banknote[quan_varios]  {"USD", "EUR", "GBP", "CHF", "JPY", "CAD", "AUD", "CNY", "SGD", "NOK"}; 
    double rate_banknote[quan_varios] = {90, 95, 110, 100, 0.65, 70, 65, 13, 65, 9}; 
    srand((time(NULL)));
    vector<Banknote> wallet(N);
    for (int i = 0; i < N; i++){
        int k = rand() % quan_varios;
        wallet[i].name = name_banknote[k];
        wallet[i].rate = rate_banknote[k];
        wallet[i].denomination = getRandomDenomination();
        cout << wallet[i].name <<"  "<< wallet[i].denomination <<"  "<< wallet[i].rate << endl;
    }
    
    double all_sum = sumVallet(wallet, N);
    cout << "\nAll sum(rub):" << all_sum << endl << endl;

    int denom;
    cout << "Enter the denomination:" << endl ;
    cin >> denom;   
    minDenom(wallet, denom, N); 

    sortDenomAndVal(wallet);

    convertAll(wallet, N);

    return 0;
}