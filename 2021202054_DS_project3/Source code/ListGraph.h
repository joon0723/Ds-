#ifndef _LIST_H_
#define _LIST_H_

#include "Graph.h"

class ListGraph : public Graph{
private:
	map < int, int >* m_List, * m_List_nodir;
	ofstream fout;

public:	
	ListGraph(bool type, int size);
	~ListGraph();
		
	void getAdjacentEdges(int vertex, map<int, int>* m);
	void getAdjacentEdgesWithoutDir(int vertex, map<int, int>* m);
	void insertEdge(int from, int to, int weight);
	bool printGraph();
};

#endif