#pragma once
#include <bits\stdc++.h>

using namespace std;

struct Node {
	int data;
	int size = 0;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* parent = nullptr;

	Node() {};
	Node(int value) : data(value) {}
};

namespace BST {

	void insert(Node*& node, int value)
	{
		if (!node) {
			node = new Node(value);
			return;
		}

		if (node->data == value) return;

		++node->size;
		if (value < node->data) {
			insert(node->left, value);
		}
		else {
			insert(node->right, value);
		}
	}

	bool compareSkeleton(Node* node1, Node* node2)
	{
		if (!node1 && node2 || node1 && !node2) return false;
		if (!node1 || !node2) return true;

		return compareSkeleton(node1->left, node2->left) &&
			compareSkeleton(node1->right, node2->right);
	}

	void inorder(Node* node) {
		if (node == nullptr) return;

		inorder(node->left);
		//cout << node->data << ' ' << node->size << endl;
		inorder(node->right);
	}

}

