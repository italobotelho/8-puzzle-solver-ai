# 8-Puzzle Solver: Inteligência Artificial Clássica em C

Este projeto implementa um resolvedor para o jogo **8-puzzle** (também conhecido como Jogo dos Oito), utilizando algoritmos de busca em espaço de estados. O objetivo é levar uma configuração inicial de peças até um estado final (objetivo) realizando o menor número de movimentos possível.

## 🚀 Tecnologias e Conceitos Utilizados

* **Linguagem C:** Implementação de baixo nível com foco em performance e gerenciamento de memória.
* **Algoritmos de Busca:**
    * **Busca em Largura (BFS):** Garante a solução ótima (caminho mais curto).
    * **Busca em Profundidade Iterativa (IDDFS):** Combina a economia de memória da busca em profundidade com a completude da busca em largura.
* **Estruturas de Dados Dinâmicas:** Implementação manual de **Filas** e **Pilhas** para o gerenciamento da fronteira de busca.
* **Heurísticas:** Lógica para detecção de estados repetidos e otimização do processamento.

## 🧠 Como Funciona?

O resolvedor trata cada configuração do tabuleiro como um **nó em um grafo**. A partir do estado inicial, o algoritmo explora todas as possibilidades de movimentos (Cima, Baixo, Esquerda, Direita) até encontrar a sequência correta.

### Algoritmos Implementados:
1.  **BFS (Breadth-First Search):** Explora todos os nós de um nível antes de passar para o próximo. Ideal para encontrar a solução mais curta em tabuleiros simples.
2.  **IDDFS (Iterative Deepening Depth-First Search):** Realiza buscas em profundidade com limites sucessivos, sendo mais eficiente em termos de memória para estados mais profundos.

## 🛠️ Como Executar o Projeto

1.  Clone o repositório:
    ```bash
    git clone [https://github.com/italobotelho/8-puzzle-solver-ai.git](https://github.com/italobotelho/8-puzzle-solver-ai.git)
    ```
2.  Acesse a pasta `src`:
    ```bash
    cd 8-puzzle-solver-ai/src
    ```
3.  Compile o código (usando GCC):
    ```bash
    gcc *.c -o puzzle_solver
    ```
4.  Execute o programa:
    ```bash
    ./puzzle_solver
    ```

## 📊 Resultados e Performance

*(Sugestão: Insira aqui uma breve descrição de quanto tempo o algoritmo leva para resolver um caso médio ou quantos nós ele explora)*

---
Projeto desenvolvido como parte da graduação em **Ciência de Dados e Inteligência Artificial** na PUC-Campinas.
