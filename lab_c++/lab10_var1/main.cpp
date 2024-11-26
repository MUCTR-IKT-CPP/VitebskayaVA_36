#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <regex>


// Типы данных для столбцов
enum class DataType { INT, STRING, FLOAT, BOOL };

/**
 * @brief Проверяет, соответствует ли значение указанному типу данных.
 * @param data Значение в виде строки.
 * @param type Тип данных, которому должно соответствовать значение.
 * @return true, если значение соответствует типу данных, иначе false.
 */
bool validateData(const std::string& data, DataType type) {
    switch (type) {
    case DataType::INT:
        return std::regex_match(data, std::regex(R"(-?\d+)"));
    case DataType::STRING:
        return true;
    case DataType::FLOAT:
        return std::regex_match(data, std::regex(R"(-?\d+(\.\d+)?)"));
    case DataType::BOOL:
        return data == "true" || data == "false";
    default:
        return false;
    }
}

// Класс столбец
class Column {
public:
    std::string name;
    DataType type;

    Column(const std::string& name, DataType type) : name(name), type(type) {}
};

// Класс значение
class Value {
public:
    std::string data;
    Column* column;

    Value(const std::string& data, Column* column) : data(data), column(column) {}
};

// Класс строка
class Row {
public:
    /**
     * @brief Добавляет значение в строку.
     * @param value Значение для добавления.
     */
    std::vector<Value> values;

    void addValue(const Value& value) { 
        values.push_back(value); 
    }
};

// Класс таблица
class Table {
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;
    int idCounter = 1;

    Table(const std::string& name) : name(name) {
        columns.emplace_back("id", DataType::INT); // Автоматический столбец идентификаторов
    }

    /**
     * @brief Добавляет столбец в таблицу.
     * @param columnName Имя нового столбца.
     * @param type Тип данных нового столбца.
     */
    void addColumn(const std::string& columnName, DataType type) {
        for (const auto& col : columns) {
            if (col.name == columnName) {
                throw std::runtime_error("Error: Column '" + columnName + "' already exists.");
            }
        }
        columns.emplace_back(columnName, type);
    }

    /**
     * @brief Удаляет столбец из таблицы вместе с его данными.
     * @param columnName Имя столбца для удаления.
     */
    void removeColumn(const std::string& columnName) {
        if (columnName == "id") {
            throw std::runtime_error("Error: Cannot remove the 'id' column.");
        }

        // Найти столбец для удаления
        auto colIt = std::find_if(columns.begin(), columns.end(), [&](const Column& col) {
            return col.name == columnName;
        });

        if (colIt == columns.end()) {
            throw std::runtime_error("Error: Column '" + columnName + "' does not exist.");
        }

        // Вычисляется индекс удаляемых значений
        size_t colIndex = std::distance(columns.begin(), colIt);

        // Удалить данные столбца из каждой строки
        for (auto& row : rows) {
            row.values.erase(row.values.begin() + colIndex);
        }

        // Удалить сам столбец
        columns.erase(colIt);
    }


    /**
     * @brief Добавляет строку в таблицу.
     * @param rowData Данные строки, разделённые символом ';'.
     */
    void addRow(const std::string& rowData) {
        // Разделяем входные данные строки на значения
        std::stringstream ss(rowData);
        std::string token;
        std::vector<std::string> rowDataTokens;
        
        while (std::getline(ss, token, ';')) {
            rowDataTokens.push_back(token);
        }

        // Проверяем количество значений
        if (rowDataTokens.size() > columns.size() - 1) { // -1, так как первый столбец — это ID
            throw std::runtime_error("Error: Too many values in the row.");
        }
        if (rowDataTokens.size() < columns.size() - 1) {
            throw std::runtime_error("Error: Too few values in the row.");
        }

        // Проверяем типы данных для каждого значения
        for (size_t i = 0; i < rowDataTokens.size(); ++i) {
            if (!validateData(rowDataTokens[i], columns[i + 1].type)) { // i+1 для пропуска ID
                throw std::runtime_error("Error: Value '" + rowDataTokens[i] + "' does not match type of column '" + columns[i + 1].name + "'.");
            }
        }

        // Создаём новую строку
        Row newRow;

        // Добавляем ID в строку
        newRow.addValue(Value(std::to_string(idCounter++), &columns[0]));

        // Добавляем остальные значения в строку
        for (size_t i = 0; i < rowDataTokens.size(); ++i) {
            newRow.addValue(Value(rowDataTokens[i], &columns[i + 1]));
        }

        // Добавляем строку в таблицу
        rows.push_back(newRow);
    }

