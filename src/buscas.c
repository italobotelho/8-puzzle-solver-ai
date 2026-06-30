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
    printf(BOLD YELLOW ">>> SOLUTION REPLAY <<<\n" RESET);
    printf("States analyzed: " CYAN "%d" RESET " | Time: " GREEN "%.2fs\n\n" RESET, visitados, tempoTotal);
    printf(BOLD BLUE "=== START (Step 0/%d) ===\n" RESET, e.num_passos);
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
        printf(BOLD YELLOW ">>> SOLUTION REPLAY <<<\n" RESET);
        printf("States analyzed: " CYAN "%d" RESET " | Time: " GREEN "%.2fs\n\n" RESET, visitados, tempoTotal);
        printf(BOLD BLUE "=== STEP %d/%d (Tile %d) ===\n" RESET, i+1, e.num_passos, pecaMover);
        printEstado(tabuleiroAnimacao);
        delay_ms(600);
    }

    printf("\n\nSolution completed! Press ENTER to exit...\n");
    fflush(stdout);
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
    printf("\nStarting Breadth-First Search (BFS)...\n");

    while (!VaziaFila(fila)) { 
        Estado atual = RetiraFila(fila); 
        visitados++;

        if (visitados % 200 == 0) {
            double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
            printf("\r" BOLD GREEN "[BFS]" RESET " Analyzed: " CYAN "%6d" RESET " | Depth: " YELLOW "%2d" RESET " | Time: %.1fs", visitados, atual.profundidade, tempo);
            #if defined(__EMSCRIPTEN__)
            printf("\n");
            #endif
            fflush(stdout);
            delay_ms(0);
        }
        
        if (verifica_solucao(atual.matriz, solucao)) {
            double tempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;
            printf("\nProcessing finished in %.2f seconds!\n", tempoTotal);
            printf(YELLOW "\nPress ENTER to watch the solution replay...\n" RESET);
            fflush(stdout);
            #if defined(_WIN32) || defined(WIN32) || defined(__EMSCRIPTEN__)
                _getch();
            #else
                getchar();
            #endif
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
    printf("\nNo solution (Queue emptied).\n"); 
    liberaFila(fila);
}

// Algoritmo de Busca em Profundidade Limitada Iterativa (IDDFS) usando Pilha
void BuscaProfundidadeIterativa(int matrizInicial[3][3]) {
    int limite = 0;
    int MAX_PROFUNDIDADE_TOTAL = MAX_PASSOS;

    clock_t inicio = clock();

    while (limite <= MAX_PROFUNDIDADE_TOTAL) {
        limpar_tela();
        printf("\nStarting Iterative Deepening (IDDFS)...\n");  
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

            if (visitados % 200 == 0) {
                double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
                printf("\r   " BOLD YELLOW "[Level %d]" RESET " Analyzed: " CYAN "%5d" RESET " | Time: %.1fs", limite, visitados, tempo);
                #if defined(__EMSCRIPTEN__)
                printf("\n");
                #endif
                fflush(stdout);
                delay_ms(0);
            }

            if (verifica_solucao(atual.matriz, solucao)) {
                double tempoTotal = (double)(clock() - inicio) / CLOCKS_PER_SEC;
                printf("\n\nFound at level %d in %.2f seconds!\n", limite, tempoTotal);
                printf(YELLOW "\nPress ENTER to watch the solution replay...\n" RESET);
                fflush(stdout);
                #if defined(_WIN32) || defined(WIN32) || defined(__EMSCRIPTEN__)
                    _getch();
                #else
                    getchar();
                #endif
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
