#include "BoundedBuffer.h"
#include <string>
#include <queue>
using namespace std;

BoundedBuffer::BoundedBuffer(int _cap) {
	pthread_mutex_init(&mut, NULL);
	cap = _cap;
}

BoundedBuffer::~BoundedBuffer() {
	pthread_mutex_destroy(&mut);
}

int BoundedBuffer::size() {
	pthread_mutex_lock(&mut);
	int size = q.size();
	pthread_mutex_unlock(&mut);
	return size;
}

void BoundedBuffer::push(string str) {
	pthread_mutex_lock(&mut);
	while (q.size() == cap){
		pthread_cond_wait(&full, &mut);
	}
	q.push(str);
	pthread_mutex_unlock(&mut);
	pthread_cond_signal(&empty);
}

string BoundedBuffer::pop() {
	pthread_mutex_lock(&mut);
	while (q.size() == 0){
		pthread_cond_wait(&empty, &mut);
	}
	string s = q.front();
	q.pop();
	pthread_cond_signal(&full);
	pthread_mutex_unlock(&mut);
	return s;
}
