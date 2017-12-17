#include "../../str.h"
#include <pthread.h>


typedef struct _thread_watch_prms {     
        //pcscf prms
        int proto;
	unsigned int port_c;
        unsigned int port_s;
        //threads prms
        pthread_t thread_c;
        pthread_t thread_s;
        //client prms
        str host;
        unsigned int port_us;        
        } thread_watch_prms;


void * thread_rcv_func(void * arg);
void * thread_watch_func(void * arg);





