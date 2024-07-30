#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

// Define structures for tables and dependencies
struct Column {
    std::string name;
    std::string type;
};

struct Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<std::string> primaryKey;
    std::vector<std::vector<std::string>> candidateKeys;
};

struct FunctionalDependency {
    std::vector<std::string> lhs;
    std::vector<std::string> rhs;
};

struct MultiValuedDependency {
    std::vector<std::string> lhs;
    std::vector<std::string> rhs;
};

// Helper function to generate SQL query
std::string generateSQLQuery(const Table& table) {
    std::string query = "CREATE TABLE " + table.name + " (";
    for (const auto& column : table.columns) {
        query += column.name + " " + column.type + ", ";
    }
    query += "PRIMARY KEY (";
    for (size_t i = 0; i < table.primaryKey.size(); ++i) {
        query += table.primaryKey[i];
        if (i < table.primaryKey.size() - 1) query += ", ";
    }
    query += "));";
    return query;
}

// Function to print SQL queries
void printSQLQueries(const std::vector<Table>& tables) {
    for (const auto& table : tables) {
        std::cout << generateSQLQuery(table) << std::endl;
    }
}

// Function to parse the CSV input into a Table
Table parseTableFromCSV(const std::string& filename) {
    Table table;
    std::ifstream file(filename);
    std::string line;

    if (std::getline(file, line)) {
        std::istringstream headerStream(line);
        std::string columnName;
        while (std::getline(headerStream, columnName, ',')) {
            table.columns.push_back({columnName, "VARCHAR(255)"}); // Default type, modify as needed
        }
    }

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string cell;
        std::vector<std::string> cells;
        while (std::getline(lineStream, cell, ',')) {
            cells.push_back(cell);
        }
        if (!cells.empty() && table.primaryKey.empty()) {
            table.primaryKey.push_back(cells[0]); // Assume the first column is the primary key
        }
    }

    table.name = "ParsedTable"; // You can modify this to a more meaningful name
    return table;
}

// Placeholder normalization functions
Table normalizeTo1NF(const Table& table) {
    return table;
}

std::vector<Table> normalizeTo2NF(const Table& table, const std::vector<FunctionalDependency>& fds) {
    std::vector<Table> result;
    result.push_back(table);
    result.push_back({"CustomerOrders", {{"CustomerID", "INT"}, {"OrderID", "INT"}}, {"CustomerID", "OrderID"}, {}});
    return result;
}

std::vector<Table> normalizeTo3NF(const std::vector<Table>& tables, const std::vector<FunctionalDependency>& fds) {
    std::vector<Table> result;
    result.push_back({"Orders", {{"OrderID", "INT"}, {"OrderDate", "DATE"}}, {"OrderID"}, {}});
    result.push_back({"Customers", {{"CustomerID", "INT"}}, {"CustomerID"}, {}});
    result.push_back({"CustomerOrders", {{"CustomerID", "INT"}, {"OrderID", "INT"}}, {"CustomerID", "OrderID"}, {}});
    return result;
}

std::vector<Table> normalizeToBCNF(const std::vector<Table>& tables, const std::vector<FunctionalDependency>& fds) {
    return tables;
}

std::vector<Table> normalizeTo4NF(const std::vector<Table>& tables, const std::vector<MultiValuedDependency>& mvds) {
    return tables;
}

std::vector<Table> normalizeTo5NF(const std::vector<Table>& tables) {
    return tables;
}

int main() {
    // Input filename
    std::string filename = "dataset.csv"; // Update this with the path to your CSV file

    // Parse the input CSV file
    Table table = parseTableFromCSV(filename);

    // Example functional dependencies and multi-valued dependencies
    std::vector<FunctionalDependency> fds = {
        // Add actual FDs here
    };

    std::vector<MultiValuedDependency> mvds = {
        // Add actual MVDs here
    };

    // Normalize and generate output
    Table table1NF = normalizeTo1NF(table);
    auto tables2NF = normalizeTo2NF(table1NF, fds);
    auto tables3NF = normalizeTo3NF(tables2NF, fds);
    auto tablesBCNF = normalizeToBCNF(tables3NF, fds);
    auto tables4NF = normalizeTo4NF(tablesBCNF, mvds);
    auto tables5NF = normalizeTo5NF(tables4NF);

    // Output SQL queries for each normalization step
    std::cout << "1NF (First Normal Form):" << std::endl;
    printSQLQueries({table1NF});

    std::cout << "2NF (Second Normal Form):" << std::endl;
    printSQLQueries(tables2NF);

    std::cout << "3NF (Third Normal Form):" << std::endl;
    printSQLQueries(tables3NF);

    std::cout << "BCNF (Boyce-Codd Normal Form):" << std::endl;
    printSQLQueries(tablesBCNF);

    std::cout << "4NF (Fourth Normal Form):" << std::endl;
    printSQLQueries(tables4NF);

    std::cout << "5NF (Fifth Normal Form):" << std::endl;
    printSQLQueries(tables5NF);

    return 0;
}
