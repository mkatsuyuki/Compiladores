#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//Funcao para verificar se o char eh espaco, tab ou /n
_Bool Espaco(char c){
    if((c == '\n')| (c == '\t') | (c ==' ')){
	return 1;
    }
    else{
	return 0;
    }

}


//identificar se o char eh uma letra
_Bool Identificador(char c){
    char letras[52] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i=0;
    while(i<52){    
	if(c == letras[i]){
		return 1;
    	}
	i++;
    }
    return 0;
}


//identificar se o char eh um numero
_Bool Numero(char c){
    char numeros[10] = "0123456789";
    int i=0;
    while(i<10){    
	if(c == numeros[i]){
	    return 1;
    	}
	i++;
    }
    return 0;
}

//identificar se eh um operador
_Bool Operador(char c){
    char operadores[13] = "<>=+-*/;,:().";
    int i=0;
    while(i<13){    
	if(c == operadores[i]){
	    return 1;
    	}
	i++;
    }
    return 0;
}

//identificar se eh comentario
_Bool Comentario(char c){
    if(c == '{'){
	return 1;
    }
    else
        return 0;
}




//encontra o Identificador
int AutomatoId(char *buffer, int * contador, int * controle, FILE *f, char *palavra,FILE *erros){

    char letras[62] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int verificador=0,i=0,tamanhoId=0;

while(1){
    //verifica se o proximo eh letra
    while(i<62){    
        if(buffer[*(contador)] == letras[i]){
	    palavra[tamanhoId] = letras[i]; 
	    tamanhoId++;verificador++;
            break;
    	}
    i++;
    }
    if(verificador ==0){break;}
    verificador = 0;*(contador)=*(contador)+1;i=0;*(controle)=*(controle)+1;
    if(*contador == 200){
	*contador = 0;
	fread(buffer,200,1,f);
    }
    if(tamanhoId == 200){fprintf(erros,"Identificador ultrapassou o tamanho maximo! , %s\n",palavra);break;}
}	
palavra[tamanhoId] = '\0';
return(0);

}





//encontra o numero 
int AutomatoNum(char *buffer, int * contador, int * controle, FILE *f, char *palavra, FILE *erros){


    unsigned int verificador=0,tamanhoId=0,real = 0;

while(1){
    //verifica se o proximo eh numero
        if(Numero(buffer[*(contador)])){
	    palavra[tamanhoId] = buffer[*(contador)]; 
	    tamanhoId++;verificador++;
	}

    if(buffer[*(contador)] == '.'){
	verificador++;real++;palavra[tamanhoId] = '.';tamanhoId++;
	if(!Numero(buffer[*(contador) +1])){real++; }
    }
    if(verificador ==0){break;}
    verificador = 0;     *(contador)=*(contador)+1;    *(controle)=*(controle)+1;
    if(*contador == 200){
	*contador = 0;
	fread(buffer,200,1,f);
    }
    if(tamanhoId == 200){fprintf(erros,"Numero ultrapassou o tamanho maximo! , %s\n",palavra);break;}
}	
palavra[tamanhoId] = '\0';
if(real>1){fprintf(erros,"Numero escrito errado! %s\n",palavra);}
if(real >=1){return 36;}
else{return 35;}

}


//encontra o operador 
int AutomatoOp(char *buffer, int * contador, int * controle, FILE *f, char *palavra){

    char operandos1[9] = "+-*/;,().";
    char operandos2[4] = ":<>=";
    int i=0,tamanhoId=0, qual = 0,verifica=0;

    //verifica qual operador eh
    while(i<9){    
        if(buffer[*(contador)] == operandos1[i]){
	    palavra[tamanhoId] = operandos1[i];tamanhoId++; 
            *(contador)=*(contador)+1;*(controle)=*(controle)+1;qual = i;
	    verifica++;break;
    	}
    i++;
    }
    i=0;
    if(verifica==0){
    while(i<4){    
        if(buffer[*(contador)] == operandos2[i]){
	    palavra[tamanhoId] = operandos2[i];tamanhoId++; 
            *(contador)=*(contador)+1;*(controle)=*(controle)+1;qual = 9+i;
	    if(*contador == 200){
		*contador = 0;
		fread(buffer,200,1,f);
    	    }
	    if(buffer[*(contador)] == '='){	    
		palavra[tamanhoId] = '=';tamanhoId++; qual+=4;
	        *(contador)=*(contador)+1;*(controle)=*(controle)+1;
	    }
	    else if(palavra[tamanhoId-1] == '<' && buffer[*(contador)] == '>'){
		palavra[tamanhoId] = buffer[*(contador)];tamanhoId++; qual=17;
	        *(contador)=*(contador)+1;*(controle)=*(controle)+1;		
	    }
            break;
    	}
    i++;
    }
    }

	
palavra[tamanhoId] = '\0';
return 17 + qual;
}






