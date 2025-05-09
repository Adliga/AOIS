#include "AOIS_7.cpp"
#include <iostream>
#include <string>
#include <limits>
#include <random>

std::vector<std::vector<int>> generateRandomMatrix(int rows, int cols) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen); 
        }
    }
    return matrix;
}

void printBinaryWord(const std::vector<int>& word, const std::string& label) {
    std::cout << label;
    if (word.empty()) {
        std::cout << "None\n";
    }
    else {
        for (int bit : word) {
            std::cout << bit;
        }
        std::cout << "\n";
    }
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::vector<int> inputBinaryVector(int length, const std::string& prompt) {
    std::vector<int> result;
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    for (char c : input) {
        if (c == '0' || c == '1') {
            result.push_back(c - '0');
            if (result.size() == length) break;
        }
    }
    while (result.size() < length) {
        result.push_back(0);
    }
    return result;
}

int inputInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
        clearInputBuffer();
    }
}

void printVector(const std::vector<int>& vec, const std::string& label) {
    std::cout << label;
    for (int val : vec) {
        std::cout << val;
    }
    std::cout << "\n";
}

void printBoolVector(const std::vector<bool>& vec, const std::string& label) {
    std::cout << label;
    for (bool val : vec) {
        std::cout << (val ? "1" : "0") << " ";
    }
    std::cout << "\n";
}

void displayMenu() {
    std::cout << "\n=== Diagonal Matrix Menu (Variant 1) ===\n";
    std::cout << "1. Write Word\n";
    std::cout << "2. Write Diagonal Column\n";
    std::cout << "3. Read Word\n";
    std::cout << "4. Read Diagonal Column\n";
    std::cout << "5. Logical F1 (OR)\n";
    std::cout << "6. Logical F14 (NOR)\n";
    std::cout << "7. Logical F3 (Copy First Word)\n";
    std::cout << "8. Logical F12 (Invert First Word)\n";
    std::cout << "9. Sum Fields by Key\n";
    std::cout << "10. Find Nearest Above/Below\n";
    std::cout << "11. Print Matrix\n";
    std::cout << "12. Generate Random Matrix\n";
    std::cout << "13. Exit\n";
    std::cout << "Enter choice (1-12): ";
}

