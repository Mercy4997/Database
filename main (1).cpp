#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct FunctionalDependency {
    string lhs;
    vector<string> rhs;
};

struct Relation {
    string name;
    vector<string> attributes;
    string primaryKey;
};

void decomposeTo3NF(vector<Relation> &relations, const vector<FunctionalDependency> &fds) {
    // Decompose based on functional dependencies
    map<string, unordered_set<string>> attributeDependencies;
    for (const auto &fd : fds) {
        for (const auto &attr : fd.rhs) {
            attributeDependencies[fd.lhs].insert(attr);
        }
    }

    // Relations and primary keys
    map<string, vector<string>> decomposedRelations;
    map<string, string> primaryKeys;

    // Main Employee relation excluding DeptName
    decomposedRelations["Employee"] = {"EmpID", "EmpName", "DeptID", "Salary"};
    primaryKeys["Employee"] = "EmpID";

    // Separate relation for Department based on DeptID -> DeptName
    decomposedRelations["Department"] = {"DeptID", "DeptName"};
    primaryKeys["Department"] = "DeptID";

    // Convert the decomposed relations into Relation structures
    for (const auto &relation : decomposedRelations) {
        Relation r;
        r.name = relation.first;
        r.attributes = relation.second;
        r.primaryKey = primaryKeys[relation.first];
        relations.push_back(r);
    }
}

void generateSQL(const vector<Relation> &relations) {
   
    for (const auto &relation : relations) {
        if (relation.name == "Employee") {
            cout << "CREATE TABLE " << relation.name << " (" << endl;
            for (const auto &attr : relation.attributes) {
                cout << "    " << attr << " VARCHAR(255)," << endl;
            }
            cout << "    PRIMARY KEY (" << relation.primaryKey << ")" << endl;
            cout << ");" << endl << endl;
        }
    }
    // Output other tables
    for (const auto &relation : relations) {
        if (relation.name != "Employee") {
            cout << "CREATE TABLE " << relation.name << " (" << endl;
            for (const auto &attr : relation.attributes) {
                cout << "    " << attr << " VARCHAR(255)," << endl;
            }
            cout << "    PRIMARY KEY (" << relation.primaryKey << ")" << endl;
            cout << ");" << endl << endl;
        }
    }
}

int main() {
    vector<FunctionalDependency> fds = {
        {"EmpID", {"EmpName", "DeptID", "DeptName", "Salary"}},
        {"DeptID", {"DeptName"}}
    };

    vector<Relation> relations;
    decomposeTo3NF(relations, fds);
    generateSQL(relations);

    return 0;
}
