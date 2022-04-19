#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<string> simple_tokenizer(string s)
{
    vector<string> ans;
    stringstream ss(s);
    string word;
    while (ss >> word) {
        ans.push_back(word);
    }
    return ans;
}
 

int main(){
    string str,substr="",substr2;
    int i;
    getline(cin,str);
    vector<string> arr = simple_tokenizer(str);
    if(arr[0]==arr[2]){
        cout<<"left recursion present"<<endl;
    }

    i=3;
    while(arr[i]!="|"){
        substr=substr+arr[i]+" ";
        i++;
    }

    int pos = str.find("|");
    substr2 = str.substr(pos+2);
    
    cout<<arr[0]<<" -> "<<substr2<<" "<<"NEW"<<endl;
    cout<<"NEW -> "<<substr<<"NEW | #"<<endl;

return 0;
}

