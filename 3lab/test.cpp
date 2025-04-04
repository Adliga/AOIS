#include "pch.h"
#include "gtest/gtest.h"
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\AOIS_3.cpp"
#include <sstream>
#include <string>

std::string captureMinimizeOutput(const std::string& expr, bool isDNF) {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    minimize(expr, isDNF);

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(MinimizeTest, DNF_Minimization_Output) {
    std::string input = "(abc)|(!abc)|(!a!b!c)";
    std::string output = captureMinimizeOutput(input, true); 

    EXPECT_NE(output.find("Минимизация СДНФ:"), std::string::npos);
    EXPECT_NE(output.find("Исходные импликанты: (abc)|(!abc)|(!a!b!c)"), std::string::npos);
    EXPECT_NE(output.find("Стадия склеивания:"), std::string::npos);
    EXPECT_NE(output.find("(abc) | (!abc) => (bc)"), std::string::npos);
    EXPECT_NE(output.find("Итоговый результат: (bc)|(!a!b!c)"), std::string::npos);
    EXPECT_NE(output.find("В бинарной форме: (X,1,1)(0,0,0)"), std::string::npos);
}

std::string captureMinimizeQuineOutput(const std::string& expr, bool isDNF) {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    minimizeQuine(expr, isDNF);

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(MinimizeQuineTest, DNF_MinimizationQuine_Output) {
    std::string input = "(abc)|(!abc)|(!a!b!c)";
    std::string output = captureMinimizeQuineOutput(input, true); 

    EXPECT_NE(output.find("Минимизация СДНФ:"), std::string::npos);
    EXPECT_NE(output.find("Группы по количеству единиц:"), std::string::npos);
    EXPECT_NE(output.find("Таблица покрытия:"), std::string::npos);
    EXPECT_NE(output.find("Итоговый результат:"), std::string::npos);
}

std::string captureMinimizeKarnaughOutput(const std::string& expr, bool isDNF) {
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    minimizeKarnaugh(expr, isDNF);

    std::cout.rdbuf(oldCout);
    return buffer.str();
}

TEST(MinimizeKarnaughTest, DNF_MinimizationKarnaugh_Output) {
    std::string input = "(abc)|(!abc)|(!a!b!c)";
    std::string output = captureMinimizeKarnaughOutput(input, true); 

    EXPECT_NE(output.find("Минимизация СДНФ методом карты Карно:"), std::string::npos);
    EXPECT_NE(output.find("Исходные импликанты:"), std::string::npos);
    EXPECT_NE(output.find("Найденные группы:"), std::string::npos);
    EXPECT_NE(output.find("Итоговый результат:"), std::string::npos);
}

TEST(MinimizeKarnaughTest, CNF_MoreThan4Variables_Error) {
    std::string input = "(abcde)&(!abcde)&(!a!b!c!d!e)";
    std::string output = captureMinimizeKarnaughOutput(input, false); 

    EXPECT_NE(output.find("Ошибка: Поддерживается до 4 переменных"), std::string::npos);
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
