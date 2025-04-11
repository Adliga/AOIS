#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <algorithm>
#include <tuple>
#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\2lab.cpp" 

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
/////////////////////////////////////////////////////////////////////////////
void printInitialTerms(const vector<Term>& terms, const string& input, const vector<char>& varNames, bool isDNF) {
    cout << (isDNF ? "Минимизация СДНФ:\n" : "Минимизация СКНФ:\n");
    cout << "Исходные импликанты: " << input << "\n";
    cout << "В бинарной форме: ";
    for (const auto& term : terms) {
        cout << formatBinary(term.vars, isDNF);
    }
    cout << "\n\n";
}

vector<Term> glueTerms(vector<Term> terms, const vector<char>& varNames, bool isDNF) {
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

                    if (isDNF) {
                        cout << "(" << formatDNFterm(terms[i].vars, varNames) << ") | ("
                            << formatDNFterm(terms[j].vars, varNames) << ") => ("
                            << formatDNFterm(gluedTerm, varNames) << ")\n";
                    }
                    else {
                        cout << "(" << formatCNFterm(terms[i].vars, varNames) << ") ("
                            << formatCNFterm(terms[j].vars, varNames) << ") => ("
                            << formatCNFterm(gluedTerm, varNames) << ")\n";
                    }
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

void printResultTerms(const vector<Term>& terms, const vector<char>& varNames, bool isDNF) {
    cout << "\nИтоговый результат: ";
    for (int i = 0; i < terms.size(); i++) {
        if (isDNF) {
            cout << "(" << formatDNFterm(terms[i].vars, varNames) << ")";
            if (i < terms.size() - 1) cout << "|";
        }
        else {
            cout << "(" << formatCNFterm(terms[i].vars, varNames) << ")";
            if (i < terms.size() - 1) cout << "&";
        }
    }

    cout << "\nВ бинарной форме: ";
    for (const auto& term : terms) {
        cout << formatBinary(term.vars, isDNF);
    }
    cout << "\n\n";
}

void minimize(const string& input, bool isDNF) {
    vector<char> varNames;
    vector<Term> terms = parseFunction(input, isDNF, varNames);
    printInitialTerms(terms, input, varNames, isDNF);
    vector<Term> minimized = glueTerms(terms, varNames, isDNF);
    printResultTerms(minimized, varNames, isDNF);
}

/////////////////////////////////////////////////////////////////////////////////////
void printInitialInfo(const string& input, const vector<Term>& terms, const vector<char>& varNames, bool isDNF) {
    cout << (isDNF ? "Минимизация СДНФ:\n" : "Минимизация СКНФ:\n");
    cout << "Исходные импликанты: " << input << "\n";
    cout << "В бинарной форме: ";
    for (const auto& term : terms) {
        cout << formatBinary(term.vars, isDNF);
    }
    cout << "\n\n";
}

map<int, vector<Term>> groupTerms(const vector<Term>& terms, bool isDNF) {
    map<int, vector<Term>> groups;
    for (const auto& term : terms) {
        int count = countOnesOrZeros(term.vars, isDNF);
        groups[count].push_back(term);
    }
    return groups;
}

void printGroups(const map<int, vector<Term>>& groups, bool isDNF) {
    cout << "Группы по количеству " << (isDNF ? "единиц" : "нулей") << ":\n";
    for (const auto& group : groups) {
        cout << group.first << ": ";
        for (const auto& term : group.second) {
            cout << formatBinary(term.vars, isDNF) << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

vector<Term> performGluing(map<int, vector<Term>> groups, const vector<char>& varNames, bool isDNF, int varCount) {
    vector<Term> primeImplicants;
    bool glued;
    do {
        glued = false;
        map<int, vector<Term>> newGroups;
        set<string> usedTerms;

        for (auto it = groups.begin(); it != groups.end(); ++it) {
            auto nextIt = next(it);
            if (nextIt == groups.end()) continue;

            for (const auto& t1 : it->second) {
                for (const auto& t2 : nextIt->second) {
                    if (canGlue(t1.vars, t2.vars, varCount)) {
                        string gluedTerm = glue(t1.vars, t2.vars);
                        if (usedTerms.find(gluedTerm) == usedTerms.end()) {
                            Term newTerm(gluedTerm);
                            newTerm.coveredTerms.insert(t1.coveredTerms.begin(), t1.coveredTerms.end());
                            newTerm.coveredTerms.insert(t2.coveredTerms.begin(), t2.coveredTerms.end());
                            newGroups[countOnesOrZeros(gluedTerm, isDNF)].push_back(newTerm);
                            primeImplicants.push_back(newTerm);
                            usedTerms.insert(gluedTerm);
                            glued = true;

                            if (isDNF) {
                                cout << "(" << formatDNFterm(t1.vars, varNames) << ") | ("
                                    << formatDNFterm(t2.vars, varNames) << ") => ("
                                    << formatDNFterm(gluedTerm, varNames) << ")\n";
                            }
                            else {
                                cout << "(" << formatCNFterm(t1.vars, varNames) << ")("
                                    << formatCNFterm(t2.vars, varNames) << ") => ("
                                    << formatCNFterm(gluedTerm, varNames) << ")\n";
                            }
                        }
                    }
                }
            }
        }

        groups = newGroups;
    } while (glued);

    return primeImplicants;
}

void printCoverTable(const vector<Term>& primeImplicants, const vector<string>& originalTerms, bool isDNF, int varCount) {
    const int colWidth = 10;
    cout << "\nТаблица покрытия:\n";
    cout << setw(colWidth) << left << "Импликанты" << " |";
    for (const auto& term : originalTerms) {
        cout << setw(colWidth) << right << formatBinary(term, isDNF);
    }
    cout << "\n";

    cout << string(colWidth, '-') << "-+";
    for (size_t i = 0; i < originalTerms.size(); ++i) {
        cout << string(colWidth, '-');
    }
    cout << "\n";

    for (const auto& pi : primeImplicants) {
        cout << setw(colWidth) << left << formatBinary(pi.vars, isDNF) << " |";
        for (const auto& term : originalTerms) {
            cout << setw(colWidth) << right << (covers(pi.vars, term, varCount) ? "X" : "");
        }
        cout << "\n";
    }
}

int countLiterals(const string& vars) {
    int count = 0;
    for (char c : vars) {
        if (c != '-') count++;
    }
    return count;
}

vector<Term> selectFinalImplicants(const vector<Term>& primeImplicants, const vector<string>& originalTerms, int varCount) {
    vector<Term> finalImplicants;
    set<string> covered;

    for (const auto& term : originalTerms) {
        vector<const Term*> coveringImplicants;
        for (const auto& pi : primeImplicants) {
            if (covers(pi.vars, term, varCount)) {
                coveringImplicants.push_back(&pi);
            }
        }
        if (coveringImplicants.size() == 1) {
            const Term* essential = coveringImplicants[0];
            if (find(finalImplicants.begin(), finalImplicants.end(), *essential) == finalImplicants.end()) {
                finalImplicants.push_back(*essential);
                for (const auto& t : originalTerms) {
                    if (covers(essential->vars, t, varCount)) {
                        covered.insert(t);
                    }
                }
            }
        }
    }

    while (covered.size() < originalTerms.size()) {
        int maxNewCovered = 0;
        int minLiterals = varCount + 1;
        const Term* bestImplicant = nullptr;

        for (const auto& pi : primeImplicants) {
            if (find(finalImplicants.begin(), finalImplicants.end(), pi) != finalImplicants.end()) continue;
            int newCovered = 0;
            for (const auto& term : originalTerms) {
                if (covered.find(term) == covered.end() && covers(pi.vars, term, varCount)) {
                    newCovered++;
                }
            }
            int literals = countLiterals(pi.vars);
            if (newCovered > maxNewCovered || (newCovered == maxNewCovered && literals < minLiterals)) {
                maxNewCovered = newCovered;
                minLiterals = literals;
                bestImplicant = &pi;
            }
        }

        if (bestImplicant) {
            finalImplicants.push_back(*bestImplicant);
            for (const auto& term : originalTerms) {
                if (covers(bestImplicant->vars, term, varCount)) {
                    covered.insert(term);
                }
            }
        }
        else {
            break;
        }
    }

    return finalImplicants;
}

void printFinalResult(const vector<Term>& finalImplicants, const vector<char>& varNames, bool isDNF) {
    cout << "\nИтоговый результат: ";
    string result;
    for (size_t i = 0; i < finalImplicants.size(); ++i) {
        if (isDNF) {
            result += formatDNFterm(finalImplicants[i].vars, varNames);
            cout << "(" << formatDNFterm(finalImplicants[i].vars, varNames) << ")";
            if (i < finalImplicants.size() - 1) {
                result += "|";
                cout << "|";
            }
        }
        else {
            result += formatCNFterm(finalImplicants[i].vars, varNames);
            cout << "(" << formatCNFterm(finalImplicants[i].vars, varNames) << ")";
            if (i < finalImplicants.size() - 1) {
                result += "&";
                cout << "&";
            }
        }
    }
    cout << "\nВ бинарной форме: ";
    for (const auto& fi : finalImplicants) {
        cout << formatBinary(fi.vars, isDNF);
    }
    cout << "\n\n";
}

void minimizeQuine(const string& input, bool isDNF) {
    vector<char> varNames;
    vector<Term> terms = parseFunction(input, isDNF, varNames);
    int varCount = varNames.size();

    printInitialInfo(input, terms, varNames, isDNF);
    map<int, vector<Term>> groups = groupTerms(terms, isDNF);
    printGroups(groups, isDNF);
    vector<Term> primeImplicants = performGluing(groups, varNames, isDNF, varCount);

    vector<string> originalTerms;
    for (const auto& t : terms) originalTerms.push_back(t.vars);

    printCoverTable(primeImplicants, originalTerms, isDNF, varCount);
    vector<Term> finalImplicants = selectFinalImplicants(primeImplicants, originalTerms, varCount);
    printFinalResult(finalImplicants, varNames, isDNF);
}

///////////////////////////////////////////////////////////////////////////////////// 

vector<vector<vector<int>>> buildKarnaughMap(const map<vector<int>, int>& results, int n) {
    int rows, cols, layers;
    switch (n) {
    case 1: rows = 1; cols = 2; layers = 1; break;
    case 2: rows = 2; cols = 2; layers = 1; break;
    case 3: rows = 2; cols = 4; layers = 1; break;
    case 4: rows = 4; cols = 4; layers = 1; break;
    case 5: rows = 4; cols = 4; layers = 2; break;
    default: throw runtime_error("Поддерживается до 5 переменных");
    }

    vector<vector<vector<int>>> kmap(layers, vector<vector<int>>(rows, vector<int>(cols, 0)));
    for (const auto& entry : results) {
        const vector<int>& values = entry.first;
        int value = entry.second;
        int row, col, layer;
        if (n == 1) {
            row = 0; col = values[0]; layer = 0;
        }
        else if (n == 2) {
            row = values[0]; col = values[1]; layer = 0;
        }
        else if (n == 3) {
            row = values[0]; col = grayCode((values[1] << 1) | values[2]); layer = 0;
        }
        else if (n == 4) {
            row = grayCode((values[0] << 1) | values[1]);
            col = grayCode((values[2] << 1) | values[3]);
            layer = 0;
        }
        else if (n == 5) {
            row = grayCode((values[0] << 1) | values[1]);
            col = grayCode((values[2] << 1) | values[3]);
            layer = values[4];
        }
        kmap[layer][row][col] = value;
    }
    return kmap;
}
//
void printKarnaughMap(const vector<vector<vector<int>>>& kmap, int n, const vector<char>& varNames) {
    cout << "Карта Карно:\n";
    if (n == 1) {
        cout << varNames[0] << " | 0  1\n";
        cout << "---+------\n";
        cout << "   | " << kmap[0][0][0] << "  " << kmap[0][0][1] << "\n";
    }
    else if (n == 2) {
        cout << varNames[0] << "\\" << varNames[1] << " | 0  1\n";
        cout << "----+------\n";
        for (int i = 0; i < 2; ++i) {
            cout << i << "   | " << kmap[0][i][0] << "  " << kmap[0][i][1] << "\n";
        }
    }
    else if (n == 3) {
        cout << varNames[0] << "\\" << varNames[1] << varNames[2] << " | 00  01  11  10\n";
        cout << "-----+---------------\n";
        for (int i = 0; i < 2; ++i) {
            cout << i << "    | " << kmap[0][i][0] << "   " << kmap[0][i][1] << "   " << kmap[0][i][2] << "   " << kmap[0][i][3] << "\n";
        }
    }
    else if (n == 4 || n == 5) {
        string rowVars = (n == 4) ? string(1, varNames[0]) + varNames[1] : string(1, varNames[0]) + varNames[1];
        string colVars = (n == 4) ? string(1, varNames[2]) + varNames[3] : string(1, varNames[2]) + varNames[3];
        cout << rowVars << "\\" << colVars << " | 00  01  11  10\n";
        cout << "------+---------------\n";
        int layers = (n == 5) ? 2 : 1;
        for (int layer = 0; layer < layers; ++layer) {
            if (n == 5) {
                cout << varNames[4] << "=" << layer << "\n";
            }
            for (int i = 0; i < 4; ++i) {
                string rowLabel = (i == 0 ? "00" : i == 1 ? "01" : i == 2 ? "11" : "10");
                cout << rowLabel << "    | ";
                for (int j = 0; j < 4; ++j) {
                    cout << kmap[layer][i][j] << "   ";
                }
                cout << "\n";
            }
            if (layer == 0 && layers == 2) cout << "\n";
        }
    }
}

map<vector<int>, int> computeFunctionValues(const vector<Term>& terms, const vector<vector<int>>& cases, bool isDNF) {
    map<vector<int>, int> results;//
    for (const auto& caseValues : cases) {
        string binary;
        for (int val : caseValues) binary += to_string(val);
        bool value = isDNF ? false : true;
        for (const auto& term : terms) {
            bool match = true;
            for (int i = 0; i < term.vars.length(); ++i) {
                if (term.vars[i] != '-' && term.vars[i] != binary[i]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                value = isDNF ? true : false;
                break;
            }
        }
        results[caseValues] = value ? 1 : 0;
    }
    return results;
}

vector<vector<tuple<int, int, int>>> findKarnaughGroups(const vector<vector<vector<int>>>& kmap, int targetValue) {
    int layers = kmap.size();
    int rows = kmap[0].size();
    int cols = kmap[0][0].size();
    vector<vector<tuple<int, int, int>>> groups;

    vector<tuple<int, int, int>> sizes = {
        {2, 4, 4}, {2, 2, 4}, {2, 4, 2}, {2, 2, 2}, {2, 1, 4}, {2, 4, 1}, {2, 2, 1}, {2, 1, 2}, {2, 1, 1},
        {1, 4, 4}, {1, 2, 4}, {1, 4, 2}, {1, 2, 2}, {1, 1, 4}, {1, 4, 1}, {1, 2, 1}, {1, 1, 2}, {1, 1, 1}
    };

    set<string> uniqueGroups;

    for (const auto& size : sizes) {
        int l = get<0>(size), h = get<1>(size), w = get<2>(size);
        if (l > layers || h > rows || w > cols) continue;

        for (int layer = 0; layer <= layers - l; ++layer) {
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    bool valid = true;
                    vector<tuple<int, int, int>> cells;
                    for (int dl = 0; dl < l; ++dl) {
                        for (int i = 0; i < h; ++i) {
                            for (int j = 0; j < w; ++j) {
                                int row = (r + i) % rows;
                                int col = (c + j) % cols;
                                int currLayer = (layer + dl) % layers;
                                if (kmap[currLayer][row][col] != targetValue) {
                                    valid = false;
                                    break;
                                }
                                cells.emplace_back(currLayer, row, col);
                            }
                            if (!valid) break;
                        }
                        if (!valid) break;
                    }
                    if (valid) {
                        sort(cells.begin(), cells.end());
                        string key;
                        for (const auto& cell : cells)
                            key += to_string(get<0>(cell)) + "," + to_string(get<1>(cell)) + "," + to_string(get<2>(cell)) + ";";
                        if (uniqueGroups.insert(key).second) {
                            groups.push_back(cells);
                        }
                    }
                }
            }
        }
    }

    vector<vector<tuple<int, int, int>>> filteredGroups;
    set<tuple<int, int, int>> coveredCells;
    sort(groups.begin(), groups.end(), [](const auto& a, const auto& b) {
        return a.size() > b.size();
        });
    for (const auto& group : groups) {
        bool addsNew = false;
        for (const auto& cell : group) {
            if (!coveredCells.count(cell)) {
                addsNew = true;
                break;
            }
        }
        if (addsNew) {
            filteredGroups.push_back(group);
            coveredCells.insert(group.begin(), group.end());
        }
    }

    return filteredGroups;
}

map<vector<int>, int> computeKarnaughValues(const string& input, bool isDNF, const vector<char>& varNames) {
    int n = varNames.size();
    vector<vector<int>> cases = generateCases(n);
    map<vector<int>, int> results;

    if (isDNF) {
        vector<Term> terms = parseFunction(input, true, const_cast<vector<char>&>(varNames));
        for (const auto& caseValues : cases) {
            string binary;
            for (int val : caseValues) binary += to_string(val);
            bool value = false;
            for (const auto& term : terms) {
                bool match = true;
                for (int i = 0; i < term.vars.length(); ++i) {
                    if (term.vars[i] != '-' && term.vars[i] != binary[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    value = true;
                    break;
                }
            }
            results[caseValues] = value ? 1 : 0;
        }
    }
    else {
        vector<Term> terms = parseFunction(input, false, const_cast<vector<char>&>(varNames));
        for (const auto& caseValues : cases) {
            string binary;
            for (int val : caseValues) binary += to_string(val);
            bool value = true;
            for (const auto& term : terms) {
                bool match = true;
                for (int i = 0; i < term.vars.length(); ++i) {
                    if (term.vars[i] != '-' && term.vars[i] != binary[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    value = false;
                    break;
                }
            }
            results[caseValues] = value ? 1 : 0;
        }
    }
    return results;
}

vector<int> coordsToValues(int layer, int row, int col, int n) {
    vector<int> values(n);
    if (n == 1) {
        values[0] = col;
    }
    else if (n == 2) {
        values[0] = row;
        values[1] = col;
    }
    else if (n == 3) {
        values[0] = row;
        int bc = grayCode(col);
        values[1] = (bc >> 1) & 1;
        values[2] = bc & 1;
    }
    else if (n == 4) {
        int ab = grayCode(row), cd = grayCode(col);
        values[0] = (ab >> 1) & 1;
        values[1] = ab & 1;
        values[2] = (cd >> 1) & 1;
        values[3] = cd & 1;
    }
    else if (n == 5) {
        int ab = grayCode(row), cd = grayCode(col);
        values[0] = (ab >> 1) & 1;
        values[1] = ab & 1;
        values[2] = (cd >> 1) & 1;
        values[3] = cd & 1;
        values[4] = layer;
    }
    return values;
}

string karnaughGroupToTerm(const vector<tuple<int, int, int>>& group, int n, const vector<char>& varNames, bool isDNF) {
    vector<set<int>> varValues(n);
    for (const auto& cell : group) {
        vector<int> values = coordsToValues(get<0>(cell), get<1>(cell), get<2>(cell), n);
        for (int i = 0; i < n; ++i) {
            varValues[i].insert(values[i]);
        }
    }

    string term;
    bool first = true;
    if (isDNF) {
        for (int i = 0; i < n; ++i) {
            if (varValues[i].size() == 1) {
                int val = *varValues[i].begin();
                if (!first) term += "&";
                if (val == 0) term += "!" + string(1, varNames[i]);
                else term += string(1, varNames[i]);
                first = false;
            }
        }
    }
    else {
        for (int i = 0; i < n; ++i) {
            if (varValues[i].size() == 1) {
                int val = *varValues[i].begin();
                if (!first) term += "|";
                if (val == 1) term += "!" + string(1, varNames[i]);
                else term += string(1, varNames[i]);
                first = false;
            }
        }
    }
    return term.empty() ? (isDNF ? "1" : "0") : term;
}

void minimizeKarnaugh(const string& input, bool isDNF) {
    vector<char> varNames;
    vector<Term> terms = parseFunction(input, isDNF, varNames);
    int n = varNames.size();
    if (n > 5) {
        cout << "Ошибка: Поддерживается до 5 переменных\n";
        return;
    }

    cout << (isDNF ? "Минимизация СДНФ методом карты Карно:\n" : "Минимизация СКНФ методом карты Карно:\n");
    cout << "Исходные импликанты: " << input << "\n";

    map<vector<int>, int> values = computeKarnaughValues(input, isDNF, varNames);
    vector<vector<vector<int>>> kmap = buildKarnaughMap(values, n);
    printKarnaughMap(kmap, n, varNames);

    vector<vector<tuple<int, int, int>>> groups = findKarnaughGroups(kmap, isDNF ? 1 : 0);
    cout << "\nНайденные группы:\n";
    vector<string> finalTerms;
    for (const auto& group : groups) {
        string term = karnaughGroupToTerm(group, n, varNames, isDNF);
        cout << (isDNF ? term : "(" + term + ")") << "\n";
        finalTerms.push_back(term);
    }

    cout << "\nИтоговый результат: ";
    if (finalTerms.empty()) {
        cout << (isDNF ? "0" : "1") << "\n\n";
    }
    else {
        for (int i = 0; i < finalTerms.size(); ++i) {
            if (isDNF) {
                cout << "(" << finalTerms[i] << ")";
                if (i < finalTerms.size() - 1) cout << "|";
            }
            else {
                cout << "(" << finalTerms[i] << ")";
                if (i < finalTerms.size() - 1) cout << "&";
            }
        }
        cout << "\n\n";
    }
}

//////////////////////////////////////////////////////////////////////////////
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

//int main() {
//    setlocale(LC_ALL, "Russian");
//    string input;
//    int choice;
//
//    while (true) {
//        cout << "|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|/|\n";
//        cout << "Выберите тип минимизации:\n";
//        cout << "1. СДНФ\n2. СКНФ\n0. Выход\nВаш выбор: ";
//        cin >> choice;
//        cin.ignore();
//
//        if (choice == 0) break;
//
//        cout << "Введите логическую функцию: ";
//        getline(cin, input);
//
//        try {
//            cout << "\nТаблица истинности и канонические формы:\n";
//            string canonicalForm;
//            bool isDNF = (choice == 1);
//
//            vector<char> variables = extractVariables(input);
//            vector<vector<bool>> allValues = generateVariableCombinations(variables);
//            vector<string> rpn = toRPN(input);
//            vector<pair<string, vector<bool>>> results = evaluateExpressions(rpn, variables, allValues);
//            pair<string, vector<bool>> finalExpr = selectFinalExpression(results);
//
//            vector<int> minterms, maxterms;
//            vector<bool> binaryForm;
//            for (size_t i = 0; i < allValues.size(); ++i) {
//                if (finalExpr.second[i]) {
//                    minterms.push_back(i);
//                }
//                else {
//                    maxterms.push_back(i);
//                }
//                binaryForm.push_back(finalExpr.second[i]);
//            }
//
//            if (isDNF) {
//                canonicalForm = constructSDNF(variables, minterms);
//            }
//            else {
//                canonicalForm = constructSKNF(variables, maxterms);
//            }
//
//            printTruthTable(input);
//
//            if ((isDNF && !isValidDNF(canonicalForm)) || (!isDNF && !isValidCNF(canonicalForm))) {
//                cout << "Ошибка: сгенерировано выражение некорректного формата.\n";
//                continue;
//            }
//
//            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
//            cout << "\nКаноническая форма для минимизации (" << (isDNF ? "СДНФ" : "СКНФ") << "): " << canonicalForm << "\n";
//
//            cout << "\nМинимизация расчётным методом (методом простых преобразований):\n";
//            minimize(canonicalForm, isDNF);
//            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
//
//            cout << "\nМинимизация расчётно-табличным методом (методом Квайна-МакКласки):\n";
//            minimizeQuine(canonicalForm, isDNF);
//            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
//
//            cout << "\nМинимизация табличным методом (по карте Карно):\n";
//            minimizeKarnaugh(canonicalForm, isDNF);
//            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
//        }
//        catch (const exception& e) {
//            cout << "Ошибка: " << e.what() << "\n";
//        }
//    }
//
//    return 0;
//}