#include <stdio.h>
#include <stdlib.h>

struct JogoInfo {
  int pontuacao;
  int *max_peca;
  int *max_retirar;
};

struct Campeonato {
  int pontuacao_humano;
  int pontuacao_computador;
};

void inicializarPeca(struct JogoInfo *info) {
  info->pontuacao = 0;
  info->max_peca = NULL;
  info->max_retirar = NULL;
}

void exibirJogoInfo(const struct JogoInfo *info) {
  printf("Pontuação: %d\n", info->pontuacao);
  printf("Máximo de peças no tabuleiro: %d\n", *(info->max_peca));
  printf("Máximo de peças a serem retiradas por jogada: %d\n",
         *(info->max_retirar));
}

int retirarPeca(struct JogoInfo *info, int pecaTirada) {
  if (pecaTirada > *(info->max_peca)) {
    printf("Você está tentando retirar mais peças do que o número máximo "
           "permitido!\n");
    return 0;
  }

  if (pecaTirada > *(info->max_retirar)) {
    printf("Você está tentando retirar mais peças do que o máximo permitido "
           "por jogada!\n");
    return 0;
  }

  if (info->pontuacao + pecaTirada >= *(info->max_peca)) {
    printf("Parabéns, você ganhou o jogo!\n");
    return 1;
  }

  info->pontuacao += pecaTirada;

  // Redimensionando o número máximo de peças no tabuleiro
  *(info->max_peca) -= pecaTirada;
  info->max_peca = (int *)realloc(info->max_peca, sizeof(int));

  return -1;
}

int partida() {
  struct JogoInfo info;
  inicializarPeca(&info);

  printf("Digite o número máximo de peças no tabuleiro: ");
  info.max_peca = (int *)malloc(sizeof(int));
  scanf("%d", info.max_peca);

  printf("Digite o número máximo de peças a serem retiradas por jogada: ");
  info.max_retirar = (int *)malloc(sizeof(int));
  scanf("%d", info.max_retirar);

  info.pontuacao = 0;

  printf("\nConfigurações do jogo:\n");
  exibirJogoInfo(&info);

  int n = *(info.max_peca);
  printf("Deseja começar o jogo? (0 para sim, 1 para não): ");
  int vez_do_pc;
  scanf("%d", &vez_do_pc);

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
                 *(info.max_retirar));
          scanf("%d", &remove_jogador);
          int resultado = retirarPeca(&info, remove_jogador);
          if (resultado == 1) {
              printf("Fim do jogo! Você ganhou!\n");
              free(info.max_peca);
              free(info.max_retirar);
              return 1; // Jogador ganhou
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
  free(info.max_peca);
  free(info.max_retirar);
  return 0; // Computador ganhou
}



void contar(struct Campeonato *pontos_jogador, int partidas) {
  int ganhou_humano = 0;
  int ganhou_computador = 0;

  for (int i = 0; i < partidas; i++) {
    if (pontos_jogador[i].pontuacao_humano == 1) {
      ganhou_humano++;
    }
    if (pontos_jogador[i].pontuacao_computador == 1) {
      ganhou_computador++;
    }
  }

  if (ganhou_humano > ganhou_computador) {
    printf("Parabéns! Você ganhou o campeonato!\n");
  } else if (ganhou_humano < ganhou_computador) {
    printf("O computador ganhou o campeonato!\n");
  } else {
    printf("O campeonato terminou em empate!\n");
  }
}

void campeonato(int partidas) {
  struct Campeonato *campeonato = malloc(partidas * sizeof(struct Campeonato));

  for (int i = 0; i < partidas; i++) {
      printf("\nPartida %d:\n", i + 1);
      if (partida()) {
          campeonato[i].pontuacao_humano += 1;
      } else {
          campeonato[i].pontuacao_computador += 1;
      }
  }

  // Chamando a função contar para determinar o vencedor do campeonato
  contar(campeonato, partidas);

  free(campeonato);
}
int main() {
  int escolha;
  int partidas;
  printf("Bem-vindo ao jogo do NIM!\n");
  printf("Escolha uma opção:\n");
  printf("1. Jogar partida normal\n");
  printf("2. Jogar campeonato\n");
  scanf("%d", &escolha);

  switch (escolha) {
  case 1:
    partida();
    break;
  case 2:
    printf("Digite o número de partidas do campeonato: ");
    scanf("%d", &partidas);
    campeonato(partidas);
    break;
  default:
    printf("Opção inválida.\n");
  }

  return 0;
}
