
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <chrono>
#include <cctype>

// Node for the self-balancing AVL tree
class AVLNode {
public:
    std::string courseNumber;
    std::string name;
    std::vector<std::string> prerequisites;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const std::string& cn, const std::string& n,
            const std::vector<std::string>& pr)
        : courseNumber(cn), name(n), prerequisites(pr),
          left(nullptr), right(nullptr), height(1) {}
};

// AVL tree for managing course information
class AVLTree {
private:
    AVLNode* root;
    int rotationCount;

    int getHeight(AVLNode* node) const {
        return node == nullptr ? 0 : node->height;
    }

    int getBalance(AVLNode* node) const {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(AVLNode* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(
                getHeight(node->left),
                getHeight(node->right));
        }
    }

    // Right rotation restores balance in a left-heavy subtree
    AVLNode* rotateRight(AVLNode* node) {
        AVLNode* newRoot = node->left;
        AVLNode* subtree = newRoot->right;

        newRoot->right = node;
        node->left = subtree;

        updateHeight(node);
        updateHeight(newRoot);
        ++rotationCount;

        return newRoot;
    }

    // Left rotation restores balance in a right-heavy subtree
    AVLNode* rotateLeft(AVLNode* node) {
        AVLNode* newRoot = node->right;
        AVLNode* subtree = newRoot->left;

        newRoot->left = node;
        node->right = subtree;

        updateHeight(node);
        updateHeight(newRoot);
        ++rotationCount;

        return newRoot;
    }

    // Insert a course and rebalance the tree when necessary
    AVLNode* insert(AVLNode* node, const std::string& courseNumber,
                    const std::string& name,
                    const std::vector<std::string>& prerequisites) {
        if (node == nullptr) {
            return new AVLNode(courseNumber, name, prerequisites);
        }

        if (courseNumber < node->courseNumber) {
            node->left = insert(node->left, courseNumber,
                                name, prerequisites);
        }
        else if (courseNumber > node->courseNumber) {
            node->right = insert(node->right, courseNumber,
                                 name, prerequisites);
        }
        else {
            // Update existing courses instead of creating duplicates
            node->name = name;
            node->prerequisites = prerequisites;
            return node;
        }

        updateHeight(node);
        int balance = getBalance(node);

        // Left-left case
        if (balance > 1 &&
            courseNumber < node->left->courseNumber) {
            return rotateRight(node);
        }

        // Right-right case
        if (balance < -1 &&
            courseNumber > node->right->courseNumber) {
            return rotateLeft(node);
        }

        // Left-right case
        if (balance > 1 &&
            courseNumber > node->left->courseNumber) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right-left case
        if (balance < -1 &&
            courseNumber < node->right->courseNumber) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    void inorder(AVLNode* node) const {
        if (node == nullptr) return;

        inorder(node->left);
        std::cout << node->courseNumber << ", "
                  << node->name << '\n';
        inorder(node->right);
    }

    AVLNode* search(AVLNode* node,
                    const std::string& courseNumber) const {
        while (node != nullptr) {
            if (courseNumber == node->courseNumber) {
                return node;
            }

            if (courseNumber < node->courseNumber) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        return nullptr;
    }

    void clear(AVLNode* node) {
        if (node == nullptr) return;

        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr), rotationCount(0) {}

    ~AVLTree() {
        clear(root);
    }

    // Prevent accidental copying of tree-owned pointers
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    void insert(const std::string& courseNumber,
                const std::string& name,
                const std::vector<std::string>& prerequisites) {
        root = insert(root, courseNumber, name, prerequisites);
    }

    void inorder() const {
        if (root == nullptr) {
            std::cout << "No course data loaded.\n";
            return;
        }

        inorder(root);
    }

    AVLNode* search(const std::string& courseNumber) const {
        return search(root, courseNumber);
    }

    int height() const {
        return getHeight(root);
    }

    int rotations() const {
        return rotationCount;
    }

    void reset() {
        clear(root);
        root = nullptr;
        rotationCount = 0;
    }
};

// Remove extra spaces from CSV fields and user input
std::string trim(const std::string& value) {
    const std::string spaces = " \t\r\n";
    size_t first = value.find_first_not_of(spaces);

    if (first == std::string::npos) return "";

    size_t last = value.find_last_not_of(spaces);
    return value.substr(first, last - first + 1);
}

// Make course searches case-insensitive for user input
std::string normalizeCourseNumber(std::string value) {
    value = trim(value);

    for (char& character : value) {
        character = static_cast<char>(
            std::toupper(static_cast<unsigned char>(character)));
    }

    return value;
}

// Load and validate course records before replacing existing data
bool loadCourses(AVLTree& courseTree, const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Could not open file.\n";
        return false;
    }

    AVLTree newTree;
    std::string line;
    int loaded = 0;
    int skipped = 0;

    auto start = std::chrono::steady_clock::now();

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;

        std::istringstream iss(line);
        std::string courseNumber;
        std::string name;
        std::string prerequisite;
        std::vector<std::string> prerequisites;

        std::getline(iss, courseNumber, ',');
        std::getline(iss, name, ',');

        courseNumber = normalizeCourseNumber(courseNumber);
        name = trim(name);

        if (courseNumber.empty() || name.empty()) {
            ++skipped;
            continue;
        }

        while (std::getline(iss, prerequisite, ',')) {
            prerequisite = normalizeCourseNumber(prerequisite);

            if (!prerequisite.empty()) {
                prerequisites.push_back(prerequisite);
            }
        }

        newTree.insert(courseNumber, name, prerequisites);
        ++loaded;
    }

