#include <iostream>
#include "RapizinhoFront.h"
#include "PainelContrFront.h"
using namespace std;

int main(int argc, char* argv[]) {
	system("chcp 1252 > nul");				// Formata os caracteres para acentua��o.

	if (argc > 1) {							// Testa se h� algum comando ap�s o nome do execut�vel
		if (strcmp(argv[1], "-c") == 0) {	// Testa se o comando realmente � o -c
			corpoPainelControle();			// Possui todas as telas do Painel de Controle
		}
		else {
			cout << "Sem o comando certo n�o � poss�vel acessar o Painel de Controle.";
		}
	}
	else {
		corpoRapizinho();					// Possui todas as telas que o cliente pode ter contato
	}
	return 0;
}