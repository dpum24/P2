#ifndef AYUDA_P3
#define AYUDA_P3

int BuscarVariable (char * var, char *e[]);
void Cmd_showvar(int count, char *vars[], char *envp[]);
void Cmd_fork (char *tr[]);
int CambiarVariable(char * var, char * valor, char *e[]);
char * Ejecutable (char *s);
int ExecWithPriority(char *tr[]);

#endif