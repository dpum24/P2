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
#include "memlist.h"

#define TAMANO 2048

void ImprimirMemoriaLista(MEM shared){
    MEMALLOC m;
    if(esVaciamem(shared)!=1){
    for(TNODOMEM d = primeromem(shared);d != finmem(shared);d = siguientemem(shared,d)){
        recuperamem(shared,d,&m);
        printf("Asignado en %p: %lu bytes\n",m.pointer,m.size);
    }
    }else{
        printf("No hay memoria de este tipo\n");
    }
}

void LiberarMemoriaLista(MEM *lista){
   if(!esVaciamem(lista)){
      MEMALLOC m;
      for(TNODOMEM d = primeromem(*lista);d != finmem(*lista);d = siguientemem(*lista,d)){
        recuperamem(lista,d,&m);
        if(m.tipo == MALLOC){
         free(m.pointer);
        }
    }
   }else{
      return;
   }
}

void Recursiva (int n)
{
  char automatico[TAMANO];
  static char estatico[TAMANO];

  printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

  if (n>0)
    Recursiva(n-1);
}


void LlenarMemoria (void *p, size_t cont, unsigned char byte)
{
  unsigned char *arr=(unsigned char *) p;
  size_t i;

  for (i=0; i<cont;i++)
		arr[i]=byte;
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam,MEM *shared)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    MEMALLOC m;
    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL; /*cuando no es crear pasamos de tamano 0*/
    if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
             shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s); 
    m.pointer = p;
    m.tipo = SHARED;
    m.clave = clave;
    m.size = s.shm_segsz;
    insertamem(shared,finmem(*shared),m);
 /* Guardar en la lista   InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}
void do_AllocateCreateshared (char *tr[], MEM shared){
   key_t cl;
   size_t tam;
   void *p;

   if (tr[2]==NULL || tr[3]==NULL) {
		ImprimirMemoriaLista(shared);
		return;
   }
  
   cl=(key_t)  strtoul(tr[2],NULL,10);
   tam=(size_t) strtoul(tr[3],NULL,10);
   if (tam==0) {
	printf ("No se asignan bloques de 0 bytes\n");
	return;
   }
   if ((p=ObtenerMemoriaShmget(cl,tam,&shared))!=NULL){
      printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
   }
   else{
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
   }
}

void do_AllocateShared (char *tr[], MEM shared)
{
   key_t cl;
   void *p;

   if (tr[2]==NULL) {
		ImprimirMemoriaLista(&shared);
		return;
   }
  
   cl=(key_t)  strtoul(tr[2],NULL,10);

   if ((p=ObtenerMemoriaShmget(cl,0,&shared))!=NULL)
		printf ("Asignada memoria compartida de clave %lu en %p\n",(unsigned long) cl, p);
   else
		printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void * MapearFichero (char * fichero, int protection,MEM *list)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    MEMALLOC m;

    if (protection&PROT_WRITE)
          modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
          return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
           return NULL;
    m.pointer = p;
    m.df = df;
    m.file = fichero;
    m.size = s.st_size;
    m.tipo = MAPPED;
    insertamem(list,finmem(*list),m);
    return p;
}

void do_AllocateMmap(char *arg[],MEM mmap)
{ 
     char *perm;
     void *p;
     int protection=0;
     
     if (arg[1]==NULL)
            {
            ImprimirMemoriaLista(&mmap); 
            return;
            }
     if ((perm=arg[3])!=NULL && strlen(perm)<4) {
            if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
            if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
            if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
     }
     if ((p=MapearFichero(arg[2],protection,&mmap))==NULL)
             perror ("Imposible mapear fichero");
     else
             printf ("fichero %s mapeado en %p\n", arg[1], p);
}

void do_DeallocateDelkey (char *args[])
{
   key_t clave;
   int id;
   char *key=args[1];

   if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
   }
   if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
   }
   if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}



ssize_t LeerFichero (char *f, void *p, size_t cont)
{
   struct stat s;
   ssize_t  n;  
   int df,aux;

   if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	return -1;     
   if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	cont=s.st_size;
   if ((n=read(df,p,cont))==-1){
	aux=errno;
	close(df);
	errno=aux;
	return -1;
   }
   close (df);
   return n;
}

void *cadtop(char *cadena) {
    // Convertir la cadena a un número hexadecimal y castearlo a puntero
    return (void *) strtoull(cadena, NULL, 16);
}

void Cmd_ReadFile (char *ar[])
{
   void *p;
   size_t cont=-1;  /*si no pasamos tamano se lee entero */
   ssize_t n;
   if (ar[1]==NULL || ar[2]==NULL){
	printf ("faltan parametros\n");
	return;
   }
   p=cadtop(ar[2]);  /*convertimos de cadena a puntero*/
   if (ar[2]!=NULL)
	cont=(size_t) atoll(ar[2]);

   if ((n=LeerFichero(ar[1],p,cont))==-1)
	perror ("Imposible leer fichero");
   else
	printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[1],p);
}


void Do_pmap (void) /*sin argumentos*/
 { pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
   char elpid[32];
   char *argv[4]={"pmap",elpid,NULL};
   
   sprintf (elpid,"%d", (int) getpid());
   if ((pid=fork())==-1){
      perror ("Imposible crear proceso");
      return;
      }
   if (pid==0){
      if (execvp(argv[0],argv)==-1)
         perror("cannot execute pmap (linux, solaris)");
         
      argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;   
      if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
         perror("cannot execute procstat (FreeBSD)");
         
      argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;    
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
         perror("cannot execute procmap (OpenBSD)");
         
      argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
      if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
         perror("cannot execute vmmap (Mac-OS)");      
      exit(1);
  }
  waitpid (pid,NULL,0);
}
