#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\functions.cpp" 

vector<vector<int>> buildKarnaughMap(const map<vector<int>, int>& results, int n) {
    int rows, cols;
    switch (n) {
    case 1: rows = 1; cols = 2; break;
    case 2: rows = 2; cols = 2; break;
    case 3: rows = 2; cols = 4; break;
    case 4: rows = 4; cols = 4; break;
    default: throw runtime_error("Поддерживается до 4 переменных");
    }

    vector<vector<int>> kmap(rows, vector<int>(cols, 0));
    for (const auto& entry : results) {
        const vector<int>& values = entry.first;
        int value = entry.second;
        int row, col;
        if (n == 1) { row = 0; col = values[0]; }
        else if (n == 2) { row = values[0]; col = values[1]; }
        else if (n == 3) { row = values[0]; col = grayCode((values[1] << 1) | values[2]); }
        else { row = grayCode((values[0] << 1) | values[1]); col = grayCode((values[2] << 1) | values[3]); }
        kmap[row][col] = value;
    }
    return kmap;
}

void printKarnaughMap(const vector<vector<int>>& kmap, int n, const vector<char>& varNames) {
    cout << "Карта Карно:\n";
    if (n == 1) {
        cout << varNames[0] << " | 0  1\n";
        cout << "---+------\n";
        cout << "   | " << kmap[0][0] << "  " << kmap[0][1] << "\n";
    }
    else if (n == 2) {
        cout << varNames[0] << "\\" << varNames[1] << " | 0  1\n";
        cout << "----+------\n";
        for (int i = 0; i < 2; ++i) {
            cout << i << "   | " << kmap[i][0] << "  " << kmap[i][1] << "\n";
        }
    }
    else if (n == 3) {
        cout << varNames[0] << "\\" << varNames[1] << varNames[2] << " | 00  01  11  10\n";
        cout << "-----+---------------\n";
        for (int i = 0; i < 2; ++i) {
            cout << i << "    | " << kmap[i][0] << "   " << kmap[i][1] << "   " << kmap[i][2] << "   " << kmap[i][3] << "\n";
        }
    }
    else if (n == 4) {
        cout << varNames[0] << varNames[1] << "\\" << varNames[2] << varNames[3] << " | 00  01  11  10\n";
        cout << "------+---------------\n";
        for (int i = 0; i < 4; ++i) {
            cout << (i == 0 ? "00" : i == 1 ? "01" : i == 2 ? "11" : "10") << "    | ";
            for (int j = 0; j < 4; ++j) {
                cout << kmap[i][j] << "   ";
            }
            cout << "\n";
        }
    }
}

map<vector<int>, int> computeFunctionValues(const vector<Term>& terms, const vector<vector<int>>& cases, bool isDNF) {
    map<vector<int>, int> results;
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

void findGroupsForSize(const vector<vector<int>>& kmap, int rows, int cols, int h, int w, int targetValue,
    vector<vector<pair<int, int>>>& groups, set<string>& uniqueGroups) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            bool valid = true;
            vector<pair<int, int>> cells;
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    int row = (r + i) % rows;
                    int col = (c + j) % cols;
                    if (kmap[row][col] != targetValue) {
                        valid = false;
                        break;
                    }
                    cells.emplace_back(row, col);
                }
                if (!valid) break;
            }
            if (valid) {
                sort(cells.begin(), cells.end());
                string key;
                for (const auto& cell : cells)
                    key += to_string(cell.first) + "," + to_string(cell.second) + ";";
                if (uniqueGroups.insert(key).second) {
                    groups.push_back(cells);
                }
            }
        }
    }
}

vector<vector<pair<int, int>>> findKarnaughGroups(const vector<vector<int>>& kmap, int targetValue) {
    int rows = kmap.size(), cols = kmap[0].size();
    vector<vector<pair<int, int>>> groups;

    vector<pair<int, int>> sizes = {
        {4, 4}, {2, 4}, {4, 2}, {2, 2},
        {1, 4}, {4, 1}, {2, 1}, {1, 2},
        {1, 1}
    };

    set<string> uniqueGroups;

    for (const auto& size : sizes) {
        int h = size.first, w = size.second;
        findGroupsForSize(kmap, rows, cols, h, w, targetValue, groups, uniqueGroups);
    }

    vector<vector<pair<int, int>>> filteredGroups;
    set<pair<int, int>> coveredCells;
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

vector<int> coordsToValues(int row, int col, int n) {
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
    return values;
}

string karnaughGroupToTerm(const vector<pair<int, int>>& group, int n, const vector<char>& varNames, bool isDNF) {
    vector<set<int>> varValues(n);
    for (const auto& cell : group) {
        vector<int> values = coordsToValues(cell.first, cell.second, n);
        for (int i = 0; i < n; ++i) {
            varValues[i].insert(values[i]);
        }
    }

    string term;
    if (isDNF) {
        for (int i = 0; i < n; ++i) {
            if (varValues[i].size() == 1) {
                int val = *varValues[i].begin();
                if (val == 0) term += "!" + string(1, varNames[i]);
                else term += string(1, varNames[i]);
            }
        }
    }
    else {
        bool first = true;
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
    if (n > 4) {
        cout << "Ошибка: Поддерживается до 4 переменных\n";
        return;
    }

    cout << (isDNF ? "Минимизация СДНФ методом карты Карно:\n" : "Минимизация СКНФ методом карты Карно:\n");
    cout << "Исходные импликанты: " << input << "\n";

    map<vector<int>, int> values = computeKarnaughValues(input, isDNF, varNames);
    vector<vector<int>> kmap = buildKarnaughMap(values, n);
    printKarnaughMap(kmap, n, varNames);

    vector<vector<pair<int, int>>> groups = findKarnaughGroups(kmap, isDNF ? 1 : 0);
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
            }
        }
        cout << "\n\n";
    }
}