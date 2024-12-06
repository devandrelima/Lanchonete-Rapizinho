#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "Backend.h"
#define PRIMEIRA_POSICAO_MENU 65 // Letra A das opc�es do Menu
#define TAXA_ENTREGA 6
#define DESCONTO10 0.9
#define DESCONTO5 0.95

using namespace std;

void tituloLimpaTela();
char telaMenu(Produto estoque[], Pedido listaPedidos[], unsigned tamEstoque, int tamLista);
int telaPedido(Produto comida[], int qntReservada, char opcao);
void visualizarListaPedidos(Produto estoque[], Pedido lista[], int tamLista);
char telaPagamento(Produto estoque[], Pedido lista[], int tamLista, float total);
char telaConfirmarPedido(Produto estoque[], Pedido lista[], int tamLista, float total, char formaPagamento);

//----------------------------------------------------------------------
// Descri��o_: Fun��o principal desse arquivo, pois chama todas as telas
// Entrada___: void
// Sa�da_____: void
//----------------------------------------------------------------------
void corpoRapizinho() {
	char formaPagamento = ' ';
	char escolha = ' ';
	float total = 0;
	float desconto = 0;
	int porcentDesconto = 0;
	int tamListaPedidos = 0;
	int numPedido = 0;
	int indice = 0;
	int qntReservada = 0;
	unsigned tamEstoque = 0;
	bool crescerLista;
	Pedido pedidoCliente = {};
	Produto* estoque;
	Produto produt = {};
	ifstream fin;
	ofstream fout;

	// Testa se o arquivo bin�rio existe ou n�o.
	fin.open("estoque.bin", ios_base::in | ios_base::binary);

	if (!fin.is_open()) {
		// Caso n�o exista, exibe um aviso e encerra o programa.
		cout << "\nErro na abertura do arquivo de estoque.\n";
		exit(EXIT_FAILURE);
	}
	else {
		// Caso exista, � alocado mem�ria dinamicamente para preencher o vetor com os produtos
		// registrados no arquivo bin�rio.
		fin.read((char*)&tamEstoque, sizeof(unsigned));
		estoque = new Produto[tamEstoque];

		while (fin.read((char*)&produt, sizeof(produt))) {

			strcpy(estoque[indice].nome, produt.nome);
			estoque[indice].quantidade = produt.quantidade;
			estoque[indice].preco = produt.preco;

			++indice;
		}
		fin.close();
	}

	do {
		tamListaPedidos = 0;
		total = 0;
		Pedido* listaPedidos = new Pedido[1]{ 0 };

		tituloLimpaTela(); // Limpa o prompt e exibe o t�tulo do aplicativo

		do {
			// Exibe a tela de menu para escolha dos pedidos.
			pedidoCliente.opcao = telaMenu(estoque,listaPedidos,tamEstoque,tamListaPedidos);

			if (pedidoCliente.opcao != 'S') {
				qntReservada = 0;
				crescerLista = true;

				// Os pedidos escolhidos s�o reservados para que o cliente n�o acabe comprando uma quantidade que n�o est� no estoque.
				strcpy(pedidoCliente.produt.nome, estoque[pedidoCliente.opcao - PRIMEIRA_POSICAO_MENU].nome);
				for (int i = 0; i < tamListaPedidos + 1; i++) {
					if (strcmp(pedidoCliente.produt.nome, listaPedidos[i].produt.nome) == 0) {
						qntReservada = listaPedidos[i].produt.quantidade;
					}
				}

				// Pre�o do produto escolhido � preenchido no vetor de pedidos  
				pedidoCliente.produt.preco = estoque[pedidoCliente.opcao - PRIMEIRA_POSICAO_MENU].preco;

				// Exibe tela para escolher a quantidade que o cliente deseja, limitando sua escolha quanto
				// ao que est� dispon�vel no estoque.
				pedidoCliente.produt.quantidade = telaPedido(estoque, qntReservada, pedidoCliente.opcao);

				// Verifica se o cliente est� pedindo um produto que j� est� na lista de pedidos.
				// Caso j� esteja, apenas aumenta a quantidade do que j� estava.
				// Caso n�o esteja, � liberado o acesso para preencher a vaga dispon�vel do vetor e
				// � solicitado o aumento do vetor para caber um produto novo na lista de pedidos.
				for (int i = 0; i < tamListaPedidos+1; i++) {
					if (strcmp(pedidoCliente.produt.nome, listaPedidos[i].produt.nome) == 0) {
						listaPedidos[i].produt.quantidade += pedidoCliente.produt.quantidade;
						crescerLista = false;
					}
				} 

				if (crescerLista) {
					// Preenche a �ltima posi��o do vetor
					strcpy(listaPedidos[tamListaPedidos].produt.nome, pedidoCliente.produt.nome);
					listaPedidos[tamListaPedidos].produt.preco = pedidoCliente.produt.preco;
					listaPedidos[tamListaPedidos].produt.quantidade = pedidoCliente.produt.quantidade;
					listaPedidos[tamListaPedidos].opcao = pedidoCliente.opcao;

					// Ao preencher todo o vetor dispon�vel, � solicitado seu aumento para outro poss�vel pedido
					tamListaPedidos++;
					listaPedidos = expandirVetorPedidos(listaPedidos, tamListaPedidos);	
				}

				tituloLimpaTela();
				visualizarListaPedidos(estoque, listaPedidos, tamListaPedidos);
			}

		} while (pedidoCliente.opcao != 'S');

		// Se a op��o escolhida for 'S' e o tamanho da lista de pedidos � 0, o programa inicia uma rota de fechamento
		// para encerrar o programa de forma adequada, sem gravar pedidos e sem interferir no estoque.
		if (tamListaPedidos != 0) { 

			// Calcula total gasto pelo cliente
			for (int i = 0; i < tamListaPedidos; i++) {
				total += listaPedidos[i].total;
			}
			total += TAXA_ENTREGA;

			// Exibe a tela para escolher a forma de Pagamento.
			tituloLimpaTela();
			formaPagamento = telaPagamento(estoque, listaPedidos, tamListaPedidos, total);

			// Exibe a tela para confirmar o pedido com todos os dados necesss�rios listados.
			escolha = telaConfirmarPedido(estoque, listaPedidos, tamListaPedidos, total, formaPagamento);

			if (escolha == 'S') {
				
				if (formaPagamento == 'P') {
					porcentDesconto = 10;
					desconto = total - (total*DESCONTO10);
				} else {
					porcentDesconto = 5;
					desconto = total - (total * DESCONTO5);
				}
				total -= desconto;

				// Salva as compras em um arquivo txt com o n�mero do pedido. 
				gerarRecibo(++numPedido, listaPedidos, tamListaPedidos, total, porcentDesconto, desconto, TAXA_ENTREGA);

				// Atualiza o vetor de estoque, subtraindo a quantidade dos produtos comprados.
				for (int i = 0; i < tamListaPedidos; i++) {
					for (int j = 0; j < tamEstoque; j++) {
						if (strcmp(estoque[j].nome, listaPedidos[i].produt.nome) == 0) {
							estoque[j].quantidade -= listaPedidos[i].produt.quantidade;
						}
					}
				}

				// Atualiza arquivo bin�rio de estoque
				fout.open("estoque.bin", ios_base::out | ios_base::trunc | ios_base::binary);

				fout.write((char*)&tamEstoque, sizeof(unsigned));

				for (int i = 0; i < tamEstoque; i++) {
					fout.write((char*)&estoque[i], sizeof(Produto));
				}
				fout.close();
			}
		}
		delete[]listaPedidos;
		
	} while (tamListaPedidos > 0);

	delete[]estoque;
	cout << "\n -------------- Pedidos Encerrados --------------\n";
}

