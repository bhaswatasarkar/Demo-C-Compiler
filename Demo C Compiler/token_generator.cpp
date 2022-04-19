#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

string constr="";
int dfa=0;
vector<string> token_list;

void addToken(string);
void start(char);
void state1(char);
void final(char);

bool isCondition1(char ch){
    return (ch==',' || ch==';' || ch=='\'' || ch=='+' || ch=='*' || ch=='-' || ch == '{'
    || ch=='}' || ch=='(' || ch==')' || ch=='[' || ch==']' || ch=='=' || ch=='>' || ch=='<');
}

isnumber(string str){
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
      dfa=0;
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
    else if(constr.length()==0 && (ch=='('||ch=='{'||ch=='['||ch==')'||ch=='}'||ch==']')){
        addToken(s);//passing ch
        dfa = 2;
    }
    else
    {
        constr=constr+ch;
        dfa = 1;
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
    else if(str=="{")
        token_list.push_back("{");
    else if(str=="}")
        token_list.push_back("}");
    else if(str=="[")
        token_list.push_back("[");
    else if(str=="]")
        token_list.push_back("]");
    else {
        if(isnumber(str))
            token_list.push_back("const");
        else
            token_list.push_back("var");
    }
    
    
}

int main(){
    ifstream myfile;
    string temp="";
    myfile.open("input.txt");
    if(myfile.is_open()){
        char ch;

    while(myfile){
         ch = myfile.get();
        if(ch=='{')
            break;
    }

    while(myfile){
        ch = myfile.get();
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
    for(auto e: token_list){
        cout<<e<<endl;
    }
 

}

