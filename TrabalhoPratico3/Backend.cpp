#define _CRT_SECURE_NO_WARNINGS
#include "Backend.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

//----------------------------------------------------------------------------------------
// Descri��o_: Fun��o que expande o vetor da lista de pedidos.   
// Entrada___: lista -> Vetor que ser� expandido.
//			   indiceNovo -> N�mero que representa o novo �ndice que poder� ser preenchido 
// Sa�da_____: ponteiro de Pedido.
//----------------------------------------------------------------------------------------
Pedido* expandirVetorPedidos(Pedido* lista, int indiceNovo) {

	// Ao inicializar, preenche o espa�o em branco que falta no novo espa�o
	Pedido* novaLista = new Pedido[indiceNovo + 1]{0};	 // Para o �ndice novo ser criado o vetor precisa ter o tamanho 1 unidade maior.
														 // Exemplo: Um vetor com tamanho 5 s� vai at� o �ndice 4, caso se deseje 
														 // que ele v� at� o �ndice 5, o vetor precisa ter tamanho 6. 

	for (int i = 0; i < indiceNovo; i++) {				 // 'for' que grava vetor menor no maior e deixa o espa�o novo em branco.
		novaLista[i].opcao = lista[i].opcao;
		novaLista[i].produt.quantidade = lista[i].produt.quantidade;
		strcpy(novaLista[i].produt.nome, lista[i].produt.nome);
		novaLista[i].produt.preco = lista[i].produt.preco;
	}

	delete[]lista;
	return novaLista;
}

//----------------------------------------------------------------------------------------
// Descri��o_: Fun��o que expande o vetor de produtos do estoque.   
// Entrada___: estoque -> Vetor que ser� expandido.
//			   indiceNovo -> N�mero que representa o novo �ndice que poder� ser preenchido 
// Sa�da_____: ponteiro de Produto.
//----------------------------------------------------------------------------------------
Produto* expandirVetorEstoque(Produto* estoque, int indiceNovo) {
	Produto* novaLista = new Produto[indiceNovo + 1]{0}; 
 
	for (int i = 0; i < indiceNovo; i++) {	// 'for' que grava vetor menor no maior e deixa o espa�o novo em branco.
		strcpy(novaLista[i].nome, estoque[i].nome);
		novaLista[i].quantidade = estoque[i].quantidade;
		novaLista[i].preco = estoque[i].preco;
	}
	
	delete[]estoque;

	return novaLista;
}

//------------------------------------------------------------------------------------
// Descri��o_: Fun��o que transforma um n�mero inteiro em seu caractere representante.   
// Entrada___: num -> N�mero que ser� transformado em caractere.
// Sa�da_____: char.
//------------------------------------------------------------------------------------
char convertIntChar(int num) {
	switch (num)
	{
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	default:
		return '-1';
	}
}

//-----------------------------------------------------------------------------------------------
// Descri��o_: Fun��o que transforma um n�mero inteiro em uma string para p�r no nome do arquivo.   
// Entrada___: num -> N�mero que ser� transformado em string.
// Sa�da_____: ponteiro de char.
//-----------------------------------------------------------------------------------------------
 char* intParaString(int num) {
	 char numPedido[3] = { 0 };

	// Converte at� o inteiro 99 para string.
	if (num / 10 == 0) {
		numPedido[0] = convertIntChar(num);
		numPedido[1] = '\0';
	}
	else {
		numPedido[0] = convertIntChar(num / 10);
		numPedido[1] = convertIntChar(num % 10);
		numPedido[2] = '\0';
	}
	return numPedido;
}

 //---------------------------------------------------------------------------------------
// Descri��o_: Fun��o que gera um recibo com a numera��o do pedido
// Entrada___: numPedido -> N�mero do pedido para ser colocado no recibo;
//			   pedidos[] -> Lista dos pedidos que o cliente realizou;
// 			   tamListaPedidos -> N�mero que representa o tamanho do vetor;
//			   total -> Valor total das compras;
//			   porcentDesconto -> Porcentagem que depende da forma de pagamento;
//			   desconto -> Quanto o cliente ganhou de desconto pelo valor de suas compras;
//			   taxaEntrega -> Valor da taixa para entrega.
// Sa�da_____: Sem retorno.
//----------------------------------------------------------------------------------------
 void gerarRecibo(int numPedido, Pedido pedidos[],int tamListaPedidos, float total, int porcentDesconto, float desconto, float taxaEntrega) {
	ofstream fout;

	char nomeArquivo[20] = "pedido_";
	char numeroPedido[4] = {0};
	strcpy(numeroPedido, intParaString(numPedido));
	char extensao[5] = ".txt";

	// Monta o nome do arquivo com o n�mero de pedido.
	strcat(nomeArquivo, numeroPedido);
	strcat(nomeArquivo, extensao);

	// Cria um arquivo txt e registra o n�mero de pedido nele.
	fout.open(nomeArquivo);
	if (!fout.is_open()) {
		cout << "Falha em gerar recibo!";
		exit(EXIT_FAILURE);
	}
	fout << "Pedido #" << numPedido;
	fout << "\n---------------------------------------";
	fout << fixed;
	fout.precision(2);
	for (int i = 0; i < tamListaPedidos; i++) {
		fout << endl;
		fout << pedidos[i].produt.quantidade << " x " << pedidos[i].produt.nome << " de R$" << pedidos[i].produt.preco << " = R$" << pedidos[i].total;
	}
	fout << "\nTaxa de Entrega = R$" << taxaEntrega;
	fout << "\nDesconto de " << porcentDesconto << "% = R$" << desconto;
	fout << "\n---------------------------------------";
	fout << "\n\nTotal = R$" << total;

	fout.close();
}

 //-----------------------------------------------------------
// Descri��o_: Fun��o que exibe o caractere espa�o em loop.   
// Entrada___: qntEspacos -> N�mero de espa�os a ser exibidos;
// Sa�da_____: Sem retorno.
//------------------------------------------------------------
 void escreverEspacos(int qntEspacos) {
	 if (qntEspacos > 0) {
		 while (qntEspacos--) {
			 cout << ' ';
		 }
	 }
 }