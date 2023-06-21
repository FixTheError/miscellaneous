#ifndef GRAPH_H
#define GRAPH_H

#include <new>
#include <stdexcept>
#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>

using namespace std;

class graph {
private:
	int time;
	int num_vert;
	int num_edges;
	int Q_size;
	int A_size;
	int source;
	int destination;
	struct Edge;
	struct Vertex;
	typedef Vertex* Vptr;
	Vertex* V;
	vector <int>* adj;
	vector <Vptr> Q;
	typedef Edge* Eptr;
	vector <Edge> E;
	vector <Vptr> S;
	vector <Vptr> V_sorted;
	vector <Eptr> back_edges;
	bool acyclic;
	bool neg_edge;
	struct Vertex {
		Vptr pi;
		int d;
		int f;
		string color;
		int num_adj;
		int rank;
		int num;
		int Q_index;
		vector<Eptr> adj_edge;
	};

	struct Edge {
		Vptr x;
		Vptr y;
		int weight;
	};

	int parent(int i) {
		i = ((i - 1) / 2);
		return i;
	}

	int left(int i) {
		i = (2 * i) + 1;
		return i;
	}

	int right(int i) {
		i = (2 * i) + 2;
		return i;
	}

	void insert(Vptr v) {
		Q.push_back(v);
		Heapify_up(Q_size);
		Q_size = Q_size + 1;
	}

	Vptr Heap_delete(int i) {
		Q_size = Q_size - 1;
		Vptr temp = Q[i];
		Q[i] = Q[Q_size];
		Q.pop_back();
		if (i > 0 && Q[parent(i)]->d > Q[i]->d) {
			Heapify_up(i);
		}
		else if (left(i) < Q_size) {
			Heapify_down(i);
		}
		return temp;
	}

	Vptr Extract_min() {
		Vptr min = Heap_delete(0);
		return min;
	}

	void Heapify_up(int i) {
		int p = parent(i);
		if (i > 0 && Q[i]->d < Q[p]->d) {
			Vptr temp = Q[i];
			Q[i] = Q[p];
			Q[p] = temp;
			Heapify_up(p);
		}
	}

	void Heapify_down(int i) {
		int smallest;
		int l = left(i);
		int r = right(i);
		if (l < Q_size && Q[l]->d < Q[i]->d) {
			smallest = l;
		}
		else {
			smallest = i;
		}
		if (r < Q_size && Q[r]->d < Q[smallest]->d) {
			smallest = r;
		}
		if (smallest != i) {
			Vptr temp = Q[i];
			Q[i] = Q[smallest];
			Q[smallest] = temp;
			Heapify_down(smallest);
		}
	}

	void Decrease_key(Vptr r, int w) {
		if (r->d < w) {
			cout << "Error, new key is larger";
			exit(EXIT_FAILURE);
		}
		r->d = w;
		if (r->Q_index > 0 && Q[parent(r->Q_index)]->d > Q[r->Q_index]->d) {
			Heapify_up(r->Q_index);
		}
		else if (left(r->Q_index) < Q_size) {
			Heapify_down(r->Q_index);
		}
	}

	void Build_Min_Heap() {
		for (int i = (Q_size / 2) - 1; i >= 0; i--) {
			Heapify_down(i);
		}
	}

	void Init_Single_Source(Vptr s) {
		for (int i = 0; i < num_vert; i++) {
			V[i].d = std::numeric_limits<int>::max();
			V[i].pi = nullptr;
		}
		s->d = 0;
	}

	void Relax(Vptr u, Vptr v, int w) {
		if (v->d > u->d + w && u->d != std::numeric_limits<int>::max()) {
			v->d = u->d + w;
			v->pi = u;
		}
		
	}

	void DFS() {
		for (int i = 0; i < num_vert; i++) {
			V[i].color = "white";
			V[i].pi = NULL;
		}
		time = 0;
		for (int i = 0; i < num_vert; i++) {
			//Vertex u = V[i];
			if (V[i].color == "white") {
				DFS_visit(&V[i], i);
			}
		}
	}
	void DFS_visit(Vptr u, int& i) {
		time = time + 1;
		u->d = time;
		u->color = "gray";
		for (int j = 0; j < u->num_adj; j = j + 2) {
			Vptr v = &V[adj[i][j]];
			if (v->color == "white") {
				DFS_visit(v, adj[i][j]);
			}
			if (v->color == "gray") {
				acyclic = false;
			}
		}
		u->color = "black";
		time = time + 1;
		u->f = time;
		V_sorted.push_back(u);
	}

