#include <stdio.h>
#include "processo.h"

int main() { 
    Processo processos[MAX_PROCESSOS];
    int total_processos = ler_csv("processo_043_202409032338.csv", processos);

    ordenar_por_id(processos, total_processos);
    escrever_csv("ordenado_por_id.csv", processos, total_processos);

    ordenar_por_data_desc(processos, total_processos);
    escrever_csv("ordenado_por_data.csv", processos, total_processos);

    int id_classe;
    printf("Digite o ID de classe para contar os processos: ");
    scanf("%d", &id_classe);
    printf("Total de processos com id_classe %d: %d/n", id_classe, contar_por_classe(processos, total_processos, id_classe));

    printf("Total de assuntos únicos: %d/n", contar_assuntos_unicos(processos,total_processos));

    listar_multiplos_assuntos(processos, total_processos);

    calcular_dias_em_tramitacao(processos, total_processos);

    return 0;

}
