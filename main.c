#include <stdio.h>
#include "processo.h"

#define MAX_PROCESSOS 1000

int main() {
    Processo processos[MAX_PROCESSOS];
    int total = ler_processos("processo_043_202409032338.csv", processos, MAX_PROCESSOS);

    if (total == 0) {
        printf("Erro ao ler o arquivo.\n");
        return 1;
    }
    
    ordenar_por_id(processos, total);
    salvar_csv("ordenado_por_id.csv", processos, total);

    ordenar_por_data(processos, total);
    salvar_csv("ordenado_por_data.csv", processos, total);

    int classe_alvo = 11778;  
    int count_classe = contar_por_classe(processos, total, classe_alvo);
    printf("Total de processos com id_classe %d: %d\n", classe_alvo, count_classe);

    int assuntos_distintos = contar_assuntos_distintos(processos, total);
    printf("Assuntos distintos encontrados: %d\n", assuntos_distintos);

    printf("Processos com mais de um assunto:\n");
    listar_multiplos_assuntos(processos, total);

    printf("Dias de tramitação para os primeiros 5 processos:\n");
    for (int i = 0; i < total && i < 5; i++) {
        int dias = calcular_dias_tramitando(processos[i].data_ajuizamento);
        printf("ID %d - %d dias\n", processos[i].id, dias);
    }

    return 0;
}
