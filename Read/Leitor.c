#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include "livraria.c"

int main(){
    char nome_arquivo[257]; // Array para armazenar o nome do arquivo PBM
    printf("Digite o nome do arquivo PBM: "); // Solicita ao usuário o nome do arquivo PBM
    fgets(nome_arquivo, 256, stdin); // Lê o nome do arquivo
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0; // Remove o caractere de nova linha da entrada do usuário

    FILE *arquivo = fopen(nome_arquivo, "r"); // Abre o arquivo PBM em modo de leitura
    if (!arquivo) { // Se o arquivo não puder ser aberto
        perror("Erro ao abrir o arquivo.\n");
        return 0;
    }
    parametros_struct parametros = ler_parametros(arquivo); //lê os parametros
    char **matriz_img = ler_arquivo(arquivo, parametros); //lê o arquivo e armazena numa matriz

    //lê, traduz e printa o identificador
    ler_matriz_codigo(matriz_img, parametros);
    printf("digite 1 para encerrar a leitura:\n");
    char lixo[3];
    fgets(lixo, 3, stdin);
    fclose(arquivo);
    return 0;
}