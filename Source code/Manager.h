#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "GraphMethod.h"
#include "ListGraph.h"
#include "MatrixGraph.h"
#include <sstream>


class Manager{
private:
	Graph* graph;
	ofstream fout;
	int load;

public:
	Manager();
	~Manager();

	void run(const char * command_txt);
	
	bool LOAD(const char* filename);
	bool PRINT();
	
	bool mBFS(int vertex);
	bool mDFS_R(int vertex);
	bool mDFS(int vertex);
	bool mDIJKSTRA(int vertex);
	bool mKRUSKAL();
	bool mBELLMANFORD(int s_vertex, int e_vertex);
	bool mFLOYD();

	void printErrorCode(int n);	
};

#endif