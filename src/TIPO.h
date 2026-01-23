#ifndef TIPO_H_INCLUDED
#define TIPO_H_INCLUDED

#define MAX_PASSOS 100 

typedef struct {
    int matriz[3][3];
    int profundidade;
    int historico[MAX_PASSOS]; 
    int num_passos;
} Estado;

#endif // TIPO_H_INCLUDED