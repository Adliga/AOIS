#include "pch.h"
#include "gtest/gtest.h"
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\AOIS_3.cpp"
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

string captureMinimizeOutput(const string& expr, bool isDNF) {
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());

    minimize(expr, isDNF);

    cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(MinimizeTest, PrintInitialTerms_DNF_Output) {
    string input = "(a&b&c)|(!a&b&c)|(!a&!b&!c)";
    string output = captureMinimizeOutput(input, true);

    EXPECT_NE(output.find("Минимизация СДНФ:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (a&b&c)|(!a&b&c)|(!a&!b&!c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (1,1,1)(0,1,1)(0,0,0)"), string::npos);
}

TEST(MinimizeTest, PrintInitialTerms_CNF_Output) {
    string input = "(a|b|c)&(!a|b|c)&(!a|!b|!c)";
    string output = captureMinimizeOutput(input, false);

    EXPECT_NE(output.find("Минимизация СКНФ:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (a|b|c)&(!a|b|c)&(!a|!b|!c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (0,0,0)(1,0,0)(1,1,1)"), string::npos);
}

TEST(MinimizeTest, GlueTerms_DNF_Output) {
    string input = "(a&b&c)|(!a&b&c)|(!a&!b&!c)";
    string output = captureMinimizeOutput(input, true);

    EXPECT_NE(output.find("Стадия склеивания:"), string::npos);
    EXPECT_NE(output.find("(a&b&c) | (!a&b&c) => (b&c)"), string::npos);
    EXPECT_NE(output.find("Итоговый результат:"), string::npos);
}

TEST(MinimizeTest, GlueTerms_CNF_Output) {
    string input = "(a|b|c)&(!a|b|c)";
    string output = captureMinimizeOutput(input, false);

    EXPECT_NE(output.find("Стадия склеивания:"), string::npos);
    EXPECT_NE(output.find("(a|b|c) (!a|b|c) => (b|c)"), string::npos);
    EXPECT_NE(output.find("Итоговый результат:"), string::npos);
}

TEST(MinimizeTest, PrintResultTerms_DNF_Output) {
    string input = "(a&b&c)|(!a&b&c)|(!a&!b&!c)";
    string output = captureMinimizeOutput(input, true);

    EXPECT_NE(output.find("Итоговый результат: (b&c)|(!a&!b&!c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (X,1,1)(0,0,0)"), string::npos);
}

TEST(MinimizeTest, PrintResultTerms_CNF_Output) {
    string input = "(a|b|c)&(!a|b|c)";
    string output = captureMinimizeOutput(input, false);

    EXPECT_NE(output.find("Итоговый результат: (b|c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (X,0,0)"), string::npos);
}

TEST(MinimizeTest, Full_DNF_Minimization) {
    string input = "(a&b&c)|(!a&b&c)|(!a&!b&!c)";
    string output = captureMinimizeOutput(input, true);

    EXPECT_NE(output.find("Минимизация СДНФ:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (a&b&c)|(!a&b&c)|(!a&!b&!c)"), string::npos);
    EXPECT_NE(output.find("Стадия склеивания:"), string::npos);
    EXPECT_NE(output.find("(a&b&c) | (!a&b&c) => (b&c)"), string::npos);
    EXPECT_NE(output.find("Итоговый результат: (b&c)|(!a&!b&!c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (X,1,1)(0,0,0)"), string::npos);
}

TEST(MinimizeTest, Full_CNF_Minimization) {
    string input = "(a|b|c)&(!a|b|c)";
    string output = captureMinimizeOutput(input, false);

    EXPECT_NE(output.find("Минимизация СКНФ:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (a|b|c)&(!a|b|c)"), string::npos);
    EXPECT_NE(output.find("Стадия склеивания:"), string::npos);
    EXPECT_NE(output.find("(a|b|c) (!a|b|c) => (b|c)"), string::npos);
    EXPECT_NE(output.find("Итоговый результат: (b|c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (X,0,0)"), string::npos);
}

string captureMinimizeQuineOutput(const string& expr, bool isDNF) {
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());

    minimizeQuine(expr, isDNF);

    cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(MinimizeQuineTest, PrintInitialInfo_DNF_Output) {
    string input = "(a&b&c)|(!a&b&c)|(!a&!b&!c)";
    string output = captureMinimizeQuineOutput(input, true);

    EXPECT_NE(output.find("Минимизация СДНФ:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (a&b&c)|(!a&b&c)|(!a&!b&!c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (1,1,1)(0,1,1)(0,0,0)"), string::npos);
}

