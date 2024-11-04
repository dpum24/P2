#ifndef AYUDAP2_H
#define AYUDAP2_H

void ImprimirMemoriaLista(MEM *shared);
void LiberarMemoriaLista(MEM lista);
void Recursiva (int n);
void LlenarMemoria (void *p, size_t cont, unsigned char byte);
void * ObtenerMemoriaShmget (key_t clave, size_t tam,MEM *shared);
void do_AllocateCreateshared (char *tr[],MEM shared);
void do_AllocateShared (char *tr[],MEM shared);
void * MapearFichero (char * fichero, int protection,MEM *list);
void do_AllocateMmap(char *arg[],MEM mmap);
void do_DeallocateDelkey (char *args[]);
ssize_t LeerFichero (char *f, void *p, size_t cont);
void Cmd_ReadFile (char *ar[]);
void Do_pmap (void);
void *cadtop(char *cadena);

#endif