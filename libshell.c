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
#include <linux/stat.h>
#include <pwd.h> 
#include <grp.h>
#include <errno.h>
#include "abiertolista.h"
#include "memlist.h"
#include "listahist.h"

void authors(){
    printf("Rubén Sayáns Fortes, ruben.sayans@udc.es\nDiego Emilio Pumarol Guerrero, diego.pumarol@udc.es\n");
}
void authorsl(){
    printf("ruben.sayans@udc.es, diego.pumarol@udc.es\n");
}
void authorsn(){
    printf("Rubén Sayáns Fortes, Diego Emilio Pumarol Guerrero\n");
}

void fecha(){
    time_t t;
    struct tm *current;
    time(&t);
    current = localtime(&t);
    printf("%02d/%02d/%04d\n", current->tm_mday, current->tm_mon + 1, current->tm_year + 1900);
    struct tm *now;
    time(&t);
    now = localtime(&t);
    printf("%02d/%02d/%04d\n",now->tm_hour,now->tm_min,now->tm_sec);
}
void fechad(){
    time_t t;
    struct tm *current;
    time(&t);
    current = localtime(&t);
    printf("%02d/%02d/%04d\n", current->tm_mday, current->tm_mon + 1, current->tm_year + 1900);
}
void fechat(){
    time_t t;
    struct tm *now;
    time(&t);
    now = localtime(&t);
    printf("%02d/%02d/%04d\n",now->tm_hour,now->tm_min,now->tm_sec);
}
void infosys(){
    struct utsname udata;
    if(uname(&udata)<0){
        perror("Error en funcion uname\n");
    }
    printf("nombresistema = %s\n", udata.sysname);
    printf("node name   = %s\n", udata.nodename);
    printf("lanzamiento     = %s\n", udata.release);
    printf("version     = %s\n", udata.version);
    printf("maquina     = %s\n", udata.machine);
}
void historics(char *args[],HIST history,COMMAND *elemento){
    int i = 0;
    if(esVaciahist(history)){
        elemento->num=0;
    }else{
        elemento->num = longitudhist(history);
    }
    while(args[i]!=NULL){
        if (i==0){
        strncpy(elemento->cmd,args[i],sizeof(elemento->cmd));
        strcat(elemento->cmd," ");
        i++;
        continue;
        }
        strncat(elemento->cmd,args[i],20);
        strcat(elemento->cmd," ");
        i++;
    }
    elemento->num++;
}

void listar_abiertos(ABIERTOLISTA abiertos){
    TNODOLISTA nod; 
    FILES f;
    if (!esVacia(abiertos)){//si no es vacia
        for(nod=primero(abiertos); nod != fin(abiertos);nod=siguiente(abiertos,nod)){
        recupera(abiertos,nod,&f);
        printf("File Descriptor= %d -> %s %d\n",f.filedes,f.filename,f.mode);
    }
    }else{
        printf("No hay ningun archivo abierto\n");
        }
}

