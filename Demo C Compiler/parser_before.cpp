#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

//Grammar
class Grammar{

public:
 unordered_map<string, vector<string>> productions;
 

void addgrammar(string nonterminal, vector<string> rule){
    productions[nonterminal] = rule;
}

Grammar(){
    addgrammar("S", {"ID eq_op OPERAND ;","var BINARY_OP ;","do { STATEMENT } while ( EXPRESSION ) ;","if ( EXPRESSION ) { STATEMENT }","get ( var ) ;","put ( OUTPUTS ) ;",
    "break ;","continue ;","OPERAND UNARY_OP ARITHMETIC_EXPRESSION ;"});
    addgrammar("ID", {"var NEW"});
    addgrammar("NEW",{"( TYPE ) NEW", "#"});
    addgrammar("OPERAND",{"var","const"});
    addgrammar("TYPE",{ "int","float","char"});
    addgrammar("OUTPUTS",{"str","const","var"});
    addgrammar("ARITHMETIC_EXPRESSION",{"OPERAND","OPERAND UNARY_OP ARITHMETIC_EXPRESSION"});
    addgrammar("RELATIONAL_OP",{"gt_op","lt_op","ge_op","lt_op"});
    addgrammar("BINARY_OP",{"incre_op","decre_op"});
    addgrammar("STATEMENT",{"S STATEMENT","S"});
    addgrammar("EXPRESSION",{"OPERAND RELATIONAL_OP OPERAND"});
}



};

int main(){
    Grammar g;
    cout<<g.productions["OPERAND"][1];
    return 0;
}
