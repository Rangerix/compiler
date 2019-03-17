#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

int count=0;
map<string,vector<string> > rules;
map<string,vector<string> > newrules;


void printmap(map<string,vector<string> > rules){

	map<string,vector<string> > :: iterator itr;
	for(itr=rules.begin();itr!=rules.end();itr++){
		string key=(itr->first);
		vector<string> val=(itr->second);
		cout<<key<<" -> " ;
		for(int i=0;i<val.size();i++)
			cout<<val[i]<<" ";
		cout<<endl;
	}


}

void remove_left(){
	int i;
	map<string,vector<string> > :: iterator itri,itrj;
	for(itri=rules.begin();itri!=rules.end();itri++){
		vector<string> vec=(itri->second);

		cout<<"deal : "<<(itri->first)<<" -> ";
		for(i=0;i<vec.size();i++){
			cout<<vec[i]<<" ";
		}
		cout<<endl;

		vector<string> tempvec;
		int flag=0;
		for(itrj=rules.begin();itrj!=itri;itrj++){
			string s=(itrj->first);
			int l=s.length();
			vector<string> svec=(itrj->second);
			for(i=0;i<vec.size();i++){
				if(vec[i].substr(0,l)==s){
					//cout<<vec[i]<<endl;
					string temp=vec[i].substr(l);

					for(int j=0;j<svec.size();j++){
						tempvec.push_back(svec[j]+temp);
					}
					
				}
				else{
					int j;
					for(j=0;j<tempvec.size();j++)
						if(tempvec[j]==vec[i]) break;
					if(j==tempvec.size()) tempvec.push_back(vec[i]);
				}
			}	
			flag=1;
		}
		/*for(i=0;i<tempvec.size();i++){
			cout<<tempvec[i]<<" ";
		}*/
		cout<<endl;
		if(flag) rules[itri->first]=tempvec;
		//printmap(rules);
		//cout<<endl;

		//immediate left recursion removal

		tempvec=itri->second;
		vector<string> newvec;
		vec.clear();
		string s=itri->first;
		int l=s.length();
		flag=0;
		for(i=0;i<tempvec.size();i++){
			if(tempvec[i].substr(0,l)==s)
				flag=1;
		}
		cout<<"flag : "<<flag<<endl;
		if(!flag) continue;
		for(i=0;i<tempvec.size();i++){
			if(tempvec[i].substr(0,l)!=s){
				vec.push_back(tempvec[i]+s+"'");
			}
			else{
				string temp=tempvec[i].substr(l);
				temp=temp+s+"'";
				newvec.push_back(temp);
			}
		}
		newvec.push_back("#");
		rules[itri->first]=vec;
		newrules[s+"'"]=newvec;
		printmap(rules);
		cout<<endl;
	}
}

int main()
{
	cin>>count;
	int i;

	for(i=0;i<count;i++){
		string str;
		cin>>str;
		string key=str.substr(0,1);
		string temp=str.substr(2);
		if(rules.find(key)==rules.end()){//not found
			vector<string> val;
			val.push_back(temp);
			rules[key]=val;
		}
		else{
			rules[key].push_back(temp);
		}
	}
	
	
	remove_left();
	printmap(rules);
	printmap(newrules);
return 0;
}