void Cmd_open(char *tr[], ABIERTOLISTA abiertos, FILES *elemento, int *control) {
    int df, mode = 0;

    // Si no se especifica un nombre de fichero, listar los archivos abiertos
    if (tr[1] == NULL) {
        listar_abiertos(abiertos);
        *control = 0;  // Asignación correcta del valor apuntado
        return;
    }

    // Comprobar si el modo de apertura ha sido especificado
    if (tr[2] == NULL) {
        perror("Modo de apertura no especificado");
        *control = 0;  // Error al no proporcionar el modo
        return;
    }

    // Determinar el modo de apertura
    if (!strcmp(tr[2], "cr")) {
        mode |= O_CREAT;
    } else if (!strcmp(tr[2], "ex")) {
        mode |= O_CREAT | O_EXCL; 
    } else if (!strcmp(tr[2], "ro")) {
        mode |= O_RDONLY;  // Solo lectura
    } else if (!strcmp(tr[2], "wo")) {
        mode |= O_WRONLY;  // Solo escritura
    } else if (!strcmp(tr[2], "rw")) {
        mode |= O_RDWR;  // Lectura y escritura
    } else if (!strcmp(tr[2], "ap")) {
        mode |= O_WRONLY | O_APPEND;  // Agregar al final del archivo
    } else if (!strcmp(tr[2], "tr")) {
        mode |= O_WRONLY | O_TRUNC;  // Truncar el archivo a tamaño 0
    } else {
        perror("Error: Modo de apertura desconocido.\n");
        *control = 0;  // Error si el modo no es reconocido
        return;
    }

    // Abrir el archivo con el modo especificado
    if ((df = open(tr[1], mode, 0644)) == -1) {
        perror("Imposible abrir fichero");
        *control = 0;  // Indicar que hubo un error al abrir el archivo
    } else {
        // Configurar la estructura elemento con la información del archivo abierto
        strcpy(elemento->filename, tr[1]);
        elemento->filedes = df;
        int flags = fcntl(df, F_GETFL);
        if (flags == -1) {
            perror("Error al obtener las banderas del archivo");
            *control = 0;  // Indicar un problema si fcntl falla
            close(df);
            return;
        }

        elemento->mode = flags; 
        *control = 1;  // Indicar que la operación fue exitosa
    }
}


void Cmd_close (char *tr[], ABIERTOLISTA *abiertos){
    int df;
    TNODOLISTA nod;
    FILES e;
    if (tr[1]==NULL || (df=atoi(tr[1]))<0){ 
      listar_abiertos(*abiertos);// el descriptor es menor que 0
        return;
    }
    if (close(df)==-1){
        perror("Inposible cerrar descriptor");
        return;
}
    else{
       for(nod=primero(*abiertos); nod != fin(*abiertos);nod=siguiente(*abiertos,nod)){
        recupera(abiertos,nod,&e);
        if(e.filedes==atoi(tr[1])){
            break;
        }
    }
       suprime(abiertos,nod);
    }
}
int TrocearCadena(char * cadena, char * trozos[]){ 
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
    return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
    i++;
    return i;
}
void help() {
    printf("authors [-l/-n]           Imprime los nombres y logins de los autores del programa.\n");
    printf("pid                       Imprime el PID del proceso actual.\n");
    printf("ppid                      Imprime el PID del proceso padre.\n");
    printf("cd [dir]                  Cambia el directorio de trabajo actual a 'dir'.\n");
    printf("date [-d/-t]              Imprime la fecha y hora actual.\n");
    printf("historic [-N/N]           Muestra o repite comandos del historial.\n");
    printf("open [archivo] modo       Abre un archivo con el modo especificado.\n");
    printf("close [df]                Cierra el descriptor de archivo especificado.\n");
    printf("dup [df]                  Duplica el descriptor de archivo especificado.\n");
    printf("infosys                   Imprime informacion sobre la maquina que ejecuta la shell.\n");
    printf("help [cmd]                Muestra la lista de comandos o ayuda sobre un comando especifico.\n");
    printf("quit/exit/bye             Termina la shell.\n");
    printf("makefile [archivo]        Crea un archivo nuevo.\n");
    printf("makedir [directorio]      Crea un nuevo directorio.\n");
    printf("listfile [archivo/dir]    Da informacion sobre un archivo o directorio.\n");
    printf("cwd                       Imprime el directorio de trabajo actual.\n");
    printf("listdir [directorio]      Lista el contenido de un directorio.\n");
    printf("reclist [directorio]      Lista un directorio recursivamente (subdirectorios despues).\n");
    printf("revlist [directorio]      Lista un directorio recursivamente (subdirectorios antes).\n");
    printf("erase [archivo/dir]       Elimina archivos o directorios vacios.\n");
    printf("delrec [directorio]       Elimina recursivamente un directorio y su contenido.\n");
}

