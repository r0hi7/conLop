#include <map>
#include <set>
#include <string>
#include <vector>
#include "edge.cpp"
#include <iostream>
#include <fstream>
using namespace std;

extern bool isValidHoareTriple(vector<string> invariants,string stmt,string predicate);
extern set<int> get_succ(int);

extern map<int,set<string>> locks;

inline void put_lock(edge& e,string predicate)
{
	locks[e.u].insert("acquire_"+predicate);
	locks[e.v].insert("release_"+predicate);
}

void find_lock(map<int,set<string>> &vertex,map<edge,set<string>> &edge_stmt)
{
	std::ofstream out("/home/rohit/Desktop/cs639/project/result/Hoaretriples.dat");
	for(auto i :vertex)
	{
		set<string> invariants = i.second;

		for(auto const & predicate : invariants)
		{

			for(auto j:vertex)
			{
				int u = j.first;
				set<int> succ = get_succ(u);
				for(auto const v : succ)
				{
					edge e(u,v);

					set<string> stmt = edge_stmt[e];
					bool isValid = true;
					for(auto singleStmt:stmt)
					{

						//bool tmp = isValidHoareTriple(vec,singleStmt,predicate);
						out<<"{";
						for(auto v : vertex[u])
							out<<v<<" ";
						out<<predicate;
						out<<"}";
						out<<singleStmt;
						out<<"\n";
						//isValid = isValid and tmp;

					}
					//if(!isValid)
						//put_lock(e,predicate);
				}
			}
		}
	}
}

