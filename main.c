#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Erro: uso correto: ./scheduler <rate|edf> <arquivo>\n");
        return 1;
    }

    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0) {
        fprintf(stderr, "Erro: algoritmo deve ser rate ou edf\n");
        return 1;
    }

    return 0;
}