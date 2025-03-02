#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livraria.c"


int main(){
    int* id = read_id(); // leitura do id
    parametros_struct parametros = parametros_adicionais(); // leitura dos parametros adicionais

    //declaração da altura e largura maxima da imagem
    int max_x, max_y;
    max_x = (parametros.largura_linhas * 67) + (2 * parametros.esp_lateral);
    max_y = parametros.altura + (2 * parametros.esp_lateral);


    int** img;
    img = gerar_img(parametros, id, max_x, max_y); //gera a matriz imagem do codigo de barras
    print(max_y, max_x, img, parametros.nome_arquivo, parametros.largura_linhas, parametros.esp_lateral); //cria um arquivo .pbm com a matriz previamente criada

    //libera memoria alocada dinamicamente
    free(parametros.nome_arquivo);

    for(int i=0; i<max_y; i++){
        free(img[i]);
    }
    free(img);
    free(id);
    return 0;
}