TEST(MinimizeQuineTest, PrintInitialInfo_CNF_Output) {
    string input = "(a|b|c)&(!a|b|c)&(!a|!b|!c)";
    string output = captureMinimizeQuineOutput(input, false);

    EXPECT_NE(output.find("Минимизация СКНФ:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (a|b|c)&(!a|b|c)&(!a|!b|!c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (0,0,0)(1,0,0)(1,1,1)"), string::npos);
}

TEST(MinimizeQuineTest, PrintGroups_DNF_Output) {
    string input = "(a&b&c)|(!a&b&c)|(!a&!b&!c)";
    string output = captureMinimizeQuineOutput(input, true);

    EXPECT_NE(output.find("Группы по количеству единиц:"), string::npos);
    EXPECT_NE(output.find("0: (0,0,0)"), string::npos);
    EXPECT_NE(output.find("2: (0,1,1)"), string::npos);
    EXPECT_NE(output.find("3: (1,1,1)"), string::npos);
}

TEST(MinimizeQuineTest, PrintGroups_CNF_Output) {
    string input = "(a|b|c)&(!a|b|c)&(!a|!b|!c)";
    string output = captureMinimizeQuineOutput(input, false);

    EXPECT_NE(output.find("Группы по количеству нулей:"), string::npos);
    EXPECT_NE(output.find("0: (1,1,1)"), string::npos);
    EXPECT_NE(output.find("2: (1,0,0)"), string::npos);
    EXPECT_NE(output.find("3: (0,0,0)"), string::npos);
}

TEST(MinimizeQuineTest, PrintCoverTable_DNF_Output) {
    string input = "(a&b&c)|(!a&b&c)|(!a&!b&!c)";
    string output = captureMinimizeQuineOutput(input, true);

    EXPECT_NE(output.find("Таблица покрытия:"), string::npos);
    EXPECT_NE(output.find("Импликанты"), string::npos);
    EXPECT_NE(output.find("(1,1,1)"), string::npos); 
    EXPECT_NE(output.find("(X,1,1)"), string::npos); 
    EXPECT_NE(output.find("X"), string::npos);      
}

TEST(MinimizeQuineTest, PrintCoverTable_CNF_Output) {
    string input = "(a|b|c)&(!a|b|c)";
    string output = captureMinimizeQuineOutput(input, false);

    EXPECT_NE(output.find("Таблица покрытия:"), string::npos);
    EXPECT_NE(output.find("Импликанты"), string::npos);
    EXPECT_NE(output.find("(0,0,0)"), string::npos); 
    EXPECT_NE(output.find("(X,0,0)"), string::npos); 
    EXPECT_NE(output.find("X"), string::npos);     
}

TEST(MinimizeQuineTest, PrintFinalResult_CNF_Output) {
    string input = "(a|b|c)&(!a|b|c)";
    string output = captureMinimizeQuineOutput(input, false);

    EXPECT_NE(output.find("Итоговый результат: (b|c)"), string::npos);
    EXPECT_NE(output.find("В бинарной форме: (X,0,0)"), string::npos);
}

string capturePrintKarnaughMapOutput(const vector<vector<vector<int>>>& kmap, int n, const vector<char>& varNames) {
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());

    printKarnaughMap(kmap, n, varNames);

    cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(KarnaughMapTest, BuildKarnaughMap_Structure) {
    map<vector<int>, int> results = {
        {{0, 0, 0}, 0}, {{0, 0, 1}, 1}, {{0, 1, 0}, 0}, {{0, 1, 1}, 1},
        {{1, 0, 0}, 0}, {{1, 0, 1}, 0}, {{1, 1, 0}, 1}, {{1, 1, 1}, 1}
    };
    vector<vector<vector<int>>> kmap = buildKarnaughMap(results, 3);

    EXPECT_EQ(kmap.size(), 1);      
    EXPECT_EQ(kmap[0].size(), 2);   
    EXPECT_EQ(kmap[0][0].size(), 4); 

    EXPECT_EQ(kmap[0][0][0], 0); 
    EXPECT_EQ(kmap[0][0][1], 1); 
    EXPECT_EQ(kmap[0][0][2], 1); 
    EXPECT_EQ(kmap[0][0][3], 0); 

    EXPECT_EQ(kmap[0][1][0], 0); 
    EXPECT_EQ(kmap[0][1][1], 0); 
    EXPECT_EQ(kmap[0][1][2], 1); 
    EXPECT_EQ(kmap[0][1][3], 1); 
}

