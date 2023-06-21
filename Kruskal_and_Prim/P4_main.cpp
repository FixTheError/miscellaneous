#include <new>
#include <stdexcept>
#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <sstream>
#include "graph.h"

using namespace std;

void main() {
	string input;
	int opt;
	do {
		cout << "Enter the number corresponding to the desired input file from the following list.\n";
		cout << "1: " << "graphin_w_ud.txt\n";
		cout << "2: " << "graphin_w_ud2.txt\n";
		cin >> opt;
	} while (opt < 1 || opt > 2);
	switch (opt) {
	case 1:
		input = "graphin_w_ud.txt";
		break;
	case 2:
		input = "graphin_w_ud2.txt";
		break;
	}
	string kruskalout = "kruskalout.txt";
	string primout = "primout.txt";
	graph G(input);
	G.Kruskal();
	G.out(kruskalout);
	G.out(primout);
}