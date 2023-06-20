#include <new>
#include <stdexcept>
#include <string>
#include <iostream>
#include <queue>
#include <time.h>
#include <chrono>

#include "RBTree.h"
#include "bstIterative.h"

using namespace std::chrono;


void main() {
	RBT <string> RBTree;
	BST <string> BSTree;
	int opt;
	high_resolution_clock::time_point init;
	high_resolution_clock::time_point fin;
	duration<double, std::milli> elapsed;
	string filename;
	string word;
	cout << "Input the name of the file you would like to grow a tree from\n";
	cin >> filename;
	cout << "Input the number corresponding to the type of tree you wish to create:\n";
	cout << "1. Red-Black Tree\n2. Binary Search Tree\n";
	cin >> opt;
	switch (opt) {
	case 1:
		init = high_resolution_clock::now();
		RBTree.plant_file(filename);
		fin = high_resolution_clock::now();
		elapsed = fin - init;
		//RBTree.inorder();
		cout << "Time to grow Red-Black Tree is: " << elapsed.count() << " milliseconds\n";
		cout << "Input the word you wish to find in the file\n";
		cin >> word;
		init = high_resolution_clock::now();
		RBTree.find(word);
		fin = high_resolution_clock::now();
		elapsed = fin - init;
		cout << "Search time: " << elapsed.count() << " milliseconds\n";
		break;
	case 2:
		init = high_resolution_clock::now();
		BSTree.plant_file(filename);
		fin = high_resolution_clock::now();
		elapsed = fin - init;
		cout << "Time to grow Binary Search Tree is: " << elapsed.count() << " milliseconds\n";
		cout << "Input the word you wish to find in the file\n";
		cin >> word;
		init = high_resolution_clock::now();
		BSTree.find(word);
		fin = high_resolution_clock::now();
		elapsed = fin - init;
		cout << "Search time: " << elapsed.count() << " milliseconds\n";
		break;
	default:
		cout << "invalid option, exiting";
		exit(1);
	}


}