//--------------------------------------------------------
// Descri��o_: Fun��o que exibe o cabe�alho do aplicativo
// Entrada___: void
// Sa�da_____: void
//--------------------------------------------------------
void tituloLimpaTela() {
	system("cls");
	cout << " RapiZinho \n";
	cout << "===========\n";
}

//--------------------------------------------------------------------------------------------
// Descri��o_: Fun��o que lista os produtos dispon�veis no estoque para o cliente escolher.
// Entrada___: estoque[] -> Vetor de produtos baseado no arquivo bin�rio;
//			   listaPedidos[] -> Lista dos pedidos que o cliente est� requisitando; 
//			   tamEstoque -> Diz o tamanho do vetor de estoque proveniente do arquivo bin�rio;
//			   tamLista -> Diz o tamanho do vetor da lista dos pedidos do cliente.
// Sa�da_____: Char que representa a letra do produto pedido pelo cliente. 
//--------------------------------------------------------------------------------------------
char telaMenu(Produto estoque[], Pedido listaPedidos[], unsigned tamEstoque, int tamLista) {
	int qntReservada;
	bool saida = true;
	char letra = PRIMEIRA_POSICAO_MENU;
	char opcao;

	cout << endl; 
	// Lista os produtos, listando-os de 'A' at� a letra que representa a quantidade 
	// de produtos presentes no estoque. 
	for(int i = 0; i < tamEstoque; i++){
		cout << '(' << letra++ << ')' << ' ' << estoque[i].nome << endl;
	}
	cout << "(S) Sair\n\n";
	cout << "===========\n";

	do{
		do { // Verifica se a entrada realmente � uma das op��es dispon�veis
			cout << "Op��o: [ ]\b\b";
			cin >> opcao;
			opcao = toupper(opcao);

			// Analisa se a letra escolhida est� entre A e D, por exemplo
			if (opcao >= PRIMEIRA_POSICAO_MENU && opcao < letra || opcao == 'S') {
				saida = false;
			}
			else {
				cout << "\nOp��o Inv�lida, tente outra letra.\n";
			}

		} while (saida);

		qntReservada = 0;

		// Verifica se esse produto j� est� reservado para n�o exceder o estoque.
		for (int i = 0; i < tamLista + 1; i++) { 
			if (strcmp(estoque[opcao - PRIMEIRA_POSICAO_MENU].nome, listaPedidos[i].produt.nome) == 0) {
				qntReservada = listaPedidos[i].produt.quantidade;
			}
		}

		// Solicita uma nova op��o caso a requisitada j� esteja zerada por falta no estoque.
		if ((estoque[opcao - PRIMEIRA_POSICAO_MENU].quantidade - qntReservada) == 0 && opcao != 'S') {
			cout << "Est� em falta no estoque, escolha outra op��o do Menu.\n";
			saida = true;
		}
	} while (saida);

	return opcao;
}

