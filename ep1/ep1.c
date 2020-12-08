/******************************************************************************
 *** Regis de Abreu Barbosa                 Numero USP: 3135701             ***
 *** Rodrigo Mendes Leme                    Numero USP: 3151151             ***
 *** Exercicio-Programa 1                                                   ***
 *** Descricao: simula o jogo de cartas paciencia, utilizando uma pilha pa- ***
 ***            ra armazenar as cartas do jogo.                             ***
 *****************************************************************************/

#include <stdio.h>
#include <math.h>

#define P_ESTOQUE 0
#define P_JOGO1 1
#define P_JOGO2 2
#define P_JOGO3 3
#define P_JOGO4 4
#define P_JOGO5 5
#define P_JOGO6 6
#define P_JOGO7 7
#define P_SAIDA1 8
#define P_SAIDA2 9
#define P_SAIDA3 10
#define P_SAIDA4 11
#define P_DESCARTE 12
#define P_AUX 13

int p_cartas[60],       // Pilha que guarda todas as cartas do jogo
    p_base[14],         // Pilha que indica a base de cada subpilha do jogo
    p_topo[14],         // Pilha que indica o topo de cada subpilha do jogo
    p_ex_topo[14],      // Pilha que indica o topo anterior a ultima realocacao
    ult_aberta[8];      // Pilha que indica a ultima carta aberta em cada pilha
                        // de jogo

// Funcao: embaralha
// Descricao: embaralha as cartas, ocupando o vetor p_cartas de 0 a 51.

void embaralha () {
  int i, rand1, rand2, temp;
  srand(time(0)); 
  for (i = 0; i < 52; i++) p_cartas[i] = i;        // Coloca as cartas em ordem
  for (i = 0; i < 500; i++) {                      // no vetor p_cartas
    rand1 = rand()%52;                   // Gera dois indices de 0 a 51
    rand2 = rand()%52;
    temp = p_cartas[rand1];              // Troca as cartas sorteadas de lugar
    p_cartas[rand1] = p_cartas[rand2];
    p_cartas[rand2] = temp;
  }
}

// Funcao: converte
// Entrada: num: o valor de uma carta (As, 2,..., Q, K)
//          naipe: (C)opas, (O)uros, (E)spadas ou (P)aus.
// Saida: uma identificacao univoca para cada carta (0,..., 51).
// Descricao: converte uma carta em um numero representativo.

int converte (char num, char naipe) {
  int x;
  switch (num) {
    case 'A': x = 0;
              break;
    case 'J': x = 10;
              break;
    case 'Q': x = 11;
              break;
    case 'K': x = 12;
              break;
    case '1': x = 9;
              break;
    default : x = (num - 48) - 1;      // Na tabela ASCII, '0' vale 48
              break;
  }
  switch(naipe) {
    case 'O': break;
    case 'C': x = x + 13;
              break;
    case 'P': x = x + 26;
              break;
    case 'E': x = x + 39;
              break;  
  }
  return x;
}

// Funcao: gera_pilhas
// Descricao: gera as pilhas que serao usados na partida.

void gera_pilhas () {
  p_base[P_ESTOQUE] = 0;   // As primeiras cartas entram na pilha de estoque
  p_base[P_JOGO1] = 24;    // As seguintes sao distribuidas nas pilhas de jogo
  p_base[P_JOGO2] = 25;
  p_base[P_JOGO3] = 27;
  p_base[P_JOGO4] = 30;
  p_base[P_JOGO5] = 34;
  p_base[P_JOGO6] = 39;
  p_base[P_JOGO7] = 45;
  p_base[P_SAIDA1] = 52;
  p_base[P_SAIDA2] = 53;
  p_base[P_SAIDA3] = 54;
  p_base[P_SAIDA4] = 55;
  p_base[P_DESCARTE] = 56;
  p_base[P_AUX] = 57;

  p_ex_topo[P_ESTOQUE] = p_topo[P_ESTOQUE] = 23;
  p_ex_topo[P_JOGO1] = ult_aberta[P_JOGO1] = p_topo[P_JOGO1] = 24;
  p_ex_topo[P_JOGO2] = ult_aberta[P_JOGO2] = p_topo[P_JOGO2] = 26;
  p_ex_topo[P_JOGO3] = ult_aberta[P_JOGO3] = p_topo[P_JOGO3] = 29;
  p_ex_topo[P_JOGO4] = ult_aberta[P_JOGO4] = p_topo[P_JOGO4] = 33;
  p_ex_topo[P_JOGO5] = ult_aberta[P_JOGO5] = p_topo[P_JOGO5] = 38;
  p_ex_topo[P_JOGO6] = ult_aberta[P_JOGO6] = p_topo[P_JOGO6] = 44;
  p_ex_topo[P_JOGO7] = ult_aberta[P_JOGO7] = p_topo[P_JOGO7] = 51;
  p_ex_topo[P_SAIDA1] = p_topo[P_SAIDA1] = 51;
  p_ex_topo[P_SAIDA2] = p_topo[P_SAIDA2] = 52;
  p_ex_topo[P_SAIDA3] = p_topo[P_SAIDA3] = 53;
  p_ex_topo[P_SAIDA4] = p_topo[P_SAIDA4] = 54;
  p_ex_topo[P_DESCARTE] = p_topo[P_DESCARTE] = 55;
  p_ex_topo[P_AUX] = p_topo[P_AUX] = 56;
}

