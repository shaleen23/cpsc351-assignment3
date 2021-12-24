#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

using namespace std;

// lines 14 through 111 is for the binary search tree
struct node
{
  int key;
  struct node *left, *right;
};

// Create a node
struct node *newNode(int item)
{
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  temp->key = item;
  temp->left = temp->right = NULL;
  return temp;
}

// Inorder Traversal
void inorder(struct node *root)
{
  if (root != NULL)
  {
    // Traverse left
    inorder(root->left);

    // Traverse root
    cout << root->key << endl;

    // Traverse right
    inorder(root->right);
  }
}

// Insert a node
struct node *insert(struct node *node, int key)
{
  // Return a new node if the tree is empty
  if (node == NULL)
    return newNode(key);

  // Traverse to the right place and insert the node
  if (key < node->key)
    node->left = insert(node->left, key);
  else
    node->right = insert(node->right, key);

  return node;
}

// Find the inorder successor
struct node *minValueNode(struct node *node)
{
  struct node *current = node;

  // Find the leftmost leaf
  while (current && current->left != NULL)
    current = current->left;

  return current;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key)
{
  // Return if the tree is empty
  if (root == NULL)
    return root;

  // Find the node to be deleted
  if (key < root->key)
    root->left = deleteNode(root->left, key);
  else if (key > root->key)
    root->right = deleteNode(root->right, key);
  else
  {
    // If the node is with only one child or no child
    if (root->left == NULL)
    {
      struct node *temp = root->right;
      free(root);
      return temp;
    }
    else if (root->right == NULL)
    {
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


node *root = NULL;  // set the tree root to NULL
pthread_mutex_t fMutex = PTHREAD_MUTEX_INITIALIZER; // create a mutex

int argv[1]; // create argv to determine number of threads to create

void *threadFunc(void *data)
{
  
if(pthread_mutex_lock(&fMutex) < 0) //lock the mutex to prevent deadlocking and race conditions from occuring in the critical sections
	{
		perror("pthread_mutex_lock");
		exit(1);
	}
	
	

  for (int i = 0; i < 100; ++i) // iterate through 100 times. 1 thread = 100 random numbers generated between 0-100000
  {
     
      int random = rand() % 100001;  // generate 100 random numbers in range of 0 -- 100,000
      root = insert(root, random); // generate 100 random numbers in range of 0 -- 100,000
    
     
  }
  if(pthread_mutex_unlock(&fMutex) < 0) // Unlock the mutex to allow other threads to enter the critical section
	{
		perror("pthread_mutex_unlock");
		exit(1);
	}
  return root; //return the root values 
}

int main(int argc, char **argv)
{
  char *p; // create a char in order to convert argv[1] into an int    
  long conv = strtol(argv[1], &p, 10); // conv converts argv[1] into an int
  int NUMBER_OF_THREADS = conv; // create a variable to iterate through the thread and set it equal to conv aka the number of threads you want to create
  pthread_t tids[NUMBER_OF_THREADS]; // create the thread

  for (int i = 0; i < conv; ++i) // iterate through the number of threads requested
  {
    
    if (pthread_create(&(tids[i]), NULL, &threadFunc, NULL) != 0) // check if the pthread isn't equal to 0, then create the thread if it isn't
    {
      perror("pthread_create");
      exit(-1);
    }
  }

  for (int j = 0; j < conv; j++)
  { // iterate through the threads being joined
    if (pthread_join(tids[j], NULL) != 0)
    {
      perror("pthread_join");
      exit(-1);
    }
  }


  inorder(root); //set all the numbers in an inorder fashion once completed
   return 0;
} 
