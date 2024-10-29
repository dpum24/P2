#include <stdio.h>
#include <stdlib.h>


typedef struct COMMAND{
    char cmd[30];
    int num;
}COMMAND;



typedef struct nodoLista {COMMAND elemento;
               struct nodoLista * sig;} STNODOHIST;
typedef STNODOHIST *TNODOHIST;

typedef struct lista {TNODOHIST inicio;
                int longitud;
                TNODOHIST fin;} STLISTA;
typedef STLISTA *HIST;




void creahist(HIST *l)
{
    *l=(HIST) malloc(sizeof(STLISTA));
    (*l)->inicio = (TNODOHIST) malloc(sizeof(STNODOHIST));
    (*l)->inicio->sig = NULL;
    (*l)->fin = (*l)->inicio ;
    (*l)->longitud=0;
}


void destruyehist(HIST *l)
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


TNODOHIST primerohist(HIST l)
{
    return l->inicio;
}


TNODOHIST finhist(HIST l)
{
   return l->fin;
}

int esVaciahist(HIST l)
{
   if (l->longitud == 0)   return 1;
   else return 0;
}

void recuperahist(HIST l, TNODOHIST p, COMMAND *e)
{
    *e = p->sig->elemento;
}

int longitudhist(HIST l)
{
    return l->longitud;
}

void insertahist(HIST *l, TNODOHIST p, COMMAND e)
{
    TNODOHIST q ;
    q=p->sig;
    p->sig = (STNODOHIST *) malloc(sizeof(STNODOHIST)) ;
    p->sig->elemento = e;
    p->sig->sig = q;
    if (q==NULL) (*l)->fin=p->sig;
    (*l)->longitud ++ ;
}



void suprimehist(HIST *l, TNODOHIST p)
{
TNODOHIST q;

q=p->sig;
p->sig=q->sig;
if (p->sig==NULL)
    (*l)->fin = p;
free(q);
(*l)->longitud -- ;
}

void modificahist(HIST *l, TNODOHIST p, COMMAND e)
{
    p->sig->elemento=e;
}


TNODOHIST siguientehist(HIST l, TNODOHIST p)
{
   return p->sig;
}