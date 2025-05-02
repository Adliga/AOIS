#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>

using namespace std;

struct Term {
    string vars;
    bool used;
    set<string> coveredTerms;

    Term(string v) : vars(v), used(false) {}
};

bool canGlue(const string& t1, const string& t2, int varCount) {
    int diff = 0;
    for (int i = 0; i < varCount; i++) {
        if (t1[i] != t2[i]) diff++;
        if (diff > 1) return false;
    }
    return diff == 1;
}

string glue(const string& t1, const string& t2) {
    string result = t1;
    for (int i = 0; i < t1.length(); i++) {
        if (t1[i] != t2[i]) result[i] = '-';
    }
    return result;
}

string formatDNFterm(const string& term, const vector<char>& varNames) {
    string result;
    bool first = true;
    for (int i = 0; i < term.length(); i++) {
        if (term[i] == '-') continue;
        if (!first) result += "&";
        if (term[i] == '0') result += "!" + string(1, varNames[i]);
        else if (term[i] == '1') result += string(1, varNames[i]);
        first = false;
    }
    return result.empty() ? "1" : result;
}

vector<Term> glueTerms(vector<Term> terms, const vector<char>& varNames) {
    cout << "Стадия склеивания:\n";
    int varCount = varNames.size();
    bool glued;
    do {
        glued = false;
        set<string> newTerms;
        vector<bool> used(terms.size(), false);

        for (int i = 0; i < terms.size(); i++) {
            for (int j = i + 1; j < terms.size(); j++) {
                if (canGlue(terms[i].vars, terms[j].vars, varCount)) {
                    string gluedTerm = glue(terms[i].vars, terms[j].vars);
                    newTerms.insert(gluedTerm);
                    used[i] = used[j] = true;
                    glued = true;
                    cout << "(" << formatDNFterm(terms[i].vars, varNames) << ") | ("
                        << formatDNFterm(terms[j].vars, varNames) << ") => ("
                        << formatDNFterm(gluedTerm, varNames) << ")\n";
                }
            }
        }

        for (int i = 0; i < terms.size(); i++) {
            if (!used[i]) newTerms.insert(terms[i].vars);
        }

        terms.clear();
        for (const auto& t : newTerms) {
            terms.push_back(Term(t));
        }
    } while (glued);

    return terms;
}

string parseDNFterm(const string& term, vector<char>& varNames) {
    string result;
    set<char> foundVars;
    for (size_t i = 0; i < term.length(); i++) {
        if (term[i] == '!') {
            char var = term[i + 1];
            foundVars.insert(var);
            result += '0';
            if (find(varNames.begin(), varNames.end(), var) == varNames.end())
                varNames.push_back(var);
            i++;
        }
        else if (isalpha(term[i])) {
            char var = term[i];
            foundVars.insert(var);
            result += '1';
            if (find(varNames.begin(), varNames.end(), var) == varNames.end())
                varNames.push_back(var);
        }
        else if (term[i] == '&') {
            continue;
        }
    }
    return result;
}

vector<Term> parseFunction(const string& input, vector<char>& varNames) {
    vector<Term> terms;
    stringstream ss(input);
    string term;
    while (getline(ss, term, '|')) {
        term.erase(remove_if(term.begin(), term.end(), isspace), term.end());
        if (term.empty()) continue;
        Term t(parseDNFterm(term, varNames));
        t.coveredTerms.insert(t.vars);
        terms.push_back(t);
    }
    return terms;
}

