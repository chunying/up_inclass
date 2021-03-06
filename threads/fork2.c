/* with pthread_atfork: no more deadlocks */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void prepare(void) {
	pthread_mutex_lock(&lock);
}

void parent(void) {
	pthread_mutex_unlock(&lock);
}

void child(void) {
	pthread_mutex_unlock(&lock);
}

void *worker(void *arg) {
	pthread_mutex_lock(&lock);
	puts("worker: locked.");
	sleep(3);
	puts("worker: unlocked.");
	pthread_mutex_unlock(&lock);
	return(0);
}

int main(void) {
	pid_t pid;
	pthread_t tid;

	setlinebuf(stdout);
	pthread_atfork(prepare, parent, child);
	pthread_create(&tid, NULL, worker, 0);
	sleep(1);
	puts("parent: The lock is held by the worker thread.");

	pid = fork();
	if (pid == 0) {
		puts("child: start.");
		pthread_mutex_lock(&lock);
		puts("child: locked.");
		pthread_mutex_unlock(&lock);
		puts("child: terminated.");
		return 0;
	}

	puts("parent: after fork.");
	pthread_join(tid, NULL);
	puts("parent: terminated.");

	return 0;
}
