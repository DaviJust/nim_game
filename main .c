#include <stdio.h>
#include <stdlib.h>

struct JogoInfo {
  int pontuacao;
  int *max_peca;
  int *max_retirar;
};

// Definição da estrutura para pontuações do campeonato
struct Campeonato {
    int pontuacao_humano;
    int pontuacao_computador;
    struct Campeonato *prox_pontuacao; // Próxima pontuação na lista
};

// Variável global para o início da lista
struct Campeonato *inicioL = NULL;



// Função para adicionar uma pontuação à lista
void adicionarPontuacao(int ponto_computador, int ponto_humano) {
    // Aloca memória para o novo elemento
    struct Campeonato *novoElemento = (struct Campeonato *)malloc(sizeof(struct Campeonato));
    if (novoElemento == NULL) {
        printf("Erro ao alocar memória para novo elemento.\n");
        return; // Retorna se não foi possível alocar memória
    }

    // Define a pontuação do novo elemento
    novoElemento->pontuacao_humano = ponto_computador;
    novoElemento->pontuacao_humano = ponto_humano;
    // Define o próximo como NULL, pois é o último elemento
    novoElemento->prox_pontuacao = NULL;

    // Se a lista estiver vazia, o novo elemento será o primeiro
    if (inicioL == NULL) {
        inicioL = novoElemento;
    } else {
        // Se não estiver vazia, percorre até o último elemento e adiciona o novo no final
        struct Campeonato *temp = inicioL;
        while (temp->prox_pontuacao != NULL) {
            temp = temp->prox_pontuacao;
        }
        temp->prox_pontuacao = novoElemento; // Adiciona o novo elemento ao final
    }
}

// Função para liberar a memória alocada para a lista
void liberarLista() {
    struct Campeonato *atual = inicioL;
    while (atual != NULL) {
        struct Campeonato *proximo = atual->prox_pontuacao; // Salva o próximo elemento
        free(atual); // Libera a memória do elemento atual
        atual = proximo; // Move para o próximo elemento
    }
    inicioL = NULL; // Define o início da lista como NULL, indicando que a lista está vazia
}

void inicializarPeca(struct JogoInfo *info) {
  info->pontuacao = 0;
  info->max_peca = NULL;
  info->max_retirar = NULL;
}


struct Campeonato *inicioL = NULL;

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

void partida() {
  char ganhou = "Fim do jogo! Você ganhou!";
  char perdeu = "Fim do jogo! Você ganhou!";
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
             *(info.max_retirar));
      scanf("%d", &remove_jogador);
      int resultado = retirarPeca(&info, remove_jogador);
      if (resultado == 1) {
        printf("%s",ganhou);
        free(info.max_peca);
        free(info.max_retirar);
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

  printf("%s",perdeu);
  adicionarPontuacao(0,1);
  free(info.max_peca);
  free(info.max_retirar);
}

int main() {
  printf("Bem-vindo ao jogo do NIM!\n");
  partida();
  return 0;
}
