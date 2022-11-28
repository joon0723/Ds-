#include "Manager.h"

vector<string> split(string str, char Delimiter) {
    istringstream iss(str);
    string buf;
    vector<string> r;
 
    while (getline(iss, buf, Delimiter))
        r.push_back(buf);
 
    return r;
}

Manager::Manager()
{
	graph = nullptr;
	fout.open("log.txt", ios::app);
	load = 0;
}

Manager::~Manager()
{
	if(load)
		delete graph;
	fout.close();
}

void Manager::run(const char* command_txt){
	ifstream fin;
	fin.open(command_txt);
		
	if(!fin)
	{
		cout<<"[ERROR] command file open error!"<<endl;
		return;
	}
	
	char* str=NULL;
	char* str2=NULL;
	char buf[129]={0};

	while(fin.getline(buf, 128))
	{
		string b(buf);
		vector<string> parsed = split(b, ' ');
		if(parsed[0] == "LOAD")
		{
			if (!LOAD(parsed[1].c_str())) printErrorCode(100);
		}
		if(parsed[0] == "PRINT")
		{
			if (!PRINT()) printErrorCode(200);
		}
		if(parsed[0] == "BFS")
		{
			if (!mBFS(stoi(parsed[1]))) printErrorCode(300);
		}
		else if(parsed[0] == "DFS")
		{
			if (!mDFS(stoi(parsed[1]))) printErrorCode(400);
		}
		else if(parsed[0] == "DFS_R")
		{
			if (!mDFS_R(stoi(parsed[1]))) printErrorCode(500);
		}
		else if(parsed[0] == "KRUSKAL")
		{
			if (!mKRUSKAL()) printErrorCode(600);
		}
		else if(parsed[0] == "DIJKSTRA")
		{
			if (!mDIJKSTRA(stoi(parsed[1]))) printErrorCode(700);
		}
		else if(parsed[0] == "BELLMANFORD")
		{
			if (!mBELLMANFORD(stoi(parsed[1]), stoi(parsed[2]))) printErrorCode(800);
		}
		else if(parsed[0] == "FLOYD")
		{
			if (!mFLOYD()) printErrorCode(900);
		}
		else if(parsed[0] == "EXIT")
		{
			cout << "======= EXIT ========" << endl;
			cout << "success" << endl;
			cout << "=====================" << endl;
		}
	}	
	fin.close();
}

bool Manager::LOAD(const char* filename)
{
    ifstream input_file;
	input_file.open(filename);
    if (!input_file.is_open() || load) {
		return false;
    }

	string type, size, line;
	getline(input_file, type);
	getline(input_file, size);
	if (type == "L")
	{
		int vertex = -1;
		graph = new ListGraph(0,stoi(size));
		while (getline(input_file, line)){
			vector<string> parsed = split(line, ' ');
			if (parsed.size() == 1)
				vertex = stoi(parsed[0]);
			else
				graph->insertEdge(vertex, stoi(parsed[0]), stoi(parsed[1]));
		}
	}
	else if (type == "M")
	{
		graph = new MatrixGraph(1,stoi(size));
		for(int i=0; i<graph->getSize(); i++)
		{
			getline(input_file, line);
			vector<string> parsed = split(line, ' ');
			for(int j=0; j<parsed.size(); j++)
			{
				if (stoi(parsed[j]) != 0)
					graph->insertEdge(i, j, stoi(parsed[j]));
			}
		}
	}
	cout << "========= LOAD ========" << endl;
	cout << "Success" << endl;
	cout << "=======================" << endl << endl;
    return true;
}

bool Manager::PRINT()
{
	if(graph->printGraph())
		return true;
	return false;
}

bool Manager::mBFS(int vertex)
{
	if (graph->getSize() <= vertex)
		return false;

	cout << "========= BFS =========" << endl;
	cout << "startvertex: " << vertex << endl;
	BFS(graph, vertex);
	cout << endl << "=======================" << endl << endl;
    return true;
}

bool Manager::mDFS(int vertex)
{
	if (graph->getSize() <= vertex)
		return false;

	cout << "========= DFS =========" << endl;
	cout << "startvertex: " << vertex << endl;
	DFS(graph, vertex);
	cout << endl << "=======================" << endl << endl;
	return true;
}


bool Manager::mDFS_R(int vertex)
{
	if (graph->getSize() <= vertex)
		return false;
	auto visit = new vector<bool>(graph->getSize(), false);

	cout << "======== DFS_R ========" << endl;
	cout << "startvertex: " << vertex << endl;
	DFS_R(graph, visit, vertex);
	cout << endl << "=======================" << endl << endl;
	return true;
}

bool Manager::mDIJKSTRA(int vertex)
{
	if (graph->getSize() <= vertex)
		return false;

	cout << "====== Djikstra =======" << endl;
	cout << "startvertex: " << vertex << endl;
	Dijkstra(graph, vertex);
	cout << "=======================" << endl << endl;
	return true;
}

bool Manager::mKRUSKAL()
{
	return Kruskal(graph);
}

bool Manager::mBELLMANFORD(int s_vertex, int e_vertex)
{
	return Bellmanford(graph, s_vertex, e_vertex);
}

bool Manager::mFLOYD()
{
	return FLOYD(graph);
}

void Manager::printErrorCode(int n)
{
	cout<<"======== ERROR ========"<<endl;
	cout<<n<<endl;
	cout<<"======================="<<endl;
}