// Funcao: le_arquivo
// Saida: numero de cartas lidas do arquivo.
// Descricao: le de um arquivo texto as cartas que serao usadas no jogo.

int le_arquivo(FILE *arquivo, int pilha[])
{
  char num,
       naipe,
       c;
  int topo = 0;

  while (!feof(arquivo))
  {
    c = fgetc (arquivo);
    num = c;
    while (c != ' ') c = fgetc (arquivo);
    naipe = fgetc (arquivo);
    pilha[topo++] = converte (num, naipe);
    while (c != '\n' && c != EOF) c = fgetc (arquivo); 
  }
  fclose(arquivo);
  return topo;
}

// Funcao: prnc
// Entrada: a identificacao univoca de uma carta (0,..., 51).
// Descricao: recebe um numero representativo de uma carta e imprime na tela a
//            carta correspondente.

void prnc (int carta) {
  char num, naipe;
  int numero = carta%13,
      n_naipe = (int) carta/13;

  switch (numero) {          // Define o numero da carta
    case 0:  num = 'A';
             break;
    case 10: num = 'J';
             break;
    case 11: num = 'Q';
             break;
    case 12: num = 'K';
             break;
    default: numero++;
             num = 'N';
             break;
  }
  switch (n_naipe) {         // Define o naipe da carta
    case 0: naipe = 'O';
            break;
    case 1: naipe = 'C';
            break;
    case 2: naipe = 'P';
            break;
    case 3: naipe = 'E';
            break;
  }
  if(num != 'N') printf ("%c %c", num, naipe);       // Imprime a carta
  else printf ("%d %c", numero, naipe);
}

// Funcao: qual_p_saida
// Entrada: uma carta qualquer.
// Saida: qualquer uma das quatro pilhas de saida (P_SAIDA1,..., P_SAIDA4).
// Descricao: verifica se carta serve em uma das pilhas de sa¡da. Se sim,
//            retorna o identificador da pilha, senao devolve -1.

int qual_psaida (int carta) {
  int pilha;
  if (carta%13 == 0)     // Carta As, procura uma pilha vazia
  {
    for (pilha = P_SAIDA1; pilha <= P_SAIDA4; pilha++)
      if (pilha_vazia(p_base[pilha],p_topo[pilha])) return pilha;
  }
  else                   // Procura o valor imediatamente anterior ao da carta
    for (pilha = P_SAIDA1; pilha <= P_SAIDA4; pilha++)
      if (!pilha_vazia(p_base[pilha],p_topo[pilha]))
        if (carta == p_cartas[p_topo[pilha]] + 1) return pilha;
  return (-1);
}

// Funcao: serve_em_pjogo
// Entrada: uma carta qualquer.
// Saida: qualquer uma das sete pilhas de jogo (P_JOGO1,..., P_JOGO7).
// Descricao: verifica se carta serve em uma das pilhas de jogo. Se sim,
//            retorna o identificador da pilha, senao devolve -1.

int serve_em_pjogo (int carta) {
  int pilha, prox, carta_pilha;
  if ((carta%13) == 12)     // Carta K, procura uma pilha vazia
  {
    for (pilha = P_JOGO1; pilha <= P_JOGO7; pilha++)
      if (pilha_vazia(p_base[pilha], p_topo[pilha])) return pilha;
  }
  else           // Procura pilha onde a carta do topo tem valor imediatamente
  {              // superior a entrada com naipe de cor inversa
    for (pilha = P_JOGO1; pilha <= P_JOGO7; pilha++) {
      if (!pilha_vazia(p_base[pilha], p_topo[pilha])) {
        carta_pilha = p_cartas[p_topo[pilha]];
        if (carta_pilha <= 25)     // Carta vermelha 
          prox = (carta_pilha%13) + 25; 
        else prox = (carta_pilha%13) - 1;    // Carta preta
        if (carta == prox || carta == prox + 13) return pilha;
      }
    }
  }
  return (-1);
}

