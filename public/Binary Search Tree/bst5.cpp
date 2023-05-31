TreeNode *root = this;
TreeNode *parent = root;

while(root) {
    parent = root;
    root = (root->value > x.value) ? root->left : root->right;
}
if(parent->value > x->value)
    parent->left = x;
else
    parent->right = x;




queue<TreeNode*> nodes;
nodes.push(this);
while(1)
{
    if(!nodes.front()->left)
    {
        nodes.front()->left = x;
        return;
    }
    else if (!nodes.front()->right)
    {
        nodes.front()->right = x;
        return;
    }
    nodes.push(nodes.front()->left);
    nodes.push(nodes.front()->right);
    nodes.pop();
}