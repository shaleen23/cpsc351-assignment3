#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream> 
#include <mutex>
#include <unistd.h>
using namespace std;

struct node {
  int key;
  struct node *left, *right;
};

// Create a node
struct node *newNode(int item) {
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  temp->key = item;
  temp->left = temp->right = NULL;
  return temp;
}

// Inorder Traversal
void inorder(struct node *root) {
  if (root != NULL) {
    // Traverse left
    inorder(root->left);

    // Traverse root
    cout << root->key << " -> ";

    // Traverse right
    inorder(root->right);
  }
}

// Insert a node
struct node *insert(struct node *node, int key) {
  // Return a new node if the tree is empty
  if (node == NULL) return newNode(key);

  // Traverse to the right place and insert the node
  if (key < node->key)
    node->left = insert(node->left, key);
  else
    node->right = insert(node->right, key);

  return node;
}

// Find the inorder successor
struct node *minValueNode(struct node *node) {
  struct node *current = node;

  // Find the leftmost leaf
  while (current && current->left != NULL)
    current = current->left;

  return current;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key) {
  // Return if the tree is empty
  if (root == NULL) return root;

  // Find the node to be deleted
  if (key < root->key)
    root->left = deleteNode(root->left, key);
  else if (key > root->key)
    root->right = deleteNode(root->right, key);
  else {
    // If the node is with only one child or no child
    if (root->left == NULL) {
      struct node *temp = root->right;
      free(root);
      return temp;
    } else if (root->right == NULL) {
      struct node *temp = root->left;
      free(root);
      return temp;
    }

    // If the node has two children
    struct node *temp = minValueNode(root->right);

    // Place the inorder successor in position of the node to be deleted
    root->key = temp->key;

    // Delete the inorder successor
    root->right = deleteNode(root->right, temp->key);
  }
  return root;
}



pthread_mutex_t fMutex = PTHREAD_MUTEX_INITIALIZER;
int argv[1];
void* threadFunc(void* data)
{
    if(argv[1] < 0)
	{
		perror("invalid input");
		exit(-1);
	}
	for(int i = 0; i<100; ++i)
	{
		pthread_mutex_lock(&fMutex);
		int random = rand() % 100001;
		 node* root = NULL;
		 root = insert(root, random);
		 inorder(root);
		pthread_mutex_unlock(&fMutex);
	}
}


int main()
{
	int NUMBER_OF_THREADS = argv[1];
    pthread_t tids[NUMBER_OF_THREADS];
    
	for(int i =0; i<NUMBER_OF_THREADS; i++)
	{
		pthread_create(tids, NULL, threadFunc, (void*)(NUMBER_OF_THREADS));
	}

    if(pthread_create(tids, NULL, threadFunc, (void*)(argv[1])) != 0)
	{
		perror("pthread_create");
		exit(-1);
	}
	

	/**
	 * The parent thread will wait here for 
	 * pthread_t tids to terminate
	 */	
	if(pthread_join(tids, NULL) != 0)
	{
		perror("pthread_join");
		exit(-1);
	}

	return 0;
}