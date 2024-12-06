struct Produto {
	char nome[24];
	float preco;
	unsigned quantidade;
};

struct Pedido {
	char opcao;
	Produto produt;
	float total;
};

int convertCharInt(char caractere);
Pedido* expandirVetorPedidos(Pedido* vetor, int indiceNovo);
Produto* expandirVetorEstoque(Produto* estoque, int indiceNovo);
void gerarRecibo(int numPedido, Pedido pedidos[], int tamListaPedidos, float total, int porcentDesconto, float desconto, float taxaEntrega);
void escreverEspacos(int qntEspacos);