/*
Names:  Jonah Ebent and Evan Whitmer
Class:  CSC-344-A   Algorithms and Complexity
Date:   October 19, 2024

Description:
Write a program in C++ that implements a red-black tree, where at each node
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

RB Tree Properties
- Node Color: Each node is either red or black.
- Root Property: The root of the tree is always black.
- Red Property: Red nodes cannot have red children (no two consecutive red nodes on any path).
- Black Property: Every path from a node to its descendant null nodes (leaves) has the same number of black nodes.
- Leaf Property: All leaves (NIL nodes) are black.

Much of the implementation was taken from https://www.geeksforgeeks.org/introduction-to-red-black-tree/
*/
#include <iostream>
#include <functional>
#include <random>
#include <string>
#include <vector>

using namespace std;

struct Employee {
    Employee() {};
    Employee(string firstName, string lastName, string jobTitle, int salary) :
        firstName(firstName),
        lastName(lastName),
        jobTitle(jobTitle),
        salary(salary) {}

    int salary{};
    string firstName{};
    string lastName{};
    string jobTitle{};

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

    bool operator==(const Employee& other) const {
        return salary == other.salary
            && firstName == other.firstName
            && lastName == other.lastName
            && jobTitle == other.jobTitle;
    }

    bool operator!=(const Employee& other) const {
        return !(*this == other);
    }

    friend ostream& operator<<(ostream& os, const Employee& obj) {
        os << obj.firstName << " " << obj.lastName << ", " << obj.jobTitle << " ($" << obj.salary << ")";
        return os;
    }
};


class EmployeeRBT {
    enum color {red, black};

    struct node {
        node(Employee data) : 
            data(data),
            left(nullptr),
            right(nullptr),
            p(nullptr),
            color(red) {}

        Employee data;
        node* left;
        node* right;
        node* p;
        color color;

        node* sibling() {
            if (p == nullptr) return nullptr;
            if (this == p->left) return p->right;
            else return p->left;
        }

        node* uncle() {
            if (p == nullptr) return nullptr;
            return p->sibling();
        }

        bool operator==(const node& other) const {
            return data == other.data
                && left == other.left
                && right == other.right
                && p == other.p
                && color == other.color;
        }
    };

    node* root;
    bool ll;    // flag for left-rotation
    bool lr;    // flag for left-, then right-rotation
    bool rl;    // flag for right-, then left-rotation
    bool rr;    // flag for right-rotation

    // Function to perform Left Rotation
    node* rotateLeft(node* t) {
        node* x = t->right;
        node* y = x->left;
        x->left = t;
        t->right = y;
        t->p = x;
        if (y != nullptr)
            y->p = t;
        return x;
    }

    // Function to perform Right Rotation
    node* rotateRight(node* t) {
        node* x = t->left;
        node* y = x->right;
        x->right = t;
        t->left = y;
        t->p = x;
        if (y != nullptr)
            y->p = t;
        return x;
    }

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

