// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 
 * Estrutura: Territorio
 * Armazena as informações principais do território no mapa do jogo.
 */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* 
 * Função: atribuirMissao
 */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

/*
 * Função: exibirMissao
 */
void exibirMissao(char *missao) {
    printf("\nSua missão: %s\n", missao);
}

/*
 * Função: atacar
 * Objetivo: simular um ataque entre dois territórios com base em sorteio de dados.
 */
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nVocê não pode atacar um território do mesmo exército!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nRolagem de dados: atacante [%d] vs defensor [%d]\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Ataque bem-sucedido! %s conquistou %s!\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("O ataque falhou! O atacante perdeu uma tropa.\n");
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/*
 * Função: verificarMissao
 * Objetivo: verificar se a missão do jogador foi cumprida.
 */
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    if (strstr(missao, "Conquistar 3 territórios")) {
        int consecutivos = 0;
        for (int i = 0; i < tamanho - 1; i++) {
            if (strcmp(mapa[i].cor, mapa[i + 1].cor) == 0) {
                consecutivos++;
                if (consecutivos >= 2) return 1; // missão cumprida
            } else {
                consecutivos = 0;
            }
        }
    }

    if (strstr(missao, "Eliminar todas as tropas vermelhas")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0 && mapa[i].tropas > 0)
                return 0; // ainda há tropas vermelhas
        }
        return 1; // todas eliminadas
    }

    // Outras missões poderiam ser adicionadas aqui.
    return 0; // missão não cumprida ainda
}

/*
 * Função: exibirMapa
 */
void exibirMapa(Territorio *mapa, int tamanho) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/*
 * Função: liberarMemoria
 */
void liberarMemoria(Territorio *mapa, char *missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

/*
 * Função principal: main
 */
int main() {
    srand(time(NULL));

    // Definição das missões pré-estabelecidas
    char *missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas vermelhas",
        "Controlar pelo menos 4 territórios",
        "Manter 2 territórios com mais de 10 tropas cada",
        "Conquistar um território azul"
    };
    int totalMissoes = 5;

    // Aloca dinamicamente o mapa de territórios
    int totalTerritorios = 5;
    Territorio *mapa = (Territorio *) malloc(totalTerritorios * sizeof(Territorio));

    // Inicialização de exemplo dos territórios
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "vermelha");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "azul");
    mapa[1].tropas = 4;

    strcpy(mapa[2].nome, "Chile");
    strcpy(mapa[2].cor, "verde");
    mapa[2].tropas = 3;

    strcpy(mapa[3].nome, "Peru");
    strcpy(mapa[3].cor, "vermelha");
    mapa[3].tropas = 2;

    strcpy(mapa[4].nome, "Uruguai");
    strcpy(mapa[4].cor, "azul");
    mapa[4].tropas = 6;

    // Atribui dinamicamente a missão do jogador
    char *missaoJogador = (char *) malloc(100 * sizeof(char));
    atribuirMissao(missaoJogador, missoes, totalMissoes);

    printf("Bem-vindo ao Sistema de Missões Estratégicas!\n");
    exibirMissao(missaoJogador);
    exibirMapa(mapa, totalTerritorios);

    // Simulação simples de rodadas
    for (int turno = 1; turno <= 5; turno++) {
        printf("\n===== TURNO %d =====\n", turno);

        // Simula um ataque aleatório
        int a = rand() % totalTerritorios;
        int d = rand() % totalTerritorios;
        while (d == a) d = rand() % totalTerritorios;

        atacar(&mapa[a], &mapa[d]);
        exibirMapa(mapa, totalTerritorios);

        if (verificarMissao(missaoJogador, mapa, totalTerritorios)) {
            printf("\nMissão cumprida! Você venceu o jogo!\n");
            break;
        }
    }

    liberarMemoria(mapa, missaoJogador);
    printf("\nMemória liberada e jogo encerrado.\n");

    return 0;
}
