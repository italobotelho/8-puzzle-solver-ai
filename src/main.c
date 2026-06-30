#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "FuncoesGerais.h"
#include "buscas.h"

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif

int main()
{
    srand(time(NULL));
    int jogar_novamente = 1;

    do {
        int option = MenuPrincipalInterativo();

        if (option == 0) {
            break;
        }

        switch (option)
        {
            case 1: // MODO JOGADOR
            {
                int estado[3][3];
                GeraTabuleiroValido(estado);
                
                while (true) {
                    int vazio_posx = 0, vazio_posy = 0, opcoes[4];
                    acha_vazio(estado, &vazio_posx, &vazio_posy);
                    acha_opcoes(estado, vazio_posx, vazio_posy, opcoes);
                    
                    int movimento = MenuMovimentoInterativo(opcoes, estado, solucao);
                    
                    if (movimento == 0) {
                        break; 
                    }
                    
                    int movimento_posx = 0, movimento_posy = 0;
                    acha_mov(estado, &movimento_posx, &movimento_posy, movimento);
                    muda_tabuleiro(estado, vazio_posx, vazio_posy, movimento_posx, movimento_posy);
                    
                    if (verifica_solucao(estado, solucao)) {
                        limpar_tela();
                        printf(GREEN BOLD "\n*********************************************");
                        printf("\n* PARABENS!! VOCE COMPLETOU O 8-PUZZLE!! *");
                        printf("\n*********************************************\n" RESET);
                        printEstado(estado);
                        printf("\nPressione qualquer tecla para continuar...");
                        fflush(stdout);
                        #if defined(_WIN32) || defined(__EMSCRIPTEN__)
                            _getch();
                        #else
                            getchar(); getchar();
                        #endif
                        break;
                    }
                }
                break;
            }

            case 2: // MODO IA
            {
                int option_ia = MenuIAInterativo();

                if (option_ia == 3) {
                    break; 
                }

                int estado[3][3];
                GeraTabuleiroValido(estado); 
                
                limpar_tela();
                printf(CYAN "=== TABULEIRO INICIAL (IA) ===\n" RESET);
                printEstado(estado);
                printf(BLUE "\nOBJETIVO:\n" RESET);
                printEstado(solucao);

                printf("\nPressione ENTER para iniciar a busca da IA...");
                fflush(stdout);
                #if defined(__EMSCRIPTEN__)
                _getch();
                #else
                // Limpeza de buffer para o getchar não passar direto
                while (getchar() != '\n'); 
                getchar();
                #endif

                switch (option_ia)
                {
                    case 1:
                        BuscaEmLargura(estado); 
                        break;
                    case 2:
                        BuscaProfundidadeIterativa(estado); 
                        break;
                }
                
                #if defined(_WIN32) || defined(__EMSCRIPTEN__)
                    _getch();
                #else
                    getchar();
                #endif
                break;
            }

            default:
                printf(RED "Opcao invalida. Tente novamente.\n" RESET);
                delay_ms(1000);
                break;
        }

        // Se o usuário não saiu pelo Menu Principal (opcao 0), perguntamos se quer jogar de novo
        if (option != 0) {
            limpar_tela();
            printf(BOLD "\n------------------------------------------------");
            printf("\nDeseja retornar ao Menu Principal? (1 = Sim / 0 = Sair): ");
            printf("\n------------------------------------------------\n>> ");
            #if defined(__EMSCRIPTEN__)
            fflush(stdout);
            int k = _getch();
            jogar_novamente = k - '0';
            #else
            if (scanf("%d", &jogar_novamente) != 1) {
                jogar_novamente = 0;
            }
            #endif
        }

    } while (jogar_novamente == 1);

    printf(CYAN "\nObrigado por jogar! Encerrando...\n" RESET);
    return 0;
}