TEST(KarnaughMapTest, BuildKarnaughMap_5Variables) {
    map<vector<int>, int> results = {
        {{0, 0, 0, 0, 0}, 1}, {{0, 0, 0, 0, 1}, 0}, {{1, 1, 1, 1, 1}, 1}
    };
    vector<vector<vector<int>>> kmap = buildKarnaughMap(results, 5);

    EXPECT_EQ(kmap.size(), 2);      
    EXPECT_EQ(kmap[0].size(), 4);   
    EXPECT_EQ(kmap[0][0].size(), 4); 
    EXPECT_EQ(kmap[0][0][0], 1); 
    EXPECT_EQ(kmap[1][2][2], 1); 
}

TEST(KarnaughMapTest, BuildKarnaughMap_ErrorMoreThan5Variables) {
    map<vector<int>, int> results = { {{0, 0, 0, 0, 0, 0}, 1} };
    EXPECT_THROW(buildKarnaughMap(results, 6), runtime_error);
}

TEST(KarnaughMapTest, PrintKarnaughMap_2Variables_Output) {
    vector<vector<vector<int>>> kmap = { {{0, 1}, {1, 0}} };
    vector<char> varNames = { 'A', 'B' };
    string output = capturePrintKarnaughMapOutput(kmap, 2, varNames);

    EXPECT_NE(output.find("Карта Карно:"), string::npos);
    EXPECT_NE(output.find("A\\B | 0  1"), string::npos);
    EXPECT_NE(output.find("----+------"), string::npos);
    EXPECT_NE(output.find("0   | 0  1"), string::npos);
    EXPECT_NE(output.find("1   | 1  0"), string::npos);
}

TEST(KarnaughMapTest, PrintKarnaughMap_4Variables_Output) {
    vector<vector<vector<int>>> kmap = { {{0, 1, 1, 0}, {1, 0, 0, 1}, {1, 1, 0, 0}, {0, 0, 1, 1}} };
    vector<char> varNames = { 'A', 'B', 'C', 'D' };
    string output = capturePrintKarnaughMapOutput(kmap, 4, varNames);

    EXPECT_NE(output.find("Карта Карно:"), string::npos);
    EXPECT_NE(output.find("AB\\CD | 00  01  11  10"), string::npos);
    EXPECT_NE(output.find("------+---------------"), string::npos);
    EXPECT_NE(output.find("00    | 0   1   1   0"), string::npos);
    EXPECT_NE(output.find("01    | 1   0   0   1"), string::npos);
    EXPECT_NE(output.find("11    | 1   1   0   0"), string::npos);
    EXPECT_NE(output.find("10    | 0   0   1   1"), string::npos);
}

string captureMinimizeKarnaughOutput(const string& expr, bool isDNF) {
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());
    minimizeKarnaugh(expr, isDNF);
    cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(KarnaughTest, FindKarnaughGroups_DNF) {
    vector<vector<vector<int>>> kmap = { {{0, 1, 1, 0}, {1, 0, 0, 1}, {1, 1, 0, 0}, {0, 0, 1, 1}} };
    vector<vector<tuple<int, int, int>>> groups = findKarnaughGroups(kmap, 1);
    EXPECT_FALSE(groups.empty());
    EXPECT_GE(groups[0].size(), 1);
}

TEST(KarnaughTest, FindKarnaughGroups_CNF) {
    vector<vector<vector<int>>> kmap = { {{1, 0, 0, 1}, {0, 1, 1, 0}, {0, 0, 1, 1}, {1, 1, 0, 0}} };
    vector<vector<tuple<int, int, int>>> groups = findKarnaughGroups(kmap, 0);
    EXPECT_FALSE(groups.empty());
    EXPECT_GE(groups[0].size(), 1);
}

TEST(KarnaughTest, MinimizeKarnaugh_DNF_Output) {
    string input = "(A&B&C)|(!A&B&C)|(!A&!B&!C)";
    string output = captureMinimizeKarnaughOutput(input, true);
    EXPECT_NE(output.find("Минимизация СДНФ методом карты Карно:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (A&B&C)|(!A&B&C)|(!A&!B&!C)"), string::npos);
    EXPECT_NE(output.find("Карта Карно:"), string::npos);
    EXPECT_NE(output.find("Найденные группы:"), string::npos);
    EXPECT_NE(output.find("Итоговый результат: (B&C)|(!A&!B&!C)"), string::npos);
}

