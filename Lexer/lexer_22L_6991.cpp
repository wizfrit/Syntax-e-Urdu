#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <cstring>
using namespace std;

//this is just to populate tables
int transitionTable[50][50] ;
int advanceTable[50][50]; 
string inputs[50];
int countacc;
int asterickstates[10];
int* acceptance = new int[50];
int numStates = 0, numInputs = 0;
////////////////////////
//for meow category

char buffer1[25], buffer2[25]; 
char* bufferPtr = buffer1;
bool bufferToggle = false;
unordered_map<string, bool> keywordMap = {
    {"asm", true}, {"Wagarna", true}, {"new", true}, {"this", true}, {"auto", true},
    {"enum", true}, {"operator", true}, {"throw", true}, {"Mantiqi", true}, {"explicit", true},
    {"private", true}, {"True", true}, {"break", true}, {"export", true}, {"protected", true},
    {"try", true}, {"case", true}, {"extern", true}, {"public", true}, {"typedef", true},
    {"catch", true}, {"False", true}, {"register", true}, {"typeid", true}, {"Harf", true},
    {"Ashriya", true}, {"typename", true}, {"Adadi", true}, {"class", true}, {"for", true},
    {"Wapas", true}, {"union", true}, {"const", true}, {"dost", true}, {"short", true},
    {"unsigned", true}, {"goto", true}, {"signed", true}, {"using", true}, {"continue", true},
    {"Agar", true}, {"sizeof", true}, {"virtual", true}, {"default", true}, {"inline", true},
    {"static", true}, {"Khali", true}, {"delete", true}, {"volatile", true}, {"do", true},
    {"long", true}, {"struct", true}, {"double", true}, {"mutable", true}, {"switch", true},
    {"while", true}, {"namespace", true}, {"template", true}, {"Marqazi", true}, {"Matn", true},
    {"input->", true}, {"output<-", true}
};

string trim(string& s) {
    while (!s.empty() && isspace(s.front())) s.erase(s.begin());
    while (!s.empty() && isspace(s.back())) s.pop_back();
    return s;
}

bool isAcceptanceState(int state) {
    for (int i = 0; i < countacc; i++) {
        if (state == acceptance[i]) return true;
    }
    return false;
}
int IndexSearchinArray(const string& category) {
    for (int i = 0; i < numInputs; i++) {
        if (inputs[i] == category) {
            return i;
        }
    }
    return -1;
}
int findIndex(char ch) {
    if (ch == '.') {
        return IndexSearchinArray(".");
    }
    if (isdigit(ch)) {
        return IndexSearchinArray("[0-9]");
    }
    if (ch == '+') {
        return IndexSearchinArray("+");
    }
    if (ch == '-') {
        return IndexSearchinArray("-");
    }
    if (ch == 'e') {
        return IndexSearchinArray("e");
    }
    if (ch == '<') {
        return IndexSearchinArray("<");
    }
    if (ch == '>') {
        return IndexSearchinArray(">");
    }
    if (ch == '=') {
        return IndexSearchinArray("=");
    }
    if (ch == '&') {
        return IndexSearchinArray("&");
    }
    if (ch == '|') {
        return IndexSearchinArray("|");
    }
    if (ch == ':') {
        return IndexSearchinArray(":");
    }
    if (ch == '!') {
        return IndexSearchinArray("!");
    }
    if (ch == '_') {
        return IndexSearchinArray("_");
    }
    /*if (ch == 'i') {
        return IndexSearchinArray("i");
    }
    if (ch == 'o') {
        return IndexSearchinArray("o");
    }  
    if (ch == 'n') {
        return IndexSearchinArray("n");
    }
    if (ch == 'u') {
        return IndexSearchinArray("u");
    }
    if (ch == 't') {
        return IndexSearchinArray("t");
    }
    if (ch == 'p') {
        return IndexSearchinArray("p");
    }*/
    if (isalpha(ch)) {
        return IndexSearchinArray("letters");
    }
    if (ch == '{' || ch == '(' || ch == ')' || ch == '}') {
        return IndexSearchinArray("[{()}]");
    }
    if (ch == '-' || ch == '%' || ch == '/' || ch == '*') {
        return IndexSearchinArray("[-%/*]");
    }

    return IndexSearchinArray("others");
}

