#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FuncoesGerais.h"

#if defined(_WIN32) || defined(WIN32)
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
#endif

// --- IMPLEMENTAÇÃO DA VARIÁVEL GLOBAL ---

int solucao[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE UTILIDADE ---

// Função para pausar a execução por X milissegundos (Cross-platform)
void delay_ms(int ms) {
    #if defined(_WIN32) || defined(WIN32)
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif 
}

// Função para limpar a tela (Cross-platform)
void limpar_tela() {
    #if defined(_WIN32) || defined(WIN32)
        system("cls");
    #else
        system("clear");
    #endif
}

// --- FUNÇÕES DO TABULEIRO ---

// Gera o tabuleiro de forma aleatória
void gera_tabuleiro(int estado[3][3])
{
    int posicoes[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int verifica[9], num_rand;

    for (int count = 0; count < 9; count++)
    {
        do 
        {
            num_rand = rand() % 9;

        } while (posicoes[num_rand] == -1);

        verifica[count] = posicoes[num_rand];
        posicoes[num_rand] = -1;
    }

    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            estado[i][j] = verifica[k];
            k++;
        }
    }
}

// Função wrapper que garante tabuleiro válido
void GeraTabuleiroValido(int estado[3][3]) {
    do {
        gera_tabuleiro(estado);
    } while (!verifica_solubilidade(estado));
}

//Imprime a variavel estado, que é uma matriz
void printEstado(int estado[3][3])
{
    printf("---------------------------------\n");
    for(int i=0; i<3; i++)
    {
        printf("|\t");
        for(int j=0; j<3; j++)
        {
            if (estado[i][j] == 0)
            {
                printf("X\t");
            }
            else
            {
                printf("%d\t",estado[i][j]);
            }
        }
        printf("|\n");
    }
    printf("---------------------------------\n");
    return;
}

//Copia o conteudo da matriz estado para a matriz copiaEstado
void deepcopy(int estado[3][3], int copiaEstado[3][3]){
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            copiaEstado[i][j] = estado[i][j];
        }
    }
    return;
}

// acha os índeces de onde está o vazio
void acha_vazio(int matriz[3][3], int *x, int *y){
    acha_mov(matriz, x, y, 0);
}

// acha os índices do numero que o usuário quer mover
void acha_mov(int matriz[3][3], int *x, int *y, int c){
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if (matriz[i][j] == c){
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

// acha as opções de ações do usuário
void acha_opcoes(int m[3][3], int x, int y, int *V){
        int o = 0;

        if (x > 0) {
            V[o] = m[x - 1][y];
            o++;
        }
        if (x < 2) {
            V[o] = m[x + 1][y];
            o++;
        }
        if (y > 0) {
            V[o] = m[x][y - 1];
            o++;
        }
        if (y < 2) {
            V[o] = m[x][y + 1];
            o++;
        }
        for (int k = o; k < 4; k++){
            V[k] = 0;
        }
}


//função que verifica se o numero que o usuario escolheu é uma das opções
int verifica_opcoes(int movimento, int *V){
    for (int i = 0; i < 4; i++){
        if (V[i] != 0){
            if (V[i] == movimento){
                return 1;
            }
        }
    }
    return 0;
}

// muda a posicao de dois lugares do tabuleiro
void muda_tabuleiro(int estado[3][3], int vazio_x, int vazio_y, int mov_x, int mov_y){
    int valor_mov;
    valor_mov = estado[mov_x][mov_y];

    estado[mov_x][mov_y] = 0;

    estado[vazio_x][vazio_y] = valor_mov;
}


// funcao que verifica se chegou na solução do 8-puzzle
int verifica_solucao(int estado[3][3], int solucao[3][3]){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j <3; j++){
            if (estado[i][j] != solucao[i][j]){
                return 0;
            }
        }
    }
    return 1;
}

// função que verifica a paridade das inversões para garantir solubilidade
int verifica_solubilidade(int estado[3][3]) {
    int lista[9];
    int k = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado[i][j] != 0) {
                lista[k] = estado[i][j];
                k++;
            }
        }
    }

    int inversoes = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 8; j++) {
            if (lista[i] > lista[j]) {
                inversoes++;
            }
        }
    }

    return (inversoes % 2 == 0);
}

// Função auxiliar para verificar se o movimento é redundante (volta pro estado anterior)
int EhMovimentoReverso(Estado e, int movimentoProposto) {
    if (e.num_passos > 0) {
        if (movimentoProposto == e.historico[e.num_passos - 1]) {
            return 1;
        }
    }
    return 0;
}

// --- MENUS ---

int MenuPrincipalInterativo() {
    #if defined(_WIN32) || defined(WIN32)
    int posicao = 0; 
    while (1) {
        limpar_tela();
        printf("=== 8-PUZZLE SOLVER ===\n\n");
        printf("%s 1. Modo Jogador (Manual)\n", (posicao == 0) ? "->" : "  ");
        printf("%s 2. Modo IA (Automatico)\n", (posicao == 1) ? "->" : "  ");
        printf("%s 0. Sair\n", (posicao == 2) ? "->" : "  ");
        
        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72) { posicao--; if(posicao < 0) posicao = 2; }
            if (tecla == 80) { posicao++; if(posicao > 2) posicao = 0; }
        } else if (tecla == 13) {
            return (posicao == 0) ? 1 : (posicao == 1) ? 2 : 0;
        } else if (tecla >= '0' && tecla <= '2') return tecla - '0';
    }
    #else
    int option; printf("1=Jogar, 2=IA, 0=Sair: "); scanf("%d", &option); return option;
    #endif
}

int MenuIAInterativo() {
    #if defined(_WIN32) || defined(WIN32)
    int posicao = 0; 
    while (1) {
        limpar_tela();
        printf("\n--- MODO IA ---\n\n");
        printf("%s 1. Busca em Largura (BFS)\n", (posicao == 0) ? "->" : "  ");
        printf("%s 2. Busca Profundidade (IDDFS)\n", (posicao == 1) ? "->" : "  ");

        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72) { posicao--; if(posicao < 0) posicao = 1; }
            if (tecla == 80) { posicao++; if(posicao > 1) posicao = 0; }
        } else if (tecla == 13) {
            return (posicao == 0) ? 1 : 2;
        } else if (tecla == '1' || tecla == '2') return tecla - '0';
    }
    #else
    int op; printf("1=BFS, 2=IDDFS: "); scanf("%d", &op); return op;
    #endif
}

int MenuMovimentoInterativo(int *opcoes, int estado[3][3], int solucao[3][3]) {
    #if defined(_WIN32) || defined(WIN32)
    int validos[4], qtd=0;
    for(int i=0; i<4; i++) if(opcoes[i]!=0) validos[qtd++] = opcoes[i];

    limpar_tela();
    printf("=== MODO JOGADOR ===\n\n=== OBJETIVO ===\n");
    printEstado(solucao);
    printf("\n=== SEU TABULEIRO ===\n");
    printEstado(estado);
    printf("\nMover peca: ");
    for(int i=0; i<qtd; i++) printf("[%d] ", validos[i]);
    printf("\n");

    while(1) {
        int tecla = _getch();
        int num = tecla - '0';
        if (verifica_opcoes(num, opcoes)) return num;
    }
    #else
    int mov; printf("Mover: "); scanf("%d", &mov); return mov;
    #endif
}