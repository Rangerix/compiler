#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;


int isterminal(char x){
	if(x>='A'&&x<='Z')
		return 0;
	return 1;
}

vector<string> rules;
int rulecount=0;
vector<set<string> > statelabel1(100);
map<pair<int,char>, int> dfa;
int index;
map<char, set<char> > first;
map<char, set<char> > follow;


set<string> getclosure(char ch){
	set<string> closure;
	if(isterminal(ch))
		return closure;
	int i;
	for(i=0;i<rulecount;i++){
		if(rules[i][0]==ch){
			string temp1=rules[i].substr(0,2)+"."+rules[i].substr(2);
			closure.insert(temp1);
			if(rules[i][2]!=ch){
				set<string> temp2=getclosure(rules[i][2]);
				closure.insert(temp2.begin(),temp2.end());
			}
		}
	}
	return closure;
}

set<string> calculateI0(){
	set<string> i0;
	i0=getclosure(rules[0][0]);
	return i0;
}

int getlabel(set<string> state){
	int i=0;
	for(i=0;i<statelabel1.size();i++)
		if(state==statelabel1[i])
			return i;
	return i;
}

set<string> calculategoto(set<string> state, char ch){
	set<string> gotoset;
	set<string>::iterator itr;
	for(itr=state.begin();itr!=state.end();itr++){
		string str=(*itr);
		int x=str.find('.');
		if((x!=str.length()-1)&&(str[x+1]==ch)){
			string temp1=str.substr(0,x)+str[x+1]+"."+str.substr(x+2);
			gotoset.insert(temp1);

			if(temp1.find('.')!=temp1.length()-1){
				char s=temp1[temp1.find('.')+1];
				set<string> temp2=getclosure(s);
				gotoset.insert(temp2.begin(),temp2.end());
			}
		}
	}
	//printf("goto(%d,%c) : \n",getlabel(state),ch );
	for(itr=gotoset.begin();itr!=gotoset.end();itr++){
		//cout<<(*itr)<<endl;
	}
	//cout<<endl;
	return gotoset;
}

vector<char> getmoves(set<string> state){
	int i;
	vector<char> alphabet;
	set<string> :: iterator itr;
	for(itr=state.begin();itr!=state.end();itr++){
		string temp1=(*itr);
		int x=temp1.find('.');
		if(x!=temp1.length()-1){
			for(i=0;i<alphabet.size();i++){
				if(alphabet[i]==temp1[x+1])
					break;
			}
			if(i>=alphabet.size()&&temp1[x+1]!='$')
				alphabet.push_back(temp1[x+1]);
		}
	}
	return alphabet;
}

map<pair<int,char>,int>  computedfa(){		//int here denotes state number
	set<string> i0=calculateI0();
	statelabel1[0]=i0;
	int i,j,k;
	index=1;
	map<set<string>,int>::iterator itr;
	for(j=0;j<index;j++){
		set<string> state=statelabel1[j];
		vector<char> alphabet=getmoves(state);
		for(i=0;i<alphabet.size();i++){
			set<string> nextstate=calculategoto(state,alphabet[i]);
			if(getlabel(nextstate)==statelabel1.size()){ 		//new state
				statelabel1[index]=nextstate;
				index++;
			}
			int x=getlabel(state);
			int y=getlabel(nextstate);
			pair<int,char> temppair=make_pair(x,alphabet[i]);
			if(dfa.find(temppair)==dfa.end())
				dfa[temppair]=y;
		}
	}
	map<pair<int,char>,int> :: iterator itr1;
	cout<<"\ndfa:\n";
	for(itr1=dfa.begin();itr1!=dfa.end();itr1++){
		pair<int,char> temppair=itr1->first;
		cout<<(temppair.first)<<" , "<<(temppair.second)<<" : "<<(itr1->second)<<endl;
	}
	return dfa;
}

