 int countNodes( TreeNode *root ) {
    if ( root == NULL )
       return 0;  
    else {
       int count = 1;
       count += countNodes(root->left); 
       count += countNodes(root->right); 
                                        
       return count;
    }
 }