void help_cmd(char *args[]) {
    if (strcmp(args[1], "authors") == 0) {
        printf("Imprime los nombres y logins de los autores del programa.\n"
               "authors -l imprime solo los logins y authors -n imprime solo los nombres.\n");
    } else if (strcmp(args[1], "pid") == 0) {
        printf("Imprime el pid del proceso que está ejecutando la shell.\n");
    } else if (strcmp(args[1], "ppid") == 0) {
        printf("Imprime el pid del proceso padre de la shell.\n");
    } else if (strcmp(args[1], "cd") == 0) {
        printf("Cambia el directorio de trabajo actual de la shell a dir.\n"
               "Si se invoca sin argumentos, imprime el directorio de trabajo actual.\n");
    } else if (strcmp(args[1], "date") == 0) {
        printf("Imprime la fecha actual en formato DD/MM/AAAA y la hora actual en formato hh:mm:ss.\n");
    } else if (strcmp(args[1], "historic") == 0) {
        printf("Muestra el historial de comandos ejecutados por esta shell.\n"
               "historic imprime todos los comandos introducidos con su número de orden.\n"
               "historic N repite el comando número N (de la lista de históricos).\n"
               "historic -N imprime solo los últimos N comandos.\n");
    } else if (strcmp(args[1], "open") == 0) {
        printf("Abre un archivo y lo añade (junto con el descriptor de archivo y el modo de apertura)\n"
               "a la lista de archivos abiertos por la shell.\n");
    } else if (strcmp(args[1], "close") == 0) {
        printf("Cierra el descriptor de archivo df y elimina el item correspondiente de la lista.\n");
    } else if (strcmp(args[1], "dup") == 0) {
        printf("Duplica el descriptor de archivo df, creando la correspondiente nueva entrada en la lista de archivos.\n");
    } else if (strcmp(args[1], "infosys") == 0) {
        printf("Imprime información sobre la máquina que ejecuta la shell.\n");
    } else if (strcmp(args[1], "help") == 0) {
        printf("help muestra una lista de comandos disponibles.\n"
               "help cmd ofrece una breve descripción del uso del comando cmd.\n");
    } else if (strcmp(args[1], "quit") == 0 || strcmp(args[1], "bye") == 0 || strcmp(args[1], "exit") == 0) {
        printf("Finaliza la shell.\n");
    } else if (strcmp(args[1], "makefile") == 0) {
        printf("Crea un archivo con el nombre y contenido especificados.\n");
    } else if (strcmp(args[1], "makedir") == 0) {
        printf("Crea un directorio con el nombre especificado.\n");
    } else if (strcmp(args[1], "listfile") == 0) {
        printf("Proporciona información sobre un archivo o directorio.\n");
    } else if (strcmp(args[1], "cwd") == 0) {
        printf("Imprime el directorio de trabajo actual.\n");
    } else if (strcmp(args[1], "listdir") == 0) {
        printf("Lista el contenido de un directorio.\n");
    } else if (strcmp(args[1], "reclist") == 0) {
        printf("Lista recursivamente el contenido de un directorio, mostrando los subdirectorios despues.\n");
    } else if (strcmp(args[1], "revlist") == 0) {
        printf("Lista recursivamente el contenido de un directorio, mostrando los subdirectorios antes.\n");
    } else if (strcmp(args[1], "erase") == 0) {
        printf("Elimina archivos y/o directorios vacios.\n");
    } else if (strcmp(args[1], "delrec") == 0) {
        printf("Elimina archivos y/o directorios no vacíos recursivamente.\n");
    } else {
        printf("Comando no reconocido: %s\n", args[1]);
    }
}

