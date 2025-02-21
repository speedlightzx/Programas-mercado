#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>

void inserirdadosINT(int *variavel) { //função para inserir dados inteiros. Qualquer dado inserido sem ser um numero inteiro, não é validado
  char input[100];
      do {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

       int teste = sscanf(input, "%d", variavel);
       if (teste == 1) break;
        
    } while (1);       
} 

void inserirdadosDOUBLE(double *variavel) { //função para inserir dados quebrados(double). Qualquer dado inserido sem ser um numero quebrado, não é validado
  char input[100];
      do {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

       int teste = sscanf(input, "%lf", variavel);
       if (teste == 1) break;
        
    } while (1);       
}

int main() {

	sqlite3 *db; //definindo o ponteiro para a database
  char *msgerro = 0; //mensagem de erro
  sqlite3_stmt *handle_sql = 0; //handle para executar comandos
  sqlite3_stmt *handle_sql2 = 0; //handle para executar comandos
	
	int database = sqlite3_open("database.db3", &db); //abrindo o banco de dados
	
	if(database != SQLITE_OK) { //se tiver acontecido algum erro, o programa vai retornar o erro, e em seguida fechar
    printf("Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    system("pause");
    return 1;
	}

bool loopprincipal = true; //loop do programa inteiro
int opcao = 0; //variável de opção para o switch do menu principal

//variáveis gerais
int codigop; //variável para fazer pesquisas

//variáveis de GERENCIAR COLABORADORES
int op1 = 0; //selecionar opções dentro do menu de gerenciar colaboradores
char nome[51]; //variável para armazenar nome no cadastro de colaborador
int matricula_cadastrar; //variável para armazenar a matrícula de colaborador
int del; //variável para armazenar matrícula de colaborador para apagá-la

//opção dois variaveis
int op2 = 0; //selecionar opções dentro do menu de produtos
char produtonome[100]; //variável para armazenar o nome de um produto para cadastro
double valor;  //variável para armazenar o valor de um produto para cadastro
int codigobarras; //variável para armazenar o código de barras de um produto para cadastro
int codigomenor; //variável para armazenar o código menor de um produto para cadastro
double valornovo; //variável para armazenar o valor novo de um produto que terá o preço alterado
int kgun; //variável para armazenar a escolha feita se o produto é vendido por KG ou UNIDADE
double peso; //variável para armazenar o peso de um produto para cadastro caso seja vendido por KG

//opcao tres variaveis
int op3 = 0; //selecionar opções dentro do menu de estoque
int addestoque = 0; //variável para armazenar a quantidade de novos produtos de um determinado produto no estoque
int restoque = 0; //variável para armazenar a quantidade que saiu de estoque de um determinado produto no estoque

do {

printf("=================================\n");
printf("      MENU PRINCIPAL\n");
printf("=================================\n\n");

puts("(1): Gerenciar colaboradores");
puts("(2): Produtos");
puts("(3): Estoque");
puts("(4): Fechar");
inserirdadosINT(&opcao); //registrando opção inserida pelo usuário

switch (opcao) { //switch que permite navegar entre todos os menus
#pragma region // GERENCIAR COLABORADORES

case 1:       

system("cls");
bool loop1 = true; // loop do menu de gerenciar colaboradores

  do {

printf("=================================\n");
printf("      Gerenciar Colaboradores\n");
printf("=================================\n\n");

    puts("(1): Cadastrar novo colabolador");
    puts("(2): Remover colaborador");
    puts("(3): Voltar");
    inserirdadosINT(&op1); //registrando opção inserida pelo usuário

    switch(op1) { //switch que permite navegar entre os menus de GERENCIAR COLABORADORES

#pragma region // CADASTRAR NOVO COLABORADOR

      case 1:
      system("cls");

      printf("Digite o nome do colaborador: ");
      scanf(" %[^\n]%*c", nome); //armazena o nome do colaborador a ser registrado

      printf("Digite a matricula do funcionario: ");
      inserirdadosINT(&matricula_cadastrar); //armazena o nome do colaborador a ser registrado

      char select[] = "SELECT * FROM funcionarios WHERE matricula=?";//comando no banco de dados para pesquisar se já existe a matrícula inserida

      int p = sqlite3_prepare_v2(db, select, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, matricula_cadastrar);

      p = sqlite3_step(handle_sql);//realiza a pesquisa

      if(p == SQLITE_ROW) { //matrícula encontrada, ou seja, essa matrícula já foi cadastrada

        printf("\n\nEssa matricula ja foi cadastrada.\n\n");
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);
        system("pause");
        system("cls");
        break;

      } else { //matrícula não encontrada, então a matrícula inserida será cadastrada

      char insert[] = "INSERT INTO funcionarios(nome, matricula) values(?, ?)"; //comando no banco de dados para armazenar o nome e a matrícula inseridos
      int add = sqlite3_prepare_v2(db, insert, -1, &handle_sql2, 0);
      sqlite3_bind_text(handle_sql2, 1, nome, -1, SQLITE_TRANSIENT);
      sqlite3_bind_double(handle_sql2, 2, matricula_cadastrar);

      add = sqlite3_step(handle_sql2); //realizando cadastro
    
        printf("\n\nColaborador cadastrado com sucesso.\n\n"); //cadastro feito
        sqlite3_finalize(handle_sql2);
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);
        system("pause");
        system("cls");
        break;
      }

    break;
#pragma endregion
#pragma region //REMOVER COLABORADOR

case 2:

      system("cls");

      printf("Digite a matricula do funcionario: ");
      inserirdadosINT(&del); //armazenando a matrícula inserida
      
        char cmd[] = "SELECT * FROM funcionarios WHERE matricula=?"; //comando no banco de dados para pesquisar a matrícula inserida
      int rc = sqlite3_prepare_v2(db, cmd, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, del);
      rc = sqlite3_step(handle_sql); //realizando pesquisa

      if(rc == SQLITE_ROW) { //matrícula encontrada, e em seguida, sendo apagada do banco de dados
        printf("\n\nColaborador '%s' da matricula '%d' foi removido com sucesso.\n\n", sqlite3_column_text(handle_sql, 0), sqlite3_column_int(handle_sql, 1));
        
      char delete[] = "DELETE FROM funcionarios WHERE matricula=?"; //comando no banco de dados para apagar a matrícula
      int rc = sqlite3_prepare_v2(db, delete, -1, &handle_sql2, 0);
      sqlite3_bind_int(handle_sql2, 1, del);
      rc = sqlite3_step(handle_sql2); // realizando comando e apagando os dados
        
        sqlite3_finalize(handle_sql);
        sqlite3_finalize(handle_sql2);
        sqlite3_free(msgerro);

        system("pause");
        system("cls");
      } else { //matrícula não encontrada
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);

        printf("\n\nNao foi encontrado nenhum funcionario cadastrado com essa matricula.\n\n");
        system("pause");
        system("cls");
      }
        break;

#pragma endregion
#pragma region //VOLTAR

    case 3:
    //limpando o conteúdo na tela, e quebrando o loop1, que é o loop do menu de gerenciar colaboradores
    system("cls");
    loop1 = false;
    break;

#pragma endregion
#pragma region //DEFAULT
    default:
    //qualquer opção diferente de 1, 2 ou 3. Caracteres não são validados
    puts("Opcao invalida.\n");
    system("pause");
    system("cls");
    break;
#pragma endregion //FIM DEFAULT

    }

  } while(loop1 == true);

    break;

#pragma endregion // FIM GERENCIAR COLABORADORES
#pragma region //PRODUTOS

case 2:

system("cls");
bool loop2 = true; // loop do menu de produtos
do {

printf("=================================\n");
printf("            Produtos\n");
printf("=================================\n\n");

    puts("(1): Cadastrar novo produto");
    puts("(2): Remover produto cadastrado");
    puts("(3): Alterar preco");
    puts("(4): Voltar");
    inserirdadosINT(&op2); //registrando opção inserida pelo usuário

    switch(op2) {

#pragma region //ADICIONAR NOVO PRODUTO
      case 1:
      system("cls");

      printf("Digite o nome do produto: ");
      scanf(" %[^\n]%*c", produtonome); //armazenando o nome do produto para cadastro

      printf("Digite o valor desse produto: "); 
      inserirdadosDOUBLE(&valor); //armazenando o valor do produto para cadastro                 

      printf("Digite o codigo de barras: "); 
      inserirdadosINT(&codigobarras); //armazenando o código de barras do produto para cadastro

      printf("Digite o codigo menor: ");
      inserirdadosINT(&codigomenor); //armazenando o código menor do produto para cadastro

      if(codigobarras == 0 || codigobarras == 1 || codigomenor == 0 || codigomenor == 1) {
        //verificando se o código de barras ou menos do produto é 1 ou 0. Caso for, o produto não poderá ser cadastrado
        //Por causa que 1 e 0 são utilizados para remover produto e realizar pagamento no software do caixa
        printf("\n\nUm produto nao pode ter o codigo cadastrado como 1 ou 0.\n\n");
        system("pause");
        system("cls");
        break;
      }

      char select[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?"; //Comando no banco de dados
      //Para verificar se já existe um produto com algum dos códigos já registrados


      int p = sqlite3_prepare_v2(db, select, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, codigobarras);
      sqlite3_bind_int(handle_sql, 2, codigomenor);

      p = sqlite3_step(handle_sql); //realizando pesquisa

      if(p == SQLITE_ROW) {//Já existe um produto com esse código, então o produto não será cadastrado
        printf("\n\nJa existe um produto cadastrado com esse codigo de barras ou codigo menor(alternativo).\n\n");
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);
        system("pause");
        system("cls");
        break;
      } 
      //Caso não tenha encontrado nenhum produto com esses códigos, o cadastro irá prosseguir

      printf("O produto e vendido por quilogramas(1) ou unidade(0)? ");
      inserirdadosINT(&kgun); //armazenando o valor da escolha

      if(kgun == 1) { //produto sendo vendido por KG(opção 1)
        peso = 1;
        printf("\nO produto sera vendido por KG R$%0.2f\n", valor);
      } else if(kgun == 0) { //produto sendo vendido por UNIDADE(opção 0)
        peso = 0;
      } else { //Qualquer valor diferente de 1 ou 0 o cadastro será cancelado
        printf("\n\nOpcao invalida.\n");
        system("pause");
        system("cls");
        break;
      }

        char insertcmd[] = "INSERT INTO produtos(nome, preço, codigobarras, codigomenor, peso) values(?, ?, ?, ?, ?)";//comando no banco de dados
        int addrc = sqlite3_prepare_v2(db, insertcmd, -1, &handle_sql2, 0);
      sqlite3_bind_text(handle_sql2, 1, produtonome, -1, SQLITE_TRANSIENT); //cadastrando nome
      sqlite3_bind_double(handle_sql2, 2, valor); //cadastrando preço
      sqlite3_bind_int(handle_sql2, 3, codigobarras); //cadastrando código de barras
      sqlite3_bind_int(handle_sql2, 4, codigomenor); //cadastrando código menor
      sqlite3_bind_int(handle_sql2, 5, peso); //cadastrando se é vendido por peso(1) ou unidade(0)

      addrc = sqlite3_step(handle_sql2); //Realizando cadastro do produto
    
        printf("\n\nProduto cadastrado com sucesso.\n\n"); //Produto cadastrado
        sqlite3_finalize(handle_sql);
        sqlite3_finalize(handle_sql2);
        sqlite3_free(msgerro);
        system("pause");
        system("cls");

      break;

#pragma endregion
#pragma region  //REMOVER PRODUTO CADASTRADO
      case 2:
      system("cls");

      printf("Digite o codigo de barras ou o codigo menor do produto: ");
      inserirdadosINT(&codigop); //armazenando o valor do código do produto

      char delcmd[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?"; //comando no banco de dados para pesquisar se o código existe no banco de dados
      int delrc = sqlite3_prepare_v2(db, delcmd, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, codigop);
      sqlite3_bind_int(handle_sql, 2, codigop);
      delrc = sqlite3_step(handle_sql); //realizando pesquisa

      if(delrc == SQLITE_ROW) { //o código inserido foi encontrado
        printf("\n\nFoi removido o cadastro do produto '%s'\n\n", sqlite3_column_text(handle_sql, 0));
        
      char comando[] = "DELETE FROM produtos WHERE rowid = (SELECT rowid FROM produtos WHERE codigobarras=? OR codigomenor=?)"; //comando no banco de dados para pesquisar o código
      int delrc = sqlite3_prepare_v2(db, comando, -1, &handle_sql2, 0);
      sqlite3_bind_int(handle_sql2, 1, codigop);
      sqlite3_bind_int(handle_sql2, 2, codigop);
      delrc = sqlite3_step(handle_sql2); //apagando o produto do banco de dados

        sqlite3_finalize(handle_sql);
        sqlite3_finalize(handle_sql2);
        sqlite3_free(msgerro);
        
        system("pause");
        system("cls");
      } else { //o código inserido não foi encontrado
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);
        printf("\nNao foi encontrado nenhum produto cadastrado com esse codigo.\n\n");
        system("pause");
        system("cls");
      }

      break;

#pragma endregion
#pragma region  //ALTERAR PREÇO
      case 3:

system("cls");

      printf("Digite o codigo de barras ou o codigo menor do produto: ");
      inserirdadosINT(&codigop); //armazenando o código inserido

      char valorcmd[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?"; //comando no banco de dados para pesquisar se o código existe
      int attrc = sqlite3_prepare_v2(db, valorcmd, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, codigop);
      sqlite3_bind_int(handle_sql, 2, codigop);
      attrc = sqlite3_step(handle_sql); //realizando pesquisa

      if(attrc == SQLITE_ROW) { //código inserido foi encontrado

      printf("\n\n===================\n");
      printf("Produto a ser alterado:\n");
      printf("Nome: %s\n", sqlite3_column_text(handle_sql, 0));
      printf("Valor atual: %0.2f", sqlite3_column_double(handle_sql, 1));
      printf("\n===================\n\n"); //Informações do produto, nome e valor atual

      printf("Digite o novo valor desse produto: ");
      inserirdadosDOUBLE(&valornovo); //armazenar o novo preço do produto
        
      char comando[] = "UPDATE produtos SET preço=? WHERE codigobarras=? OR codigomenor=?"; //comando no banco de dados para alterar o preço
      int attrc = sqlite3_prepare_v2(db, comando, -1, &handle_sql2, 0);
      sqlite3_bind_double(handle_sql2, 1, valornovo);
      sqlite3_bind_int(handle_sql2, 2, codigop);
      sqlite3_bind_int(handle_sql2, 3, codigop);
      attrc = sqlite3_step(handle_sql2); //alterando preço
      printf("\n\nO valor do produto '%s' foi alterado de '%0.2f' para '%0.2f'\n\n", sqlite3_column_text(handle_sql, 0), sqlite3_column_double(handle_sql, 1), valornovo);
        //Preço alterado com sucesso
        sqlite3_finalize(handle_sql);
        sqlite3_finalize(handle_sql2);
        sqlite3_free(msgerro);

        system("pause");
        system("cls");
      } else { //código inserido não foi encontrado
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);
        printf("\nNao foi encontrado nenhum produto cadastrado com esse codigo.\n\n");
        system("pause");
        system("cls");
      }
    break;

 #pragma endregion
#pragma region //VOLTAR
      case 4:
      //limpando o conteúdo da tela, e quebrando o loop2, que é o loop do menu de produtos
    system("cls");
    loop2 = false;
      break;

#pragma endregion
#pragma region //DEFAULT
    default:
    //qualquer opção diferente de 1, 2, 3 ou 4. Caracteres não são validados
    puts("Opcao invalida.\n");
    system("pause");
    system("cls");
    break;
#pragma endregion
     }

} while(loop2 == true);

    break;

#pragma endregion //FIM PRODUTOS
#pragma region //ESTOQUE
case 3: 
    system("cls");    
    bool loop3 = true;

    do {


printf("=================================\n");
printf("      Estoque\n");
printf("=================================\n\n");

    puts("(1) Adicionar estoque");
    puts("(2) Remover estoque");
    puts("(3) Verificar produto");
    puts("(4) Voltar");
    inserirdadosINT(&op3); //registrando opção inserida pelo usuário

    switch(op3) {

    #pragma region //ADICIONAR ESTOQUE
    case 1:
    system("cls");

    printf("Digite o codigo de barras ou o codigo menor do produto: ");
    inserirdadosINT(&codigop); //armazenando o código inserido para pesquisa

      char estoquecmd1[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?"; //comando no banco de dados para pesquisar se o código existe
      int pesquisa = sqlite3_prepare_v2(db, estoquecmd1, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, codigop);
      sqlite3_bind_int(handle_sql, 2, codigop);
      pesquisa = sqlite3_step(handle_sql); //realizando pesquisa

      if(pesquisa == SQLITE_ROW) { //código inserido foi encontrado

    printf("Digite a quantidade a ser adicionada no estoque: ");
    inserirdadosINT(&addestoque); //armazenando a quantidade a ser adicionada no estoque
    
    int estoqueanterior = sqlite3_column_int(handle_sql, 4);
    int addtotal = estoqueanterior + addestoque; //somando a quantidade que já tinha no estoque, com o valor inserido

      char estoquecmd2[] = "UPDATE produtos SET estoque=? WHERE codigobarras=? OR codigomenor=?";//comando no banco de dados para atualizar a quantidade
      int estoquerc = sqlite3_prepare_v2(db, estoquecmd2, -1, &handle_sql2, 0);
      sqlite3_bind_int(handle_sql2, 1, addtotal);
      sqlite3_bind_int(handle_sql2, 2, codigop);
      sqlite3_bind_int(handle_sql2, 3, codigop);
      estoquerc = sqlite3_step(handle_sql2); //Adicionado mais do produto no estoque
      printf("\n\nFoi adicionado mais %d %s no estoque\n\n", addestoque, sqlite3_column_text(handle_sql, 0));
        
        sqlite3_finalize(handle_sql);
        sqlite3_finalize(handle_sql2);
        sqlite3_free(msgerro);

        system("pause");
        system("cls");
      } else { //código inserido não foi encontrado
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);
        printf("\nNao foi encontrado nenhum produto cadastrado com esse codigo.\n\n");
        system("pause");
        system("cls");
      }
    break;

    #pragma endregion
    #pragma region //REMOVER ESTOQUE
    case 2: 

       system("cls");

        printf("Digite o codigo de barras ou o codigo menor do produto: ");
    inserirdadosINT(&codigop); //armazenando o código inserido para pesquisa

      char restoquecmd1[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?"; //comando no banco de dados para pesquisar se o código existe
      int rpesquisa = sqlite3_prepare_v2(db, restoquecmd1, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, codigop);
      sqlite3_bind_int(handle_sql, 2, codigop);
      rpesquisa = sqlite3_step(handle_sql); //realizando pesquisa

      if(rpesquisa == SQLITE_ROW) { //código foi encontrado

    printf("Digite a quantidade a ser removida do estoque: ");
    inserirdadosINT(&restoque); //armazenando a quantidade a ser removida do estoque
    
    int estoqueanterior2 = sqlite3_column_int(handle_sql, 4);
    int rtotal = estoqueanterior2 - restoque; //subtraindo a quantidade que já tinha no estoque, com o valor inserido

      char restoquecmd2[] = "UPDATE produtos SET estoque=? WHERE codigobarras=? OR codigomenor=?"; //comando para atualizar o estoque no banco de dados
      int estoquerc = sqlite3_prepare_v2(db, restoquecmd2, -1, &handle_sql2, 0);
      sqlite3_bind_int(handle_sql2, 1, rtotal);
      sqlite3_bind_int(handle_sql2, 2, codigop);
      sqlite3_bind_int(handle_sql2, 3, codigop);
      estoquerc = sqlite3_step(handle_sql2); //banco de dados atualizado. Removido a quantidade inserida
      printf("\n\nFoi removido %d %s do estoque\n\n", restoque, sqlite3_column_text(handle_sql, 0));
        
        sqlite3_finalize(handle_sql);
        sqlite3_finalize(handle_sql2);
        sqlite3_free(msgerro);

        system("pause");
        system("cls");
      } else { //código não foi encontrado

        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);

        printf("\nNao foi encontrado nenhum produto cadastrado com esse codigo.\n\n");
        system("pause");
        system("cls");
      }
    break;

    #pragma endregion
    #pragma region //VERIFICAR PRODUTO
    case 3: 

           system("cls");

        printf("Digite o codigo de barras ou o codigo menor do produto: ");
    inserirdadosINT(&codigop); //armazenando o código inserido para pesquisa

      char verificarcmd[] = "SELECT * FROM produtos WHERE codigobarras=? OR codigomenor=?"; //comando no banco de dados para pesquisar se o código existe
      int verificar = sqlite3_prepare_v2(db, verificarcmd, -1, &handle_sql, 0);
      sqlite3_bind_int(handle_sql, 1, codigop);
      sqlite3_bind_int(handle_sql, 2, codigop);
      verificar = sqlite3_step(handle_sql); //realizando pesquisa

      if(verificar == SQLITE_ROW) { //código foi encontrado
   
       printf("\n\n===================\n");
       printf("Nome: %s\n", sqlite3_column_text(handle_sql, 0)); //Nome do produto
       printf("Preco: %0.2f\n", sqlite3_column_double(handle_sql, 1)); //Preço do produto
       printf("Codigo de barras: %d\n", sqlite3_column_int(handle_sql, 2)); //Código de barras do produto
       printf("Codigo menor: %d\n", sqlite3_column_int(handle_sql, 3)); //Código menor do produto
       printf("Quantidade no estoque: %d\n", sqlite3_column_int(handle_sql, 4)); //Quantidade desse produto no estoque
if(sqlite3_column_int(handle_sql, 5) == 1) printf("Produto vendido por KG\n"); //Se o produto é vendido por KG
if(sqlite3_column_int(handle_sql, 5) == 0) printf("Produto vendido por UNIDADE\n"); //Se o produto é vendido por UNIDADE
       printf("===================\n\n");
        
        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);

        system("pause");
        system("cls");
      } else { //código não foi encontrado

        sqlite3_finalize(handle_sql);
        sqlite3_free(msgerro);

        printf("\nNao foi encontrado nenhum produto cadastrado com esse codigo.\n\n");
        system("pause");
        system("cls");
      }

    break;
    #pragma endregion
    #pragma region //VOLTAR
    case 4: 
    //limpando o conteúdo da tela, e quebrando o loop3, que é o loop do menu de estoque
    loop3 = false;
    system("cls");

    break;
    #pragma endregion
    #pragma region //DEFAULT

    default:
    //qualquer opção diferente de 1, 2, 3 ou 4. Caracteres não são validados
    puts("Opcao invalida.\n");
    system("pause");
    system("cls");
    break;

    #pragma endregion

    }

    } while(loop3 == true);
    
    break;
    #pragma endregion //FIM ESTOQUE
#pragma region //FECHAR
case 4:
    //Programa fecha após 3 segundos, e o loopprincipal é quebrando, e o programa será fechado.
    printf("\nFechando programa....\n");
    sleep(3);
    loopprincipal = false;
    break;
#pragma endregion

default:
//qualquer opção diferente de 1, 2, 3 ou 4. Caracteres não são validados
    puts("Opcao invalida.\n");
    system("pause");
    system("cls");
    break;
}

} while(loopprincipal == true);

}