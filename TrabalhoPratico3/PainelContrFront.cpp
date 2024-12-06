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
// Descrição_: Função principal desse arquivo, pois chama todas as telas
// Entrada___: void
// Saída_____: void
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

	// Testa se o arquivo binário existe ou não.
	fin.open("estoque.bin", ios_base::in | ios_base::binary);
	if (!fin.is_open()) {
		// Caso não exista, exibe um aviso e inicia o ponteiro com nullptr.
		system("cls");
		cout << "Ainda não existe arquivo de estoque, por favor adicione produtos para criar um.\n";
		system("pause");
		estoque = nullptr;
	}
	else {
		// Caso exista, é alocado memória dinamicamente para preencher o vetor com os produtos
		// registrados no arquivo binário.
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
		cout << "\n\nOpção: [ ]\b\b";

		cin >> opcao;
		opcao = toupper(opcao);

			// Chama uma função tela dependendo da opção escolhida.
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
				// Caso a opção não exista, exibe mensagem e repete o laço,
				// pedindo uma nova entrada de dados.
				cout << "Opção Inválida, tente novamente.";
				system("pause");
				break;
			}
		} while (saida);

		// Atualiza (caso exista) ou cria (caso não exista) o arquivo binário.
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
// Descrição_: Função que aloca dinamicamente o vetor de produtos, caso o arquivo binário não exista;
//			   essa função também atualiza o preço e a quantidade do Produto, caso ele já exista no vetor;
//			   e caso o Produto ainda não exista no vetor, expande o vetor em 1 unidade e adiciona o Produto.
// Entrada___: estoque -> Ponteiro que representa o início do vetor dinâmico (caso o vetor já exista) ou é o
//						  ponteiro que será utilizado para realocar dinamicamente (caso o vetor não exista);
//			   tamEstoque -> Ponteiro que diz o tamanho do estoque.
// Saída_____: Retorna o vetor que é alocado dinamicamente.
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

	// Verifica se existe algum produto na lista com o nome solicitado e sai do laço de repetição se encontrar.
	for (int i = 0; i < *tamEstoque; i++) { 
		if (strcmp(nomeProduto, estoque[i].nome) == 0) {
			indiceProdutoExistente = i;
			produtoExiste = true;
			break;
		}
	}

	// Se no laço anterior for confirmado que já existe um produto com o nome inserido na entrada de dados, 
	// atualiza os dados desse produto sem precisar aumentar o vetor.
	if (produtoExiste) { 
		cout << "Preço: R$ ";
		cin >> estoque[indiceProdutoExistente].preco; // Atualiza o preço.
		cout << "Quantidade: ";
		cin >> qntAdicionada;
		estoque[indiceProdutoExistente].quantidade += qntAdicionada; // Acrescenta a quantidadea do produto que já havia no estoque.

	} else {
		// Se o nome não existir na lista, então é porque o usuário está inserindo um novo produto, precisando aumentar o tamanho do estoque
		estoque = expandirVetorEstoque(estoque, (*tamEstoque)+1);
		
		// Cadastra o novo produto no vetor.
		strcpy(estoque[*tamEstoque].nome, nomeProduto);
		cout << "Preço: R$ ";
		cin >> estoque[*tamEstoque].preco;
		cout << "Quantidade: ";
		cin >> estoque[*tamEstoque].quantidade;
		
		++(*tamEstoque);
	}
	
	return estoque;
}

//---------------------------------------------------------------------------------------
// Descrição_: Função que exclui um produto realocando o último produto do
//			   vetor e sobrescrevendo o produto escolhido para deletar.
// Entrada___: estoque -> Ponteiro que representa o início do vetor dinâmico de produtos;
//			   tamEstoque -> Ponteiro que diz o tamanho do estoque.
// Saída_____: Retorna o vetor de produtos com 1 produto a menos.
//---------------------------------------------------------------------------------------
Produto* telaExcluir(Produto* estoque, unsigned* tamEstoque) {
	unsigned numProduto;
	char escolha;
	bool saida = false;

	system("cls");
	cout << " Excluir";
	cout << "\n-------\n";

	if (estoque == nullptr || *tamEstoque == 0) {
		cout << "Não tem como excluir elementos se não existe uma lista, adicione elementos primeiro.\n\n";
	}
	else {
		// Lista os Produtos disponíveis para serem deletados
		for (int i = 0; i < *tamEstoque; i++) {
			cout << i + 1 << ") " << estoque[i].nome << endl;
		}
		cout << "\n\nEscolha o Número do Produto: [ ]\b\b";

		do { // Verifica se a entrada realmente é uma das opções disponíveis
			cin >> numProduto;
			if (numProduto > *tamEstoque || numProduto <= 0) {
				cout << "\nOpcão Inválida, tente novamente: ";
				saida = true;
			}
			else {
				cout << "Deseja excluir \"" << estoque[numProduto - 1].nome << "\" (S/N)?";
				do { // Verifica se realmente a letra escolhida é um S ou um N
					cin >> escolha;
					escolha = toupper(escolha);

					if (escolha == 'S' || escolha == 'N') {
						saida = false;
					}
					else {
						cout << "\nOpcão Inválida, tente novamente: ";
						saida = true;
					}
				} while (saida);

				if (escolha == 'S') {
					// Lógica para excluir o produto escolhido sobrescrevendo  
					// pelo último da lista e diminuindo o tamanho do estoque.
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
// Descrição_: Função que lista os produtos disponíveis no estoque formatados. 
// Entrada___: estoque -> Ponteiro que representa o início do vetor dinâmico de produtos;
//			   tamEstoque -> indica o tamanho do estoque em tempo real.
// Saída_____: void.
//---------------------------------------------------------------------------------------
void telaListar(Produto* estoque, const unsigned tamEstoque) {
	int maiorNome = 0;
	double maiorPreco = 0;
	unsigned maiorQnt = 0;

	system("cls");
	cout << " Listagem";
	cout << "\n----------\n";
	
	if (estoque == nullptr || tamEstoque == 0) {
		cout << "Não existe uma lista, adicione elementos primeiro.\n\n";
	}
	else {
		// É requisitado o maior nome de produto do estoque.
		for (int i = 0; i < tamEstoque; i++)
			if (maiorNome < strlen(estoque[i].nome)) maiorNome = strlen(estoque[i].nome);

		// É requisitado o maior preço de produto do estoque.
		for (int i = 0; i < tamEstoque; i++)
			if (maiorPreco < estoque[i].preco) maiorPreco = estoque[i].preco;

		// É requisitado a maior quantidade de produto do estoque.
		for (int i = 0; i < tamEstoque; i++)
			if (maiorQnt < estoque[i].quantidade) maiorQnt = estoque[i].quantidade;

		cout << fixed;
		cout.precision(2);

		// Lista os produtos
		for (int i = 0; i < tamEstoque; i++) {

			// Formata os nomes para "ocuparem o mesmo espaço" na coluna dos nomes
			cout << estoque[i].nome;
			escreverEspacos((maiorNome - strlen(estoque[i].nome))); 
			cout << " - ";

			// Formata os números para "ocuparem o mesmo espaço" na coluna dos preços
			cout << "R$" << estoque[i].preco;
			if (maiorPreco >= 10 && maiorPreco < 100 && estoque[i].preco < 10) cout << " ";
			if (maiorPreco >= 100 && estoque[i].preco < 10) cout << "  ";
			if (maiorPreco >= 100 && estoque[i].preco > 10 && estoque[i].preco < 100) cout << " ";

			// Formata os números da coluna de quantidade para a unidade ficar abaixo de unidade, 
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

