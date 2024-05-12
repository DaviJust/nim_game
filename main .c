#include <stdio.h>

struct JogoInfo {
    int pontuacao;
    int max_peca;
    int max_retirar;
};

struct JogoInfo inicializarJogo() {
    struct JogoInfo info;
    info.pontuacao = 0;
    info.max_peca = 0;
    info.max_retirar = 0;
    return info;
}

void exibirJogoInfo(struct JogoInfo info) {
    printf("Pontuação: %d\n", info.pontuacao);
    printf("Máximo de peças no tabuleiro: %d\n", info.max_peca);
    printf("Máximo de peças a serem retiradas por jogada: %d\n", info.max_retirar);
}

int computadorEscolheJogada(struct JogoInfo info, int n) {
    int remove_computador = 1;

    while (remove_computador != info.max_retirar) {
        if ((n - remove_computador) % (info.max_retirar + 1) == 0) {
            return remove_computador;
        } else {
            remove_computador++;
        }
    }

    return remove_computador;
}

int usuarioEscolheJogada(struct JogoInfo info, int n) {
    int remove_jogador;
    int jogada_valida = 0;

    while (!jogada_valida) {
        printf("Quantas peças você vai tirar (1 a %d)? ", info.max_retirar);
        scanf("%d", &remove_jogador);
        if (remove_jogador < 1 || remove_jogador > info.max_retirar || remove_jogador > n) {
            printf("Jogada inválida! Tente novamente.\n");
        } else {
            jogada_valida = 1;
        }
    }

    return remove_jogador;
}

void partida(struct JogoInfo info) {
    int n = info.max_peca;
    int vez_do_pc = 0;

    printf("\nComeçando uma nova partida...\n");
    printf("Número inicial de peças no tabuleiro: %d\n", n);

    while (n > 0) {
        if (vez_do_pc) {
            int remove_computador = computadorEscolheJogada(info, n);
            printf("O computador removeu %d peça(s).\n", remove_computador);
            n -= remove_computador;
            vez_do_pc = 0;
        } else {
            int remove_jogador = usuarioEscolheJogada(info, n);
            printf("Você removeu %d peça(s).\n", remove_jogador);
            n -= remove_jogador;
            vez_do_pc = 1;
        }

        if (n > 0) {
            printf("Agora restam %d peças no tabuleiro.\n", n);
        }
    }

    printf("\nFim do jogo! %s ganhou!\n", vez_do_pc ? "Você" : "O computador");
}

int main() {
    struct JogoInfo info;

    printf("Bem-vindo ao jogo do NIM!\n");

    printf("Digite o número máximo de peças no tabuleiro: ");
    scanf("%d", &info.max_peca);

    printf("Digite o número máximo de peças a serem retiradas por jogada: ");
    scanf("%d", &info.max_retirar);

    info.pontuacao = 0;

    printf("\nConfigurações do jogo:\n");
    exibirJogoInfo(info);

    partida(info);

    return 0;
}