TEST(KarnaughTest, MinimizeKarnaugh_CNF_Output) {
    string input = "(A|B|C)&(!A|B|C)";
    string output = captureMinimizeKarnaughOutput(input, false);
    EXPECT_NE(output.find("Минимизация СКНФ методом карты Карно:"), string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (A|B|C)&(!A|B|C)"), string::npos);
    EXPECT_NE(output.find("Карта Карно:"), string::npos);
    EXPECT_NE(output.find("Найденные группы:"), string::npos);
    EXPECT_NE(output.find("Итоговый результат: (B|C)"), string::npos);
}

TEST(KarnaughTest, MinimizeKarnaugh_ErrorMoreThan5Variables) {
    string input = "(A&B&C&D&E&F)";
    string output = captureMinimizeKarnaughOutput(input, true);
    EXPECT_NE(output.find("Ошибка: Поддерживается до 5 переменных"), string::npos);
}

TEST(ValidationTest, ValidDNF) {
    EXPECT_TRUE(isValidDNF("(A|B)|(C|!D)"));
    EXPECT_TRUE(isValidDNF("!(A|B)|C"));
    EXPECT_TRUE(isValidDNF("!(A|!B)|C"));
}

TEST(ValidationTest, ValidCNF) {
    EXPECT_TRUE(isValidCNF("(AB)|(CD)"));
    EXPECT_TRUE(isValidCNF("(AB)C"));
    EXPECT_TRUE(isValidCNF("(AB)CD"));
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

TEST(TruthTableTest, BinaryToDecimalConversion) {
    std::vector<bool> binaryForm = { 0, 0, 0, 1 };
    int decimalValue = binaryToDecimal(binaryForm);

    EXPECT_EQ(decimalValue, 1);
}

string capturePrintTruthTableOutput(const string& expr) {
    stringstream buffer;
    streambuf* oldCout = cout.rdbuf(buffer.rdbuf());
    printTruthTable(expr);
    cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(TruthTableTest, ExtractVariables) {
    string expr = "(A&B)|(!C)";
    vector<char> vars = extractVariables(expr);
    EXPECT_EQ(vars.size(), 3);
    EXPECT_EQ(vars[0], 'A');
    EXPECT_EQ(vars[1], 'B');
    EXPECT_EQ(vars[2], 'C');
}

TEST(TruthTableTest, GenerateVariableCombinations) {
    vector<char> vars = { 'A', 'B' };
    vector<vector<bool>> combos = generateVariableCombinations(vars);
    EXPECT_EQ(combos.size(), 4);
    EXPECT_EQ(combos[0], vector<bool>({ 0, 0 }));
    EXPECT_EQ(combos[1], vector<bool>({ 0, 1 }));
    EXPECT_EQ(combos[2], vector<bool>({ 1, 0 }));
    EXPECT_EQ(combos[3], vector<bool>({ 1, 1 }));
}

TEST(TruthTableTest, EvaluateExpressions) {
    vector<string> rpn = { "A", "B", "&" };
    vector<char> vars = { 'A', 'B' };
    vector<vector<bool>> allValues = { {0, 0}, {0, 1}, {1, 0}, {1, 1} };
    vector<pair<string, vector<bool>>> results = evaluateExpressions(rpn, vars, allValues);
    EXPECT_EQ(results.back().second, vector<bool>({ 0, 0, 0, 1 }));
}

TEST(TruthTableTest, SelectFinalExpression) {
    vector<pair<string, vector<bool>>> results = {
        {"A", {0, 1}},
        {"A&B", {0, 0}}
    };
    pair<string, vector<bool>> finalExpr = selectFinalExpression(results);
    EXPECT_EQ(finalExpr.first, "A&B");
    EXPECT_EQ(finalExpr.second, vector<bool>({ 0, 0 }));
}

TEST(TruthTableTest, PrintTableHeader_Output) {
    string expr = "A&B";
    string output = capturePrintTruthTableOutput(expr);
    EXPECT_NE(output.find("A"), string::npos);
    EXPECT_NE(output.find("B"), string::npos);
    EXPECT_NE(output.find("A&B"), string::npos);
    EXPECT_NE(output.find("--- | --- | -----"), string::npos);
}
