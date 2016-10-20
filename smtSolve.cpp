#include "z3++.h"
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>

using namespace std;
using namespace z3;

string get_lhs(string string,bool predicate);
vector<char> get_rhs_op(string str,bool is_predicate);
vector<string> get_rhs_var(string str,bool is_predicate);  //for prediacte or not

vector<expr> get_expr(string,context&,bool is_predicate);
void add_to_solver_eq(solver&,vector<expr>,vector<char>);
void add_to_solver_neq(string relop,solver&,vector<expr>,vector<char>);
string get_relop(string);



//function to check whether a string is int or not
inline int is_num(string str)
{
	int num=1;
	std::string::iterator is=str.begin();
	if(str[0]=='+'||str[0]=='-')
		is++;
	for(;is<str.end();++is)
		num = num && isdigit(*is);
	return num;
}

bool isValidHoareTriple(vector<string> invariants,string stmt,string predicate)
{
	context c;
	solver s(c);

	add_to_solver_eq(s,get_expr(stmt,c,false),get_rhs_op(stmt,false));

	for(auto const pred : invariants)
	{

		if(!(pred.compare("true")==0))
		{
			vector<char> vc = get_rhs_op(pred,true);    //ok
			vector<expr> ve = get_expr(pred,c,true);
			add_to_solver_neq(get_relop(pred),s,ve,vc);
		}
	}

	if(!(predicate.compare("true")==0))
		add_to_solver_neq(get_relop(predicate),s,get_expr(predicate,c,true),get_rhs_op(predicate,true));
	//todo if the ! triple is SAT then its not valid
	return !s.check();
}

//equlaity constraint
vector<expr> get_expr(string s,context &c,bool is_predicate)
{
	string l = get_lhs(s,is_predicate);
	vector<string> vs = get_rhs_var(s,is_predicate);
	vector<expr> ve;
	ve.push_back(c.int_const(l.c_str()));			//lhs expr at 0
	int len = vs.size();
	for(int i=0;i<len;++i)					//considering the rhs expr only two with one + - in between
	{
		string tmp = vs[i];
		if(!is_num(tmp))
			ve.push_back(c.int_const(tmp.c_str()));
		else
			ve.push_back(c.int_val(tmp.c_str()));
	}
	return ve;
}


string get_relop(string str)
{
	string relops[]={"==","!=","<=",">=","<",">"};
	set<string> relop(relops,relops+6);
	for(set<string>::iterator it = relop.begin();it!=relop.end();++it)
		if(str.find(*it)<str.size())
			return *it;
	return NULL;
}

void add_to_solver_neq(string relop,solver& s,vector<expr> ve,vector<char> op)
{

	if(relop=="!=")
	{
		if(op.size()==0)
		{
			s.add(!(ve[0] != ve[1]));
			return ;
		}

		else
			if(op[0]=='+')
				s.add(!(ve[0] != ve[1] + ve[2]));
			else if(op[0]=='-')
				s.add(!(ve[0] != ve[1] + ve[2]));
	}

	else if(relop=="==")
	{
		if(op.size()==0)
		{
			s.add(!(ve[0] == ve[1]));
				return ;
		}

		else
			if(op[0]=='+')
				s.add(!(ve[0] == ve[1] + ve[2]));
			else if(op[0]=='-')
				s.add(!(ve[0] == ve[1] + ve[2]));
	}

	else if(relop==">=")
	{
		if(op.size()==0)
		{
			s.add(!(ve[0] >= ve[1]));
			return ;
		}
		else
			if(op[0]=='+')
				s.add(!(ve[0] >= ve[1] + ve[2]));
			else if(op[0]=='-')
				s.add(!(ve[0] >= ve[1] - ve[2]));
	}
	else if(relop=="<=")
	{
		if(op.size()==0)
		{
			s.add(!(ve[0] <= ve[1]));
			return ;
		}
		else
			if(op[0]=='+')
				s.add(!(ve[0] <= ve[1] + ve[2]));
			else if(op[0]=='-')
				s.add(!(ve[0] <= ve[1] - ve[2]));
	}

	else if(relop=="<")
	{
		if(op.size()==0)
		{
			s.add(!(ve[0] < ve[1]));
				return ;
		}
		else
			if(op[0]=='+')
				s.add(!(ve[0] < ve[1] + ve[2]));
			else if(op[0]=='-')
				s.add(!(ve[0] < ve[1] - ve[2]));
	}
	else if(relop==">")
	{
		if(op.size()==0)
		{
			s.add(!(ve[0] > ve[1]));
			return ;
		}
		else
			if(op[0]=='+')
				s.add(!(ve[0] > ve[1] + ve[2]));
			else if(op[0]=='-')
				s.add(!(ve[0] > ve[1] - ve[2]));
	}
}


void add_to_solver_eq(solver &s,vector<expr> ve,vector<char> op)
{
	if(op.size()==0)
	{
		s.add(!(ve[0] == ve[1]));
		return ;
	}

	if(op[0]=='+')
		s.add(!(ve[0] == ve[1] + ve[2]));
	if(op[0]=='-')
		s.add(!(ve[0] == ve[1] - ve[2]));
}

string get_lhs(string str,bool is_predicate)
{
	int idx ;
	if(!is_predicate)
		idx=str.find("=");
	else
		idx=str.find(get_relop(str));
	string lhs="";
	int len = str.size();
	if(idx<len)
		for(int i=0;i<idx;++i)
			lhs+=str[i];
	return lhs;
}

vector<char> get_rhs_op(string str,bool is_predicate)
{
	vector<char> op;

	//can add multiple operators
	op.push_back('+');
	op.push_back('-');

	vector<char> res;

	int idx;
	if(!is_predicate)
		idx=str.find("=");
	else

		idx=str.find(get_relop(str));

	int len = str.size();
	if(idx<len)
		for(int i=idx+1;i<len;++i)
			for(vector<char>::iterator it=op.begin();it<op.end();++it)
				if(*it==str[i])
					res.push_back(str[i]);

	return res;
}

vector<string> get_rhs_var(string str,bool is_predicate)
{
	set<char> op;

	//can add multiple operators
	op.insert('+');
	op.insert('-');

	vector<string> res;

	int idx;
	if(!is_predicate)
		idx=str.find("=");
	else
		idx=str.find(get_relop(str));

	int len = str.size();
	if(idx<len and !is_predicate)
		str = str.substr(idx+1,str.size());

	else if(is_predicate)
		str = str.substr(idx+get_relop(str).size(),str.size());

	idx=0;
	string tmp("");
	len = str.size();
	while(idx<len)
	{
		//if idx is operator then skip
		if(op.find(str[idx]) == op.end())
		{
			if(str[idx]!=' ')
				tmp+=str[idx];
		}
		else
		{
			res.push_back(tmp);
			tmp="";
		}
		idx++;
	}
	res.push_back(tmp);
	return res;
}
