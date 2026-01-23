#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TIPO.h"
#include "PILHA.h"
#include "FILA.h"
#include "buscas.h"
#include "FuncoesGerais.h"

// Função para exibir a animação da solução passo a passo
void AnimaSolucao(int matrizInicial[3][3], Estado e, int visitados, double tempoTotal) {
    int tabuleiroAnimacao[3][3];
    deepcopy(matrizInicial, tabuleiroAnimacao);

    limpar_tela();
    printf("Preparando replay da solucao em 3 segundos...\n");
    delay_ms(3000);

    limpar_tela();
    printf("Total estados analisados: %d\nTempo total: %.2f segundos\n\n", visitados, tempoTotal);
    printf("=== INICIO (Passo 0/%d) ===\n", e.num_passos);
    printEstado(tabuleiroAnimacao);
    delay_ms(800);

    int vazioX, vazioY;
    int movX, movY;

    for(int i = 0; i < e.num_passos; i++) {
        int pecaMover = e.historico[i];

        acha_vazio(tabuleiroAnimacao, &vazioX, &vazioY);
        acha_mov(tabuleiroAnimacao, &movX, &movY, pecaMover);

        muda_tabuleiro(tabuleiroAnimacao, vazioX, vazioY, movX, movY);

        limpar_tela();
        printf("Total estados analisados: %d\nTempo total: %.2f segundos\n\n", visitados, tempoTotal);
        printf("=== PASSO %d/%d (Moveu peca %d) ===\n", i+1, e.num_passos, pecaMover);
        printEstado(tabuleiroAnimacao);
        
        delay_ms(600); 
    }

    printf("\n\nSolucao concluida! Pressione ENTER para sair...");
    getchar();
}

// Algoritmo de Busca em Largura (BFS) usando Fila
void BuscaEmLargura(int matrizInicial[3][3]) {
    Fila* fila = CriaFila();
    
    Estado inicial;
    deepcopy(matrizInicial, inicial.matriz);
    inicial.profundidade = 0;
    inicial.num_passos = 0;

    InsereFila(fila, inicial); 

    int visitados = 0;
    clock_t inicio = clock();

    limpar_tela();
    printf("\nIniciando Busca em Largura (BFS)...\n");

    while (!VaziaFila(fila)) { 
        Estado atual = RetiraFila(fila); 
        visitados++;

        if (visitados % 200 == 0) {
            double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
            printf("\r[BFS] Analisados: %6d | Profundidade: %2d | Tempo: %.1fs", visitados, atual.profundidade, tempo);
        }
        
        if (verifica_solucao(atual.matriz, solucao)) {
            double tempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;
            printf("\nProcessamento finalizado em %.2f segundos!\n", tempoTotal);
            delay_ms(4000);
            AnimaSolucao(matrizInicial, atual, visitados, tempoTotal);
            liberaFila(fila);
            return;
        }

        int vazioX, vazioY, opcoes[4];
        acha_vazio(atual.matriz, &vazioX, &vazioY);
        acha_opcoes(atual.matriz, vazioX, vazioY, opcoes);

        for (int i = 0; i < 4; i++) {
            if (opcoes[i] != 0) {
                if (EhMovimentoReverso(atual, opcoes[i])) {
                    continue;
                }

                Estado novo = atual; 
                novo.profundidade = atual.profundidade + 1;
                
                if (novo.num_passos < MAX_PASSOS) {
                    novo.historico[novo.num_passos] = opcoes[i];
                    novo.num_passos++;
                }

                int movX, movY;
                acha_mov(novo.matriz, &movX, &movY, opcoes[i]);
                muda_tabuleiro(novo.matriz, vazioX, vazioY, movX, movY);

                InsereFila(fila, novo);
            }
        }
    }
    printf("\nSem solucao (Fila esvaziou).\n"); 
    liberaFila(fila);
}

// Algoritmo de Busca em Profundidade Limitada Iterativa (IDDFS) usando Pilha
void BuscaProfundidadeIterativa(int matrizInicial[3][3]) {
    int limite = 0;
    int MAX_PROFUNDIDADE_TOTAL = MAX_PASSOS;

    clock_t inicio = clock();

    while (limite <= MAX_PROFUNDIDADE_TOTAL) {
        limpar_tela();
        printf("\nIniciando Busca Iterativa (IDDFS)...\n");  
        Pilha* pilha = CriaPilha();
        Estado inicial;
        deepcopy(matrizInicial, inicial.matriz);
        inicial.profundidade = 0;
        inicial.num_passos = 0;

        push(pilha, inicial); 

        int visitados = 0;

        while (!vaziaPilha(pilha)) { 
            Estado atual = pop(pilha); 
            visitados++;

            if (visitados % 50 == 0) {
                double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
                printf("\r   [Nivel %d] Analisados: %5d | Tempo: %.1fs", limite, visitados, tempo);
            }

            if (verifica_solucao(atual.matriz, solucao)) {
                double tempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;
                printf("\n\nEncontrado no nivel %d em %.2f segundos!\n", limite, tempoTotal);
                delay_ms(4000); 
                AnimaSolucao(matrizInicial, atual, visitados, tempoTotal);
                liberaPilha(pilha);
                return;
            }

            if (atual.profundidade < limite) {
                int vazioX, vazioY, opcoes[4];
                acha_vazio(atual.matriz, &vazioX, &vazioY);
                acha_opcoes(atual.matriz, vazioX, vazioY, opcoes);

                for (int i = 0; i < 4; i++) { 
                    if (opcoes[i] != 0) {
                        if (EhMovimentoReverso(atual, opcoes[i])) {
                            continue;
                        }

                        Estado novo = atual;
                        novo.profundidade++;
                        
                        if (novo.num_passos < MAX_PASSOS) {
                            novo.historico[novo.num_passos] = opcoes[i];
                            novo.num_passos++;
                        }

                        int movX, movY;
                        acha_mov(novo.matriz, &movX, &movY, opcoes[i]);
                        muda_tabuleiro(novo.matriz, vazioX, vazioY, movX, movY);

                        push(pilha, novo);
                    }
                }
            }
        }
        liberaPilha(pilha);
        limite++; 
    }
    printf("\nLimite maximo atingido sem solucao.\n");
}