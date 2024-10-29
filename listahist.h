typedef struct COMMAND{
    char cmd[30];
    int num;
}COMMAND;


typedef void *TNODOHIST;
typedef void *HIST;

void creahist(HIST *l);
void destruyehist(HIST *l);
TNODOHIST primerohist(HIST l);
TNODOHIST finhist(HIST l);
int esVaciahist(HIST l);
void recuperahist(HIST l, TNODOHIST p, COMMAND *e);
int longitudhist(HIST l);
void insertahist(HIST *l, TNODOHIST p, COMMAND e);
void suprimehist(HIST *l, TNODOHIST p);
void modificahist(HIST *l, TNODOHIST p, COMMAND e);
TNODOHIST siguientehist(HIST l, TNODOHIST p);