#include "GraphMethod.h"

bool BFS(Graph* graph, int vertex)
{
	vector<bool> visit(graph->getSize(), false);
	map<int, int>* m = new map<int, int>();

    queue<int> q;
    q.push(vertex);
    visit[vertex] = true;
    
    while(!q.empty()){
    	int f = q.front();
        q.pop();

	    graph->getAdjacentEdgesWithoutDir(f, m);
        
        cout << f;
        
        for(auto it=m->begin(); it!=m->end(); it++)
            if(!visit[it->first]){
            	q.push(it->first);
                visit[it->first] = true;
            }

        if (!q.empty()) cout << " -> ";
    }
    return false;
}

bool DFS(Graph* graph, int vertex)
{
	vector<bool> visit(graph->getSize(), false);
	map<int, int>* m = new map<int, int>();

    stack<int> s;
    s.push(vertex);
    
    visit[vertex] = true;
    cout << vertex;

    while(!s.empty()){
        int t=s.top();
        s.pop();

	    graph->getAdjacentEdgesWithoutDir(t, m);

        for(auto it=m->begin(); it!=m->end(); it++)
        {
            if(!visit[it->first])
            {
                cout << " -> " << it->first;
                visit[it->first] = true;
                s.push(t);
                s.push(it->first);
                break;
            }
        }
    }
    return true;
}

bool DFS_R(Graph* graph, vector<bool>* visit, int vertex)
{
	map<int, int>* m = new map<int, int>();
	graph->getAdjacentEdgesWithoutDir(vertex, m);

    (*visit)[vertex] = true;
    cout << vertex;
    for(auto i: *visit)
        if (!i)
        {
            cout << " -> ";
            break;
        }

    for(auto it=m->begin(); it!=m->end(); it++)
    {
        if(!(*visit)[it->first])
            DFS_R(graph, visit, it->first);
    }
    return true;
}

void insertsort(vector<pair<int, pair<int, int>>>::iterator low, vector<pair<int, pair<int, int>>>::iterator high)
{
    for(auto it=low; it!=high+1; it++)
    {
        auto min = it;
        for(auto jt=it+1; jt!=high+1; jt++)
        {
            if (jt->first < min->first)
                min = jt;
        }
        auto tmp = *min;
        *min = *it;
        *it = tmp;
    }
}

void quicksort(vector<pair<int, pair<int, int>>>::iterator low, vector<pair<int, pair<int, int>>>::iterator high)
{
    if (low<high)
        if (high-low+1 <= 6)
            insertsort(low, high);
        else
        {
            auto pivot = low + (high - low) / 2;
            auto low2 = pivot + 1;
            quicksort(low, pivot);
            quicksort(low2, high);

            while(low <= pivot && low2 <= high)
            {
                if(low->first <= low2->first)
                    low++;
                else
                {
                    auto tmp = *low2;
                    auto idx = low2;
                    while(idx != low)
                    {
                        *idx = *(idx-1);
                        idx--;
                    }
                    *low = tmp;

                    low++;
                    low2++;
                    pivot++;
                }
            }

        }
}

bool Kruskal(Graph* graph)
{
	map<int, int>* m = new map<int, int>();
    vector<pair<int, pair<int, int> > > e;
    vector<pair<int, pair<int, int> > > selected;

    vector<VertexSet*> v(graph->getSize());
    for(int i=0; i<v.size(); i++)
        v[i] = new VertexSet(i);

    for(int i=0; i<graph->getSize(); i++)
    {
	    graph->getAdjacentEdges(i, m);
        for(auto it=m->begin(); it!=m->end(); it++)
        {
            e.push_back(make_pair(it->second, make_pair(i, it->first)));
        }
    }

    quicksort(e.begin(), e.end()-1);

    int sum = 0;
    int count = 0;
    for(int i=0; i<e.size(); i++)
    {
        int from = e[i].second.first;
        int to = e[i].second.second;
        int weight = e[i].first;

        if(!v[from]->isSamePart(v[to]))
        {
			sum += weight;
			v[from]->unionParent(v[to]);
            selected.push_back(e[i]);
            ++count;
		}
    }
    if (count != graph->getSize()-1)
        return false;

	cout << "====== Kruskal ========" << endl;
    for(int i=0; i<graph->getSize(); i++)
    {
        vector<pair<int, int>> to_sort;
        for(auto e: selected)
        {
            int from = e.second.first;
            int to = e.second.second;
            int weight = e.first;
            if (from == i)
                to_sort.push_back(make_pair(to, weight));
            else if (to == i)
                to_sort.push_back(make_pair(from, weight));
        }
        sort(to_sort.begin(), to_sort.end());
        cout << "[" << i << "] ";
        for(auto e: to_sort)
            cout << e.first << "(" << e.second << ") ";
        cout << endl;
    }
    cout << "cost: " << sum << endl;
	cout << "=======================" << endl << endl;
    
    return true;
}