void Cmd_dup(char *tr[], ABIERTOLISTA *abiertos) {
    int df, duplicado;
    FILES elemento, nuevoElemento;
    char aux[512]; 
    char filename[256];
    int control = 0; 

    // Validar si el descriptor de archivo proporcionado es correcto
    if (tr[1] == NULL || (df = atoi(tr[1])) < 0) {
        listar_abiertos(*abiertos);
        return;
    }

    // Duplicar el descriptor de archivo
    duplicado = dup(df);
    if (duplicado == -1) {
        perror("Error al duplicar el descriptor de archivo");
        return;
    }

    // Encontrar el nombre del archivo asociado al descriptor original
    for (TNODOLISTA nod = primero(*abiertos); nod != fin(*abiertos); nod = siguiente(*abiertos, nod)) {
        recupera(*abiertos, nod, &elemento);
        if (elemento.filedes == df) {
            strncpy(filename, elemento.filename, sizeof(filename) - 1);  // Copiar el nombre del archivo
            filename[sizeof(filename) - 1] = '\0';  // Asegurar que la cadena esté terminada en nulo
            control = 1;
            break;
        }
    }

    // Si no se encuentra el descriptor, limpiar y retornar
    if (control == 0) {
        errno = EBADF;  // Asignar un error adecuado si no se encuentra el descriptor
        perror("Error: No se pudo encontrar el archivo asociado al descriptor");
        close(duplicado); // Cerrar el descriptor duplicado si no se encuentra el original
        return;
    }

    // Crear la descripcion del archivo duplicado
    snprintf(aux, sizeof(aux), "dup %d (%s)", df, filename);

    // Preparar el nuevo elemento para insertar en la lista
    nuevoElemento.filedes = duplicado;      
    strncpy(nuevoElemento.filename, aux, sizeof(nuevoElemento.filename) - 1);  
    nuevoElemento.filename[sizeof(nuevoElemento.filename) - 1] = '\0';  
    nuevoElemento.mode = fcntl(duplicado, F_GETFL); 

    if (nuevoElemento.mode == -1) {
        perror("Error al obtener las banderas del archivo duplicado");
        close(duplicado); // Cerrar el descriptor duplicado si hay un error
        return;
    }
    inserta(abiertos, fin(*abiertos), nuevoElemento);
}



void file_start(ABIERTOLISTA *abiertos){
    FILES elemento;
    TNODOLISTA dndfile=primero(*abiertos);
    int df;
    //Para entrada
    elemento.filedes = STDIN_FILENO; 
    strcpy(elemento.filename, "entrada estandard");
    df = fcntl(STDIN_FILENO, F_GETFL);
    elemento.mode=df;
    inserta(abiertos,dndfile,elemento);
    dndfile = siguiente(*abiertos,dndfile);

    //Para salida
    elemento.filedes = STDOUT_FILENO;
    strcpy(elemento.filename,"salida estandard");
    df=fcntl(elemento.filedes,F_GETFL);
    elemento.mode = df;
    inserta(abiertos,dndfile,elemento);
    dndfile = siguiente(*abiertos,dndfile);

    //Para error
    elemento.filedes = STDERR_FILENO;
    strcpy(elemento.filename,"error estandard");
    df=fcntl(elemento.filedes,F_GETFL);
    elemento.mode = df;
    inserta(abiertos,dndfile,elemento);
    dndfile = siguiente(*abiertos,dndfile);
}
void makefile(char *args[]) {
    char filename[100];
    FILE *file;

    strcpy(filename, args[1]);
    file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    // Cerrar el archivo
    if (fclose(file) != 0) {
        perror("Error al cerrar el archivo");
    }
}

