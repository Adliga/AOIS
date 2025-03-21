#include "pch.h"
#include "gtest/gtest.h"
#include "D:\Study\VisualStudio\AOIS_2\AOIS_2\AOIS_2.cpp" 
#include <vector>
#include <set>
#include <string>

string captureOutput(const string& expr) {
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf()); 
    printTruthTable(expr); 
    cout.rdbuf(oldCout); 
    return buffer.str(); 
}

TEST(TruthTableTest, MultipleVariables) {
    std::vector<std::string> rpn = toRPN("A&B|C");
    std::set<char> expectedVars = { 'A', 'B', 'C' };
    std::set<char> extractedVars;

    for (char ch : "A&B|C") {
        if (isalpha(ch))
            extractedVars.insert(ch);
    }

    EXPECT_EQ(extractedVars, expectedVars);
}

TEST(TruthTableTest, GenerateTruthTableRows) {
    std::vector<char> variables = { 'A', 'B', 'C' };
    size_t numRows = 1 << variables.size();
    std::vector<std::vector<bool>> allValues(numRows, std::vector<bool>(variables.size()));

    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < variables.size(); ++j) {
            allValues[i][j] = (i >> (variables.size() - 1 - j)) & 1;
        }
    }

    EXPECT_EQ(numRows, 8);
    EXPECT_EQ(allValues[0], (std::vector<bool>{0, 0, 0}));
    EXPECT_EQ(allValues[7], (std::vector<bool>{1, 1, 1}));
}

TEST(TruthTableTest, EvaluateExpression) {
    std::vector<std::string> rpn = toRPN("A&B");
    std::vector<char> variables = { 'A', 'B' };
    std::vector<std::vector<bool>> allValues = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };

    auto results = evaluateRPN(rpn, variables, allValues);

    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].second, (std::vector<bool>{0, 0, 0, 1}));
}

TEST(TruthTableTest, FindFinalExpression) {
    std::vector<std::pair<std::string, std::vector<bool>>> results = {
        {"A", {0, 0, 1, 1}},
        {"B", {0, 1, 0, 1}},
        {"A & B", {0, 0, 0, 1}}
    };

    size_t finalIndex = 0, maxLen = 0;
    for (size_t i = 0; i < results.size(); ++i) {
        if (results[i].first.length() > maxLen) {
            maxLen = results[i].first.length();
            finalIndex = i;
        }
    }

    EXPECT_EQ(results[finalIndex].first, "A & B");
}

TEST(TruthTableTest, CheckSDNF) {
    std::vector<char> variables = { 'A', 'B' };
    std::vector<int> minterms = { 3 };
    std::string sdnf = constructSDNF(variables, minterms);

    EXPECT_EQ(sdnf, "(A & B)");
}

TEST(TruthTableTest, CheckSKNF) {
    std::vector<char> variables = { 'A', 'B' };
    std::vector<int> maxterms = { 0 };
    std::string sknf = constructSKNF(variables, maxterms);

    EXPECT_EQ(sknf, "(A | B)");
}

TEST(TruthTableTest, BinaryToDecimalConversion) {
    std::vector<bool> binaryForm = { 0, 0, 0, 1 };
    int decimalValue = binaryToDecimal(binaryForm);

    EXPECT_EQ(decimalValue, 1);
}

TEST(PrintTruthTableTest, HandlesImplication) {
    string expr = "a->b";
    string expectedOutput = R"(
Таблица истинности для функции: a->b
 a  |  b  | (a->b)
--- | --- | ------
 0  |  0  |   1   
 0  |  1  |   1   
 1  |  0  |   0   
 1  |  1  |   1   

СДНФ: (!a & !b) | (!a & b) | (a & b)
СКНФ: (!a | b)
Числовая форма СДНФ: ( 0, 1, 3 )|
Числовая форма СКНФ: ( 2 )&
Индексная форма функции: 13 (бинарное: 1101)
)";

    string actualOutput = captureOutput(expr);
    EXPECT_EQ(actualOutput, expectedOutput);
}