void getfirst(char key,int ruleno){
	
	if(isterminal(key)){
		first[key].insert(key);
		return;
	}
	int j,k;
	for(j=ruleno;j<rulecount;j++){
		if(rules[j][0]==key){
			if(rules[j][2]=='#'){
				first[key].insert('#');
			}
			else if(isterminal(rules[j][2])){
				first[key].insert(rules[j][2]);
			}
			else{
				for(k=2;k<rules[j].length();k++){
					if(isterminal(rules[j][k])){
						first[key].insert(rules[j][k]);
						break;
					}
					else{
						if(rules[j][k]==key){
							getfirst(key,j+1);
							if(first[key].find('#')==first[key].end())
								break;
						}
						else{
							getfirst(rules[j][k],0);
							first[key].insert(first[rules[j][k]].begin(),first[rules[j][k]].end());
							first[key].erase('#');
							if(first[rules[j][k]].find('#')==first[rules[j][k]].end())
								break;
						}
					}
				}
				if(k==rules[j].length())
					first[key].insert('#');
			}
		}
	}
}

void getfollow(char key){
	if(rules[0][0]==key)
		follow[key].insert('$');

	int i,j,k;
	for(i=0;i<rulecount;i++){
		for(j=0;j<rules[i].length();j++){
			if(rules[i][j]==key){
				if(j+1!=rules[i].length()){
					for(k=j+1;k<rules[i].length();k++){
						getfirst(rules[i][k],0);
						follow[key].insert(first[rules[i][k]].begin(),first[rules[i][k]].end());
						follow[key].erase('#');
						if(first[rules[i][k]].find('#')==first[rules[i][k]].end())
							break;
					}
					if(k==rules[i].length()&&key!=rules[i][0]){
						getfollow(rules[i][0]);
						follow[key].insert(follow[rules[i][0]].begin(),follow[rules[i][0]].end());
					}
				}
				else if (rules[i][0]!=key){
					getfollow(rules[i][0]);
					follow[key].insert(follow[rules[i][0]].begin(),follow[rules[i][0]].end());
				}
				follow[key].erase('=');
			}
		}
	}

}

vector<vector<string> > preparetable(set<char> alphabet){
	vector<char> header;
	int i,j;
	vector<char> vec(alphabet.begin(),alphabet.end());

	vector<vector<string> > parsingtable(index+1,vector<string> (vec.size()+1));
	for(i=0;i<index;i++){
		for(j=0;j<vec.size();j++){
			parsingtable[i][j]="--";
		}
	}

	for(i=0;i<index;i++){
		for(j=0;j<vec.size();j++){
			pair<int,char> temppair=make_pair(i,vec[j]);
			string opn;
			if(dfa.find(temppair)!=dfa.end()){
				opn="S"+to_string(dfa[temppair]);
				parsingtable[i][j]=opn;
			}
			
		}
	}
	for(i=0;i<index;i++){
		cout<<setw(3)<<i<<":";
		for(j=0;j<vec.size();j++){
			cout<<setw(5)<<parsingtable[i][j]<<"|";
		}
		cout<<endl;
	}
	//shift moves done
	//now reduce moves
	for(i=0;i<index;i++){
		set<string> state=statelabel1[i];
		set<string>::iterator itr;
		string temp1;
		for(itr=state.begin();itr!=state.end();itr++){
			temp1=(*itr);
			if(temp1.find('.')==temp1.length()-1){
				temp1=temp1.substr(0,temp1.length()-1);
				for(j=0;j<rulecount;j++){
					if(rules[j]==temp1)
						break;
				}
				//j has the reduction rule no
				string opn="R"+to_string(j);
				set<char> followset=follow[temp1[0]];
				for(set<char> :: iterator itr1=followset.begin();itr1!=followset.end();itr1++){
					char ch=(*itr1);
					for(j=0;j<vec.size();j++){
						if(vec[j]==ch)
							break;
					}
					parsingtable[i][j]=opn;
				}
			}

		}
	}
	//now accept case
	for(i=0;i<index;i++){
		set<string> state=statelabel1[i];
		set<string>::iterator itr;
		for(itr=state.begin();itr!=state.end();itr++){
			string temp1=(*itr);
			if((temp1.find('.')==temp1.length()-2)&&(temp1.find('$')==temp1.length()-1)){
				for(j=0;j<vec.size();j++){
					if(vec[j]=='$')
						break;
				}
				parsingtable[i][j]="ac";
			}
		}	
	}
	cout<<endl<<endl<<"    ";
	for(i=0;i<vec.size();i++){
		cout<<setw(5)<<vec[i]<<" | ";
	}
	cout<<endl;
	for(i=0;i<index;i++){
		cout<<setw(3)<<i<<":";
		for(j=0;j<vec.size();j++){
			cout<<setw(5)<<parsingtable[i][j]<<" | ";
		}
		cout<<endl;
	}
	return parsingtable;
}

