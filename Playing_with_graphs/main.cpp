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
		cout << "1: " << "graphin_Fig1.txt\n";
		cout << "2: " << "graphin_Fig2.txt\n";
		cout << "3: " << "graphin_Fig4.txt\n";
		cout << "4: " << "graphin_Fig5.txt\n";
		cout << "5: " << "graphin_w_ud.txt\n";
		cout << "6: " << "graphin_w_ud2.txt\n";
		cin >> opt;
	} while (opt < 1 || opt > 6);
	switch (opt) {
	case 1:
		input = "graphin_Fig1.txt";
		break;
	case 2:
		input = "graphin_Fig2.txt";
		break;
	case 3:
		input = "graphin_Fig4.txt";
		break;
	case 4:
		input = "graphin_Fig5.txt";
		break;
	case 5:
		input = "graphin_w_ud.txt";
		break;
	case 6:
		input = "graphin_w_ud2.txt";
		break;
	}
	graph G(input);
	G.graph_main();
}