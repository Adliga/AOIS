#include "pch.h"
#include "gtest/gtest.h"
#include "D:/Study/VisualStudio/AOIS_7/AOIS_7/main.cpp" 
#include <vector>
#include <tuple>
#include <stdexcept>

TEST(DiagonalMatrixTest, Initialization) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word = matrix.ReadWord(0, 0, 16);
    ASSERT_EQ(word.size(), 16);
    for (int bit : word) {
        EXPECT_EQ(bit, 0);
    }
}

TEST(DiagonalMatrixTest, WriteAndReadWord) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    matrix.WriteWord(word, 0, 0);
    std::vector<int> read_word = matrix.ReadWord(0, 0, 16);
    ASSERT_EQ(read_word.size(), 16);
    EXPECT_EQ(read_word, word);
}

TEST(DiagonalMatrixTest, WriteAndReadDiagonalColumn) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> column = { 1, 1, 0, 0 };
    matrix.WriteDiagonalColumn(column, 0, 0);
    std::vector<int> read_column = matrix.ReadDiagonalColumn(0, 0, 4);
    ASSERT_EQ(read_column.size(), 4);
    EXPECT_EQ(read_column, column);
}

TEST(DiagonalMatrixTest, LogicalF1) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word1 = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<int> word2 = { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    matrix.WriteWord(word1, 0, 0);
    matrix.WriteWord(word2, 1, 1);
    std::vector<int> result = matrix.LogicalF1(0, 1, 2);
    std::vector<int> expected = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    EXPECT_EQ(result, expected);
    std::vector<int> read_result = matrix.ReadWord(2, 2, 16);
    EXPECT_EQ(read_result, expected);
}

TEST(DiagonalMatrixTest, LogicalF14) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word1 = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<int> word2 = { 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    matrix.WriteWord(word1, 0, 0);
    matrix.WriteWord(word2, 1, 1);
    std::vector<int> result = matrix.LogicalF14(0, 1, 2);
    std::vector<int> expected = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    EXPECT_EQ(result, expected);
    std::vector<int> read_result = matrix.ReadWord(2, 2, 16);
    EXPECT_EQ(read_result, expected);
}

TEST(DiagonalMatrixTest, LogicalF3) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    matrix.WriteWord(word, 0, 0);
    std::vector<int> result = matrix.LogicalF3(0, 1, 2);
    EXPECT_EQ(result, word);
    std::vector<int> read_result = matrix.ReadWord(2, 2, 16);
    EXPECT_EQ(read_result, word);
}

TEST(DiagonalMatrixTest, LogicalF12) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    matrix.WriteWord(word, 0, 0);
    std::vector<int> result = matrix.LogicalF12(0, 1, 2);
    std::vector<int> expected = { 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    EXPECT_EQ(result, expected);
    std::vector<int> read_result = matrix.ReadWord(2, 2, 16);
    EXPECT_EQ(read_result, expected);
}

TEST(DiagonalMatrixTest, SumFieldsByKeySuccess) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word = { 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
    matrix.WriteWord(word, 0, 0);

    std::vector<int> key = { 1, 0, 1 };

    testing::internal::CaptureStdout();
    matrix.SumFieldsByKey(key);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Found word in column 0: 1011100001100000"), std::string::npos);
    EXPECT_NE(output.find("V (3 bits): 101"), std::string::npos);
    EXPECT_NE(output.find("A (4 bits): 1100"), std::string::npos);
    EXPECT_NE(output.find("B (4 bits): 0011"), std::string::npos);
    EXPECT_NE(output.find("S (5 bits): 00000"), std::string::npos);
    EXPECT_NE(output.find("Sum A + B: 01111"), std::string::npos);
    EXPECT_NE(output.find("New word: 1011100001101111"), std::string::npos);

    std::vector<int> read_word = matrix.ReadWord(0, 0, 16);
    std::vector<int> expected = { 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1 };
    EXPECT_EQ(read_word, expected);
}

TEST(DiagonalMatrixTest, FindNearestAboveBelow) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word1 = { 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
    std::vector<int> word2 = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
    std::vector<int> word3 = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 
    matrix.WriteWord(word1, 0, 0);
    matrix.WriteWord(word2, 1, 1);
    matrix.WriteWord(word3, 2, 2);
    std::vector<int> target = { 0, 1, 1, 0 }; 
    auto result = matrix.FindNearestAboveBelow(target);
    EXPECT_EQ(std::get<0>(result), 0); 
    EXPECT_EQ(std::get<1>(result), word1);
    EXPECT_EQ(std::get<2>(result), 1); 
    EXPECT_EQ(std::get<3>(result), word2);
}

