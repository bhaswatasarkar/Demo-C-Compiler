#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <set>
#include <algorithm>
#include <fstream>
#include <deque>
#include <cstdlib>
using namespace std;

////Lexer
using namespace std;

string constr="";
int dfa=0,depth=0;
vector<string> token_list;

void addToken(string);
void start(char);
void state1(char);
void final(char);

unordered_map<string, int> symbol_table;

bool isCondition1(char ch){
    return (ch==',' || ch==';' || ch=='\'' || ch=='+' || ch=='*' || ch=='-' || ch == '{'
    || ch=='}' || ch=='(' || ch==')' || ch=='[' || ch==']' || ch=='=' || ch=='>' || ch=='<');
}

bool isnumber(string str){
    int i;
    for(i=0;i<str.length();i++){
        if(!isdigit(str[i]))
            return false;
    }
    return true;
}


//initial state of the dfa(state 0)
void start(char ch){
    if((ch>=97 && ch<=122)||(ch>=65 && ch<=90)){
        constr=constr+ch;
        dfa = 1;
    }
    else 
        final(ch);
}

//state1
void state1(char ch){
    if(isCondition1(ch)==true){
        final(ch);
        dfa = 2;
    }
    else{
        constr = constr + ch;
        dfa=1;
    }
}

//final state of dfa(state 2)
void final(char ch){
    string s(1,ch);
    if(constr.length()!=0){
        addToken(constr);
        constr="";
        addToken(s);//passing ch
        dfa = 2;
    }
    else if((ch>=97 && ch<=122)||(ch>=65 && ch<=90) || (isdigit(ch)==true))
    {
        constr=constr+ch;
        dfa = 1;
    }
    else{
        addToken(s);//passing ch
        dfa = 2;
    }
}

void addToken(string str){
    if(str=="break")
        token_list.push_back("break");
    else if(str=="char")
        token_list.push_back("char");
    else if(str=="continue")
        token_list.push_back("continue");
    else if(str=="do")
        token_list.push_back("do");
    else if(str=="else")
        token_list.push_back("else");
    else if(str=="float")
        token_list.push_back("float");
    else if(str=="if")
        token_list.push_back("if");
    else if(str=="int")
        token_list.push_back("int");
    else if(str=="struct")
        token_list.push_back("struct");
    else if(str=="while")
        token_list.push_back("while");
    else if(str=="gets")
        token_list.push_back("gets");
    else if(str=="puts")
        token_list.push_back("puts");
    else if(str=="+")
        token_list.push_back("add_op");
    else if(str=="-")
        token_list.push_back("sub_op");
    else if(str==">")
        token_list.push_back("gt_op");
    else if(str=="<")
        token_list.push_back("lt_op");
    else if(str=="=")
        token_list.push_back("eq_op");
    else if(str==";")
        token_list.push_back(";");
    else if(str=="(")
        token_list.push_back("(");
    else if(str==")")
        token_list.push_back(")");
    else if(str=="{"){
        token_list.push_back("{");
        depth++;
    }
    else if(str=="}"){
        token_list.push_back("}");
        depth--;
    }
    else if(str=="[")
        token_list.push_back("[");
    else if(str=="]")
        token_list.push_back("]");
    else {
        if(isnumber(str))
            token_list.push_back("const");
        else{
            token_list.push_back("var");
            symbol_table[str]=depth;
        }
    }
    
    
}

vector<string> gettokens(const char *fname){
    ifstream myfile;
    string temp="";
    myfile.open(fname);
    if(myfile.is_open()){
        char ch;

    while(myfile){
         ch = myfile.get();
        if(ch=='{')
            break;
    }

    while(myfile){
        ch = myfile.get();
        if(ch==-1)break;
        if(ch==' '||ch=='\n')
            continue;
        if(dfa==0)
            start(ch);
        else if (dfa==1){
            state1(ch);
            }
        else if(dfa==2)
            final(ch);
    }
    }

    token_list.pop_back();
    token_list.push_back("$");
    /*for(auto e: token_list){
        cout<<e<<endl;
    }

    cout<<"Symbol table"<<endl;
    for(auto e:symbol_table){
        cout<<e.first<<" : "<<e.second<<endl;
    }*/
 
    return token_list;
}
////Lexer

