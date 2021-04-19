/*
 * echoclient.c - An echo client
 */
#include "csapp.h"
#include "ftp.h"
#include "send.h"
#include "receive.h"
#include "utils.h"

int main(int argc, char **argv)
{
    int clientfd ,fd;
    char *host, buf[MAXLINE];
    rio_t rio;
    size_t size,code,rest,offset;
    char *cmd[2];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <host>\n", argv[0]);
        exit(0);
    }
    host = argv[1];

    /* Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, 2390);
    
    /* At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */
    printf("client connected to server OS\n"); 
    do {

        Rio_readinitb(&rio, clientfd);

        //Demande commande
        envoyerCmd(buf, clientfd);

        //Parsing de la commande
        getCommand(cmd,buf);

        //Potentiel arret du client
        if (cmd != NULL && cmd[0] != NULL && strcmp(cmd[0],"bye") == 0) {
            Close(clientfd);
            exit(0);
        }

        //Si cnt envoie de la taille déjà lu
        if (cmd != NULL && cmd[0] != NULL && strcmp(cmd[0],"cnt") == 0){
            offset=sizeFile(cmd[1]);
            if((long int)offset==-1)offset=0;
	    printf("Offset : %ld \n", offset);
            envoyer(clientfd, &offset, sizeof(size_t));
        } else {
            offset = 0;
        }

        //Lecture de la taille du fichier
        if (lire(&rio, &size, sizeof(size_t),1) != 0) {
            fprintf(stderr,"Error: During reception of the size of the file. \n");
        } else {
            printf("Size of the file : %ld \n", size);

            if(size!=0){
                //Ouverture du fichier avec le même nom
                if(strcmp(cmd[0],"get")==0)fd = Open(cmd[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                if(strcmp(cmd[0],"cnt")==0){
                    if( (fd = open(cmd[1], O_WRONLY | O_APPEND, NULL )) < 0 )
                        fd = Open(cmd[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
                }

                if(strcmp(cmd[0],"cnt") == 0 )rest = size - offset;
                else rest = size;

                recoitFichier(rest,&rio,buf, fd);


            } else {
                lire(&rio,&code,sizeof (size_t),1);
                afficher_code(code);
                fprintf(stderr,"Transfer cancel \n");
            }
        }
   }while (1);


}
