#include "Barrier.h"
#include <cstdlib>
#include <cstdio>

//enum stage_t {UNDEFINED_STAGE=0, MAP_STAGE=1, SHUFFLE_STAGE=2, REDUCE_STAGE=3};
#define SUCCESS 0
#define ERROR 1

Barrier::Barrier(int numThreads)
		: mutex(PTHREAD_MUTEX_INITIALIZER)
		, cv(PTHREAD_COND_INITIALIZER)
		, count(0)
		, numThreads(numThreads)
{ }


Barrier::~Barrier()
{
	if (pthread_mutex_destroy(&mutex) != 0) {
		fprintf(stderr, "[[Barrier]] error on pthread_mutex_destroy");
		exit(ERROR);
	}
	if (pthread_cond_destroy(&cv) != 0){
		fprintf(stderr, "[[Barrier]] error on pthread_cond_destroy");
		exit(ERROR);
	}
}

void Barrier::barrier()
{
	if (pthread_mutex_lock(&mutex) != SUCCESS){
		fprintf(stderr, "[[Barrier]] error on pthread_mutex_lock");
		exit(ERROR);
	}
	if (++count < numThreads) {
		if (pthread_cond_wait(&cv, &mutex) != SUCCESS){
			fprintf(stderr, "[[Barrier]] error on pthread_cond_wait");
			exit(ERROR);
		}
	} else {
		count = 0;
        // End of the part
		if (pthread_cond_broadcast(&cv) != SUCCESS) {
			fprintf(stderr, "[[Barrier]] error on pthread_cond_broadcast");
			exit(ERROR);
		}
	}
	if (pthread_mutex_unlock(&mutex) != SUCCESS) {
		fprintf(stderr, "[[Barrier]] error on pthread_mutex_unlock");
		exit(ERROR);
	}
}