//-----------------------------------------------------------------------
// Descri��o_: Fun��o que exibe os dados do pedido feito pelo cliente
// Entrada___: estoque[] -> Vetor de produtos baseado no arquivo bin�rio;
//			   qntReservada -> Quantidade j� reservada desse produto;
//			   opcao -> Letra escolhida pelo cliente na tela de menu.
// Sa�da_____: Inteiro que representa a quantidade comprada 
//			   pelo cliente de um determinado produto
//-----------------------------------------------------------------------
int telaPedido(Produto estoque[], int qntReservada, char opcao) {
	int qntComprada;
	bool saida = true;

	// Exibe os dados do produto que o cliente escolheu
	cout << "\n\nPedido\n";
	cout << "=======\n";
	cout << estoque[opcao - PRIMEIRA_POSICAO_MENU].nome << endl;
	cout << fixed;
	cout.precision(2);
	cout << "R$ " << estoque[opcao - PRIMEIRA_POSICAO_MENU].preco << endl;
	cout << "=======\n";

	do { 
		cout << "Quantidade: [   ]\b\b\b\b";
		cin >> qntComprada;

		// Verifica se a quantidade solicitada pelo cliente est� dispon�vel para compra.
		if (qntComprada > 0 && qntComprada <= (estoque[opcao - PRIMEIRA_POSICAO_MENU].quantidade - qntReservada)) {
			saida = false;
		}
		else {
			cout << "\nQuantidade Inv�lida.\n";
		}
	} while (saida);
	
	return qntComprada;
}

//---------------------------------------------------------------------------------
// Descri��o_: Fun��o que lista os pedidos realizados pelo cliente, 
//			   calcundo o total a ser gasto em cada produto escolhido.
// Entrada___: estoque[] -> Vetor de produtos baseado no arquivo bin�rio;
//			   listaPedidos[] -> Lista dos pedidos que o cliente est� requisitando; 
//			   tamLista -> Diz o tamanho do vetor da lista dos pedidos do cliente.
// Sa�da_____: void;
//---------------------------------------------------------------------------------
void visualizarListaPedidos(Produto estoque[], Pedido listaPedidos[], int tamLista) {
	// Exibe a lista.
	for (int i = 0; i < tamLista; i++) {
		// Calcula o total a ser gasto com determinado produto
		listaPedidos[i].total = estoque[listaPedidos[i].opcao - PRIMEIRA_POSICAO_MENU].preco * listaPedidos[i].produt.quantidade;

		// Exibe 1 produto com suas devidas informa��es conforme as escolhas do cliente.
		cout << listaPedidos[i].produt.quantidade << " x " << estoque[listaPedidos[i].opcao - PRIMEIRA_POSICAO_MENU].nome
			<< " de R$ " << estoque[listaPedidos[i].opcao - PRIMEIRA_POSICAO_MENU].preco << " = R$ "
			<< listaPedidos[i].total << endl;
	}
}

