#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include "edge.cpp"

using namespace std;

bool is_dummy_edge(set<string> &str);
set<string> get_predicates(string& str);
template<typename T,typename R>void print_set(map<T,set<R> >& input);
bool isValidHoareTriple(vector<string> invariants,string stmt,string predicate);
void find_lock(map<int,set<string>> &vertex,map<edge,set<string>> &edge_stmt);
extern vector<char> get_rhs_op(string,bool);
extern vector<edge> edg;

set<int> get_succ(int u);

string redend="\u001b[0m",redstart="\u001b[1;31m";

map<int,set<string> > locks;

int main()
{
	

	//cout<<input;

	string line;
	map<edge,string> pm;
	map<edge,set<string> > stmtMap;
	map<int,set<string> > vertex;
	
	set<string> trueSet;
	trueSet.insert("true");
	
	ifstream input("/home/rohit/Desktop/cs639/project/result/output2.dat");
        if( input.is_open()){
            int a,b;
            string s;
            char ch[1000];
            while(input.getline(ch,1000) ){
                stringstream ss(ch);
                ss>>a>>b>>s;
                edge e(a,b);
                edg.push_back(e);
                set<string> tmp;
                tmp.insert(s);
                stmtMap[e]=tmp;
                //cout << a << b << s <<"\n";
            }
       }
 	input.close();
	ifstream input1("/home/rohit/Desktop/cs639/project/result/output1.dat");

	if( input1.is_open()){
            int a,b;
            string s;
            char ch[1000];
            while(input1.getline(ch,1000) ){
                stringstream ss(ch);
                ss>>a>>s;
                //edge e(a,b);
                set<string> tmp;
                tmp.insert(s);
                vertex[a]=tmp;
                //cout << a << b << s <<"\n";
            }
       }


	find_lock(vertex,stmtMap);
	cout<<"Hoare triple written in file";
	/*for(auto l : locks)
	{
		cout<<redstart<<l.first<<" "<<redend;
		set<int> succ = get_succ(l.first);
		for(auto s : l.second)
			cout<<s<<"  ";

		cout<<endl;
		for(auto v:succ)
		{
			edge e(l.first,v);
			for(auto tmp :stmtMap[e])
				cout<<tmp<<"\n";
		}
		cout<<endl;
	}*/
}

template<typename P,typename Q>
void print_set(map<P,set<Q>>& input)
{
	for(auto v:input)
	{
		cout<<v.first<<" ";
		for(auto &t:v.second)
			cout<<t<<" ";
		cout<<endl;
	}
}

