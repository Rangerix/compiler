
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

typedef pair<int,int> position;

int ispunct(char c){
	vector<char> plist;
	plist.push_back(')');
	plist.push_back('(');
    plist.push_back('{');
    plist.push_back('}');
    plist.push_back('[');
    plist.push_back(']');
    
	plist.push_back('&');
	plist.push_back('_');
	plist.push_back(';');
	plist.push_back(',');
	for(int i=0;i<plist.size();i++)
		if(plist[i]==c)
			return 1;
	return 0;
}

int isoperator(char c){
	vector<char> oplist;
	oplist.push_back('+');
	oplist.push_back('-');
	oplist.push_back('/');
	oplist.push_back('*');
	oplist.push_back('%');
	oplist.push_back('=');
	oplist.push_back('>');
	oplist.push_back('<');
	oplist.push_back('?');
	oplist.push_back(':');
	oplist.push_back('|');

	for(int i=0;i<oplist.size();i++)
		if(oplist[i]==c)
			return 1;
	return 0;
}

int delim(char c){
	if(c==' '||c=='\t'||isoperator(c)||ispunct(c))
		return 1;
	return 0;
}

int isnumeric(string s){
	int i;
	for(i=0;i<s.length();i++)
		if(s[i]<'0'||s[i]>'9') 
			return 0;
	return 1;
}

int literal(string s){
	if(s[0]=='"'&&s[s.length()-1]=='"')
		return 1;
	return 0;
}

int keyword(string s){
	vector<string> keys;
	keys.push_back("int");
	keys.push_back("main");
	keys.push_back("return");
	keys.push_back("if");
	keys.push_back("else");
	keys.push_back("for");
	keys.push_back("while");
	keys.push_back("continue");
	keys.push_back("argc");
	keys.push_back("argv");

	for(int i=0;i<keys.size();i++)
		if(s==keys[i])
			return 1;
	return 0;
}

string replacement(string str){
    if(str=="main"){ return "b";}
    if(str=="int"){ return "n";}
    if(str=="float"){ return "f";}
    if(str=="void"){ return "v" ;}
    if(str=="if"){ return "i" ;}
    if(str=="else"){ return "l";}
    if(str=="read"){ return "r";}
    if(str=="print"){ return "p";}
    if(str=="return"){ return "t";}
    if(str.length()==1&&isoperator(str[0])){ return str.substr(0,1);}
    if(str.length()==1&&ispunct(str[0])){ return str.substr(0,1);}
    if(isnumeric(str)){return "d";}
    if(literal(str)){return "d";}
    return "b";    //numeric, literal, variable
}

int main(int argc,char **argv)
{
	if(argc!=2){
		cout<<"inputfile name \n";
		return 1;
	}

	ifstream file;
	file.open(argv[1]);
    string outputstring="";

	map<position,string> mymap;
	map<position,string>::iterator myitr;
	int linecount=-1,l,i;
	string s,stemp;
	while(file){
		linecount++;
		getline(file,s);
		l=s.length();
		stemp="";
		position pos;
		pos.first=linecount;
		pos.second=0;
		for(i=0;i<s.length();i++){
			if(s[i]=='\t')
				s[i]=' ';
		}
		for(i=0;i<s.length();i++){
			if(s[i]=='"'){
				pos.second=i;
				stemp=s[i];
				s[i]=' ';
				i++;
				while(s[i]!='"'){
					stemp+=s[i];
					if(i<l) s[i]=' ';
					i++;
				}
				stemp+=s[i];
				s[i]=' ';
				pair<position,string> temp(pos,stemp);
				if(stemp!="") {
                    mymap.insert(temp);
                //    cout<<stemp<<":"<<replacement(stemp)<<endl;
                    outputstring+=replacement(stemp);        
                }
				stemp="";
			}
		}
		for(i=0;i<s.length();i++){
			//cout<<i<<",";
			if(delim(s[i])){
				//cout<<" ( "<<pos.first<<" , "<<pos.second<<" ) ";
				//cout<<"stemp : "<<stemp<<endl;
				pair<position,string> temp(pos,stemp);
				if(stemp!="") {
                    mymap.insert(temp);
              //      cout<<stemp<<":"<<replacement(stemp)<<endl;
                    outputstring+=replacement(stemp);
                }
				stemp="";
				while(i<l&&delim(s[i])) i++;
				if(i<l) stemp=s[i];
				s[i]=' ';
				pos.second=i;
			}
			else{
				stemp+=s[i];
				s[i]=' ';
			}
			//cout<<i<<","<<stemp<<"x"<<stemp.size()<<"\n";
		}
		pos.second=l;
		pair<position,string> temp(pos,stemp);
		if(stemp!="") {
            mymap.insert(temp);
            //cout<<stemp<<":"<<replacement(stemp)<<endl;
            outputstring+=replacement(stemp);
        }

        
		for(i=0;i<s.length();i++){
			if(s[i]!=' '){
				pos.second=i;
				stemp=s[i];
				pair<position,string> temp(pos,stemp);
				mymap.insert(temp);
                //cout<<stemp<<":"<<replacement(stemp)<<endl;
                outputstring+=replacement(stemp);
			}
		}
	}
    outputstring="";
	for(myitr=mymap.begin();myitr!=mymap.end();myitr++)
	{
		cout<<" ( "<<(myitr->first).first<<" , "<<(myitr->first).second<<" ) ";
		cout<<myitr->second;
		s=myitr->second;
        cout<<":"<<replacement(s);
        outputstring+=replacement(s);
		/*if(s.size()==1&&ispunct(s[0]))
			cout<<" punctuation";
		else if(s.size()==1&&isoperator(s[0]))
			cout<<" operator";
		else if(isnumeric(s))
			cout<<" constant";
		else if(literal(s))
			cout<<" literal";
		else if(keyword(s))
			cout<<" keyword";
		else
			cout<<" variable";*/
		cout<<endl;
	}
    cout<<"LEXICAL ANALYSIS : \n"<<outputstring<<endl;
return 0;
}