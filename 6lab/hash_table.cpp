#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <iostream>

class AVLNode {
public:
    std::string key;
    std::string value;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const std::string& k, const std::string& v)
        : key(k), value(v), left(nullptr), right(nullptr), height(1) {
    }
};

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(AVLNode* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(AVLNode* node) {
        if (node) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        }
    }

    AVLNode* rotateRight(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    AVLNode* insert(AVLNode* node, const std::string& key, const std::string& value) {
        if (!node) {
            return new AVLNode(key, value);
        }
        if (key < node->key) {
            node->left = insert(node->left, key, value);
        }
        else if (key > node->key) {
            node->right = insert(node->right, key, value);
        }
        else {
            node->value = value; 
            return node;
        }

        updateHeight(node);
        int balance = getBalance(node);

        //left left
        if (balance > 1 && key < node->left->key) {
            return rotateRight(node);
        }
        //right right
        if (balance < -1 && key > node->right->key) {
            return rotateLeft(node);
        }
        //left right
        if (balance > 1 && key > node->left->key) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        //rght left
        if (balance < -1 && key < node->right->key) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* findMin(AVLNode* node) {
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    AVLNode* deleteNode(AVLNode* node, const std::string& key) {
        if (!node) {
            throw std::runtime_error("Key not found");
        }
        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        }
        else {

            if (!node->left) {
                AVLNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right) {
                AVLNode* temp = node->left;
                delete node;
                return temp;
            }

            AVLNode* temp = findMin(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->key);
        }

        updateHeight(node);
        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }

        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }

        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* search(AVLNode* node, const std::string& key) {
        if (!node || node->key == key) {
            return node;
        }
        if (key < node->key) {
            return search(node->left, key);
        }
        return search(node->right, key);
    }

    void destroy(AVLNode* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() {
        destroy(root);
    }

    void insert(const std::string& key, const std::string& value) {
        root = insert(root, key, value);
    }

    std::string search(const std::string& key) {
        AVLNode* node = search(root, key);
        if (!node) {
            throw std::runtime_error("Key not found");
        }
        return node->value;
    }

    void remove(const std::string& key) {
        root = deleteNode(root, key);
    }
};

class HashTable {

private:
    std::vector<AVLTree*> table;
    size_t table_size;
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    size_t value_of_key(const std::string& key) {
        if (key.length() < 2) {
            throw std::invalid_argument("Key must be at least 2 characters long");
        }
        char first = toupper(key[0]);
        char second = toupper(key[1]);
        if (alphabet.find(first) == std::string::npos || alphabet.find(second) == std::string::npos) {
            throw std::invalid_argument("First two characters must be Latin letters");
        }
        size_t base = 26; //лат. алфавит
        size_t v = alphabet.find(first) * base + alphabet.find(second);
        return v;
    }

    size_t hash(const std::string& key) {
        return value_of_key(key) % table_size;
    }


public:
    HashTable(size_t size) : table_size(size) {
        table.resize(size);
        for (size_t i = 0; i < size; ++i) {
            table[i] = new AVLTree();
        }
    }

    ~HashTable() {
        for (AVLTree* tree : table) {
            delete tree;
        }
    }

    void insert(const std::string& key, const std::string& value) {
        size_t index = hash(key);
        table[index]->insert(key, value);
    }

    std::string search(const std::string& key) {
        size_t index = hash(key);
        return table[index]->search(key);
    }

    void update(const std::string& key, const std::string& value) {
        size_t index = hash(key);
        table[index]->insert(key, value); 
    }

    void remove(const std::string& key) {
        size_t index = hash(key);
        table[index]->remove(key);
    }
};

int main() {
    HashTable ht(20); //размер таблицы

    while (true) {
        std::cout << "\n=== Hash Table Menu ===\n";
        std::cout << "1. Create (insert key-value)\n";
        std::cout << "2. Read (search by key)\n";
        std::cout << "3. Update (update key's value)\n";
        std::cout << "4. Delete (remove key)\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose operation (1-5): ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number (1-5).\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            continue;
        }
        std::cin.ignore(); 

        if (choice == 5) {
            std::cout << "Program terminated.\n";
            break;
        }

        if (choice < 1 || choice > 5) {
            std::cout << "Invalid choice. Please choose 1-5.\n";
            continue;
        }

        try {
            std::string key, value;
            if (choice == 1 || choice == 3) {
                std::cout << "Enter key (e.g., Dima): ";
                std::getline(std::cin, key);
                std::cout << "Enter value (e.g., Carpenter): ";
                std::getline(std::cin, value);
            }
            else {
                std::cout << "Enter key (e.g., Dima): ";
                std::getline(std::cin, key);
            }

            switch (choice) {
            case 1: 
                ht.insert(key, value);
                std::cout << "Key \"" << key << "\" inserted with value \"" << value << "\".\n";
                break;
            case 2: 
                value = ht.search(key);
                std::cout << "Value for key \"" << key << "\": " << value << "\n";
                break;
            case 3: 
                ht.update(key, value);
                std::cout << "Value for key \"" << key << "\" updated to \"" << value << "\".\n";
                break;
            case 4:
                ht.remove(key);
                std::cout << "Key \"" << key << "\" removed.\n";
                break;
            }
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Validation error: " << e.what() << "\n";
        }
        catch (const std::runtime_error& e) {
            std::cout << "Operation error: " << e.what() << "\n";
        }
        catch (...) {
            std::cout << "Unknown error.\n";
        }
    }

    return 0;
}
