struct Node
{
	char data;
	Node *left;
	Node *right;
	Node *parent;
};

void traverse(Node* node)
{
	if (node == nullptr) return;

	traverse(node->left);
	traverse(node->right);
}

Node* lookUp(Node* node, char key)
{
	if (node == nullptr) return nullptr;

	if (node->data == key) return node;
	else {
		if (node->data > key)
			return lookUp(node->right, key);
		else
			return lookUp(node->left, key);
	}
}

Node* leftMost(Node* node)
{
	if (node == nullptr) return nullptr;

	while (node->left != nullptr) {
		node = node->left;
	}
	return node;
}

Node* newTreeNode(char data)
{
	Node *node = new Node;
	node->data = data;
	node->left = nullptr;
	node->right = nullptr;
	node->parent = nullptr;

	return node;
}

Node* insertTreeNode(Node* node, char data)
{
	Node* p;
	Node* retNode;

	if (node == nullptr) {
		retNode = newTreeNode(data);

	}

	return new Node;
}