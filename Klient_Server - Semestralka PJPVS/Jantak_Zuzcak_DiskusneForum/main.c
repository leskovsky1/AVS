/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Petooo
 *
 * Created on December 6, 2017, 7:48 AM
 */
#include "server.h"

#include <sys/types.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#include "CMemLeak.h"
/*
 * Hlavna funkcia main
 */
int main(int argc, char *argv[]) {
    
   zoznamKlientov.pocetKlientov = 0;
   for (int p = 0; p < MAX_POCET; p++){
       zoznamKlientov.klienti[p] = NULL;
   }
    if (argc < 2) {
        vypisChybu("Sever je nutne spustit s nasledujucim argumentom: port.");
    }
    
    int port = atoi(argv[1]);
    if (port <= 0) {
	vypisChybu("Port musi byt cele cislo vacsie ako 0.");
    }
    
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        vypisChybu("Chyba - socket.");        
    }
   
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;             
    serverAddress.sin_addr.s_addr = INADDR_ANY; 
    serverAddress.sin_port = htons(port); 
    
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        vypisChybu("Chyba - bind.");
    }
    
    listen(serverSocket, MAX_POCET);
    
    printf("Server sa spustil. Caka sa na pripojenie klientov.\n");
    
    pthread_t thread2;
    pthread_create(&thread2,NULL,nacitaj,NULL);
    
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
     
    while (1){
     
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        vypisChybu("Chyba - accept.");        
    }
    
    if((zoznamKlientov.pocetKlientov) == MAX_POCET){
        printf("Momentalne je pripojeny maximalny pocet klientov. Skus sa pripojit neskor.");
        close(clientSocket);
        continue;
    }
    
    KLIENT *novyKlient = (KLIENT *)malloc(sizeof(KLIENT));
    novyKlient->addr = clientAddress;
    novyKlient->idSoket = clientSocket;
    read(clientSocket,buffer,sizeof(buffer)-1);
    sprintf(novyKlient->meno,"%s",buffer);
    
    pridajKlienta(novyKlient);
    
    pthread_t thread;
    pthread_create(&thread,NULL,obsluhaKlienta,(void*)novyKlient);
    
    }
    
    return (0);
}

