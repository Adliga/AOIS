#include "D:\Study\VisualStudio\AOIS_3\AOIS_3\functions.cpp" 

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