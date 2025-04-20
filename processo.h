#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_PROCESSOS 1000
#define MAX_ASSUNTOS 10
#define MAX_DATA 11

typedef struct {
    int id;
    char numero[30];
    char data_ajuizamento[MAX_DATA];
    int id_classe;
    int id_assunto[MAX_ASSUNTOS];
    int num_assuntos;
    int ano_eleicao;
} Processo;

int ler_csv(const char *nome_arquivo, Processo processos[]);
void escrever_csv(const char *nome_arquivo, Processo processos[], int total);
void ordenar_por_id(Processo processos[], int total);
void ordenar_por_data_desc(Processo processos[], int total);
int contar_por_classe(Processo processos[], int total, int id_classe);
int contar_assuntos_unicos(Processo processos[], int total);
void listar_multiplos_assuntos(Processo processos[], int total);
void calcular_dias_em_tramitacao(Processo processos[], int total);

#endif
