#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "TIPO.h"

/* FUNÇÕES DE MANIPULAÇÃO DE FILA */

typedef struct nos_fila
{
    Estado info; 
    struct nos_fila *prox;
} NosFila;

typedef struct fila
{
    NosFila * ini;
    NosFila * fim;
} Fila;

int VaziaFila (Fila* f)
{
    if (f->ini == NULL) return 1;
    return 0;
}

Fila* CriaFila ()
{
    Fila* f = (Fila*) malloc(sizeof(Fila));
    f->ini = f->fim = NULL;
    return f;
}

NosFila* ins_fim (NosFila *fim, Estado A)
{
    NosFila *p = (NosFila*)malloc(sizeof(NosFila));
    p->info = A; 
    p->prox = NULL;
    if (fim != NULL) 
        fim->prox = p;
    return p;
}

void InsereFila (Fila* f, Estado v)
{
    f->fim = ins_fim(f->fim, v);
    if (f->ini == NULL) 
        f->ini = f->fim;
}

NosFila* retira_ini (NosFila* ini)
{
    NosFila* p = ini->prox;
    free(ini);
    return p;
}

Estado RetiraFila (Fila* f)
{
    Estado v;
    if (VaziaFila(f))
    {
        printf("Fila vazia.\n");
        exit(0);
    }
    v = f->ini->info;
    f->ini = retira_ini(f->ini);
    if (f->ini == NULL)
        f->fim = NULL;
    return v;
}

Fila* liberaFila (Fila* f)
{
    NosFila* q = f->ini;
    while (q != NULL)
    {
        NosFila* t = q->prox;
        free(q);
        q = t;
    }
    free(f);
    return NULL;
}

#endif // FILA_H_INCLUDED