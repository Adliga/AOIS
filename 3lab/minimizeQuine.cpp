#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\functions.cpp" 

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