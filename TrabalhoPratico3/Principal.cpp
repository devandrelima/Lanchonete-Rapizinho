#include <iostream>
#include "RapizinhoFront.h"
#include "PainelContrFront.h"
using namespace std;

int main(int argc, char* argv[]) {
	system("chcp 1252 > nul");				// Formata os caracteres para acentuação.

	if (argc > 1) {							// Testa se há algum comando após o nome do executável
		if (strcmp(argv[1], "-c") == 0) {	// Testa se o comando realmente é o -c
			corpoPainelControle();			// Possui todas as telas do Painel de Controle
		}
		else {
			cout << "Sem o comando certo não é possível acessar o Painel de Controle.";
		}
	}
	else {
		corpoRapizinho();					// Possui todas as telas que o cliente pode ter contato
	}
	return 0;
}