//Grammar
struct Grammar{
vector<string> nonterminalArray;
unordered_map<string, vector<vector<string>>> productions;
unordered_map<string, set<string>> firstset, follow;
set<string> terminals;
static const string EPSILON, END;

void addgrammar(const string &nonterminal, const vector<vector<string>> &rule){
    nonterminalArray.push_back(nonterminal);
    productions[nonterminal] = rule;
}

Grammar(){
    addgrammar("STATEMENT",{
        {"S", ";", "STATEMENT"},
        {}
    });
    addgrammar("S", {
        {"OPERAND", "S_OP1"}, 
        {"do", "{", "STATEMENT", "}", "while", "(", "EXPRESSION", ")",},
        {"if", "(", "EXPRESSION", ")", "{", "STATEMENT", "}"}, 
        {"get", "(", "var", ")", "put", "(", "OUTPUTS", ")"},
        {"break"},{"continue"},
    });
    addgrammar("S_OP1", {
        {"eq_op", "OPERAND"},
        {"BINARY_OP"},
        {"UNARY_OP", "ARITHMETIC_EXPRESSION"}
    });
    addgrammar("OPERAND",{
        {"var"},
        {"const"}
    });
    addgrammar("TYPE",{{"int"},{"float"},{"char"}});
    addgrammar("OUTPUTS",{{"str"},{"const"},{"var"}});
    addgrammar("ARITHMETIC_EXPRESSION",{{"OPERAND", "ARITHMETIC_EXPRESSION_1"}});
    addgrammar("ARITHMETIC_EXPRESSION_1",{{},{"UNARY_OP", "ARITHMETIC_EXPRESSION"}});
    addgrammar("RELATIONAL_OP",{{"gt_op"}, {"lt_op"}, {"ge_op"}, {"le_op"}});
    addgrammar("BINARY_OP",{{"incre_op"}, {"decre_op"}});
    addgrammar("UNARY_OP", {{"add_op"}, {"sub_op"}});
    addgrammar("EXPRESSION",{{"OPERAND", "RELATIONAL_OP", "OPERAND"}});
    terminals.insert({"break", "char", "continue", "do", "else", "float", "if", "int", "struct", "while", 
    "gets", "puts", "add_op", "sub_op", "incre_op", "decre_op", "gt_op", "lt_op", "ge_op", "le_op", 
    "eq_op", "str", "const", "var", "(", ")", "{", "}", ";", "get", "put", "int", "float", "char", 
    });
}

string removeImmediateLeftRecursion(const string &nonterminal) {
    string newNonterminal; size_t nameGen = 1;
    do {
        newNonterminal = nonterminal + "_r" + to_string(nameGen); nameGen++;
    } while (productions.find(newNonterminal) != productions.end());
    vector<vector<string>> leftRecursionRule, nonLeftRecursionRule, newRule, newRule2;
    const auto& rule = productions[nonterminal];
    for(const auto& subRule : rule) {
        if(!(subRule.size() > 0 && subRule[0] == nonterminal)) nonLeftRecursionRule.push_back(subRule);
        else leftRecursionRule.push_back(subRule);
    }
    if(leftRecursionRule.empty()) return "";
    for(auto &subRule : nonLeftRecursionRule) {
        vector<string> subRuleCopy(subRule); subRuleCopy.push_back(newNonterminal);
        newRule.push_back(std::move(subRuleCopy));
    }
    productions[nonterminal] = std::move(newRule);
    for(auto &subRule : leftRecursionRule) {
        vector<string> subRuleCopy(subRule.begin()+1, subRule.end()); subRuleCopy.push_back(newNonterminal);
        newRule2.push_back(std::move(subRuleCopy));
    }
    newRule2.push_back(vector<string>());
    productions[newNonterminal] = std::move(newRule2);
    return newNonterminal;
}

void removeChainedLeftRecursion() {
    unordered_map<size_t, string> newNonterminals;
    for(size_t i = 0; i < nonterminalArray.size(); i++) {
        for(size_t j = 0; j < i; j++) {
            vector<vector<string>> newRuleSet;
            for(auto& subRule : productions[nonterminalArray[i]]) {
                if(subRule.size() > 0 && subRule[0] == nonterminalArray[j])
                    for(auto& jSubRule : productions[nonterminalArray[j]]) {
                        vector<string> newSubRule(jSubRule);
                        newSubRule.insert(newSubRule.end(), subRule.cbegin()+1, subRule.cend());
                        newRuleSet.push_back(std::move(newSubRule));
                    }
                else newRuleSet.push_back(subRule);
            }
            productions[nonterminalArray[i]] = newRuleSet;
        }
        const auto newNonterminal = removeImmediateLeftRecursion(nonterminalArray[i]);
        if(!newNonterminal.empty()) newNonterminals[i] = newNonterminal;
    }

    decltype(nonterminalArray) newNonterminalArray;
    for(size_t i = 0; i < nonterminalArray.size(); i++) {
        newNonterminalArray.push_back(nonterminalArray[i]);
        auto itr = newNonterminals.find(i);
        if(itr != newNonterminals.end()) newNonterminalArray.push_back(itr->second);
    }
    nonterminalArray = std::move(newNonterminalArray);
}

void generateFirstSets() {
    bool runScan;
    for(const auto& rulepair : productions) firstset[rulepair.first] = set<string>();
    for(const auto& terminal : terminals) firstset[terminal] = set<string>({terminal});
    do {
        runScan = false;
        for(const auto& rulepair : productions) {
            const auto& nonterminal = rulepair.first;
            set<string> newSet(firstset[nonterminal]); //explicit copy reqd
            const auto& rule = rulepair.second;
            for(const auto& subRule : rule) {
                bool allNullable = true;
                for(const auto& subRuleSymbol : subRule) {
                    const auto& subRuleSymbolFirst = firstset[subRuleSymbol];
                    newSet.insert(subRuleSymbolFirst.begin(), subRuleSymbolFirst.end());
                    newSet.erase(EPSILON);
                    if(subRuleSymbolFirst.find(EPSILON) == subRuleSymbolFirst.end()) {allNullable = false; break;}
                }
                if(allNullable) newSet.insert(EPSILON);
            }
            if(newSet != firstset[nonterminal]) runScan = true;
            firstset[nonterminal] = std::move(newSet);
        }
    } while (runScan);
}

template<class Itr>
set<string> compositeFirstSet(Itr begin, Itr end) {
    set<string> result;
    for(Itr i = begin; i != end; i++) {
        const auto& symbol = *i;
        const auto& symbolFirst = firstset[symbol];
        result.insert(symbolFirst.begin(), symbolFirst.end());
        if(symbolFirst.find(EPSILON) == symbolFirst.end()) {
            result.erase(EPSILON);
            break;
        }
    }
    return result;
}

void generateFollowSets() {
    bool runScan;
    for(const auto& rulepair : productions) follow[rulepair.first] = set<string>();
    follow["STATEMENT"].insert(END);
    do {
        runScan = false;
        for(const auto& rulepair : productions) {
            const auto& nonterminal = rulepair.first;
            const auto& rule = rulepair.second;
            for(const auto& subRule : rule) {
                for(size_t i = 0; i < subRule.size(); i++) {
                    const auto& targetSymbol = subRule[i];
                    if(terminals.find(targetSymbol) != terminals.end()) continue;
                    set<string> newSet(follow[targetSymbol]);
                    const auto& betaSet = compositeFirstSet(subRule.begin()+i+1, subRule.end());
                    newSet.insert(betaSet.begin(), betaSet.end());
                    newSet.erase(EPSILON);
                    auto& alphaSet = follow[nonterminal];
                    if(betaSet.empty() || betaSet.find(EPSILON) != betaSet.end()) newSet.insert(alphaSet.begin(), alphaSet.end());
                    if(newSet != follow[targetSymbol]) runScan = true;
                    follow[targetSymbol] = std::move(newSet);
                }
            }
        }
    } while(runScan);
}

unordered_map<string, unordered_map<string, vector<string>>> parsingTable;

void putintable(const string &nt, const string& t, const vector<string> &prod) {
    auto& m = parsingTable[nt];
    if(m.find(t) != m.end()) {
        cerr<<"Error: NOT LL 1\n";
        cerr<<"For nonterminal "<<nt<<" and terminal "<<t<<"\n";
        cerr<<"Production 1: "; for(const auto& symb : m[t]) cerr<<symb<<" "; cerr<<"\n";
        cerr<<"Production 2: "; for(const auto& symb : prod) cerr<<symb<<" "; cerr<<"\n";
        exit(1);
    } else m[t] = prod;
}

void generateParsingTable() {
    for(const auto& rulepair : productions) {
        const auto& nonterminal = rulepair.first;
        if(parsingTable.find(nonterminal) == parsingTable.end()) parsingTable[nonterminal] = unordered_map<string, vector<string>>();
        const auto& rule = rulepair.second;
        for(const auto& subRule : rule) {
            const auto& firstSet = compositeFirstSet(subRule.begin(), subRule.end());
            for(const auto& terminal : firstSet) putintable(nonterminal, terminal, subRule);
            if(firstSet.empty() || firstSet.find(EPSILON) != firstSet.end()) 
                for(const auto& terminal : follow[nonterminal]) putintable(nonterminal, terminal, subRule);
        }
    }
}

void parse(const vector<string> &inputArray) {
    deque<string> stack({"$", "STATEMENT"});
    size_t i = 0;
    while(!stack.empty()) {
        const auto& symbol = stack.back();
        if(symbol == inputArray[i]) {i++; stack.pop_back(); continue;}
        else if(terminals.find(symbol) != terminals.end()) {
            cerr<<"Error: expected "<<symbol<<" found "<<inputArray[i]<<"\n";
            exit(1);
        }
        auto itr = parsingTable[symbol].find(inputArray[i]);
        if(itr == parsingTable[symbol].end()) {
            cerr<<"Error; unexpected "<<inputArray[i];
            exit(1);
        }
        stack.pop_back();
        stack.insert(stack.end(), itr->second.rbegin(), itr->second.rend());
    }
}

};
const string Grammar::EPSILON("#"), Grammar::END("$");

int main(int argc, char *argv[]){
    Grammar g;
    //cout<<g.productions["OPERAND"][1];
    g.removeChainedLeftRecursion();
    g.generateFirstSets(); g.generateFollowSets();
    /*cout<<"First Sets\n";
    for(const auto& symbolpair : g.firstset) {
        cout<<symbolpair.first<<" ::= ";
        for(const auto& symbol : symbolpair.second) cout<<symbol<<" ";
        cout<<"\n";
    } 
    cout<<"\nFollow Sets\n";
    for(const auto& symbolpair : g.follow) {
        cout<<symbolpair.first<<" ::= ";
        for(const auto& symbol : symbolpair.second) cout<<symbol<<" ";
        cout<<"\n";
    } */
    g.generateParsingTable();
    g.parse(gettokens(argv[1]));
    return 0;
}