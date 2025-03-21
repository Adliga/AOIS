#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>

using namespace std;

bool isOperator(char ch) {
    return ch == '&' || ch == '|' || ch == '!' || ch == '-' ||  ch == '~';
}

int getPrecedence(const string& op) {
    if (op == "!") return 4;
    if (op == "&") return 3;
    if (op == "|") return 2;
    if (op == "->") return 1;
    if (op == "~") return 0;
    return -1;
}

void validateExpression(const string& expr) {
    set<char> variableSet;
    bool hasOperator = false;
    bool lastWasVariable = false;
    bool lastWasOperator = false;
    int balance = 0;

    for (size_t i = 0; i < expr.size(); ++i) {
        char ch = expr[i];

        if (isspace(ch)) continue;

        if (!(isalpha(ch) || isOperator(ch) || ch == '(' || ch == ')' || ch == '-' || ch == '>')) {
            throw invalid_argument("Ошибка: Обнаружен недопустимый символ '" + string(1, ch) + "'.");
        }

        if (ch == '<' || (ch == '-' && (i + 1 >= expr.size() || expr[i + 1] != '>'))) {
            throw invalid_argument("Ошибка: Найдена некорректная последовательность символов.");
        }

        if (ch == '(') balance++;
        if (ch == ')') balance--;

        if (balance < 0) {
            throw invalid_argument("Ошибка: Несбалансированные скобки.");
        }

        if (isalpha(ch)) {
            if (lastWasVariable) {
                throw invalid_argument("Ошибка: Переменные должны разделяться операторами.");
            }
            variableSet.insert(ch);
            lastWasVariable = true;
            lastWasOperator = false;
        }
        else if (isOperator(ch)) {
            if (ch != '!') {
                if (lastWasOperator) {
                    throw invalid_argument("Ошибка: Два оператора не могут идти подряд.");
                }
                hasOperator = true;
            }
            else {
                if (i + 1 >= expr.size() || !(isalpha(expr[i + 1]) || expr[i + 1] == '(')) {
                    throw invalid_argument("Ошибка: Оператор '!' должен стоять перед переменной, или '(' в одиночном числе.");
                }
            }
            lastWasVariable = false;
            lastWasOperator = true;
        }
        else {
            lastWasOperator = false;
        }
    }

    if (balance != 0) {
        throw invalid_argument("Ошибка: Несбалансированные скобки.");
    }

    if (variableSet.empty()) {
        throw invalid_argument("Ошибка: В выражении должны быть переменные.");
    }

    if (variableSet.size() > 5) {
        throw invalid_argument("Ошибка: В выражении не может быть более 5 различных переменных.");
    }

    if (!hasOperator) {
        throw invalid_argument("Ошибка: В выражении должна быть хотя бы одна логическая операция.");
    }

    if (lastWasOperator && expr.back() != '!') {
        throw invalid_argument("Ошибка: Выражение не может заканчиваться оператором.");
    }
}