//This is done to check if that specific lexeme is already in the symbol table or not
int isLexemeInSymbolTable(const string& lex) {
    ifstream file("symboltable.txt");
    string line;
    int index = 0;

    if (!file) {
        cerr << "Error opening symbolTable.txt" << endl;
        return false;
    }

    while (getline(file, line)) {
        if (line == lex) {
            file.close();
            return index;
        }
        index++;
    }

    file.close();
    return -1;
}

void writeToken(const string& token, const string& filename) {
    ofstream outFile(filename, ios::app);
    if (outFile.is_open()) {
        outFile << token << endl;
        outFile.close();
    }
    else {
        cerr << "Error: Could not open " << filename << " for writing.\n";
    }
}
void loadBuffer(ifstream& file) {
    char* currentBuffer = bufferToggle ? buffer2 : buffer1;

    file.read(currentBuffer, 24);
    int bytesRead = file.gcount();
    currentBuffer[bytesRead] = '\0';

    if (bytesRead == 0) return;

    bufferPtr = currentBuffer;
    bufferToggle = !bufferToggle;
}
char getNextChar(ifstream& file) {
    if (*bufferPtr == '\0') {
        loadBuffer(file);
        if (*bufferPtr == '\0') return EOF;
    }
    return *bufferPtr++;
}
char peekNextChar(ifstream& file) {
    if (*bufferPtr == '\0') {
        loadBuffer(file);
        if (*bufferPtr == '\0') return EOF;
    }
    return *bufferPtr;
}
char peekNextToNextChar(ifstream& file) {
    if (*bufferPtr == '\0') {  
        loadBuffer(file);
        if (*bufferPtr == '\0') return EOF;
    }

    if (*(bufferPtr + 1) == '\0') {
        loadBuffer(file);
        if (*bufferPtr == '\0') return EOF;
        return *bufferPtr;
    }

    return *(bufferPtr + 1);
}

void lexicalAnalyzer(ifstream& file) {
    int symboltablecounter = 0;
    int literaltablecounter = 0;

    char ch = getNextChar(file);

    bool isMultiLine = false;

    while (ch != EOF) {

        int state = 0;
        cout <<"char :" <<ch<< " ";
        /////Comments wala kaam
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            ch = getNextChar(file);
            continue;
        }
        if (ch == '/' && *bufferPtr == '/') {
            while (ch != '\n' && ch != EOF) 
                ch = getNextChar(file);
            continue;
        }
        if (ch == '/' && *bufferPtr == '*') {
            isMultiLine = true;
            while (isMultiLine) {
                ch = getNextChar(file);
                if (ch == '*' && *bufferPtr == '/') {
                    isMultiLine = false;
                    ch = getNextChar(file); 
                    ch = getNextChar(file);
                }
            }
            continue;
        }
        ////String wala kaam
        if (ch == '"') {
            string literal = "";
            ch = getNextChar(file);
            while (ch != '"' && ch != EOF) {
                literal += ch;
                ch = getNextChar(file);
            }

            writeToken(literal, "literaltable.txt");
            writeToken("<lit," + to_string(literaltablecounter) + ">", "tokens.txt");
            literaltablecounter++;
            ch = getNextChar(file);
            continue;
        }
        if(ch == EOF)
        {
            break;
        }
        

        string lex = "";
        cout << "start";
        while (!isAcceptanceState(state) && state != -1) {
            cout <<"state" << state << " ";
            lex += ch;
            //especially for handling gande keywords
            if ((lex == "input" && peekNextChar(file) == '-' && peekNextToNextChar(file) == '>')||(lex == "output" && peekNextChar(file)=='<' && peekNextToNextChar(file) == '-'))
            {
                ch = getNextChar(file);
                lex += ch;
                ch = getNextChar(file);
                lex += ch;
                writeToken("<" + lex + ">", "tokens.txt");
                break;
            }
            cout <<"lex"<< lex << " ";
            int inputIndex = findIndex(ch);
            if (inputIndex == -1) {
                writeToken(lex, "error.txt");
                //ch = getNextChar(file);
                break;
            }
            int newState = transitionTable[state][inputIndex];
            cout <<"new"<< newState << " ";
            //Error wala kaam
            if (newState == -1 || ch == '$' ) {
                writeToken(lex, "error.txt");
                //ch = getNextChar(file);
                break;
            }
            int sound = state;
            state = newState;

            cout << sound << inputIndex << advanceTable[sound][inputIndex] << endl;
            if (advanceTable[sound][inputIndex] == 1) {
                ch = getNextChar(file);

             
            }
        }

        if (isAcceptanceState(state)) {
            cout << "a";
            lex = trim(lex);
            if (state == 5 || state == 6 || state == 7 || state == 8) {

                writeToken("<" + lex + ">", "tokens.txt");
            }
            else if (state == 20) {
                if (keywordMap.find(lex) != keywordMap.end()) {
                    writeToken("<" + lex + ">", "tokens.txt");
                }
                else {
                    writeToken(lex, "error.txt");
                }
            }
            else if (state == 34) {
                if (isLexemeInSymbolTable(lex) == -1) {
                    writeToken("<id, " + to_string(symboltablecounter) + ">", "tokens.txt");
                    writeToken(lex, "symboltable.txt");
                    symboltablecounter++;
                }
                else
                {
                    writeToken("<id, " + to_string(isLexemeInSymbolTable(lex)) + ">", "tokens.txt");
                }
            }
            
        }
        ch = getNextChar(file);
    }

    return;
}

