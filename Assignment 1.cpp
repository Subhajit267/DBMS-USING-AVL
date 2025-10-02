/*
PERSON DATABASE MANAGEMENT SYSTEM
File: /home/subhajit/Desktop/Databases/database2025.txt(default), if needed one can work with a different file as cmdline argument
Description: Balanced Binary Search Tree (AVL) implementation for storing person records
Features: Add, search, delete, update, and display person records with automatic tree balancing 
Author: Subhajit Halder
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

// Structure to store all personal information
struct Person {
    string lastName;      // last name
    string firstName;     // first name  
    string state;         // State of residence
    string zipCode;       // Postal zip code
    int birthYear;        // YOB
    int birthMonth;       // MOB
    int birthDay;         // DOB
    string password;      // Acc PWD
    double balance;       // Acc Bal
    string ssn;           // Social Security Number
    
    // Constructor to create Person from data tokens
    Person(string last, string first, string st, string zip, 
           int year, int month, int day, string pwd, double bal, string social) {
        lastName = last;
        firstName = first;
        state = st;
        zipCode = zip;
        birthYear = year;
        birthMonth = month;
        birthDay = day;
        password = pwd;
        balance = bal;
        ssn = social;
    }
    
    // case-sensitive string comparison
    int compareStrings(const string& str1, const string& str2) const {
        // Compare character by character (case-sensitive)
        size_t minLength = str1.length();
        if (str2.length() < minLength) minLength = str2.length();
        
        for (size_t i = 0; i < minLength; i++) {
            if (str1[i] != str2[i]) {
                if (str1[i] < str2[i]) return -1;
                else return 1;
            }
        }
        
        // If common characters match, shorter string comes first
        if (str1.length() < str2.length()) return -1;
        if (str1.length() > str2.length()) return 1;
        
        return 0; // Strings are equal
    }
    
    // Compare two persons by last name, then first name using comparison
    bool isLessThan(const Person& other) const {
        // First compare last names using custom comparison
        int lastCompare = compareStrings(lastName, other.lastName);
        if (lastCompare < 0) return true;
        if (lastCompare > 0) return false;
        
        // If last names match, compare first names using comparison
        int firstCompare = compareStrings(firstName, other.firstName);
        return firstCompare < 0;
    }
    
    // Check if two persons are identical using comparison
    bool isEqualTo(const Person& other) const {
        int lastCompare = compareStrings(lastName, other.lastName);
        if (lastCompare != 0) return false;
        
        int firstCompare = compareStrings(firstName, other.firstName);
        return firstCompare == 0;
    }
};

// Tree node structure for binary search tree
struct TreeNode {
    Person data;          // The person data stored in this node
    TreeNode* left;       // Pointer to left child node
    TreeNode* right;      // Pointer to right child node
    int height;           // Height of node for balancing
    
    // Constructor to create new tree node
    TreeNode(Person p) : data(p), left(nullptr), right(nullptr), height(1) {}
};

// Main database class that manages all operations
class PersonDatabase {
private:
    TreeNode* root;  // Root node of our binary search tree
    
    // Get height of a node (returns 0 for null nodes)
    int getNodeHeight(TreeNode* node) {
        if (node == nullptr) return 0;
        return node->height;
    }
    
    // Calculate balance factor (left height - right height)
    int getBalanceFactor(TreeNode* node) {
        if (node == nullptr) return 0;
        return getNodeHeight(node->left) - getNodeHeight(node->right);
    }
    
    // Update height of node based on children's heights
    void updateNodeHeight(TreeNode* node) {
        if (node == nullptr) return;
        
        int leftHeight = getNodeHeight(node->left);
        int rightHeight = getNodeHeight(node->right);
        
        // Height is 1 + tallest child height
        if (leftHeight > rightHeight) 
            node->height = 1 + leftHeight;
        else
            node->height = 1 + rightHeight;
    }
    
    // Rotate subtree right to fix left-heavy imbalance
    TreeNode* rotateRight(TreeNode* y) {
        if (y == nullptr) return nullptr;
        
        TreeNode* x = y->left;
        if (x == nullptr) return y;
        
        TreeNode* T2 = x->right;
        
        // Perform rotation
        x->right = y;
        y->left = T2;
        
        // Update heights after rotation
        updateNodeHeight(y);
        updateNodeHeight(x);
        
        return x;  // New root of this subtree
    }
    
    // Rotate subtree left to fix right-heavy imbalance  
    TreeNode* rotateLeft(TreeNode* x) {
        if (x == nullptr) return nullptr;
        
        TreeNode* y = x->right;
        if (y == nullptr) return x;
        
        TreeNode* T2 = y->left;
        
        // Perform rotation
        y->left = x;
        x->right = T2;
        
        // Update heights after rotation
        updateNodeHeight(x);
        updateNodeHeight(y);
        
        return y;  // New root of this subtree
    }
    
    // Balance a node after insertion or deletion
    TreeNode* balanceNode(TreeNode* node) {
        if (node == nullptr) return nullptr;
        
        // Update height of current node
        updateNodeHeight(node);
        
        // Check balance factor to see if tree is unbalanced
        int balance = getBalanceFactor(node);
        
        // Left Left Case - single right rotation needed
        if (balance > 1 && getBalanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }
        
        // Left Right Case - left rotation then right rotation
        if (balance > 1 && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        
        // Right Right Case - single left rotation needed
        if (balance < -1 && getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }
        
        // Right Left Case - right rotation then left rotation
        if (balance < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        // Node is already balanced
        return node;
    }
    
    // Insert a new person into the tree
    TreeNode* insertPerson(TreeNode* node, const Person& p) {
        // Found empty spot - create new node here
        if (node == nullptr) {
            return new TreeNode(p);
        }
        
        // Compare to decide left or right subtree using custom comparison
        if (p.isLessThan(node->data)) {
            node->left = insertPerson(node->left, p);
        } else if (node->data.isLessThan(p)) {
            node->right = insertPerson(node->right, p);
        } else {
            // Person already exists - no duplicates allowed
            return node;
        }
        
        // Balance the tree after insertion
        return balanceNode(node);
    }
    
    // Custom case-sensitive string comparison for search operations
    int compareStrings(const string& str1, const string& str2) const {
        // Compare character by character (case-sensitive)
        size_t minLength = str1.length();
        if (str2.length() < minLength) minLength = str2.length();
        
        for (size_t i = 0; i < minLength; i++) {
            if (str1[i] != str2[i]) {
                if (str1[i] < str2[i]) return -1;
                else return 1;
            }
        }
        
        // If common characters match, shorter string comes first
        if (str1.length() < str2.length()) return -1;
        if (str1.length() > str2.length()) return 1;
        
        return 0; // Strings are equal
    }
    
    // Find a specific person in the tree using custom comparison
    TreeNode* findPerson(TreeNode* node, const string& first, const string& last) const {
        if (node == nullptr) return nullptr;  // Person not found
        
        // Compare last names first using custom comparison
        int lastCompare = compareStrings(last, node->data.lastName);
        
        if (lastCompare < 0) {
            return findPerson(node->left, first, last);
        }
        else if (lastCompare > 0) {
            return findPerson(node->right, first, last);
        }
        else {
            // Last names match, compare first names using custom comparison
            int firstCompare = compareStrings(first, node->data.firstName);
            
            if (firstCompare < 0) {
                return findPerson(node->left, first, last);
            }
            else if (firstCompare > 0) {
                return findPerson(node->right, first, last);
            }
            else {
                // Both first and last names match - found the person!
                return node;
            }
        }
    }
    
    // Find the smallest node in a subtree (leftmost node)
    TreeNode* findSmallestNode(TreeNode* node) {
        if (node == nullptr) return nullptr;
        
        TreeNode* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    
    // Delete a person from the tree using custom comparison
    TreeNode* deletePerson(TreeNode* node, const string& first, const string& last) {
        if (node == nullptr) return nullptr;
        
        // Compare last names first using custom comparison
        int lastCompare = compareStrings(last, node->data.lastName);
        
        // Search for the node to delete
        if (lastCompare < 0) {
            node->left = deletePerson(node->left, first, last);
        }
        else if (lastCompare > 0) {
            node->right = deletePerson(node->right, first, last);
        }
        else {
            // Last names match, compare first names using custom comparison
            int firstCompare = compareStrings(first, node->data.firstName);
            
            if (firstCompare < 0) {
                node->left = deletePerson(node->left, first, last);
            }
            else if (firstCompare > 0) {
                node->right = deletePerson(node->right, first, last);
            }
            else {
                // Found the node to delete
                
                // Case 1: Node has no children or one child
                if (node->left == nullptr || node->right == nullptr) {
                    TreeNode* temp = node->left;
                    if (temp == nullptr) temp = node->right;
                    
                    // No children case
                    if (temp == nullptr) {
                        delete node;
                        return nullptr;
                    } 
                    // One child case - replace with child
                    else {
                        // Copy the contents
                        node->data = temp->data;
                        node->left = temp->left;
                        node->right = temp->right;
                        node->height = temp->height;
                        delete temp;
                    }
                }
                // Case 2: Node has two children
                else {
                    // Find smallest node in right subtree
                    TreeNode* temp = findSmallestNode(node->right);
                    
                    // Copy data from smallest node
                    node->data = temp->data;
                    
                    // Delete the smallest node from right subtree
                    node->right = deletePerson(node->right, temp->data.firstName, temp->data.lastName);
                }
            }
        }
        
        // Balance the tree after deletion
        return balanceNode(node);
    }
    
    // Display all person information
    void displayPersonInfo(const Person& p) const {
        cout << p.lastName << " " << p.firstName << " " << p.state << " " 
             << p.zipCode << " " << p.birthYear << " " << p.birthMonth << " " 
             << p.birthDay << " " << p.password << " " << p.balance << " " 
             << p.ssn << endl;
    }
    
    // Display all persons in sorted order (in-order traversal)
    void displayAllPersons(TreeNode* node) const {
        if (node == nullptr) return;
        
        displayAllPersons(node->left);    // Process left subtree
        displayPersonInfo(node->data);    // Process current node
        displayAllPersons(node->right);   // Process right subtree
    }
    
    // Find oldest person by comparing birth dates
    void findOldestPerson(TreeNode* node, Person& oldest) const {
        if (node == nullptr) return;
        
        findOldestPerson(node->left, oldest);  // Check left subtree
        
        // Compare birth dates to find oldest
        if (node->data.birthYear < oldest.birthYear) {
            oldest = node->data;
        } else if (node->data.birthYear == oldest.birthYear) {
            if (node->data.birthMonth < oldest.birthMonth) {
                oldest = node->data;
            } else if (node->data.birthMonth == oldest.birthMonth) {
                if (node->data.birthDay < oldest.birthDay) {
                    oldest = node->data;
                }
            }
        }
        
        findOldestPerson(node->right, oldest); // Check right subtree
    }
    
    // Save all persons to file (in-order traversal)
    void saveToFile(TreeNode* node, ofstream& outFile) const {
        if (node == nullptr) return;
        
        saveToFile(node->left, outFile);  // Save left subtree
        
        // Write current person to file
        outFile << node->data.lastName << " " << node->data.firstName << " " 
                << node->data.state << " " << node->data.zipCode << " " 
                << node->data.birthYear << " " << node->data.birthMonth << " " 
                << node->data.birthDay << " " << node->data.password << " " 
                << node->data.balance << " " << node->data.ssn << endl;
        
        saveToFile(node->right, outFile); // Save right subtree
    }
    
    // Find all persons with given last name using custom comparison
    void findByLastName(TreeNode* node, const string& lastName) const {
        if (node == nullptr) return;
        
        // Compare last names using custom comparison
        int lastCompare = compareStrings(lastName, node->data.lastName);
        
        // If target last name is smaller, search left
        if (lastCompare < 0) {
            findByLastName(node->left, lastName);
        }
        // If target last name is larger, search right
        else if (lastCompare > 0) {
            findByLastName(node->right, lastName);
        }
        // Last names match - display and search both subtrees
        else {
            findByLastName(node->left, lastName);  // Check left for more matches
            displayPersonInfo(node->data);         // Display current match
            findByLastName(node->right, lastName); // Check right for more matches
        }
    }
    
    // Find all persons with given first name using custom comparison
    void findByFirstName(TreeNode* node, const string& firstName) const {
        if (node == nullptr) return;
        
        findByFirstName(node->left, firstName);  // Check left subtree
        
        // Compare first names using custom comparison
        int firstCompare = compareStrings(firstName, node->data.firstName);
        
        // Display if first name matches exactly
        if (firstCompare == 0) {
            displayPersonInfo(node->data);
        }
        
        findByFirstName(node->right, firstName); // Check right subtree
    }
    
    // Check if tree is balanced and get height
    void checkTreeBalance(TreeNode* node, bool& isBalanced, int& height) const {
        if (node == nullptr) {
            isBalanced = true;
            height = 0;
            return;
        }
        
        bool leftBalanced, rightBalanced;
        int leftHeight, rightHeight;
        
        // Check balance of left and right subtrees
        checkTreeBalance(node->left, leftBalanced, leftHeight);
        checkTreeBalance(node->right, rightBalanced, rightHeight);
        
        // Current node is balanced if both subtrees are balanced and height difference <= 1
        int diff = leftHeight - rightHeight;
        if (diff < 0) diff = -diff;
        
        isBalanced = leftBalanced && rightBalanced && (diff <= 1);
        
        // Height is 1 + tallest subtree height
        if (leftHeight > rightHeight) 
            height = 1 + leftHeight;
        else
            height = 1 + rightHeight;
    }
    
    // Free all memory used by the tree
    void deleteEntireTree(TreeNode* node) {
        if (node == nullptr) return;
        
        deleteEntireTree(node->left);   // Delete left subtree
        deleteEntireTree(node->right);  // Delete right subtree
        delete node;                    // Delete current node
    }

public:
    // Constructor - initialize empty tree
    PersonDatabase() : root(nullptr) {}
    
    // Load person data from file into tree
    bool loadFromFile(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "ERROR: Cannot open data file " << filename << endl;
            return false;
        }
        
        string line;
        int recordCount = 0;
        
        // Read file line by line
        while (getline(inputFile, line)) {
            // Skip empty lines
            if (line.empty()) continue;
            
            // Parse the line into data fields
            string fields[10];
            int fieldIndex = 0;
            string currentField = "";
            
            for (size_t i = 0; i < line.length(); i++) {
                char c = line[i];
                if (c == ' ') {
                    if (!currentField.empty()) {
                        if (fieldIndex < 10) {
                            fields[fieldIndex] = currentField;
                            fieldIndex++;
                        }
                        currentField = "";
                    }
                } else {
                    currentField += c;
                }
            }
            
            // Add last field
            if (!currentField.empty() && fieldIndex < 10) {
                fields[fieldIndex] = currentField;
                fieldIndex++;
            }
            
            // Create person if we got all 10 fields
            if (fieldIndex == 10) {
                try {
                    int year = stoi(fields[4]);
                    int month = stoi(fields[5]);
                    int day = stoi(fields[6]);
                    double bal = stod(fields[8]);
                    
                    Person newPerson(fields[0], fields[1], fields[2], fields[3],
                                   year, month, day, fields[7], bal, fields[9]);
                    
                    root = insertPerson(root, newPerson);
                    recordCount++;
                } catch (const exception& e) {
                    cout << "WARNING: Skipping invalid record: " << line << endl;
                }
            } else {
                cout << "WARNING: Skipping incomplete record: " << line << endl;
            }
        }
        
        inputFile.close();
        cout << "SUCCESS: Loaded " << recordCount << " person records" << endl;
        return true;
    }
    
    // Find and display a specific person
    void findPersonByName(const string& first, const string& last) {
        TreeNode* result = findPerson(root, first, last);
        if (result != nullptr) {
            cout << "FOUND: ";
            displayPersonInfo(result->data);
        } else {
            cout << "PERSON NOT FOUND: " << first << " " << last << endl;
        }
    }
    
    // Display all persons with given last name
    void findPersonsByLastName(const string& lastName) {
        cout << "Searching for last name: " << lastName << endl;
        findByLastName(root, lastName);
    }
    
    // Display all persons with given first name
    void findPersonsByFirstName(const string& firstName) {
        cout << "Searching for first name: " << firstName << endl;
        findByFirstName(root, firstName);
    }
    
    // Display all persons in sorted order
    void displayAllRecords() {
        if (root == nullptr) {
            cout << "DATABASE IS EMPTY" << endl;
            return;
        }
        cout << "ALL RECORDS:" << endl;
        cout << "------------" << endl;
        displayAllPersons(root);
    }
    
    // Find and display the oldest person
    void findOldestPersonInDatabase() {
        if (root == nullptr) {
            cout << "DATABASE IS EMPTY" << endl;
            return;
        }
        
        Person oldest = root->data;
        findOldestPerson(root, oldest);
        
        cout << "OLDEST PERSON: " << oldest.firstName << " " << oldest.lastName 
             << " from " << oldest.state << " (Zip: " << oldest.zipCode 
             << ") Born: " << oldest.birthYear << "-" << oldest.birthMonth 
             << "-" << oldest.birthDay << endl;
    }
    
    // Save all records to file
    void saveToFile(const string& filename) {
        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cout << "ERROR: Cannot create output file " << filename << endl;
            return;
        }
        
        saveToFile(root, outputFile);
        outputFile.close();
        cout << "SUCCESS: Database saved to " << filename << endl;
    }
    
    // Update a person's zip code
    void updatePersonZipCode(const string& first, const string& last, const string& newZip) {
        TreeNode* personNode = findPerson(root, first, last);
        if (personNode != nullptr) {
            personNode->data.zipCode = newZip;
            cout << "UPDATED: " << first << " " << last << " now lives in zip code " << newZip << endl;
        } else {
            cout << "PERSON NOT FOUND: " << first << " " << last << endl;
        }
    }
    
    // Remove a person from database
    void removePerson(const string& first, const string& last) {
        TreeNode* personNode = findPerson(root, first, last);
        if (personNode != nullptr) {
            root = deletePerson(root, first, last);
            cout << "DELETED: " << first << " " << last << endl;
        } else {
            cout << "PERSON NOT FOUND: " << first << " " << last << endl;
        }
    }
    
    // Verify tree is balanced
    void verifyTreeBalance() {
        bool isBalanced;
        int height;
        checkTreeBalance(root, isBalanced, height);
        
        if (isBalanced) {
            cout << "TREE STATUS: Balanced with height " << height << endl;
        } else {
            cout << "TREE STATUS: Not balanced (height " << height << ")" << endl;
        }
    }
    
    // Destructor - clean up all memory
    ~PersonDatabase() {
        deleteEntireTree(root);
    }
};

// Simple function to extract command and arguments from input
void parseCommand(const string& input, string& command, string& arg1, string& arg2, string& arg3) {
    command = "";
    arg1 = "";
    arg2 = "";
    arg3 = "";
    
    int part = 0;
    string current = "";
    
    for (size_t i = 0; i <= input.length(); i++) {
        if (i == input.length() || input[i] == ' ') {
            if (!current.empty()) {
                if (part == 0) command = current;
                else if (part == 1) arg1 = current;
                else if (part == 2) arg2 = current;
                else if (part == 3) arg3 = current;
                part++;
                current = "";
            }
        } else {
            current += input[i];
        }
    }
    
    // Handle case where there's text after the last space
    if (!current.empty()) {
        if (part == 0) command = current;
        else if (part == 1) arg1 = current;
        else if (part == 2) arg2 = current;
        else if (part == 3) arg3 = current;
    }
}

// Display usage information
void displayUsage(const string& programName) {
    cout << "Usage: " << programName << " <database_file>" << endl;
    cout << "Example: " << programName << " /home/subhajit/Desktop/Databases/database2025.txt" << endl;
    cout << "If no file specified, default path will be used." << endl;
}

// Main program with command line arguments
int main(int argc, char* argv[]) {
    string databaseFile;
    
    // Handle command line arguments
    if (argc == 2) {
        databaseFile = argv[1];
        cout << "Using specified database file: " << databaseFile << endl;
    } else if (argc == 1) {
        // Use default path if no arguments provided
        databaseFile = "/home/subhajit/Desktop/Databases/database2025.txt";
        cout << "No file specified. Using default: " << databaseFile << endl;
    } else {
        displayUsage(argv[0]);
        return 1;
    }
    
    cout << "PERSON DATABASE MANAGEMENT SYSTEM" << endl;
    cout << "Database File: " << databaseFile << endl;
    cout << "==========================================" << endl;
    
    // Create database and load data
    PersonDatabase database;
    if (!database.loadFromFile(databaseFile)) {
        cout << "FATAL ERROR: Cannot load database. Exiting." << endl;
        return 1;
    }
    
    cout << endl << "Available Commands:" << endl;
    cout << "FIND [first] [last]    - Find specific person" << endl;
    cout << "FAMILY [last]          - Find all with last name" << endl;
    cout << "FIRST [first]          - Find all with first name" << endl;
    cout << "PRINT                  - Display all records" << endl;
    cout << "OLDEST                 - Find oldest person" << endl;
    cout << "SAVE                   - Save database to file" << endl;
    cout << "RELOCATE [f] [l] [zip] - Update zip code" << endl;
    cout << "DELETE [f] [l]         - Remove person" << endl;
    cout << "VERIFY                 - Check tree balance" << endl;
    cout << "EXIT                   - Exit program" << endl;
    cout << "==========================================" << endl;
    
    string userInput;
    
    // Main command loop
    while (true) {
        cout << endl << "Enter command > ";
        getline(cin, userInput);
        
        // Skip empty input
        if (userInput.empty()) {
            continue;
        }
        
        string command, arg1, arg2, arg3;
        parseCommand(userInput, command, arg1, arg2, arg3);
        
        // Convert command to uppercase for case-insensitive comparison
        for (char& c : command) {
            if (c >= 'a' && c <= 'z') {
                c = c - 'a' + 'A';
            }
        }
        
        if (command == "FIND") {
            if (arg1.empty() || arg2.empty()) {
                cout << "USAGE: FIND [first name] [last name]" << endl;
            } else {
                database.findPersonByName(arg1, arg2);
            }
        }
        else if (command == "FAMILY") {
            if (arg1.empty()) {
                cout << "USAGE: FAMILY [last name]" << endl;
            } else {
                database.findPersonsByLastName(arg1);
            }
        }
        else if (command == "FIRST") {
            if (arg1.empty()) {
                cout << "USAGE: FIRST [first name]" << endl;
            } else {
                database.findPersonsByFirstName(arg1);
            }
        }
        else if (command == "PRINT") {
            database.displayAllRecords();
        }
        else if (command == "OLDEST") {
            database.findOldestPersonInDatabase();
        }
        else if (command == "SAVE") {
            database.saveToFile(databaseFile);
        }
        else if (command == "RELOCATE") {
            if (arg1.empty() || arg2.empty() || arg3.empty()) {
                cout << "USAGE: RELOCATE [first] [last] [new zip]" << endl;
            } else {
                database.updatePersonZipCode(arg1, arg2, arg3);
            }
        }
        else if (command == "DELETE") {
            if (arg1.empty() || arg2.empty()) {
                cout << "USAGE: DELETE [first] [last]" << endl;
            } else {
                database.removePerson(arg1, arg2);
            }
        }
        else if (command == "VERIFY") {
            database.verifyTreeBalance();
        }
        else if (command == "EXIT") {
            cout << "Saving database and exiting. Goodbye!" << endl;
            database.saveToFile(databaseFile);
            break;
        }
        else {
            cout << "UNKNOWN COMMAND: " << command << endl;
            cout << "Type a valid command from the list above." << endl;
        }
    }
    
    return 0;
}
