#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>

using namespace std;

typedef unsigned int ieee754; 
const int BITS = 32;
const int SIGN_BIT = 31;
const int EXPONENT_BITS = 8;
const int MANTISSA_BITS = 23;
const int EXPONENT_BIAS = 127;
const unsigned int MANTISSA_MASK = 0x7FFFFF;
const unsigned int EXPONENT_MASK = 0xFF;
const unsigned int HIDDEN_BIT = 1 << MANTISSA_BITS;
const unsigned int OVERFLOW_BIT = 1 << (MANTISSA_BITS + 1);

int fromTwoComplementToDecimal(const string& binary) {
    if (binary.length() != BITS) {
        throw invalid_argument("Длина строки должна быть 32 бита.");
    }

    bool isNegative = (binary[0] == '1');

    int decimal = 0;
    for (int i = 1; i < BITS; ++i) {
        if (binary[i] == '1') {
            decimal += pow(2, BITS - 1 - i);
        }
    }

    return isNegative ? decimal - pow(2, BITS - 1) : decimal;
}

int fromBinaryToDecimal(const string& binary) {

    if (binary.length() != 32) {
        throw invalid_argument("Длина строки должна быть 32 бита.");
    }

    bool isNegative = (binary[0] == '1');

    int decimal = 0;
    for (size_t i = 1; i < binary.length(); ++i) {
        if (binary[i] == '1') {
            decimal += pow(2, 31 - i); 
        }
    }

    return isNegative ? -decimal : decimal;
}

string toBinary(int num) {
    string result(BITS, '0');
    int absNum = abs(num);

    for (int i = BITS - 1; i > 0 && absNum > 0; --i) { 
        result[i] = (absNum % 2) + '0';
        absNum /= 2;
    }

    if (num < 0) {
        result[0] = '1'; 
    }

    return result;
}

string toBinary(double num) {
    string result = (num < 0) ? "1" : "0"; 
    num = abs(num);

    int intPart = static_cast<int>(num);
    double fracPart = num - intPart;

    string intBinary = "";
    for (int i = 0; i < BITS - 1; ++i) {
        if (intPart == 0) break;
        intBinary = to_string(intPart % 2) + intBinary;
        intPart /= 2;
    }

    string fracBinary = ".";
    for (int i = 0; i < 10 && fracPart > 0; ++i) { 
        fracPart *= 2;
        fracBinary += (fracPart >= 1) ? '1' : '0';
        fracPart -= static_cast<int>(fracPart);
    }

    return result + intBinary + fracBinary;
}

string toTwoComplement(int num) {
    string binary = toBinary(num);
    if (num >= 0) return binary;

    for (int i = 1; i < BITS; ++i) {
        binary[i] = (binary[i] == '0') ? '1' : '0';
    }

    for (int i = BITS - 1; i >= 0; --i) {
        if (binary[i] == '0') {
            binary[i] = '1';
            break;
        }
        binary[i] = '0';
    }
    return binary;
}

string toOneComplement(int num) {
    string binary = toBinary(num);
    if (num >= 0) return binary;

    for (int i = 1; i < BITS; ++i) {
        binary[i] = (binary[i] == '0') ? '1' : '0';
    }
    return binary;
} 

void addTwoNumbersInTwoComplement(int num1, int num2) {
    string binary1 = toTwoComplement(num1);
    string binary2 = toTwoComplement(num2);

    string result(BITS, '0');
    int carry = 0; 
    for (int i = BITS - 1; i >= 0; --i) {
        int bit1 = binary1[i] - '0';
        int bit2 = binary2[i] - '0';

        int sum = bit1 + bit2 + carry;
        result[i] = (sum % 2) + '0'; 
        carry = sum / 2;          
    }

    int decimalResult = fromTwoComplementToDecimal(result);

    cout << "Первое число: " << num1 << " (доп. код: " << binary1 << ")" << endl;
    cout << "Второе число: " << num2 << " (доп. код: " << binary2 << ")" << endl;
    cout << "Результат сложения в двоичном формате: " << result << endl;
    cout << "Результат сложения в десятичном формате: " << decimalResult << endl;
}

