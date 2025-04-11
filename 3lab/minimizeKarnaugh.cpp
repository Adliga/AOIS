#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\functions.cpp" 

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