    /**
     * @brief Удаляет строку по идентификатору.
     * @param id Идентификатор строки для удаления.
     */
    void removeRow(int id) {
        auto it = std::find_if(rows.begin(), rows.end(), [&](const Row& row) {
            return row.values[0].data == std::to_string(id);
        });
        if (it == rows.end()) {
            throw std::runtime_error("Error: Row with ID " + std::to_string(id) + " does not exist.");
        }
        rows.erase(it);
    }

    /**
     * @brief Выводит таблицу в консоль.
     */
    void printTable() const {
        std::cout << "Table: " << name << "\n";
        for (const auto& col : columns) {
            std::cout << col.name << "\t";
        }
        std::cout << "\n";
        for (const auto& row : rows) {
            for (const auto& value : row.values) {
                std::cout << value.data << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "Total value: " << rows.size() * columns.size() << "\n";
    }

    /**
     * @brief Ищет строки, содержащие заданное значение.
     * @param searchValue Значение для поиска.
     */
    void searchByValue(const std::string& searchValue) const {
        bool found = false;

        std::cout << "Searching for value: " << searchValue << "\n";

        // Выводим заголовки столбцов
        for (const auto& col : columns) {
            std::cout << col.name << "\t";
        }
        std::cout << "\n";

        // Проходим по строкам и ищем значение
        for (const auto& row : rows) {
            for (const auto& value : row.values) {
                if (value.data == searchValue) {
                    found = true;
                    for (const auto& value : row.values) {
                        std::cout << value.data << "\t";
                    }
                    std::cout << "\n";
                    break;
                }
            }
        }

        if (!found) {
            throw std::runtime_error("Error: Value '" + searchValue + "' not found in table.");
        }
    }

      /**
     * @brief Выводит строки в заданном диапазоне индексов.
     * @param start Начальный индекс диапазона.
     * @param end Конечный индекс диапазона.
     */
    void sliceRowsByRange(int start, int end) const {
        if (start < 1 || end < 1 || start > rows.size() || end > rows.size()) {
            throw std::runtime_error("Error: Row range out of bounds. Total rows: " + std::to_string(rows.size()));
        }
        if (start > end) {
            throw std::runtime_error("Error: Start index cannot be greater than end index.");
        }

        std::cout << "Selected rows (" << start << " to " << end << "):\n";

        // Выводим заголовки столбцов
        for (const auto& col : columns) {
            std::cout << col.name << "\t";
        }
        std::cout << "\n";

        // Выводим строки в заданном диапазоне
        for (int i = start - 1; i < end; ++i) {
            for (const auto& value : rows[i].values) {
                std::cout << value.data << "\t";
            }
            std::cout << "\n";
        }
    }

    /**
     * @brief Выводит выбранные столбцы таблицы.
     * @param columnNames Список имён столбцов для вывода.
     */    
    void printSelectedColumns(const std::vector<std::string>& columnNames) const {
        std::vector<size_t> selectedIndexes;

        for (const auto& colName : columnNames) {
            auto it = std::find_if(columns.begin(), columns.end(), [&](const Column& col) {
                return col.name == colName;
            });

            if (it == columns.end()) {
                throw std::runtime_error("Error: Column '" + colName + "' does not exist.");
            }

            selectedIndexes.push_back(std::distance(columns.begin(), it));
        }

        // Выводим заголовки
        for (size_t index : selectedIndexes) {
            std::cout << columns[index].name << "\t";
        }
        std::cout << "\n";

        // Выводим строки
        for (const auto& row : rows) {
            for (size_t index : selectedIndexes) {
                std::cout << row.values[index].data << "\t";
            }
            std::cout << "\n";
        }
    }

};

// Класс каталог
class Catalog {
public:
    std::unordered_map<std::string, Table> tables;

    /**
     * @brief Создаёт новую таблицу.
     * @param name Имя новой таблицы.
     */
    void createTable(const std::string& name) {
        if (tables.find(name) != tables.end()) {
            throw std::runtime_error("Error: Table '" + name + "' already exists.");
        }
        tables.emplace(name, Table(name));
    }

    /**
     * @brief Возвращает указатель на таблицу по имени.
     * @param name Имя таблицы.
     * @return Указатель на таблицу или nullptr, если таблица не найдена.
     */
    Table* getTable(const std::string& name) {
        auto it = tables.find(name);
        if (it == tables.end()) {
            throw std::runtime_error("Error: Table '" + name + "' does not exist.");
        }
        return &it->second;
    }

    /**
     * @brief Удаляет таблицу из каталога.
     * @param name Имя таблицы для удаления.
     */
    void deleteTable(const std::string& name) {
        if (tables.erase(name) == 0) {
            throw std::runtime_error("Error: Table '" + name + "' does not exist.");
        }
    }

    /**
     * @brief Выводит список таблиц в каталоге.
     */
    void listTables() const {
        std::cout << "Tables:\n";
        for (const auto& pair : tables) {
            std::cout << "- " << pair.first << "\n";
        }
    }
};

/**
 * @brief Основное меню для работы с системой управления таблицами.
 * @param catalog Ссылка на каталог таблиц.
 */
void menu(Catalog& catalog) {
    int choice;
    do {
        std::cout << "\nSelect operation:\n";
        std::cout << "1. Create table\n";
        std::cout << "2. Add column\n";
        std::cout << "3. Remove column\n";
        std::cout << "4. Add row\n";
        std::cout << "5. Remove row\n";
        std::cout << "6. Print table\n";
        std::cout << "7. List tables\n";
        std::cout << "8. Find value\n";
        std::cout << "9. Slice rows by range (from X to Y)\n";
        std::cout << "10. Print selected columns\n";
        std::cout << "11. Delete table\n";
        std::cout << "0. Exit\n";

        std::cin >> choice;
        std::cin.ignore();
        try {
            switch (choice) {
            case 1: {
                std::string tableName;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                catalog.createTable(tableName);
                break;
            }
            case 2: {
                std::string tableName, columnName, type;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                Table* table = catalog.getTable(tableName);
                if (table) {
                    std::cout << "Enter column name: ";
                    std::getline(std::cin, columnName);
                    std::cout << "Enter column type (int, string, float, bool): ";
                    std::getline(std::cin, type);

                    DataType dataType;
                    if (type == "int")
                        dataType = DataType::INT;
                    else if (type == "string")
                        dataType = DataType::STRING;
                    else if (type == "float")
                        dataType = DataType::FLOAT;
                    else if (type == "bool")
                        dataType = DataType::BOOL;
                    else {
                        std::cerr << "Error: Unknown type.\n";
                        break;
                    }
                    table->addColumn(columnName, dataType);
                }
                break;
            }
            case 3: {
                std::string tableName, columnName;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                Table* table = catalog.getTable(tableName);
                if (table) {
                    std::cout << "Enter column name: ";
                    std::getline(std::cin, columnName);            
                    table->removeColumn(columnName);
                }
                break;
            }
            case 4: {
                std::string tableName, rowData;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                Table* table = catalog.getTable(tableName);
                if (table) {
                    std::cout << "Enter row data (separated by ';'): ";
                    std::getline(std::cin, rowData);            
                    table->addRow(rowData);
                }
                break;
            }
            case 5: {
                std::string tableName;
                int id;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                Table* table = catalog.getTable(tableName);
                if (table) {
                    std::cout << "Enter row ID to delete: ";
                    std::cin >> id;
                    table->removeRow(id);
                }
                break;
            }
            case 6: {
                std::string tableName;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);

                Table* table = catalog.getTable(tableName);
                if (table) {
                    table->printTable();
                }
                break;
            }
            case 7:
                catalog.listTables();
                break;
            case 8: {
                std::string tableName, searchValue;

                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                Table* table = catalog.getTable(tableName);
                if (table) {
                    std::cout << "Enter value to search: ";
                    std::getline(std::cin, searchValue);
                    table->searchByValue(searchValue);
                }
                break;
            }
            case 9: {
                std::string tableName;
                int start, end;

                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                Table* table = catalog.getTable(tableName);
                if (table) {
                    std::cout << "Enter start row index: ";
                    std::cin >> start;
                    std::cout << "Enter end row index: ";
                    std::cin >> end;
                    table->sliceRowsByRange(start, end);
                }
                break;
            }
            case 10: {
                std::string tableName;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);

                Table* table = catalog.getTable(tableName);
                if (table) {
                    std::cout << "Enter column names (separated by ';'): ";
                    std::string input;
                    std::getline(std::cin, input);

                    // Разбиение строки с использованием stringstream
                    std::vector<std::string> columnNames;
                    std::stringstream ss(input);
                    std::string token;

                    while (std::getline(ss, token, ';')) {
                        columnNames.push_back(token);
                    }

                    // Печать выбранных столбцов
                    table->printSelectedColumns(columnNames);
                }
                break;
            }
            case 11: {
                std::string tableName;
                std::cout << "Enter table name: ";
                std::getline(std::cin, tableName);
                catalog.deleteTable(tableName);
                break;
            }
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cerr << "Invalid choice, try again.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
        }
    } while (choice != 0);
}
    
int main() {
    Catalog catalog;
    std::cout << "Welcome to the table management system!\n";
    menu(catalog);
    return 0;
}