// Funcao: imprime_jogada
// Descricao: imprime na tela a jogada feita pelo computador.

void imprime_jogada (void) {
  int i, j;

  printf ("\n\nJogada:\n\n");
  printf ("Pilha de Descarte: ");         // Imprime carta da pilha de descarte
  if (!pilha_vazia(p_base[P_DESCARTE],p_topo[P_DESCARTE]))
    prnc (p_cartas[p_topo[P_DESCARTE]]);
  printf ("\n");
  for (i = P_JOGO1; i <= P_JOGO7; i++) {   // Imprime cartas das pilhas de jogo
    printf ("Pilha de Jogo %d:", i);
    for (j = p_base[i]; j < ult_aberta[i]; j++) printf (" . X");     // Carta
    if (!pilha_vazia(p_base[i],p_topo[i]))                           // fechada
      for (j = ult_aberta[i]; j <= p_topo[i]; j++) {
        printf (" . ");
        prnc (p_cartas[j]);     
      }
    printf ("\n");
  }
  printf ("\n");
  for (i = P_SAIDA1; i <= P_SAIDA4; i++) {          // Imprime cartas dos topos
    printf ("Pilha de saída %d: ", i - P_JOGO7);    // das pilhas de saida
    if (p_base[i] <= p_topo[i])
      prnc (p_cartas[p_topo[i]]);
    printf("\n");
  }
}

// Funcao: realoca_r
// Descricao: realoca as pilhas do vetor p_cartas com base na distribuição da
//            função realoca_g.

void realoca_r (int nova_base[]) {
  int i, j, dif;
  for (j = 0; j <= 13; j++) {        // Realoca as pilhas para cima.
    if (nova_base[j] < p_base[j]) {
      dif = p_base[j] - nova_base[j];
      for (i = p_base[j]; i <= p_topo[j]; i++) p_cartas[i - dif] = p_cartas[i];
      p_base[j] = nova_base[j];
      p_topo[j] = p_topo[j] - dif;
      if (P_JOGO1 <= j && j <= P_JOGO7) ult_aberta[j] = ult_aberta[j] - dif;
    }
  }
  for (j = 13; j >= 0; j--) {        // Realoca as pilhas para baixo.
    if (nova_base[j] > p_base[j]) {
      dif = nova_base[j] - p_base[j];
      for (i = p_topo[j]; i >= p_base[j]; i--) p_cartas[i + dif] = p_cartas[i];
      p_base[j] = nova_base[j];
      p_topo[j] = p_topo[j] + dif;
      if (P_JOGO1 <= j && j <= P_JOGO7) ult_aberta[j] = ult_aberta[j] + dif;
    }
  }
}

// Funcao: realoca_g
// Descricao: redistribui as pilhas do vetor p_cartas.

void realoca_g (int pilha) {
  int nova_base[14], D[14];
  int soma = 60, inc = 0;
  int j, t, r;
  float a, b;

  p_topo[pilha] = p_topo[pilha] + 1;       // Espaco que sera alocado
  for (j = 0; j <= 13; j++) {              // na pilha da entrada
    soma = soma - ((p_topo[j] - p_base[j]) + 1);
    if (p_topo[j] > p_ex_topo[j]) {
      D[j] = p_topo[j] - p_ex_topo[j];
      inc += D[j];
    } else D[j] = 0;
  }

  a = (float) soma/140; 
  b = (float) (9*soma)/(10*inc);

  nova_base[0] = p_base[0];
  r = 0;
  for (j = 1; j <= 13; j++) {
    t = floor(r + a + (D[j - 1]*b));
    nova_base[j] = nova_base[j-1] + p_topo[j-1] - p_base[j-1] + t - r + 1; 
    r = t;
  }

  p_topo[pilha] = p_topo[pilha] - 1; 
  realoca_r (nova_base);
  p_topo[pilha] = p_topo[pilha] + 1;

  for (j = 0; j <= 13; j++)          // Calcula o p_ex_topo para a proxima
    p_ex_topo[j] = p_topo[j];        // chamada desta funcao

  p_topo[pilha] = p_topo[pilha] - 1;
}

