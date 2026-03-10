/*
 *  RAVL (augmented with Rank AVL) tree implementation.
 *  Author (starter code): Anya Tafliovich.
 *  Based on materials developed by F. Estrada.
 */

#include "RAVL_tree.h"

/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.  Note: this
 * should be an O(1) operation.
 */
int height(RAVL_Node* node){
  if(node==NULL){
    return 0;
  }
  return node->height;
}

/* Returns the size (number of nodes) of the tree rooted at node 'node'.
 * Returns 0 if 'node' is NULL.  Note: this should be an O(1) operation.
 */
int size(RAVL_Node* node){
  if (node==NULL){
    return 0;
  }
  return node->size;
  
}

/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void updateHeight(RAVL_Node* node){
  if(node==NULL){return;}

  int leftHeight = height(node->left);
  int rightHeight = height(node->right);
  if(leftHeight>=rightHeight){
    node->height = leftHeight+1;
  }else{
    node->height = rightHeight+1;
  }
  return;
}

/* Updates the size of the tree rooted at node 'node' based on the sizes
 * of its children. Note: this should be an O(1) operation.
 */
void updateSize(RAVL_Node* node){
  if(node==NULL){return;}

  int leftSize = size(node->left);
  int rightSize = size(node->right);
  node->size = 1+leftSize+rightSize;
  return;
}

/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 if node is NULL.  Note: this should be
 * an O(1) operation.
 */
int balanceFactor(RAVL_Node* node){
  if (node==NULL){
    return 0;
  }
  int bf = height(node->left) - height(node->right);
  return bf;
}

/* Returns the result of performing the corresponding rotation in the RAVL
 * tree rooted at 'node'.
 */
// single rotations: right/clockwise
RAVL_Node* rightRotation(RAVL_Node* node){
  if(node==NULL || node->left==NULL){
    return node;
  }
  RAVL_Node* newNode = node->left;
  RAVL_Node* originalRightChild = newNode->right;
  newNode->right = node;
  node->left=originalRightChild;

  updateHeight(node); 
  updateSize(node);
  updateHeight(newNode);
  updateSize(newNode);
  return newNode;
}

// single rotations: left/counter-clockwise
RAVL_Node* leftRotation(RAVL_Node* node){
  if(node==NULL || node->right==NULL){
    return node;
  }
  RAVL_Node* newNode = node->right;
  RAVL_Node* originalLeftChild = newNode->left;
  newNode->left = node;
  node->right = originalLeftChild;

  updateHeight(node); 
  updateSize(node);
  updateHeight(newNode);
  updateSize(newNode);
  return newNode;
}

// double rotation: right/clockwise then left/counter-clockwise
RAVL_Node* rightLeftRotation(RAVL_Node* node){
  if(node==NULL || node->right == NULL){
    return node;
  }
  //right
  node->right = rightRotation(node->right);
  //left
  return leftRotation(node);
}

// double rotation: left/counter-clockwise then right/clockwise
RAVL_Node* leftRightRotation(RAVL_Node* node){
  if(node==NULL || node->left==NULL){
    return node;
  }
  //left
  node->left = leftRotation(node->left);
  //right
  return rightRotation(node);
}

/* Returns the successor node of 'node'. */
RAVL_Node* successor(RAVL_Node* node){
  if(node==NULL || node->right==NULL){
    return NULL;
  }
  RAVL_Node* s = node->right;
  while(s->left != NULL){
    s = s->left;
  }
  return s;
}

/* Creates and returns an RAVL tree node with key 'key', value 'value', height
 * and size of 1, and left and right subtrees NULL.
 */
RAVL_Node* createNode(int key, void* value){
  RAVL_Node* node = calloc(1, sizeof(RAVL_Node));
  if(node==NULL){return NULL;}
  node->key = key;
  node->value = value;
  node->height = 1;
  node->size = 1;
  node->left = NULL;
  node->right= NULL;
  return node;
}

//Rebalance AVL tree
RAVL_Node* rebalance(RAVL_Node* node){
  if(node==NULL){return node;}
  int bf = balanceFactor(node);
  //left heavy: right: bf>1
  if(bf>1){
    RAVL_Node* leftNode = node->left;
    if(balanceFactor(leftNode)>=0){
      return rightRotation(node);
    }
    return leftRightRotation(node);
  }
  //right heavy: left: <-1
  if(bf<-1){
    RAVL_Node* rightNode = node->right;
    if(balanceFactor(rightNode)<=0){
      return leftRotation(node);
    }
    return rightLeftRotation(node);
  }
  //already!
  return node;

}

/*************************************************************************
 ** Provided functions
 *************************************************************************/

void printTreeInorder_(RAVL_Node* node, int offset) {
  if (node == NULL) return;
  printTreeInorder_(node->right, offset + 1);
  printf("%*s %d [%d / %d]\n", offset, "", node->key, node->height, node->size);
  printTreeInorder_(node->left, offset + 1);
}

void printTreeInorder(RAVL_Node* node) { printTreeInorder_(node, 0); }

void deleteTree(RAVL_Node* node) {
  if (node == NULL) return;
  deleteTree(node->left);
  deleteTree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

RAVL_Node* search(RAVL_Node* node, int key) {
  if(node==NULL){
    return NULL;
  }
  if(node->key==key){
    return node;
  }
  if(node->key<key){
    return search(node->right,key);
  }
  else{
    return search(node->left,key);
  }
}

RAVL_Node* insert(RAVL_Node* node, int key, void* value) {
  if(node==NULL){
    return createNode(key,value);
  }
  if((node->key)==key){
    node->value = value;
    return node; 
  }
  if(key<(node->key)){
    node->left=insert(node->left,key,value);
  }else{
    node->right=insert(node->right,key,value);
  }

  updateHeight(node);
  updateSize(node);
  return rebalance(node);
}

RAVL_Node* delete(RAVL_Node* node, int key) {
  RAVL_Node *temp;
  if(node==NULL){
    return NULL;
  }
  if(node->key == key){
    //case1: no children
    if(node->left==NULL && node->right==NULL){
      free(node);
      return NULL;
    }
    //case2.1 only have left child
    else if(node->right==NULL){
      temp = node->left;
      free(node);
      return temp;
    }
    //case2.2 only have right child
    else if(node->left==NULL){
      temp = node->right;
      free(node);
      return temp;
    }
    //case3 have two
    else{
      temp = successor(node);
      node->key = temp->key;
      node->value = temp->value;
      node->right = delete(node->right,temp->key);
    }
  }
  else if(key<(node->key)){
    node->left = delete(node->left,key);
  }
  else{node->right = delete(node->right,key);}

  updateHeight(node);
  updateSize(node);
  return rebalance(node);
}

int rank(RAVL_Node* node, int key) { 
  if(node==NULL|| search(node,key)==NULL){
    return NOTIN;
  }
  if(node->key ==key){
    return size(node->left)+1;
  }
  if(node->key < key){
    return size(node->left)+1+rank(node->right,key);
  }
  return rank(node->left,key);
  }

RAVL_Node* findRank(RAVL_Node* node, int rank) { 
  if(node==NULL || rank<= 0){
    return NULL;
  }
  int currRank = size(node->left)+1;
  if(currRank==rank){
    return node;
  }
  if(currRank<rank){
    return findRank(node->right, rank-currRank);
  }
  return findRank(node->left, rank);
}
