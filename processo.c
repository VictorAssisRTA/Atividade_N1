#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

int ler_csv(const char *nome_arquivo, Processo processos[]) { 
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) { 
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    char linha[256];
    fgets(linha, sizeof(linha), fp); 

    int i = 0;
    while (fgets(linha, sizeof(linha), fp) && i < MAX_PROCESSOS)
    {
        char *token = strtok(linha, ",");
        processos[i].id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(processos[i].data_ajuizamento, token);

        token = strtok(NULL, ",");
        processos[i].id_classe = atoi(token);

        token = strtok(NULL, ",");
        processos[i].num_assuntos = 0;

        if (token[0] == '{') { 
            token++;
            while (*token && *token != "}") { 
                processos[i].id_assunto[processos[i].num_assuntos++] = atoi(token);
                while (*token && *token != ',' && *token != '}') token++;
                if (*token == ',') token++;
                
            }
        } else {
            processos[i].id_assunto[0] = atoi(token);
            processos[i].num_assuntos = 1;
        }

        token = strtok(NULL, ",\n");
        processos[i].ano_eleicao = atoi(token);
        i++;
    }

    fclose(fp);
    return i;
}

void escrever_csv(const char *arquivo_n, Processo processos[], int total) { 
    FILE *fp = fopen(arquivo_n, "w");
    fprintf(fp, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < total; i++) { 
        fprintf(fp, "%d,%s,%s,%d,{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento, processos[i].id_classe);
        for (int j = 0; j < processos[i].num_assuntos; j++) { 
            fprintf(fp, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].num_assuntos - 1) fprintf(fp, ",");
        }
        fprintf(fp, "},%d\n", processos[i].ano_eleicao);
    }
    fclose(fp);
}

void ordenar_por_id(Processo processos[], int total) { 
    for (int i = 0; i < total - 1; i++) { 
        for (int j = i + 1; j < total; j++) { 
            if (processos[i].id > processos[j].id) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

int comparar_data(const char *data1, const char *data2) { 
    return strcmp(data2, data1);
}

void ordenar_por_data_desc(Processo processos[], int total) { 
    for (int i = 0; i < total - 1; i++) { 
        for (int j = i + 1; j < total; j++) { 
            if (comparar_data(processos[i].data_ajuizamento, processos[j].data_ajuizamento) > 0) { 
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
            }
        }
    }
}

int contar_por_classe(Processo processos[], int total, int id_classe) { 
    int count = 0;
    for (int i = 0; i < total; i++) { 
        if (processos[i].id_classe == id_classe) count++;
    }
    return count;
}

int contar_assuntos_unicos(Processo processos[], int total) { 
    int assuntos[10000] = {0}, count = 0;
    for (int i = 0; i < total; i++) { 
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            int id = processos[i].id_assunto[j];
            if (!assuntos[id]) { 
                assuntos[id] = 1;
                count++;
            }
        }
    }
    return count;
}

void listar_multiplos_assuntos(Processo processos[], int total) { 
    printf("\nProcessos com múltiplos assuntos:\n");
    for (int i = 0; i < total; i++) { 
        if (processos[i].num_assuntos > 1) {
            printf("ID: %d, Número: %s\n", processos[i].id, processos[i].numero);
        }
    } 
}

void calcular_dias_em_tramitacao(Processo processos[], int total) { 
    time_t t = time(NULL);
    struct tm *hoje = localtime(&t);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", hoje);

    for (int i = 0; i < total; i++) { 
        struct tm dt = {0};
        sscanf(processos[i].data_ajuizamento, "%4d-%2d-%2d", &dt.tm_year, &dt.tm_mon, &dt.tm_mday);
        dt.tm_year -= 1900;
        dt.tm_mon -= 1;
        time_t data_ini = mktime(&dt);
        double dias = difftime(t, data_ini) / (60 * 60 * 24);
        printf("Processo %s está em tramitação há %.0f dias\n", processos[i].numero, dias);
    }
}