// Часть 1: Одноразрядный двоичный сумматор на 3 входа
void buildAdder() {
    cout << "\n=== Одноразрядный двоичный сумматор на 3 входа ===\n";
    vector<char> varNames = { 'A', 'B', 'C' };
    vector<string> sdnf_S, sdnf_Cout;

    // Таблица истинности для S и Cout
    cout << "Таблица истинности:\n";
    cout << "A B C | S Cout\n";
    cout << "------+-------\n";
    for (int A = 0; A < 2; A++) {
        for (int B = 0; B < 2; B++) {
            for (int C = 0; C < 2; C++) {
                int sum = A + B + C;
                int S = sum % 2; // Сумма по модулю 2
                int Cout = sum / 2; // Перенос
                cout << A << " " << B << " " << C << " | " << S << " " << Cout << "\n";

                // СДНФ для S
                if (S == 1) {
                    string term = "";
                    term += (A ? "A" : "!A") + string("&");
                    term += (B ? "B" : "!B") + string("&");
                    term += (C ? "C" : "!C");
                    sdnf_S.push_back(term);
                }

                // СДНФ для Cout
                if (Cout == 1) {
                    string term = "";
                    term += (A ? "A" : "!A") + string("&");
                    term += (B ? "B" : "!B") + string("&");
                    term += (C ? "C" : "!C");
                    sdnf_Cout.push_back(term);
                }
            }
        }
    }

    // СДНФ для S
    string sdnf_S_str = "";
    for (size_t i = 0; i < sdnf_S.size(); i++) {
        sdnf_S_str += "(" + sdnf_S[i] + ")";
        if (i < sdnf_S.size() - 1) sdnf_S_str += "|";
    }
    cout << "\nСДНФ для S: " << sdnf_S_str << "\n";

    // Минимизация S
    vector<char> varNames_S = varNames;
    vector<Term> terms_S = parseFunction(sdnf_S_str, varNames_S);
    vector<Term> minimized_S = glueTerms(terms_S, varNames_S);
    cout << "Минимизированная форма S: ";
    for (size_t i = 0; i < minimized_S.size(); i++) {
        cout << "(" << formatDNFterm(minimized_S[i].vars, varNames_S) << ")";
        if (i < minimized_S.size() - 1) cout << "|";
    }
    cout << "\n";

    // СДНФ для Cout
    string sdnf_Cout_str = "";
    for (size_t i = 0; i < sdnf_Cout.size(); i++) {
        sdnf_Cout_str += "(" + sdnf_Cout[i] + ")";
        if (i < sdnf_Cout.size() - 1) sdnf_Cout_str += "|";
    }
    cout << "\nСДНФ для Cout: " << sdnf_Cout_str << "\n";

    // Минимизация Cout
    vector<char> varNames_Cout = varNames;
    vector<Term> terms_Cout = parseFunction(sdnf_Cout_str, varNames_Cout);
    vector<Term> minimized_Cout = glueTerms(terms_Cout, varNames_Cout);
    cout << "Минимизированная форма Cout: ";
    for (size_t i = 0; i < minimized_Cout.size(); i++) {
        cout << "(" << formatDNFterm(minimized_Cout[i].vars, varNames_Cout) << ")";
        if (i < minimized_Cout.size() - 1) cout << "|";
    }
    cout << "\n";
}

// Преобразование десятичного числа в код Д8421
vector<int> toBCD(int decimal) {
    decimal = decimal % 10; // Берем остаток от деления на 10
    vector<int> bcd(4);
    bcd[0] = (decimal >> 3) & 1; // A
    bcd[1] = (decimal >> 2) & 1; // B
    bcd[2] = (decimal >> 1) & 1; // C
    bcd[3] = decimal & 1;        // D
    return bcd;
}

