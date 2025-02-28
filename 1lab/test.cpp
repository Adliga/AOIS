#include "pch.h"
#include "gtest/gtest.h"
#include "D:\Study\VisualStudio\AOIS_1_test\AOIS_1_test\AOIS_1_test.cpp" 

TEST(TwoComplementTest, PositiveNumbers) {
    EXPECT_EQ(fromTwoComplementToDecimal("00000000000000000000000000000000"), 0);
    EXPECT_EQ(fromTwoComplementToDecimal("00000000000000000000000000000001"), 1);
    EXPECT_EQ(fromTwoComplementToDecimal("00000000000000000000000000000101"), 5);
    EXPECT_EQ(fromTwoComplementToDecimal("00111111111111111111111111111111"), 1073741823);
}

TEST(TwoComplementTest, NegativeNumbers) {
    EXPECT_EQ(fromTwoComplementToDecimal("10000000000000000000000000000000"), -2147483648);
    EXPECT_EQ(fromTwoComplementToDecimal("11111111111111111111111111111111"), -1);
    EXPECT_EQ(fromTwoComplementToDecimal("11111111111111111111111111111011"), -5);
    EXPECT_EQ(fromTwoComplementToDecimal("11000000000000000000000000000000"), -1073741824);
}

TEST(TwoComplementTest, InvalidLength) {
    EXPECT_THROW(fromTwoComplementToDecimal("0000000000000000000000000000000"), std::invalid_argument);  
    EXPECT_THROW(fromTwoComplementToDecimal("000000000000000000000000000000000"), std::invalid_argument); 
    EXPECT_THROW(fromTwoComplementToDecimal(""), std::invalid_argument); 
}

TEST(BinaryToDecimalTest, PositiveNumbers) {
    EXPECT_EQ(fromBinaryToDecimal("00000000000000000000000000001010"), 10);
    EXPECT_EQ(fromBinaryToDecimal("00000000000000000000000000000001"), 1);
    EXPECT_EQ(fromBinaryToDecimal("00000000000000000000000000000101"), 5);
    EXPECT_EQ(fromBinaryToDecimal("00111111111111111111111111111111"), 1073741823);
}

TEST(BinaryToDecimalTest, NegativeNumbers) {
    EXPECT_EQ(fromBinaryToDecimal("11111111111111111111111111111111"), -2147483647); 
    EXPECT_EQ(fromBinaryToDecimal("10000000000000000000000000000001"), -1);
    EXPECT_EQ(fromBinaryToDecimal("10000000000000000000000000000101"), -5);
    EXPECT_EQ(fromBinaryToDecimal("11000000000000000000000000000000"), -1073741824);
}

TEST(BinaryToDecimalTest, Zero) {
    EXPECT_EQ(fromBinaryToDecimal("00000000000000000000000000000000"), 0); 
}

TEST(BinaryToDecimalTest, InvalidLength) {
    EXPECT_THROW(fromBinaryToDecimal("000000000000000000000000000000000"), std::invalid_argument); 
    EXPECT_THROW(fromBinaryToDecimal(""), std::invalid_argument); 
}

TEST(ToBinaryTest, PositiveInteger) {
    int num = 5;
    string expected = "00000000000000000000000000000101"; 
    EXPECT_EQ(toBinary(num), expected);
}

TEST(ToBinaryTest, NegativeInteger) {
    int num = -5;
    string expected = "10000000000000000000000000000101";  
    EXPECT_EQ(toBinary(num), expected);
}

TEST(ToBinaryTest, Zero) {
    int num = 0;
    string expected = "00000000000000000000000000000000"; 
    EXPECT_EQ(toBinary(num), expected);
}

TEST(ToBinaryTest, MaxInt) {
    int num = 2147483647;
    string expected = "01111111111111111111111111111111"; 
    EXPECT_EQ(toBinary(num), expected);
}

