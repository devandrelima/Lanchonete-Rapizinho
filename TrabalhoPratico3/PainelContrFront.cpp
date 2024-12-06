#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "Backend.h"
using namespace std;

Produto* telaAdicionar(Produto* estoque, unsigned* tamEstoque);
Produto* telaExcluir(Produto* estoque, unsigned* tamEstoque);
void telaListar(Produto* estoque, unsigned tamEstoque);

//----------------------------------------------------------------------
// Descri��o_: Fun��o principal desse arquivo, pois chama todas as telas
// Entrada___: void
// Sa�da_____: void
//----------------------------------------------------------------------
void corpoPainelControle() {
	ifstream fin;
	ofstream fout;
	Produto* estoque;
	Produto produt = {};
	char opcao;
	bool saida = true;
	unsigned tamEstoque = 0;
	int indice = 0;

	// Testa se o arquivo bin�rio existe ou n�o.
	fin.open("estoque.bin", ios_base::in | ios_base::binary);
	if (!fin.is_open()) {
		// Caso n�o exista, exibe um aviso e inicia o ponteiro com nullptr.
		system("cls");
		cout << "Ainda n�o existe arquivo de estoque, por favor adicione produtos para criar um.\n";
		system("pause");
		estoque = nullptr;
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
		system("cls");
		cout << "\n Painel de Controle";
		cout << "\n====================";
		cout << "\n(A)dicionar";
		cout << "\n(E)xcluir";
		cout << "\n(L)istar";
		cout << "\n(S)air";
		cout << "\n====================";
		cout << "\n\nOp��o: [ ]\b\b";

		cin >> opcao;
		opcao = toupper(opcao);

			// Chama uma fun��o tela dependendo da op��o escolhida.
			switch (opcao) {
			case 'A':
				estoque = telaAdicionar(estoque, &tamEstoque);
				break;
			case 'E':
				estoque = telaExcluir(estoque, &tamEstoque);
				break;
			case 'L':
				telaListar(estoque, tamEstoque);
				break;
			case 'S':
				saida = false;
				break;
			default:
				// Caso a op��o n�o exista, exibe mensagem e repete o la�o,
				// pedindo uma nova entrada de dados.
				cout << "Op��o Inv�lida, tente novamente.";
				system("pause");
				break;
			}
		} while (saida);

		// Atualiza (caso exista) ou cria (caso n�o exista) o arquivo bin�rio.
		cout << "\nAtualizando arquivo de estoque...";

		fout.open("estoque.bin", ios_base::out | ios_base::trunc | ios_base::binary);

		fout.write((char*)&tamEstoque, sizeof(unsigned));

		for (int i = 0; i < tamEstoque; i++) {
			fout.write((char*)&estoque[i], sizeof(Produto));
		}
			
		fout.close();

		cout << "Controle de Estoque finalizado.\n";
		delete[]estoque;
}

//----------------------------------------------------------------------------------------------------------
// Descri��o_: Fun��o que aloca dinamicamente o vetor de produtos, caso o arquivo bin�rio n�o exista;
//			   essa fun��o tamb�m atualiza o pre�o e a quantidade do Produto, caso ele j� exista no vetor;
//			   e caso o Produto ainda n�o exista no vetor, expande o vetor em 1 unidade e adiciona o Produto.
// Entrada___: estoque -> Ponteiro que representa o in�cio do vetor din�mico (caso o vetor j� exista) ou � o
//						  ponteiro que ser� utilizado para realocar dinamicamente (caso o vetor n�o exista);
//			   tamEstoque -> Ponteiro que diz o tamanho do estoque.
// Sa�da_____: Retorna o vetor que � alocado dinamicamente.
//-----------------------------------------------------------------------------------------------------------
Produto* telaAdicionar(Produto* estoque, unsigned* tamEstoque) {
	char nomeProduto[24];
	bool produtoExiste = false;
	int indiceProdutoExistente = 0;
	int qntAdicionada = 0;

	if(estoque == nullptr) estoque = new Produto[1];

	system("cls");
	cout << "Adicionar";
	cout << "\n----------";
	cout << "\nProduto: ";
	cin.get();
	cin.getline(nomeProduto, 24);

	// Verifica se existe algum produto na lista com o nome solicitado e sai do la�o de repeti��o se encontrar.
	for (int i = 0; i < *tamEstoque; i++) { 
		if (strcmp(nomeProduto, estoque[i].nome) == 0) {
			indiceProdutoExistente = i;
			produtoExiste = true;
			break;
		}
	}

	// Se no la�o anterior for confirmado que j� existe um produto com o nome inserido na entrada de dados, 
	// atualiza os dados desse produto sem precisar aumentar o vetor.
	if (produtoExiste) { 
		cout << "Pre�o: R$ ";
		cin >> estoque[indiceProdutoExistente].preco; // Atualiza o pre�o.
		cout << "Quantidade: ";
		cin >> qntAdicionada;
		estoque[indiceProdutoExistente].quantidade += qntAdicionada; // Acrescenta a quantidadea do produto que j� havia no estoque.

	} else {
		// Se o nome n�o existir na lista, ent�o � porque o usu�rio est� inserindo um novo produto, precisando aumentar o tamanho do estoque
		estoque = expandirVetorEstoque(estoque, (*tamEstoque)+1);
		
		// Cadastra o novo produto no vetor.
		strcpy(estoque[*tamEstoque].nome, nomeProduto);
		cout << "Pre�o: R$ ";
		cin >> estoque[*tamEstoque].preco;
		cout << "Quantidade: ";
		cin >> estoque[*tamEstoque].quantidade;
		
		++(*tamEstoque);
	}
	
	return estoque;
}