int AutomatoComentario(char *buffer, int * contador, int * controle, FILE *f, char *palavra,int tamanho, FILE *erros){

    while(buffer[*contador] != '}'){
	*contador = *contador +1;
	*controle = *controle +1;
	if(*contador == 200){
	    *contador = 0;
	    fread(buffer,200,1,f);
    	}
	if(*controle >= tamanho - 1){
	    fprintf(erros,"Erro, Nao fechou o comentario \n");palavra[0] = '{'; palavra[1] = '\0'; return 37;
	}	
    }
    *(contador)+=1;*controle = *controle +1;palavra[0] = '{';palavra[1] = '}';palavra[2] = '\0';
    return 37;

}





int Lexico(char *buffer,int *controle, int *contador,FILE * F, char *palavra, int tamanho, FILE *erros){



//consumimos todos os espacos
    while(Espaco(buffer[*contador])){
	*contador = *contador +1;*controle=*controle +1;
        //checa se o buffer ja estorou e le os proximos
        if(*contador == 200){
	    fread(buffer,200,1,F);
	    *contador = 0;
        }
    }	
    if(*controle >= tamanho-1){return(40);}

//Vamos descobrir qual caso iremos tratar
    if(Identificador(buffer[*contador])){	
	return(AutomatoId(buffer,contador,controle,F,palavra,erros));
    }
    else if(Numero(buffer[*contador])){
	return(AutomatoNum(buffer,contador,controle,F,palavra,erros));
    }
    else if(Operador(buffer[*contador])){
	return(AutomatoOp(buffer,contador,controle,F,palavra));	
    }
    else if(Comentario(buffer[*contador])){
	return(AutomatoComentario(buffer,contador,controle,F,palavra,tamanho,erros));
		
    }
    else{
	palavra[0] = buffer[*(contador)];palavra[1] = '\0';
	*contador = *contador +1;*controle=*controle +1;
	fprintf(erros,"Erro, nao pertence ao alfabeto %s.\n", palavra);
	return(38);
    }


}











//FUNCAO MAIN
int main(int argc, char *argv[]){

FILE * F, *analisado, *erros;

char buffer[200], palavra[200];
//palavras reservadas
char Reservadas[17][30] ={"program","var","const","real","integer","procedure","begin","end","else","read",
"write","while","do","if","then","for","to"};

char Classificacao[39][30] = {"Programa","variavel","constante","real","inteiro","procedure","begin","end","else","read",
"write","while","do","if","then","for","to","mais","menos","multiplicacao","divisao","PeV",
"Virgula","abreP","fechaP","Ponto","doisP","Menor","Maior",
"Igual","DPIgual","menorIgual","maiorIgual","igualIgual","MaiorMenor","numInt","numReal","COMENTARIO","erro(caracter nao permitido)"};


int contador=0, tamanho, controle =0,i,j,verificador=0;

F = fopen(argv[1],"r");
analisado = fopen("analise.txt","w");
erros = fopen("erros.txt","w");

//Descobre o tamanho do arquivo
fseek(F, 0, SEEK_END);
tamanho = ftell(F);
fseek(F, 0, 0);

fread(buffer,200,1,F);





//enquanto nao alcancar o tamanho do arquivo
while(controle < tamanho-1){
    i = Lexico(buffer,&controle, &contador,F,palavra,tamanho,erros);

    if(i==40){break;}
    if(i>16){
	fprintf(analisado,"%s",palavra);
	fprintf(analisado,", %s\n", Classificacao[i]);
    }
    else{
        for(j=0;j<17;j++){
    	    if(strcmp(Reservadas[j], palavra)==0){
	    	fprintf(analisado,"%s",palavra);
	    	fprintf(analisado,", %s\n",Classificacao[j]);verificador++;
	    }
	}
	if(verificador==0){
	    	fprintf(analisado,"%s",palavra);
	    	fprintf(analisado,", Identificador\n");	
	}
	verificador=0;
    }

}

fclose(F);



return 0;
}












