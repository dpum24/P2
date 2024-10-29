
typedef struct FILES{
    int filedes;
    char filename[256]; 
    int mode;       
}FILES;


typedef void *TNODOLISTA;
typedef void *ABIERTOLISTA;

void crea(ABIERTOLISTA *l);
void destruye(ABIERTOLISTA *l);
TNODOLISTA primero(ABIERTOLISTA l);
TNODOLISTA fin(ABIERTOLISTA l);
int esVacia(ABIERTOLISTA l);
void recupera(ABIERTOLISTA l, TNODOLISTA p, FILES *e);
int longitud(ABIERTOLISTA l);
void inserta(ABIERTOLISTA *l, TNODOLISTA p, FILES e);
void suprime(ABIERTOLISTA *l, TNODOLISTA p);
void modifica(ABIERTOLISTA *l, TNODOLISTA p, FILES e);
TNODOLISTA siguiente(ABIERTOLISTA l, TNODOLISTA p);