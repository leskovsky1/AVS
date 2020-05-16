/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
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

/*globalne premenne*/
char escsq[8] = "!koniec";                   
char pomoc[7]="!pomoc";              
char ukazMeno[10]="!ukazmeno";             
char aktivniKlienti[9]="!aktivni";
char buffer[1024] = "";

/*
 * List klientov
 */
LIST zoznamKlientov;

/*
 * Metoda beziaca vo vlakne, pre pripad ukoncenia servera
 */
void *nacitaj(void){
     char koniec[] = "koniec";
    char pomocna[7];
    do {
    scanf("%s", pomocna);
    } while (strcmp(koniec,pomocna) != 0);
    exit(EXIT_SUCCESS);
}

/*
 * Metoda na vypis chyb
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
 * Metoda ,kt. sluzi na pridanie noveho klienta
 */
bool pridajKlienta(KLIENT * klient){
    for(int i = 0; i < MAX_POCET; i++){
         if (zoznamKlientov.klienti[i] == NULL){
             zoznamKlientov.klienti[i] = klient;
             break;
         }
    } 
    zoznamKlientov.pocetKlientov++;
    return 1;
}

/*
 * Metoda sluziaca na vymazanie klienta 
 */
bool vymazKlienta(KLIENT *klient){
    for (int i = 0; i < MAX_POCET; i++){
       if (zoznamKlientov.klienti[i] != NULL){
        if (zoznamKlientov.klienti[i]->idSoket == klient->idSoket){
            zoznamKlientov.klienti[i] = NULL;
            break;
        }
       }
    }
    zoznamKlientov.pocetKlientov--;
    return 1;
}

/*
 * Posle spravu vsetkym
 */
void posliVsetkym(const char *sprava,const KLIENT *klient){
    int n;
    for (int i = 0; i < MAX_POCET; i++){
        if (zoznamKlientov.klienti[i] != NULL){
        int soket = zoznamKlientov.klienti[i]->idSoket;
            if (soket != klient->idSoket){
                n = write(soket, sprava, strlen(sprava));
                if (n < 0) vypisChybu("Chyba - write");  
            }
        }
    }
}

/*
 * Posle spravu len sebe
 */
void posliSebe(const char *sprava, const KLIENT *klient){
	write(klient->idSoket, sprava, strlen(sprava));
}

/*
 * Vypise vsetkych aktivnych klientov
 */
void ukazAktivnychKlientov(KLIENT *klient){
    int pomocna = 1;
    char ret[1024];
    for (int i = 0; i < MAX_POCET; i++){
        if (zoznamKlientov.klienti[i] != NULL){
            if (klient->idSoket != zoznamKlientov.klienti[i]->idSoket){
            sprintf(ret,"%i. %s \n",pomocna,zoznamKlientov.klienti[i]->meno);
            posliSebe(ret,klient);
            pomocna++;
            }
        }
    }
}

/*
 * Funkcia sluziaca pre obsluhu klienta
 */
bool *obsluhaKlienta(KLIENT *klient){
    
    int n;
    char buffer_in[1024];
    char buffer_out[1024];
    char *prikaz;
    
    sprintf(buffer_out, "Vase meno je %s.\r\nMozete pisat spravy.\n\n", klient->meno);
    posliSebe(buffer_out, klient);
    
    sprintf(buffer_out, "Uzivatel s menom: %s sa prihlasil.\n",klient->meno);
    posliVsetkym(buffer_out, klient);
    
    while((n = read(klient->idSoket, buffer_in, sizeof(buffer_in)-1)) > 0){
        bzero(buffer_out,1024);
        
        if(buffer_in[0] == '!'){
            prikaz = strtok(buffer_in,"\n");
            
            if(strncmp(prikaz,escsq,8)==0){
              posliSebe("koniec\n",klient);
              break;
            }
            
            else if(strncmp(prikaz,pomoc,7)==0){ 
                strcat(buffer_out,"***********POMOCNIK***********\r\n");
                strcat(buffer_out,"!ukazmeno - vase meno na chate\n");
                strcat(buffer_out,"!aktivni - aktivni pouzivatelia\n");            
                strcat(buffer_out,"!pomoc - vyvolanie tohto pomocnika\n");
                strcat(buffer_out,"!koniec - odhlasenie sa\n");
                posliSebe(buffer_out,klient);
            }
          
            else if(strncmp(prikaz,aktivniKlienti,9)==0){ 
                ukazAktivnychKlientov(klient);
            }
            
            else if(strncmp(prikaz,ukazMeno,10)==0){
                sprintf(buffer_out,"Vase meno je: %s\n",klient->meno);
                posliSebe(buffer_out,klient);
            }
            
            else {
                posliSebe("Neznamy prikaz\n",klient);  
            }
        } else {
            sprintf(buffer_out,"%s: %s",klient->meno,buffer_in);
            posliVsetkym(buffer_out,klient);
        }
      bzero(buffer_in,1024);
    } 
    close(klient->idSoket);
    sprintf(buffer_out, "Uzivatel %s sa odhlasil.\n",klient->meno);
    posliVsetkym(buffer_out,klient);
    vymazKlienta(klient);  
    free(klient);
    pthread_detach(pthread_self());
    pthread_exit(NULL);
}