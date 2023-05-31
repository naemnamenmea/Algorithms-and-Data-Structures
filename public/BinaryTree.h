#pragma once
#include <bits\stdc++.h>

using namespace std;

struct Node {
	int data;
	int size = 0;
	unique_ptr<Node> left = nullptr;
	unique_ptr<Node> right = nullptr;
	unique_ptr<Node> parent = nullptr;

	Node() {};
	Node(int value) : data(value) {}
};

namespace BST {

	void insert(unique_ptr<Node>& node, int value)
	{
		if (!node) {
			node = unique_ptr<Node>(new Node(value));
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

	bool compareSkeleton(unique_ptr<Node>& node1, unique_ptr<Node>& node2)
	{
		if (!node1 && node2 || node1 && !node2) return false;
		if (!node1 || !node2) return true;

		return compareSkeleton(node1->left, node2->left) &&
			compareSkeleton(node1->right, node2->right);
	}

	void inorder(unique_ptr<Node>& node) {
		if (node == nullptr) return;

		inorder(node->left);
		//cout << node->data << ' ' << node->size << endl;
		inorder(node->right);
	}

}

