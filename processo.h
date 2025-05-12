#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct {
    int id;
    char numero[30];
    char data_ajuizamento[20];
    int id_classe;
    int id_assuntos[10]; 
    int num_assuntos;
    int ano_eleicao;
} Processo;


int ler_processos(const char *nome_arquivo, Processo processos[], int max);
void ordenar_por_id(Processo processos[], int n);
void ordenar_por_data(Processo processos[], int n);
int contar_por_classe(Processo processos[], int n, int classe);
int contar_assuntos_distintos(Processo processos[], int n);
void listar_multiplos_assuntos(Processo processos[], int n);
int calcular_dias_tramitando(const char *data);

#endif
