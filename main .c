#include <stdio.h>

struct JogoInfo {
  int pontuacao;
  int max_peca;
  int max_retirar;
};

void inicializarPeca(struct JogoInfo *info) {
  info->pontuacao = 0;
  info->max_peca = 0;
  info->max_retirar = 0;
}

void exibirJogoInfo(const struct JogoInfo *info) {
  printf("Pontuação: %d\n", info->pontuacao);
  printf("Máximo de peças no tabuleiro: %d\n", info->max_peca);
  printf("Máximo de peças a serem retiradas por jogada: %d\n",
         info->max_retirar);
}

int retirarPeca(struct JogoInfo *info, int pecaTirada) {
  if (pecaTirada > info->max_peca) {
    printf("Você está tentando retirar mais peças do que o número máximo "
           "permitido!\n");
    return 0;
  }

  if (pecaTirada > info->max_retirar) {
    printf("Você está tentando retirar mais peças do que o máximo permitido "
           "por jogada!\n");
    return 0;
  }

  if (info->pontuacao + pecaTirada >= info->max_peca) {
    printf("Parabéns, você ganhou o jogo!\n");
    return 1;
  }

  info->pontuacao += pecaTirada;
  return -1;
}

void partida(struct JogoInfo *info) {
  int n = info->max_peca;
  int vez_do_pc = 0;

  printf("\nComeçando uma nova partida...\n");
  printf("Número inicial de peças no tabuleiro: %d\n", n);

  while (n > 0) {
    if (vez_do_pc) {
      printf("Turno do computador...\n");
      int remove_computador = 1;
      printf("O computador removeu %d peça(s).\n", remove_computador);
      n -= remove_computador;
      vez_do_pc = 0;
    } else {
      printf("Seu turno...\n");
      int remove_jogador;
      printf("Digite quantas peças você quer remover (1 a %d): ",
             info->max_retirar);
      scanf("%d", &remove_jogador);
      int resultado = retirarPeca(info, remove_jogador);
      if (resultado == 1) {
        printf("Fim do jogo! Você ganhou!\n");
        return;
      } else if (resultado == 0) {
        printf("Jogada inválida! Tente novamente.\n");
        continue;
      }
      printf("Você removeu %d peça(s).\n", remove_jogador);
      n -= remove_jogador;
      vez_do_pc = 1;
    }

    if (n > 0) {
      printf("Agora restam %d peças no tabuleiro.\n", n);
    }
  }

  printf("Fim do jogo! O computador ganhou!\n");
}

int main() {
  struct JogoInfo info;
  inicializarPeca(&info);
  printf("Bem-vindo ao jogo do NIM!\n");

  printf("Digite o número máximo de peças no tabuleiro: ");
  scanf("%d", &info.max_peca);

  printf("Digite o número máximo de peças a serem retiradas por jogada: ");
  scanf("%d", &info.max_retirar);

  info.pontuacao = 0;

  printf("\nConfigurações do jogo:\n");
  exibirJogoInfo(&info);

  partida(&info);

  return 0;
}