	void Topo_sort() {
		DFS();
	}

public:
	graph(string& filename) {
		acyclic = true;
		neg_edge = false;
		num_vert = 0;
		ifstream file;
		file.open(filename);
		if (file.fail()) {
			cout << "Failed to open file\n";
			exit(1);
		}
		string line_buf;
		while (getline(file, line_buf)) {
			num_vert = num_vert + 1;
		}
		file.clear();
		file.seekg(0, file.beg);
		V = new Vertex[num_vert];
		adj = new vector <int> [num_vert];
		int vnum = 0;
		while (getline(file, line_buf)) {
			V[vnum].num_adj = 0;
			V[vnum].num = vnum + 1;
			V[vnum].Q_index = -1;
			stringstream tmp_stream(line_buf);
			string tmp;
			getline(tmp_stream, tmp, ' ');
			int adj_ind = 0;
			while (getline(tmp_stream, tmp, ' ')) {
				int neighbor = atoi(tmp.c_str());
				adj[vnum].push_back(neighbor - 1);
				getline(tmp_stream, tmp, ' ');
				int weight = atoi(tmp.c_str());
				if (weight < 0) {
					neg_edge = true;
				}
				Edge e;
				e.weight = weight;
				e.x = &V[vnum];
				e.y = &V[adj[vnum][adj_ind]];
				E.push_back(e);
				V[vnum].adj_edge.push_back(&E.back());
				num_edges = num_edges + 1;
				adj[vnum].push_back(weight);
				V[vnum].num_adj = V[vnum].num_adj + 1;
				adj_ind = adj_ind + 2;
			}
			vnum = vnum + 1;
		}

	}

	void Dijkstra(Vptr s) {
		Init_Single_Source(s);
		S.clear();
		Q.clear();
		Q_size = 0;
		Vptr u;
		for (int i = 0; i < num_vert; i++) {
			Q.push_back(&V[i]);
			Q_size = Q_size + 1;
		}
		while (!Q.empty()) {
			Build_Min_Heap();
			u = Extract_min();
			S.push_back(u);
			for (int i = 0; i < adj[u->num - 1].size(); i = i + 2) {
				Vptr v = &V[adj[u->num - 1][i]];
				int w = adj[u->num - 1][i + 1];
				Relax(u, v, w);
			}
		}
	}

	bool Bellman_Ford(Vptr s) {
		Init_Single_Source(s);
		for (int i = 0; i < num_vert - 1; i++) {
			for (int j = 0; j < num_edges; j++) {
				Relax(E[j].x, E[j].y, E[j].weight);
			}
		}
		for (int i = 0; i < num_edges; i++) {
			if (E[i].y->d > E[i].x->d + E[i].weight) {
				return false;
			}
		}
		return true;
	}
	
	void DAG_Shortest_Path(Vptr s) {
		V_sorted.clear();
		Topo_sort();
		Init_Single_Source(s);
		while (!V_sorted.empty()) {
			Vptr u = V_sorted.back();
			for (int i = 0; i < adj[u->num - 1].size(); i = i + 2) {
				Vptr v = &V[adj[u->num - 1][i]];
				int w = adj[u->num - 1][i + 1];
				if (v->num_adj == 0) {
					Relax(v, u, w);
				}
				Relax(u, v, w);
			}
			V_sorted.pop_back();
		}
	}

	void Find_Path() {
		vector <Vptr> path;
		while (destination != 0) {
			int path_size = 0;
			int w_total = 0;
			cout << "Shortest path:\n";
			Vptr temp = &V[destination - 1];
			while (temp->num != source) {
				w_total = w_total + temp->d;
				path.push_back(temp);
				path_size = path_size + 1;
				temp = temp->pi;
			}
			w_total = w_total + temp->d;
			cout << source << ", ";
			for (int i = path_size; i > 0; i--) {
				cout << path[i - 1]->num << ", ";
			}
			cout << "\n";
			cout << "path weight:\n";
			cout << /*w_total*/ V[destination - 1].d << "\n";
			cout << "Enter the a node 1 - " << num_vert << " to be used as the destination node\n";
			cout << "To quit, enter 0\n";
			cin >> destination;
			path.clear();
		}
	}

	void graph_main() {
		cout << "Enter the a node 1 - " << num_vert << " to be used as the source node\n";
		cin >> source;
		cout << "Enter the a node 1 - " << num_vert << " to be used as the destination node\n";
		cin >> destination;
		Vptr s = &V[source - 1];
		Topo_sort();
		if (acyclic) {
			cout << "This graph is acyclic, running DAG SP\n";
			DAG_Shortest_Path(s);
		}
		else if (neg_edge) {
			cout << "This graph is not a DAG and has negative edges, running Bellman Ford\n";
			if (!Bellman_Ford(s)) {
				cout << "Negative weight cycle detected, exiting\n";
				return;
			}
		}
		else {
			cout << "This graph is not a DAG and has only positive edges, running Dijkstra\n";
			Dijkstra(s);
		}
		Find_Path();
	}

};

#endif
