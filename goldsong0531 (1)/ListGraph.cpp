#include "ListGraph.h"

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_Type = type;
	m_List = new map<int, int>[size];
	m_List_nodir = new map<int, int>[size];
}

ListGraph::~ListGraph()
{
	delete[] m_List;
	delete[] m_List_nodir;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	m->clear();
	for(auto it=m_List[vertex].begin(); it!=m_List[vertex].end(); it++)
		m->insert({it->first, it->second});
}

void ListGraph::getAdjacentEdgesWithoutDir(int vertex, map<int, int>* m)
{
	m->clear();
	for(auto it=m_List_nodir[vertex].begin(); it!=m_List_nodir[vertex].end(); it++)
		m->insert({it->first, it->second});
}

void ListGraph::insertEdge(int from, int to, int weight)
{
	m_List[from].insert({to, weight});
	m_List_nodir[from].insert({to, weight});
	m_List_nodir[to].insert({from, weight});
}

bool ListGraph::printGraph()
{
	cout << "======== PRINT ========" << endl;

	for(int i=0; i<m_Size; i++)
	{
		cout<<"["<<i<<"]";

		for(auto it_=m_List[i].begin(); it_!=m_List[i].end() && cout<<" -> "; it_++)
		{
			cout<<"("<<it_->first<<","<<it_->second<<")";
		}
		cout<<endl;
	}
	cout << "=======================" << endl;
	cout<<endl;
	return 1;
}