#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <Windows.h>

using namespace std;

enum class DataType { INTEGER, FLOAT, STRING };

class Column {
public:
    string name;
    DataType type;

    Column(const string& name, DataType type) : name(name), type(type) {}
};

class Value {
public:
    Column* column;
    string value;

    Value(Column* column, const string& value) : column(column), value(trim(value)) {}

    /**
     * Убирает ведущие и замыкающие пробелы из строки.
     * 
     * @param str Строка, из которой нужно удалить пробелы.
     * @return Строка без ведущих и замыкающих пробелов.
     */
    string trim(const string& str) {
        size_t first = str.find_first_not_of(' ');
        if (string::npos == first) {
            return "";
        }
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    /**
     * Проверяет, корректно ли значение в соответствии с типом данных колонки.
     * 
     * @return true, если значение корректное; false иначе.
     * @throws invalid_argument Если тип данных колонка неизвестен.
     */
    bool validate() const {
        switch (column->type) {
        case DataType::INTEGER:
            stoi(value);
            return true;
        case DataType::FLOAT:
            stof(value);
            return true;
        case DataType::STRING:
            return true;
        default:
            throw invalid_argument("Неизвестный тип данных.");
        }
    }
};

class Row {
public:
    vector<Value> values;

    /**
     * Добавляет значение в колонку строки.
     * 
     * @param column Указатель на колонку, значение которой добавляется.
     * @param value Значение для добавления.
     */
    void add_value(Column* column, const string& value) {
        values.emplace_back(column, value);
    }
};

class Table {
private:
    vector<Column> columns;
    vector<Row> rows;

public:
    Table() {
        columns.emplace_back("id", DataType::INTEGER);
    }

    /**
     * Возвращает имена всех колонок в таблице.
     * 
     * @return Вектор строк с именами колонок.
     */
    vector<string> get_column_names() const {
        vector<string> names;
        for (const auto& col : columns) {
            names.push_back(col.name);
        }
        return names;
    }

    /**
     * Возвращает тип данных колонки по индексу.
     * 
     * @param index Индекс колонки.
     * @return Тип данных колонки.
     * @throws out_of_range Если индекс вне диапазона.
     */
    DataType get_column_type(int index) const {
        if (index < 0 || index >= columns.size()) {
            throw out_of_range("Индекс столбца вне диапазона.");
        }
        return columns[index].type;
    }

    /**
     * Добавляет колонку в таблицу.
     * 
     * @param name Имя колонки.
     * @param type Тип данных колонки (INTEGER, FLOAT, STRING).
     * @throws invalid_argument Если колонка с таким именем уже существует.
     */
    void add_column(const string& name, DataType type) {
        auto it = find_if(columns.begin(), columns.end(), [&name](const Column& col) {
            return col.name == name;
            });

        if (it != columns.end()) {
            throw invalid_argument("Столбец с именем " + name + " уже существует.");
        }

        columns.emplace_back(name, type);
        for (Row& row : rows) {
            row.values.emplace_back(&columns.back(), "");
        }
    }

    /**
     * Выполняет поиск по значению в строках таблицы.
     * 
     * @param value Значение для поиска.
     * @return Вектор строк, содержащих найденные строки.
     */
    vector<Row> search(const string& value) const {
        vector<Row> results;

        for (const auto& row : rows) {
            for (const auto& val : row.values) {
                switch (val.column->type) {
                case DataType::INTEGER: {
                    try {
                        int intValue = stoi(value);
                        if (stoi(val.value) == intValue) {
                            results.push_back(row);
                        }
                    }
                    catch (const invalid_argument&) {
                    }
                    break;
                }
                case DataType::FLOAT: {
                    try {
                        float floatValue = stof(value);
                        if (stof(val.value) == floatValue) {
                            results.push_back(row);
                        }
                    }
                    catch (const invalid_argument&) {
                    }
                    break;
                }
                case DataType::STRING: {
                    string lowerValue = value;
                    transform(value.begin(), value.end(), lowerValue.begin(), ::tolower);
                    string lowerValValue = val.value;
                    transform(lowerValValue.begin(), lowerValValue.end(), lowerValValue.begin(), ::tolower);

                    if (lowerValValue == lowerValue) {
                        results.push_back(row);
                    }
                    break;
                }
                }
            }
        }
        return results;
    }

