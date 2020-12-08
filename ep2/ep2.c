/******************************************************************************
 *** Regis de Abreu Barbosa                 Numero USP: 3135701             ***
 *** Rodrigo Mendes Leme                    Numero USP: 3151151             ***
 *** Exercicio-Programa 2                                                   ***
 *** Descricao: simula o funcionamento de um sistema de multiprocessamento  ***
 ***            para compartilhar o uso de impressoras.                     ***
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VEL_IMP 0.05          // Linhas por milissegundo
#define TEMPO_ATIV_PROC 5000
#define TEMPO_INT_SIST 100
#define TEMPO_ESC_PROC 25
#define TEMPO_IMP_EST 100000
#define TEMPO_TOTAL 3600000

typedef enum {
  IMP1,
  IMP2,
  IMP3,
  IMP4,
  IMP5,
  GERADOR,
  CPU,
  ESTADO_FILAS
  } DISPOSITIVOS;

typedef struct _PROCESSO
        {
          int id,
              classe,
              tempo_cpu,
              tempo_total_cpu,
              linhas_imp,
              tempo_inicial,
              tempo_f_classe,
              tempo_f_cpu,
              tempo_f_imp;
        } Tprocesso;

typedef struct _F_PROC * ap_f_proc;

struct _F_PROC              // Tipo generico que sera usado nas filas
{                           // de classes e de CPU
  Tprocesso processo;
  ap_f_proc prox;
};

typedef struct _F_IMPS * ap_f_imps;

struct _F_IMPS              // Tipo que sera usado na fila de impressoras
{
  Tprocesso processo;
  ap_f_imps ant,
            prox;
};

ap_f_proc pri_f_clas1    = NULL,        // Fila de processos da classe 1
          ult_f_clas1    = NULL,
          pri_f_clas2    = NULL,        // Fila de processos da classe 2
          ult_f_clas2    = NULL,
          pri_f_cpu      = NULL,        // Fila de processos da CPU
          ult_f_cpu      = NULL,
          pri_liberado   = NULL;        // Pilha de nos disponiveis para a CPU
ap_f_imps pri_f_imps     = NULL,        // Fila de processos das impressoras
          ult_f_imps     = NULL;
static int num_proc_term = 0,
           media_perm    = 0,
           media_f_clas1 = 0,
           media_f_clas2 = 0,
           media_f_cpu   = 0,
           media_f_imps  = 0,
           media_cpu     = 0;

// Funcao: inicializa_filas
// Descricao: cria os nos cabecas das filas da classe 1, classe 2 e impressora
//            e cria a lista de nos liberados para a fila da CPU.

void inicializa_filas(void)
{
  int indice;
  ap_f_proc atu, novo;

  pri_f_clas1                  = (ap_f_proc) malloc(sizeof(struct _F_PROC));
  pri_f_clas1->processo.id     = 0;
  pri_f_clas1->processo.classe = 1;
  pri_f_clas1->prox            = NULL;
  ult_f_clas1                  = pri_f_clas1;

  pri_f_clas2                  = (ap_f_proc) malloc(sizeof(struct _F_PROC));
  pri_f_clas2->processo.id     = 0;
  pri_f_clas2->processo.classe = 2;
  pri_f_clas2->prox            = NULL;
  ult_f_clas2                  = pri_f_clas2;

  pri_liberado = (ap_f_proc) malloc(sizeof(struct _F_PROC));
  atu          = pri_liberado;
  for (indice = 1; indice <= 6; indice++)      // Cria a pilha de nos liberados
  {
    novo = (ap_f_proc) malloc(sizeof(struct _F_PROC));
    atu->prox = novo;
    atu = novo;
  }
  atu->prox = NULL;

  pri_f_imps              = (ap_f_imps) malloc(sizeof(struct _F_IMPS));
  pri_f_imps->processo.id = 0;
  pri_f_imps->ant         = pri_f_imps->prox = pri_f_imps;
  ult_f_imps              = pri_f_imps;
}

// Funcao: insere_f_clas
// Entrada: o apontador final da fila mais um processo.
// Descricao: insere um novo no numa das filas de classe de processos.

void insere_f_clas(ap_f_proc *ult, Tprocesso processo)
{
  ap_f_proc novo;

  novo           = (ap_f_proc) malloc(sizeof(struct _F_PROC));
  novo->processo = processo;
  novo->prox     = NULL;
  (*ult)->prox   = novo;
  *ult           = novo;
}

// Funcao: remove_f_clas
// Entrada: os apontadores inicial e final da fila mais um processo.
// Descricao: remove um no de uma das filas de classe de processos.

void remove_f_clas(ap_f_proc pri, ap_f_proc *ult, Tprocesso *processo)
{
  ap_f_proc aux;

  if (pri->prox != NULL)       // A fila nao esta vazia
  {
    aux       = pri->prox;
    pri->prox = aux->prox;
    if (pri->prox == NULL)     // Removeu o ultimo no; aponta para o no cabeca
      *ult = pri;
    *processo = aux->processo;
    free(aux);
  }
}

// Funcao: insere_f_cpu
// Entrada: um processo.
// Descricao: insere um novo no na fila da CPU, pegando-o da lista de nos libe-
//            rados.

void insere_f_cpu(Tprocesso processo)
{
  if (pri_f_cpu == NULL)       // A fila da CPU esta vazia
  {
    pri_f_cpu    = ult_f_cpu = pri_liberado;
    pri_liberado = pri_liberado->prox;
  }
  else
  {
    ult_f_cpu->prox = pri_liberado;
    ult_f_cpu       = ult_f_cpu->prox;
    pri_liberado    = pri_liberado->prox;
  }
  ult_f_cpu->processo = processo;
  ult_f_cpu->prox     = pri_f_cpu;
}

// Funcao: remove_f_cpu
// Entrada: um processo.
// Descricao: remove um no da fila da CPU, mandando-o para a lista de nos libe-
//            rados.

void remove_f_cpu(Tprocesso *processo)
{
  ap_f_proc temp;

  if (pri_f_cpu != NULL)        // A fila nao esta vazia
  {
    *processo = pri_f_cpu->processo;
    if (pri_f_cpu == ult_f_cpu)       // A fila so tem um no
    {
      pri_f_cpu->prox = pri_liberado;
      pri_liberado    = pri_f_cpu;
      pri_f_cpu       = ult_f_cpu = NULL;
    }
    else
    {
      temp = pri_f_cpu;
      pri_f_cpu = pri_f_cpu->prox;
      temp->prox = pri_liberado;
      pri_liberado = temp;
    }
  }
}

// Funcao: insere_f_imps
// Entrada: os apontadores inicial e final da fila mais um processo.
// Descricao: insere um novo no na fila de impressoras.

void insere_f_imps(ap_f_imps pri, ap_f_imps *ult, Tprocesso processo)
{
  ap_f_imps novo;

  novo           = (ap_f_imps) malloc(sizeof(struct _F_IMPS));
  novo->processo = processo;

  (*ult)->prox = novo;
  novo->prox   = pri;
  pri->ant     = novo;
  novo->ant    = *ult;
  *ult         = novo;
}

// Funcao: remove_f_imps
// Entrada: os apontadores inicial e final da fila mais um processo.
// Saida: 1 se o no foi removido; 0 caso contrario.
// Descricao: remove um no da fila de impressoras.

int remove_f_imps(ap_f_imps pri, ap_f_imps *ult, Tprocesso *processo)
{
  ap_f_imps aux;

  if (pri != ult_f_imps)       // A fila nao esta vazia
  {
    aux            = pri->prox;
    pri->prox      = aux->prox;
    aux->prox->ant = pri;
    if (pri->prox == pri)      // Removeu o ultimo no; aponta para o no cabeca
      *ult = pri;
    *processo = aux->processo;
    free(aux);
    return 1;
  }
  return 0;
}

// Funcao: menor_tempo
// Entrada: os relogios dos dispositivos e um ponteiro para o dispositivo
// Saida: menor valor dos relogios de entrada
//        e o dispositivo que indica qual e o relogio.
// Descricao: calcula qual dispositivo deve ser o proximo a ser executado

int menor_tempo (int relogio_gera, int relogio_cpu, int relogio_estado,
                 int *relogio_imp, DISPOSITIVOS *dispositivo)
{
  int menor = relogio_gera;
  DISPOSITIVOS i;

  *dispositivo = GERADOR;
  if (menor > relogio_cpu) {
    menor = relogio_cpu;
    *dispositivo = CPU;
  }

  if (menor > relogio_estado) {
    menor = relogio_estado;
    *dispositivo = ESTADO_FILAS;
  }

  for (i = 0; i < 5 ; i++) {
    if (menor > relogio_imp[i]) {
      menor = relogio_imp[i];
      *dispositivo = i;
    }
  }

  return menor;
}

// Funcao: gera_processo
// Entrada: o tempo atual marcado pelo relogio principal do programa.
// Saida: um processo.
// Descricao: gera aleatoriamente um processo.

Tprocesso gera_processo(int tempo_atual)
{
  double prob;
  Tprocesso processo;
  static int cont_id = 1;     // Auxilia na identificacao dos processos

  processo.id             = cont_id++;
  processo.tempo_inicial  = tempo_atual;
  processo.tempo_f_classe = 0;
  processo.tempo_f_cpu    = 0;
  processo.tempo_f_imp    = 0;
  prob                    = rand() % 10;
  if (prob < 7)             // Gera um processo da classe 1
  {
    processo.classe     = 1;
    processo.tempo_cpu  = processo.tempo_total_cpu = rand() % 5000 + 1;
    processo.linhas_imp = rand() % 500 + 1;
  }
  else                      // Gera um processo da classe 2
  {
    processo.classe     = 2;
    processo.tempo_cpu  = processo.tempo_total_cpu = rand() % 30000 + 1;
    processo.linhas_imp = rand() % 10000 + 1;
  }
  return processo;
}

// Funcao: confere_classe
// Entrada: um processo.
// Descricao: insere um processo numa das filas de classe de processos.

void confere_classe(Tprocesso processo)
{
  if (processo.classe == 1)
    insere_f_clas(&ult_f_clas1,processo);
  else
    insere_f_clas(&ult_f_clas2,processo);
}

// Funcao: entra_na_cpu
// Entrada: o tempo atual de execucao marcado pelo relogio principal.
// Descricao: checa as filas de processos das classes 1 e 2 e joga um dos
//            mesmos na fila da CPU.

void entra_na_cpu(int tempo_atual)
{
  Tprocesso processo;
  static int qtos_proc_clas1 = 0;

  if (pri_f_clas1 == ult_f_clas1 || qtos_proc_clas1 == 4 )   // A fila da clas-
  {                                                          // se 1 esta vazia
                                      // ou ja estourou o limite de 4 processos
    if (pri_f_clas2 != ult_f_clas2)   // A fila da classe 2 nao esta vazia
    {
      remove_f_clas(pri_f_clas2,&ult_f_clas2,&processo);
      processo.tempo_f_classe = tempo_atual - processo.tempo_inicial;
      insere_f_cpu(processo);
      qtos_proc_clas1 = 0;
    }    
  }
  else if (pri_f_clas1 != ult_f_clas1)     // A fila da classe 1 nao esta vazia
  {
    remove_f_clas(pri_f_clas1,&ult_f_clas1,&processo);
    processo.tempo_f_classe = tempo_atual - processo.tempo_inicial;
    insere_f_cpu(processo);
    qtos_proc_clas1++;
  }
}

// Funcao: executa_processo
// Entrada: o tempo atual marcado pelo relogio principal do programa.
// Saida: o tempo gasto pela CPU para executar o processo.
// Descricao: retira um processo da fila da CPU e "executa-o" durante o tempo
//            de intervencao padrao do sistema. No final, se ele acabou, joga-o
//            na fila das impressoras; caso contrario, ele volta para a fila da
//            CPU.

int executa_processo(int tempo_atual)
{
  int tempo_gasto = 0,
      tempo_exec_proc;
  Tprocesso processo;

  if (pri_f_cpu != NULL)       // A fila da CPU nao esta vazia
  {
    remove_f_cpu(&processo);
    tempo_exec_proc = processo.tempo_cpu - TEMPO_INT_SIST;
    if (tempo_exec_proc > 0)       // Processo ainda nao acabou
    {
      tempo_gasto         = TEMPO_INT_SIST;
      processo.tempo_cpu  = tempo_exec_proc;
      insere_f_cpu(processo);
    }
    else           // Processo terminou
    {
      tempo_gasto          = processo.tempo_cpu;
      processo.tempo_cpu   = 0;
      processo.tempo_f_cpu = tempo_atual - processo.tempo_inicial -
                             processo.tempo_f_classe;
      insere_f_imps(pri_f_imps,&ult_f_imps,processo);
      entra_na_cpu (tempo_atual);
    }
  }
  return tempo_gasto;
}

// Funcao: termina_processo
// Entrada: recebe um processo e o tempo atual marcado pelo relogio
//          principal do programa.
// Descricao: imprime, na saida-padrao, as caracteristicas do processo e calcu-
//            la as medias das estatisticas das filas.

void termina_processo (Tprocesso proc_terminado, int tempo)
{
  if (proc_terminado.id != 0) {
    int tempo_sistema = tempo - proc_terminado.tempo_inicial;

    num_proc_term++;
    media_perm += tempo_sistema;
    if (proc_terminado.classe == 1)
      media_f_clas1 += proc_terminado.tempo_f_classe;
    else
      media_f_clas2 += proc_terminado.tempo_f_classe;
    media_f_cpu  += proc_terminado.tempo_f_cpu;
    media_f_imps += proc_terminado.tempo_f_imp;
    media_cpu    += proc_terminado.tempo_total_cpu;

    printf ("\nProcesso %d terminado.\n", proc_terminado.id);
    printf ("  Classe %d\n", proc_terminado.classe);
    printf ("  Inicio: %.3f Fim: %.3f\n", (double)
            proc_terminado.tempo_inicial / 1000, (double) tempo / 1000);
    printf ("  Tempo de CPU: %.3f\n", (double) proc_terminado.tempo_total_cpu /
            1000);
    printf ("  Numero de linhas impressas: %d\n", proc_terminado.linhas_imp);
    printf ("  Tempo de permanencia no sistema: %.3f\n", (double) tempo_sistema
            / 1000);
    printf ("  Tempo nas filas\n");
    printf ("    de classe: %.3f\n", (double) proc_terminado.tempo_f_classe /
            1000);
    printf ("    de execucao: %.3f\n", (double) proc_terminado.tempo_f_cpu /
            1000);
    printf ("    de impressoras: %.3f\n", (double) proc_terminado.tempo_f_imp /
            1000);
    printf ("  Razao: %f\n", (double) proc_terminado.tempo_total_cpu /
            tempo_sistema);
  } else proc_terminado.id = 0;
}

// Funcao: imprime_processo
// Entrada: os processos que estao sendo imprimidos no momento mais o identifi-
//          cador das impressoras.
// Saida: o tempo que a impressora vai levar para imprimir o processo.
// Descricao: envia um processo da fila de impressoras para uma impressora.

int imprime_processo (Tprocesso proc_imprimindo[], int relogio, DISPOSITIVOS
                      disp)
{
  Tprocesso processo;

  if (remove_f_imps (pri_f_imps, &ult_f_imps, &processo))  // Ha processos a
  {                                                        // serem impressos
    processo.tempo_f_imp = relogio - processo.tempo_inicial -
                           processo.tempo_f_classe - processo.tempo_f_cpu;
    proc_imprimindo[disp] = processo;
    return (processo.linhas_imp/VEL_IMP);
  } else {
    proc_imprimindo[disp].id = 0;
    return 0;
  }
}

// Funcao: verifica_estado_filas
// Entrada: o tempo atual marcado pelo relogio principal do programa.
// Descricao: imprime o conteudo das filas de classes, cpu e impressoras.

void verifica_estado_filas (int tempo)
{
  ap_f_proc temp;
  ap_f_imps temp_i;

  printf ("\nVerificando estado das filas:\n=============================\n");

  printf ("\nFila da classe 1:\n processo (tempo que esta na fila)\n");
  if (pri_f_clas1 == ult_f_clas1) printf (" vazia.\n");
  else
    for (temp = pri_f_clas1->prox; temp != NULL; temp = temp->prox)
      printf (" %d (%d segundos)\n", temp->processo.id,
                (tempo - temp->processo.tempo_inicial) / 1000);

  printf ("\nFila da classe 2:\n processo (tempo que esta na fila)\n");
  if (pri_f_clas2 == ult_f_clas2) printf (" vazia.\n");
  else
    for (temp = pri_f_clas2->prox; temp != NULL; temp = temp->prox)
      printf (" %d (%d segundos)\n", temp->processo.id,
                (tempo - temp->processo.tempo_inicial) / 1000);

  printf ("\nFila da CPU:\n processo (tempo de CPU, tempo restante de execucao)\n");
  if (pri_f_cpu == NULL) printf (" vazia.\n");
  else {
    for (temp = pri_f_cpu; temp != ult_f_cpu; temp = temp->prox)
      printf (" %d (%.3f segundos, %.3f segundos)\n", temp->processo.id,
              (double) temp->processo.tempo_total_cpu / 1000,
              (double) temp->processo.tempo_cpu / 1000);
    printf (" %d (%.3f segundos, %.3f segundos)\n", temp->processo.id,
            (double) temp->processo.tempo_total_cpu / 1000,
            (double) temp->processo.tempo_cpu / 1000);
  }

  printf ("\nFila das impressoras:\n processo (numero de linhas, tempo na fila)\n");
  if (pri_f_imps == ult_f_imps) printf (" vazia.");
  else {
    for (temp_i= pri_f_imps->prox; temp_i != ult_f_imps; temp_i = temp_i->prox)
      printf (" %d (%d linhas, %.3f segundos)\n", temp_i->processo.id,
              temp_i->processo.linhas_imp,
              (double) (tempo - temp_i->processo.tempo_inicial
              - temp_i->processo.tempo_f_classe -
               temp_i->processo.tempo_f_cpu) / 1000);
    printf (" %d (%d linhas, %.3f segundos)\n", temp_i->processo.id,
            temp_i->processo.linhas_imp,
            (double) (tempo - temp_i->processo.tempo_inicial
            - temp_i->processo.tempo_f_classe - temp_i->processo.tempo_f_cpu)
            / 1000);
  }

  printf("\n");
}

// Funcao: main
// Descricao: controla a geracao, execucao e impressao de processos, atraves de
//            relogios para cada tipo de evento (abordagem event driven).

void main (void)
{
  int i,
      relogio        = 0,
      relogio_estado = 0,
      relogio_gera   = 0,
      relogio_cpu    = 0,
      relogio_imp[5],
      tempo_gasto;
  Tprocesso processo,
            proc_imprimindo[5];
  DISPOSITIVOS dispositivo;

  processo.id = 0;
  for (i = 0; i < 5; i++) {
    relogio_imp[i] = 0;
    proc_imprimindo[i].id = 0;
  }

  srand(time(0));

  inicializa_filas();
  while (relogio < TEMPO_TOTAL) {
    relogio = menor_tempo (relogio_gera, relogio_cpu, relogio_estado,
                           relogio_imp, &dispositivo);

    switch (dispositivo) {
      case GERADOR: processo = gera_processo(relogio);
                    confere_classe(processo);
                    if (pri_liberado != NULL)      // A fila da CPU nao
                      entra_na_cpu(relogio);       // esta cheia
                    relogio_gera += TEMPO_ATIV_PROC;
                    break;
      case CPU: tempo_gasto = executa_processo(relogio);
                if (tempo_gasto == 0)    // Nao tem processos na fila. Espera
                  relogio_cpu = relogio_gera;        // ate gerar um processo
                else
                  relogio_cpu += (tempo_gasto + TEMPO_ESC_PROC);
                break;
      case ESTADO_FILAS: relogio_estado += TEMPO_IMP_EST;
                         verifica_estado_filas (relogio);
                         break;
      default: termina_processo (proc_imprimindo[dispositivo], relogio);
               tempo_gasto = imprime_processo (proc_imprimindo, relogio,
                                               dispositivo);
               if (tempo_gasto == 0)       // Nao tem processos na fila. Espera
                 relogio_imp[dispositivo] = relogio_cpu;    // a execucao de um
               else                                         // processo
                 relogio_imp[dispositivo] += tempo_gasto;
    }
  }

  verifica_estado_filas (TEMPO_TOTAL);

  printf ("\nNumero total de processos: %d\n", processo.id);
  printf ("Numero total de processos terminados: %d\n", num_proc_term);
  num_proc_term *= 1000;         // Transformacao do tempo para segundos
  printf ("Tempo medio de permanencia no sistema: %.5f segundos\n",
          (double) media_perm / num_proc_term);
  printf ("Tempo medio gasto nas filas\n");
  printf ("   de classe 1: %.5f segundos\n", (double) media_f_clas1 /
          num_proc_term);
  printf ("   de classe 2: %.5f segundos\n", (double) media_f_clas2 /
          num_proc_term);
  printf ("   de execucao: %.5f segundos\n", (double) media_f_cpu /
          num_proc_term);
  printf ("   de impressoras: %.5f segundos\n", (double) media_f_imps /
          num_proc_term);
  printf ("Media das razoes: %.5f\n", (double) media_cpu / media_perm);
}
