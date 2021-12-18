#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream> 
#include <mutex>
#include <unistd.h>
using namespace std;


void* threadFunc(void* data)
{
    
}


int main()
{
    pthread_t tids[100];
    int intArray[100];
    if(pthread_create(&tids, NULL, threadFunc, (void*)(intArray)) != 0)
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