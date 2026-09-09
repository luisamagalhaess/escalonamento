#include <stdio.h>
#include <string.h>

typedef struct Tarefa{
    char nome[20];
    int periodo;
    int deadline;
    int burst;
    int restante;
    int proximaChegada;
    int deadlineAbsoluto;
    int ativa;
    int lost;
    int complete;
    int killed;
} Tarefa;

int escolherRate(Tarefa tarefas[], int qtdTarefas) {
    int escolhida = -1;
    for (int i = 0; i < qtdTarefas; i++) {
        if (tarefas[i].ativa == 1) {
            if (escolhida == -1 || tarefas[i].periodo < tarefas[escolhida].periodo) {
                escolhida = i;
            }
        }
    }

    return escolhida;
}

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
        tarefas[qtdTarefas].restante = 0;
        tarefas[qtdTarefas].proximaChegada = 0;
        tarefas[qtdTarefas].deadlineAbsoluto = 0;
        tarefas[qtdTarefas].ativa = 0;
        tarefas[qtdTarefas].lost = 0;
        tarefas[qtdTarefas].complete = 0;
        tarefas[qtdTarefas].killed = 0;

        qtdTarefas++;
    }
    fclose(arquivo);

    FILE *saida;
    if (strcmp(argv[1], "rate") == 0) {
        saida = fopen("rate_lfm3.out", "w");
    } else {
        saida = fopen("edf_lfm3.out", "w");
    }

    if (saida == NULL) {
        fprintf(stderr, "Erro: nao foi possivel criar arquivo de saida\n");
        return 1;
    }

    for (int tempo = 0; tempo < tempoTotal; tempo++) {

        for (int i = 0; i < qtdTarefas; i++) {
            if (tarefas[i].ativa == 1 && tempo == tarefas[i].deadlineAbsoluto && tarefas[i].restante > 0) {
                tarefas[i].lost++;
                tarefas[i].restante = 0;
                tarefas[i].ativa = 0;
            }
        }

        for (int i = 0; i < qtdTarefas; i++) {

            if (tempo == tarefas[i].proximaChegada) {

                tarefas[i].restante = tarefas[i].burst;
                tarefas[i].deadlineAbsoluto = tempo + tarefas[i].deadline;
                tarefas[i].proximaChegada = tempo + tarefas[i].periodo;
                tarefas[i].ativa = 1;
            }
        }
        int escolhida = escolherRate(tarefas, qtdTarefas);
        if (escolhida != -1) {
            tarefas[escolhida].restante--;

            if (tarefas[escolhida].restante == 0) {
                tarefas[escolhida].complete++;
                tarefas[escolhida].ativa = 0;
            }
        }
    }
    
    for (int i = 0; i < qtdTarefas; i++) {
        if (tarefas[i].ativa == 1 && tarefas[i].restante > 0) {
            tarefas[i].killed++;
            tarefas[i].ativa = 0;
            tarefas[i].restante = 0;
        }
    }

    fprintf(saida, "LOST DEADLINES\n");
    for (int i = 0; i < qtdTarefas; i++) {
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].lost);
    }

    fprintf(saida, "COMPLETE EXECUTION\n");
    for (int i = 0; i < qtdTarefas; i++) {
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].complete);
    }

    fprintf(saida, "KILLED\n");
    for (int i = 0; i < qtdTarefas; i++) {
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].killed);
    }
    fclose(saida);
    
    return 0;
}