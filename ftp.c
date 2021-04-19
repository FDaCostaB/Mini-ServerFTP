/*
 * ftp - open and send files until client closes connection
 */
#include "csapp.h"
#include "ftp.h"
#include "send.h"
#include "receive.h"
#include "utils.h"



int ftp(int *connfd, char **cmd){
    size_t code,offset=0;
    long int size = 0;
    int n;
    char buf[MAXLINE];
    rio_t rio1,rio2;
    int fd;

    do {
        Rio_readinitb(&rio1, *connfd);
        if(*connfd>0)printf("Attente d'une nouvelle commande.\n");

            //Reception commande
            if (*connfd>=0 && (n = lireMessage(&rio1, buf, *connfd) ) >= 0) {
                if(getCommand(cmd, buf)<0){
                    askNewCmd(*connfd,CMD_INCONNU);
		    continue;
		}
            }else {
            	closeConnection(connfd);
	    	return -1;
            }


        if( commandeReconnu(cmd) ) {
            if(strcmp(cmd[0],"bye")==0){
		closeConnection(connfd);
		return 1;
	    }

            if(strcmp(cmd[0],"cnt")==0) {
                if(lire(&rio1, &offset, sizeof(size_t), *connfd)<0) {
                    closeConnection(connfd);
                } else {
                    printf("Size already received : %ld \n", offset);
                }
            } else {
                offset = 0;
            }


            //Envoi de la taille du fichier
            if( (size=sizeFile(cmd[1])) >= 0){
                printf("Size : %ld \n", size);
                envoyer(*connfd, &size, sizeof(size_t));

                //Pas d'erreur d'ouverture si on a réussi à lire la taille à condition de ne pas modifier un fichier
                //pendant un transfert mais on considère que les fichiers ne sont pas modifiés quand le serveur est ouvert.
                printf("Opening : %s \n", cmd[1]);
                fd = Open(cmd[1], O_RDONLY, 0);

                //Si cnt on reprend du bonne endroit
                if(offset > 0) lseek(fd,offset,SEEK_SET);

		        Rio_readinitb(&rio2, fd);
            	//Envoi fichier
            	if(envoieFichier(size,offset,&rio2,connfd)<0){
		            fprintf(stderr,"Client has exited before the end of transfer.\n");
		            return -1;
		        }
            	code=FICHIER_VIDE;
            	envoyer(*connfd,&code,sizeof(size_t));
            } else {
                askNewCmd(*connfd,FICHIER_MANQUANT);
            }

        } else {
            askNewCmd(*connfd,CMD_INCONNU);
            size = 0;
            envoyer(*connfd, &size, sizeof(size_t));
        }
    } while(1);
    return 0;
}
