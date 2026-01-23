#ifndef FuncoesGerais_H
#define FuncoesGerais_H

// Definições de Cores ANSI para o Terminal
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"

#include "TIPO.h"

// --- FUNÇÕES DE UTILIDADE ---

// Função para pausar a execução por X milissegundos (Cross-platform)
void delay_ms(int ms);

// Função para limpar a tela (Cross-platform)
void limpar_tela();

// Função para interagir com o menu principal
int MenuPrincipalInterativo();

// Função para interagir com o menu da busca IA
int MenuIAInterativo();

// Função para interagir com o jogo manual
int MenuMovimentoInterativo(int *opcoes, int estado[3][3], int solucao[3][3]);

// --- FUNÇÕES DO TABULEIRO ---

// Gera o tabuleiro de forma aleatória
void gera_tabuleiro(int estado[3][3]);

// Gera e garante que é solúvel (loop interno)
void GeraTabuleiroValido(int estado[3][3]);

// Imprime a variavel estado, que é uma matriz
void printEstado(int estado[3][3]);

// Copia o conteudo da matriz estado para a matriz copiaEstado
void deepcopy(int estado[3][3], int copiaEstado[3][3]);

//acha a posição do espaço vazio
void acha_vazio(int matriz[3][3], int *x, int *y);

// acha os índices do numero que o usuário quer mover
void acha_mov(int matriz[3][3], int *x, int *y, int c);

// acha as opções de ações do usuário
void acha_opcoes(int m[3][3], int x, int y, int *V);

// muda a posicao de dois lugares do tabuleiro
void muda_tabuleiro(int estado[3][3], int vazio_x, int vazio_y, int mov_x, int mov_y);

// funcao que verifica se chegou na solução do 8-puzzle
int verifica_solucao(int estado[3][3], int solucao[3][3]);

//função que verifica se o numero que o usuario escolheu é uma das opções
int verifica_opcoes(int movimento, int *V);

// 
int verifica_solubilidade(int estado[3][3]);

// Função auxiliar para verificar se o movimento é redundante (volta pro estado anterior)
int EhMovimentoReverso(Estado e, int movimentoProposto);

// --- VARIÁVEL GLOBAL ---

// Solução declarada uma única vez
extern int solucao[3][3];


#endif // FuncoesGerais_H
