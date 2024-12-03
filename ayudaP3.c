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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h> 
#include <sys/wait.h>
#include <dirent.h>
#include <linux/stat.h>
#include <pwd.h> 
#include <grp.h>
#include <errno.h>
#include <linux/types.h>
#include <sys/resource.h>
#include "abiertolista.h"
#include "searchlist.c"

#define MAXNAME 512
#define MAXVAR 256

extern char **environ;


int BuscarVariable (char * var, char *e[]) {
  int pos=0;
  char aux[MAXVAR];
  
  strcpy (aux,var);
  strcat (aux,"=");
  
  while (e[pos]!=NULL)
    if (!strncmp(e[pos],aux,strlen(aux)))
      return (pos);
    else 
      pos++;
  errno=ENOENT;
  return(-1);
}

void Cmd_showvar(int count, char *vars[], char *envp[]) {
    for (int i = 0; i < count; i++) {
        char *var_name = vars[i];
        int value;
        // Con el tercer argumento de main
        value = BuscarVariable(var_name, envp);
        if (value != -1) {
            printf("Con arg3 main %s (%p) @%p\n", envp[value], strchr(envp[value], '=') + 1, envp[value]);
        } else {
            printf("Con arg3 main %s no encontrado\n", var_name);
        }

        // Con environ
        value = BuscarVariable(var_name, environ);
        if (value != -1) {
            printf("  Con environ %s (%p) @%p\n", environ[value], strchr(environ[value], '=') + 1, environ[value]);
        } else {
            printf("  Con environ %s no encontrado\n", var_name);
        }

        // Con getenv
        char *getenv_value = getenv(var_name);
        if (getenv_value) {
            printf("   Con getenv %s (%p)\n", getenv_value, getenv_value);
        } else {
            printf("   Con getenv %s no encontrado\n", var_name);
        }
    }
}


void Cmd_fork (char *tr[]){
	pid_t pid;
	
	if ((pid=fork())==0){
/*		VaciarListaProcesos(&LP); Depende de la implementaciÃ³n de cada uno*/
		printf ("ejecutando proceso %d\n", getpid());
	}
	else if (pid!=-1)
		waitpid (pid,NULL,0);
}


int CambiarVariable(char * var, char * valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parÃ¡metro*/
{                                                        /*lo hace directamente, no usa putenv*/
  int pos;
  char *aux;
   
  if ((pos=BuscarVariable(var,e))==-1)
    return(-1);
 
  if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
	return -1;
  strcpy(aux,var);
  strcat(aux,"=");
  strcat(aux,valor);
  e[pos]=aux;
  return (pos);
}

char * Ejecutable (char *s, SEARCH dirs) //Busca en la lista el ejecutable s
{
        static char path[MAXNAME];
        struct stat st;
        DIR p;
        TNODOSEARCH nodosearch = primerosearch(dirs);
   
        if (s==NULL || (esVaciasearch(dirs)))
                return s;
        if (s[0]=='/' || !strncmp (s,"./",2) || !strncmp (s,"../",3))
        return s;        /*s is an absolute pathname*/
        
        strncpy (path, p, MAXNAME-1);strncat (path,"/",MAXNAME-1); strncat(path,s,MAXNAME-1);
        if (lstat(path,&st)!=-1)
                return path;
        while (nodosearch != finsearch(dirs)){
            recuperasearch(dirs,nodosearch,&p);
            strncpy (path, p, MAXNAME-1);strncat (path,"/",MAXNAME-1); strncat(path,s,MAXNAME-1);
            if (lstat(path,&st)!=-1)
                   return path;
            nodosearch = siguientesearch(dirs,nodosearch);
        }
        return s;
}

int Execpve(char *tr[], char **NewEnv, int * pprio) { //Modificar argumentos

char *p; /*NewEnv contains the address of the new environment*/
/*pprio the address of the new priority*/
/*NULL indicates no change in environment and/or priority*/
if (tr[0]==NULL || (p=Ejecutable(tr[0]))==NULL){
errno=EFAULT;
return-1;
}
if (pprio !=NULL && setpriority(PRIO_PROCESS,getpid(),*pprio)==-1 && errno){
printf ("Imposible cambiar prioridad: %s\n",strerror(errno));
return -1;
}
if (NewEnv==NULL)
return execv (p,tr);
else
return execve (p, tr, NewEnv);
}
/*

static struct SEN sigstrnum[]={   
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL}, 
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2}, 
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP}, 
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, 
	{"IO", SIGIO},
	{"SYS", SIGSYS},

#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
 	{NULL,-1},
	};   

*/

/*
int ValorSenal(char * sen)  devuelve el numero de senial a partir del nombre
{ 
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (!strcmp(sen, sigstrnum[i].nombre))
		return sigstrnum[i].senal;
  return -1;
}


//char *NombreSenal(int sen)  devuelve el nombre senal a partir de la senal
//{			para sitios donde no hay sig2str
 int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	if (sen==sigstrnum[i].senal)
		return sigstrnum[i].nombre;
 return ("SIGUNKNOWN");
}*/