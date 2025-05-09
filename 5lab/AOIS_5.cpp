#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <iomanip>

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

string formatDNFterm(const string& term, const vector<string>& varNames) {
    string result;
    bool first = true;
    for (int i = 0; i < term.length(); i++) {
        if (term[i] == '-') continue;
        if (!first) result += "&";
        if (term[i] == '0') result += "!" + varNames[i];
        else if (term[i] == '1') result += varNames[i];
        first = false;
    }
    return result.empty() ? "1" : result;
}

vector<Term> glueTerms(vector<Term> terms, const vector<string>& varNames) {
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
                    cout << "(" << formatDNFterm(terms[i].vars, varNames) << ") | ("//
                        << formatDNFterm(terms[j].vars, varNames) << ") => ("//
                        << formatDNFterm(gluedTerm, varNames) << ")\n";//
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

string parseDNFterm(const string& term, vector<string>& varNames) {
    string result;
    set<string> foundVars;
    for (size_t i = 0; i < term.length(); i++) {
        if (term[i] == '!') {
            string var = string(1, term[i + 1]) + string(1, term[i + 2]);
            foundVars.insert(var);
            result += '0';
            if (find(varNames.begin(), varNames.end(), var) == varNames.end())
                varNames.push_back(var);
            i += 2;
        }
        else if (term.substr(i, 2) == "Q3" || term.substr(i, 2) == "Q2" ||
            term.substr(i, 2) == "Q1" || term.substr(i, 2) == "Q0") {
            string var = term.substr(i, 2);
            foundVars.insert(var);
            result += '1';
            if (find(varNames.begin(), varNames.end(), var) == varNames.end())
                varNames.push_back(var);
            i += 1;
        }
        else if (term[i] == '&') {
            continue;
        }
    }
    return result;
}

vector<Term> parseFunction(const string& input, vector<string>& varNames) {
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

void printTruthTableHeader(const vector<string>& arguments, const int colWidth) {
    string header = "";
    stringstream ss;
    for (const auto& arg : arguments) {
        ss << setw(colWidth) << arg << "|";
    }
    header = ss.str();
    cout << header << "\n";
    cout << string(arguments.size() * (colWidth + 1) - 1, '-') << "\n";
}

void printTruthTableRow(int Q3, int Q2, int Q1, int Q0, int V,
    int Q3_next, int Q2_next, int Q1_next, int Q0_next,
    int h3, int h2, int h1, int h0, const int colWidth) {
    cout << setw(colWidth) << Q3 << "|"
        << setw(colWidth) << Q2 << "|"
        << setw(colWidth) << Q1 << "|"
        << setw(colWidth) << Q0 << "|"
        << setw(colWidth) << V << "|"
        << setw(colWidth) << Q3_next << "|"
        << setw(colWidth) << Q2_next << "|"
        << setw(colWidth) << Q1_next << "|"
        << setw(colWidth) << Q0_next << "|"
        << setw(colWidth) << h3 << "|"
        << setw(colWidth) << h2 << "|"
        << setw(colWidth) << h1 << "|"
        << setw(colWidth) << h0 << "\n";
}

string generateSDNFterm(int Q3, int Q2, int Q1, int Q0, int V) {
    string term = "";
    term += (Q3 ? "Q3" : "!Q3") + string("&");
    term += (Q2 ? "Q2" : "!Q2") + string("&");
    term += (Q1 ? "Q1" : "!Q1") + string("&");
    term += (Q0 ? "Q0" : "!Q0") + string("&");
    term += (V ? "V" : "!V");
    return term;
}

void minimizeFunction(const vector<string>& sdnf, int h, const vector<string>& varNames) {
    string sdnf_str;
    for (size_t i = 0; i < sdnf.size(); i++) {
        sdnf_str += "(" + sdnf[i] + ")";
        if (i < sdnf.size() - 1) sdnf_str += "|";
    }

    cout << "\nМинимизация h" << h << ":\n";
    cout << "СДНФ: " << (sdnf_str.empty() ? "0" : sdnf_str) << "\n";

    if (!sdnf_str.empty()) {
        vector<string> varNames_h = varNames;
        vector<Term> terms_h = parseFunction(sdnf_str, varNames_h);
        vector<Term> minimized_h = glueTerms(terms_h, varNames_h);
        cout << "Минимизированная форма в базисе НЕ, И, ИЛИ: ";
        for (size_t i = 0; i < minimized_h.size(); i++) {
            cout << "(" << formatDNFterm(minimized_h[i].vars, varNames_h) << ")";
            if (i < minimized_h.size() - 1) cout << "|";
        }
        cout << "\n";
    }
    else {
        cout << "Минимизированная форма: 0\n";
    }
}

void printSubtractorCounter() {
    cout << "Таблица истинности вычитающего счётчика (вариант 3)\n";
    vector<string> arguments = { "Q3 ", "Q2 ", "Q1 ", "Q0 ", "V  ", "Q3'", "Q2'", "Q1'", "Q0'", "h3 ", "h2 ", "h1 ", "h0 " };
    const int colWidth = 4;

    printTruthTableHeader(arguments, colWidth);

    vector<string> sdnf_h3, sdnf_h2, sdnf_h1, sdnf_h0;

    for (int state = 15; state >= 0; state--) {
        int Q3 = (state >> 3) & 1;
        int Q2 = (state >> 2) & 1;
        int Q1 = (state >> 1) & 1;
        int Q0 = state & 1;

        int nextState = (state == 0) ? 15 : state - 1;
        int Q3_next = (nextState >> 3) & 1;
        int Q2_next = (nextState >> 2) & 1;
        int Q1_next = (nextState >> 1) & 1;
        int Q0_next = nextState & 1;

        int V = 1;

        int h3 = Q3 ^ Q3_next;
        int h2 = Q2 ^ Q2_next;
        int h1 = Q1 ^ Q1_next;
        int h0 = Q0 ^ Q0_next;

        printTruthTableRow(Q3, Q2, Q1, Q0, V, Q3_next, Q2_next, Q1_next, Q0_next, h3, h2, h1, h0, colWidth);

        string term = generateSDNFterm(Q3, Q2, Q1, Q0, V);

        if (h3 == 1) sdnf_h3.push_back(term);
        if (h2 == 1) sdnf_h2.push_back(term);
        if (h1 == 1) sdnf_h1.push_back(term);
        if (h0 == 1) sdnf_h0.push_back(term);
    }

    vector<string> varNames = { "Q3", "Q2", "Q1", "Q0", "V" };
    minimizeFunction(sdnf_h3, 3, varNames);
    minimizeFunction(sdnf_h2, 2, varNames);
    minimizeFunction(sdnf_h1, 1, varNames);
    minimizeFunction(sdnf_h0, 0, varNames);
}

int main() {
    setlocale(LC_ALL, "Russian");
    printSubtractorCounter();
    return 0;
}