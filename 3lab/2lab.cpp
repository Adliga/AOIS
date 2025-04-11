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
    return ch == '&' || ch == '|' || ch == '!' || ch == '-' || ch == '~';
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
            throw invalid_argument("������: ��������� ������������ ������ '" + string(1, ch) + "'.");
        }

        if (ch == '<' || (ch == '-' && (i + 1 >= expr.size() || expr[i + 1] != '>'))) {
            throw invalid_argument("������: ������� ������������ ������������������ ��������.");
        }

        if (ch == '(') balance++;
        if (ch == ')') balance--;

        if (balance < 0) {
            throw invalid_argument("������: ������������������ ������.");
        }

        if (isalpha(ch)) {
            if (lastWasVariable) {
                throw invalid_argument("������: ���������� ������ ����������� �����������.");
            }
            variableSet.insert(ch);
            lastWasVariable = true;
            lastWasOperator = false;
        }
        else if (isOperator(ch)) {
            if (ch != '!') {
                if (lastWasOperator) {
                    throw invalid_argument("������: ��� ��������� �� ����� ���� ������.");
                }
                hasOperator = true;
            }
            else {
                if (i + 1 >= expr.size() || !(isalpha(expr[i + 1]) || expr[i + 1] == '(')) {
                    throw invalid_argument("������: �������� '!' ������ ������ ����� ����������, ��� '(' � ��������� �����.");
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
        throw invalid_argument("������: ������������������ ������.");
    }

    if (variableSet.empty()) {
        throw invalid_argument("������: � ��������� ������ ���� ����������.");
    }

    if (variableSet.size() > 5) {
        throw invalid_argument("������: � ��������� �� ����� ���� ����� 5 ��������� ����������.");
    }

    if (lastWasOperator && expr.back() != '!') {
        throw invalid_argument("������: ��������� �� ����� ������������� ����������.");
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
        if (!sdnf.empty()) sdnf += "|";
        sdnf += "(";
        for (size_t i = 0; i < variables.size(); ++i) {
            if (row & (1 << (variables.size() - 1 - i)))
                sdnf += string(1, variables[i]);
            else
                sdnf += "!" + string(1, variables[i]);
            if (i < variables.size() - 1) sdnf += "&";
        }
        sdnf += ")";
    }
    return sdnf; //////////////////////////////////////////////////////////////////
}

string constructSKNF(const vector<char>& variables, const vector<int>& maxterms) {
    string sknf;
    for (int row : maxterms) {
        if (!sknf.empty()) sknf += "&";
        sknf += "(";
        for (size_t i = 0; i < variables.size(); ++i) {
            if (row & (1 << (variables.size() - 1 - i)))
                sknf += "!" + string(1, variables[i]);
            else
                sknf += string(1, variables[i]);
            if (i < variables.size() - 1) sknf += "|";
        }
        sknf += ")";
    }
    return sknf;//////////////////////////////////////////////////////////////////
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

vector<char> extractVariables(const string& expr) {
    set<char> variableSet;
    for (char ch : expr) {
        if (isalpha(ch))
            variableSet.insert(ch);
    }
    return vector<char>(variableSet.begin(), variableSet.end());
}

vector<vector<bool>> generateVariableCombinations(const vector<char>& variables) {
    size_t numRows = 1 << variables.size();
    vector<vector<bool>> allValues(numRows, vector<bool>(variables.size()));

    for (size_t i = 0; i < numRows; ++i) {
        for (size_t j = 0; j < variables.size(); ++j) {
            allValues[i][j] = (i >> (variables.size() - 1 - j)) & 1;
        }
    }
    return allValues;
}

vector<pair<string, vector<bool>>> evaluateExpressions(const vector<string>& rpn, const vector<char>& variables, const vector<vector<bool>>& allValues) {
    return evaluateRPN(rpn, variables, allValues);
}

pair<string, vector<bool>> selectFinalExpression(vector<pair<string, vector<bool>>>& results) {
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
    return finalExpr;
}

void printTableHeader(const vector<char>& variables, const vector<pair<string, vector<bool>>>& results, const vector<int>& exprWidths) {
    int varWidth = 3;

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
}

void printTableData(const vector<char>& variables, const vector<pair<string, vector<bool>>>& results,
    const vector<vector<bool>>& allValues, vector<bool>& binaryForm, vector<int>& minterms, vector<int>& maxterms) {
    int varWidth = 3;
    vector<int> exprWidths;
    for (const auto& pr : results)
        exprWidths.push_back(max((int)pr.first.size(), 3));

    for (size_t i = 0; i < allValues.size(); i++) {
        for (size_t j = 0; j < variables.size(); j++) {
            cout << centerText(to_string(allValues[i][j]), varWidth) << " | ";
        }

        for (size_t k = 0; k < results.size(); k++) {
            cout << centerText(to_string(results[k].second[i]), exprWidths[k]);
            if (k != results.size() - 1)
                cout << " | ";
        }
        cout << "\n";

        binaryForm.push_back(results.back().second[i]);
        if (results.back().second[i] == 1) minterms.push_back(i);
        else maxterms.push_back(i);
    }
}

void printCanonicalForms(const vector<char>& variables, const vector<int>& minterms, const vector<int>& maxterms, const vector<bool>& binaryForm) {
    int decimalIndexForm = binaryToDecimal(binaryForm);

    cout << "\n����: " << constructSDNF(variables, minterms) << "\n";
    cout << "����: " << constructSKNF(variables, maxterms) << "\n";
    cout << "�������� ����� ����: ( " << joinNumbers(minterms) << " )|\n";
    cout << "�������� ����� ����: ( " << joinNumbers(maxterms) << " )&\n";
    cout << "��������� ����� �������: " << decimalIndexForm << " (��������: ";
    for (bool bit : binaryForm) cout << bit;
    cout << ")\n";
}

void printTruthTable(const string& expr) {
    vector<string> rpn = toRPN(expr);
    vector<char> variables = extractVariables(expr);
    vector<vector<bool>> allValues = generateVariableCombinations(variables);

    vector<pair<string, vector<bool>>> results = evaluateExpressions(rpn, variables, allValues);
    pair<string, vector<bool>> finalExpr = selectFinalExpression(results);

    vector<int> exprWidths;
    for (const auto& pr : results)
        exprWidths.push_back(max((int)pr.first.size(), 3));

    cout << "\n������� ���������� ��� �������: " << expr << "\n";
    printTableHeader(variables, results, exprWidths);

    vector<int> minterms, maxterms;
    vector<bool> binaryForm;
    printTableData(variables, results, allValues, binaryForm, minterms, maxterms);

    printCanonicalForms(variables, minterms, maxterms, binaryForm);
}