    /**
     * Выполняет срез строк таблицы.
     * 
     * @param start Начальный индекс среза (включительно).
     * @param end Конечный индекс среза (не включительно).
     * @return Вектор строк, содержащих срез.
     * @throws invalid_argument Если диапазон не корректен.
     */
    vector<Row> slice(int start, int end) const {
        if (start < 0 || end > rows.size() || start >= end) {
            throw invalid_argument("Неверный диапазон для среза.");
        }
        vector<Row> slicedRows(rows.begin() + start, rows.begin() + end);
        return slicedRows;
    }

    /**
     * Печатает выбранные колонки таблицы.
     * 
     * @param selected_columns Вектор строк с именами выбранных колонок.
     * @throws invalid_argument Если колонка не существует.
     */
    void print_selected_columns(const vector<string>& selected_columns) const {
        cout << "Структура таблицы (выбранные столбцы):\n";
        for (const auto& col : selected_columns) {
            auto it = find_if(columns.begin(), columns.end(), [&col](const Column& column) {
                return column.name == col;
                });

            if (it != columns.end()) {
                cout << it->name << " (тип: " << static_cast<int>(it->type) << ")\t";
            }
            else {
                throw invalid_argument("Столбец " + col + " не существует.");
            }
        }
        cout << "\n";

        for (const auto& row : rows) {
            for (const auto& col : selected_columns) {
                auto it = find_if(columns.begin(), columns.end(), [&col](const Column& column) {
                    return column.name == col;
                    });

                if (it != columns.end()) {
                    int index = distance(columns.begin(), it);
                    cout << row.values[index].value << "\t";
                }
            }
            cout << "\n";
        }
        cout << "Всего строк: " << rows.size() << "\n";
    }

    /**
     * Удаляет колонку по имени.
     * 
     * @param name Имя колонки для удаления.
     * @throws runtime_error Если колонка не существует.
     */
    void remove_column(const string& name) {
        auto it = remove_if(columns.begin(), columns.end(), [&name](const Column& col) {
            return col.name == name;
            });

        if (it != columns.end()) {
            int index = distance(columns.begin(), it);
            columns.erase(it, columns.end());

            for (Row& row : rows) {
                row.values.erase(row.values.begin() + index);
            }
        }
        else {
            throw runtime_error("Столбец не существует.");
        }
    }

    /**
     * Добавляет строку с данными.
     * 
     * @param data Данные строки, разделенные символом «;».
     * @throws invalid_argument Если количество предоставленных значений не соответствует количеству колонок.
     */
    void add_row(const string& data) {
        int newID = 1;
        vector<int> ids;

        for (const Row& row : rows) {
            if (!row.values.empty()) {
                ids.push_back(stoi(row.values[0].value));
            }
        }

        while (find(ids.begin(), ids.end(), newID) != ids.end()) {
            newID++;
        }

        Row row;
        row.add_value(&columns[0], to_string(newID));
        stringstream ss(data);
        string value;

        int count_column = columns.size() - 1;
        int i = 0;

        while (getline(ss, value, ';')) {
            if (i >= count_column) {
                throw invalid_argument("Количество предоставленных значений не соответствует количеству столбцов.");
            }

            Value newValue(&columns[i + 1], value);
            try {
                newValue.validate();
            }
            catch (const invalid_argument& e) {
                throw invalid_argument("Ошибка при добавлении строки: " + string(e.what()));
            }

            row.add_value(&columns[i + 1], value);
            i++;
        }

        if (row.values.size() - 1 != count_column) {
            throw invalid_argument("Количество предоставленных значений не соответствует количеству столбцов.");
        }

        rows.push_back(row);
    }

    /**
     * Удаляет строку по ID.
     * 
     * @param id ID строки для удаления.
     * @throws runtime_error Если строка не существует.
     */
    void remove_row(int id) {
        if (id <= 0 || id > rows.size()) {
            throw runtime_error("Строка не существует.");
        }

        rows.erase(rows.begin() + (id - 1));
    }

