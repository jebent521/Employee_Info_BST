/*
Names:  Johan Ebent and Evan Whitmer
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
class Employee
{
public:
    Employee(int salary, string firstName, string lastName, string jobTitle) : salary(salary),
                                                                               firstName(firstName),
                                                                               lastName(lastName),
                                                                               jobTitle(jobTitle) {}

    // tree relations
    Employee *parent = nullptr;
    Employee *left = nullptr;
    Employee *right = nullptr;

    // tree priority (doubling as satellite data)
    int salary;

    // satellite data
    string firstName;
    string lastName;
    string jobTitle;

    friend ostream &operator<<(ostream &os, const Employee &obj)
    {
        os << obj.firstName << " " << obj.lastName << " - " << obj.jobTitle << " ($" << obj.salary << ")";
        return os;
    }

    bool operator<(const Employee &other) const
    {
        return salary < other.salary;
    }

    bool operator<=(const Employee &other) const
    {
        return salary <= other.salary;
    }

    bool operator>(const Employee &other) const
    {
        return salary > other.salary;
    }

    bool operator>=(const Employee &other) const
    {
        return salary >= other.salary;
    }
};

/* A binary search tree that contains Employees */
class EmployeeBST
{
private:
    Employee *root = nullptr;

    Employee *search(Employee *x, int salary)
    {
        if (x == nullptr || x->salary == salary)
            return root;
        if (x->salary < salary)
            return search(x->left, salary);
        return search(x->right, salary);
    }

public:
    void insert(Employee employee)
    {
    }

    Employee remove(int salary)
    {
    }

    vector<Employee> searchRange(int min, int max)
    {
    }
};

/*
The UI class contains functions relating to the UI of the application.
They do not need to be wrapped in a class, but since they are logically
associated, this groups them under the same namespace.
*/
class UI
{
public:
    void mainMenu()
    {
    }

    void addEmployee()
    {
    }

    void deleteEmployee()
    {
    }

    void searchEmployee()
    {
    }

    void quit()
    {
    }
};

void initializeDummyData(EmployeeBST *bst)
{
}

UI ui;
EmployeeBST bst;

int main()
{
    cout << Employee(1, "John", "Smith", "Plumber") << endl;
}
