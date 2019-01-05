#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <string.h>
#include <locale>
#include <algorithm>    // std::find_if
#include <iterator>
#include <iomanip>

using namespace std;

enum TokenType
{
    KEYWORD,
    NAME,
    EQLS,
    ADD,
    DEC,
    EQN,
    STR
};

char* Types[] = {"keyword","name","equals","add","dec","eqn","str"};

struct Var
{
    string value;
    string name;
    enum TokenType tokenType;
    
};

struct Frame
{
    vector<Var>varField;
    vector<Var>opStack;
    uint8_t *byteStream;
};

vector<Frame>frameStack;


string getStringBefore(char key,string str, size_t t)
{
   size_t pos = str.find(key,t);
   return str.substr(pos,pos-t);   
}


vector<Var> Tokenize(string src)
{
    vector<Var>varField;

    for(size_t t = 0; t < src.size();t++)
    {
        if(!isspace(src[t]))
        {
            if(src.substr(t,4) == "var ")
            {
                t+=4;
                size_t pos = src.find(' ',t);
                //var name = 5
                Var var;
                var.name = src.substr(t,pos-t);
                t = pos;

                if(src[t+1] == '=')
                {
                    t+=3;
                    size_t valpos = src.find('\n',t);
                    string assigned_value = src.substr(t,valpos-t);
                    var.value = assigned_value;                    
                    if(isdigit(assigned_value[0])) ///var x = (5)
                    {
                        var.tokenType = DEC;
                    }
                    else if(isalpha(assigned_value[0]))
                    {
                        var.tokenType = EQN;
                    }                    
                }
                else /// var x;
                {
                    var.value = "0";
                    var.tokenType = DEC;
                }
                t+=2;
                varField.push_back(var);
            }
        }
    }
    return varField;
}

vector<Var>::iterator searchVar(vector<Var>varField,string name)
{
    vector<Var>::iterator it =  find_if(varField.begin(), varField.end(), [&name](const Var& obj) 
                {return obj.name == name;});
        return it;
}

void execFrame(vector<Var>varField)
{
    for(auto var : varField)
    {
        if(var.tokenType == EQN)
        {
            
            string assigned_value = var.value;
            vector<string>eqnTokens;
            istringstream in (assigned_value);
            copy(istream_iterator<string>(in), istream_iterator<string>(), 
                                                back_inserter(eqnTokens));

            /// To implement: RPN
            /// var z = "x + y + "
            /// Search Var Field for matching var names to eqnTokens 0 2 4 6
            for(size_t vT = 0; vT < eqnTokens.size(); vT+=2)
            {   
                string var_token = eqnTokens[vT];
                string opToken = eqnTokens[vT+1];
                string rToken = eqnTokens[vT+2];

                
                auto it = find_if(varField.begin(), varField.end(), [&var_token](const Var& obj) 
                {return obj.name == var_token;});

                
                if(it != varField.end())
                {
                  // found element. it is an iterator to the first matching element.
                  // if you really need the index, you can also get it:
                  auto index = distance(varField.begin(), it);
                  
                    if(opToken == "+")
                    {
                       if(it->tokenType == DEC)
                       {

                        auto it2 = find_if(varField.begin(), varField.end(), [&rToken](const Var& obj) 
                            {return obj.name == rToken;});
                            cout<<"first asssigned="<<assigned_value<<endl;
                           assigned_value = to_string(stod(assigned_value))+to_string(stod(it->value)+stod(it2->value));
                           cout<<it->name<<":"<<it->value<<" + "<<it2->name<<":"<<it2->value<<" = "<<assigned_value<<endl;
                        //   vT+=2;
                       }
                    }
                }
                else
                {                       
                    cout<<"error var "<<var_token<<" is not found";
                }
                 
            }
        }
    }
}

void PrintLocalVariables(vector<Var>varField)
{
cout<<"Local Var Table\n"
    <<"Name Type Value\n";
    for(const Var &var : varField)
    {
        cout<<setw(4)<<var.name<<" "<<setw(1)<<Types[var.tokenType]<<" "<<setw(6)<<var.value<<endl;
    }
}

string LoadFile(string path)
{
  std::ifstream ifs(path);
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
return content;
}
int main()
{
    
    string file = LoadFile("input");
    
    auto a = Tokenize(file); 
    PrintLocalVariables(a);
    cout<<"________________\n";
    execFrame(a);

    return 0;
}