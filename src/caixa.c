#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

#define FILENAME_SIZE 1024
#define MAX_LINE 2048

void inserirdadosINT(int *variavel) { //função para inserir dados inteiros. Qualquer dado inserido sem ser um numero inteiro, não é validado
  char input[100];
      do {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

       int teste = sscanf(input, "%d", variavel);
       if (teste == 1) break;
        
    } while (1);       
}

void inserirdadosDOUBLE(double *variavel) { //função para inserir dados inteiros. Qualquer dado inserido sem ser um numero inteiro, não é validado
  char input[100];
      do {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

       int teste = sscanf(input, "%lf", variavel);
       if (teste == 1) break;
        
    } while (1);       
}

void RegistrarProduto(int *variavel_codigo, int *variavel_qnt) { //função para registar produtos, podendo pegar até dois valores, o código do produto(obrigatório), e a quantidade(opcional)
  char input[100];
      do {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

       int teste = sscanf(input, "%d %d", variavel_codigo, variavel_qnt);
       if (teste == 1 || teste == 2) break;
        
    } while (1);       
}

int main() {

  sqlite3 *db;
  char *msgerro = 0;
  sqlite3_stmt *handle_sql = 0;
   sqlite3_stmt *handle_sql2 = 0;
	
	int rc = sqlite3_open("database.db3", &db); //abrindo a database sqlite
	
	if(rc != SQLITE_OK) { //printar o erro caso tenha algum problema ao abrir a database
    printf("Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
	}

  char listacompras[] = "DROP TABLE listacompras";
  sqlite3_exec(db, listacompras, NULL, 0, &msgerro); //executa o comando de apagar a tabela "listacompras" da database
sqlite3_free(msgerro);

  char listacompras1[] = "CREATE TABLE listacompras(nome TEXT, preço REAL, codigobarras INT, codigomenor INT, peso INT, quantidade INT)";
  sqlite3_exec(db, listacompras1, NULL, 0, &msgerro); //cria a tabela "listacompras" com as informações de nome, preço, codigo de barras, codigo menor, peso e quantidade
sqlite3_free(msgerro);

#pragma region //TELA DE LOGIN DO CAIXA

bool login = true;

do {//loop com a váriavel login, para fazer o login no caixa, caso erre, o programa ainda não fecha
    printf("============================\n");
    printf("    ENTRADA DE CAIXA\n");
    printf("============================\n\n");
    int matricula;
    char input2[100];

    printf("Insira sua matricula de funcionario: ");//print para o usuário inserir a matrícula
    inserirdadosINT(&matricula); //registrando a matrícula inserida
  
    char cmd[] = "SELECT * FROM funcionarios WHERE matricula=?"; //comando no banco de dados para válidar a matrícula inserida

    rc = sqlite3_prepare_v2(db, cmd, -1, &handle_sql, 0);
    rc = sqlite3_bind_int(handle_sql, 1, matricula); 
    rc = sqlite3_step(handle_sql); //realizando pesquisa

    if(rc == SQLITE_ROW) { //matrícula encontrada, então o caixa será aberto
            printf("Abrindo caixa...");
            sleep(3);
            system("cls");
            login = false;
    } else { //matrícula não foi encontrada
          printf("\nNenhum funcionario cadastrado com essa matricula.\n\n");
          system("pause");
          system("cls");
    }

sqlite3_free(handle_sql);
sqlite3_free(msgerro);

} while(login == true);
#pragma endregion

bool comprando = true; //loop principal para manter o caixa aberto

    system("cls");
    printf("======================================================================================\n");
    printf("                           CAIXA ABERTO\n");
    printf("======================================================================================\n( Digite 1 para realizar o pagamento. || Digite 0 para remover um produto. )\n\n");

 int codigo = 0; //variável do código do produto
 double total = 0; //variável do valor total da compra

   do {
int qnt = 1; //quantidade de produtos inseridos de uma só vez, no caso, 1 por padrão,

     if(total < 0) total = 0;
        printf("Digite o codigo do produto: ");
        RegistrarProduto(&codigo, &qnt); //registrando o produto com a função

    if(codigo == 0) { //se o código inserido for 0, o programa entenderá que é para remover um produto
      
    int rcodigo = 0;
    printf("\nDigite o codigo do produto que deseja remover: ");
    inserirdadosINT(&rcodigo); //registrando o código inserido para remover da tabela de "listacompras" da database

    char cmd[] = "SELECT * FROM listacompras WHERE codigobarras=? OR codigomenor=?"; //pesquisando se foi cadastrado um produto com esse código

    int pesquisadelrc = sqlite3_prepare_v2(db, cmd, -1, &handle_sql, 0);
    pesquisadelrc = sqlite3_bind_int(handle_sql, 1, rcodigo);
    pesquisadelrc = sqlite3_bind_int(handle_sql, 2, rcodigo);
    pesquisadelrc = sqlite3_step(handle_sql); //realizando pesquisa
    
    if(pesquisadelrc == SQLITE_ROW) { //foi encontrado o código já cadastrado
      double peso = sqlite3_column_double(handle_sql, 4); //peso do produto cadastrado
      int quantidade = sqlite3_column_int(handle_sql, 5); //quantidade cadastrada do produto
      double valor = (sqlite3_column_double(handle_sql, 1)); //valor do produto

      total = total - valor; //subtraindo o valor do produto com o total

          printf("\n================\nREMOVIDO:\n");
           printf("%s - R$%0.2f\n", sqlite3_column_text(handle_sql, 0), sqlite3_column_double(handle_sql, 1)); //nome e preço cheio do produto
           if(peso != 0) printf("PESO: %0.3f - R$%0.2f\n", peso, valor); //peso e preço calculado por KG
           if(quantidade > 1) printf("Quantidade: %d\n", quantidade); //quantidade, se for maior que 1
           printf("TOTAL: R$%0.2f\n", total); //total atual da compra
           printf("================\n\n");

          char comando[] = "DELETE FROM listacompras WHERE rowid = (SELECT rowid FROM listacompras WHERE codigobarras=? OR codigomenor=? LIMIT 1)"; //comando no banco de dados para remover o produto cadastrado da tabela "listacompras"
          int delrc = sqlite3_prepare_v2(db, comando, -1, &handle_sql2, 0);
          delrc = sqlite3_bind_int(handle_sql2, 1, rcodigo);
          delrc = sqlite3_bind_int(handle_sql2, 2, rcodigo);
          delrc = sqlite3_step(handle_sql2); //produto apagado da tabela de "listacompras"

sqlite3_finalize(handle_sql);
sqlite3_finalize(handle_sql2);

    } else { //não foi encontrado um produto cadastrado com esse código
      sqlite3_finalize(handle_sql);
      printf("\nNao foi encontrado um produto com esse codigo.\n");
    }

    } else if(codigo == 1) { //se o código inserido for 1, o programa entenderá que é para realizar o pagamento da compra

    system("cls");
   printf("==================\n");
   printf(" PAGAMENTO\n");
   printf("==================\n");
   printf("( Digite 0 para voltar. )\n\n");

bool pagamento = true; //loop do pagamento

double totalinteiro = total; // variável para armazenar o valor da variável total
double pagamentointeiro = 0; // variável para armazenar o valor de cada pagamento que receber
double pagamento1 = 0; // variável para armazenar o valor recebido

do {

printf("O valor total da compra foi de: %0.2f\n", total); //Mostrando na tela o valor total da compra
printf("Insira o valor recebido: ");
inserirdadosDOUBLE(&pagamento1); //registrando os valores recebidos
printf("\n");

if(pagamento1 == 0) { //Inserindo 0 na tela de compras, o programa irá retornar a tela de registro de compras
 system("cls");

    system("cls");
    printf("======================================================================================\n");
    printf("                           CAIXA ABERTO\n");
    printf("======================================================================================\n( Digite 1 para realizar o pagamento. || Digite 0 para remover um produto. )\n\n");

 break;
}

total = total - pagamento1; //subtraindo o total com o pagamento recebido
pagamentointeiro = pagamento1 + pagamentointeiro; //somando os valores recebidos e armazenando eles na variável de pagamentointeiro

if(total <= 0) { //quando o total chegar a 0 ou menor, a compra é finalizada, e o troco é mostrado na tela, e em seguida o programa será fechado
 printf("\nTROCO: %0.2f\n", pagamentointeiro - totalinteiro);
 sleep(3);
 system("pause");
 comprando = false;
 break;
}

} while(pagamento == true);

    } else { //Inserindo um código diferente de 0 ou 1, o programa registra normalmente qualquer produto

    char cmd[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?"; //comando no banco de dados para pesquisar o código inserido

    int addrc = sqlite3_prepare_v2(db, cmd, -1, &handle_sql, 0);
    addrc = sqlite3_bind_int(handle_sql, 1, codigo);
    addrc = sqlite3_bind_int(handle_sql, 2, codigo);
    addrc = sqlite3_step(handle_sql); //realizando pesquisa

    if(addrc == SQLITE_ROW) { //código existe, então o produto será registrado
      int peso = sqlite3_column_int(handle_sql, 5); //produto vendido por peso ou unidade. (1=peso), (2=unidade)
      double valor = 0; 
      double pesagem = 0;

      if(peso == 1) { //produto vendido por peso, então o programa irá solicitar o peso
        printf("Insira o peso do produto em KG: ");
        inserirdadosDOUBLE(&pesagem); //registrando o peso inserido

        double soma = (sqlite3_column_double(handle_sql, 1) * pesagem); //fazendo o cálculo. Multiplicando o preço do KG pelo peso
        valor = soma; //atribuindo o resultado da soma para a variável de valor
        if(qnt > 1) valor = valor * qnt; //se tiver mais de um produto, a variável valor será multiplicado pela quantidade
        total = total + valor; //atribuindo o valor multiplicado no total
      } else { //se o produto não for vendido por peso
        valor = sqlite3_column_double(handle_sql, 1); 
         if(qnt > 1) valor = valor * qnt; //multiplica o valor registrado no banco de dados pela quantidade, caso tenha mais de um produto
          total = total + valor; //atribui o valor multiplicado no total
      }

           printf("\n================\nADICIONADO:\n");
           printf("%s - R$%0.2f\n", sqlite3_column_text(handle_sql, 0), sqlite3_column_double(handle_sql, 1)); //Nome e preço cheio do produto
           if(peso != 0) printf("PESO: %0.3f - R$%0.2f\n", pesagem, valor); //Peso inserido e o preço calculado do peso. Caso o produto seja vendido por KG
           if(qnt > 1) printf("Quantidade: %d - R$%0.2f\n", qnt, valor); //Quantidade e o preço multiplicado pela quantidade. Caso tenha inserido um valor maior que 1 na quantidade
           printf("TOTAL: R$%0.2f\n", total); //Total atual da compra
           printf("================\n\n");

      char comando[] = "INSERT INTO listacompras(nome, preço, codigobarras, codigomenor, peso, quantidade) values(?, ?, ?, ?, ?, ?)"; //comando no banco de dados para registrar o produto na tabela de "listacompras"
      int listarc = sqlite3_prepare_v2(db, comando, -1, &handle_sql2, 0);
      sqlite3_bind_text(handle_sql2, 1, sqlite3_column_text(handle_sql, 0), -1, SQLITE_TRANSIENT); //registrando o nome do produto
      sqlite3_bind_double(handle_sql2, 2, valor); //registrando o valor do produto
      sqlite3_bind_int(handle_sql2, 3, sqlite3_column_int(handle_sql, 2)); //registrando o código de barras do produto
      sqlite3_bind_int(handle_sql2, 4, sqlite3_column_int(handle_sql, 3)); //registrando o código menor do produto
      sqlite3_bind_double(handle_sql2, 5, pesagem); //registrando o peso inserido do produto
      sqlite3_bind_int(handle_sql2, 6, qnt); //registrando a quantidade do produto

      listarc = sqlite3_step(handle_sql2); //produto registrado na tabela "listacompras"

      sqlite3_finalize(handle_sql);
sqlite3_finalize(handle_sql2);

    } else { //código inserido não foi encontrado
      sqlite3_finalize(handle_sql);
      printf("\nNao foi encontrando um produto com esse codigo.\n");
   }
     }

   } while(comprando == true);
    
} 