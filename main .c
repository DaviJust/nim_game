#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estruturas
struct jogo_informacao {
  int pontuacao;
  int *max_peca;
  int *max_retirar;
};

struct campeonato {
  int pontuacao_humano;
  int pontuacao_computador;
};

void inicializa_peca(struct jogo_informacao * jogo_informacao) {
  jogo_informacao->pontuacao = 0;
  jogo_informacao->max_peca = (int *)malloc(sizeof(int));
  jogo_informacao->max_retirar = (int *)malloc(sizeof(int));
}

void exibir_jogo_informacao(const struct jogo_informacao *_informacao) {
  printf("Pontuação: %d\n", _informacao->pontuacao);
  if (_informacao->max_peca != NULL) {
    printf("Máximo de peças no tabuleiro: %d\n", *(_informacao->max_peca));
  }
  if (_informacao->max_retirar != NULL) {
    printf("Máximo de peças a serem retiradas por jogada: %d\n",
           *(_informacao->max_retirar));
  }
}

int retirar_peca(struct jogo_informacao *_informacao, int pecaTirada) {
  if (pecaTirada > *(_informacao->max_peca)) {
    printf("Você está tentando retirar mais peças do que o número máximo permitido!\n");
    return 0;
  }

  if (pecaTirada > *(_informacao->max_retirar)) {
    printf("Você está tentando retirar mais peças do que o máximo permitido por jogada!\n");
    return 0;
  }

  if (_informacao->pontuacao + pecaTirada >= *(_informacao->max_peca)) {
    printf("Parabéns, você ganhou o jogo!\n");
    return 1;
  }

  _informacao->pontuacao += pecaTirada;
  *(_informacao->max_peca) -= pecaTirada;

  return -1;
}

int partida() {
  struct jogo_informacao _informacao;
  inicializa_peca(&_informacao);

  printf("Digite o número máximo de peças no tabuleiro: ");
  scanf("%d", _informacao.max_peca);

  printf("Digite o número máximo de peças a serem retiradas por jogada: ");
  scanf("%d", _informacao.max_retirar);

  printf("\nConfigurações do jogo:\n");
  exibir_jogo_informacao(&_informacao);

  int n = *(_informacao.max_peca);
  printf("Deseja começar o jogo? (0 para sim, 1 para não): ");
  int vez_do_pc;
  scanf("%d", &vez_do_pc);

  printf("\nComeçando uma nova partida...\n");
  printf("Número inicial de peças no tabuleiro: %d\n", n);

  srand(time(NULL));

  while (n > 0) {
    if (vez_do_pc) {
      printf("Turno do computador...\n");
      int remove_computador = (rand() % *(_informacao.max_retirar)) + 1;
      if (remove_computador > n) {
        remove_computador = n;
      }
      printf("O computador removeu %d peça(s).\n", remove_computador);
      n -= remove_computador;
      vez_do_pc = 0;
    } else {
      printf("Seu turno...\n");
      int remove_jogador;
      printf("Digite quantas peças você quer remover (1 a %d): ",
             *(_informacao.max_retirar));
      scanf("%d", &remove_jogador);
      int resultado = retirar_peca(&_informacao, remove_jogador);
      if (resultado == 1) {
        printf("Fim do jogo! Você ganhou!\n");
        free(_informacao.max_peca);
        free(_informacao.max_retirar);
        return 1;
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
  free(_informacao.max_peca);
  free(_informacao.max_retirar);
  return 0;
}

void salvar_pontuacao(const char *filename, int *score) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo para salvar.\n");
    return;
  }
  fprintf(file, "%s\n%d\n", filename, *score);
  fclose(file);
  printf("Pontuação salva com sucesso.\nNome: %s\nScore: %d\n", filename,
         *score);
  
}

void carregar_pontuacao( char *filename, int *score) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo %s para leitura.\n", filename);
    return;
  }

  char nome[100]; 
  int temp_score; 

  if (fgets(nome, sizeof(nome), file) == NULL) {
    printf("Erro ao ler o nome do jogador.\n");
    fclose(file);
    return;
  }

  nome[strcspn(nome, "\n")] = '\0';

  if (fscanf(file, "%d", &temp_score) != 1) {
    printf("Erro ao ler a pontuação do jogador.\n");
    fclose(file);
    return;
  }

  strcpy(filename, nome);
  *score = temp_score;

  fclose(file);
  printf("Pontuação carregada com sucesso.\n");
  printf("Nome do jogador: %s\n", nome);
  printf("Pontuação: %d\n", *score);
}



float campeonatos(int partidas, int *total_campeonatos_ganhos,
                  int *total_campeonatos_feitos) {
  float score_final = 0.0;
  for (int i = 0; i < partidas; i++) {
    if (partida()) {
      (*total_campeonatos_ganhos) += 1;
      (*total_campeonatos_feitos) += 1;
    } else {
      (*total_campeonatos_feitos) += 1;
    }
  }
  if (*total_campeonatos_feitos > 0) {
    score_final = ((float)(*total_campeonatos_ganhos) * partidas) /
                  *total_campeonatos_feitos;
    printf("%f", score_final);
  }
  return score_final;
}

int main(int argc, char *argv[]) {
  int escolha;
  int partidas = 0;
  int score = 0;
  int total_campeonatos_feitos = 0;
  int total_campeonatos_ganhos = 0;
  do {
    printf("\nBem-vindo ao jogo do NIM!\n");
    printf("Escolha uma opção:\n");
    printf("1. Jogar partida normal\n");
    printf("2. Jogar campeonato\n");
    printf("3. Salvar pontuação\n");
    printf("4. Carregar pontuação\n");
    printf("5. Sair\n");
    scanf("%d", &escolha);

    if (escolha == 1) {
      partida();
    } else if (escolha == 2) {
      printf("Digite o número de partidas do campeonato:");
      scanf("%d", &partidas);
      score = campeonatos(partidas, &total_campeonatos_ganhos,
                          &total_campeonatos_feitos);
    } else if (escolha == 3) {
      char filename[256];
      printf("Digite seu nome para salvar: ");
      scanf("%s", filename);
      salvar_pontuacao(filename, &score);
    } else if (escolha == 4) {
      char filename[256];
      printf("Digite o nome do arquivo para carregar: ");
      scanf("%s", filename);
      carregar_pontuacao(filename, &score);
    } else if (escolha != 5) {
      printf("Opção inválida. Tente novamente.\n");
    }
  } while (escolha < 5 || escolha < 1);

  return 0;
}

