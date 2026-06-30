#include <stdio.h>
#include <stdlib.h>
#include <time.h> // chamada para usar rand

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define CLEAR_SCREEN EM_ASM({ var el = document.getElementById('terminal'); if(el) el.innerHTML = ''; });
#else
#include <windows.h> // chamada para usar o clear do terminal
#define CLEAR_SCREEN system("cls")
#endif

#define tam 5 // tamanho do grid do labirinto 5x5

// Função que controla o tempo de impressão dos estados
void temporizador(int segundos) {
    while (segundos > 0) {
        fflush(stdout);
#ifdef __EMSCRIPTEN__
        emscripten_sleep(1000);
#else
        Sleep(1000); // Pausa por 1000 milissegundos (1 segundo)
#endif
        segundos--;
    }
}

// Função com um preset de layout
void layout()
{
    printf("-------------------------------------------------------\n");
}

// Função que 'seta' o estado padrão e inicial do labirinto
void inicializa_labirinto(char labirinto[tam][tam])
{
    for(int i=0; i<tam; i++)
    {
        for(int j=0; j<tam; j++)
        {
            labirinto[i][j] = '.';
        }
    }

    // Posições das paredes e objetivo (nao mudam durante o jogo)
    labirinto[1][1] = labirinto[2][1] = labirinto[4][1] = labirinto[0][3] = labirinto[1][3] = labirinto[3][3] = '#';
    labirinto[3][4] = 'G';
}

// Função que vai imprimir o estado do labirinto após o movimento do robô 'S'
void renderiza_labirinto(char labirinto[tam][tam], int linha_robo, int coluna_robo)
{
    char celula_anterior = labirinto[linha_robo][coluna_robo];
    labirinto[linha_robo][coluna_robo] = 'S';

    for(int i=0; i<tam; i++)
    {
        printf("\t");

        for(int j=0; j<tam; j++)
        {
            printf("  ");
            printf("%c\t", labirinto[i][j]);
        }
        printf("\n");
    }

    labirinto[linha_robo][coluna_robo] = celula_anterior;
}

// Função que calcula a heuristica através do cálculo da Distancia de Manhattan
void heuristica(int linha_robo, int coluna_robo)
{
    int distancia = abs(linha_robo - 3) + abs(coluna_robo - 4);
    
    printf("       Distancia de Manhattan (ignora paredes): %d\n", distancia);
}

// Função que altera a posicao atual do robô 'S' aplicando a decisão do usuario
void mover_robo_manual(char movimento, int *linha_ptr, int *coluna_ptr, char labirinto[tam][tam])
{
    int proxima_linha = *linha_ptr;
    int proxima_coluna = *coluna_ptr;

    // case que vai executar o movimento do robô 'S' de acordo com a escolha do user
    switch(movimento)
    {
        case 'w': case 'W': proxima_linha--; break;
        case 'a': case 'A': proxima_coluna--; break;
        case 's': case 'S': proxima_linha++; break;
        case 'd': case 'D': proxima_coluna++; break;
        default: return;
    }

    // verifica se a proxima celula é uma barreira '#' e se está dentro das limitações do labirinto 5x5
    if (proxima_linha >= 0 && proxima_linha < tam && proxima_coluna >= 0 && proxima_coluna < tam)
    {
        if (labirinto[proxima_linha][proxima_coluna] != '#')
        {
            *linha_ptr = proxima_linha;
            *coluna_ptr = proxima_coluna;
        }
    }
}

