!!! Для классов контейнеров предполагается перегрузка оператора вывода в поток через дружественную функцию и вывод информации о классе и всех внутренних объектах `<Наименование класса> {<список элементов через запятую>}`

## Разработать контейнерный класс без использования контейнерных классов стандартной библиотеки C++

### Вариант 1 - Очередь

В рамках задачи требуется написать программу, которая будет содержать реализацию контейнера "очередь" и демонстрировать его работу.

! Разработанный контейнерный класс должен быть шаблонным.
! Разработанный контейнерный класс не должен использовать стандартную библиотеку c++ для хранения своих элементов.

Для очереди определены следующие операции:
- создание пустой очереди
- добавить элемент в конец
- получить элемент из начала
- удалить элемент из начала
- проверка, пустая ли очередь
- получить размер очереди

(Усложненный вариант) Контейнер должен иметь объект итератор, позволяющий обойти всю очередь через цикл for-each(for(auto el: container))