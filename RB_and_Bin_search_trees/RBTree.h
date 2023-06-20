#ifndef RBTREE_H
#define RBTREE_H

#include <new>
#include <stdexcept>
#include <string>
#include <iostream>
#include <queue>
#include <fstream>

using namespace std;

template <typename K>
class RBT {
private:
	class Node;
	typedef Node* Nodeptr;

	class Node {
	public:
		Nodeptr parent;
		Nodeptr left;
		K key;
		Nodeptr right;
		string color;

		Node(Nodeptr const parent, Nodeptr const left, const K& key, Nodeptr const right, const string& color)
			: parent(parent), left(left), key(key), right(right), color(color)
		{}
	};


	Nodeptr root;
	Nodeptr nil;

	Nodeptr createNode(Nodeptr const parent, Nodeptr const left, const K& key, Nodeptr const right, const string color) throw (bad_alloc) {
		return new Node(parent, left, key, right, color);
	}// end createNode()

	void inorder(Nodeptr node) const {
		if (node != nil) {
			inorder(node->left);
			cout << node->key << "\n";
			inorder(node->right);
		}
	}// end inorder()

	void left_rotate(Nodeptr x) {
		Nodeptr y = x->right;
		x->right = y->left;
		if (y->left != nil) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nil) {
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void right_rotate(Nodeptr x) {
		Nodeptr y = x->left;
		x->left = y->right;
		if (y->right != nil) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nil) {
			root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	void insert_fixup(Nodeptr z) {
		while (z->parent->color == "red") {
			if (z->parent == z->parent->parent->left) {
				Nodeptr y = z->parent->parent->right;
				if (y->color == "red") {
					z->parent->color = "black";
					y->color = "black";
					z->parent->parent->color = "red";
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->right) {
						z = z->parent;
						left_rotate(z);
					}
					z->parent->color = "black";
					z->parent->parent->color = "red";
					right_rotate(z->parent->parent);
				}
			}
			else {
				Nodeptr y = z->parent->parent->left;
				if (y->color == "red") {
					z->parent->color = "black";
					y->color = "black";
					z->parent->parent->color = "red";
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->left) {
						z = z->parent;
						right_rotate(z);
					}
					z->parent->color = "black";
					z->parent->parent->color = "red";
					left_rotate(z->parent->parent);
				}
			}
		}
		root->color = "black";
	}

public:

	RBT(){
		K n;
		root = createNode(nullptr, nullptr, n, nullptr, "black");
		root->right = createNode(root, root, n, root, "black");
		nil = root->right;
		root->left = nil;
		root->parent = nil;
	}

	void plant_file(string& filename) {
		ifstream file;
		file.open(filename);
		if (file.fail()) {
			cout << "Failed to open file\n";
			exit(1);
		}
		K word;
		while (file >> word) {
			//cout << word << "\n";
			insert(word);
		}
	}

	const K& find(const K& key) const throw (logic_error) {
		Nodeptr iter = root;

		while (iter != nil) {
			if (key == iter->key) { return iter->key; }
			else if (key < iter->key) { iter = iter->left; }
			else { iter = iter->right; }
		}

		// Did not find key
		cout << key << " not found, exiting.\n";
		exit(1);
	}// end find()
	
	void insert(const K& key) {
		Nodeptr z = createNode(nullptr, nullptr, key, nullptr, "red");
		Nodeptr y = nil;
		Nodeptr x = root;
		while (x != nil) {
			y = x;
			if (z->key < x->key) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}
		z->parent = y;
		if (y == nil) {
			root = z;
		}
		else if (z->key < y->key) {
			y->left = z;
		}
		else {
			y->right = z;
		}
		z->left = nil;
		z->right = nil;
		z->color = "red";
		insert_fixup(z);
	}
	
	void inorder() const { inorder(root); }// end inorder()
};

#endif