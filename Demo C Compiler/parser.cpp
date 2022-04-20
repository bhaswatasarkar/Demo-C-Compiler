#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <set>
#include <algorithm>
#include <cstdlib>
using namespace std;



//Grammar
struct Grammar{
unordered_map<string, vector<string>> productions;
unordered_map<string, set<string>> firstset, follow;
set<string> terminals;
static const string EPSILON, END;

void addgrammar(string nonterminal, vector<string> rule){
    productions[nonterminal] = rule;
}

Grammar(){
//     addgrammar("S", {"ID eq_op OPERAND ;",
//     //"var BINARY_OP ;",
//     "do { STATEMENT } while ( EXPRESSION ) ;","if ( EXPRESSION ) { STATEMENT }","get ( var ) ;","put ( OUTPUTS ) ;",
//     "break ;","continue ;",
//     //"OPERAND UNARY_OP ARITHMETIC_EXPRESSION ;"
//     });
//     //addgrammar("ID", {"var NEW"});
//     //addgrammar("NEW",{"( TYPE ) NEW", ""}); //Please use empty strings for epsilon
//     addgrammar("ID",{"var"});
//     addgrammar("OPERAND",{"var","const"});
//     addgrammar("TYPE",{ "int","float","char"});
//     addgrammar("OUTPUTS",{"str","const","var"});
//    // addgrammar("ARITHMETIC_EXPRESSION",{"OPERAND","OPERAND UNARY_OP ARITHMETIC_EXPRESSION"});
//     addgrammar("RELATIONAL_OP",{"gt_op","lt_op","ge_op","le_op"});
//     //addgrammar("BINARY_OP",{"incre_op","decre_op"});
//     addgrammar("UNARY_OP", {"add_op", "sub_op"});
//     addgrammar("STATEMENT",{"S STATEMENTDASH"});//CHIRU EKHANE CHANGE KORLAM
//     addgrammar("STATEMENTDASH",{"STATEMENT","#"});//EI DUTO LEFT FACTORING
//     addgrammar("EXPRESSION",{"OPERAND RELATIONAL_OP OPERAND"});
//     terminals.insert({"break", "char", "continue", "do", "else", "float", "if", "int", "struct", "while", 
//     "gets", "puts", "add_op", "sub_op", "incre_op", "decre_op", "gt_op", "lt_op", "ge_op", "le_op", 
//     "eq_op", "str", "const", "var"
//     });

// addgrammar("VAR",{"String_name"});
// addgrammar("IDENTIFIER",{"DATA_TYPE","void"});
// addgrammar("DATA_TYPE",{"int","float"});
// addgrammar("LOOP_DEF",{"do { STATEMENT ; } while ( EXPRESSION )"});
// addgrammar("JUMP_STATEMENT",{"continue","break"});
// addgrammar("STATEMENT",{"PARA_STATEMENT","DEC_STATEMENT","EXPRESSION","LOOP_DEF","JUMP_STATEMENT","STATEMENT ; STATEMENT"});
// addgrammar("ASSIGN",{"eq_op"});
// //addgrammar("UNARY_EXP",{"VAR UNARY_OPER","VAR OPER VAR","UNARY_OPER VAR","VAR"});
// addgrammar("OPER",{"add_op","sub_op"});
// addgrammar("CONDITION",{"UNARY_EXP CON_OPER UNARY_EXP"});
// addgrammar("CON_OPER",{"gt_op","lt_op","ge_op","le_op"});
// addgrammar("PARA_STATEMENT",{"DATA_TYPE VAR","PARA_STATEMENT , PARA_STATEMENT"});
// addgrammar("DEC_STATEMENT",{"DATA_TYPE VAR","DATA_TYPE VAR ASSIGN UNARY_EXP","DEC_STATEMENT , VAR","DEC_STATEMENT , VAR ASSIGN UNARY_EXP"});
// terminals.insert({"String_name","void","int","float","do","while","continue","break","gt_op","lt_op","ge_op","le_op","add_op","sub_op","eq_op"});

addgrammar("S", {"ID eq_op OPERAND","do { STATEMENT } while ( EXPRESSION )",
    "if ( EXPRESSION ) { STATEMENT }","get ( var )","put ( OUTPUTS )",
    "break","continue",
    //"OPERAND UNARY_OP ARITHMETIC_EXPRESSION"
    });
    //addgrammar("ID", {"var NEW"});
    //addgrammar("NEW",{"( TYPE ) NEW", ""}); //Please use empty strings for epsilon
    addgrammar("ID",{"var"});
    addgrammar("OPERAND",{"var","const"});
    //addgrammar("TYPE",{ "int","float","char"});
    addgrammar("OUTPUTS",{"str","const","var"});
    // addgrammar("ARITHMETIC_EXPRESSION",{"OPERAND ARITHMETIC_EXPRESSION_1"});
    // addgrammar("ARITHMETIC_EXPRESSION_1",{"","UNARY_OP ARITHMETIC_EXPRESSION"});
    addgrammar("RELATIONAL_OP",{"gt_op","lt_op","ge_op","le_op"});
    addgrammar("BINARY_OP",{"incre_op","decre_op"});
    addgrammar("UNARY_OP", {"add_op", "sub_op"});
    addgrammar("STATEMENT",{"S ; STATEMENT_1"});
    addgrammar("STATEMENT_1",{"", "STATEMENT"});
    addgrammar("EXPRESSION",{"OPERAND RELATIONAL_OP OPERAND"});
    terminals.insert({"break", "char", "continue", "do", "else", "float", "if", "int", "struct", "while", 
    "gets", "puts", "add_op", "sub_op", "incre_op", "decre_op", "gt_op", "lt_op", "ge_op", "le_op", 
    "eq_op", "str", "const", "var"});
}

template<class T> bool isdisjoint(const set<T> &s1, const set<T> &s2) {
    typename set<T>::const_iterator first1 = s1.begin(), last1 = s1.end(), first2 = s2.begin(), last2 = s2.end();
    while(first1 != last1 && first2 != last2) {
        if(*first1 < *first2) first1++;
        else if(*first2 < *first1) first2++;
        else return false;
    }
    return true;
}

vector<string> split(const string& str) {
    //TODO: do LATER LMAO!!!
    vector<string> result;
    string current;
    for(char ch : str) {
        if(ch == ' ') {result.push_back(current); current.clear();}
        else current.push_back(ch);
    }
    if(!current.empty()) result.push_back(current);
    return result;
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
                for(const auto& subRuleSymbol : split(subRule)) {
                    const auto& subRuleSymbolFirst = firstset[subRuleSymbol];
                    newSet.insert(subRuleSymbolFirst.begin(), subRuleSymbolFirst.end());
                    newSet.erase(EPSILON);
                    if(newSet.find(EPSILON) == newSet.end()) {allNullable = false; break;}
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
    for(Itr i = begin; i < end; i++) {
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
    follow["S"].insert(END);
    do {
        runScan = false;
        for(const auto& rulepair : productions) {
            const auto& nonterminal = rulepair.first;
            const auto& rule = rulepair.second;
            for(const auto& sb : rule) {
                const auto& subRule = split(sb);
                for(size_t i = 0; i < subRule.size(); i++) {
                    const auto& targetSymbol = subRule[i];
                    if(terminals.find(targetSymbol) != terminals.end()) continue;
                    set<string> newSet(follow[targetSymbol]);
                    const auto& betaSet = compositeFirstSet(subRule.begin()+1, subRule.end());
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
        for(const auto& sb : rule) {
            const auto& subRule = split(sb);
            const auto& firstSet = compositeFirstSet(subRule.begin(), subRule.end());
            for(const auto& terminal : firstSet) putintable(nonterminal, terminal, subRule);
            if(firstSet.empty() || firstSet.find(EPSILON) != firstSet.end()) 
                for(const auto& terminal : follow[nonterminal]) putintable(nonterminal, terminal, subRule);
        }
    }
}


void checkIfLL1() {
    for(const auto& rulepair : productions) {
        const auto& nonterminal = rulepair.first;
        const auto& rules = rulepair.second;
        if(find(rules.begin(), rules.end(), string()) != rules.end() && !isdisjoint(firstset[nonterminal], follow[nonterminal])) {
            cerr<<"Given grammar is not LL(1)\n";
            cerr<<"For nonterminal "<<nonterminal<<" first set = ";
            for(const auto& symb : firstset[nonterminal]) cerr<<symb<<" ";
            cerr<<"and follow set = ";
            for(const auto& symb : follow[nonterminal]) cerr<<symb<<" ";
            cerr<<"\nFirst and follow sets are not disjoint.\n";
            exit(1);
        }
        for(size_t i = 0; i < (rules.size() - 1); i++)
            for(size_t j = i+1; j < rules.size(); j++) {
            const auto& prod1 = split(rules[i]), prod2 = split(rules[j]);
            const auto& prod1First = compositeFirstSet(prod1.begin(), prod1.end()), prod2First = compositeFirstSet(prod2.begin(), prod2.end());
            if(isdisjoint(prod1First, prod2First)) continue;
            cerr<<"Given grammar is not LL(1)\n";
            cerr<<"For nonterminal "<<nonterminal<<"\n";
            cerr<<"Production "<<rules[i]<<"; first set = "; for(const auto& symb : prod1First) cerr<<symb<<" ";
            cerr<<"\nProduction "<<rules[j]<<"; first set = "; for(const auto& symb : prod2First) cerr<<symb<<" ";
            cerr<<"\nFirst sets are not disjoint.\n";
            exit(1);
        }
    }
}

};
const string Grammar::EPSILON("#"), Grammar::END("$");

int main(){
    Grammar g;
    //cout<<g.productions["OPERAND"][1];
    g.generateFirstSets(); g.generateFollowSets();
    cout<<"First Sets\n";
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
    } 
    g.checkIfLL1();
    g.generateParsingTable();
    return 0;
}