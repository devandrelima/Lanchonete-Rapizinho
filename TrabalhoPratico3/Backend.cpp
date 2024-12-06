#define _CRT_SECURE_NO_WARNINGS
#include "Backend.h"
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

//----------------------------------------------------------------------------------------
// Descrição_: Função que expande o vetor da lista de pedidos.   
// Entrada___: lista -> Vetor que será expandido.
//			   indiceNovo -> Número que representa o novo índice que poderá ser preenchido 
// Saída_____: ponteiro de Pedido.
//----------------------------------------------------------------------------------------
Pedido* expandirVetorPedidos(Pedido* lista, int indiceNovo) {

	// Ao inicializar, preenche o espaço em branco que falta no novo espaço
	Pedido* novaLista = new Pedido[indiceNovo + 1]{0};	 // Para o índice novo ser criado o vetor precisa ter o tamanho 1 unidade maior.
														 // Exemplo: Um vetor com tamanho 5 só vai até o índice 4, caso se deseje 
														 // que ele vá até o índice 5, o vetor precisa ter tamanho 6. 

	for (int i = 0; i < indiceNovo; i++) {				 // 'for' que grava vetor menor no maior e deixa o espaço novo em branco.
		novaLista[i].opcao = lista[i].opcao;
		novaLista[i].produt.quantidade = lista[i].produt.quantidade;
		strcpy(novaLista[i].produt.nome, lista[i].produt.nome);
		novaLista[i].produt.preco = lista[i].produt.preco;
	}

	delete[]lista;
	return novaLista;
}

//----------------------------------------------------------------------------------------
// Descrição_: Função que expande o vetor de produtos do estoque.   
// Entrada___: estoque -> Vetor que será expandido.
//			   indiceNovo -> Número que representa o novo índice que poderá ser preenchido 
// Saída_____: ponteiro de Produto.
//----------------------------------------------------------------------------------------
Produto* expandirVetorEstoque(Produto* estoque, int indiceNovo) {
	Produto* novaLista = new Produto[indiceNovo + 1]{0}; 
 
	for (int i = 0; i < indiceNovo; i++) {	// 'for' que grava vetor menor no maior e deixa o espaço novo em branco.
		strcpy(novaLista[i].nome, estoque[i].nome);
		novaLista[i].quantidade = estoque[i].quantidade;
		novaLista[i].preco = estoque[i].preco;
	}
	
	delete[]estoque;

	return novaLista;
}

//------------------------------------------------------------------------------------
// Descrição_: Função que transforma um número inteiro em seu caractere representante.   
// Entrada___: num -> Número que será transformado em caractere.
// Saída_____: char.
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
// Descrição_: Função que transforma um número inteiro em uma string para pôr no nome do arquivo.   
// Entrada___: num -> Número que será transformado em string.
// Saída_____: ponteiro de char.
//-----------------------------------------------------------------------------------------------
 char* intParaString(int num) {
	 char numPedido[3] = { 0 };

	// Converte até o inteiro 99 para string.
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
// Descrição_: Função que gera um recibo com a numeração do pedido
// Entrada___: numPedido -> Número do pedido para ser colocado no recibo;
//			   pedidos[] -> Lista dos pedidos que o cliente realizou;
// 			   tamListaPedidos -> Número que representa o tamanho do vetor;
//			   total -> Valor total das compras;
//			   porcentDesconto -> Porcentagem que depende da forma de pagamento;
//			   desconto -> Quanto o cliente ganhou de desconto pelo valor de suas compras;
//			   taxaEntrega -> Valor da taixa para entrega.
// Saída_____: Sem retorno.
//----------------------------------------------------------------------------------------
 void gerarRecibo(int numPedido, Pedido pedidos[],int tamListaPedidos, float total, int porcentDesconto, float desconto, float taxaEntrega) {
	ofstream fout;

	char nomeArquivo[20] = "pedido_";
	char numeroPedido[4] = {0};
	strcpy(numeroPedido, intParaString(numPedido));
	char extensao[5] = ".txt";

	// Monta o nome do arquivo com o número de pedido.
	strcat(nomeArquivo, numeroPedido);
	strcat(nomeArquivo, extensao);

	// Cria um arquivo txt e registra o número de pedido nele.
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
// Descrição_: Função que exibe o caractere espaço em loop.   
// Entrada___: qntEspacos -> Número de espaços a ser exibidos;
// Saída_____: Sem retorno.
//------------------------------------------------------------
 void escreverEspacos(int qntEspacos) {
	 if (qntEspacos > 0) {
		 while (qntEspacos--) {
			 cout << ' ';
		 }
	 }
 }