    /**
     * Печатает всю таблицу.
     */
    void print_table() const {
        cout << "Структура таблицы:\n";
        for (const auto& col : columns) {
            cout << col.name << " (тип: " << static_cast<int>(col.type) << ")\t";
        }
        cout << "\n";

        for (const auto& row : rows) {
            for (const auto& val : row.values) {
                cout << val.value << "\t\t";
            }
            cout << "\n";
        }
        cout << "Всего строк: " << rows.size() << "\n";
    }
};

class Catalog {
private:
    map<string, Table> tables;

    /**
     * Проверяет, существует ли таблица с указанным именем.
     * 
     * @param tableName Имя таблицы для проверки.
     * @throws runtime_error Если таблица не существует.
     */
    void throwIfTableDoesNotExist(const string& tableName) const {
        if (tables.find(tableName) == tables.end()) {
            throw runtime_error("Таблица не существует.\n");
        }
    }

    /**
     * Проверяет, есть ли таблицы в каталоге.
     * 
     * @throws runtime_error Если таблицы отсутствуют.
     */
    void throwIfNoTablesExist() const {
        if (tables.empty()) {
            throw runtime_error("Нет таблиц для выполнения операции.\n");
        }
    }

public:
    /**
     * Создает новую таблицу.
     */
    void create_table() {
        string table_name;
        cout << "Введите название таблицы: ";
        getline(cin, table_name);
        tables.insert({ table_name, Table() });
        cout << "Таблица " << table_name << " создана успешно.\n";
    }

    /**
     * Удаляет таблицу по имени.
     * 
     * @throws runtime_error Если таблица не существует.
     */
    void remove_table() {
        throwIfNoTablesExist();

        string table_name;
        cout << "Введите имя таблицы для удаления: ";
        getline(cin, table_name);

        if (tables.erase(table_name) == 0) {
            throw runtime_error("Таблица не существует.\n");
        }

        cout << "Таблица " << table_name << " удалена успешно.\n";
    }

    /**
     * Добавляет колонку в таблицу.
     */
    void add_column_to_table() {
        throwIfNoTablesExist();

        string table_name, column_name;
        int type_choice;

        cout << "Введите имя таблицы для добавления столбца: ";
        getline(cin, table_name);
        throwIfTableDoesNotExist(table_name);

        cout << "Введите имя столбца: ";
        getline(cin, column_name);

        while (true) {
            cout << "Введите тип данных (0 - INTEGER, 1 - FLOAT, 2 - STRING): ";
            cin >> type_choice; cin.ignore();

            if (type_choice < 0 || type_choice > 2) {
                cout << "Ошибка: Неверный ввод. Пожалуйста, введите 0, 1 или 2.\n";
            }
            else {
                break;
            }
        }

        DataType type = static_cast<DataType>(type_choice);
        tables.at(table_name).add_column(column_name, type);

        cout << "Столбец " << column_name << " (тип: " << type_choice << ") добавлен в таблицу " << table_name << ".\n";
    }

    /**
     * Добавляет строку в таблицу.
     */
    void add_row_to_table() {
        throwIfNoTablesExist();

        string table_name, data;
        cout << "Введите имя таблицы для добавления строки: ";
        getline(cin, table_name);

        throwIfTableDoesNotExist(table_name);

        cout << "Введите данные строки (разделенные символом «;»): ";
        getline(cin, data);
        tables.at(table_name).add_row(data);
        cout << "Строка добавлена в таблицу " << table_name << ".\n";
    }

    /**
     * Удаляет строку из таблицы по ее ID.
     */
    void remove_row_from_table() {
        throwIfNoTablesExist();

        string table_name;
        int rowID;
        cout << "Введите имя таблицы для удаления строки: ";
        getline(cin, table_name);

        throwIfTableDoesNotExist(table_name);

        cout << "Введите ID строки для удаления: ";
        cin >> rowID; cin.ignore();
        tables.at(table_name).remove_row(rowID);
        cout << "Строка " << rowID << " удалена из таблицы " << table_name << ".\n";
    }