TEST(ToBinaryTest, MinInt) {
    int num = -2147483648;
    string expected = "10000000000000000000000000000000";  
    EXPECT_EQ(toBinary(num), expected);
}

TEST(ToBinaryTest, PositiveDouble) {
    double num = 5.625;
    string expected = "0" + string("101") + "." + string("101"); 
    EXPECT_EQ(toBinary(num), expected);
}

TEST(ToBinaryTest, NegativeDouble) {
    double num = -5.625;
    string expected = "1" + string("101") + "." + string("101");  
    EXPECT_EQ(toBinary(num), expected);
}

TEST(ToTwoComplementTest, PositiveInteger) {
    int num = 5;
    string expected = "00000000000000000000000000000101";  
    EXPECT_EQ(toTwoComplement(num), expected);
}

TEST(ToTwoComplementTest, NegativeInteger) {
    int num = -5;
    string expected = "11111111111111111111111111111011"; 
    EXPECT_EQ(toTwoComplement(num), expected);
}

TEST(ToTwoComplementTest, Zero) {
    int num = 0;
    string expected = "00000000000000000000000000000000";  
    EXPECT_EQ(toTwoComplement(num), expected);
}

TEST(ToTwoComplementTest, MaxInt) {
    int num = 2147483647;
    string expected = "01111111111111111111111111111111"; 
    EXPECT_EQ(toTwoComplement(num), expected);
}

TEST(ToOneComplementTest, PositiveInteger) {
    int num = 5;
    string expected = "00000000000000000000000000000101";  
    EXPECT_EQ(toOneComplement(num), expected);
}

TEST(ToOneComplementTest, NegativeInteger) {
    int num = -5;
    string expected = "11111111111111111111111111111010"; 
    EXPECT_EQ(toOneComplement(num), expected);
}

TEST(ToOneComplementTest, Zero) {
    int num = 0;
    string expected = "00000000000000000000000000000000"; 
    EXPECT_EQ(toOneComplement(num), expected);
}

TEST(ToOneComplementTest, MaxInt) {
    int num = 1073741823;
    string expected = "00111111111111111111111111111111"; 
    EXPECT_EQ(toOneComplement(num), expected);
}

TEST(ToOneComplementTest, MinInt) {
    int num = -2147483648;
    string expected = "11111111111111111111111111111111";  
    EXPECT_EQ(toOneComplement(num), expected);
}

