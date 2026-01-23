#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 
#include "FuncoesGerais.h"
#include "buscas.h"

int main()
{
    srand(time(NULL));

    int jogar_novamente = 1;

    do {
        limpar_tela();

        int option = 0;
        int estado[3][3];

        option = MenuPrincipalInterativo();

        if (option == 0) {
            break;
        }

        switch (option)
        {
            case 1:
            {
                GeraTabuleiroValido(estado);
                
                while (true){
                    int vazio_posx = 0, vazio_posy = 0, opcoes[4];
                    
                    acha_vazio(estado, &vazio_posx, &vazio_posy);
                    acha_opcoes(estado, vazio_posx, vazio_posy, opcoes);
                    int movimento = MenuMovimentoInterativo(opcoes, estado, solucao);
                    
                    int movimento_posx = 0, movimento_posy = 0;
                    acha_mov(estado, &movimento_posx, &movimento_posy, movimento);
                    muda_tabuleiro(estado, vazio_posx, vazio_posy, movimento_posx, movimento_posy);
                    
                    if (verifica_solucao(estado, solucao)){
                        limpar_tela();
                        printf("\n=== OBJETIVO ===\n");
                        printEstado(solucao);
                        printf("\n=== SEU TABULEIRO ===\n");
                        printEstado(estado);
                        
                        printf("\n\n*********************************************");
                        printf("\n* PARABENS!! VOCE COMPLETOU O 8-PUZZLE!! *");
                        printf("\n*********************************************\n");
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                int option_ia = MenuIAInterativo();

                GeraTabuleiroValido(estado); 
                
                limpar_tela();
                printf("=== OBJETIVO (ALVO DA IA) ===\n");
                printEstado(solucao);
                
                printf("\n=== TABULEIRO INICIAL GERADO (VALIDO) ===\n");
                printEstado(estado);

                printf("\nPressione ENTER para iniciar a busca da IA...");
                getchar();
                getchar();

                switch (option_ia)
                {
                    case 1:
                        BuscaEmLargura(estado); 
                        break;

                    case 2:
                        BuscaProfundidadeIterativa(estado); 
                        break;

                    default:
                        printf("Opcao invalida.\n");
                        break;
                }
                break;
            }
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }

        printf("\n------------------------------------------------");
        printf("\nDeseja jogar novamente? (1 = Sim / 0 = Nao): ");
        scanf("%d", &jogar_novamente);

    } while (jogar_novamente == 1);

    printf("\nObrigado por jogar! Encerrando...\n");
    return 0;
}