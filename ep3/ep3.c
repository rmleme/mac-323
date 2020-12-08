/******************************************************************************
 *** Regis de Abreu Barbosa                 Numero USP: 3135701             ***
 *** Rodrigo Mendes Leme                    Numero USP: 3151151             ***
 *** Exercicio-Programa 3                                                   ***
 *** Descricao: compacta ou descompacta um arquivo, atraves do metodo de    ***
 ***            Huffman. Utiliza arvores binarias e listas de prioridades.  ***
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_CARAC 256
#define MAX_CAM 256
#define VAZIO '\0'

typedef struct _NO *ap_no;

typedef struct _NO
        {
          int frequencia;
          unsigned char caracter;
          ap_no esq,
                dir;
        } Tno;

typedef struct _CELULA
        {
          int frequencia;
          ap_no raiz;
        } Tcelula;

// Funcao: le_entrada
// Entrada: o nome do arquivo de entrada e o vetor de ocorrencias de cada
//          caracter.
// Saida: 1 se o arquivo existe; 0 caso contrario.
// Descricao: le cada caracter do arquivo de entrada, imprimindo-os na tela.

int le_entrada(char *nome_arq_ent, int ocorrencias[])
{
  FILE *entrada;
  unsigned char caracter;

  entrada = fopen(nome_arq_ent,"r");
  printf("\n");
  if (entrada != NULL)
  {
    printf("Texto do arquivo de entrada:\n");
    while (!feof(entrada))
    {
      caracter = fgetc(entrada);
      if (!feof(entrada))
      {
        printf("%c",caracter);
        ocorrencias[caracter]++;
      }
    }
    printf("\n\n");
    fclose(entrada);
    return 1;
  }
  else
  {
    printf("Erro: arquivo de entrada nao existe.\n");
    return 0;
  }
}

// Funcao: le_arvore
// Entrada: o nome do arquivo que contem a arvore de Huffman.
// Saida: 1 se o arquivo existe; 0 caso contrario.
// Descricao: verifica se o arquivo da arvore de Huffman existe.

int le_arvore(char *nome_arq_arv)
{
  FILE *arquivo_arv;

  arquivo_arv = fopen(nome_arq_arv,"r");
  if (arquivo_arv != NULL)
  {
    fclose(arquivo_arv);
    return 1;
  }
  else
  {
    printf("\nErro: arquivo da arvore de Huffman nao existe.\n");
    return 0;
  }
}

// Funcao: le_codificado
// Entrada: o nome do arquivo codificado.
// Saida: 1 se o arquivo existe; 0 caso contrario.
// Descricao: verifica se o arquivo a ser decodificado existe.

int le_codificado(char *nome_arq_cod)
{
  FILE *arquivo_cod;

  arquivo_cod = fopen(nome_arq_cod,"rb");
  printf("\n");
  if (arquivo_cod != NULL)
  {
    fclose(arquivo_cod);
    return 1;
  }
  else
  {
    printf("Erro: arquivo de codificacao nao existe.\n");
    return 0;
  }
}

// Funcao: imprime_frequencias
// Entrada: o vetor de ocorrencias de cada caracter.
// Descricao: imprime o numero de ocorrencias de cada caracter do arquivo de
//            entrada.

void imprime_frequencias(int ocorrencias[])
{
  int indice;

  printf("Tabela de frequencias dos caracteres do arquivo:\n");
  for (indice = 0; indice < MAX_CARAC; indice++)
    if (ocorrencias[indice] > 0)     // O caracter existe no arquivo de entrada
      printf("%c: %d\n",indice,ocorrencias[indice]);
  printf("\n");
}

// Funcao: imprime_arvore
// Entrada: a raiz da arvore de Huffman e o nivel da raiz.
// Descricao: imprime a arvore de Huffman na saida padrao.

void imprime_arvore(ap_no raiz, int nivel)
{
  int indice;

  if (raiz != NULL)
  {
    for (indice = 1; indice < nivel; indice++)
      printf("-");
    if (raiz->caracter == VAZIO)
      printf("%d",raiz->frequencia);
    else
      printf("%c",raiz->caracter);
    printf("\n");
    imprime_arvore(raiz->esq,nivel + 1);
    imprime_arvore(raiz->dir,nivel + 1);
  }
}

// Funcao: imprime_decodificado
// Entrada: o nome do arquivo decodificado.
// Descricao: imprime o arquivo ja decodificado na tela.

void imprime_decodificado(char *nome_arq_saida)
{
  FILE *arquivo_saida;
  char caracter;

  printf("Texto do arquivo decodificado:\n");
  arquivo_saida = fopen(nome_arq_saida,"r");
  while (!feof(arquivo_saida))
  {
    caracter = fgetc(arquivo_saida);
    if (caracter != EOF)
      printf("%c",caracter);
  }
  printf("\n");
  fclose(arquivo_saida);
}

// Funcao: subir
// Entrada: a lista de prioridades e o elemento a ser "subido".
// Descricao: movimenta um no da lista de prioridades em direcao a raiz, ate a
//            frequencia do mesmo ser menor que a dos filhos.

void subir(Tcelula heap[], int i)
{
  int j = floor(i / 2);
  Tcelula aux;

  if (j >= 1)
    if (heap[i].frequencia < heap[j].frequencia)      // As prioridades estao
    {                                                 // invertidas
      aux     = heap[i];
      heap[i] = heap[j];
      heap[j] = aux;
      subir(heap,j);
    }
}

// Funcao: descer
// Entrada: a lista de prioridades, o elemento a ser "descido" e o numero de
//          elementos da lista.
// Descricao: movimenta um no da lista de prioridades para longe da raiz, ate a
//            frequencia do mesmo ser maior que a do pai.

void descer(Tcelula heap[], int i, int quantos_nos)
{
  int j = 2 * i;
  Tcelula aux;

  if (j <= quantos_nos)
  {
    if (j < quantos_nos)
      if (heap[j + 1].frequencia < heap[j].frequencia)       // A prioridade do
        j++;                       // filho esquerdo e menor que a do direito
    if (heap[i].frequencia > heap[j].frequencia)        // As prioridades estao
    {                                                   // invertidas
      aux     = heap[i];
      heap[i] = heap[j];
      heap[j] = aux;
      descer(heap,j,quantos_nos);
    }
  }
}

// Funcao: insere_no
// Entrada: a lista de prioridades, o numero de elementos da mesma e o novo no
//          a ser inserido.
// Descricao: insere um novo elemento na lista de prioridades.

void insere_no(Tcelula heap[], int *quantos_nos, ap_no novo)
{
  if (*quantos_nos < MAX_CARAC)
  {
    (*quantos_nos)++;
    heap[*quantos_nos].raiz       = novo;
    heap[*quantos_nos].frequencia = novo->frequencia;
    subir(heap,*quantos_nos);
  }
}

// Funcao: remove_no
// Entrada: a lista de prioridades e o numero de elementos da mesma.
// Saida: o elemento removido.
// Descricao: remove o elemento da lista de menor prioridade.

ap_no remove_no(Tcelula heap[], int *quantos_nos)
{
  ap_no removido;

  if (*quantos_nos >= 1)
  {
    removido           = heap[1].raiz;
    heap[1] = heap[*quantos_nos];
    (*quantos_nos)--;
    descer(heap,1,*quantos_nos);
    return removido;
  }
  else
    return NULL;
}

// Funcao: constroi_heap
// Entrada: a lista de prioridades, o numero de nos da mesma e o vetor de ocor-
//          rencias de cada caracter.
// Descricao: constroi a lista de prioridades, pegando os elementos do vetor de
//            ocorrencias e ajustando a lista a uma estrutura de heap.

void constroi_heap(Tcelula heap[], int *quantos_nos, int ocorrencias[])
{
  int indice;
  ap_no novo;

  for (indice = 0; indice < MAX_CARAC; indice++)
    if (ocorrencias[indice] > 0)         // Cria uma folha e a inclui na heap
    {
      novo                  = (ap_no) malloc(sizeof(Tno));
      novo->caracter        = indice;
      novo->frequencia      = ocorrencias[indice];
      novo->esq = novo->dir = NULL;
      (*quantos_nos)++;
      heap[*quantos_nos].raiz       = novo;
      heap[*quantos_nos].frequencia = ocorrencias[indice];
    }
  for (indice = floor(*quantos_nos / 2); indice >= 1; indice--)  // Acerta as
    descer(heap,indice,*quantos_nos);                            // prioridades
}

// Funcao: constroi_arvore
// Entrada: a lista de prioridades e o numero de elementos da mesma.
// Saida: a raiz da arvore.
// Descricao: constroi a arvore de Huffman, baseado na lista de prioridades.

ap_no constroi_arvore(Tcelula heap[], int *quantos_nos)
{
  ap_no no_a,
        no_b,
        novo;

  while (*quantos_nos > 1)     // A arvore de Huffman ainda nao esta completa
  {
    no_a             = remove_no(heap,quantos_nos);
    no_b             = remove_no(heap,quantos_nos);
    novo             = (ap_no) malloc(sizeof(Tno));
    novo->caracter   = VAZIO;
    novo->frequencia = no_a->frequencia + no_b->frequencia;
    novo->esq        = no_a;
    novo->dir        = no_b;
    insere_no(heap,quantos_nos,novo);
  }
  return heap[1].raiz;
}

// Funcao: codifica_arvore
// Entrada: o arquivo que guardara a arvore, a raiz da arvore (chave),
//          os codigos que indicam o caminho dos caracteres,
//          uma pilha que guardará o caminho percorrido na arvore, o topo da
//          pilha e o caminho k de origem ('0' se for esquerda e '1' direita).
// Descricao: imprime num arquivo dado a arvore de Huffman, recursivamente.

void codifica_arvore(FILE *arquivo_arv, ap_no chave, unsigned char codigos[][MAX_CAM],
                     unsigned char pilha[], int topo, char k)
{
  int i, c;

  if (chave->caracter != VAZIO)       // E folha
  {
    fprintf(arquivo_arv,"c%c\n",chave->caracter);
    c = chave->caracter;
    for (i = 0; i < topo; i++)
      codigos[c][i] = pilha[i];
    codigos[c][topo] = k;
    codigos[c][topo+1] = VAZIO;
  }
  else    // E no interno
  {
    fprintf(arquivo_arv,"n%d\n",chave->frequencia);
    if (topo >= 0)
      pilha[topo] = k;
    codifica_arvore(arquivo_arv, chave->esq, codigos, pilha, topo + 1, '0');
    codifica_arvore(arquivo_arv, chave->dir, codigos, pilha, topo + 1, '1');
  }
}

// Funcao: decodifica_arvore
// Entrada: o arquivo da arvore de Huffman.
// Saida: raiz para a arvore de Huffman.
// Descricao: le o arquivo que contem a arvore de Huffman 
//            e constroi a arvore recursivamente.

ap_no decodifica_arvore(FILE *arquivo_arv)
{
  char caracter;
  ap_no novo;

  caracter = fgetc(arquivo_arv);

  if (!feof(arquivo_arv))
  {
    novo = (ap_no) malloc(sizeof(Tno));
    if (caracter == 'c')   // E uma folha
    {
      novo->caracter = fgetc(arquivo_arv);
      while ((fgetc(arquivo_arv)) != '\n');
    }
    else
      if (caracter == 'n')   // E no interno
      {
        novo->frequencia = 0;
        caracter         = fgetc(arquivo_arv);

        while (caracter != '\n')
        {
          novo->frequencia *= 10;
          novo->frequencia += (caracter - '0');
          caracter          = fgetc(arquivo_arv);
        }

        novo->caracter = VAZIO;
        novo->esq      = decodifica_arvore(arquivo_arv);
        novo->dir      = decodifica_arvore(arquivo_arv);    
      }
      else
        return NULL;
  }
  else
    return NULL;
  return novo;
}

// Funcao: converte_bin
// Entrada: o arquivo que que guarda os codigos e um vetor de '0' e '1'.
// Descricao: transforma uma base de codigos binarios em um caracter
//            de acordo com a tabela ASCII, e guarda o caracter no arquivo.

void converte_bin(FILE *arquivo_cod, unsigned char *bin)
{
  unsigned char n = 0;
  int indice;

  for (indice = 0; indice < 8; indice++)
    if (bin[indice] == '1')
      n += pow(2,7 - indice);
  fwrite(&n,sizeof(unsigned char),1,arquivo_cod);
}

// Funcao: converte_int
// Entrada: um caracter ASCII.
// Saida: uma string contendo a representacao binaria do codigo ASCII do
//        caracter de entrada.
// Descricao: converte o codigo ASCII de uma string para a base binaria.

void converte_int(unsigned char caracter, unsigned char *convertido)
{
  int indice;

  for (indice = 7; indice >= 0; indice--)
  {
    convertido[indice] = (caracter % 2) + '0';
    caracter /= 2;
  }
  convertido[8] = VAZIO;
}

// Funcao: determina_caminho
// Entrada: o arquivo de entrada, a tabela com os codigos dos caracteres
//          e um vetor.
// Saida: 0 se o arquivo estiver acabado, 1 caso contrario.
// Descricao: le um caracter do arquivo 
//            e copia o seu codigo para o vetor temporario.

int determina_caminho(FILE *arq_dados, unsigned char codigos[][MAX_CAM],
                      unsigned char *tmp)
{
  unsigned char caracter;
  int indice;

  if (feof(arq_dados))
    return 0;
  else
  {
    caracter = fgetc(arq_dados);
    for (indice = 0; codigos[caracter][indice] != VAZIO; indice++)
      tmp[indice] = codigos[caracter][indice];
    tmp[indice] = VAZIO;
    return 1;
  }
}

// Funcao: codifica_arquivo
// Entrada: o arquivo que contera a codificacao da entrada, o arquivo de
//          entrada e os codigos binarios dos caracteres.
// Saida: o numero de bytes do arquivo codificado.
// Descricao: codifica o arquivo de entrada.

int codifica_arquivo(char *nome_arq_cod, char *nome_arq_ent,
                     unsigned char codigos[][MAX_CAM])
{
  FILE *arquivo_cod,
       *arquivo_dados;
  int i,                     // Indice que percorre o vetor bin
      j          = 0,        // Indice que percorre o vetor tmp
      qtos_bytes = 0;        // Tamanho do arquivo de saida
  unsigned char bin[8],      // Guarda o caminho de 8 em 8 caracteres binarios
           tmp[MAX_CAM];     // Guarda o caminho de cada caracter

  arquivo_cod   = fopen(nome_arq_cod,"wb+");
  arquivo_dados = fopen(nome_arq_ent,"r");
  i = 0;
  while (determina_caminho(arquivo_dados,codigos,tmp))
  {
    j = 0;
    while (tmp[j] != VAZIO)       // O buffer ainda contem bits
    {
      bin[i++] = tmp[j];
      if (i == 8)             // Completou um byte
      {
        converte_bin(arquivo_cod,bin);
        i = 0;
        qtos_bytes++;
      }
      j++;
    }
  }
  if (i < 8)        // Falta escrever o ultimo byte no arquivo
  {
    while (i < 8)
      bin[i++] = '0';
    converte_bin(arquivo_cod,bin);
  }
  fclose(arquivo_cod);
  fclose(arquivo_dados);
  return qtos_bytes * sizeof(unsigned char);
}

// Funcao: decodifica_arquivo
// Entrada: o nome do arquivo de saida, o nome do arquivo a ser decodificado e
//          a raiz da arvore de Huffman.
// Descricao: le o arquivo codificado caracter a caracter, convertendo-o para
//            seu formato original e gravando o mesmo num outro arquivo.

void decodifica_arquivo(char *nome_arq_saida, char *nome_arq_cod, ap_no raiz)
{
  FILE *arquivo_saida,
       *arquivo_cod;
  int indice,
      qtos_carac_lidos = 0;      // Numero de caracteres lidos da entrada
  ap_no atu = raiz;
  unsigned char caracter,
                caminho[MAX_CAM + 1];   // O caminho a ser percorrido na arvore
                                        // de Huffman para achar um caracter

  arquivo_saida = fopen(nome_arq_saida,"wb+");
  arquivo_cod   = fopen(nome_arq_cod,"rb"); 
  while (!feof(arquivo_cod))
  {
    caracter = fgetc(arquivo_cod);
    indice   = 0;
    if (!feof(arquivo_cod))
    {
      converte_int(caracter,caminho);
      while (indice <= 7 && qtos_carac_lidos < raiz->frequencia)    // O buffer
      {                                                      //ainda nao acabou
        if (caminho[indice] == '0')      // Caminha para a esquerda
          atu = atu->esq;
        else                             // Caminha para a direita
          atu = atu->dir;
        indice++;
        if (atu->caracter != VAZIO)      // Chegou numa folha
	{
          fprintf(arquivo_saida,"%c",atu->caracter);
          qtos_carac_lidos++;
          atu = raiz;
        }
      }
    }
  }
  fclose(arquivo_saida);
  fclose(arquivo_cod);
}

// Funcao: main
// Descricao: testa os argumentos passados na linha de comando e executa as
//            chamadas de funcoes que codificam e decodificam arquivos.

void main(int argc, char *argv[])
{
  FILE *arquivo_arv;               // Arquivo que guardara a arvore de Huffman
  int ocorrencias[MAX_CARAC],      // Frequencia de cada caracter no arquivo
      quantos_nos = 0,             // Numero de elementos na heap
      qtos_bytes_cod,              // Numero de bytes do arquivo codificado
      indice;
  unsigned char codigos[MAX_CARAC][MAX_CAM],       // Codigos binarios de
                tmp[8],                            // cada caracter
                nome_arq_ent[21],
                nome_arq_saida[21],
                nome_arq_arv[21],
                nome_arq_cod[21];
  Tcelula heap[MAX_CARAC + 1];     // Lista de prioridades
  ap_no raiz;                      // Raiz da arvore de Huffman

  if (argc != 2)
    printf("Erro: numero de argumentos invalido.\n");
  else
    if (strcmp(argv[1],"c") == 0)      // Usuario quer codificar um arquivo
    {
      for (indice = 0; indice < MAX_CARAC; indice++)
        ocorrencias[indice] = 0;
      printf("Digite o nome do arquivo de entrada (.dat): ");
      gets(nome_arq_ent);
      if (le_entrada(nome_arq_ent,ocorrencias))
      {
        printf("Digite o nome do arquivo da arvore de Huffman (.arv): ");
        gets(nome_arq_arv);
        printf("Digite o nome do arquivo de codificacao (.cod): ");
        gets(nome_arq_cod);
        printf("\n");
        imprime_frequencias(ocorrencias);
        constroi_heap(heap,&quantos_nos,ocorrencias);
        raiz = constroi_arvore(heap,&quantos_nos);
        arquivo_arv = fopen(nome_arq_arv,"w");
        codifica_arvore(arquivo_arv,raiz,codigos,tmp,-1,'0');
        fclose(arquivo_arv);
        qtos_bytes_cod = codifica_arquivo(nome_arq_cod,nome_arq_ent,codigos);
        printf("Taxa de reducao: %.2f\n",(double) qtos_bytes_cod /
               (sizeof(unsigned char) * raiz->frequencia));
      }
    }
    else
      if (strcmp(argv[1],"d") == 0)      // Usuario quer decodificar um arquivo
      {
        printf("Digite o nome do arquivo da arvore de Huffman (.arv): ");
        gets(nome_arq_arv);
        if (le_arvore(nome_arq_arv))
        {
          printf("Digite o nome do arquivo de codificacao (.cod): ");
          gets(nome_arq_cod);
          if (le_codificado(nome_arq_cod))
          {
            arquivo_arv = fopen(nome_arq_arv,"r");
            raiz = decodifica_arvore(arquivo_arv);
            fclose(arquivo_arv);
            printf("Arvore de Huffman:\n");
            imprime_arvore(raiz,1);
            printf("\nDigite o nome do arquivo de saida (.dec): ");
            gets(nome_arq_saida);
            printf("\n");
            decodifica_arquivo(nome_arq_saida,nome_arq_cod,raiz);
            imprime_decodificado(nome_arq_saida);
          }
        }
      }
      else
        printf("Erro: argumento invalido.\n");
}
