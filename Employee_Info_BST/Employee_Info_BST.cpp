/*
Names:  Jonah Ebent and Evan Whitmer
Class:  CSC-344-A   Algorithms and Complexity
Date:   October 19, 2024

Description:
Write a program in C++ that implements a binary search tree, where at each node
there is an object representing an employee’s information within a company (i.e. an
object whose member variables are their first and last name, their job title, and their
salary). The tree should be organized based on the employees’ salary.

Your program should have the following features:

1.
A menu asking user whether they want to add an employee, delete an
employee, quit the program, or search the “database” for employees who
fall between a minimum and maximum salary range.

2.
An implementation for each of these menu options (I’ll let details up to you).
After the program executes each option (sans “quit”) the program should return
the user to the main menu.

3.
A driver that demonstrates that each function works correctly.
Be sure to add at least 10000 random “employees” to the tree in your
driver using your “add" function, where salary ranges are from 30000 and 200000.
Then, print the menu for users to choose from.
*/
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

    // The node's parent
    Employee* p = nullptr;
    // The node's left child
    Employee* left = nullptr;
    // The node's right child
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

    bool operator==(const Employee& other) const {
        return salary == other.salary
            && firstName == other.firstName
            && lastName == other.lastName
            && jobTitle == other.jobTitle;
    }
};

/* A binary search tree that contains Employees */
class EmployeeBST {
private:
    Employee* minimum(Employee* e) {
        while (e->left != nullptr) {
            e = e->left;
        }
        return e;
    }

    Employee* maximum(Employee* e) {
        while (e->right != nullptr) {
            e = e->right;
        }
        return e;
    }

    Employee* successor(Employee* e) {
        // if e has a right child, return minimum of right subtree
        if (e->right != nullptr) return minimum(e->right);

        // else, return first parent greater than e
        Employee* parent = e->p;
        while (parent != nullptr && e == parent->right) {
            e = parent;
            parent = parent->p;
        }
        return parent;
    }

    void transplant(Employee* u, Employee* v) {
        if (u->p == nullptr) root = v;
        else if (u == u->p->left) u->p->left = v;
        else u->p->right = v;
    }

public:
    EmployeeBST() {}

    // The root of the tree, initialized to NIL
    Employee* root = nullptr;

