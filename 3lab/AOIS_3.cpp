#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\minimizeKarnaugh.cpp"
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\minimizeQuine.cpp"
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\minimizeTabular.cpp"
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\2lab.cpp"

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
        else if (!isalpha(ch) && ch != '!' && ch != '|' && ch != '&') {
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
        cout << "Выберите тип минимизации:\n";
        cout << "1. СДНФ\n2. СКНФ\n0. Выход\nВаш выбор: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        cout << "Введите логическую функцию: ";
        getline(cin, input);

        try {
            cout << "\nТаблица истинности и канонические формы:\n";
            string canonicalForm;
            bool isDNF = (choice == 1);

            vector<char> variables = extractVariables(input);
            vector<vector<bool>> allValues = generateVariableCombinations(variables);
            vector<string> rpn = toRPN(input);
            vector<pair<string, vector<bool>>> results = evaluateExpressions(rpn, variables, allValues);
            pair<string, vector<bool>> finalExpr = selectFinalExpression(results);

            vector<int> minterms, maxterms;
            vector<bool> binaryForm;
            for (size_t i = 0; i < allValues.size(); ++i) {
                if (finalExpr.second[i]) {
                    minterms.push_back(i);
                }
                else {
                    maxterms.push_back(i);
                }
                binaryForm.push_back(finalExpr.second[i]);
            }

            if (isDNF) {
                canonicalForm = constructSDNF(variables, minterms);
            }
            else {
                canonicalForm = constructSKNF(variables, maxterms);
            }

            printTruthTable(input);

            if ((isDNF && !isValidDNF(canonicalForm)) || (!isDNF && !isValidCNF(canonicalForm))) {
                cout << "Ошибка: сгенерировано выражение некорректного формата.\n";
                continue;
            }

            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            cout << "\nКаноническая форма для минимизации (" << (isDNF ? "СДНФ" : "СКНФ") << "): " << canonicalForm << "\n";

            cout << "\nМинимизация расчётным методом (методом простых преобразований):\n";
            minimize(canonicalForm, isDNF);
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            cout << "\nМинимизация расчётно-табличным методом (методом Квайна-МакКласки):\n";
            minimizeQuine(canonicalForm, isDNF);
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

            cout << "\nМинимизация табличным методом (по карте Карно):\n";
            minimizeKarnaugh(canonicalForm, isDNF);
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}