void parsing(vector<vector<string> > parsingtable,set<char> alphabet){
	string parse;
	cout<<"enter string : \n";
	cin>>parse;
	vector<string> mystack;
	int stindex=0;
	mystack.push_back("0");
	vector<char> vec(alphabet.begin(),alphabet.end());
	int i,j;
	for(i=0;i<parse.length();){

		for(j=0;j<mystack.size();j++)
			cout<<mystack[j];
		cout<<"\t";
		for(j=i;j<parse.length();j++)
			cout<<parse[j];
		cout<<"\t";
		int row=stoi(mystack[mystack.size()-1]);
		for(j=0;j<vec.size();j++){
			if(vec[j]==parse[i])
				break;
		}
		string action=parsingtable[row][j];
		cout<<action;
		if(action=="ac"){
			cout<<"\naccept\n";
			break;
		}
		if(action=="--"){
			cout<<"\nno action for this move...\n";
			break;
		}
		if(action[0]=='S'){
			string temp1(1,parse[i]);
			mystack.push_back(temp1);
			i++;
			mystack.push_back(action.substr(1));
		}
		else if(action[0]=='R'){
			int j=action[1]-'0';
			int x=rules[j].length()-2;
			x*=2;
			while(x--){
				mystack.pop_back();
			}
			mystack.push_back(rules[j].substr(0,1));
			for(x=0;x<vec.size();x++){
				if(vec[x]==rules[j][0])
					break;
			}
			row=stoi(mystack[mystack.size()-2]);
			string temp1=parsingtable[row][x];
			mystack.push_back(temp1.substr(1));
		}
		cout<<endl;
	}
} 

int main()
{
	int i,j;
	cout<<"enter number of productions: \n";
	cin>>rulecount;
	string str;
	set<char> alphabet;

	cout<<"enter rules\n";
	for(i=0;i<rulecount;i++){
		cin>>str;
		rules.push_back(str);
	}
	for(i=0;i<rulecount;i++){
		for(j=0;j<rules[i].length();j++){
			alphabet.insert(rules[i][j]);
		}
	}
	for(i=0;i<rulecount;i++){
		getfirst(rules[i][0],0);
	}
	map<char, set<char> > :: iterator itr;
	cout<<"first : \n";
	for(itr=first.begin();itr!=first.end();itr++){
		char key=itr->first;
		cout<<key<<" | ";
		set<char> temp=itr->second;
		set<char>::iterator itr1;
		for(itr1=temp.begin();itr1!=temp.end();itr1++){
			cout<<(*itr1)<<",";
		}
		cout<<endl;
	}
	for(i=0;i<rulecount;i++){
		getfollow(rules[i][0]);
	}
	cout<<"follow : \n";
	for(itr=follow.begin();itr!=follow.end();itr++){
		char key=itr->first;
		cout<<key<<" | ";
		set<char> temp=itr->second;
		set<char>::iterator itr1;
		for(itr1=temp.begin();itr1!=temp.end();itr1++){
			cout<<(*itr1)<<",";
		}
		cout<<endl;	
	}
	computedfa();
	vector<vector<string> > p=preparetable(alphabet);
	parsing(p,alphabet);
return 0;
}