vector<string> toRPN(const string& expr) {
    validateExpression(expr);  

    stack<string> operators;
    vector<string> output;

    string token;
    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];

        if (isspace(ch)) continue;

        if (isalpha(ch)) {
            output.push_back(string(1, ch));
        }
        else if (ch == '(') {
            operators.push("(");
        }
        else if (ch == ')') {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop();  
        }
        else if (isOperator(ch)) {
            string op(1, ch);
            if (ch == '-' && i + 1 < expr.length() && expr[i + 1] == '>') {
                op = "->";
                ++i;
            }
            while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(op)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(op);
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

vector<pair<string, vector<bool>>> evaluateRPN(const vector<string>& rpn, const vector<char>& variables, const vector<vector<bool>>& allValues) {
    vector<pair<string, vector<bool>>> results; 
    unordered_map<string, size_t> exprIndex; 

    for (size_t row = 0; row < allValues.size(); ++row) {
        map<char, bool> values;
        for (size_t j = 0; j < variables.size(); ++j) {
            values[variables[j]] = allValues[row][j];
        }
        stack<bool> tempStack;
        stack<string> exprStack;
        
        for (const string& token : rpn) {
            if (isalpha(token[0])) {
                tempStack.push(values[token[0]]);
                exprStack.push(token);
            }
            else if (token == "!") {
                bool a = tempStack.top(); tempStack.pop();
                string subExpr = "!" + exprStack.top(); exprStack.pop();
                tempStack.push(!a);
                exprStack.push(subExpr);
                if (exprIndex.find(subExpr) == exprIndex.end()) {
                    exprIndex[subExpr] = results.size();
                    results.push_back({ subExpr, vector<bool>(allValues.size()) });
                }
                results[exprIndex[subExpr]].second[row] = !a;
            }
            else {
                bool a = tempStack.top(); tempStack.pop();
                string exprA = exprStack.top(); exprStack.pop();
                bool b = tempStack.top(); tempStack.pop();
                string exprB = exprStack.top(); exprStack.pop();
                bool result;
                string subExpr;
                if (token == "&") {
                    result = a && b;
                    subExpr = "(" + exprB + "&" + exprA + ")";
                }
                else if (token == "|") {
                    result = a || b;
                    subExpr = "(" + exprB + "|" + exprA + ")";
                }
                else if (token == "->") {
                    result = !b || a;
                    subExpr = "(" + exprB + "->" + exprA + ")";
                }
                else if (token == "~") {
                    result = a == b;
                    subExpr = "(" + exprB + "~" + exprA + ")";
                }
                tempStack.push(result);
                exprStack.push(subExpr);
                if (exprIndex.find(subExpr) == exprIndex.end()) {
                    exprIndex[subExpr] = results.size();
                    results.push_back({ subExpr, vector<bool>(allValues.size()) });
                }
                results[exprIndex[subExpr]].second[row] = result;
            }
        }
    }
    return results;
}

string centerText(const string& text, int width) {
    int padding = width - text.size();
    int padLeft = padding / 2;
    int padRight = padding - padLeft;
    return string(padLeft, ' ') + text + string(padRight, ' ');
}

string constructSDNF(const vector<char>& variables, const vector<int>& minterms) {
    string sdnf;
    for (int row : minterms) {
        if (!sdnf.empty()) sdnf += " | ";
        sdnf += "(";
        for (size_t i = 0; i < variables.size(); ++i) {
            if (row & (1 << (variables.size() - 1 - i)))
                sdnf += string(1, variables[i]);
            else
                sdnf += "!" + string(1, variables[i]);
            if (i < variables.size() - 1) sdnf += " & ";
        }
        sdnf += ")";
    }
    return sdnf;
}

string constructSKNF(const vector<char>& variables, const vector<int>& maxterms) {
    string sknf;
    for (int row : maxterms) {
        if (!sknf.empty()) sknf += " & ";
        sknf += "(";
        for (size_t i = 0; i < variables.size(); ++i) {
            if (row & (1 << (variables.size() - 1 - i)))
                sknf += "!" + string(1, variables[i]);
            else
                sknf += string(1, variables[i]);
            if (i < variables.size() - 1) sknf += " | ";
        }
        sknf += ")";
    }
    return sknf;
}

string joinNumbers(const vector<int>& numbers) {
    string result;
    for (size_t i = 0; i < numbers.size(); ++i) {
        if (i > 0) result += ", ";
        result += to_string(numbers[i]);
    }
    return result;
}

int binaryToDecimal(const vector<bool>& binary) {
    int decimal = 0;
    for (bool bit : binary) {
        decimal = (decimal << 1) | bit;
    }
    return decimal;
}

void printTruthTable(const string& expr) {
    vector<string> rpn = toRPN(expr);
    set<char> variableSet;

    for (char ch : expr) {
        if (isalpha(ch))
            variableSet.insert(ch);
    }
    
    vector<char> variables(variableSet.begin(), variableSet.end());
    size_t numRows = 1 << variables.size();
    vector<vector<bool>> allValues(numRows, vector<bool>(variables.size()));

    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < variables.size(); ++j) {
            allValues[i][j] = (i >> (variables.size() - 1 - j)) & 1;
        }
    }

    vector<pair<string, vector<bool>>> results = evaluateRPN(rpn, variables, allValues);

    size_t finalIndex = 0, maxLen = 0;
    for (size_t i = 0; i < results.size(); ++i) {
        if (results[i].first.length() > maxLen) {
            maxLen = results[i].first.length();
            finalIndex = i;
        }
    }
    pair<string, vector<bool>> finalExpr = results[finalIndex];
    results.erase(results.begin() + finalIndex);
    results.push_back(finalExpr);

    int varWidth = 3; 
    vector<int> exprWidths;
    for (const auto& pr : results) {
        int width = max((int)pr.first.size(), 3);
        exprWidths.push_back(width);
    }

    cout << "\nТаблица истинности для функции: " << expr << "\n";

    for (char var : variables)
        cout << centerText(string(1, var), varWidth) << " | ";

    for (size_t i = 0; i < results.size(); i++) {
        cout << centerText(results[i].first, exprWidths[i]);
        if (i != results.size() - 1)
            cout << " | ";
    }
    cout << "\n";

    for (size_t i = 0; i < variables.size(); i++) {
        cout << string(varWidth, '-');
        cout << " | ";
    }
    for (size_t i = 0; i < results.size(); i++) {
        cout << string(exprWidths[i], '-');
        if (i != results.size() - 1)
            cout << " | ";
    }
    cout << "\n";

    vector<int> minterms, maxterms;
    vector<bool> binaryForm;
    for (size_t i = 0; i < numRows; i++) {

        for (size_t j = 0; j < variables.size(); j++) {
            string val = to_string(allValues[i][j]);
            cout << centerText(val, varWidth) << " | ";
        }

        for (size_t k = 0; k < results.size(); k++) {
            string val = to_string(results[k].second[i]);
            cout << centerText(val, exprWidths[k]);
            if (k != results.size() - 1)
                cout << " | ";
        }
        cout << "\n";

        binaryForm.push_back(finalExpr.second[i]);
        if (finalExpr.second[i] == 1) minterms.push_back(i);
        else maxterms.push_back(i);
    }

    int decimalIndexForm = binaryToDecimal(binaryForm);

    cout << "\nСДНФ: " << constructSDNF(variables, minterms) << "\n";
    cout << "СКНФ: " << constructSKNF(variables, maxterms) << "\n";
    cout << "Числовая форма СДНФ: ( " << joinNumbers(minterms) << " )|\n";
    cout << "Числовая форма СКНФ: ( " << joinNumbers(maxterms) << " )&\n";
    cout << "Индексная форма функции: " << decimalIndexForm << " (бинарное: ";
    for (bool bit : binaryForm) cout << bit;
    cout << ")\n";
}

int main() {
    setlocale(LC_ALL, "ru");
    string input;
    while (true) {
        cout << "Введите логическую функцию (или 'exit' для выхода): ";
        getline(cin, input);
        if (input == "exit") break;

        try {
            printTruthTable(input);
        }
        catch (const exception& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}