bool Dijkstra(Graph* graph, int vertex)
{
    vector<int> dist(graph->getSize(), 1e+9);
    vector<bool> visit(graph->getSize(), false);
    vector<int> parents(graph->getSize(), -1);
	map<int, int>* m = new map<int, int>();

    dist[vertex] = 0;

    for(int i=1; i<graph->getSize(); i++)
    {
        int idx = -1;
        int d = 1e+9;
        for(int i=0; i<graph->getSize(); i++)
        {
            if(!visit[i] && dist[i] < d)
            {
                d = dist[i];
                idx = i;
            }
        }
        if (idx == -1)
            break;
        visit[idx] = true;

	    graph->getAdjacentEdges(idx, m);
        for(auto it=m->begin(); it!=m->end(); it++)
        {
            if (d + it->second < dist[it->first])
            {
                parents[it->first] = idx;
                dist[it->first] = d + it->second;
            }
        }
    }
    for(int i=0; i<graph->getSize(); i++)
    {
        cout << "[" << i << "]" << " ";
        if (dist[i] == 1e+9)
            cout << 'x' << endl;
        else
        {
            vector<int> t;
            int track = i;
            t.push_back(track);
            while(parents[track] != -1)
            {
                track = parents[track];
                t.push_back(track);
            }
            reverse(begin(t), end(t));
            cout << t[0];
            for(int i=1; i<t.size(); i++)
                cout << " -> " << t[i];

            cout << " (" << dist[i] << ")" << endl;
        }
    }

    return true;
}

bool Bellmanford(Graph* graph, int s_vertex, int e_vertex)
{
    vector<int> dist(graph->getSize(), 1e+9);
    vector<int> parent(graph->getSize(), -1);
    vector<pair<int, pair<int, int> > > e;
	map<int, int>* m = new map<int, int>();

    for(int i=0; i<graph->getSize(); i++)
    {
	    graph->getAdjacentEdges(i, m);
        for(auto it=m->begin(); it!=m->end(); it++)
        {
            e.push_back(make_pair(it->second, make_pair(i, it->first)));
        }
    }

    dist[s_vertex] = 0;
    for (int i=1; i<graph->getSize(); i++)
    {
        for (auto edge: e)
        {
            int from = edge.second.first;
            int to = edge.second.second;
            int weight = edge.first;
 
            if (dist[from] == 1e+9) continue;
            if (dist[to] > dist[from] + weight) 
            {
                dist[to] = dist[from] + weight;
                parent[to] = from;
            }
        }
    }
 
    for (auto edge: e)
    {
        int from = edge.second.first;
        int to = edge.second.second;
        int weight = edge.first;
 
        if (dist[from] == 1e+9) continue;
        if (dist[to] > dist[from] + weight)
            return false;
    }
    
    vector<int> res;
    auto vertex = e_vertex;
    res.push_back(vertex);
    while(parent[vertex] != -1)
    {
        vertex = parent[vertex];
        res.push_back(vertex);
    }
    reverse(res.begin(), res.end());
    cout << "==== Bellman-Ford =====" << endl;
    for(auto i: res)
    {
        cout << i;
        if (i != e_vertex)
        cout << " -> ";
    }
    cout << endl << "cost: " << dist[e_vertex] << endl;
    cout << "=======================" << endl << endl;

    return true;
}

bool FLOYD(Graph* graph)
{
    int v = graph->getSize(); 
    vector<vector<int>> dist(v, vector<int>(v, 1e+9));
	map<int, int>* m = new map<int, int>();

    for(int i=0; i<v; i++)
    {
        dist[i][i] = 0;
	    graph->getAdjacentEdges(i, m);
        for(auto it=m->begin(); it!=m->end(); it++)
        {
            dist[i][it->first] = it->second;
        }
    }

    for(int k=0; k<v; k++)
        for(int i=0; i<v; i++)
            for(int j=0; j<v; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];


    cout << "======== FLOYD ========" << endl;
    cout << "    ";
    for(int i=0; i<v; i++)
        cout << "[" << i << "] ";
    cout << endl;

    for(int i=0; i<v; i++)
    {
        cout << "[" << i << "] ";
        for(int j=0; j<v; j++)
        {
            if (dist[i][j]==1e+9)
                cout << ' ' << 'x' << "  ";
            else
                cout << ' ' << dist[i][j] << "  ";
        }
        cout << endl;
    }
    cout << "=======================" << endl;



    return true;
}