// Funcao: overflow
// Saida: flag indicando se houve ou nao overflow.
// Descricao: verifica se ocorre overflow. Se sim, realoca e devolve 1, senão
//            devolve 0.

int overflow (int pilha) {
  int base_sup = (pilha == 13) ? 60 : p_base[pilha + 1];  
  if ((base_sup - p_topo[pilha]) > 1) return 0;     // Existe espaço
  else { 
    realoca_g (pilha);     // Ocorreu overflow
    return 1;
  }
}

// Funcao: pilha_vazia
// Saida: flag indicando se uma pilha esta ou nao vazia.
// Descricao: verifica se uma pilha generica esta vazia.

int pilha_vazia(int base, int topo)
{
  return (topo < base);
}

// Funcao: empilha
// Saida: 1 se houve overflow, 0 caso contrario.
// Descricao: empilha um elemento numa pilha generica.

int empilha(int pilha[], int *topo, int carta, int qual_pilha)
{
  int valor = overflow(qual_pilha);

  *topo = *topo + 1;
  pilha[*topo] = carta;
  return valor;
}

// Funcao: desempilha
// Saida: o elemento do topo da pilha.
// Descricao: desempilha um elemento de uma pilha generica.

void desempilha(int pilha[], int base, int *topo, int *carta)
{
  if (!pilha_vazia(base,*topo))
  {
    *carta = pilha[*topo];
    *topo = *topo - 1;
  }
}

// Funcao: muda_pilhas_jogo
// Saida: um flag indicando se as pilhas de saida foram ou nao modificadas.
// Descricao: testa todas as pilhas de jogo ate achar uma carta que sirva numa 
//            das pilhas de saida.

int muda_pilhas_jogo(int *qtas_colisoes)
{
  int carta,
      qual_p_jogo   = P_JOGO1,
      p_saida       = -1,
      cont_colisoes = 0;

  while ((p_saida == -1) && (qual_p_jogo <= P_JOGO7))
  {
    if (!pilha_vazia(p_base[qual_p_jogo],p_topo[qual_p_jogo]))   // Pula pilhas
      {                                                          // vazias
      carta   = p_cartas[p_topo[qual_p_jogo]];
      p_saida = qual_psaida(carta);
      if (p_saida != -1)
      {
        desempilha(p_cartas,p_base[qual_p_jogo],&p_topo[qual_p_jogo],&carta);
        *qtas_colisoes += empilha(p_cartas,&p_topo[p_saida],carta,p_saida);
        if (ult_aberta[qual_p_jogo] > p_topo[qual_p_jogo])    // Se era a ul-
          ult_aberta[qual_p_jogo] = p_topo[qual_p_jogo];      // tima carta
      }                                                  // aberta, abre a se-
    }                                                    // guinte
    qual_p_jogo++;
  }
  return (p_saida != -1);
}

// Funcao: muda_pilha_descarte
// Saida: um flag indicando se a pilha de descarte foi ou nao modificada.
// Descricao: testa as pilhas de saida e de jogo para achar uma carta da pilha
//            de descarte que sirva numa das primeiras.

int muda_pilha_descarte(int *qtas_colisoes)
{
  int carta,
      p_saida = -1,
      p_jogo  = -1;

  if (!pilha_vazia(p_base[P_DESCARTE],p_topo[P_DESCARTE]))
  {
    carta = p_cartas[p_topo[P_DESCARTE]];
    p_saida = qual_psaida(carta);
    if (p_saida != -1)       // Testa primeira nas pilhas de saida
    {
      desempilha(p_cartas,p_base[P_DESCARTE],&p_topo[P_DESCARTE],&carta);
      *qtas_colisoes += empilha(p_cartas,&p_topo[p_saida],carta,p_saida);
    }
    else                     // Em seguida, testa as pilhas de jogo
    {
      p_jogo = serve_em_pjogo(carta);
      if (p_jogo != -1)
      {
        if (pilha_vazia(p_base[p_jogo],p_topo[p_jogo]))
          ult_aberta[p_jogo] = p_base[p_jogo];
        desempilha(p_cartas,p_base[P_DESCARTE],&p_topo[P_DESCARTE],&carta);
        *qtas_colisoes += empilha(p_cartas,&p_topo[p_jogo],carta,p_jogo);
      }
    }
  }
  return (!(p_saida == -1 && p_jogo == -1));
}

