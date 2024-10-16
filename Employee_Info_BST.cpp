#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* Objects of the Employee class are nodes in the BST */
class Employee {
public:
    Employee(int salary, string firstName, string lastName, string jobTitle):
        salary(salary),
        firstName(firstName),
        lastName(lastName),
        jobTitle(jobTitle) {}

    // tree relations
    Employee* parent = nullptr;
    Employee* left = nullptr;
    Employee* right = nullptr;

    // tree priority (doubling as satellite data)
    int salary;

    // satellite data
    string firstName;
    string lastName;
    string jobTitle;

    friend ostream& operator<<(ostream& os, const Employee& obj) {
        os << obj.firstName << " " << obj.lastName << " - " << obj.jobTitle << " ($" << obj.salary << ")";
        return os;
    }

    bool operator<(const Employee& other) const {
        return salary < other.salary;
    }

    bool operator<=(const Employee& other) const {
        return salary <= other.salary;
    }

    bool operator>(const Employee& other) const {
        return salary > other.salary;
    }

    bool operator>=(const Employee& other) const {
        return salary >= other.salary;
    }
};

/* A binary search tree that contains Employees */
class EmployeeBST {
private:
    Employee* root = nullptr;

    Employee* search(Employee* x, int salary) {
        if (x == nullptr || x->salary == salary) return root;
        if (x->salary < salary) return search(x->left, salary);
        return search(x->right, salary);
    }



public:
    void insert(Employee employee) {

    }

    Employee remove(int salary) {

    }

    vector<Employee> searchRange(int min, int max) {

    }
};

/*
The UI class contains functions relating to the UI of the application.
They do not need to be wrapped in a class, but since they are logically
associated, this groups them under the same namespace.
*/
class UI {
public:
    void mainMenu() {

    }

    void addEmployee() {

    }

    void deleteEmployee() {

    }

    void searchEmployee() {

    }

    void quit() {

    }
};

void initializeDummyData(EmployeeBST* bst) {

}

UI ui;
EmployeeBST bst;

int main()
{
    cout << Employee(1, "John", "Smith", "Plumber");
}
