#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <algorithm>
#include <tuple>

using namespace std;

struct Term {
    string vars;
    bool used;
    set<string> coveredTerms;

    Term(string v) : vars(v), used(false) {}
};

bool operator==(const Term& lhs, const Term& rhs) {
    return lhs.vars == rhs.vars;
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


string parseCNFterm(const string& term, vector<char>& varNames) {
    string result;
    set<char> foundVars;
    for (size_t i = 0; i < term.length(); i++) {
        if (term[i] == '!') {
            char var = term[i + 1];
            foundVars.insert(var);
            result += '1';
            if (find(varNames.begin(), varNames.end(), var) == varNames.end())
                varNames.push_back(var);
            i++;
        }
        else if (isalpha(term[i])) {
            char var = term[i];
            foundVars.insert(var);
            result += '0';
            if (find(varNames.begin(), varNames.end(), var) == varNames.end())
                varNames.push_back(var);
        }
        else if (term[i] == '|') {
            continue;
        }
    }
    return result;
}

vector<Term> parseFunction(const string& input, bool isDNF, vector<char>& varNames) {
    vector<Term> terms;
    stringstream ss(input);
    string term;

    if (isDNF) {
        while (getline(ss, term, '|')) {
            term.erase(remove_if(term.begin(), term.end(), isspace), term.end());
            if (term.empty()) continue;
            Term t(parseDNFterm(term, varNames));
            t.coveredTerms.insert(t.vars);
            terms.push_back(t);
        }
    }
    else {
        while (getline(ss, term, '&')) {
            term.erase(remove_if(term.begin(), term.end(), isspace), term.end());
            if (term.empty() || term == "(") continue;
            if (term[0] == '(') term = term.substr(1);
            if (term.back() == ')') term.pop_back();
            Term t(parseCNFterm(term, varNames));
            t.coveredTerms.insert(t.vars);
            terms.push_back(t);
        }
    }
    return terms;
}

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

string formatCNFterm(const string& term, const vector<char>& varNames) {
    string result;
    bool first = true;
    for (int i = 0; i < term.length(); i++) {
        if (term[i] == '-') continue;
        if (!first) result += "|";
        if (term[i] == '1') result += "!" + string(1, varNames[i]);
        else if (term[i] == '0') result += string(1, varNames[i]);
        first = false;
    }
    return result.empty() ? "0" : result;
}

string formatBinary(const string& term, bool isDNF) {
    string result = "(";
    for (int i = 0; i < term.length(); i++) {
        if (i > 0) result += ",";
        if (term[i] == '-') result += "X";
        else if (isDNF) result += term[i];
        else result += (term[i] == '0' ? "0" : "1");
    }
    result += ")";
    return result;
}

int countOnesOrZeros(const string& term, bool isDNF) {
    int count = 0;
    for (char c : term) {
        if (isDNF && c == '1') count++;
        else if (!isDNF && c == '0') count++;
    }
    return count;
}

bool covers(const string& implicant, const string& term, int varCount) {
    for (int i = 0; i < varCount; i++) {
        if (implicant[i] != '-' && implicant[i] != term[i]) return false;
    }
    return true;
}

vector<vector<int>> generateCases(int n) {
    vector<vector<int>> cases;
    int total = 1 << n;
    for (int i = 0; i < total; ++i) {
        vector<int> values(n);
        for (int j = 0; j < n; ++j) {
            values[j] = (i >> (n - 1 - j)) & 1;
        }
        cases.push_back(values);
    }
    return cases;
}

int grayCode(int n) {
    return n ^ (n >> 1);
}