void listdirrec(char *path, int mode) {
    DIR *dir;
    struct dirent *ent;
    struct stat strat;
    struct tm *tm_info;
    char ruta_completa[1024], buffer[75], enlace_destino[1024];

    // Abrir el directorio actual
    dir = opendir(path);
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    }

    // Imprimir encabezado para el directorio actual
    printf("************%s\n", path);

    // Primer recorrido: listar archivos y directorios en el directorio actual
    while ((ent = readdir(dir)) != NULL) {
        // Saltar "." y ".."
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        // Construir la ruta completa del archivo o directorio
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);

        // Obtener la información del archivo o directorio
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        // Modo normal: listar tamaño y nombre
        if (mode == 0) {
            printf("%7ld  %s\n", (long)strat.st_size, ent->d_name);
        } 
        // Modo -link: mostrar enlaces simbólicos y sus destinos
        else if (mode == 1) {
            if (S_ISLNK(strat.st_mode)) {
                ssize_t len = readlink(ruta_completa, enlace_destino, sizeof(enlace_destino) - 1);
                if (len == -1) {
                    perror("Error al leer el enlace simbólico");
                } else {
                    enlace_destino[len] = '\0';
                    printf("Enlace simbólico %s ----> %s\n", ent->d_name, enlace_destino);
                }
            } else {
                printf("%7ld  %s\n", (long)strat.st_size, ent->d_name);
            }
        } 
        // Modo -acc: mostrar fecha y hora del último acceso
        else if (mode == 2) {
            tm_info = localtime(&strat.st_atime);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
            printf("%7ld  %s\tÚltimo acceso: %s\n", (long)strat.st_size, ent->d_name, buffer);
        }
    }

    // Volver al inicio del directorio para recorrer subdirectorios
    rewinddir(dir);

    // Segundo recorrido: entrar en subdirectorios recursivamente
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        // Si es un subdirectorio, aplicar la llamada recursiva
        if (S_ISDIR(strat.st_mode)) {
            listdirrec(ruta_completa, mode);
        }
    }

    // Cerrar el directorio
    closedir(dir);
}
char LetraTF (mode_t m) {
    switch (m & __S_IFMT) {  
        case __S_IFSOCK: return 's'; 
        case __S_IFLNK:  return 'l';
        case __S_IFREG:  return '-';  
        case __S_IFBLK:  return 'b';  
        case __S_IFDIR:  return 'd'; 
        case __S_IFCHR:  return 'c';  
        case __S_IFIFO:  return 'p';
        default:         return '?'; 
    }
}

char* ConvierteModo(mode_t m)
{
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&__S_ISVTX) permisos[9]='t';
    
    return permisos;
}

void listdir(char *path,int mode) {
    struct dirent *ent;
    struct stat strat;
    DIR *dir;
    struct passwd *pwd;
    struct group *grp;
    struct tm *tm_info;
    char buffer[80];
    char ruta_completa[1024];

    dir = opendir(path);
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    }

        // Recorre las entradas del directorio
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {//Ignoramos . y ..
                continue;
            }
            snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);//En ruta_completa tiene toda la ruta, es decir directorio/directorio/archivo
            if (stat(ruta_completa, &strat) == -1) {
                perror("Error al obtener información del archivo");
                continue;
            }
            if(mode==0){//Listdir normal
            printf("%lu\t%s\n", strat.st_size, ent->d_name);
            }
            if(mode == 1){//Para -acc, demuestra tiempos de acceso
            tm_info = localtime(&strat.st_atime);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
            printf("%s\tÚltimo acceso: %s\n", ent->d_name, buffer);
            }
            if(mode==2){//Para -link, hace normal pero si hay link simbolico demuestra a lo que apunta
            if (S_ISLNK(strat.st_mode)) {
                ssize_t len = readlink(ruta_completa, path, sizeof(path) - 1);
                    if (len == -1) {
                        perror("Error al leer el enlace simbolico");
                    }else {
                            path[len] = '\0';
                            printf("El enlace simbolico %s ----> %s\n", ent->d_name, path);
                        }
                }else{
                    printf("%lu\t%s\n", strat.st_size, ent->d_name);
                }
            }if(mode==3){//Para -long
                pwd = getpwuid(strat.st_uid);
                grp = getgrgid(strat.st_gid);
                tm_info = localtime(&strat.st_atime);
                strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
                printf("%s %lu  %lu %s  %s  %s  %lu  %s\n", buffer, strat.st_nlink, strat.st_ino, grp->gr_name,pwd->pw_name,
                   ConvierteModo(strat.st_mode),             
                   strat.st_size,                
                   ent->d_name);
            }if (mode == 4) {  // Para -hid, lista archivos ocultos
            if (ent->d_name[0] == '.') {
                printf("%lu\t%s\n", strat.st_size, ent->d_name);
            }
        }
        }
    closedir(dir);
}

