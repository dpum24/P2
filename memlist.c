#include <stdio.h>
#include <stdlib.h>

typedef enum {
    MALLOC,
    SHARED,
    MAPPED
} AllocationType;

typedef struct MEMALLOC{
    void *pointer;
    size_t size;
    AllocationType tipo;
    char *file;
    int df;
    __key_t clave;
}MEMALLOC;


typedef struct nodoLista {MEMALLOC elemento;
               struct nodoLista * sig;} STNODOMEM;
typedef STNODOMEM *TNODOMEM;

typedef struct lista {TNODOMEM inicio;
                int longitud;
                TNODOMEM fin;} STLISTA;
typedef STLISTA *MEM;



void creamem(MEM *l)
{
    *l=(MEM) malloc(sizeof(STLISTA));
    (*l)->inicio = (TNODOMEM) malloc(sizeof(STNODOMEM));
    (*l)->inicio->sig = NULL;
    (*l)->fin = (*l)->inicio ;
    (*l)->longitud=0;
}


void destruyemem(MEM *l)
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


TNODOMEM primeromem(MEM l)
{
    return l->inicio;
}


TNODOMEM finmem(MEM l)
{
   return l->fin;
}

int esVaciamem(MEM l)
{
   if (l->longitud == 0)   return 1;
   else return 0;
}

void recuperamem(MEM l, TNODOMEM p, MEMALLOC *e)
{
    *e = p->sig->elemento;
}

int longitudmem(MEM l)
{
    return l->longitud;
}

void insertamem(MEM *l, TNODOMEM p, MEMALLOC e)
{
    TNODOMEM q ;
    q=p->sig;
    p->sig = (STNODOMEM *) malloc(sizeof(STNODOMEM)) ;
    p->sig->elemento = e;
    p->sig->sig = q;
    if (q==NULL) (*l)->fin=p->sig;
    (*l)->longitud ++ ;
}



void suprimemem(MEM *l, TNODOMEM p)
{
TNODOMEM q;

q=p->sig;
p->sig=q->sig;
if (p->sig==NULL)
    (*l)->fin = p;
free(q);
(*l)->longitud -- ;
}

void modificamem(MEM *l, TNODOMEM p, MEMALLOC e)
{
    p->sig->elemento=e;
}


TNODOMEM siguientemem(MEM l, TNODOMEM p)
{
   return p->sig;
}