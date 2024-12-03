#include <stdio.h>
#include <stdlib.h>

typedef char* DIR;


typedef struct nodoLista {DIR elemento;
               struct nodoLista * sig;} STNODOSEARCH;
typedef STNODOSEARCH *TNODOSEARCH;

typedef struct lista {TNODOSEARCH inicio;
                int longitud;
                TNODOSEARCH fin;} STLISTA;
typedef STLISTA *SEARCH;



void creasearch(SEARCH *l)
{
    *l=(SEARCH) malloc(sizeof(STLISTA));
    (*l)->inicio = (TNODOSEARCH) malloc(sizeof(STNODOSEARCH));
    (*l)->inicio->sig = NULL;
    (*l)->fin = (*l)->inicio ;
    (*l)->longitud=0;
}


void destruyesearch(SEARCH *l)
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


TNODOSEARCH primerosearch(SEARCH l)
{
    return l->inicio;
}


TNODOSEARCH finsearch(SEARCH l)
{
   return l->fin;
}

int esVaciasearch(SEARCH l)
{
   if (l->longitud == 0)   return 1;
   else return 0;
}

void recuperasearch(SEARCH l, TNODOSEARCH p, DIR *e)
{
    *e = p->sig->elemento;
}

int longitudsearch(SEARCH l)
{
    return l->longitud;
}

void insertasearch(SEARCH *l, TNODOSEARCH p, DIR e)
{
    TNODOSEARCH q ;
    q=p->sig;
    p->sig = (STNODOSEARCH *) malloc(sizeof(STNODOSEARCH)) ;
    p->sig->elemento = e;
    p->sig->sig = q;
    if (q==NULL) (*l)->fin=p->sig;
    (*l)->longitud ++ ;
}



void suprimesearch(SEARCH *l, TNODOSEARCH p)
{
TNODOSEARCH q;

q=p->sig;
p->sig=q->sig;
if (p->sig==NULL)
    (*l)->fin = p;
free(q);
(*l)->longitud -- ;
}

void modificasearch(SEARCH *l, TNODOSEARCH p, DIR e)
{
    p->sig->elemento=e;
}


TNODOSEARCH siguientesearch(SEARCH l, TNODOSEARCH p)
{
   return p->sig;
}