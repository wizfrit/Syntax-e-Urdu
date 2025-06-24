#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int MAX_TOKENS = 100;
string tokens[MAX_TOKENS];
int current = 0, totalTokens = 0;

// ID-to-variable name mapping: id 1 ? x, 2 ? y, 3 ? z, etc.
char idNames[] = { 'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g' };

ofstream tacFile("tac_output.txt");

struct CodeGen {
    int tempCount = 0;
    int labelCount = 0;

    string newTemp() {
        return "t" + to_string(tempCount++);
    }

    string newLabel() {
        return "L" + to_string(labelCount++);
    }

    void gen(string op, string arg1, string arg2 = "", string result = "") {
        if (result == "") {
            tacFile << op << " " << arg1 << ", " << arg2 << endl;
        }
        else {
            tacFile << result << " = " << arg1 << " " << op << " " << arg2 << endl;
        }
    }

    void label(string l) {
        tacFile << "label " << l << endl;
    }

    void assign(string lhs, string rhs) {
        tacFile << lhs << " := " << rhs << endl;
    }

    void ifgoto(string cond, string label) {
        tacFile << "if " << cond << " goto " << label << endl;
    }

    void gotoLabel(string label) {
        tacFile << "goto " << label << endl;
    }
};

CodeGen codeGen;

// Forward declarations
string Factor();
string Term();
string Mag();
string Rvalue();
bool Stmt();
bool WhileStmt();
bool IfStmt();
bool CompStmt();
bool Expr();

void loadTokens(const string& filename) {
    ifstream file(filename);
    string token;
    while (file >> token && totalTokens < MAX_TOKENS) {
        tokens[totalTokens++] = token;
    }
    file.close();
}

string extractLexeme(const string& token) {
    // Handle format like <id,1>
    if (token.find("<id,") == 0) {
        size_t start = token.find(',') + 1;
        size_t end = token.find('>');
        string idNum = token.substr(start, end - start);
        int idIndex = stoi(idNum) - 1;
        if (idIndex >= 0 && idIndex < sizeof(idNames)) {
            return string(1, idNames[idIndex]);
        }
        else {
            return "id" + idNum;
        }
    }

    // Handle format like <5> or <num>
    if (token[0] == '<' && token[token.length() - 1] == '>') {
        string inner = token.substr(1, token.length() - 2);
        if (isdigit(inner[0])) {
            return inner;
        }
    }

    return token;
}



bool match(const string& expected) {
    if (tokens[current] == expected || expected == "<num>" && isdigit(tokens[current][1]) || expected == tokens[current].substr(0, 3)) {
        current++;
        return true;
    }
    return false;
}

string Factor() {
    if (match("<(>")) {
        string val = Mag();
        match("<)>");
        return val;
    }
    else if (match("<id") || match("<num>")) {
        return extractLexeme(tokens[current - 1]);
    }
    return "";
}

string Term() {
    string left = Factor();
    while (match("<*>") || match("</>")) {
        string op = tokens[current - 1] == "<*>" ? "*" : "/";
        string right = Factor();
        string temp = codeGen.newTemp();
        codeGen.gen(op, left, right, temp);
        left = temp;
    }
    return left;
}

string Mag() {
    string left = Term();
    while (match("<+>") || match("<->")) {
        string op = tokens[current - 1] == "<+>" ? "+" : "-";
        string right = Term();
        string temp = codeGen.newTemp();
        codeGen.gen(op, left, right, temp);
        left = temp;
    }
    return left;
}

string Rvalue() {
    string left = Mag();
    string result = left;

    string op = "";
    if (match("<==>")) op = "==";
    else if (match("<<=>")) op = "<=";
    else if (match("<<>")) op = "<";
    else if (match("<<>>")) op = "!=";
    else if (match("<>>")) op = ">";
    else if (match("<>=>")) op = ">=";

    if (op != "") {
        string right = Mag();
        string temp = codeGen.newTemp();
        codeGen.gen(op, left, right, temp);
        result = temp;
    }

    return result;
}

bool Expr() {
    if (match("<id")) {
        string id = extractLexeme(tokens[current - 1]);
        if (!match("<:=>")) return false;
        string rhs = Mag();
        codeGen.assign(id, rhs);
        if (!match("<::>")) return false;
        return true;
    }
    return false;
}

bool Stmt() {
    return WhileStmt() || IfStmt() || CompStmt() || Expr();
}

bool WhileStmt() {
    if (!match("<while>")) return false;
    if (!match("<(>")) return false;

    string Lbegin = codeGen.newLabel();
    string Ltrue = codeGen.newLabel();
    string Lfalse = codeGen.newLabel();

    codeGen.label(Lbegin);
    string cond = Rvalue();
    if (!match("<)>")) return false;

    codeGen.ifgoto(cond, Ltrue);
    codeGen.gotoLabel(Lfalse);
    codeGen.label(Ltrue);

    if (!Stmt()) return false;

    codeGen.gotoLabel(Lbegin);
    codeGen.label(Lfalse);
    return true;
}

bool IfStmt() {
    if (!match("<Agar>")) return false;
    if (!match("<(>")) return false;

    string Ltrue = codeGen.newLabel();
    string Lfalse = codeGen.newLabel();
    string Lend = codeGen.newLabel();

    string cond = Rvalue();
    if (!match("<)>")) return false;

    codeGen.ifgoto(cond, Ltrue);
    codeGen.gotoLabel(Lfalse);
    codeGen.label(Ltrue);

    if (!Stmt()) return false;

    codeGen.gotoLabel(Lend);
    codeGen.label(Lfalse);

    if (match("<Wagarna>")) {
        if (!Stmt()) return false;
    }

    codeGen.label(Lend);
    return true;
}

bool CompStmt() {
    if (!match("<{>")) return false;
    while (!match("<}>")) {
        if (!Stmt()) return false;
    }
    return true;
}

int main() {
    loadTokens("tokens1.txt");
    if (Stmt() && current == totalTokens) {
        cout << "Parsing successful. TAC written to tac_output.txt\n";
    }
    else {
        cout << "Syntax error at token: " << tokens[current] << "\n";
    }
    return 0;
}
