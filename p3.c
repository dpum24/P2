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
#include <pwd.h>
#include <sys/wait.h>
#include "abiertolista.h"
#include "listahist.h"
#include "memlist.h"
#include "ayudaP2.h"
#include "libshell.h"
#include "searchlist.h"
#include "backlist.h"

//Probar que ejecuta con variables de entorno
//Ver como funciona back y backpri en shell referencia (mirar si hay señales o cosas raras)

//valgrind --leak-check=yes ./p2
int glob,globini=10;
float glob2,globini2=1.4;
char glob3,globini3='c';

#define MAX_ARGS 20

extern char **environ;

int main(int argc, char* argv[], char* envp[]) {
    COMMAND c;
    FILES f;
    MEMALLOC m;
    static float hola,adios,hasta;
    static int static1=0,static2=1,static3=2;
    pid_t pid;
    size_t cont;
    time_t t;
    struct tm *now;
    uid_t  uid;
    struct passwd *p;
    int counter, control,i,tam;
    int *prio;
    void *del;
    char *args[MAX_ARGS],*new_env[MAX_ARGS],*env_vars[MAX_ARGS],*cmd_args[MAX_ARGS];
    char *input = malloc(sizeof(char) * 50);
    char *path_var, *pathconfi, *otrodir, *val;
    void *mem;
    LOC dir;
    ABIERTOLISTA abiertos;
    HIST historial;
    MEM memorial;
    SEARCH dirs;
    PRO procesos;
    
    crea(&abiertos);
    creahist(&historial);
    creamem(&memorial);
    creasearch(&dirs);
    creapro(&procesos);
    file_start(&abiertos);
    TNODOHIST dndhist = primerohist(historial);
    TNODOLISTA dndfile = fin(abiertos);
    TNODOSEARCH dndsearch;
    
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
                            listdir(args[2],4);
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
                if (counter == 1) {
                    ImprimirMemoriaLista(memorial);
                } else {
                        if(strcmp(args[1],"-malloc")==0){
                            tam = atoi(args[2]);
                            if (tam != 0){//No se puede asignar 0 bytes de memoria
                            mem = malloc(tam);
                            if (mem != NULL){
                            m.pointer = mem;
                            m.size = tam;
                            m.tipo = MALLOC;
                            t = time(NULL);
                            now = localtime(&t);
                            if (now != NULL) {
                                m.time = *now; 
                        }
                            insertamem(&memorial,finmem(memorial),m);//Insertar en la lista de memoria
                            printf("Memoria malloc asignada en: %p (%d bytes)\n", mem, tam);
                        }
                            else{
                            printf("Error al asignar memoria malloc\n");
                        }
                    }else{
                        printf("No se pueden asignar 0 bytes\n");
                    }}
                        else if(strcmp(args[1],"-mmap")==0){//Para los siguientes, los argumentos de funciones originales pueden estar mal. Falta tambien listas
                            do_AllocateMmap(args,memorial,&abiertos);
                        }else if(strcmp(args[1],"-shared")==0){
                            do_AllocateShared (args,memorial);
                        }else if(strcmp(args[1],"-createshared")==0){
                            do_AllocateCreateshared(args,memorial);
                        }
                }
            }else if (strcmp(args[0],"deallocate")==0){
                if (counter == 1){
                    ImprimirMemoriaLista(memorial);
                }
                if(counter>2){
                    if(strcmp(args[1],"-malloc")==0){
                            tam = atoi(args[2]);
                            for(TNODOMEM d = primeromem(memorial);d !=finmem(memorial);d=siguientemem(memorial,d)){
                                recuperamem(memorial,d,&m);
                                if(m.size == tam){
                                    free(m.pointer);
                                    printf("Liberado %p\n",m.pointer);
                                    break;
                                }
                            }
                        }
                        else if(strcmp(args[1],"-mmap")==0){//Para los siguientes, los argumentos de funciones originales pueden estar mal. Falta tambien listas
                            DetachMmap(args[2],&memorial,&abiertos);
                        }else if(strcmp(args[1],"-shared")==0){
                            DetachSharedMemory(atoi(args[2]),&memorial);
                        }else if(strcmp(args[1],"-delkey")==0){
                            do_DeallocateDelkey(args);
                        }
                    else{
                        if(counter == 2){
                            del = cadtop(args[1]);
                            for(TNODOMEM nodomem=primeromem(memorial);nodomem !=finmem(memorial);nodomem=siguientemem(memorial,nodomem)){
                                recuperamem(memorial,nodomem,&m);
                                if(m.pointer == del && m.tipo==MALLOC){
                                    free(del);
                                    suprimemem(&memorial,nodomem);
                                    break;
                                }
                                if(nodomem==finmem(memorial)){
                                printf("Bloque de memoria no asignado\n");
                            }
                            }}
                }
            }
            }
            else if(strcmp(args[0],"memfill")==0){
                if(counter == 4){
                    del = cadtop(args[1]);
                    LlenarMemoria(del,atoi(args[2]),args[3][0]);
                    printf("Llenando %d bytes de memoria con el byte %hhx a partir de la direccion %p\n",atoi(args[2]),args[3][0],del);
                }
            }
            else if (strcmp(args[0],"memory")==0){
                if(counter > 1){
                    if(strcmp(args[1],"-funcs")==0){
                        printf("Funciones Programa %p %p %p\n",&delrec,&Recursiva,&listdirrec);
                        printf("Funciones Libreria %p %p %p\n",&printf,&fopen,&strcat);
                    }if(strcmp(args[1],"-vars")==0){
                        printf("Variables locales %p %p %p\n",&historial,&counter,&args);
                        printf("Variables globales %p %p %p\n",&globini,&globini2,&globini3);
                        printf("Variables (N.I)globales %p %p %p\n",&glob,&glob2,&glob3);
                        printf("Variables staticas %p %p %p\n",&static1,&static2,&static3);
                        printf("Var (N.I.)staticas %p %p %p\n",&hola,&adios,&hasta);
                    }if(strcmp(args[1],"-all")==0){
                        printf("Funciones Programa %p %p %p\n",&delrec,&Recursiva,&listdirrec);
                        printf("Funciones Libreria %p %p %p\n",&printf,&fopen,&strcat);
                        printf("Variables locales %p %p %p\n",&historial,&counter,&args);
                        printf("Variables globales %p %p %p\n",&globini,&globini2,&globini3);
                        printf("Variables (N.I)globales %p %p %p\n",&glob,&glob2,&glob3);
                        printf("Variables staticas %p %p %p\n",&static1,&static2,&static3);
                        printf("Var (N.I.)staticas %p %p %p\n",&hola,&adios,&hasta);
                        ImprimirMemoriaLista(memorial);
                    }if(strcmp(args[1],"-blocks")==0){
                        ImprimirMemoriaLista(memorial);
                    }
                    if(strcmp(args[1],"-pmap")==0){
                        Do_pmap();
                    }
                }
            }
            else if (strcmp(args[0],"recurse")==0){
                Recursiva(atoi(args[1]));
            }
            else if(strcmp(args[0],"readfile")==0){//En este no tiene que estar abierto
                Cmd_ReadFile(args);
            }
            else if(strcmp(args[0],"read")==0){//Unica diferencia es que tiene que estar abierto
                if(counter==4){
                    for(TNODOLISTA a = primero(abiertos);a!=fin(abiertos);a=siguiente(abiertos,a)){
                        recupera(abiertos,a,&f);
                        if(f.filedes == atoi(args[1])){
                            del=cadtop(args[2]);  /*convertimos de cadena a puntero*/
                            if (args[3]!=NULL){
	                        cont=(size_t) atoll(args[3]);
                            }
                            if ((tam=LeerFichero2(f.filedes,f.filename,del,cont))==-1){
                                perror ("Imposible leer fichero");
                            }
                        else{
	                    printf ("leidos %lld bytes de %s en %p\n",(long long) tam,f.filename,del);
                        }
                        break;
                        }
                    
                }
            }
            }else if (strcmp("memdump",args[0])==0){
                Cmd_memdump(args);
            }else if (strcmp(args[0],"writefile")==0){
                Cmd_writefile(args);
            }else if (strcmp(args[0],"write")==0){
                if(counter==4){
                    for(TNODOLISTA a = primero(abiertos);a!=fin(abiertos);a=siguiente(abiertos,a)){
                        recupera(abiertos,a,&f);
                        if(f.filedes == atoi(args[1])){
                            del=cadtop(args[2]);  /*convertimos de cadena a puntero*/
                            if (args[3]!=NULL){
	                        cont=(size_t) atoll(args[3]);
                            }
                            if ((tam=WriteFichero2(f.filedes,f.filename,del,cont))==-1){
                                perror ("Imposible leer fichero");
                            }
                        else{
	                    printf ("leidos %lld bytes de %s en %p\n",(long long) tam,f.filename,del);
                        }
                        break;
                        }
                    
                }
            }
            }
            else if (!strcmp(args[0],"getuid")){
                uid = getuid();
                if ((p = getpwuid(uid)) == NULL) {
                    perror("getpwuid() error");
                }
                printf("Credencial Real: %s (%d)\n", p->pw_name,uid);
                
                uid = geteuid();
                if ((p = getpwuid(uid)) == NULL) {
                    perror("getpwuid() error");
                }
                printf("Credencial Efectivo: %s (%d)\n", p->pw_name,uid);
            }else if (!strcmp(args[0],"fork")){
                Cmd_fork(args);
            }else if (!strcmp(args[0],"search")){
                if(counter == 3){
                    if (!strcmp(args[1],"-add")){
                        insertasearch(&dirs,finsearch(dirs),args[2]);
                    }if(!strcmp(args[1],"-del")){
                        for(dndsearch=primerosearch(dirs);dndsearch != finsearch(dirs);dndsearch = siguientesearch(dirs,dndsearch)){
                            recuperasearch(dirs,dndsearch,&dir);
                            if(!strcmp(args[2],dir)){
                                suprimesearch(&dirs,dndsearch);
                            }
                        }
                        if(dndsearch == finsearch(dirs)){
                            printf("No se encontro el directorio.\n");
                        }
                    }
                }if(counter == 2){
                    if(!strcmp(args[1],"-clear")){
                        for(dndsearch=primerosearch(dirs);dndsearch != finsearch(dirs);dndsearch = siguientesearch(dirs,dndsearch)){
                                suprimesearch(&dirs,dndsearch);
                        }
                    }
                    if(!strcmp(args[1],"-path")){
                        path_var = getenv("PATH");
                        if (path_var == NULL) {
                            perror("No se pudo obtener la variable PATH\n");
                        }
                        // Crear una copia de PATH porque strtok modifica la cadena
                        pathconfi = strdup(path_var);
                        if (pathconfi == NULL) {
                            perror("Error al duplicar la variable PATH");
                        }
                        for(dndsearch=primerosearch(dirs);dndsearch != finsearch(dirs);dndsearch = siguientesearch(dirs,dndsearch)){
                                suprimesearch(&dirs,dndsearch);//Basicamente search -clear
                        }
                        otrodir = strtok(pathconfi, ":");
                        dndsearch = primerosearch(dirs);
                        while (otrodir != NULL) {
                        insertasearch(&dirs,dndsearch,otrodir); 
                        otrodir = strtok(NULL, ":");
                        dndsearch = siguientesearch(dirs,dndsearch);
                        }}
            }}
            else if (!strcmp(args[0],"exec")){
                exec_chop(args,counter,new_env,cmd_args,0);   
                if (Execpve(cmd_args, new_env, 0, dirs) == -1) {
                    perror("exec");
                }
            }else if(!strcmp(args[0],"execpri")){ 
                exec_chop(args,counter,new_env,cmd_args,1);
                i = atoi(args[1]);
                prio = &i;
                if (Execpve(cmd_args, new_env,prio,dirs) == -1) {
                    perror("exec");
                }
            }else if (!strcmp(args[0],"fg")){
                pid = fork();
    
                if (pid < 0) {
                perror("fallo fork");
                }

                if (pid == 0) {
                // proceso hijo
                exec_chop(args, counter, new_env, cmd_args,0);   
                if (Execpve(cmd_args, new_env, 0, dirs) == -1) {
                perror("exec");
                }
                _exit(1);
                } else {
                // Proceso padre
                if (waitpid(pid, &control, 0) == -1) {
                perror("waitpid");
                }
            }}
            else if (!strcmp(args[0],"fgpri")){
                pid = fork();
                if(pid < 1){
                    perror("fork");
                }
                if(pid == 0){
                    exec_chop(args,counter,new_env,cmd_args,1);   
                    i = atoi(args[1]);
                    prio = &i;
                    if (Execpve(cmd_args, new_env, prio, dirs) == -1) {
                        perror("exec");
                    }
                _exit(1);
                }else {
                    if(waitpid(pid,&control,0)==-1){
                        perror("waipid");
                    }
                }
            }
            else if (strcmp(args[0], "cwd") == 0) {
                cwd();
            } else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "bye") == 0 || strcmp(args[0], "quit") == 0) { // Sale del shell
                free(input); // Al salir liberamos memoria
                destruye(&abiertos);
                destruyehist(&historial);
                destruyesearch(&dirs);
                destruyepro(&procesos);
                LiberarMemoriaLista(&memorial);
                destruyemem(&memorial);
                printf("Saliendo del shell...\n");
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