//int main() {
//    DiagonalMatrix matrix(16, 16);
//    int choice;
//
//    while (true) {
//        displayMenu();
//        choice = inputInt("", 1, 12);
//
//        if (choice == 13) {
//            std::cout << "Exiting program.\n";
//            break;
//        }
//
//        switch (choice) {
//        case 1: { 
//            int start_row = inputInt("Enter start row (0-15): ", 0, 15);
//            int col = inputInt("Enter column (0-15): ", 0, 15);
//            int length = inputInt("Enter word length (1-16): ", 1, 16);
//            std::vector<int> word = inputBinaryVector(length, "Enter binary word (e.g., 1100): ");
//            matrix.WriteWord(word, start_row, col);
//            std::cout << "Word written successfully.\n";
//            break;
//        }
//        case 2: { 
//            int start_row = inputInt("Enter start row (0-15): ", 0, 15);
//            int start_col = inputInt("Enter start column (0-15): ", 0, 15);
//            int length = inputInt("Enter column length (1-16): ", 1, 16);
//            std::vector<int> column = inputBinaryVector(length, "Enter binary column (e.g., 1100): ");
//            matrix.WriteDiagonalColumn(column, start_row, start_col);
//            std::cout << "Diagonal column written successfully.\n";
//            break;
//        }
//        case 3: { 
//            int start_row = inputInt("Enter start row (0-15): ", 0, 15);
//            int col = inputInt("Enter column (0-15): ", 0, 15);
//            int length = inputInt("Enter word length (1-16): ", 1, 16);
//            std::vector<int> word = matrix.ReadWord(start_row, col, length);
//            printVector(word, "Word: ");
//            break;
//        }
//        case 4: { 
//            int start_row = inputInt("Enter start row (0-15): ", 0, 15);
//            int start_col = inputInt("Enter start column (0-15): ", 0, 15);
//            int length = inputInt("Enter column length (1-16): ", 1, 16);
//            std::vector<int> column = matrix.ReadDiagonalColumn(start_row, start_col, length);
//            printVector(column, "Diagonal column: ");
//            break;
//        }
//        case 5: {
//            int col1 = inputInt("Enter first column (0-15): ", 0, 15);
//            int col2 = inputInt("Enter second column (0-15): ", 0, 15);
//            int write_col = inputInt("Enter column to write result (0-15): ", 0, 15);
//            std::vector<int> result = matrix.LogicalF1(col1, col2, write_col);
//            printVector(result, "Logical F1 (OR) result: ");
//            std::cout << "Result written to column " << write_col << "\n";
//            break;
//        }
//        case 6: {
//            int col1 = inputInt("Enter first column (0-15): ", 0, 15);
//            int col2 = inputInt("Enter second column (0-15): ", 0, 15);
//            int write_col = inputInt("Enter column to write result (0-15): ", 0, 15);
//            std::vector<int> result = matrix.LogicalF14(col1, col2, write_col);
//            printVector(result, "Logical F14 (NOR) result: ");
//            std::cout << "Result written to column " << write_col << "\n";
//            break;
//        }
//        case 7: {
//            int col1 = inputInt("Enter first column (0-15): ", 0, 15);
//            int col2 = inputInt("Enter second column (0-15, ignored): ", 0, 15);
//            int write_col = inputInt("Enter column to write result (0-15): ", 0, 15);
//            std::vector<int> result = matrix.LogicalF3(col1, col2, write_col);
//            printVector(result, "Logical F3 (Copy First Word) result: ");
//            std::cout << "Result written to column " << write_col << "\n";
//            break;
//        }
//        case 8: {
//            int col1 = inputInt("Enter first column (0-15): ", 0, 15);
//            int col2 = inputInt("Enter second column (0-15, ignored): ", 0, 15);
//            int write_col = inputInt("Enter column to write result (0-15): ", 0, 15);
//            std::vector<int> result = matrix.LogicalF12(col1, col2, write_col);
//            printVector(result, "Logical F12 (Invert First Word) result: ");
//            std::cout << "Result written to column " << write_col << "\n";
//            break;
//        }
//        case 9: {
//            int key_length = 3;
//            std::vector<int> key = inputBinaryVector(key_length, "Enter binary key (e.g., 101): ");
//            matrix.SumFieldsByKey(key);
//            std::cout << "Fields summed successfully.\n";
//            break;
//        }
//
//        case 10: {
//            std::string target_str;
//            std::cout << "Enter target value (16 bit): ";
//            std::cin >> target_str;
//            std::vector<int> target;
//            for (char c : target_str) {
//                if (c != '0' && c != '1') {
//                    std::cout << "Invalid input: Use only 0 or 1\n";
//                    break;
//                }
//                target.push_back(c - '0');
//            }
//            if (target.empty()) {
//                std::cout << "Empty or invalid target\n";
//                break;
//            }
//
//            auto result = matrix.FindNearestAboveBelow(target);
//            int below_column = std::get<0>(result);
//            std::vector<int> below_word = std::get<1>(result);
//            int above_column = std::get<2>(result);
//            std::vector<int> above_word = std::get<3>(result);
//
//            std::cout << "Nearest below: ";
//            if (below_column == -1) {
//                std::cout << "No such word\n";
//            }
//            else {
//                std::cout << "Column " << below_column << ", Word ";
//                printBinaryWord(below_word, "");
//            }
//
//            std::cout << "Nearest above: ";
//            if (above_column == -1) {
//                std::cout << "No such word\n";
//            }
//            else {
//                std::cout << "Column " << above_column << ", Word ";
//                printBinaryWord(above_word, "");
//            }
//            break;
//        }
//        case 11: { 
//            std::cout << "Current matrix:\n";
//            matrix.PrintMatrix();
//            break;
//        }
//        case 12: { 
//            std::vector<std::vector<int>> random_matrix = generateRandomMatrix(16, 16);
//
//            for (int col = 0; col < 16; ++col) {
//                std::vector<int> word(16);
//                for (int row = 0; row < 16; ++row) {
//                    word[row] = random_matrix[row][col];
//                }
//                matrix.WriteWord(word, 0, col); 
//            }
//            std::cout << "Random matrix generated and written successfully.\n";
//            matrix.PrintMatrix(); 
//            break;
//        }
//
//        default:
//            std::cout << "Invalid choice.\n";
//            break;
//        }
//    }
//
//    return 0;
//}