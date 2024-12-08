#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

typedef enum {
    FINISHED,
    STOPPED,
    SIGNALED,
    ACTIVE
}Status;

typedef struct PROCESS{
    pid_t pid;
    struct tm time;
    Status status;
    char cmd[256];
    int prio;      
}PROCESS;

typedef struct nodoLista {PROCESS elemento;
               struct nodoLista * sig;} STNODOPRO;
typedef STNODOPRO *TNODOPRO;

typedef struct lista {TNODOPRO inicio;
                int longitud;
                TNODOPRO fin;} STLISTA;
typedef STLISTA *PRO;



void creapro(PRO *l)
{
    *l=(PRO) malloc(sizeof(STLISTA));
    (*l)->inicio = (TNODOPRO) malloc(sizeof(STNODOPRO));
    (*l)->inicio->sig = NULL;
    (*l)->fin = (*l)->inicio ;
    (*l)->longitud=0;
}


void destruyepro(PRO *l)
{
(*l)->fin = (*l)->inicio;
while ((*l)->fin != NULL)
    {
    (*l)->fin = (*l)->fin->sig;
    free((*l)->inicio);
    (*l)->inicio = (*l)->fin;
    }
free(*l);
}


TNODOPRO primeropro(PRO l)
{
    return l->inicio;
}


TNODOPRO finpro(PRO l)
{
   return l->fin;
}

int esVaciapro(PRO l)
{
   if (l->longitud == 0)   return 1;
   else return 0;
}

void recuperapro(PRO l, TNODOPRO p, PROCESS *e)
{
    *e = p->sig->elemento;
}

int longitudpro(PRO l)
{
    return l->longitud;
}

void insertapro(PRO *l, TNODOPRO p, PROCESS e)
{
    TNODOPRO q ;
    q=p->sig;
    p->sig = (STNODOPRO *) malloc(sizeof(STNODOPRO)) ;
    p->sig->elemento = e;
    p->sig->sig = q;
    if (q==NULL) (*l)->fin=p->sig;
    (*l)->longitud ++ ;
}



void suprimepro(PRO *l, TNODOPRO p)
{
TNODOPRO q;

q=p->sig;
p->sig=q->sig;
if (p->sig==NULL)
    (*l)->fin = p;
free(q);
(*l)->longitud -- ;
}

void modificapro(PRO *l, TNODOPRO p, PROCESS e)
{
    p->sig->elemento=e;
}


TNODOPRO siguientepro(PRO l, TNODOPRO p)
{
   return p->sig;
}