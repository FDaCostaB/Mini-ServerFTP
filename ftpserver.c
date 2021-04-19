#include "csapp.h"
#include "ftp.h"
#include "time.h"

#define MAX_NAME_LEN 256
#define NB_PROC 10

pid_t pids[NB_PROC];
int connfd,isParent=0;

/*
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */

void handler_sig(int sig)
{
    if(isParent==1){
        for(int i=0; i<NB_PROC; i++)
            kill(pids[i],sig);
    }
}

void handler_pip(int sig)
{
    Close(connfd);
    connfd=-1;
    fprintf(stderr,"SIGPIPE:Connection closed\n");
}

int main(int argc, char **argv)
{
    int listenfd;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    pid_t pid;
    int status;
    char *cmd[2];

    if (argc != 1) {
        fprintf(stderr, "usage: %s \n", argv[0]);
        exit(0);
    }

    Signal(SIGINT, handler_sig);
    Signal(SIGTERM, handler_sig);
    Signal(SIGPIPE, handler_pip);
    clientlen = (socklen_t)sizeof(clientaddr);
    listenfd = Open_listenfd(2390);

    int i=0;
    // Le fork n'est pas évalué si i>=NB_PROC
    while( i<NB_PROC && ( pid = Fork() ) > 0 ){
        pids[i]=pid;
        i++;
    }

    if(i==NB_PROC) isParent=1;
    if(isParent==0) {
	    Signal(SIGINT, SIG_DFL);
	    Signal(SIGTERM, SIG_DFL);
        while (1) {
            while( (connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen)) < 0 );

            /* determine the name of the client */
            Getnameinfo((SA *) &clientaddr, clientlen,
                        client_hostname, MAX_NAME_LEN, 0, 0, 0);

            /* determine the textual representation of the client's IP address */
            Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                      INET_ADDRSTRLEN);
            printf("server connected to %s (%s)\n", client_hostname,
                   client_ip_string);

	    ftp(&connfd,cmd);
	    printf("Attente d'une nouvelle connexion\n");
        }
        exit(0);
    } else {
        Close(listenfd);
        for(int i=0;i<NB_PROC;i++)
            wait(&status);
	}
    printf("Terminaison du père ( tous les fils sont mort )\n");
    exit(0);
}