// Вывод таблицы истинности и СДНФ с минимизацией для Д8421+6
void printBCDConverterTableAndDNF() {
    cout << "\n=== Преобразователь Д8421 в Д8421+6 ===\n";
    vector<char> varNames = { 'A', 'B', 'C', 'D' };
    vector<string> sdnf_Y1, sdnf_Y2, sdnf_Y3, sdnf_Y4;

    // Таблица истинности в стиле zachar.txt
    cout << "D8421\t\tD8421+6\n";
    cout << "A B C D\t\tY1 Y2 Y3 Y4\n";
    cout << "--------------------------\n";

    for (int i = 0; i < 10; i++) {
        vector<int> bits = toBCD(i); // Входной код Д8421
        int A = bits[0], B = bits[1], C = bits[2], D = bits[3];
        string term = "";
        term += (A ? "A" : "!A") + string("&");
        term += (B ? "B" : "!B") + string("&");
        term += (C ? "C" : "!C") + string("&");
        term += (D ? "D" : "!D");

        int result = (i + 6) % 10; // (i + 6) mod 10
        vector<int> output = toBCD(result);
        int Y1 = output[0], Y2 = output[1], Y3 = output[2], Y4 = output[3];
        cout << A << " " << B << " " << C << " " << D << "\t\t"
            << Y1 << "  " << Y2 << "  " << Y3 << "  " << Y4 << "\n";

        // СДНФ для Y1
        if (Y1 == 1) sdnf_Y1.push_back(term);
        // СДНФ для Y2
        if (Y2 == 1) sdnf_Y2.push_back(term);
        // СДНФ для Y3
        if (Y3 == 1) sdnf_Y3.push_back(term);
        // СДНФ для Y4
        if (Y4 == 1) sdnf_Y4.push_back(term);
    }

    //// Минимизация для каждого выхода
    //for (int output = 1; output <= 4; output++) {
    //    string sdnf_str;
    //    vector<string> sdnf;
    //    if (output == 1) sdnf = sdnf_Y1;
    //    else if (output == 2) sdnf = sdnf_Y2;
    //    else if (output == 3) sdnf = sdnf_Y3;
    //    else sdnf = sdnf_Y4;

    //    // Формирование СДНФ
    //    for (size_t i = 0; i < sdnf.size(); i++) {
    //        sdnf_str += "(" + sdnf[i] + ")";
    //        if (i < sdnf.size() - 1) sdnf_str += "|";
    //    }
    //    cout << "\nСДНФ для Y" << output << ": " << (sdnf_str.empty() ? "0" : sdnf_str) << "\n";

    //    // Минимизация
    //    if (!sdnf_str.empty()) {
    //        vector<char> varNames_Y = varNames;
    //        vector<Term> terms_Y = parseFunction(sdnf_str, varNames_Y);
    //        vector<Term> minimized_Y = glueTerms(terms_Y, varNames_Y);
    //        cout << "Минимизированная форма Y" << output << ": ";
    //        for (size_t i = 0; i < minimized_Y.size(); i++) {
    //            cout << "(" << formatDNFterm(minimized_Y[i].vars, varNames_Y) << ")";
    //            if (i < minimized_Y.size() - 1) cout << "|";
    //        }
    //        cout << "\n";
    //    }
    //    else {
    //        cout << "Минимизированная форма Y" << output << ": 0\n";
    //    }
    //}
}

// Обработка входного числа и вывод результата преобразования
void processBCDConversion(int inputNumber) {
    cout << "\n=== Обработка числа " << inputNumber << " ===\n";
    int decimal = inputNumber >= 0 ? inputNumber % 10 : 0; 
    int resultDecimal = (decimal + 6) % 10; // 
    vector<int> inputBits = toBCD(decimal);
    vector<int> outputBits = toBCD(resultDecimal);

    cout << "Входное число: " << inputNumber << " (используется: " << decimal
        << ", результат: " << resultDecimal << ")\n";
    cout << "Вход (Д8421): " << inputBits[0] << " " << inputBits[1] << " "
        << inputBits[2] << " " << inputBits[3] << "\n";
    cout << "Выход (Д8421+6): " << outputBits[0] << " " << outputBits[1] << " "
        << outputBits[2] << " " << outputBits[3] << " (десятичное: " << resultDecimal << ")\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    buildAdder();
    printBCDConverterTableAndDNF(); 
    int inputNumber;
    while (true) {
        cout << "\nВведите целое число (для выхода введите отрицательное число): ";
        cin >> inputNumber;
        if (inputNumber < 0) break;
        processBCDConversion(inputNumber);
    }
    return 0;
}