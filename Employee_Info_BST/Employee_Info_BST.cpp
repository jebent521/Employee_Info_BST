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
#include <functional>
#include <string>
#include <vector>

using namespace std;

struct Emp {
    Emp() {};
    Emp(string firstName, string lastName, string jobTitle, int salary) :
        firstName(firstName),
        lastName(lastName),
        jobTitle(jobTitle),
        salary(salary) {}

    int salary;
    string firstName;
    string lastName;
    string jobTitle;

    bool operator<(const Emp& other) const {
        return salary < other.salary;
    }

    bool operator<=(const Emp& other) const {
        return salary <= other.salary;
    }

    bool operator>(const Emp& other) const {
        return salary > other.salary;
    }

    bool operator>=(const Emp& other) const {
        return salary >= other.salary;
    }
    
    bool operator==(const Emp& other) const {
        return salary == other.salary
            && firstName == other.firstName
            && lastName == other.lastName
            && jobTitle == other.jobTitle;
    }

    bool operator!=(const Emp& other) const {
        return !(*this == other);
    }

    friend ostream& operator<<(ostream& os, const Emp& obj) {
        os << obj.firstName << " " << obj.lastName << ", " << obj.jobTitle << " ($" << obj.salary << ")";
        return os;
    }
};

class BST {

    struct node {
        node(Emp data) : data(data), left(nullptr), right(nullptr) {}

        Emp data;
        node* left;
        node* right;
    };

    node* root;

    node* makeEmpty(node* t) {
        if (t == nullptr)
            return nullptr;
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        return nullptr;
    }

    node* insert(Emp x, node* t) {
        if (t == nullptr) {
            t = new node(x);
            // t->data = x;
            // t->left = t->right = nullptr;
        }
        else if (x < t->data) {
            t->left = insert(x, t->left);
        }
        else {
            t->right = insert(x, t->right);
        }
        return t;
    }

    node* findMin(node* t) {
        if (t == nullptr) return nullptr;
        else if (t->left == nullptr) return t;
        else return findMin(t->left);
    }

    node* findMax(node* t) {
        if (t == nullptr) return nullptr;
        else if (t->right == nullptr) return t;
        else return findMax(t->right);
    }

    node* remove(Emp x, node* t) {
        node* temp;
        if (t == nullptr) return nullptr;
        else if (x != t->data) { // traverse down the tree
            if (x < t->data) {
                t->left = remove(x, t->left);
            }
            else if (x > t->data) {
                t->right = remove(x, t->right);
            }
        }
        // found a node with the same salary
        else if (t->left && t->right) {         // if it has two children
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->data, t->right);
        }
        else {                                  // if it has one
            temp = t;
            if (t->left == nullptr) {
                t = t->right;
            }
            else if (t->right == nullptr) {
                t = t->left;
            }
            delete temp;
        }

        return t;
    }

    void inorder(node* t) {
        if (t == nullptr)
            return;
        inorder(t->left);
        cout << t->data << endl;
        inorder(t->right);
    }

    void inorderConditional(node* t, function<bool(node *)> condition) {
        if (t == nullptr || !condition(t)) return;
        inorderConditional(t->left, condition);
        cout << t->data << endl;
        inorderConditional(t->right, condition);
    }

    node* find(node* t, int x) {
        if (t == nullptr) return nullptr;
        else if (x < t->data.salary) return find(t->left, x);
        else if (x > t->data.salary) return find(t->right, x);
        else return t;
    }

public:
    BST() {
        root = nullptr;
    }

    ~BST() {
        root = makeEmpty(root);
    }

    void insert(Emp x) {
        root = insert(x, root);
    }

    void remove(Emp x) {
        remove(x, root);
    }

    void display() {
        inorder(root);
        cout << endl;
    }

    Emp* search(int x) {
        node* result = find(root, x);
        if (result == nullptr) return nullptr;
        return &result->data;
    }

    vector<Emp> findAll(int x) {
        vector<Emp> out;
        node* y = find(root, x);
        if (y == nullptr) return out;
        out.push_back(y->data);
        while (true) {
            y = findMin(y->right);
            if (y == nullptr || y->data.salary != x) break;
            out.push_back(y->data);
        }
        return out;
    }

    void printInRange(int min, int max) {
        inorderConditional(root, [min, max](node* t) {
            return t->data.salary >= min && t->data.salary <= max;
        });
    }
};

/* The UI class contains functions relating to the UI of the
application. They do not need to be wrapped in a class, but
since they are logically associated, this groups them under
the same namespace. */
class UI {
private:
    BST* employees = nullptr;

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

    Emp inputEmployee() {
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
        return Emp(firstName, lastName, jobTitle, salary);
    }

public:
    UI(BST* bst): employees(bst) {}

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
        Emp e = inputEmployee();
        employees->insert(e);
        cout << "Successfully inserted " << e.firstName << " " << e.lastName << " into the database" << endl;
    }

    void deleteEmployee() {
        int min = 30000;
        int max = 200000;
        cout << "Select a salary to search for." << endl;
        int salary = inputInteger(&min, &max);
        vector<Emp> v = employees->findAll(salary);
        Emp z;
        int n = v.size();
        if (n == 0) {
            cout << "No employees found with the given salary." << endl;
            return;
        }
        else if (n == 1) {
            z = v.at(0);
        }
        else {
            // Display employees
            cout << "Found " << n << " employees with salary $" << salary << ". Which would you like to delete?" << endl;
            for (int i = 0; i < n; i++) {
                cout << "  " << i+1 << ") " << v.at(i);
            }
            // Get user input
            min = 1;
            z = v.at(inputInteger(&min, &n) - 1);
        }
        cout << "Delete " << z << "?" << endl;
        cout << "  1) Yes" << endl;
        cout << "  2) No" << endl;
        min = 1;
        max = 2;
        if (inputInteger(&min, &max) == 1) {
            employees->remove(z);
            cout << "Deleted " << z << endl;
        }
        else {
            cout << "Canceled" << endl;
        }
    }

    void searchEmployee() {
        cout << "Enter a minimum value." << endl;
        int min = inputInteger(nullptr, nullptr);
        cout << "Enter a maximum value." << endl;
        int max = inputInteger(&min, nullptr);
        employees->printInRange(min, max);
    }
};

/* Inserts 10,000 employees into the BST, with random data
and salaries ranging from 30,000 to 200,000.*/
void initializeDummyData(BST* bst) {
    //Employee e = Employee(30000, "Jonah", "Ebent", "Code Monkey");
    //bst->insert(e);
}

int main()
{
    BST bst;
    UI ui = UI(&bst);
    initializeDummyData(&bst);
    while (true) ui.mainMenu();
    //Employee* jonah = new Employee(100000, "J", "A", "E");
    //bst.insert(Employee(100000, "J", "A", "E"));
    //bst.printInOrder(bst.root);
    //BST t;
    //t.insert(Emp("A", "A", "A", 100000));
    //t.insert(Emp("B", "B", "B", 150000));
    //t.insert(Emp("C", "C", "C", 75000));
    //t.insert(Emp("D", "D", "D", 50000));
    //t.insert(Emp("E", "E", "E", 200000));
    //t.insert(Emp("F", "F", "F", 200000));
    //t.insert(Emp("G", "G", "G", 200000));
    //t.insert(Emp("H", "H", "H", 200000));
    //t.display();
    //Emp a = *t.search(100000);
    //t.remove(a);
    //t.display();
    //vector<Emp> v = t.findAll(200000);
    //for (Emp e : v) cout << e << endl;
    //return 0;
}
