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
	struct Edge;
	struct Vertex;
	typedef Vertex* Vptr;
	struct Node;
	typedef Node* Nodeptr;
	Nodeptr sort_head;
	Nodeptr sort_temp;
	Vertex* V;
	vector <int>* adj;
	vector <Vptr> Q;
	typedef Edge* Eptr;
	vector <Edge> E;
	vector <Eptr> A;
	struct Vertex {
		Vptr p;
		int num_adj;
		int rank;
		int key;
		int num;
		int Q_index;
		vector<Eptr> adj_edge;
	};

	struct Edge {
		Vptr x;
		Vptr y;
		int weight;
	};

	void Make_Set(Vptr x) {	//Union by height heuristic
		x->p = x;
		x->rank = 0;
	}

	void Union(Vptr x, Vptr y) {
		Link(Find_Set(x), Find_Set(y));
	}

	void Link(Vptr x, Vptr y){
		if (x->rank > y->rank) {
			y->p = x;
		}
		else {
			x->p = y;
			if (x->rank == y->rank) {
				y->rank = y->rank + 1;
			}
		}
	}

	Vptr Find_Set(Vptr x) {
		if(x != x->p){
			x->p = Find_Set(x->p);
		}
		return x->p;
	}

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

	void insert(Vptr v){
		Q.push_back(v);
		Heapify_up(Q_size);
		Q_size = Q_size + 1;
	}

	Vptr Heap_delete(int i){
		Q_size = Q_size - 1;
		Vptr temp = Q[i];
		Q[i] = Q[Q_size];
		Q.pop_back();
		if (i > 0 && Q[parent(i)]->key > Q[i]->key) {
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
		if (i > 0 && Q[i]->key < Q[p]->key) {
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
		if (l < Q_size && Q[l]->key < Q[i]->key) {
			smallest = l;
		}
		else {
			smallest = i;
		}
		if (r < Q_size && Q[r]->key < Q[smallest]->key) {
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
		if (r->key < w) {
			cout << "Error, new key is larger";
			exit(EXIT_FAILURE);
		}
		r->key = w;
		if (r->Q_index > 0 && Q[parent(r->Q_index)]->key > Q[r->Q_index]->key) {
			Heapify_up(r->Q_index);
		}
		else if (left(r->Q_index) < Q_size) {
			Heapify_down(r->Q_index);
		}
	}

	void Merge_Sort(int p, int r) {
		if (p < r) {
			int q = ((p + r) / 2);
			Merge_Sort(p, q);
			Merge_Sort(q + 1, r);
			Merge(E, p, q, r);
		}
	}

	void Merge(vector <Edge> e, int p, int q, int r) {
		int n1 = q - p + 1;
		int n2 = r - q;
		vector <Edge> L;
		vector <Edge> R;
		for (int i = 0; i < n1; i++) {
			L.push_back(E[p + i]);
		}
		for (int i = 0; i < n2; i++) {
			R.push_back(E[q + i + 1]);
		}
		Edge dummy;
		dummy.weight = INFINITY;
		L.push_back(dummy);
		R.push_back(dummy);
		int i = 0;
		int j = 0;
		for (int k = p; k <= r; k++) {
			if (L[i].weight <= R[j].weight) {
				e[k] = L[i];
				if (i < n1) {
					i = i + 1;
				}
			}
			else {
				e[k] = R[j];
				if (j < n2) {
					j = j + 1;
				}
			}
		}
	}

public:
	graph(string& filename) {
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
				//cout << adj[vnum][adj_ind] << "\n";
				getline(tmp_stream, tmp, ' ');
				int weight = atoi(tmp.c_str());
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

	void Kruskal() {
		A.clear();
		A_size = 0;
		for (int i = 0; i < num_vert; i++) {
			Make_Set(&V[i]);
		}
		Merge_Sort(0, num_edges - 1);
		for (int i = 0; i < num_edges; i++) {
			if (Find_Set(E[i].x) != Find_Set(E[i].y)) {
				A.push_back(&E[i]);
				A_size = A_size + 1;
				Union(E[i].x, E[i].y);
			}
		}
	}

	void Prim() {
		A.clear();
		A_size = A_size + 1;
		Q.clear();
		Q_size = 0;
		Vptr u;
		Vptr r;
		for (int i = 0; i < num_vert; i++) {
			u = &V[i];
			if (i == 0) {
				r = u;
			}
			u->key = INFINITY;
			u->p = nullptr;
			u->Q_index = i;
			insert(u);
		}
		Decrease_key(r, 0);
		while (!Q.empty()) {
			u = Extract_min();
			u->Q_index = -1;
			for (int i = 0; i < u->num_adj; i = i + 2) {
				Vptr v = &V[adj[u->num - 1][i]];
				int w = adj[u->num - 1][i + (int) 1];
				if (v->Q_index != -1 && w < v->key) {
					v->p = u;
					Decrease_key(v, w);
					A.push_back(u->adj_edge[i / 2]);
					A_size = A_size + 1;
				}
			}
		}
	}
	void out(string& filename) {
		ofstream file;
		file.open(filename);
		if (file.fail()) {
			cout << "Failed to open file\n";
			exit(1);
		}
		for (int i = 0; i < A_size; i++) {
			file << A[i]->x->num << " " << A[i]->y->num << "\n";
		}
	}
};

#endif
