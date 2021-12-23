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

// lines 15 through 111 is for the binary search tree
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

/*
1.  The program shall be invoked as ./threads <number> where <number> is the number of threads.  For example, ./threads 10 will create 10 threads.

2. The program will then create the specified number of threads and each thread will then generate 100 random numbers in range of 0 -- 100000 and insert them into the same shared binary search tree that is shared between all threads.

3.  All threads should be allowed to insert the numbers at the same time while ensuring proper synchronization using pthread mutexes (i.e., no two threads will be modifying the tree at the same time).

4. While the created threads are busy inserting numbers, the main thread waits for the other threads to terminate.  Once all threads terminate the main thread will print all numbers in the binary tree. Correctly printed numbers should be in order (as that is the point of the binary search tree).
*/

node *root = NULL;                                  // set the tree root to NULL
pthread_mutex_t fMutex = PTHREAD_MUTEX_INITIALIZER; // create a mutex

int argv[1]; // create argv to determine number of threads to create

void *threadFunc(void *data)
{

  for (int i = 0; i < 100; ++i) // iterate through 100 times. 1 thread = 100 random numbers between 0-100000
  {
    pthread_mutex_lock(&fMutex); // lock mutex to prevent deadlocking

    int random = rand() % 100001;  // generate 100 random numbers in range of 0 -- 100,000
    root = insert(root, random);   // insert the random number into the BST
    pthread_mutex_unlock(&fMutex); // unlock mutex once done
  }
}

int main(int argc, char **argv)
{
  char *p;                           // create a char in order to convert argv[1] into an int
  int NUMBER_OF_THREADS;             // create a variable to iterate through the thread
  pthread_t tids[NUMBER_OF_THREADS]; // create the thread

  long conv = strtol(argv[1], &p, 10); // conv converts argv[1] into an int

  for (int i = 0; i < conv; ++i) // iterate through the number of threads requested
  {
    /*pthread_create(&(tids[i]), NULL, threadFunc, NULL);*/
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

    inorder(root); // set the numbers in order once completed

    return 0;
  }
}