TEST(DiagonalMatrixTest, FindNearestAboveBelowNoMatch) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> word = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    matrix.WriteWord(word, 0, 0);
    std::vector<int> target = { 1, 1, 1, 1 };
    auto result = matrix.FindNearestAboveBelow(target);
    EXPECT_EQ(std::get<0>(result), 0); 
    EXPECT_EQ(std::get<2>(result), -1); 
}

TEST(DiagonalMatrixTest, BinaryAdd) {
    DiagonalMatrix matrix(16, 16);
    std::vector<int> a = { 1, 1, 0, 0 }; 
    std::vector<int> b = { 0, 0, 1, 1 }; 
    std::vector<int> result = matrix.BinaryAdd(a, b);
    std::vector<int> expected = { 1, 1, 1, 1 };
    EXPECT_EQ(result, expected);
}

TEST(UtilityTest, GenerateRandomMatrix) {
    std::vector<std::vector<int>> matrix = generateRandomMatrix(4, 4);
    ASSERT_EQ(matrix.size(), 4);
    ASSERT_EQ(matrix[0].size(), 4);
    for (const auto& row : matrix) {
        for (int val : row) {
            EXPECT_TRUE(val == 0 || val == 1); 
        }
    }
}

TEST(UtilityTest, PrintBinaryWord) {
    std::vector<int> word = { 1, 0, 1, 0 };
    testing::internal::CaptureStdout();
    printBinaryWord(word, "Word: ");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Word: 1010\n");

    testing::internal::CaptureStdout();
    printBinaryWord({}, "Empty: ");
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Empty: None\n");
}

TEST(UtilityTest, ClearInputBuffer) {
    std::istringstream input("abc\n123");
    std::cin.rdbuf(input.rdbuf());
    std::cin.setstate(std::ios::failbit); 
    clearInputBuffer();
    EXPECT_FALSE(std::cin.fail()); 
    int value;
    std::cin >> value;
    EXPECT_EQ(value, 123); 
    std::cin.rdbuf(std::cin.rdbuf()); 
}

TEST(UtilityTest, InputBinaryVector) {
    std::istringstream input("1010\n");
    std::cin.rdbuf(input.rdbuf());
    testing::internal::CaptureStdout();
    std::vector<int> result = inputBinaryVector(4, "Enter binary: ");
    std::string output = testing::internal::GetCapturedStdout();
    std::vector<int> expected = { 1, 0, 1, 0 };
    EXPECT_EQ(result, expected);
    EXPECT_EQ(output, "Enter binary: ");

    input.str("10\n");
    input.clear();
    std::cin.rdbuf(input.rdbuf());
    result = inputBinaryVector(4, "Enter binary: ");
    expected = { 1, 0, 0, 0 };
    EXPECT_EQ(result, expected);
    std::cin.rdbuf(std::cin.rdbuf());
}

TEST(UtilityTest, InputInt) {
    std::istringstream input("5\n");
    std::cin.rdbuf(input.rdbuf());
    testing::internal::CaptureStdout();
    int result = inputInt("Enter number: ", 1, 10);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(result, 5);
    EXPECT_EQ(output, "Enter number: ");

    input.str("0\n5\n");
    input.clear();
    std::cin.rdbuf(input.rdbuf());
    testing::internal::CaptureStdout();
    result = inputInt("Enter number: ", 1, 10);
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(result, 5);
    EXPECT_NE(output.find("Invalid input"), std::string::npos);
    std::cin.rdbuf(std::cin.rdbuf());
}

TEST(UtilityTest, PrintVector) {
    std::vector<int> vec = { 1, 0, 1, 0 };
    testing::internal::CaptureStdout();
    printVector(vec, "Vector: ");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Vector: 1010\n");
}

TEST(UtilityTest, PrintBoolVector) {
    std::vector<bool> vec = { true, false, true, false };
    testing::internal::CaptureStdout();
    printBoolVector(vec, "Bool Vector: ");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Bool Vector: 1 0 1 0 \n");
}

TEST(UtilityTest, DisplayMenu) {
    testing::internal::CaptureStdout();
    displayMenu();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Diagonal Matrix Menu"), std::string::npos);
    EXPECT_NE(output.find("1. Write Word"), std::string::npos);
    EXPECT_NE(output.find("13. Exit"), std::string::npos);
    EXPECT_NE(output.find("Enter choice (1-12): "), std::string::npos);
}