int multiply(int num1, int num2) {
    bool isNegative = (num1 < 0) ^ (num2 < 0);

    num1 = abs(num1);
    num2 = abs(num2);

    cout << "Первое число: " << num1 << " (прямой код: " << toBinary(num1) << ")" << endl;
    cout << "Второе число: " << num2 << " (прямой код: " << toBinary(num2) << ")" << endl;

    int result = 0;

    for (int i = 0; i < BITS; ++i) {
        if (num2 & (1 << i)) { 
            result += (num1 << i); 
        }
    }

    cout << "Результат умножения в десятичном формате: " << (isNegative ? -result : result) << endl;
    cout << "Результат умножения в двоичном формате: " << toBinary(isNegative ? -result : result) << endl;

    return isNegative ? -result : result;
}

double divide(int dividend, int divisor) {

    bool sign_a = dividend < 0;
    bool sign_b = divisor < 0;
    bool result_sign = sign_a != sign_b; 

    dividend = abs(dividend);
    divisor = abs(divisor);

    int integer_part = dividend / divisor; 
    int remainder = dividend % divisor;   

    double fractional_part = 0.0;
    double base = 1.0;
    for (int i = 0; i < 5; i++) {
        base /= 2; 
        remainder *= 2;
        if (remainder >= divisor) {
            fractional_part += base;
            remainder -= divisor;
        }
    }

    double result = integer_part + fractional_part;
    if (result_sign) {
        result = -result;
    }

    cout << "Результат деления (десятичный): " << result << endl;

    string binaryResult = toBinary(result);
    cout << "Результат деления (бинарный): " << binaryResult << endl;

    return result;
}

void printIEEE754(ieee754 binary) {
    cout << ((binary >> SIGN_BIT) & 1) << "  ";
    for (int i = SIGN_BIT - 1; i >= MANTISSA_BITS; i--) {
        cout << ((binary >> i) & 1);
    }
    cout << "  ";
    for (int i = MANTISSA_BITS - 1; i >= 0; i--) {
        cout << ((binary >> i) & 1);
    }
    cout << endl;
}

ieee754 floatToIEEE754(float num) {
    if (num == 0.0f) return 0;

    unsigned int sign = 0;
    if (num < 0) {
        sign = 1;
        num = -num;
    }

    int exponent = 0;
    while (num >= 2.0f) { num /= 2; exponent++; }
    while (num < 1.0f && num > 0.0f) { num *= 2; exponent--; }

    exponent += EXPONENT_BIAS;

    num -= 1.0f;
    unsigned int mantissa = 0;
    for (int i = 0; i < MANTISSA_BITS; i++) {
        num *= 2;
        if (num >= 1.0f) {
            mantissa |= (1 << (MANTISSA_BITS - 1 - i));
            num -= 1.0f;
        }
    }

    ieee754 result = (sign << SIGN_BIT) | (exponent << MANTISSA_BITS) | mantissa;
    printIEEE754(result);
    return result;
}

float ieee754ToFloat(ieee754 binary) {
    if (binary == 0) return 0.0f;

    int sign = (binary >> SIGN_BIT) & 1;
    int exponent = ((binary >> MANTISSA_BITS) & EXPONENT_MASK) - EXPONENT_BIAS;
    unsigned int mantissa = binary & MANTISSA_MASK;

    float value = 1.0f;
    for (int i = 0; i < MANTISSA_BITS; i++) {
        if (mantissa & (1 << (MANTISSA_BITS - 1 - i))) {
            value += (1.0f / (1 << (i + 1)));
        }
    }

    float result = value * (1 << exponent);
    return sign ? -result : result;
}

ieee754 ieee754Add(ieee754 a, ieee754 b) {
    int exponentA = (a >> MANTISSA_BITS) & EXPONENT_MASK;
    int exponentB = (b >> MANTISSA_BITS) & EXPONENT_MASK;
    unsigned int mantissaA = (a & MANTISSA_MASK) | HIDDEN_BIT;
    unsigned int mantissaB = (b & MANTISSA_MASK) | HIDDEN_BIT;

    if (exponentA > exponentB) {
        mantissaB >>= (exponentA - exponentB);
        exponentB = exponentA;
    }
    else if (exponentB > exponentA) {
        mantissaA >>= (exponentB - exponentA);
        exponentA = exponentB;
    }

    unsigned int mantissaSum = mantissaA + mantissaB;

    if (mantissaSum & OVERFLOW_BIT) {
        mantissaSum >>= 1;
        exponentA++;
    }

    mantissaSum &= MANTISSA_MASK;

    ieee754 result = (exponentA << MANTISSA_BITS) | mantissaSum;
    printIEEE754(result);
    return result;
}

