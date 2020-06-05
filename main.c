#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Lexico.h"
#include "Sintatico.h"

//FUNCAO MAIN
int main(int argc, char *argv[]){

FILE * F,  *erros;

char palavra[200], buffer[200],classificacao[200];
//palavras reservadas

int contador=0, tamanho, controle =0,linha=1;

F = fopen(argv[1],"r");
erros = fopen("erros.txt","w");

//Descobre o tamanho do arquivo
fseek(F, 0, SEEK_END);
tamanho = ftell(F);
fseek(F, 0, 0);
fread(buffer,200,1,F);

sintatico(buffer,&controle, &contador,F,palavra,tamanho,erros,classificacao,&linha);

/*Para testar o lexico usar abaixo
while(controle < tamanho-1){
    Classe(buffer,&controle, &contador,F,palavra,tamanho,erros,classificacao,&linha);
    if(i!=1){    printf("Palavra:  %s,   Classificacao:   %s,   Linha: %d \n",palavra,classificacao,linha);}

}
*/
fseek(erros, 0, SEEK_END);
tamanho = ftell(erros);

if(tamanho <= 1){
    fprintf(erros,"Compilado com sucesso!\n");
}
    
    
fclose(F);
fclose(erros);

return 0;
}


