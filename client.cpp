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

#include "reqchannel.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
using namespace std;

struct request_struct {
    int n;
    string name;
    BoundedBuffer* buffer;
};

struct worker_struct {
    RequestChannel* workerChannel;
    BoundedBuffer* buffer;
    Histogram* histogram;
};

void* request_thread_function(void* arg) {
    request_struct* args = (request_struct*) arg;

	for(int i = 0; i < args->n; ++i) {
        args->buffer->push("data " + args->name);
	}
    pthread_exit(NULL);
}

void* worker_thread_function(void* arg) {
    worker_struct* args = (worker_struct*) arg;

    while(true) {
        string request = args->buffer->pop();
        args->workerChannel->cwrite(request);

        if(request == "quit") {
            args->workerChannel->cwrite("quit");
            delete args->workerChannel;
            pthread_exit(NULL);
            break;
        }else{
            string response = args->workerChannel->cread();
            args->histogram->update (request, response);
        }
    }
}

void* stat_thread_function(void* arg) {
    /*
		Fill in this function. 

		There should 1 such thread for each person. Each stat thread 
        must consume from the respective statistics buffer and update
        the histogram. Since a thread only works on its own part of 
        histogram, does the Histogram class need to be thread-safe????

     */

    for(;;) {

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
		Histogram hist;

        for(int i = 0; i < n; ++i) {
            request_buffer.push("data John Smith");
            request_buffer.push("data Jane Smith");
            request_buffer.push("data Joe Smith");
        }
        cout << "Done populating request buffer" << endl;

        cout << "Pushing quit requests... ";
        for(int i = 0; i < w; ++i) {
            request_buffer.push("quit");
        }
        cout << "done." << endl;

	
        chan->cwrite("newchannel");
		string s = chan->cread ();
        RequestChannel *workerChannel = new RequestChannel(s, RequestChannel::CLIENT_SIDE);

        while(true) {
            string request = request_buffer.pop();
			workerChannel->cwrite(request);

			if(request == "quit") {
			   	delete workerChannel;
                break;
            }else{
				string response = workerChannel->cread();
				hist.update (request, response);
			}
        }
        chan->cwrite ("quit");
        delete chan;
        cout << "All Done!!!" << endl; 

		hist.print ();
    }
}
