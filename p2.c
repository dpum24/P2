#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/utsname.h>  
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "abiertolista.h"
#include "listahist.h"
#include "memlist.h"
#include "ayudaP2.h"
#include "libshell.h"

//Falla reclist y revlist en que no lista archivos directorio actual
//Falla erase, no borra directorio
//falta listfile
//Alguno sin argumento no pone directorio actual
//-hid en todos

//valgrind --leak-check=yes ./p1

int main(int argc, char** argv) {
    COMMAND c;
    FILES f;
    MEMALLOC m;
    pid_t pid;
    int counter, control,i,tam;
    char *args[20];
    char *input = malloc(sizeof(char) * 30);
    void *mem;
    ABIERTOLISTA abiertos;
    HIST historial;
    MEM memorial, mmap, shared;
    
    crea(&abiertos);
    creahist(&historial);
    creamem(&memorial);
    creamem(&mmap);
    creamem(&shared);
    file_start(&abiertos);
    TNODOHIST dndhist = primerohist(historial);
    TNODOLISTA dndfile = fin(abiertos);
    TNODOMEM dndmem = primeromem(memorial);
    
    while (1) {
        printf("->");
        fgets(input, 30, stdin);
        counter = TrocearCadena(input, args); // Trocea "input" en el array de strings "args", numero de args es "counter"
        
        if (counter != 0) {
            if (strcmp(args[0], "date") == 0) { // Comando de fechas falta el caso de sin argumentos
                if (args[1] != NULL && strcmp(args[1], "-t") == 0) {
                    fechat(); // Formato hh:mm:ss
                } else if (args[1] != NULL && strcmp(args[1], "-d") == 0) {
                    fechad(); // Formato DD/MM/YYYY
                } else {
                    perror("No se encontró tu comando\n");
                }
            } else if (strcmp(args[0], "pid") == 0) { // PID
                pid = getpid();
                printf("%d\n", pid);
            } else if (strcmp(args[0], "ppid") == 0) {
                pid = getppid();
                printf("%d\n", pid);
            } else if (strcmp(args[0], "cd") == 0) { // Directorios
                if (args[1] == NULL) {
                    cwd();
                } else { // Si recibimos argumentos, cambiar al directorio si existe
                    if (chdir(args[1]) == -1) {
                        printf("Error al encontrar el directorio %s\n", args[1]);
                    } else {
                        printf("Exitoso cambio a %s\n", args[1]);
                    }
                }
            } else if (strcmp(args[0], "open") == 0) { // Comando Open
                Cmd_open(args, abiertos, &f, &control); // Guarda en "e" los elementos del archivo abierto
                if (control == 1) {
                    inserta(&abiertos, dndfile, f); // Insertamos los elementos del archivo en la lista "abiertos"
                    dndfile = siguiente(abiertos, dndfile);
                }
            } else if (strcmp(args[0], "close") == 0) { // Comando Close
                Cmd_close(args, &abiertos);
                dndfile = fin(abiertos);
            } else if (strcmp(args[0], "dup") == 0) {
                Cmd_dup(args, &abiertos);
            } else if (strcmp(args[0], "infosys") == 0) { // Información del Sistema
                infosys();
            } else if (strcmp(args[0], "authors") == 0) { // Autores de la práctica
                if (counter == 1) {
                    authors(); // Devuelve nombres y correos
                } else {
                    if (strcmp(args[1], "-l") == 0) {
                        authorsl(); // Devuelve nuestros correos
                    } else if (strcmp(args[1], "-n") == 0) {
                        authorsn(); // Devuelve nuestros nombres
                    } else {
                        perror("No se encontró tu comando\n");
                    }
                }
            } else if (strcmp(args[0], "help") == 0) {
                if (counter > 1) {
                    help_cmd(args); // Ayuda con argumento incluido
                } else {
                    help(); // Imprime por pantalla todos los comandos disponibles
                }
            } else if (strcmp(args[0], "historic") == 0) {
                phistorics(historial, args[1],abiertos); // Acceso a la lista de comandos introducidos
            } else if(strcmp(args[0], "makefile") == 0) {
                if (counter>1){
                    makefile(args);
                }
                else{
                    cwd();
                }
            } else if (strcmp(args[0], "makedir") == 0) {
                if (counter == 0) {
                    cwd();
                } else {
                    mkdir(args[1], 0777);
                    opendir(args[1]);
                }
            } else if (strcmp(args[0], "listdir") == 0) {
                if (counter == 1) {
                    cwd();
                } else {
                    if (counter == 3) {
                        if (strcmp(args[1], "-hid") == 0) {
                            listdir(args[2],0);
                        } else if (strcmp(args[1], "-acc") == 0) { // Tiempo de acceso
                           listdir(args[2],1);
                        } else if (strcmp(args[1], "-link") == 0) { // Si es enlace simbolico, mostrar directorio a que apunta
                            listdir(args[2],2);
                    } else if (strcmp(args[1],"-long")==0){
                        listdir(args[2],3);
                    }
                    else {
                        listdir(args[1],0);
                    }
                }else {
                    listdir(args[1],0);
                }
            }}else if(strcmp(args[0],"reclist")==0){
                //Es igual que listdir, primero (-acc,-link,-long,-hid) luego directorio
                if (counter == 1) {
                    cwd();
                } else {
                    if (counter == 3) {
                        if (strcmp(args[1], "-hid") == 0) {
                            listdirrec(args[2],3);
                        } else if (strcmp(args[1], "-acc") == 0) { // Tiempo de acceso
                           listdirrec(args[2],2);
                        } else if (strcmp(args[1], "-link") == 0) { // Si es enlace simbolico, mostrar directorio a que apunta
                            listdirrec(args[2],1);
                    } else if (strcmp(args[1],"-long")==0){
                        longlistdirrec(args[2]);
                    }
                    else {
                        listdirrec(args[2],0);
                    }
                }else {
                    listdirrec(args[1],0);
                }
            }
            }
            else if (strcmp(args[0],"revlist")==0){
                if (counter == 1) {
                    cwd();
                } else {
                    if (counter == 3) {
                        if (strcmp(args[1], "-hid") == 0) {
                            revlist(args[2],3);
                        } else if (strcmp(args[1], "-acc") == 0) { // Tiempo de acceso
                           revlist(args[2],1);
                        } else if (strcmp(args[1], "-link") == 0) { // Si es enlace simbolico, mostrar directorio a que apunta
                            revlist(args[2],2);
                    } else if (strcmp(args[1],"-long")==0){
                        longrevlist(args[2]);
                    }
                    else {
                        revlist(args[2],0);
                    }
                }else {
                    revlist(args[1],0);
                }
            }}
            else if (strcmp(args[0], "erase") == 0) {
                if (counter > 1) {
                    i = 1;
                    while (args[i] != NULL) {
                        remove(args[i]);
                        i++;
                    }
                } else {
                    cwd();
                }
            } else if (strcmp(args[0], "delrec") == 0){
                if (counter > 1){
                    i = 1;
                    while (args[i] != NULL) {
                        delrec(args[i]);
                        i++;
                    }
                    } else {
                    cwd();
            }}
            else if(strcmp(args[0],"allocate")==0){
                if (counter == 1) {
                    cwd();
                } else {
                    if (counter == 3) {//Puede que en -mmap, etc no sean solo 3 argumentos.
                        if(strcmp(args[1],"-malloc")==0){
                            tam = atoi(args[2]);
                            mem = malloc(tam);
                            if (mem != NULL){
                            m.pointer = mem;
                            m.size = tam;
                            insertamem(&memorial,dndmem,m);
                            dndmem = siguiente(memorial,dndmem);
                            printf("Memoria malloc asignada en: %p (%d bytes)\n", mem, tam);
                        }
                            else{
                            printf("Error al asignar memoria\n");
                        }
                    }
                        }else if(strcmp(args[1],"-mmap")==0){//Para los siguientes, los argumentos de funciones originales pueden estar mal. Falta tambien listas
                            do_AllocateMmap(args);
                        }else if(strcmp(args[1],"-shared")==0){
                            do_AllocateShared (args,shared);
                        }else if(strcmp(args[1],"-createshared")==0){
                            do_AllocateCreateshared(args,shared);
                        }
                    else{
                        cwd();
                    }
                }
            }
            else if(strcmp(args[0],"memfill")==0){
                if(counter > 2){
                    LlenarMemoria((void*)args[2],atoi(args[3]),(unsigned char)args[4]);
                }else{
                    cwd();
                }
            }
            else if (strcmp(args[0], "cwd") == 0) {
                cwd();
            } else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "bye") == 0 || strcmp(args[0], "quit") == 0) { // Sale del shell
                printf("Saliendo del shell...\n");
                free(input); // Al salir liberamos memoria
                destruye(&abiertos);
                destruyehist(&historial);
                LiberarMemoriaLista(&shared),
                LiberarMemoriaLista(&memorial);
                destruyemem(&memorial);
                destruyemem(&mmap);
                destruyemem(&shared);
                break;
            } else {
                printf("Comando no reconocido: %s\n", args[0]);
            }
            historics(args, historial, &c); // Guardamos en el historial los comandos introducidos
            insertahist(&historial, dndhist, c);
            dndhist = siguientehist(historial, dndhist);
        } else {
            perror("Error al escanear la línea.\n");
        }
    }
}