// Função que calcula a heuristica do proximo movimento possivel e altera a posicao atual do robô 'S' aplicando a busca gulosa
void mover_robo_guloso(int *linha_ptr, int *coluna_ptr, char labirinto[tam][tam])
{
    int proxima_linha[4], proxima_coluna[4], heuristicas[4], melhores_indices[4];
    int count = 0;
    int menor_heuristica = tam * 2;

    int linha = *linha_ptr;
    int coluna = *coluna_ptr;

    // Cima
    proxima_linha[0] = linha - 1;
    proxima_coluna[0] = coluna;
    heuristicas[0] = abs(proxima_linha[0] - 3) + abs(proxima_coluna[0] - 4);

    // Baixo
    proxima_linha[1] = linha + 1;
    proxima_coluna[1] = coluna;
    heuristicas[1] = abs(proxima_linha[1] - 3) + abs(proxima_coluna[1] - 4);

    // Esquerda
    proxima_linha[2] = linha;
    proxima_coluna[2] = coluna - 1;
    heuristicas[2] = abs(proxima_linha[2] - 3) + abs(proxima_coluna[2] - 4);

    // Direita
    proxima_linha[3] = linha;
    proxima_coluna[3] = coluna + 1;
    heuristicas[3] = abs(proxima_linha[3] - 3) + abs(proxima_coluna[3] - 4);

    // Verifica movimentos válidos e encontra menor heurística
    for (int i = 0; i < 4; i++)
    {
        // Primeira validação - Garante que o proximo movimento esteja dentro do labirinto e não permite que obstaculos sejam visualizados como caminhos a serem percorridos pelo robô
        if (proxima_linha[i] >= 0 && proxima_linha[i] < tam && proxima_coluna[i] >= 0 && proxima_coluna[i] < tam && labirinto[proxima_linha[i]][proxima_coluna[i]] != '#')
        {
            if (heuristicas[i] < menor_heuristica)
            {
                menor_heuristica = heuristicas[i];
                count = 0;
                melhores_indices[count++] = i;
            }
            else if (heuristicas[i] == menor_heuristica)
            {
                melhores_indices[count++] = i;
            }
        }
    }

    // Escolhe aleatoriamente entre os melhores movimentos
    if (count > 0)
    {
        int escolhido = melhores_indices[rand() % count];

        *linha_ptr = proxima_linha[escolhido];
        *coluna_ptr = proxima_coluna[escolhido];
    }
}

// Função apenas de impressão da tela inicial do 2d_game com as duas opçoes de jogo: manual e busca gulosa
void tela_inicial()
{
    CLEAR_SCREEN;

    printf("--------------- Movimento em Labirinto! ---------------\n");
    layout();
    printf("              Selecione um modo de jogo:\n\n");
    printf("    1. Controlar os movimentos do robo\n");
    printf("    2. Robo se movimenta sozinho (Busca Gulosa)\n\n");
}

// Função que exige INPUT do usuário (opcoes)
int opcoes_user()
{
    int a;

    do
    {
        printf("Digite uma das opcoes (1 ou 2) => ");
        fflush(stdout);

#ifdef __EMSCRIPTEN__
        char k = 0;
        while(k == 0) {
            k = (char)EM_ASM_INT({
                if (window.keyBuffer && window.keyBuffer.length > 0) {
                    return window.keyBuffer.shift().charCodeAt(0);
                }
                return 0;
            });
            if(k == 0) emscripten_sleep(50);
        }
        a = k - '0';
        printf("%c\n", k); // Echo da tecla
#else
        scanf("%d", &a);
#endif
    } while (a != 1 && a != 2);

    return a;
}

// Função main - principal
int main()
{
    srand(time(NULL)); // gera numero aleatorios

    char labirinto[tam][tam], movimento;
    int linha = 0, coluna = 0, modo_jogo, opcao;

    inicializa_labirinto(labirinto);

    do
    {
        tela_inicial();
        
        modo_jogo = opcoes_user();

        while(1)
        {       
            CLEAR_SCREEN; // limpa campos

            layout();
            heuristica(linha, coluna);
            layout();
            renderiza_labirinto(labirinto, linha, coluna);

            if(linha == 3 && coluna == 4)
            {
                layout();
                printf("              Parabens! Objetivo atingido!\n");
                layout();
                break;
            }

            if(modo_jogo == 1)
            {
                layout();
                printf("           Movimente [S] para chegar em [G]\n");
                printf("            Mova usando WASD (Q para sair)\n");
                layout();

                printf("===> ");
                fflush(stdout);

#ifdef __EMSCRIPTEN__
                char k = 0;
                while(k == 0) {
                    k = (char)EM_ASM_INT({
                        if (window.keyBuffer && window.keyBuffer.length > 0) {
                            return window.keyBuffer.shift().charCodeAt(0);
                        }
                        return 0;
                    });
                    if(k == 0) emscripten_sleep(50);
                }
                movimento = k;
                printf("%c\n", k); // Echo da tecla
#else
                scanf(" %c", &movimento);
#endif

                if(movimento == 'Q' || movimento == 'q')
                {
                    layout();
                    break;
                }
            
                mover_robo_manual(movimento, &linha, &coluna, labirinto);
            }
            else
            {
                layout();
                printf("               Busca Gulosa de robo [S]\n");
                layout();

                mover_robo_guloso(&linha, &coluna, labirinto);

                temporizador(2);
            }
        }

        printf("                    Jogar de novo?\n");
        printf("                  1. Sim  |  2. Nao\n");
        layout();

        opcao = opcoes_user();
        
        if(opcao == 2)
        {
            break;
        }
        else
        {
            linha = 0;
            coluna = 0;
        }

    }while(1);

    layout();
    printf("             - - Execucao encerrada! - -\n");
    layout();

    return 0;
}