//this is just to populate tables

void getAcceptanceStates() {

    for (int i = 0; i < numStates; i++) {
        if (transitionTable[i][numInputs - 2] == 1) { 
            acceptance[countacc++] = i;
        }
    }
}
void getAsterickStates() {
    int count = 0; 

    for (int i = 0; i < numStates; i++) {
        if (transitionTable[i][numInputs - 1] == 1) { 
            asterickstates[count++] = i;
        }
    }
}
int stringToInt(const string& value) {
    int num = 0;
    int i = 0;
    bool isNegative = false;

    if (value[0] == '-') {
        isNegative = true;
        i++;
    }

    for (; i < value.length(); i++) {
        if (!isdigit(value[i])) {
            return -1;
        }
        num = num * 10 + (value[i] - '0');
    }

    if (isNegative) {
        num = -num;
    }

    return num;
}
void populateTables()
{
    ifstream file("tt1.txt");
    if (!file) {
        cerr << "Error: Cannot open file!" << endl;
    }

    string line;

    if (getline(file, line)) {
        stringstream ss(line);
        string temp;
        while (ss >> inputs[numInputs] && numInputs < 50) {
            numInputs++;
        }
    }

    while (getline(file, line) && numStates < 50) {
        stringstream ss(line);

        for (int j = 0; j < numInputs; j++) {
            string value;
            ss >> value;
            transitionTable[numStates][j] =  stringToInt(value);
        }

        numStates++;
    }

    file.close();
    getAcceptanceStates();
    getAsterickStates();

    for (int i = 0; i < numStates; i++) {
        advanceTable[i][0] = transitionTable[i][0];

        for (int j = 1; j < numInputs - 2; j++) {
            if (transitionTable[i][j] == -1) {
                advanceTable[i][j] = -1;
            }
            else {
                bool isMarked = false;
                for (int k = 0; k < 10; k++) {
                    if (transitionTable[i][j] == asterickstates[k]) {
                        isMarked = true;
                        break;
                    }
                }
                advanceTable[i][j] = isMarked ? 0 : 1;
            }
        }
    }
    cout << "Stored Input Symbols:\n";
    for (int i = 0; i < numInputs; i++) {
        cout << inputs[i] << " ";
    }
    cout << endl;
    cout << "\nStored Transition Table:\n";
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numInputs; j++) {
            cout << transitionTable[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "\nAdvance Table:\n";
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numInputs; j++) {
            cout << advanceTable[i][j] << "\t";
        }
        cout << endl;
    }

}

int main() {
   
    populateTables();
    ifstream input("input1.txt");

    cout << "\n\nOutput after this on terminal is for my own understanding please ignore it, ThankYou!!\n\n";
    lexicalAnalyzer(input);
    return 0;
}