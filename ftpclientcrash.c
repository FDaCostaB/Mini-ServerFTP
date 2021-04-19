//
// Created by fabie on 11/04/2021.
//

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
    int clientfd ,fd,n;
    char *host, buf[MAXLINE];
    rio_t rio;
    size_t size=1;
    size_t rest,offset;
    char *cmd[2];
    FILE *f;

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
        printf("Veuillez entrez une commande valide :\n");
        Fgets(buf, MAXLINE, stdin);
        while (strlen(buf) == 0) {
            Fgets(buf, MAXLINE, stdin);
            printf("Veuillez entrez une commande valide :\n");
        }
        //Envoie de la commande (nom de fichier)
        Rio_writen(clientfd, buf, strlen(buf));

        //Parsing de la commande
        getCommand(cmd,buf);

        //Potentiel arret du client
        if (cmd[1] == NULL) break;

        //Si cnt envoie de la taille déjà lu

        if (strcmp(cmd[0],"cnt") == 0){
            if ((f = fopen(cmd[1], "r")) != NULL) {
                //taille fichier
                fseek(f, 0, SEEK_END);
                offset = ftell(f);
                Fclose(f);
                printf("Offset : %ld \n", offset);
                //envoyer entier
                Rio_writen(clientfd, &offset, sizeof(size_t));
            }
        } else {
            offset = 0;
        }

        //Lecture de la taille du fichier
        if ((n = Rio_readnb(&rio, &size, sizeof(size_t))) < sizeof(size_t)) {
            printf("Error: Transfer incomplete (%d/%ld) bytes\n", n, sizeof(size_t));
        } else {
            printf("Size of the file : %ld \n", size);

            if(size!=0){
                exit(0);

                //Fin du transfert du fichier
                Close(fd);
                if (rest == 0) printf("Received a total of %ld bytes\n", size);
                else printf("%ld bytes are missing\n", rest);
            } else {
                //lire_entier
                //afficher message
                printf("Transfer cancel \n");
            }
        }
    }while (1);

    Close(clientfd);
    exit(0);
}