// Funcao: move_pilhas_jogo
// Saida: um flag indicando se as pilhas de jogo foram ou nao modificadas.
// Descricao: testa todas as pilhas de jogo ate achar uma carta que sirva numa
//            das outras pilhas de jogo.

int move_pilhas_jogo(int *qtas_colisoes)
{
  int carta,
      carta2,
      qual_p_jogo = P_JOGO1,
      p_a_mover   = -1;

  while ((p_a_mover == -1) && (qual_p_jogo <= P_JOGO7))
  {
    if (!pilha_vazia(p_base[qual_p_jogo],p_topo[qual_p_jogo]))
    {
      carta = p_cartas[ult_aberta[qual_p_jogo]];
      p_a_mover = serve_em_pjogo(carta);
      if (p_a_mover != -1)
        if (carta % 13 != 12 || (carta % 13 == 12 && ult_aberta[qual_p_jogo] >
            p_base[qual_p_jogo]))       // So continua se nao for a carta K; em
        {                               // sendo, so se o K nao estiver na base
          if (pilha_vazia(p_base[p_a_mover],p_topo[p_a_mover]))   // Se a pilha
            ult_aberta[p_a_mover] = p_base[p_a_mover];       // estava vazia, a
                                // nova carta torna-se a ultima aberta na pilha
          while (!pilha_vazia(ult_aberta[qual_p_jogo],p_topo[qual_p_jogo]))
	  {
            desempilha(p_cartas,ult_aberta[qual_p_jogo],&p_topo[qual_p_jogo],
                       &carta2);
            *qtas_colisoes += empilha(p_cartas,&p_topo[P_AUX],carta2,P_AUX);
	  }
          while (!pilha_vazia(p_base[P_AUX], p_topo[P_AUX]))
          {
            desempilha(p_cartas,p_base[P_AUX],&p_topo[P_AUX],&carta);
            *qtas_colisoes += empilha(p_cartas,&p_topo[p_a_mover],carta,
                                      p_a_mover);
          }
          if (ult_aberta[qual_p_jogo] > p_topo[qual_p_jogo])    // Se era a ul-
            ult_aberta[qual_p_jogo] = p_topo[qual_p_jogo];      // tima carta
        }                                            // aberta, abre a seguinte
        else                  // Encontrou uma carta K que nao pode ser movida;
          p_a_mover = -1;     // reinicializa o flag da pilha de jogo
    }
    qual_p_jogo++;
  }
  return (p_a_mover != -1);
}

// Funcao: muda_pilha_estoque
// Descricao: transfere uma carta da pilha de estoque para a de descarte.

void muda_pilha_estoque(int *qtas_colisoes)
{
  int carta;

  desempilha(p_cartas,p_base[P_ESTOQUE],&p_topo[P_ESTOQUE],&carta);
  *qtas_colisoes += empilha(p_cartas,&p_topo[P_DESCARTE],carta,P_DESCARTE);
}

// Função: main
// Descricao:

void main(int argc, char *argv[])
{
  FILE *arquivo;
  int k,
      qtas_colisoes = 0,         // Numero de colisoes das pilhas no jogo
      ha_movimentos = 1;         // Flag indicando se ainda eh possivel
                                 // continuar o jogo

  if (argc > 1)
  {
    arquivo = fopen(argv[1],"r");
    if (arquivo != NULL)
    {
      k = le_arquivo(arquivo,p_cartas);
      if (k < 52)
      {
        printf("Erro: numero de cartas do arquivo eh insuficiente.\n");
        return;
      }
    }
    else
    { 
      printf("Arquivo não encontrado.\n");
      return;
    }
  }
  else
    embaralha ();

  gera_pilhas ();
  imprime_jogada();
  while (!pilha_vazia(p_base[P_ESTOQUE],p_topo[P_ESTOQUE]))
  {
    if (!muda_pilhas_jogo(&qtas_colisoes))
      if (!move_pilhas_jogo(&qtas_colisoes))
        if (!muda_pilha_descarte(&qtas_colisoes))
          muda_pilha_estoque(&qtas_colisoes);
    imprime_jogada();
  }
  while (ha_movimentos)   // Apos a pilha de estoque ter-se esgotado, ainda
  {                       // testa outros possiveis movimentos com as cartas
    if (!muda_pilhas_jogo(&qtas_colisoes))
      if (!move_pilhas_jogo(&qtas_colisoes))
        if (!muda_pilha_descarte(&qtas_colisoes))
          ha_movimentos = 0;
    imprime_jogada();
  }
  printf("Numero de colisoes: %d\n",qtas_colisoes);
}
