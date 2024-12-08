#include <sys/ipc.h>
#include "searchlist.h"
#include "backlist.h"
#ifndef LIBSHELL_H
#define LIBSHELL_H
void authors();
void authorsl();
void authorsn();
void fechad();
void fechat();
void fecha();
void infosys();
void listar_abiertos(ABIERTOLISTA abiertos);
void historics(char *args[],HIST history,COMMAND *elemento);
void phistorics(HIST history, char *args, ABIERTOLISTA abiertos);
void repeat_cmd(char* input, HIST historial,ABIERTOLISTA abiertos);
void Cmd_open (char * tr[],ABIERTOLISTA abiertos, FILES *elemento,int *control);
int TrocearCadena(char * cadena, char * trozos[]);
void help();
void help_cmd(char * args[]);
void Cmd_close (char *tr[], ABIERTOLISTA *abiertos);
void Cmd_dup (char * tr[],ABIERTOLISTA *abiertos);
void file_start(ABIERTOLISTA *abiertos);
void makefile(char *args[]);
void listdir(char* path, int mode);
void listdirrec(char* path,int mode);
char* ConvierteModo(mode_t m);
char LetraTF (mode_t m);
void longlistdirrec(char* path);
void revlist(char* path, int mode);
void longrevlist(char* path);
void delrec(char* path);
void Cmd_memdump(char *args[]);
ssize_t WriteFichero(char *f, void *p, size_t cont);
ssize_t LeerFichero2 (int df,char *f, void *p, size_t cont);
void Cmd_writefile(char *args[]);
ssize_t WriteFichero2(int df, char *file,void *p, size_t cont);
void DetachSharedMemory(key_t clave, MEM *shared);
void DetachMmap(char *file,MEM *memorial,ABIERTOLISTA *abiertos);
void cwd();
void Cmd_fork (char *tr[]);
char * Ejecutable (char *s, SEARCH dirs);
int Execpve(char *tr[], char **NewEnv, int * pprio, SEARCH dirs);
void exec_chop(char *args[], int counter, char *new_env[],char *cmd_args[],int control);
void Cmd_listjobs(PRO procesos);
#endif