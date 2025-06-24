#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int MAX_TOKENS = 100;
string tokens[MAX_TOKENS];
int current = 0, totalTokens = 0;
int counti = 0;

// Load tokens from tokens.txt
void loadTokens(const string& filename) {
    ifstream file(filename);
    string token;
    while (file>> token && totalTokens < MAX_TOKENS) {
        tokens[totalTokens] = token;
        totalTokens++;
    }
    file.close();
}
void printtabs()
{
    if (counti > 0) 
    {
        for (int i = 0; i < counti; i++)
        {
            cout << "\t";
        }
    }
}
bool match(const string& expected) { //expected me <id> hoga aur tokens[current] mein <id, 3> inko match hona chaiye ,aur <num> aur <10>
    //cout <<"\n???"<< expected << "\t" << tokens[current] << "???\n ";          /////////////////////////////////////////////////////////////////////////////ye line jo dummy print kar rhi hai
    if (tokens[current] == expected || expected == "<num>" && isdigit(tokens[current][1]) || expected == tokens[current].substr(0, 3)) {
        counti++;
        printtabs();
        cout << expected<<"\n";
        current++;
        counti--;
        return true;
    }
    else
    {
        return false;
    }
    
}
// Grammar parsing functions start here

bool Type() {
    counti++;
    printtabs();
    cout << "Type \n";
    if (match("<Adadi>") || match("<Ashriya>") || match("<Harf>") || match("<Matn>") || match("<Mantiqi>"))
    {
        counti--;
        return true;
    }
    else
    {
        counti--;
        return false;
    }
}

bool Factor();
bool H_();
bool I_();
bool Term();
bool G_();
bool J_();
bool Mag();
bool Compare();
bool D_();
bool E_();
bool F_();
bool Expr();
bool OptExpr();
bool ForStmt();
bool WhileStmt();
bool IfStmt();
bool ElsePart();
bool CompStmt();
bool Stmt();
bool StmtList();
bool C_();
bool Declaration();
bool idenList();
bool B_();
bool Arg();
bool A_();
bool ArgList();
bool Function();
bool K_();


 //Function is perfect now error check is happening and tree insertion is also happening
//we need to see the number of tabs as well
//so ja jahil admi
bool Function() {
    counti++;
    printtabs();
    cout << "Function\n";

    if (!Type()) {
        counti--;
        return false;
    }
    if (!match("<id"))
    {
        cout << "Error in Function ! identifier expected after Type \n";
        counti--;
        return false;
    }
    if (!match("<(>"))
    {
        cout << "Error in Function ! ( expected \n";
        counti--;
        return false;
    }
    if (!ArgList()) {
        counti--;
        return false;
    }
    if (!match("<)>"))
    {
        cout << "Error in Function ! ) expected \n";
        counti--;
        return false;
    }
    if (!CompStmt())
    {
        counti--;
        return false;
    }

    counti--;
    return true;
}

bool ArgList() {
    counti++;
    printtabs();
    cout << "ArgList \n";
    if (!Arg()) {
        counti--;
        return false;
    }
    if (!A_()) {
        counti--;
        return false;
    }
    counti--;
    return true;
}
bool A_() {
    counti++;
    printtabs();
    cout << "A_ \n";
    if (match("<,>")) {
        if (!Arg()) {
            counti--;
            return false;
        }
        if (!A_()) {
            counti--;
            return false;
        }
    }
    counti--;
    return true;
}

bool Arg() {
    counti++;
    printtabs();
    cout << "Arg \n";
    if (!Type()) {
        counti--;
        return false;
    }
    if (!match("<id"))
    {
        cout << "Error in Arg! Expected identifier after Type\n";
        counti--;
        return false;
    }
    counti--;
    return true;
}

bool Declaration() {
    counti++;
    printtabs();
    cout << "Declaration \n";
    if (!Type()) {
        counti--;
        return false;
    }
    if (!idenList()) {
        counti--;
        return false;
    }
    if (!match("<::>"))
    {
        cout << " Error in Declaration! Expected :: \n";
        counti--;
        return false;
    }
    counti--;
    return true;
}

bool idenList() {
    counti++;
    printtabs();
    cout << "idenList \n";
    if (!match("<id"))
    {
        counti--;
        cout << " Error in idenList! Expected an identifier\n ";
        return false;
    }
    if (!B_()) {
        counti--;
        return false;
    }
    counti--;
    return true;
}