TEST(AddTwoNumbersInTwoComplementTest, PositiveNumbers) {
    int num1 = 5;
    int num2 = 3;

    testing::internal::CaptureStdout();
    addTwoNumbersInTwoComplement(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Результат сложения в десятичном формате: 8") != std::string::npos);
}

TEST(AddTwoNumbersInTwoComplementTest, NegativeAndPositiveNumber) {
    int num1 = -5;
    int num2 = 3;

    testing::internal::CaptureStdout();
    addTwoNumbersInTwoComplement(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Результат сложения в десятичном формате: -2") != std::string::npos);
}

TEST(AddTwoNumbersInTwoComplementTest, NegativeNumbers) {
    int num1 = -5;
    int num2 = -3;

    testing::internal::CaptureStdout();
    addTwoNumbersInTwoComplement(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Результат сложения в десятичном формате: -8") != std::string::npos);
}

TEST(AddTwoNumbersInTwoComplementTest, Overflow) {
    int num1 = 2147483647; 
    int num2 = 1;

    testing::internal::CaptureStdout();
    addTwoNumbersInTwoComplement(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Результат сложения в десятичном формате: -2147483648") != std::string::npos);
}

TEST(AddTwoNumbersInTwoComplementTest, ZeroResult) {
    int num1 = 5;
    int num2 = -5;

    testing::internal::CaptureStdout();
    addTwoNumbersInTwoComplement(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Результат сложения в десятичном формате: 0") != std::string::npos);
}

TEST(MultiplyTest, PositiveNumbers) {
    int num1 = 6;
    int num2 = 7;

    testing::internal::CaptureStdout();
    int result = multiply(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, 42);
    EXPECT_TRUE(output.find("Результат умножения в десятичном формате: 42") != std::string::npos);
    EXPECT_TRUE(output.find("Результат умножения в двоичном формате: 00000000000000000000000000101010") != std::string::npos);  // Проверка на двоичное представление
}

TEST(MultiplyTest, NegativeAndPositiveNumber) {
    int num1 = -6;
    int num2 = 7;

    testing::internal::CaptureStdout();
    int result = multiply(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, -42);
    EXPECT_TRUE(output.find("Результат умножения в десятичном формате: -42") != std::string::npos);
    EXPECT_TRUE(output.find("Результат умножения в двоичном формате: 10000000000000000000000000101010") != std::string::npos); 
}

TEST(MultiplyTest, NegativeNumbers) {
    int num1 = -6;
    int num2 = -7;

    testing::internal::CaptureStdout();
    int result = multiply(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, 42);
    EXPECT_TRUE(output.find("Результат умножения в десятичном формате: 42") != std::string::npos);
    EXPECT_TRUE(output.find("Результат умножения в двоичном формате: 00000000000000000000000000101010") != std::string::npos);  
}

TEST(MultiplyTest, ZeroResult) {
    int num1 = 0;
    int num2 = 7;

    testing::internal::CaptureStdout();
    int result = multiply(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, 0);
    EXPECT_TRUE(output.find("Результат умножения в десятичном формате: 0") != std::string::npos);
    EXPECT_TRUE(output.find("Результат умножения в двоичном формате: 00000000000000000000000000000000") != std::string::npos); 
}

TEST(MultiplyTest, LargeNumbers) {
    int num1 = 1024;
    int num2 = 2048;


    testing::internal::CaptureStdout();
    int result = multiply(num1, num2);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(result, 2097152);
    EXPECT_TRUE(output.find("Результат умножения в десятичном формате: 2097152") != std::string::npos);
    EXPECT_TRUE(output.find("Результат умножения в двоичном формате: 00000000001000000000000000000000") != std::string::npos);
}                                                                             

TEST(DivideTest, DivisionByOne) {
    int dividend = 20;
    int divisor = 1;

    testing::internal::CaptureStdout();
    double result = divide(dividend, divisor);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Результат деления (десятичный): 20") != std::string::npos);
    EXPECT_TRUE(output.find("Результат деления (бинарный): 010100.") != std::string::npos);
}

TEST(DivideTest, NegativeDividend) {
    int dividend = -15;
    int divisor = 2;

    testing::internal::CaptureStdout();
    double result = divide(dividend, divisor);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Результат деления (десятичный): -7.5") != std::string::npos);
    EXPECT_TRUE(output.find("Результат деления (бинарный): 1111.1") != std::string::npos);
}

TEST(DivideTest, DivisionWithRemainder) {
    int dividend = 10;
    int divisor = 3;

    testing::internal::CaptureStdout();
    double result = divide(dividend, divisor);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Результат деления (десятичный): 3.3125") != std::string::npos);
    EXPECT_TRUE(output.find("Результат деления (бинарный): 011.0101") != std::string::npos);
}

TEST(IEEE754Test, AllFunctionsTest) {

    float num1 = 3.75f;
    float num2 = 1.25f;

    ieee754 ieeeNum1 = floatToIEEE754(num1);
    ieee754 ieeeNum2 = floatToIEEE754(num2);

    float result1 = ieee754ToFloat(ieeeNum1);
    float result2 = ieee754ToFloat(ieeeNum2);

    EXPECT_FLOAT_EQ(result1, num1);  
    EXPECT_FLOAT_EQ(result2, num2); 

    ieee754 sumIEEE = ieee754Add(ieeeNum1, ieeeNum2);

    float sumResult = ieee754ToFloat(sumIEEE);

    float expectedSum = num1 + num2;

    EXPECT_FLOAT_EQ(sumResult, expectedSum);  
}
