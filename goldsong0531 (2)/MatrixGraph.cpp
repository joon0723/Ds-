#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_Mat = new int*[size];
	for(int i=0; i<size; i++)
	{
		m_Mat[i] = new int[size];
		memset(m_Mat[i], 0, sizeof(int)*size);
	}
}

MatrixGraph::~MatrixGraph()
{
	for(int i=0; i<getSize(); i++)
	{
		delete[] m_Mat[i];
	}
	delete[] m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	m->clear();
	for(int i=0; i<m_Size; i++)
	{
		if (m_Mat[vertex][i] != 0)
			m->insert({i, m_Mat[vertex][i]});
	}
}
void MatrixGraph::getAdjacentEdgesWithoutDir(int vertex, map<int, int>* m)
{
	m->clear();
	for(int i=0; i<m_Size; i++)
	{
		if (m_Mat[vertex][i] != 0)
			m->insert({i, m_Mat[vertex][i]});
		if (m_Mat[i][vertex] != 0)
			m->insert({i, m_Mat[i][vertex]});
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)
{
	m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph()
{
	if( m_Size < 0 )
		return 0;

	cout << "======== PRINT ========" << endl;
    cout << "    ";
    for(int i=0; i<m_Size; i++)
        cout << "[" << i << "] ";
    cout << endl;

    for(int i=0; i<m_Size; i++)
    {
        cout << "[" << i << "] ";
        for(int j=0; j<m_Size; j++)
			cout << ' ' << m_Mat[i][j] << "  ";
        cout << endl;
    }
    cout << "=======================" << endl;

	return 1;
}