bool B_() {
    counti++;
    printtabs();
    cout << "B_ \n";
    if (match("<,>")) {
        if (!idenList()) {
            counti--;
            return false;
        }
    }
    counti--;
    return true;
}
//fine
bool Stmt() {
    counti++;
    printtabs();
    cout << "Stmt\n";
    if (ForStmt() || WhileStmt() || (Expr() && match("<::>")) || IfStmt() || CompStmt() || Declaration() || match("<::>"))
    {
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool ForStmt() {
    counti++;
    printtabs();
    cout << "ForStmt\n";
    if (!match("<for>"))
    {
        counti--;
        return false;
    }
    if (!match("<(>"))
    {
        cout << "Error in ForStmt \n";
        counti--;
        return false;
    }
    if (!Expr()) {
        counti--;
        return false;
    }
    if (!match("<::>"))
    {
        cout << "Error in ForStmt \n";
        counti--;
        return false;
    }
    if (!OptExpr())
    {
        counti--;
        return false;
    }
    if (!match("<::>"))
    {
        cout << "Error in ForStmt \n";
        counti--;
        return false;
    }
    if (!OptExpr()) {
        counti--;
        return false;
    }
    if (!match("<)>"))
    {
        cout << "Error in ForStmt \n";
        counti--;
        return false;
    }
    if (!Stmt())
    {
        counti--;
        return false;
    }
    counti--;
    return true;
}

bool OptExpr() {
    counti++;
    printtabs();
    cout << "Opt Expr\n";
    if (Expr() || true)
    {
        counti--;
        return true;
    }
}

bool WhileStmt() {
    counti++;
    printtabs();
    cout << "WhileStmt\n";
    if (!match("<while>")) {
        counti--;
        return false;
    }
    if (!match("<(>"))
    {
        cout << "Error in WhileStmt \n";
        counti--;
        return false;
    }
    if (!Expr()) {
        counti--;
        return false;
    }
    if (!match("<)>"))
    {
        cout << "Error in WhileStmt \n";
        counti--;
        return false;
    }
if (!Stmt()) {
    counti--;
    return false;
}
counti--;
return true;
}

bool IfStmt() {
    counti++;
    printtabs();
    cout << "IfStmt\n";
    if (!match("<Agar>")) {
        counti--;
        return false;
    }
    if (!match("<(>"))
    {
        cout << "Error in IfStmt \n";
        counti--;
        return false;
    }
    if (!Expr()) {
        counti--;
        return false;
    }
    if (!match("<)>"))
    {
        cout << "Error in IfStmt \n";
        counti--;
        return false;
    }
    if (!Stmt()) {
        counti--;
        return false;
    }
    if (!ElsePart()) {
        counti--;
        return false;
    }
    counti--;
    return true;
}

bool ElsePart() {
    counti++;
    printtabs();
    cout << "ElsePart \n";
    if (match("<Wagarna>"))
    {
        if (!Stmt()) {
            counti--;
            return false;
        }
    }
    counti--;
    return true;
}

bool CompStmt() {
    counti++;
    printtabs();
    cout << "CompStmt \n";
    if (!match("<{>"))
    {
        counti--;
        return false;
    }
    if (!StmtList()) {
        counti--;
        return false;
    }
    if (!match("<}>"))
    {
        cout << "Error in CompStmt \n";
        counti--;
        return false;
    }
    counti--;
    return true;
}

bool StmtList() {
    counti++;
    printtabs();
    cout << "StmtList \n";
    if (C_())
    {
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool C_() {
    counti++;
    printtabs();
    cout << "C_ \n";
    if ((Stmt() && C_()) || true)
    {
        counti--;
        return true;
    }
}
bool Expr() {
    counti++;
    printtabs();
    cout << "Expr \n";
    if (match("<id"))
    {
        if (!K_()) {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    else if (match("<(>")) { 
        if (!Expr()) {
            counti--;
            return false;
        }
        if (!match("<)>"))
        {
            cout << "Error in Expr! Expected )";
            counti--;
            return false;
        }
        if (!I_()) {
            counti--;
            return false;
        }
        if (!J_()) {
            counti--;
            return false;
        }
        if (!D_()) {
            counti--;
            return false;
        }
        counti--;
        return true;

    }
    else if (match("<num>"))
    {
        if (!I_()) {
            counti--;
            return false;
        }
        if (!J_()) {
            counti--;
            return false;
        }
        if (!D_()) {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool K_() {
    counti++;
    printtabs();
    cout << "K_\n";
    if (match("<:=>")) {
        if (Expr())
        {
            counti--;
            return true;
        }
        counti--;
        return false;
    }
    if (I_() && J_() && D_())
    {
        counti--;
        return true;
    }
    counti--;
    return false;
}

//fine
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool D_() {
    counti++;
    printtabs();
    cout << "D_\n";
    if (Compare())
    {
        if (!Mag()) {
            counti--;
            return false;
        }
        if (!D_())
        {
            counti--;
            return false;
        }
    }
    counti--;
    return true;
}

bool Compare() {
    counti++;
    printtabs();
    cout << "Compare\n";
    if (match("<==>") || match("<!=>"))
    {
        counti--;
        return true;
    }
    if (match("<<>"))  /////////jab lexer se output aye ga tou wo <<=> aye ga, lekin hum <<> aur <=> use kar rhe hein!!!
    {
        if (!E_())
        {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    if (match("<>>")) //sem
    {
        if (!F_())
        {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool E_() {
    counti++;
    printtabs();
    cout << "E_\n";
    if (match("<>>") || match("<=>") || true)
    {
        counti--;
        return true;
    }
}

bool F_() {
    counti++;
    printtabs();
    cout << "F_\n";
    if (match("<=>") || true)
    {
        counti--;
        return true;
    }
}
/////////////////////////////////////////////////////////////////////////////
bool Mag() {
    counti++;
    printtabs();
    cout << "Mag\n";
    if (match("<(>")) {
        if (!Expr())
        {
            counti--;
            return false;
        }
        if (!match("<)>"))
        {
            cout << "Error in Mag! Expected a )\n";
            counti--;
            return false;
        }
        if (!I_()) {
            counti--;
            return false;
        }
        if (!J_()) {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    else if (match("<id")) {
        if (!I_()) {
            counti--;
            return false;
        }
        if (!J_()) {
            return false;
            counti--;
        }
        counti--;
        return true;
    }
    else if (match("<num>")) {
        if (!I_()) {
            counti--;
            return false;
        }
        if (!J_()) {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool J_() {
    counti++;
    printtabs();
    cout << "J_\n";
    if (G_()) {
        if (!J_())
        {
            counti--;
            return false;
        }
    }
    counti--;
    return true;
}

bool G_() {
    counti++;
    printtabs();
    cout << "G_\n";
    if (match("<+>") || match("<->")) {
        if (!Term()) {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool Term() {
    counti++;
    printtabs();
    cout << "Term \n";
    if (match("<(>")) {
        if (!Expr()) {
            counti--;
            return false;
        }
        if (!match("<)>"))
        {
            cout << "Error in Term! Expected a )\n";
            counti--;
            return false;
        }
        if (!I_()) {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    else if (match("<id") || match("<num>")) {
        if (!I_()) {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool I_() {
    counti++;
    printtabs();
    cout << "I_\n";
    if (H_())
    {
        if (!J_())
        {
            counti--;
            return false;
        }
    }
    counti--;
    return true;
}

bool H_() {
    counti++;
    printtabs();
    cout << "H_\n";
    if (match("<*>") || match("</>"))
    {
        if (!Factor())
        {
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    counti--;
    return false;
}

bool Factor() {
    counti++;
    printtabs();
    cout << "Factor\n";
    if (match("<(>"))
    {
        if (!Expr()) {
            counti--;
            return false;
        }
        if (!match("<)>"))
        {
            cout << "Error in Factor! expected a )\n";
            counti--;
            return false;
        }
        counti--;
        return true;
    }
    if (match("<id") || match("<num>"))
    {
        counti--;
        return true;
    }
    counti--;
    return false;
}

// -----------------------

int main() {
    loadTokens("tokens.txt");
    
    if (Function() && current == totalTokens) {
        cout << "Parsing successful.\n";
    }
    else {
        cout << "Syntax error at token: " << current << tokens[current] << "\n";
    }

    return 0;
}
