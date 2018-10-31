/*
    Based on original assignment by: Dr. R. Bettati, PhD
    Department of Computer Science
    Texas A&M University
    Date  : 2013/01/31
 */


#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>

#include <sys/time.h>
#include <cassert>
#include <assert.h>

#include <cmath>
#include <numeric>
#include <algorithm>

#include <list>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "reqchannel.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
using namespace std;

struct request_struct {
    int n;
    string name;
    BoundedBuffer* request_buffer;
};

struct worker_struct {
    RequestChannel* workerChannel;
    BoundedBuffer* request_buffer;
    BoundedBuffer* response_buffers[3];
    string names[3];
};

struct stat_struct {
    BoundedBuffer* response_buffer;
    string name;
    Histogram* histogram;
};

struct hist_struct {
    Histogram* histogram;
};

void* request_thread_function(void* arg) {
    request_struct* args = (request_struct*) arg;

	for(int i = 0; i < args->n; ++i) {
        args->request_buffer->push("data " + args->name);
	}
    pthread_exit(NULL);
}

void* worker_thread_function(void* arg) {
    worker_struct* args = (worker_struct*) arg;

    while(true) {
        string request = args->request_buffer->pop();
        args->workerChannel->cwrite(request);

        if(request == "quit") {
            args->workerChannel->cwrite("quit");
            delete args->workerChannel;
            pthread_exit(NULL);
            break;
        }else{
            string response = args->workerChannel->cread();
            if (request == "data " + args->names[0]){
                args->response_buffers[0]->push(response);
            }
            else if (request == "data " + args->names[1]){
                args->response_buffers[1]->push(response);
            }
            else if (request == "data " + args->names[2]){
                args->response_buffers[2]->push(response);
            }
        }
    }
}

void* stat_thread_function(void* arg) {
    stat_struct* args = (stat_struct*) arg;
    do{
        string response = args->response_buffer->pop();
        if (response == "quit"){
            pthread_exit(NULL);
            break;
        }
        else{
            args->histogram->update("data " + args->name, response);
        }
    }
    while(true);
}

void* display_histogram_function(void* arg){
    hist_struct* args = (hist_struct*) arg; 
    while (true){
        system("clear");
        args->histogram->print();
        sleep(2);
    }
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
    int n = 100; //default number of requests per "patient"
    int w = 1; //default number of worker threads
    int b = 3 * n; // default capacity of the request buffer, you should change this default
    int opt = 0;
    while ((opt = getopt(argc, argv, "n:w:b:")) != -1) {
        switch (opt) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'w':
                w = atoi(optarg); //This won't do a whole lot until you fill in the worker thread function
                break;
            case 'b':
                b = atoi (optarg);
                break;
		}
    }

    int pid = fork();
	if (pid == 0){
		execl("dataserver", (char*) NULL);
	}
	else {

        cout << "n == " << n << endl;
        cout << "w == " << w << endl;
        cout << "b == " << b << endl;

        RequestChannel *chan = new RequestChannel("control", RequestChannel::CLIENT_SIDE);
        BoundedBuffer request_buffer(b);
        BoundedBuffer response_buffers[3] = {BoundedBuffer(b/3), BoundedBuffer(b/3), BoundedBuffer(b/3)};
        string names[3] = {"John Smith", "Jane Smith", "Joe Smith"};
		Histogram hist(names);

        cout << "Creating Request Threads..." << endl;

        pthread_t request_threads[3];
        request_struct request_args[3];
        for (int i = 0; i < 3; ++i){
            request_args[i].n = n;
            request_args[i].name = names[i];
            request_args[i].request_buffer = &request_buffer;
            pthread_create(&request_threads[i], NULL, &request_thread_function, (void*) &request_args[i]);
        }

        pthread_t worker_threads[w];
        worker_struct worker_args[w];

        cout << "Creating Worker Threads..." << endl;

        for(int i = 0; i < w; ++i) {
            chan->cwrite("newchannel");
            string s = chan->cread ();
            RequestChannel *workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);
            worker_args[i].workerChannel = workerChannel;
            worker_args[i].request_buffer = &request_buffer;
            for (int j = 0; j < 3; j++) worker_args[i].response_buffers[j] = &response_buffers[j];
            for (int j = 0; j < 3; j++) worker_args[i].names[j] = names[j];
            pthread_create(&worker_threads[i], NULL, &worker_thread_function, (void*) &worker_args[i]);
        }

        cout << "Creating Stat Threads..." << endl;

        pthread_t stat_threads[3];
        stat_struct stat_args[3];

        for (int i = 0; i < 3; ++i){
            stat_args[i].response_buffer = &response_buffers[i];
            stat_args[i].name = names[i];
            stat_args[i].histogram = &hist;
            pthread_create(&stat_threads[i], NULL, &stat_thread_function, (void*) &stat_args[i]);
        }

        pthread_t histogram_thread;
        hist_struct hist_args;
        hist_args.histogram = &hist;
        pthread_create(&histogram_thread, NULL, &display_histogram_function, (void*) &hist_args);

        for(int i = 0; i < 3; ++i) pthread_join(request_threads[i], NULL);
        for(int i = 0; i < w; ++i) request_buffer.push("quit");
        for(int i = 0; i < w; ++i) pthread_join(worker_threads[i], NULL);
        for (int i = 0; i < 3; ++i) response_buffers[i].push("quit");
        for(int i = 0; i < 3; ++i) pthread_join(stat_threads[i], NULL);

        system("clear");
        chan->cwrite ("quit");
        delete chan;
        hist.print();
    }
}
