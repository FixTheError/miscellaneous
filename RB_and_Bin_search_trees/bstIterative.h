#ifndef BST_ITERATIVE_H
#define BST_ITERATIVE_H

#include <new>
#include <stdexcept>
#include <string>
#include <iostream>
#include <queue>
#include <time.h>
#include <fstream>
using namespace std;


template <typename K>
class BST {
private:
	class Node;
	typedef Node* Nodeptr;

	class Node {
	public:
		Nodeptr parent;
		Nodeptr left;
		K key;
		Nodeptr right;

		Node(Nodeptr const parent, Nodeptr const left, const K& key, Nodeptr const right)
			: parent(parent), left(left), key(key), right(right)
		{}
	};


	Nodeptr root;



	Nodeptr createNode(Nodeptr const parent, Nodeptr const left, const K& key, Nodeptr const right) throw (bad_alloc) {
		return new Node(parent, left, key, right);
	}// end createNode()


	void removeNode(Nodeptr del) {
		// What kind of node are we removing?
		if (isLeaf(del)) {
			Nodeptr parent = del->parent;

			if (parent->right == del) { parent->right = nullptr; }
			else { parent->left = nullptr; }
			delete del;

		}
		else if (hasLeftChildOnly(del)) {
			Nodeptr parent = del->parent;

			if (parent->left == del) { parent->left = del->left; }
			else { parent->right = del->left; }
			delete del;

		}
		else if (hasRightChildOnly(del)) {
			Nodeptr parent = del->parent;

			if (parent->left == del) { parent->left = del->right; }
			else { parent->right = del->right; }
			delete del;

		}
		else {
			// Has two children.
			Nodeptr successor = del->right;
			while (successor->left != nullptr) {
				successor = successor->left;
			}
			// Copy successor's key to del's key.
			del->key = successor->key;

			// Remove successor now.
			if (del->right == successor) {
				del->right = successor->right;
			}
			else {
				Nodeptr parent = successor->parent;
				parent->left = successor->right;
			}

			// Release the node.
			delete successor;
		}
	}// end removeNode()

	bool hasTwoChildren(Nodeptr const node) const { return (node->left && node->right); }// end hasTwoChildren()
	bool hasLeftChildOnly(Nodeptr const node) const { return (node->left && !node->right); }// end hasLeftChildOnly()
	bool hasRightChildOnly(Nodeptr const node) const { return (node->right && !node->left); }// end hasRightChildOnly()
	bool isLeaf(Nodeptr const node) const { return (!node->left && !node->right); }// end isLeaf()

	void inorder(Nodeptr node) const {
		if (node != nullptr) {
			inorder(node->left);
			cout << node->key << " ";
			inorder(node->right);
		}
	}// end inorder()

public:
	BST() : root(nullptr) { }// end BST()

	void plant_file(string& filename) {
		ifstream file;
		file.open(filename);
		if (file.fail()) {
			cout << "Failed to open file\n";
			exit(1);
		}
		K word;
		while (file >> word) {
			//cout << word;
			insert(word);
		}
	}

	const K& find(const K& key) const throw (logic_error) {
		Nodeptr iter = root;

		while (iter != nullptr) {
			if (key == iter->key) { return iter->key; }
			else if (key < iter->key) { iter = iter->left; }
			else { iter = iter->right; }
		}

		// Did not find key
		cout << key << " not found, exiting.\n";
		exit(1);
	}// end find()

	void insert(const K& key) {
		if (root == nullptr) {
			root = createNode(nullptr, nullptr, key, nullptr);

		}
		else {
			Nodeptr parent = root;
			while (parent != nullptr) {
				if (key < parent->key) {
					if (parent->left == nullptr) {
						parent->left = createNode(parent, nullptr, key, nullptr);
						parent = nullptr;

					}
					else {
						parent = parent->left;
					}

				}
				else {
					if (parent->right == nullptr) {
						parent->right = createNode(parent, nullptr, key, nullptr);
						parent = nullptr;
					}
					else {
						parent = parent->right;
					}
				}
			}
		}
	}// end insert()


	bool remove(const K& key) throw (logic_error) {
		Nodeptr del = root;

		/* The node to be deleted must be found first. */
		while (del != nullptr) {
			if (key == del->key) {
				// Found it, so now remove it.
				removeNode(del);
				return true;

			}
			else if (key < del->key) {
				del = del->left;
			}
			else {
				del = del->right;
			}
		}

		// Did not find node.
		throw logic_error("Did not remove " + to_string(key) + ". It was not found.");
	}// end remove()


	void inorder() const { inorder(root); }// end inorder()
	bool isEmpty() const { return root == nullptr; }// end isEmpty()
};

#endif
