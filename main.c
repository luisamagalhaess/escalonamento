#include <stdio.h>
#include <string.h>

typedef struct Tarefa{
    char nome[20];
    int periodo;
    int deadline;
    int burst;
} Tarefa;

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Erro: uso correto: ./scheduler <rate|edf> <arquivo>\n");
        return 1;
    }

    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0) {
        fprintf(stderr, "Erro: algoritmo deve ser rate ou edf\n");
        return 1;
    }

    FILE *arquivo = fopen(argv[2], "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo\n");
        return 1;
    }

    
    int tempoTotal;
    if (fscanf(arquivo, "%d", &tempoTotal) != 1) {
        fprintf(stderr, "Erro: tempo total invalido\n");
        fclose(arquivo);
        return 1;
    }
    
    if (tempoTotal <= 0) {
        fprintf(stderr, "Erro: tempo total deve ser positivo\n");
        fclose(arquivo);
        return 1;
    }
    Tarefa tarefas[100];
    int qtdTarefas = 0;
    int leitura;
    while ((leitura = fscanf(arquivo, "%19s %d %d %d", tarefas[qtdTarefas].nome, &tarefas[qtdTarefas].periodo, &tarefas[qtdTarefas].deadline, &tarefas[qtdTarefas].burst)) != EOF) {
        if (leitura != 4) {
            fprintf(stderr, "Erro: arquivo mal formatado\n");
            fclose(arquivo);
            return 1;
        }

        if (tarefas[qtdTarefas].periodo <= 0 || tarefas[qtdTarefas].deadline <= 0 || tarefas[qtdTarefas].burst <= 0) {
            fprintf(stderr, "Erro: valores da tarefa devem ser positivos\n");
            fclose(arquivo);
            return 1;
        }

        if (tarefas[qtdTarefas].deadline > tarefas[qtdTarefas].periodo) {
            fprintf(stderr, "Erro: deadline maior que periodo\n");
            fclose(arquivo);
            return 1;
        }

        if (tarefas[qtdTarefas].burst > tarefas[qtdTarefas].deadline) {
            fprintf(stderr, "Erro: burst maior que deadline\n");
            fclose(arquivo);
            return 1;
        }

        qtdTarefas++;
    }
    fclose(arquivo);
    
    return 0;
}