    node* insert(node* t, Employee data) {
        bool redViolation = false; // Flag to indicate red violations

        if (t == nullptr) return new node(data);
        // recurse down the tree
        else if (data < t->data) {
            t->left = insert(t->left, data);    // insert red node
            t->left->p = t;
            if (t != this->root) {
                // check for red-violation
                if (t->color == red && t->left->color == red)
                    redViolation = true;
            }
        }
        else {
            t->right = insert(t->right, data);  // insert red node
            t->right->p = t;
            if (t != this->root) {
                // check for red violation
                if (t->color == red && t->right->color == red)
                    redViolation = true;
            }
        }

        // Perform rotations
        if (ll) {       // left-rotation
            t = rotateLeft(t);
            // recolor t and right child accordingly
            t->color = black;
            t->left->color = red;
            ll = false;
        }
        else if (rr) {  // right-rotation
            t = rotateRight(t);
            // recolor t and right child accordingly
            t->color = black;
            t->right->color = red;
            rr = false;
        }
        else if (rl) {  // right-, then left-rotation
            t->right = rotateRight(t->right);
            t->right->p = t;
            t = rotateLeft(t);
            // recolor t and left child accordingly
            t->color = black;
            t->left->color = red;
            rl = false;
        }
        else if (lr) {  // left-, then right-rotation
            t->left = rotateLeft(t->left);
            t->left->p = t;
            t = rotateRight(t);
            // recolor t and right child accordingly
            t->color = black;
            t->right->color = red;
            lr = false;
        }

        // Handle red violations
        if (redViolation) {
            if (t->p->right == t) {     // t is a right child
                // if sibling is black
                if (t->sibling() == nullptr || t->sibling()->color == black) {
                    // if left-child is red
                    if (t->left != nullptr && t->left->color == red)
                        rl = true;  // right, then left rotate
                    // if right child is red
                    else if (t->right != nullptr && t->right->color == red)
                        ll = true;  // left-rotate
                }
                // sibling is red
                else {
                    // recolor t and sibling black
                    t->sibling()->color = black;
                    t->color = black;
                    // if parent isn't root, color it red
                    if (t->p != this->root)
                        t->p->color = red;
                }
            }
            else { // t is a left child
                // if sibling is black
                if (t->sibling() == nullptr || t->sibling()->color == black) {
                    // if left-child is null
                    if (t->left != nullptr && t->left->color == red)
                        rr = true;  // right-rotate
                    else if (t->right != nullptr && t->right->color == red)
                        lr = true;  // left, then right rotate
                }
                // sibling is red
                else {
                    // recolor t and sibling black
                    t->sibling()->color = black;
                    t->color = black;
                    // if parent isn't root, color it red
                    if (t->p != this->root)
                        t->p->color = red;
                }
            }
            redViolation = false;
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

    node* remove(Employee x, node* t) {
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

    void inorderConditional(node* t, function<bool(node*)> condition) {
        if (t == nullptr) return;
        inorderConditional(t->left, condition);
        if (condition(t)) cout << t->data << endl;
        inorderConditional(t->right, condition);
    }

    node* find(node* t, int x) {
        if (t == nullptr) return nullptr;
        else if (x < t->data.salary) return find(t->left, x);
        else if (x > t->data.salary) return find(t->right, x);
        else return t;
    }

public:
    EmployeeRBT() :
        root(nullptr),
        ll(false),
        lr(false),
        rl(false),
        rr(false)
    {}

    ~EmployeeRBT() {
        root = makeEmpty(root);
    }

    void insert(Employee data) {
        if (root == nullptr) {
            root = new node(data);
            root->color = black;
        }
        else
            root = insert(root, data);
    }

    void remove(Employee x) {
        remove(x, root);
    }

    void display() {
        inorder(root);
        cout << endl;
    }

    Employee* search(int x) {
        node* result = find(root, x);
        if (result == nullptr) return nullptr;
        return &result->data;
    }

    vector<Employee> findAll(int x) {
        vector<Employee> out;
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
    EmployeeRBT* employees = nullptr;

    bool isBetween(int num, int* min, int* max) {
        if (min != nullptr && num < *min) return false;
        if (max != nullptr && num > *max) return false;
        return true;
    }

    int inputInteger(int* min, int* max) {
        int intput;
        string input;
        do {
            cout << "  Input: ";      // prompt user for input
            getline(cin, input);

            try { intput = stoi(input); }
            catch (const std::invalid_argument&) {
                cout << "  Invalid input. Please enter a number between " << *min << " and " << *max << " (inclusive)." << endl;
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
        return Employee(firstName, lastName, jobTitle, salary);
    }

public:
    UI(EmployeeRBT* rbt) : employees(rbt) {}

    void mainMenu() {
        cout << "----------------------------------" << endl;
        cout << "What would you like to do?" << endl;
        cout << "  1) Add an employee" << endl;
        cout << "  2) Delete an employee" << endl;
        cout << "  3) Search for employees" << endl;
        cout << "  4) Quit" << endl;
        cout << "----------------------------------" << endl;
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
        vector<Employee> v = employees->findAll(salary);
        Employee z;
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
                cout << "  " << i + 1 << ") " << v.at(i) << endl;
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

static string randStr(int length) {
    // Define the list of possible characters
    const string CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    // Create a random number generator
    random_device rd;
    mt19937 generator(rd());

    // Create a distribution to uniformly select from all
    // characters
    uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    // Generate the random string
    string random_string;
    for (int i = 0; i < length; ++i) {
        random_string += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

static int randInt(int min, int max) {
    return min + (rand() % (max - min + 1));
}

/* Inserts 10,000 employees into the BST, with random data
and salaries ranging from 30,000 to 200,000.*/
static void initializeDummyData(EmployeeRBT& bst) {
    const string CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    srand((unsigned int)time(NULL));
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> salaryDist(30'000, 200'000);

    for (int i = 0; i < 10000; i++) {
        Employee e(randStr(8), randStr(8), randStr(8), salaryDist(gen));
        bst.insert(e);
    }
}

int main() {
    EmployeeRBT bst;
    UI ui = UI(&bst);
    initializeDummyData(bst);
    cout << "Welcome to the employee \"Database\"" << endl;
    while (true) ui.mainMenu();
    return 0;
}