//---------------------------------------------------------------------------------------
// Descri��o_: Fun��o que exclui um produto realocando o �ltimo produto do
//			   vetor e sobrescrevendo o produto escolhido para deletar.
// Entrada___: estoque -> Ponteiro que representa o in�cio do vetor din�mico de produtos;
//			   tamEstoque -> Ponteiro que diz o tamanho do estoque.
// Sa�da_____: Retorna o vetor de produtos com 1 produto a menos.
//---------------------------------------------------------------------------------------
Produto* telaExcluir(Produto* estoque, unsigned* tamEstoque) {
	unsigned numProduto;
	char escolha;
	bool saida = false;

	system("cls");
	cout << " Excluir";
	cout << "\n-------\n";

	if (estoque == nullptr || *tamEstoque == 0) {
		cout << "N�o tem como excluir elementos se n�o existe uma lista, adicione elementos primeiro.\n\n";
	}
	else {
		// Lista os Produtos dispon�veis para serem deletados
		for (int i = 0; i < *tamEstoque; i++) {
			cout << i + 1 << ") " << estoque[i].nome << endl;
		}
		cout << "\n\nEscolha o N�mero do Produto: [ ]\b\b";

		do { // Verifica se a entrada realmente � uma das op��es dispon�veis
			cin >> numProduto;
			if (numProduto > *tamEstoque || numProduto <= 0) {
				cout << "\nOpc�o Inv�lida, tente novamente: ";
				saida = true;
			}
			else {
				cout << "Deseja excluir \"" << estoque[numProduto - 1].nome << "\" (S/N)?";
				do { // Verifica se realmente a letra escolhida � um S ou um N
					cin >> escolha;
					escolha = toupper(escolha);

					if (escolha == 'S' || escolha == 'N') {
						saida = false;
					}
					else {
						cout << "\nOpc�o Inv�lida, tente novamente: ";
						saida = true;
					}
				} while (saida);

				if (escolha == 'S') {
					// L�gica para excluir o produto escolhido sobrescrevendo  
					// pelo �ltimo da lista e diminuindo o tamanho do estoque.
					strcpy(estoque[numProduto-1].nome, estoque[(*tamEstoque)-1].nome);
					estoque[numProduto-1].preco = estoque[(*tamEstoque-1)].preco;
					estoque[numProduto-1].quantidade = estoque[(*tamEstoque-1)].quantidade;

					--(*tamEstoque);
				}
			}
		} while (saida);
	}
	system("pause");
	return estoque;
}

//---------------------------------------------------------------------------------------
// Descri��o_: Fun��o que lista os produtos dispon�veis no estoque formatados. 
// Entrada___: estoque -> Ponteiro que representa o in�cio do vetor din�mico de produtos;
//			   tamEstoque -> indica o tamanho do estoque em tempo real.
// Sa�da_____: void.
//---------------------------------------------------------------------------------------
void telaListar(Produto* estoque, const unsigned tamEstoque) {
	int maiorNome = 0;
	double maiorPreco = 0;
	unsigned maiorQnt = 0;

	system("cls");
	cout << " Listagem";
	cout << "\n----------\n";
	
	if (estoque == nullptr || tamEstoque == 0) {
		cout << "N�o existe uma lista, adicione elementos primeiro.\n\n";
	}
	else {
		// � requisitado o maior nome de produto do estoque.
		for (int i = 0; i < tamEstoque; i++)
			if (maiorNome < strlen(estoque[i].nome)) maiorNome = strlen(estoque[i].nome);

		// � requisitado o maior pre�o de produto do estoque.
		for (int i = 0; i < tamEstoque; i++)
			if (maiorPreco < estoque[i].preco) maiorPreco = estoque[i].preco;

		// � requisitado a maior quantidade de produto do estoque.
		for (int i = 0; i < tamEstoque; i++)
			if (maiorQnt < estoque[i].quantidade) maiorQnt = estoque[i].quantidade;

		cout << fixed;
		cout.precision(2);

		// Lista os produtos
		for (int i = 0; i < tamEstoque; i++) {

			// Formata os nomes para "ocuparem o mesmo espa�o" na coluna dos nomes
			cout << estoque[i].nome;
			escreverEspacos((maiorNome - strlen(estoque[i].nome))); 
			cout << " - ";

			// Formata os n�meros para "ocuparem o mesmo espa�o" na coluna dos pre�os
			cout << "R$" << estoque[i].preco;
			if (maiorPreco >= 10 && maiorPreco < 100 && estoque[i].preco < 10) cout << " ";
			if (maiorPreco >= 100 && estoque[i].preco < 10) cout << "  ";
			if (maiorPreco >= 100 && estoque[i].preco > 10 && estoque[i].preco < 100) cout << " ";

			// Formata os n�meros da coluna de quantidade para a unidade ficar abaixo de unidade, 
			// a dezena ficar abaixo da dezena, e a centena ficar abaixo da centena.
			cout << " - ";
			if (maiorQnt >= 10 && maiorQnt < 100 && estoque[i].quantidade < 10) cout << " ";
			if (maiorQnt >= 100 && estoque[i].quantidade < 10) cout << "  ";
			if (maiorQnt >= 100 && estoque[i].quantidade >= 10 && estoque[i].quantidade < 100) cout << " ";
			cout << estoque[i].quantidade << " und.\n";
		}
		cout << endl;
	}
	system("pause");
}