    void insert(Employee employee) {
        // start at the root
        Employee* current = root;
        Employee* parent = nullptr;
        // traverse down the tree
        while (current != nullptr) {
            parent = current;
            if (employee.salary < current->salary) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        // establish tree relationships
        employee.p = parent;
        if (parent == nullptr) root = &employee;
        else if (employee.salary < parent->salary) parent->left = &employee;
        else parent->right = &employee;
    }

    void remove(Employee* z) {
        if (z->left == nullptr) transplant(z, z->right);
        else if (z->right == nullptr) transplant(z, z->left);
        else {
            Employee* y = successor(z);
            if (y != z->right) {
                transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
        }
    }

    Employee* search(Employee* e, int salary) {
        if (e == nullptr || e->salary == salary) return e;
        if (e->salary < salary) return search(e->left, salary);
        return search(e->right, salary);
    }

    void printEmployeesInRange(int min, int max) {
        Employee* p = nullptr;
        Employee* e = root;
        while (e != nullptr && e->salary != min) {
            if (min < e->salary) {
                p = e;
                e = e->left;
            }
            else {
                p = e;
                e = e->right;
            }
        }
        if (e == nullptr) e = p;
        if (e->salary < min) e = successor(e);
        while (e->salary < max) {
            cout << e << endl;
            e = successor(e);
        }
    }

    void printInOrder(Employee* e) {
        if (e != nullptr) {
            printInOrder(e->left);
            cout << *e << endl;
            printInOrder(e->right);
        }
    }
};

/* The UI class contains functions relating to the UI of the
application. They do not need to be wrapped in a class, but
since they are logically associated, this groups them under
the same namespace. */
class UI {
private:
    EmployeeBST* employees = nullptr;

    bool isBetween(int num, int* min, int* max) {
        if (min != nullptr && num < *min) return false;
        if (max != nullptr && num > *max) return false;
        return true;
    }

    int inputInteger(int* min, int* max) {
        int intput;
        string input;
        cout << "--------------------------" << endl;
        do {
            cout << "Input: ";      // prompt user for input
            getline(cin, input);

            try { intput = stoi(input); }
            catch (const std::invalid_argument&) {
                cout << "  Invalid input. Please enter a number between " << *min << " and " << *max << "(inclusive)." << endl;
                continue;
            }

            if (isBetween(intput, min, max)) break;

            cout << "  Invalid input. Please enter a number between " << *min << " and " << *max << " (inclusive)." << endl;
        } while (true);                 // continue looping until input is valid
        // input is valid, so return it
        return intput;
    }

    Employee inputEmployee() {
        int salary;
        string firstName, lastName, jobTitle;
        cout << "Enter a first name: " << endl;
        getline(cin, firstName);
        cout << "Enter a last name: " << endl;
        getline(cin, lastName);
        cout << "Enter a job title: " << endl;
        getline(cin, jobTitle);
        cout << "Enter a salary between 30000 and 200000." << endl;
        int min = 30000;
        int max = 200000;
        salary = inputInteger(&min, &max);
        return Employee(salary, firstName, lastName, jobTitle);
    }

public:
    UI(EmployeeBST* bst): employees(bst) {}

    void mainMenu() {
        cout << "Welcome to the employee \"database\"" << endl;
        cout << "What would you like to do?" << endl;
        cout << "  1) Add an employee" << endl;
        cout << "  2) Delete an employee" << endl;
        cout << "  3) Search for employees" << endl;
        cout << "  4) Quit" << endl;
        int min = 1;
        int max = 4;
        switch (inputInteger(&min, &max))
        {
        case 1:
            addEmployee();
            break;
        case 2:
            deleteEmployee();
            break;
        case 3:
            searchEmployee();
            break;
        case 4:
            exit(0);
        default:
            throw runtime_error("How did we get here?!?!\n");
            break;
        }
    }

    void addEmployee() {
        Employee e = inputEmployee();
        employees->insert(e);
        cout << "Successfully inserted " << e.firstName << " " << e.lastName << " into the database" << endl;
    }

    void deleteEmployee() {
        int min = 30000;
        int max = 200000;
        cout << "Select a salary to search for." << endl;
        int salary = inputInteger(&min, &max);
        Employee* z = employees->search(employees->root, salary);
        string confirmation;
        do {
            if (z == nullptr) {
                cout << "No employee found with the given salary." << endl;
                break;
            }
            cout << "Delete employee " << *z << "? (y/n)" << endl;
            getline(cin, confirmation);
            if (confirmation == "y") {
                employees->remove(z);
                cout << "Successfully deleted " << *z << " from the database" << endl;
                break;
            }
            else if (confirmation == "n") {
                z = employees->search(z->right, salary);
            }
            else {
                cout << "Invalid input" << endl;
            }
        } while (true);
    }

    void searchEmployee() {
        cout << "Enter a minimum value." << endl;
        int min = inputInteger(nullptr, nullptr);
        cout << "Enter a maximum value." << endl;
        int max = inputInteger(&min, nullptr);
        employees->printEmployeesInRange(min, max);
    }
};

/* Inserts 10,000 employees into the BST, with random data
and salaries ranging from 30,000 to 200,000.*/
void initializeDummyData(EmployeeBST* bst) {
    //Employee e = Employee(30000, "Jonah", "Ebent", "Code Monkey");
    //bst->insert(e);
}

int main()
{
    EmployeeBST bst;
    UI ui = UI(&bst);
    initializeDummyData(&bst);
    while (true) ui.mainMenu();
}
