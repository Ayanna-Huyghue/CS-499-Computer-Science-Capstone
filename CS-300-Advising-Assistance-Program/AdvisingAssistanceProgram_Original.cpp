#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

// Class for a node in the binary search tree
class BSTNode {
public:
    std::string courseNumber; // Unique identifier for the course
    std::string name; // Name of the course
    std::vector<std::string> prerequisites; // List of prerequisites for the course
    BSTNode* left, * right; // Pointers to left and right child nodes

    // Constructor to initialize a node with course information
    BSTNode(std::string cn, std::string n, std::vector<std::string> pr)
        : courseNumber(cn), name(n), prerequisites(pr), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class for managing course data
class BST {
public:
    BSTNode* root; // Root node of the BST

    // Constructor initializes an empty BST
    BST() : root(nullptr) {}

    // Private helper function to insert a new node into the BST
    BSTNode* insert(BSTNode* node, std::string courseNumber, std::string name, std::vector<std::string> prerequisites) {
        // Base case: Found a position to insert the new node
        if (node == nullptr) {
            return new BSTNode(courseNumber, name, prerequisites);
        }

        // Recursive case: Traverse the tree to find the insertion point
        if (courseNumber < node->courseNumber) {
            node->left = insert(node->left, courseNumber, name, prerequisites);
        }
        else if (courseNumber > node->courseNumber) {
            node->right = insert(node->right, courseNumber, name, prerequisites);
        }

        return node; // Return the (possibly new) root of the subtree
    }

    // Public function to insert a new course into the BST
    void insert(std::string courseNumber, std::string name, std::vector<std::string> prerequisites) {
        root = insert(root, courseNumber, name, prerequisites);
    }

    // Private helper function for inorder traversal of the BST
    void inorder(BSTNode* node) {
        if (node != nullptr) {
            inorder(node->left); // Visit left subtree
            std::cout << node->courseNumber << ", " << node->name << std::endl; // Process current node
            inorder(node->right); // Visit right subtree
        }
    }

    // Public function to perform inorder traversal of the BST and print courses
    void inorder() {
        inorder(root);
    }

    // Private helper function to search for a course in the BST
    BSTNode* search(BSTNode* node, std::string courseNumber) {
        // Base case: Found the course or reached end of tree
        if (node == nullptr || node->courseNumber == courseNumber) return node;

        // Recursive case: Traverse the tree to find the course
        if (courseNumber < node->courseNumber) return search(node->left, courseNumber);
        else return search(node->right, courseNumber);
    }

    // Public function to search for a course in the BST
    BSTNode* search(std::string courseNumber) {
        return search(root, courseNumber);
    }
};

// Main program
int main() {
    BST courseTree;
    std::string fileName, line, courseNumber, name;
    std::vector<std::string> prerequisites;
    int choice;

    std::cout << "Welcome to the ABCU Course Planner.\n";

    // Declare variables outside the switch statement
    std::ifstream file; // Moved the declaration outside of the switch
    BSTNode* found = nullptr; // Initialize with nullptr

    while (true) {
        std::cout << "1. Load Data Structure\n2. Print Course List\n3. Print Course\n9. Exit\nWhat would you like to do? ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore newline char left in the input stream

        switch (choice) {
        case 1:
            std::cout << "Enter filename: ";
            std::getline(std::cin, fileName);

            file.open(fileName); // Now we just open the file using the previously declared variable

            if (!file) {
                std::cout << "Could not open file.\n";
                file.close(); // Make sure to close the file in case it's in an odd state
                continue;
            }

            while (std::getline(file, line)) {
                std::istringstream iss(line);
                prerequisites.clear();
                std::getline(iss, courseNumber, ',');
                std::getline(iss, name, ',');
                std::string prereq;

                while (std::getline(iss, prereq, ',')) {
                    prerequisites.push_back(prereq);
                }

                courseTree.insert(courseNumber, name, prerequisites);
            }
            file.close(); // Close the file after reading
            std::cout << "Data loaded successfully.\n";
            break;
        case 2:
            courseTree.inorder();
            break;
        case 3:
            std::cout << "Enter course number: ";
            std::cin >> courseNumber;
            found = courseTree.search(courseNumber); // Assign to the previously declared variable

            if (found) {
                std::cout << "Course: " << found->name << "\nPrerequisites: ";
                for (const auto& prereq : found->prerequisites) {
                    std::cout << prereq << " ";
                }
                std::cout << std::endl;
            }
            else {
                std::cout << "Course not found.\n";
            }
            break;
        case 9:
            std::cout << "Exiting program.\n";
            return 0;
        default:
            std::cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}