void Menu() {
    setlocale(LC_ALL, "ru");
    int choice;
    do {
        cout << "Выберите операцию:\n";
        cout << "1 - Перевод числа в прямой, обратный и дополнительный коды\n";
        cout << "2 - Сложение в дополнительном коде\n";
        cout << "3 - Вычитание через сложение с дополнительным кодом\n";
        cout << "4 - Умножение в прямом коде\n";
        cout << "5 - Деление в прямом коде (точность 5 знаков)\n";
        cout << "6 - Сложение чисел с плавающей точкой IEEE-754 (32 бита)\n";
        cout << "0 - Выход\n";
        cout << "Введите номер операции: ";
        while (!(cin >> choice)) {
            cout << "Ошибка: Введите целое число.\n";
            cin.clear();        
            cin.ignore(10000, '\n'); 
        }

        if (choice == 1) {
            int num;
            cout << "Введите число: ";

            while (!(cin >> num)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();           
                cin.ignore(10000, '\n'); 
            }

            string binary = toBinary(num);
            string oneComp = toOneComplement(num);
            string twoComp = toTwoComplement(num);

            cout << "Число введено: " << num << endl;
            cout << "Прямой код:         " << binary << endl;
            cout << "Обратный код:       " << oneComp << endl;
            cout << "Дополнительный код: " << twoComp << endl;
        }

        else if (choice == 2) {
            int num1, num2;
            cout << "Введите первое число: ";
            while (!(cin >> num1)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();           
                cin.ignore(10000, '\n'); 
            }
            cout << "Введите второе число: ";
            while (!(cin >> num2)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();           
                cin.ignore(10000, '\n'); 
            }
            addTwoNumbersInTwoComplement(num1, num2);
        }
        else if (choice == 3) {
            int num1, num2;
            cout << "Введите уменьшаемое: ";
            while (!(cin >> num1)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();            
                cin.ignore(10000, '\n'); 
            }
            cout << "Введите вычитаемое: ";
            while (!(cin >> num2)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();            
                cin.ignore(10000, '\n'); 
            }
            addTwoNumbersInTwoComplement(num1, -num2);
        }
        else if (choice == 4) {
            int num1, num2;
            cout << "Введите первое число: ";
            while (!(cin >> num1)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();            
                cin.ignore(10000, '\n'); 
            }
            cout << "Введите второе число: ";
            while (!(cin >> num2)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();            
                cin.ignore(10000, '\n'); 
            }
            multiply(num1, num2);
        }
        else if (choice == 5) {
            int dividend, divisor;
            cout << "Введите делимое: ";
            while (!(cin >> dividend)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();            
                cin.ignore(10000, '\n'); 
            }
            cout << "Введите делитель: ";
            while (!(cin >> divisor)) {
                cout << "Ошибка: Введите целое число в диапазоне [-2147483648, 2147483647]: ";
                cin.clear();           
                cin.ignore(10000, '\n'); 
            }
            try {
                divide(dividend, divisor);
            }
            catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }
        else if (choice == 6) { 
            float num1, num2;
            cout << "Введите первое число с плавающей точкой: ";
            cin >> num1;
            ieee754 ieee1 = floatToIEEE754(num1);
            cout << "Введите второе число с плавающей точкой: ";
            cin >> num2;
            ieee754 ieee2 = floatToIEEE754(num2);

            cout << "Результат сложения (бинарный): ";
            ieee754 ieeeSum = ieee754Add(ieee1, ieee2);
            float sum = ieee754ToFloat(ieeeSum);

            cout << "Результат сложения (десятичный): " << sum << endl;
        }

    } while (choice != 0);

}

int main() {
    setlocale(LC_ALL, "ru");
    Menu();
    return 0;
}