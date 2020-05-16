/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "klient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>
#include <errno.h>

#include "CMemLeak.h"
/*
 * Globalna premenna
 */
char retazec[DLZKA_RETAZCA] = "";

/*
 * Vypisovanie chybovych hlasok
 */
void vypisChybu(char *str) {
    if (errno != 0) {
		perror(str);
	}
	else {
		fprintf(stderr, "%s\n", str);
	}
    exit(EXIT_FAILURE);
}

/*
 * Uvitaci napis pre klienta
 */
void vypis (){
    printf("##################################### VITA VAS DISKUSNE FORUM ####################################\n"
           "\n");  
}

/*
 * Metoda,ktora sa stara o asynchronnost prenosu vo vlakne
 */
void *prichadzajuceSpravy(int sock){
    int n;
    char escsq[8] = "koniec\n";
    
    while((n = read(sock, retazec, sizeof(retazec)-1)) > 0){
        if(strncmp(retazec,escsq,6)==0){
                printf("\nOdhlasenie prebehlo uspesne.");
                close(sock);
                exit(EXIT_SUCCESS);
        }else{
                printf("%s",retazec);
                bzero(retazec,1024);           
        }
    }    
}