//---------------------------------------------------------------------------------
// Descri��o_: Fun��o que exibe a tela de pagamento para o cliente visualizar as  
//			   informa��es gerais das compras e escolher a forma de pagamento.
// Entrada___: estoque[] -> Vetor de produtos baseado no arquivo bin�rio;
//			   listaPedidos[] -> Lista dos pedidos que o cliente est� requisitando; 
//			   tamLista -> Diz o tamanho do vetor da lista dos pedidos do cliente;
//			   total -> Soma dos valores dos produtos pedidos.
// Sa�da_____: Char que representa a escolha de pagamento do cliente.
//---------------------------------------------------------------------------------
char telaPagamento(Produto estoque[], Pedido listaPedidos[], int tamLista, float total) {
	bool saida = true;
	char formaPagamento;

	visualizarListaPedidos(estoque, listaPedidos, tamLista);
	cout << "\nTaxa de Entrega = R$ " << TAXA_ENTREGA;
	cout << "\n===========";
	cout << "\nTotal = R$ " << total;
	cout << "\n\n[P] Pix";
	cout << "\n[C] Cart�o";
	
	do { 
		cout << "\n\nPagamento: [ ]\b\b";
		cin >> formaPagamento;
		formaPagamento = toupper(formaPagamento);

		// Verifica se a op��o escolhida corresponde a uma forma pagamento existente.
		if (formaPagamento == 'P' || formaPagamento == 'C') { 
			saida = false;
		}
		else {
			cout << "\nOp��o Inv�lida, tente outra forma de pagamento.\n";
		}
	} while (saida);

	return formaPagamento;
}

//---------------------------------------------------------------------------------------
// Descri��o_: Fun��o que exibe todas as informa��es das compras realizadas pelo cliente,
//			   adicionando um desconto conforme a forma de pagamento escolhida.
// Entrada___: estoque[] -> Vetor de produtos baseado no arquivo bin�rio;
//			   listaPedidos[] -> Lista dos pedidos que o cliente est� requisitando; 
//			   tamLista -> Diz o tamanho do vetor da lista dos pedidos do cliente;
//			   total -> Soma dos valores dos produtos pedidos.
//			   formaPagamento -> Forma de pagamento para definir o desconto a ser dado.
// Sa�da_____: Char que define se o pedido ser� realizado e gravado em um recibo ou n�o.
//---------------------------------------------------------------------------------------
char telaConfirmarPedido(Produto estoque[], Pedido listaPedidos[], int tamLista, float total, char formaPagamento) {
	bool saida = true;
	char escolha = ' ';

	tituloLimpaTela();
	visualizarListaPedidos(estoque, listaPedidos, tamLista);
	cout << "\nTaxa de Entrega = R$ " << TAXA_ENTREGA;

	// Calcula o desconto conforme a forma de pagamento.
	if (formaPagamento == 'P') {
		cout << "\nDesconto de 10% = R$ " << total - (total* DESCONTO10);
		total *= DESCONTO10;
	}
	else {
		cout << "\nDesconto de 5% = R$ " << total - (total * DESCONTO5);
		total *= DESCONTO5;
	}

	cout << "\n===========";
	cout << "\nTotal = R$ " << total;

	do { 
		cout << "\n\nConfirmar Pedido(S/N): [ ]\b\b";
		cin >> escolha;
		escolha = toupper(escolha);

		// Analisa se a letra escolhida � uma das requisitadas pelo programa.
		if (escolha == 'S' || escolha == 'N') { 
			saida = false;
		}
		else {
			cout << "\nOp��o Inv�lida.\n";
		}
	} while (saida);

	return escolha;
}

