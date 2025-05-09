#pragma once
#include <vector>
#include <iostream>
#include <tuple>

class DiagonalMatrix {
private:
    std::vector<std::vector<int> > matrix;
    int rows;
    int columns;

public:
    DiagonalMatrix(int rows, int cols) : rows(rows), columns(cols) {
        matrix = std::vector<std::vector<int> >(rows, std::vector<int>(cols, 0));
    }

    void WriteWord(const std::vector<int>& word, int start_row, int col) {
        for (size_t i = 0; i < word.size(); ++i) {
            int row = (start_row + i) % rows;
            matrix[row][col] = word[i];
        }
    }

    void WriteDiagonalColumn(const std::vector<int>& index, int start_row, int start_column) {
        for (size_t i = 0; i < index.size(); ++i) {
            int row = (start_row + i) % rows;
            int col = (start_column + i) % columns;
            matrix[row][col] = index[i];
        }
    }

    std::vector<int> ReadWord(int start_row, int col, int length) const {
        std::vector<int> result;
        for (int i = 0; i < length; ++i) {
            int row = (start_row + i) % rows;
            result.push_back(matrix[row][col]);
        }
        return result;
    }

    std::vector<int> ReadDiagonalColumn(int start_row, int start_column, int length) const {
        std::vector<int> result;
        for (int i = 0; i < length; ++i) {
            int row = (start_row + i) % rows;
            int col = (start_column + i) % columns;
            result.push_back(matrix[row][col]);
        }
        return result;
    }

    std::vector<int> LogicalF1(int col1, int col2, int write_col) {
        std::vector<int> word1 = ReadWord(col1, col1, rows);
        std::vector<int> word2 = ReadWord(col2, col2, rows);
        std::vector<int> result(rows);
        for (int i = 0; i < rows; ++i) {
            result[i] = (word1[i] || word2[i]) ? 1 : 0; // f1: a ∨ b
        }
        WriteWord(result, write_col, write_col); 
        return result;
    }

    std::vector<int> LogicalF14(int col1, int col2, int write_col) {
        std::vector<int> word1 = ReadWord(col1, col1, rows);
        std::vector<int> word2 = ReadWord(col2, col2, rows);
        std::vector<int> result(rows);
        for (int i = 0; i < rows; ++i) {
            result[i] = !(word1[i] || word2[i]) ? 1 : 0; // f14: ¬(a ∨ b)
        }
        WriteWord(result, write_col, write_col); 
        return result;
    }

    std::vector<int> LogicalF3(int col1, int col2, int write_col) {
        std::vector<int> result = ReadWord(col1, col1, rows); // f3: a
        WriteWord(result, write_col, write_col); 
        return result;
    }

    std::vector<int> LogicalF12(int col1, int col2, int write_col) {
        std::vector<int> word = ReadWord(col1, col1, rows);
        std::vector<int> result(rows);
        for (int i = 0; i < rows; ++i) {
            result[i] = !word[i] ? 1 : 0; // f12: ¬a
        }
        WriteWord(result, write_col, write_col); 
        return result;
    }

