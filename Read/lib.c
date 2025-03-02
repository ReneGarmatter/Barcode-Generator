#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char numeros_lcode[10][8] = {"0001101","0011001","0010011","0111101","0100011",
"0110001","0101111","0111011","0110111","0001011"};

const char numeros_rcode[10][8] = {"1110010", "1100110", "1101100", "1000010", "1011100", 
"1001110", "1010000", "1000100", "1001000", "1110100"};

const char numeros[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

struct parametros_struct{
    int largura;
    int esp_lateral;
    int altura;
    int largura_linhas;
    char* nome_arquivo;
}typedef parametros_struct; // struct dos parametros adicionais

//gerador
int file_exists(const char* nome_arquivo) {
    //checagem se o arquivo ja existe
    FILE * file = fopen(nome_arquivo, "r");
    if (file) {
        fclose(file);
        return 1;  // arquivo existe
    }
    return 0;  // arquivo não existe
}
void print(int max_y, int max_x, int** img, char* nome, int largura, int espaçamento){
    //verifica se o arquivo já existe
    char resposta;
    if(file_exists(nome)){
        printf("Arquivo já existente, deseja sobrescrever-lo?(S|N)\n");
        scanf("%c", &resposta);
        while(resposta != 'S' && resposta != 'N' && resposta != 's' && resposta != 'n'){ //repete caso a resposta seja invalida
            scanf("%c", &resposta);
        }
        
        if(resposta == 'N' || resposta == 'n'){
            printf("ERRO: arquivo resultante já existe\n");
            exit(0);
        }
    }
    
    //cria e preenche o arquivo pbm
    FILE * fp;
    fp = fopen(nome, "w+");

    fprintf(fp,"P1 #%d %d\n", largura, espaçamento);
    fprintf(fp,"%d %d\n", max_x, max_y);
    for(int i=0; i< max_y; i++){
        for(int j=0; j<max_x; j++){
            fprintf(fp, "%d", img[i][j]);
        }
        fprintf(fp, "\n");
    }
}
int* char_to_int(char* id_aux){
    //conversão de um vetor de char para vetor de int
    char* id_char = (char*)malloc(sizeof(char) * 9);
    int* id_int = (int*)malloc(sizeof(int) * 8);

    strcpy(id_char, id_aux);
    for(int i=0; i<8; i++){
        id_int[i] = id_char[i] - 48;
    }
    free(id_char);
    return id_int;
}
void verify_id(char id_aux[]){
    //verifica se o identificador tem o numero correto de digitos
    if(strlen(id_aux) != 9){
        printf("ERRO: Identificador não possui 8 dígitos");
        exit(0);
    }

    //verifica se o identificador tem caracteres invalidos
    int verif;
    for(int i=0; i<8; i++){
        verif = 0;
        for(int j=0; j<10; j++){
            if(id_aux[i] == numeros[j]){
                verif = 1;
            }
        }
        if(verif == 0){
            printf("ERRO: Identificador contém valores não numéricos");
            exit(0);
        }
    }

    //conversão de um vetor de char para vetor de inteiros
    int* id_int = char_to_int(id_aux);
    
    //verifica o digito verificador
    int digito_verificador = id_int[7];
    int soma=0;
    for(int i=0; i<7; i++){ // faz a operação do dígito verificador
        if(i%2 == 0){
            soma = soma + (3 * id_int[i]);
        }
        else{
            soma = soma + (1 * id_int[i]);
        }
    }
    if(digito_verificador != 10 - (soma % 10)){
        printf("ERRO: O dígito verificador do identificador é inválido");
        exit(0);
    }
}
int* read_id(){
    //leitura do identificador
    printf("Insira o código identificador:\n");

    char id_aux[12]; //id aux para verificar se o comando extrapola 8 caracteres
    fgets(id_aux, 11, stdin); // lê o id
    verify_id(id_aux); //verifica a validade do id
    int* id = char_to_int(id_aux); //copia o array de char id_aux para o array de inteiros id
    
    return id; // retorna o array de inteiros id
}
parametros_struct parametros_adicionais(){
    //declaração dos parametros padrão
    parametros_struct parametros;
    parametros.esp_lateral = 15;
    parametros.altura = 50;
    parametros.largura_linhas = 1;
    parametros.nome_arquivo = (char*)malloc(sizeof(char)*31);
    strcpy(parametros.nome_arquivo, "Codigo_de_barras.pbm");

    //padroes declarados pelo user
    printf("Deseja adicionar parametros adicionais?:(S/N)\n");
    char resposta;
    scanf("%c", &resposta);
    
    while(resposta != 'S' && resposta != 'N' && resposta != 's' && resposta != 'n'){ //repete caso a resposta seja invalida
        scanf("%c", &resposta);
    }
    if(resposta == 'N' || resposta == 'n'){
        return parametros;
    }

    //prompt dos parametors numericos
    printf("(espaçamento lateral  altura  largura das linhas):");
    scanf("%d %d %d", &parametros.esp_lateral, &parametros.altura, &parametros.largura_linhas);

    int c;
    //flush no stdin
    while ((c = getchar()) != '\n' && c != EOF);

    //prompt do nome do arquivo
    printf("Insira o nome do arquivo(.pbm):");
    fgets(parametros.nome_arquivo, 27, stdin);

    return parametros;
}
int** gerar_img(parametros_struct parametros,int* id, int max_x, int max_y){
    //declaração da matriz imagem
    int** img = (int**)malloc(max_y * sizeof(int *));

    for (int i = 0; i < max_y; i++) {
        img[i] = (int*)malloc(max_x * sizeof(int));
    }

    
    //criar canvas em branco
    for(int i=0; i<max_y; i++){
        for(int j=0; j<max_x; j++){
            img[i][j] = 0;
        }
    }

    //gera o codigo de barras
    int boolean;
    for(int i=0; i<parametros.altura; i++){
    
    int coluna; //declaração da variavel de seleção de casa/coluna
    coluna = parametros.esp_lateral; // a coluna começa de acordo com o espaçamento
        
        boolean = 1;
        //gera o marcador de começo
        for(int j=1; j<(3 * parametros.largura_linhas)+1; j++){
            if(boolean == 1){
                img[parametros.esp_lateral + i][coluna]=1;
            }
            coluna++; //aumenta a seleção da coluna a cada casa preenchida
            //permutação do valor da variavel boolean de acordo com a largura das linhas
            if((j % parametros.largura_linhas) == 0 && boolean == 1){
                boolean = 0;
            }
            else if((j % parametros.largura_linhas) == 0 && boolean == 0){
                boolean = 1;
            }
        }
        
        //gera o Lcode
        for(int j=0; j<4; j++){ //4 numeros
            for(int k=0; k<7; k++){ //7 digitos para cada numero do lcode
                for(int l=0; l<parametros.largura_linhas; l++){
                    if(numeros_lcode[id[j]][k] ==  '1'){ //cada linha do array de strings lcode representa seu numero decimal correspondente
                        img[parametros.esp_lateral + i][coluna] = 1;
                        
                    }
                coluna++; //aumenta a seleção da coluna a cada casa preenchida
                }
            } 
        }
        
        //marcador de meio
        boolean = 0;
        for(int j=1; j<(5 * parametros.largura_linhas)+1; j++){
            if(boolean == 1){
                img[parametros.esp_lateral + i][coluna]=1;
            }
            coluna++; //aumenta a seleção da coluna a cada casa preenchida
            //permutação do valor da variavel boolean de acordo com a largura das linhas
            if((j % parametros.largura_linhas) == 0 && boolean == 1){
                boolean = 0;
            }
            else if((j % parametros.largura_linhas) == 0 && boolean == 0){
                boolean = 1;
            }
        }

        //Rcode
        for(int j=4; j<8; j++){ //4 numeros
            for(int k=0; k<7; k++){ //7 digitos para cada numero do rcode
                for(int l=0; l<parametros.largura_linhas; l++){
                    if(numeros_rcode[id[j]][k] ==  '1'){ //cada linha do array de strings rcode representa um numero decimal
                        img[parametros.esp_lateral + i][coluna] = 1;
                    }
                    coluna++; //aumenta a seleção da coluna a cada casa preenchida
                }
            } 
        }

        //marcador de final
        for(int j=1; j<(3 * parametros.largura_linhas)+1; j++){
            if(boolean == 1){
                img[parametros.esp_lateral + i][coluna]=1;
            }
            coluna++; //aumenta a seleção da coluna a cada casa preenchida
            //permutação do valor da variavel boolean de acordo com a largura das linhas
            if((j % parametros.largura_linhas) == 0 && boolean == 1){
                boolean = 0;
            }
            else if((j % parametros.largura_linhas) == 0 && boolean == 0){
                boolean = 1;
            }
        }
    }
    return img; //retorna a matriz imagem preenchida
}

//leitor
void traduz_lcode(char **codigos){
    int *l_numbers = (int *)malloc(4 * sizeof(int));
    int contador = 0;

    for(int i=0; i<4; i++){
        for(int j=0; j<10; j++){
            if(strcmp(codigos[contador], numeros_lcode[j]) == 0){
                l_numbers[i] = j;
                contador++;
                printf("%d", j);
                break;
            }
        }  
    }
    free(l_numbers); 
}
void traduz_rcode(char **codigos){
    int *r_numbers = (int *)malloc(4 * sizeof(int));
    int contador = 0;

    for(int i=0; i<4; i++){
        for(int j=0; j<10; j++){
            if(strcmp(codigos[contador], numeros_rcode[j]) == 0){
                r_numbers[i] = j;
                contador++;
                printf("%d", j);
                break;
            }
        }  
    }
    free(r_numbers);
}
parametros_struct ler_parametros(FILE *arquivo){
    //lê o tipo de arquivo, caso seja invalido dá exit
    parametros_struct parametros;
    char lixeira;
    char tipo_arquivo_letra;
    int tipo_arquivo_numero; 
    fscanf(arquivo, "%c %d %c", &tipo_arquivo_letra, &tipo_arquivo_numero, &lixeira );
    if(tipo_arquivo_letra != 'P' || tipo_arquivo_numero != 1){
        printf("Arquivo invalido");
        exit(0);
    }
    // lê os parametros (largura das linhas, largura e altura)
    fscanf(arquivo, "%d %d %d %d", &parametros.largura_linhas, &parametros.esp_lateral, &parametros.largura, &parametros.altura);

    return parametros;
}
char** ler_arquivo(FILE *arquivo, parametros_struct parametros){
    //aloca dinamicamente a matriz imagem
    char **matriz_img = (char**)malloc(sizeof(char*) * parametros.altura);
    for(int i=0; i<parametros.altura; i++){
        matriz_img[i] = (char*)malloc(sizeof(char) * parametros.largura);
    }

    //preenche a matriz imagem
    int i=0, j=0;    
    while(fscanf(arquivo, "%c", &matriz_img[i][j]) != -1){
        j++;
        if(j == parametros.largura){
            j=0;
            i++;
        }
        if(i == parametros.altura){
            break;
        }
    }        
        
    return matriz_img;
}
void ler_matriz_codigo(char **matriz_img, parametros_struct parametros){
    //faz uma linha de codigo de barras de acordo com a matriz dada
    char *linha_codigo = (char*)malloc(sizeof(char) * 67);
    int contador = 0;
    for(int i=0; i<67; i++){
        linha_codigo[i] = matriz_img[parametros.esp_lateral][contador + parametros.largura_linhas + 2*parametros.esp_lateral];
        contador += parametros.largura_linhas;
    }

    //extrai o lcode
    char **l_codigos = (char**)malloc(sizeof(char*) * 4);
    for(int i=0; i<4; i++){
        l_codigos[i] = (char*)malloc(sizeof(char) * 7);
    }

    contador = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<7; j++){
            l_codigos[i][j] = linha_codigo[contador + 3];
            contador++;
        }
    }

    //extrai o rcode
    char **r_codigos = (char**)malloc(sizeof(char*) * 4);
    for(int i=0; i<4; i++){
        r_codigos[i] = (char*)malloc(sizeof(char) * 7);
    }

    contador = 0;
    for(int i=0; i<4; i++){
        for(int j=0; j<7; j++){
            r_codigos[i][j] = linha_codigo[contador + 36];
            contador++;
        }
    }

    printf("Código identificador: ");
    // traduz e printa o identificador do codigo de barras
   traduz_lcode(l_codigos);
   traduz_rcode(r_codigos);
   printf("\n");

    //libera memoria alocada dinamicamente
   free(linha_codigo);
    for(int i=0; i<parametros.altura; i++){
        free(matriz_img[i]);
    }
    free(matriz_img);

    for(int i=0; i<4; i++){
        free(l_codigos[i]);
    }
    free(l_codigos);
    for(int i=0; i<4; i++){
        free(r_codigos[i]);
    }
    free(r_codigos);
}
