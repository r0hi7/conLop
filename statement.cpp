#include <set>
#include <string>
#include <vector>
#include "edge.cpp"

using namespace std;
vector<edge> edg;

set<string> get_predicates(string& str)
{
	set<string> stmts;
	int idx = str.find("Function start dummy edge");
	if(idx > str.size()) // if not a dummy edge
	{
		int label = str.find("label=");
		label+=7;
		string tmp("");
		while(label<str.size())
		{
			if(str[label]!=';')
			{
				if(str[label]!=' ')
					tmp+=str[label];
			}
			else
			{
				if(tmp.find("=")<str.size())
					stmts.insert(tmp);
				tmp="";
			}
			label++;
		}
	}
	return stmts;
}

set<int> get_succ(int u)
{
	set<int> res;
	for(auto const& tmp:edg)
	{
		edge e = tmp;
		if(e.u==u)
			res.insert(e.v);
	}
	return res;
}
