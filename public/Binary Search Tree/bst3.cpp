struct TreeNode {
        // An object of type TreeNode represents one node
        // in a binary tree of strings.
   string item;       // The data in this node.
   TreeNode *left;    // Pointer to left subtree.
   TreeNode *right;   // Pointer to right subtree.
   TreeNode(string str = "") {
          // Constructor, defined for convenience. 
          // Make a node containing the specified string.
      item = str;
      left = NULL;
      right = NULL;
   }
};  // end struct TreeNode


void treeInsert(TreeNode *&root, string newItem) {
       // Add the item to the binary sort tree to which the parameter
       // "root" refers.  Note that root is passed by reference since
       // its value can change in the case where the tree is empty.
   if ( root == NULL ) {
          // The tree is empty.  Set root to point to a new node containing
          // the new item.  This becomes the only node in the tree.
      root = new TreeNode( newItem );
      return;
   }
   else if ( newItem < root->item ) {
      treeInsert( root->left, newItem );
   }
   else {
      treeInsert( root->right, newItem );
   }
}  // end treeInsert()


bool treeContains( TreeNode *root, string item ) {
       // Return true if item is one of the items in the binary
       // sort tree to which root points.   Return false if not.
   if ( root == NULL ) {
         // Tree is empty, so it certainly doesn't contain item.
      return false;
   }
   else if ( item == root->item ) {
         // Yes, the item has been found in the root node.
      return true;
   }
   else if ( item < root->item ) {
         // If the item occurs, it must be in the left subtree.
      return treeContains( root->left, item );
   }
   else {
         // If the item occurs, it must be in the right subtree.
      return treeContains( root->right, item );
   }
}  // end treeContains()



bool treeContainsNR( TreeNode *root, string item ) {
         // Return true if item is one of the items in the binary
         // sort tree to which root points.   Return false if not.
     TreeNode *runner;  // For "running" down the tree.
     runner = root;     // Start at the root node.
     while (true) {
        if (runner == NULL) {
              // We've fallen off the tree without finding item.
           return false;  
        }
        else if ( item == runner->item ) {
              // We've found the item.
           return true;
        }
        else if ( item < runner->item ) {
              // If the item occurs, it must be in the left subtree,
              // So, advance the runner down one level to the left.
           runner = runner->left;
        }
        else {
              // If the item occurs, it must be in the right subtree.
              // So, advance the runner down one level to the right.
           runner = runner->right;
        }
     }  // end while
  } // end treeContainsNR();


void treeList(TreeNode *node) {
       // Print the items in the tree in postorder, one item
       // to a line.  Since the tree is a sort tree, the output
       // will be in increasing order.
   if ( node != NULL ) {
       treeList(node->left);           // Print items in left subtree.
       cout << "  " << node->item << endl;  // Print item in the node.
       treeList(node->right);          // Print items in the right subtree.
   }
} // end treeList()


int countNodes(TreeNode *node) {
      // Count the nodes in the binary tree to which node 
      // points.  Return the answer.
   if ( node == NULL ) {
           // Tree is empty, so it contains no nodes.
      return 0;
   }
   else {
          // Add up the root node and the nodes in its two subtrees.
      int leftCount = countNodes( node->left );
      int rightCount = countNodes( node->right );
      return  1 + leftCount + rightCount;  
   }
} // end countNodes()


int main() {

   TreeNode *root;  // Pointer to the root node in a binary tree.  This
                    // tree is used in this program as a binary sort tree.
                    // The tree is not allowed to contain duplicate
                    // items.  When the tree is empty, root is null.

   root = NULL;     // Start with an empty tree.

   cout << "This programs stores strings that you enter in a binary sort\n";
   cout << "tree.  After each items is inserted, the contents of the tree\n";
   cout << "are displayed.  The number of nodes in the tree is also output.\n";
   cout << "    Any string you enter will be converted to lower case.\n";
   cout << "Duplicate entries are ignored.\n";

   while (true) {
           // Get one string from the user, insert it into the tree,
           // and print some information about the tree.  Exit if the
           // user enters an empty string.  Note that all strings are
           // converted to lower case.
       cout << ("\n\nEnter a string to be inserted, or press return to end.\n");
       cout << ("?  ");
       string item;  // The user's input.
       if (cin.peek() == '\n')
          break;
       cin >> item; 
       cin.ignore(10000,'\n');
       if (treeContains(root,item)) {
              // Don't insert a second copy of an item that is already
              // in the tree.
          cout << "\nThat item is already in the tree.\n";
       }
       else {
          treeInsert(root,item);  // Add user's string to the tree.
          cout << "\nThe tree contains " << countNodes(root) << " items.\n";
          cout << "\nContents of tree:\n\n";
          treeList(root);
       }
   }  // end while

   cout << "\n\nExiting program.\n\n";

}  // end main()


