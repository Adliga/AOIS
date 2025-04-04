#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <algorithm>
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\functions.cpp" 
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\minimizeTabular.cpp" 
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\minimizeQuine.cpp" 
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\minimizeKarnaugh.cpp" 

bool isValidDNF(const string& expr) {

    int openParen = 0, closeParen = 0;
    bool isInClause = false;

    for (char ch : expr) {
        if (ch == '(') {
            openParen++;
            if (isInClause) return false; 
            isInClause = true;
        }
        else if (ch == ')') {
            closeParen++;
            if (!isInClause) return false; 
            isInClause = false;
        }
        else if (!isalpha(ch) && ch != '!' && ch != '|') {
            return false; 
        }
    }

    return openParen == closeParen && !isInClause; 
}

bool isValidCNF(const string& expr) {

    int openParen = 0, closeParen = 0;
    bool isInClause = false;

    for (char ch : expr) {
        if (ch == '(') {
            openParen++;
            if (isInClause) return false; 
            isInClause = true;
        }
        else if (ch == ')') {
            closeParen++;
            if (!isInClause) return false; 
            isInClause = false;
        }
        else if (!isalpha(ch) && ch != '!' && ch != '|' && ch != '&') {
            return false; 
        }
    }

    return openParen == closeParen && !isInClause; 
}

int main() {
    setlocale(LC_ALL, "Russian");
    string input;
    int choice;

    while (true) {
        cout << "|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|\n"; 
        cout << "Выберите тип ввода:\n";
        cout << "1. СДНФ\n2. СКНФ\n0. Выход\nВаш выбор: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        cout << "Введите выражение: ";
        getline(cin, input);

        bool isDNF = (choice == 1);

        if ((isDNF && !isValidDNF(input)) || (!isDNF && !isValidCNF(input))) {
            cout << "Ошибка: введено выражение некорректного формата.\n";
            continue;
        }

        cout << "\nМинимизация расчётным методом (методом простых преобразований):\n";
        minimize(input, isDNF);

        cout << "\nМинимизация расчётно-табличным методом (методом Квайна-МакКласки):\n";
        minimizeQuine(input, isDNF);

        cout << "\nМинимизация табличным методом (по карте Карно):\n";
        minimizeKarnaugh(input, isDNF);
    }

    return 0;
}






