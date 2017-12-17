#include "../../udp_server.h"
#include "udp_threads.h"
#include "registrar_storage.h"


extern char * pcscf_ipsec_host;			/**< IP for protected server */
extern int   pcscf_ipsec_port_c;		/**< PORT for protected client */
extern int   pcscf_ipsec_port_s;		/**< PORT for protected server */
struct socket_info * si_pc;
struct socket_info * si_ps;


/**
 * Thread's function.Execute receive loop for listening socket
 * @param arg - socket_info for listening socket
 * @returns void
 */
void * thread_rcv_func(void *arg)
{ 
  struct socket_info * bind_addr = (struct socket_info * ) arg; 
  thread_rcv_loop(bind_addr);
}


/**
 * Thread's function.Execute watching loop for stop listening threads.
 * @param arg - socket_info for listening socket
 * @returns void
 */

void * thread_watch_func(void *arg)
{
  thread_watch_prms prms = * (thread_watch_prms *) arg; 
  for(;;)
  {       
    r_contact * contact=get_r_contact(prms.host,prms.port_us,prms.proto,NORMAL_REG);
    if(contact==0)
    {
      str str_prm={pcscf_ipsec_host,strlen(pcscf_ipsec_host)};
      struct socket_info * bind_address_c=grep_sock_info(&str_prm,prms.port_c,prms.proto);
      struct socket_info * bind_address_s=grep_sock_info(&str_prm,prms.port_s,prms.proto);
       
      pthread_cancel(prms.thread_c);
      pthread_cancel(prms.thread_s);
      close(bind_address_c->socket);
      close(bind_address_s->socket);
      delete_sock_info(bind_address_c,prms.proto);
      delete_sock_info(bind_address_s,prms.proto);
      pthread_exit(NULL);
    }
    else
    {
      r_unlock(contact->hash); 
      sleep(10);
    }
  }
}