    std::vector<int> BinaryAdd(const std::vector<int>& a, const std::vector<int>& b) const {
        std::vector<int> result;
        int carry = 0;
        for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
            int total = a[i] + b[i] + carry;
            result.insert(result.begin(), total % 2);
            carry = total / 2;
        }
        if (carry) {
            result.insert(result.begin(), carry);
        }
        return result;
    }

    void SumFieldsByKey(const std::vector<int>& key_bits) {
        bool found = false;
        for (int column = 0; column < columns; ++column) {
            std::vector<int> word = ReadWord(column, column, rows);
            std::vector<int> word_prefix(word.begin(), word.begin() + key_bits.size());
            if (word_prefix == key_bits) {
                found = true;
                std::cout << "Found word in column " << column << ": ";
                for (int bit : word) std::cout << bit;
                std::cout << "\n";

                int v_length = 3;
                int a_b_length = 4;
                int s_length = 5; 

                std::vector<int> V(word.begin(), word.begin() + v_length);
                std::vector<int> A(word.begin() + v_length, word.begin() + v_length + a_b_length);
                std::vector<int> B(word.begin() + v_length + a_b_length, word.begin() + v_length + 2 * a_b_length);
                std::vector<int> S(word.begin() + v_length + 2 * a_b_length, word.begin() + v_length + 2 * a_b_length + s_length);

                std::cout << "Divided into fields:\n";
                std::cout << "V (3 bits): "; for (int bit : V) std::cout << bit; std::cout << "\n";
                std::cout << "A (4 bits): "; for (int bit : A) std::cout << bit; std::cout << "\n";
                std::cout << "B (4 bits): "; for (int bit : B) std::cout << bit; std::cout << "\n";
                std::cout << "S (5 bits): "; for (int bit : S) std::cout << bit; std::cout << "\n";

                std::vector<int> sum = BinaryAdd(A, B);
                while (sum.size() < s_length) sum.insert(sum.begin(), 0);
                if (sum.size() > s_length) sum.resize(s_length);

                std::cout << "Sum A + B: "; for (int bit : sum) std::cout << bit; std::cout << "\n";

                std::vector<int> new_word = V;
                new_word.insert(new_word.end(), A.begin(), A.end());
                new_word.insert(new_word.end(), B.begin(), B.end());
                new_word.insert(new_word.end(), sum.begin(), sum.end());
                while (new_word.size() < static_cast<size_t>(rows)) {
                    new_word.push_back(0);
                }

                std::cout << "New word: "; for (int bit : new_word) std::cout << bit; std::cout << "\n";
                WriteWord(new_word, column, column);
            }
        }
        if (!found) {
            std::cout << "No words found with key "; for (int bit : key_bits) std::cout << bit; std::cout << "\n";
        }
    }

    std::tuple<int, std::vector<int>, int, std::vector<int>> FindNearestAboveBelow(const std::vector<int>& target) const {
        std::vector<bool> below_flags(rows, false);
        std::vector<bool> above_flags(rows, false);
        int below_column = -1, above_column = -1;
        std::vector<int> below_word, above_word;

        //Дополняем target нулями справа до 16 бит
        std::vector<int> adjusted_target = target;
        while (adjusted_target.size() < static_cast<size_t>(rows)) {
            adjusted_target.push_back(0);
        }
        if (adjusted_target.size() > static_cast<size_t>(rows)) {
            adjusted_target.resize(rows);
        }

        bool has_below = false, has_above = false;
        for (int i = 0; i < rows; ++i) {
            std::vector<int> word = ReadWord(i, i, rows); 
            if (word.size() != static_cast<size_t>(rows)) {
                std::cerr << "Error: word size (" << word.size() << ") in column " << i << " does not match rows (16)\n";
                continue;
            }

            int g = 0, l = 0; //Инициализация триггеров
            for (int j = 0; j < rows; ++j) {
                int g_next = g || (!adjusted_target[j] && word[j] && !l);
                int l_next = l || (adjusted_target[j] && !word[j] && !g);
                g = g_next;
                l = l_next;
            }
            if (!g && l) { // word < target
                below_flags[i] = true;
                has_below = true;
            }
            else if (g && !l) { // word > target
                above_flags[i] = true;
                has_above = true;
            }
        }

        //поиск максимума среди below_flags и минимума среди above_flags
        if (has_below) {
            std::vector<bool> current_below = below_flags;
            for (size_t bit = 0; bit < static_cast<size_t>(rows); ++bit) {
                std::vector<bool> next_below(rows, false);
                bool found_one = false;
                for (int i = 0; i < rows; ++i) {
                    if (current_below[i]) {
                        std::vector<int> word = ReadWord(i, i, rows);
                        if (word.size() != static_cast<size_t>(rows)) {
                            std::cerr << "Error: word size (" << word.size() << ") in column " << i << " does not match rows (16)\n";
                            continue;
                        }
                        if (word[bit] == 1) {
                            next_below[i] = true;
                            found_one = true;
                        }
                    }
                }
                if (found_one) {
                    current_below = next_below;
                }
            }
            //выбираем первый столбец с максимальным словом
            for (int i = 0; i < rows; ++i) {
                if (current_below[i]) {
                    below_column = i;
                    below_word = ReadWord(i, i, rows);
                    break;
                }
            }
        }

        if (has_above) {
            std::vector<bool> current_above = above_flags;
            for (size_t bit = 0; bit < static_cast<size_t>(rows); ++bit) {
                std::vector<bool> next_above(rows, false);
                bool found_zero = false;
                for (int i = 0; i < rows; ++i) {
                    if (current_above[i]) {
                        std::vector<int> word = ReadWord(i, i, rows);
                        if (word.size() != static_cast<size_t>(rows)) {
                            std::cerr << "Error: word size (" << word.size() << ") in column " << i << " does not match rows (16)\n";
                            continue;
                        }
                        if (word[bit] == 0) {
                            next_above[i] = true;
                            found_zero = true;
                        }
                    }
                }
                if (found_zero) {
                    current_above = next_above;
                }
            }
            //выбираем первый столбец с минимальным словом
            for (int i = 0; i < rows; ++i) {
                if (current_above[i]) {
                    above_column = i;
                    above_word = ReadWord(i, i, rows);
                    break;
                }
            }
        }

        return std::make_tuple(below_column, below_word, above_column, above_word);
    }

    void PrintMatrix() const {
        for (const auto& row : matrix) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
}; 