void longlistdirrec(char* path){
    struct dirent *ent;
    struct stat strat;
    DIR *dir;
    char ruta_completa[1024];
    char buffer[80];
    dir = opendir(path);
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    }
    printf("************%s\n", path);
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }
        if (S_ISDIR(strat.st_mode)) {
            longlistdirrec(ruta_completa);
        } else {
            struct passwd *pwd = getpwuid(strat.st_uid);
            struct group *grp = getgrgid(strat.st_gid);
            struct tm *tm_info = localtime(&strat.st_atime);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
            printf("%s %lu  %lu %s  %s  %s  %lu  %s\n", buffer, strat.st_nlink, strat.st_ino, grp->gr_name,pwd->pw_name,
                   ConvierteModo(strat.st_mode),             
                   strat.st_size,                
                   ent->d_name);
        }
    }
    closedir(dir);
}

void revlist(char* path, int mode) {
    struct dirent *ent;
    struct stat strat;
    DIR *dir;
    struct tm *tm_info;
    char buffer[80];
    char ruta_completa[1024];

    dir = opendir(path);//Abrir el directorio introducido en "path"
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    }
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {//Si se encuentra al directorio padre o el actual, no lo listamos
            continue;
        }

        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);//En ruta_completa tiene toda la ruta, es decir directorio/directorio/archivo
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (S_ISDIR(strat.st_mode)) {//Si es un directorio, listarlo primero
            revlist(ruta_completa,mode);
        }
    }
    rewinddir(dir);//Volver atras en el directorio

    printf("************%s\n", path);//Si no hay directorios, imprimir todo lo que haya
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (!S_ISDIR(strat.st_mode)) {//Si no es directorio, imprimir dependiendo del modo
            if(mode==0){//Normal
            printf("%lu\t%s\n", strat.st_size, ent->d_name);
            }
            if(mode == 1){//Para -acc, demuestra tiempos de acceso
            tm_info = localtime(&strat.st_atime);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
            printf("%s\tÚltimo acceso: %s\n", ent->d_name, buffer);
            }
            if(mode==2){//Para -link, hace normal pero si hay link simbolico demuestra a lo que apunta
            if (S_ISLNK(strat.st_mode)) {
                ssize_t len = readlink(ruta_completa, path, sizeof(path) - 1);
                    if (len == -1) {
                        perror("Error al leer el enlace simbolico");
                    }else {
                            path[len] = '\0';
                            printf("El enlace simbolico %s ----> %s\n", ent->d_name, path);
                        }
                }else{
                    printf("%lu\t%s\n", strat.st_size, ent->d_name);
                }
            }
            if (mode == 3) {  // Para -hid, lista archivos ocultos
            if (ent->d_name[0] == '.') {
                printf("%lu\t%s\n", strat.st_size, ent->d_name);
            }
        }
        }
    }

    closedir(dir);
}

void longrevlist(char* path){
        struct dirent *ent;
    struct stat strat;
    DIR *dir;
    char ruta_completa[1024];
    char buffer[85];

    dir = opendir(path);
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    }
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (S_ISDIR(strat.st_mode)) {
            longrevlist(ruta_completa);
        }
    }
    rewinddir(dir);

    printf("************%s\n", path);
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }

        if (!S_ISDIR(strat.st_mode)) {
            struct tm *tm_info = localtime(&strat.st_atime);
            strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", tm_info);
            printf("%s %lu  %lu %u  %u  %s  %lu  %s\n", buffer, strat.st_nlink, strat.st_ino, strat.st_gid,strat.st_uid,
                   ConvierteModo(strat.st_mode),             
                   strat.st_size,                
                   ent->d_name);
        }
    }

    closedir(dir);
}
void delrec(char* path) {
    struct dirent *ent;
    struct stat strat;
    DIR *dir;
    char ruta_completa[1024];
    dir = opendir(path);
    if (dir == NULL) {
        perror("No se pudo abrir el directorio");
        return;
    }
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        snprintf(ruta_completa, sizeof(ruta_completa), "%s/%s", path, ent->d_name);
        if (stat(ruta_completa, &strat) == -1) {
            perror("Error al obtener información del archivo");
            continue;
        }
        if (S_ISDIR(strat.st_mode)) {
            delrec(ruta_completa);
        } else {
            // Si es un archivo regular, eliminarlo
            if (remove(ruta_completa) == -1) {
                perror("Error al eliminar archivo");
            }
        }
    }
    closedir(dir);
    if (remove(path) == -1) {
        perror("Error al eliminar el directorio");
    }
}


