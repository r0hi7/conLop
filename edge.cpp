#include <iostream>

using namespace std;

struct edge{
	public :
		int u,v;
		edge(int a,int b)
		{
			u=a;
			v=b;
		}
		bool operator<(const edge &rhs)const
		{
			if(u<rhs.u)
			return true;
			else if(u>rhs.u)
				return false;
			else
				return (v<rhs.v)?true:false;
		}

		friend std::ostream & operator<<(std::ostream& out,const edge &e)
		{
			out<<"("<<e.u<<","<<e.v<<")";
			return out;
		}
};
