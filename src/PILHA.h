#ifndef PILHA_H_INCLUDED
#define PILHA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "TIPO.h"

/* FUNÇÕES DE MANIPULAÇÃO DE PILHA */

typedef struct no_pilha
{
    Estado info;
    struct no_pilha *prox;
} NoPilha;

typedef struct pilha
{
   NoPilha *Topo;
} Pilha;

int vaziaPilha(Pilha *p)
{
    if (p->Topo == NULL) return 1;
    return 0;
}

Pilha* CriaPilha (void)
{
     Pilha *p = (Pilha*)malloc(sizeof(Pilha));
     p->Topo = NULL;
     return p;
}

NoPilha* ins_ini (NoPilha* t, Estado a)
{
    NoPilha* aux = (NoPilha*) malloc(sizeof(NoPilha));
    aux->info = a;
    aux->prox = t;
    return aux;
}

void push (Pilha* p, Estado v)
{
    p->Topo = ins_ini(p->Topo, v);
}

NoPilha* ret_ini_pilha (NoPilha* aux)
{
    NoPilha* p = aux->prox;
    free(aux);
    return p;
}

Estado pop (Pilha *p)
{
    Estado v;
    if (vaziaPilha(p))
    {
        printf("\n\n\t==> Pilha VAZIA.\b\n");
        exit(1);
    }
    v = p->Topo->info;
    p->Topo = ret_ini_pilha(p->Topo);
    return v;
}

Pilha* liberaPilha (Pilha *p)
{
    NoPilha *q = p->Topo;
    while (q != NULL)
    {
        NoPilha* t = q->prox;
        free(q);
        q = t;
    }
    free(p);
    return(NULL);
}

#endif // PILHA_H_INCLUDED