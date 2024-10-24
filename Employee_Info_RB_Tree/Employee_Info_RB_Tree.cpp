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
            parent(nullptr),
            color(red) {}

        Employee data;
        node* left;
        node* right;
        node* parent;
        color color;

        bool operator==(const node& other) const {
            return data == other.data
                && left == other.left
                && right == other.right
                && parent == other.parent
                && color == other.color;
        }
    };

    node* root;
    node* NIL;

    // Function to perform Left Rotation
    void leftRotate(node* x) {
        node* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Function to perform Left Rotation
    void rightRotate(node* x) {
        node* y = x->left;
        x->left = y->right;
        if (y->right != NIL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void insertFixup(node* n) {
        node* u;
        while (n->parent->color == red) {   // while n's parent is red (i.e. a red-violation exists)
            if (n->parent == n->parent->parent->right) {    // if n's parent is a right child
                u = n->parent->parent->left;                    // u = n's uncle
                if (u->color == red) {                          // case 1: uncle is red
                    u->color = black;                               // color p and u black
                    n->parent->color = black;
                    n->parent->parent->color = black;               // color grandparent red
                    n = n->parent->parent;                          // bubble red violation to grandparent
                }
                else {                                      // cases 2 and 3: uncle is black
                    if (n == n->parent->left) {                 // case 2: n makes a triangle with its parent and grandparent
                        n = n->parent;                              // set n to its parent
                        rightRotate(n);                             // right-rotate on n
                    }
                    n->parent->color = black;                   // case 3: n makes a line with its parent and grandparent
                    n->parent->parent->color = red;                 // color parent black, grandparent red
                    leftRotate(n->parent->parent);                  // left-rotate on grandparent
                }
            }
            else {  // n's parent is a left child (symmetrical with above code)
                u = n->parent->parent->right;

                if (u->color == red) {
                    u->color = black;
                    n->parent->color = black;
                    n->parent->parent->color = red;
                    n = n->parent->parent;
                }
                else {
                    if (n == n->parent->right) {
                        n = n->parent;
                        leftRotate(n);
                    }
                    n->parent->color = black;
                    n->parent->parent->color = red;
                    rightRotate(n->parent->parent);
                }
            }
            if (n == root) {
                break;
            }
        }
        root->color = black;
    }

    void removeFixup(node* x) {
        node* s;
        while (x != root && x->color == black) {
            if (x == x->parent->left) { // x is a left child
                s = x->parent->right;       // s is x's sibling
                if (s->color == red) {      // case 1: s is red
                    s->color = black;           // color s black
                    x->parent->color = red;     // color parent red
                    leftRotate(x->parent);      // left-rotate
                    s = x->parent->right;       // reassign s to x's new sibling
                }

                if (s->left->color == black && s->right->color == black) {  // case 2: both children are black
                    s->color = red;     // color s red
                    x = x->parent;      // bubble up
                }
                else {
                    if (s->right->color == black) {     // case 3: triangle, turn into a case 4
                        s->left->color = black;             // color s's left child black
                        s->color = red;                     // color s red
                        rightRotate(s);                     // right-rotate
                        s = x->parent->right;               // reassign s to x's new sibling
                    }
                                                // case 4: line
                    s->color = x->parent->color;    // set x's color to its parent        
                    x->parent->color = black;       // color parent black
                    s->right->color = black;        // color sibling's right child black
                    leftRotate(x->parent);          // left-rotate on parent
                    x = root;                       // bubble to top
                }
            }
            else {  // x is a right child (symmetrical to above code)
                s = x->parent->left;
                if (s->color == red) {
                    s->color = black;
                    x->parent->color = red;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if (s->left->color == black && s->right->color == black) {
                    s->color = red;
                    x = x->parent;
                }
                else {
                    if (s->left->color == black) {
                        s->right->color = black;
                        s->color = red;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = black;
                    s->left->color = black;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = black;
    }

    void transplant(node* u, node* v) {
        if (u->parent == nullptr) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    node* find(node* t, int x) {
        if (t == NIL) return nullptr;
        else if (x < t->data.salary) return find(t->left, x);
        else if (x > t->data.salary) return find(t->right, x);
        else return t;
    }

    node* minimum(node* t) {
        if (t == nullptr) return nullptr;
        else if (t->left == nullptr) return t;
        else return minimum(t->left);
    }

    void inorder(node* t) {
        if (t == NIL)
            return;
        inorder(t->left);
        cout << t->data << endl;
        inorder(t->right);
    }

    void inorderConditional(node* t, function<bool(node*)> condition) {
        if (t == NIL) return;
        inorderConditional(t->left, condition);
        if (condition(t)) cout << t->data << endl;
        inorderConditional(t->right, condition);
    }

public:
    EmployeeRBT() {
        Employee nullEmployee;
        NIL = new node(nullEmployee);
        NIL->color = black;
        NIL->left = NIL->right = NIL;
        root = NIL;
    }

    void insert(Employee e) {
        node* n = new node(e);  // create a new node
        n->left = n->right = NIL;

        node* y = nullptr;  // parent of current node
        node* x = root;     // current node

        while (x != NIL) {  // traverse down the tree
            y = x;
            if (n->data < x->data) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        n->parent = y;      // set n's parent to y
        if (y == nullptr) { // if it's null, set root to n
            root = n;
        }
        else if (n->data < y->data) {
            y->left = n;    // if n < y, make n its left child
        }
        else {
            y->right = n;   // otherwise, make it its right child
        }

        if (n->parent == nullptr) { // insertFixup doesn't check against parents
            n->color = black;           // if n is root, color black and exit
            return;
        }

        if (n->parent->parent == nullptr) { // if n's parent is root (adding a red won't screw it up)
            return;                             // exit
        }

        insertFixup(n);
    }

    void remove(Employee data) {
        node* z = NIL;
        node* x, * y;

        // search for node to delete
        node* n = root;
        while (n != NIL) {
            if (n->data == data) {
                z = n;
                break;
            }
            else if (n->data < data) {
                n = n->right;
            }
            else {
                n = n->left;
            }
        }
        if (z == NIL) return;   // couldn't find node

        y = z;
        color original_color = y->color;    // save original color
        if (z->left == NIL) {       // if left child is null, transplant with right child
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == NIL) { // if right child is null, transplant with left child
            x = z->left;
            transplant(z, z->left);
        }
        else {  // neither children null, replace with successor
            y = minimum(z->right);  // y is z's successor (save its color)
            original_color = y->color;
            x = y->right;           // x is y's right child
            if (y->parent == z) {   // if y is a child of z
                x->parent = y;          // set x's parent to y
            }
            else {                  // y isn't z's immediate child
                transplant(y, y->right);// transplant
                y->right = z->right;    // fix relationships
                y->right->parent = y;
            }

            transplant(z, y);       // replace z with y
            y->left = z->left;      // fix relationships
            y->left->parent = y;
            y->color = z->color;    // set y's color to z's color
        }
        delete z;
        if (original_color == black) {
            removeFixup(x);         // if original color is black, fixup
        }
    }

    Employee* find(int x) {
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
            y = minimum(y->right);
            if (y == nullptr || y->data.salary != x) break;
            out.push_back(y->data);
        }
        return out;
    }

    void display() {
        inorder(root);
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
    EmployeeRBT rbt;
    UI ui = UI(&rbt);
    cout << "~~~ Inserting Evan, Thor, and Jonah ~~~" << endl;
    rbt.insert(Employee("evan", "whitmer", "frontend developer", 199999));
    rbt.insert(Employee("jonah", "ebent", "retired", 200000));
    rbt.insert(Employee("thor", "pilegaard", "code monkey", 199998));
    rbt.display();
    cout << endl;
    cout << "~~~ Displaying all employees with salary in range [199999, 200000] ~~~" << endl;
    rbt.printInRange(199999, 200000);
    cout << endl;
    cout << "~~~ Searching for the first employee with salary $199999 ~~~" << endl;
    cout << *rbt.find(199999) << endl;
    cout << endl;
    cout << "~~~ Removing jonah ~~~" << endl;
    rbt.remove(Employee("jonah", "ebent", "retired", 200000));
    rbt.display();
    cout << endl;
    initializeDummyData(rbt);
    cout << "Welcome to the employee \"Database\"" << endl;
    while (true) ui.mainMenu();
    return 0;
}