    if (file.bad()) {
        std::cout << "An error occurred while reading the file.\n";
        return false;
    }

    auto finish = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<double, std::milli>(
        finish - start).count();

    // Replace old records only after a successful read
    courseTree.reset();

    // Reopen the validated file to populate the working tree
    file.clear();
    file.seekg(0);

    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;

        std::istringstream iss(line);
        std::string courseNumber;
        std::string name;
        std::string prerequisite;
        std::vector<std::string> prerequisites;

        std::getline(iss, courseNumber, ',');
        std::getline(iss, name, ',');

        courseNumber = normalizeCourseNumber(courseNumber);
        name = trim(name);

        if (courseNumber.empty() || name.empty()) continue;

        while (std::getline(iss, prerequisite, ',')) {
            prerequisite = normalizeCourseNumber(prerequisite);

            if (!prerequisite.empty()) {
                prerequisites.push_back(prerequisite);
            }
        }

        courseTree.insert(courseNumber, name, prerequisites);
    }

    std::cout << "Data loaded successfully.\n";
    std::cout << "Valid records: " << loaded << '\n';
    std::cout << "Skipped records: " << skipped << '\n';
    std::cout << "AVL tree height: " << courseTree.height() << '\n';
    std::cout << "AVL rotations: " << courseTree.rotations() << '\n';
    std::cout << "Validation and initial build time: "
              << elapsed << " ms\n";

    return true;
}

// Main program
int main() {
    AVLTree courseTree;
    std::string fileName;
    std::string courseNumber;
    std::string input;

    std::cout << "Welcome to the ABCU Course Planner.\n";

    while (true) {
        std::cout << "\n1. Load Data Structure\n"
                  << "2. Print Course List\n"
                  << "3. Print Course\n"
                  << "9. Exit\n"
                  << "What would you like to do? ";

        if (!std::getline(std::cin, input)) {
            std::cout << "\nExiting program.\n";
            return 0;
        }

        input = trim(input);

        // Reject invalid entries without leaving cin in a failed state
        if (input != "1" && input != "2" &&
            input != "3" && input != "9") {
            std::cout << "Invalid option. Please try again.\n";
            continue;
        }

        if (input == "1") {
            std::cout << "Enter filename: ";

            if (!std::getline(std::cin, fileName)) {
                std::cout << "\nExiting program.\n";
                return 0;
            }

            fileName = trim(fileName);
            loadCourses(courseTree, fileName);
        }
        else if (input == "2") {
            courseTree.inorder();
        }
        else if (input == "3") {
            std::cout << "Enter course number: ";

            if (!std::getline(std::cin, courseNumber)) {
                std::cout << "\nExiting program.\n";
                return 0;
            }

            courseNumber = normalizeCourseNumber(courseNumber);
            AVLNode* found = courseTree.search(courseNumber);

            if (found != nullptr) {
                std::cout << "Course: " << found->name
                          << "\nPrerequisites: ";

                if (found->prerequisites.empty()) {
                    std::cout << "None";
                }
                else {
                    for (const auto& prerequisite :
                         found->prerequisites) {
                        std::cout << prerequisite << " ";
                    }
                }

                std::cout << '\n';
            }
            else {
                std::cout << "Course not found.\n";
            }
        }
        else if (input == "9") {
            std::cout << "Exiting program.\n";
            return 0;
        }
    }
}
