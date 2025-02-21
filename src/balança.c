#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

int main() {

  sqlite3 *db;
  char *msgerro = 0;
  sqlite3_stmt *handle_sql = 0;

  	int rc = sqlite3_open("database.db3", &db); //abrindo database sqlite
	
	if(rc != SQLITE_OK) { //printa erro no console caso tenha erro ao abrir a database
    printf("Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
	}

bool loop = true;
int pesquisa = 0;
int qnt = 0;

    do { //loop para não fechar o programa, e possibilitando pesquisar vários produtos, um após o outro

    printf("======================================================================================\n");
    printf("                           BALANCA\n");
    printf("======================================================================================\n");
    
    printf("Digite o codigo do produto: ");
    scanf("%d", &pesquisa); // printf seguido de um scanf, para indicar ao usuário para inserir um código de produto

    char cmd[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?";
    //pesquisando o código inserido no banco de dados, nas duas colunas de codigo de barras e na de codigo menor

    rc = sqlite3_prepare_v2(db, cmd, -1, &handle_sql, 0); //preparando para pesquisar na database
    rc = sqlite3_bind_int(handle_sql, 1, pesquisa); //atribuindo o codigo inserido no "codigobarras=?"
    rc = sqlite3_bind_int(handle_sql, 2, pesquisa); //atribuindo o codigo inserido no "codigomenor=?"
    rc = sqlite3_step(handle_sql); //pesquisando

        if(rc == SQLITE_ROW) { //produto encontrado

    int peso = sqlite3_column_int(handle_sql, 5); //coluna de peso do produto na database, onde se o peso for = 1, o produto é vendido por peso
    float pesagem = 0;
    float valorfinal = 0;
float valor = sqlite3_column_double(handle_sql, 1); //coluna do preço do produto na database


    printf("Digite a quantidade: ");
    scanf("%d", &qnt); //printf seguido de um scanf para inserir a quantidade desse produto

    if(peso == 1) { //se o peso desse produto na database for igual a 1, o produto é vendido por peso
        
        printf("Digite o peso em KG: ");
        scanf("%f", &pesagem); //então é necessario inserir o peso
        valorfinal = (valor * pesagem) * qnt; //calculando o preço final, multiplicando o preço cheio pelo peso, e depois pela quantidade
    
    } else { //se o peso for igual a 0, não é necessário colocar peso
        valorfinal = valor * qnt; //calculando o preço final pelo valor cheio do produto pela quantidade
    }


       printf("\n\n===================\n");
       printf("Nome: %s\n", sqlite3_column_text(handle_sql, 0)); //nome do produto pesquisado
       if(peso != 0) printf("Valor KG: %0.2f\n", valor); //valor em KG do produto(valor cheio), se for vendido por peso
       printf("Valor: R$%0.2f\n", valorfinal); //valor do produto já multiplicado
       printf("Quantidade: %d\n", qnt); //quantidade do produto
       if(peso != 0) printf("Peso: %0.3f\n", pesagem); //peso do produto(se for vendido por peso)
       printf("===================\n\n");

system("pause");
system("cls");

    } else {//produto não encontrado
      printf("\nNao foi encontrando um produto com esse codigo.\n");
     system("pause");
     system("cls");
   }
    sqlite3_finalize(handle_sql);
    sqlite3_free(msgerro);

    
    } while(loop == true);


}