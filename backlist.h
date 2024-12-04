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


typedef void *TNODOPRO;
typedef void *PRO;

void creapro(PRO *l);
void destruyepro(PRO *l);
TNODOPRO primeropro(PRO l);
TNODOPRO finpro(PRO l);
int esVaciapro(PRO l);
void recuperapro(PRO l, TNODOPRO p, PROCESS *e);
int longitudpro(PRO l);
void insertapro(PRO *l, TNODOPRO p, PROCESS e);
void suprimepro(PRO *l, TNODOPRO p);
void modificapro(PRO *l, TNODOPRO p, PROCESS e);
TNODOPRO siguientepro(PRO l, TNODOPRO p);