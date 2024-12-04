typedef char* LOC;

typedef void *TNODOSEARCH;
typedef void *SEARCH;

void creasearch(SEARCH *l);
void destruyesearch(SEARCH *l);
TNODOSEARCH primerosearch(SEARCH l);
TNODOSEARCH finsearch(SEARCH l);
int esVaciasearch(SEARCH l);
void recuperasearch(SEARCH l, TNODOSEARCH p, LOC *e);
int longitudsearch(SEARCH l);
void insertasearch(SEARCH *l, TNODOSEARCH p, LOC e);
void suprimesearch(SEARCH *l, TNODOSEARCH p);
void modificasearch(SEARCH *l, TNODOSEARCH p, LOC e);
TNODOSEARCH siguientesearch(SEARCH l, TNODOSEARCH p);