#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

int ler_processos(const char *nome_arquivo, Processo processos[], int max) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) return 0;
    
    char linha[256];
    int count = 0;
    fgets(linha, sizeof(linha), fp); // Pular cabeçalho

    while (fgets(linha, sizeof(linha), fp) && count < max) {
        Processo p;
        char *token = strtok(linha, ",");
        p.id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(p.numero, token);

        token = strtok(NULL, ",");
        strcpy(p.data_ajuizamento, token);

        token = strtok(NULL, ",{}\\n");
        p.id_classe = atoi(token);

        token = strtok(NULL, "{}\\n");
        p.num_assuntos = 0;
        char *subtok = strtok(token, ",");
        while (subtok) {
            p.id_assuntos[p.num_assuntos++] = atoi(subtok);
            subtok = strtok(NULL, ",");
        }

        token = strtok(NULL, ",\\n");
        p.ano_eleicao = atoi(token);

        processos[count++] = p;
    }
    fclose(fp);
    return count;
}

void ordenar_por_id(Processo processos[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (processos[i].id > processos[j].id) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

int comparar_data(const char *d1, const char *d2) {
    return strcmp(d1, d2);
}

void ordenar_por_data(Processo processos[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (comparar_data(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

int contar_por_classe(Processo processos[], int n, int classe) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (processos[i].id_classe == classe)
            count++;
    }
    return count;
}

int contar_assuntos_distintos(Processo processos[], int n) {
    int assuntos[10000] = {0};
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            int id = processos[i].id_assuntos[j];
            if (!assuntos[id]) {
                assuntos[id] = 1;
                total++;
            }
        }
    }
    return total;
}

void listar_multiplos_assuntos(Processo processos[], int n) {
    for (int i = 0; i < n; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("Processo ID %d tem %d assuntos.\n", processos[i].id, processos[i].num_assuntos);
        }
    }
}

int calcular_dias_tramitando(const char *data) {
    struct tm tm = {0};
    strptime(data, "%Y-%m-%d %H:%M:%S", &tm);
    time_t data_ajuiz = mktime(&tm);
    time_t hoje = time(NULL);
    double diff = difftime(hoje, data_ajuiz);
    return diff / (60 * 60 * 24);
}

void salvar_csv(const char *nome_arquivo, Processo processos[], int n) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) return;
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,%s,%s,%d,", processos[i].id, processos[i].numero, processos[i].data_ajuizamento, processos[i].id_classe);
        fprintf(fp, "{");
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            fprintf(fp, "%d", processos[i].id_assuntos[j]);
            if (j < processos[i].num_assuntos - 1) fprintf(fp, ",");
        }
        fprintf(fp, "},%d\n", processos[i].ano_eleicao);
    }
    fclose(fp);
}