    /**
     * Удаляет колонку из таблицы по имени.
     */
    void remove_column_from_table() {
        throwIfNoTablesExist();

        string table_name, column_name;
        cout << "Введите имя таблицы для удаления столбца: ";
        getline(cin, table_name);

        throwIfTableDoesNotExist(table_name);

        cout << "Введите имя столбца: ";
        getline(cin, column_name);
        tables.at(table_name).remove_column(column_name);
        cout << "Столбец " << column_name << " удален из таблицы " << table_name << ".\n";
    }

    /**
     * Выполняет поиск по значению в таблице.
     */
    void search_in_table() {
        throwIfNoTablesExist();

        string table_name, value;
        cout << "Введите имя таблицы для поиска: ";
        getline(cin, table_name);

        throwIfTableDoesNotExist(table_name);

        cout << "Введите значение для поиска: ";
        getline(cin, value);

        vector<Row> results = tables.at(table_name).search(value);
        cout << "Результаты поиска:\n";

        vector<string> column_names = tables.at(table_name).get_column_names();
        for (const auto& col_name : column_names) {
            cout << col_name << "\t";
        }
        cout << "\n";

        for (const auto& row : results) {
            for (const auto& val : row.values) {
                cout << val.value << "\t";
            }
            cout << "\n";
        }
        cout << "Всего найдено строк: " << results.size() << "\n";
    }

    /**
     * Выполняет срез таблицы.
     */
    void slice_table() {
        throwIfNoTablesExist();

        string table_name;
        int start, end;
        cout << "Введите имя таблицы для среза: ";
        getline(cin, table_name);

        throwIfTableDoesNotExist(table_name);

        cout << "Введите начальный индекс среза: ";
        cin >> start;
        cout << "Введите конечный индекс среза: ";
        cin >> end;
        cin.ignore();

        vector<Row> slicedRows = tables.at(table_name).slice(start, end);
        cout << "Результат среза:\n";
        for (const auto& row : slicedRows) {
            for (const auto& val : row.values) {
                cout << val.value << "\t";
            }
            cout << "\n";
        }
    }

    /**
     * Печатает выбранные колонки таблицы.
     */
    void print_selected_columns() {
        throwIfNoTablesExist();

        string table_name;
        cout << "Введите имя таблицы для вывода определенных столбцов: ";
        getline(cin, table_name);

        throwIfTableDoesNotExist(table_name);

        cout << "Введите имена столбцов, разделенные запятыми: ";
        string columns_input;
        getline(cin, columns_input);
        vector<string> selected_columns;
        stringstream ss(columns_input);
        string column;

        while (getline(ss, column, ',')) {
            selected_columns.push_back(column);
        }

        tables.at(table_name).print_selected_columns(selected_columns);
    }

    /**
     * Печатает таблицу по имени.
     * 
     * @param table_name Имя таблицы для печати.
     * @throws runtime_error Если таблицы не существует.
     */
    void printTable(const string& table_name) const {
        throwIfTableDoesNotExist(table_name);
        tables.at(table_name).print_table();
    }
};

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Catalog catalog;
    string command;

    while (true) {
        cout << "\nВведите команду(create_table, remove_table, \nadd_column, add_row, remove_row, remove_column, \nsearch, slice, print_selected_columns, print_table, exit): ";
        getline(cin, command);
        try {
            if (command == "create_table") {
                catalog.create_table();
            }
            else if (command == "remove_table") {
                catalog.remove_table();
            }
            else if (command == "add_column") {
                catalog.add_column_to_table();
            }
            else if (command == "add_row") {
                catalog.add_row_to_table();
            }
            else if (command == "remove_row") {
                catalog.remove_row_from_table();
            }
            else if (command == "remove_column") {
                catalog.remove_column_from_table();
            }
            else if (command == "search") {
                catalog.search_in_table();
            }
            else if (command == "slice") {
                catalog.slice_table();
            }
            else if (command == "print_selected_columns") {
                catalog.print_selected_columns();
            }
            else if (command == "print_table") {
                string table_name;
                cout << "Введите название таблицы для печати: ";
                getline(cin, table_name);
                catalog.printTable(table_name);
            }
            else if (command == "exit") {
                break;
            }
            else {
                cout << "\t·Error: Неверная команда\n\n";
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}