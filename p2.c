//Diego Emilio Pumarol Guerrero, login: diego.pumarol@udc.es
//Rubén Sayáns Fortes, login: ruben.sayans@udc.es
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

//falta listfile
//-hid en todos

//valgrind --leak-check=yes ./p1

int main(int argc, char** argv) {
    COMMAND c;
    FILES f;
    MEMALLOC m;
    pid_t pid;
    int counter, control,i,tam;
    char *args[20];
    char *input = malloc(sizeof(char) * 50);
    void *mem;
    ABIERTOLISTA abiertos;
    HIST historial;
    MEM memorial;
    
    crea(&abiertos);
    creahist(&historial);
    creamem(&memorial);
    file_start(&abiertos);
    TNODOHIST dndhist = primerohist(historial);
    TNODOLISTA dndfile = fin(abiertos);
    TNODOMEM dndmem = primeromem(memorial);
    
    while (1) {
        printf("->");
        fgets(input, 50, stdin);
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
                        struct stat info;

                if (stat(args[i], &info) == 0) {
                    if (S_ISDIR(info.st_mode)) {
                    // Si es un directorio, llamamos a la función recursiva
                    if (rmdir(args[i]) < 0) {
                        perror("No se pudo borrar el directorio");
                    }
                    }else {
                        // Si es un archivo, lo borramos
                     if (remove(args[i]) < 0) {
                        perror("Error al ejecutar erase");
                }}
                } else {
                    perror("Error al ejecutar erase");
                }
                    i++;
                }
                }else {
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
                if (counter < 1) {
                    cwd();
                } else {
                        if(strcmp(args[1],"-malloc")==0){
                            tam = atoi(args[2]);
                            mem = malloc(tam);
                            if (mem != NULL){
                            m.pointer = mem;
                            m.size = tam;
                            m.tipo = MALLOC;
                            insertamem(&memorial,dndmem,m);
                            dndmem = siguiente(memorial,dndmem);
                            printf("Memoria malloc asignada en: %p (%d bytes)\n", mem, tam);
                        }
                            else{
                            printf("Error al asignar memoria\n");
                        }
                    }
                        else if(strcmp(args[1],"-mmap")==0){//Para los siguientes, los argumentos de funciones originales pueden estar mal. Falta tambien listas
                            do_AllocateMmap(args);
                        }else if(strcmp(args[1],"-shared")==0){
                            do_AllocateShared (args,memorial);
                        }else if(strcmp(args[1],"-createshared")==0){
                            do_AllocateCreateshared(args,memorial);
                        }
                    else{
                        cwd();
                    }
                }
            }else if (strcmp(args[0],"deallocate")==0){
                if(counter>2){
                    if(strcmp(args[1],"-malloc")==0){
                            //free(pointerlista)
                        }
                            else{
                            printf("Error al asignar memoria\n");
                        }
                    }
                        else if(strcmp(args[1],"-mmap")==0){//Para los siguientes, los argumentos de funciones originales pueden estar mal. Falta tambien listas
                            printf("Mmap dealloc\n");
                        }else if(strcmp(args[1],"-shared")==0){
                            printf("Shared Dealloc\n");
                        }else if(strcmp(args[1],"-delkey")==0){
                            printf("Delkey\n");
                        }
                    else{
                        if(counter == 2){
                            void *del = cadtop(args[1]);
                            free(del);
                            suprimemem(&memorial,primeromem(memorial));//Habria que encontrar donde esta
                            printf("Liberado %p",del);
                        }
                }
            }
            else if(strcmp(args[0],"memfill")==0){
                if(counter > 2){
                    LlenarMemoria((void*)args[2],atoi(args[3]),args[4][0]);
                }else{
                    cwd();
                }
            }
            else if (strcmp(args[0],"memory")==0){
                if(counter > 1){
                    
                }
            }
            else if (strcmp(args[0],"recurse")==0){
                Recursiva(atoi(args[1]));
            }
            else if(strcmp(args[0],"readfile")==0){
                Cmd_ReadFile(args);
            }
            else if(strcmp(args[0],"read")==0){
                if(LeerFichero(args[1],(void*)args[2],atoi(args[3]))==-1){
                    perror("Error al leer el archivo");
                }else{
                    //printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[1],p);
                }
            }
            else if (strcmp(args[0], "cwd") == 0) {
                cwd();
            } else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "bye") == 0 || strcmp(args[0], "quit") == 0) { // Sale del shell
                printf("Saliendo del shell...\n");
                free(input); // Al salir liberamos memoria
                destruye(&abiertos);
                destruyehist(&historial);
                LiberarMemoriaLista(&memorial);
                destruyemem(&memorial);
                break;
            } else {
                printf("Comando no reconocido: %s\n", args[0]);
            }
            historics(args, historial, &c); // Guardamos en el historial los comandos introducidos
            insertahist(&historial, dndhist, c);
            dndhist = siguientehist(historial, dndhist);
        }else {
            perror("Error al escanear la línea.\n");
        }
}
}