void phistorics(HIST history, char *args, ABIERTOLISTA abiertos);
void repeat_cmd(char* input, HIST historial,ABIERTOLISTA abiertos);

void phistorics(HIST history, char *args, ABIERTOLISTA abiertos){
    int num;
    TNODOHIST nod, aux;
    COMMAND e;
    if(args == NULL){
    for(nod =primerohist(history); nod != finhist(history);nod=siguientehist(history,nod)){
        recuperahist(history,nod,&e);
        printf("N=%d %s\n",e.num,e.cmd);
    }}
    else{
        num = atoi(args);
        if(num<0){//Imprime por pantalla los ultimos -n comandos
            num = abs(num);
            nod = primerohist(history);
            for(int i=0; i!=longitudhist(history)-num;i++){
                nod = siguientehist(history,nod);
            }
            for(aux=nod;aux!=fin(history);aux=siguientehist(history,aux)){
                recuperahist(history,aux,&e);
                printf("N=%d %s\n",e.num,e.cmd);
            }
        }else{
            for(nod = primerohist(history);nod!=finhist(history);nod=siguientehist(history,nod)){
            recuperahist(history,nod,&e);
            if(e.num==num){
                printf("%s\n",e.cmd);//Repite el comando numero N
                repeat_cmd(e.cmd,history,abiertos);
                break;
            }
        }
        }
    }
}
void repeat_cmd(char* input, HIST historial,ABIERTOLISTA abiertos){
    COMMAND c;
    FILES f;
    pid_t pid;
    int counter, control,i;
    char *args[20], wd[1024];
    TNODOHIST dndhist = primerohist(historial);
    TNODOLISTA dndfile = fin(abiertos);
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
                    getcwd(wd, sizeof(wd));
                    printf("%s\n", wd);
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
                    inserta(abiertos, dndfile, f); // Insertamos los elementos del archivo en la lista "abiertos"
                    dndfile = siguiente(abiertos, dndfile);
                }
            } else if (strcmp(args[0], "close") == 0) { // Comando Close
                Cmd_close(args, abiertos);
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
            } else if(strcmp(args[0], "makefile") == 0 && args[1] != NULL) {
                makefile(args);
            } else if (strcmp(args[0], "makedir") == 0) {
                if (counter == 0) {
                    getcwd(wd, sizeof(wd));
                    printf("%s\n", wd);
                } else {
                    mkdir(args[1], 0777);
                    opendir(args[1]);
                }
            } else if (strcmp(args[0], "listdir") == 0) {
                if (counter == 1) {
                    getcwd(wd, sizeof(wd));
                    printf("%s\n", wd);
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
                    getcwd(wd, sizeof(wd));
                    printf("%s\n", wd);
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
                    getcwd(wd, sizeof(wd));
                    printf("%s\n", wd);
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
                    getcwd(wd, sizeof(wd));
                    printf("%s\n", wd);
                }
            } else if (strcmp(args[0], "delrec") == 0){
                if (counter > 1){
                    i = 1;
                    while (args[i] != NULL) {
                        delrec(args[i]);
                        i++;
                    }
                    } else {
                    getcwd(wd, sizeof(wd));
                    printf("%s\n", wd);
            }}
            else if (strcmp(args[0], "cwd") == 0) {
                getcwd(wd, sizeof(wd));
                printf("%s\n", wd);
            }else {
                printf("Comando no reconocido: %s\n", args[0]);
            }
            historics(args, historial, &c); // Guardamos en el historial los comandos introducidos
            insertahist(&historial, dndhist, c);
            dndhist = siguientehist(historial, dndhist);
        } else {
            perror("Error al escanear la línea.\n");
        }
}
void cwd(){
    char wd[512];
    getcwd(wd, sizeof(wd));
    printf("%s\n", wd);
}