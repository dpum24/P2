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


typedef void *TNODOMEM;
typedef void *MEM;

void creamem(MEM *l);
void destruyemem(MEM *l);
TNODOMEM primeromem(MEM l);
TNODOMEM finmem(MEM l);
int esVaciamem(MEM l);
void recuperamem(MEM l, TNODOMEM p, MEMALLOC *e);
int longitudmem(MEM l);
void insertamem(MEM *l, TNODOMEM p, MEMALLOC e);
void suprimemem(MEM *l, TNODOMEM p);
void modificamem(MEM *l, TNODOMEM p, MEMALLOC e);
TNODOMEM siguientemem(MEM l, TNODOMEM p);