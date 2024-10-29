#include <stdio.h>
#include <stdlib.h>

typedef struct FILES{
    int filedes;
    char filename[256]; 
    char mode[4];       
}FILES;


typedef struct nodoLista {FILES elemento;
               struct nodoLista * sig;} STNODOLISTA;
typedef STNODOLISTA *TNODOLISTA;

typedef struct lista {TNODOLISTA inicio;
                int longitud;
                TNODOLISTA fin;} STLISTA;
typedef STLISTA *ABIERTOLISTA;




void crea(ABIERTOLISTA *l)
{
    *l=(ABIERTOLISTA) malloc(sizeof(STLISTA));
    (*l)->inicio = (TNODOLISTA) malloc(sizeof(STNODOLISTA));
    (*l)->inicio->sig = NULL;
    (*l)->fin = (*l)->inicio ;
    (*l)->longitud=0;
}


void destruye(ABIERTOLISTA *l)
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


TNODOLISTA primero(ABIERTOLISTA l)
{
    return l->inicio;
}


TNODOLISTA fin(ABIERTOLISTA l)
{
   return l->fin;
}

int esVacia(ABIERTOLISTA l)
{
   if (l->longitud == 0)   return 1;
   else return 0;
}

void recupera(ABIERTOLISTA l, TNODOLISTA p, FILES *e)
{
    *e = p->sig->elemento;
}

int longitud(ABIERTOLISTA l)
{
    return l->longitud;
}

void inserta(ABIERTOLISTA *l, TNODOLISTA p, FILES e)
{
    TNODOLISTA q ;
    q=p->sig;
    p->sig = (STNODOLISTA *) malloc(sizeof(STNODOLISTA)) ;
    p->sig->elemento = e;
    p->sig->sig = q;
    if (q==NULL) (*l)->fin=p->sig;
    (*l)->longitud ++ ;
}



void suprime(ABIERTOLISTA *l, TNODOLISTA p)
{
TNODOLISTA q;

q=p->sig;
p->sig=q->sig;
if (p->sig==NULL)
    (*l)->fin = p;
free(q);
(*l)->longitud -- ;
}

void modifica(ABIERTOLISTA *l, TNODOLISTA p, FILES e)
{
    p->sig->elemento=e;
}


TNODOLISTA siguiente(ABIERTOLISTA l, TNODOLISTA p)
{
   return p->sig;
}