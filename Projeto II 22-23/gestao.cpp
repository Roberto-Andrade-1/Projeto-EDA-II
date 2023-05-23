#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include "structs.h"
#include "inicializacao.h"
#include "gestao.h"

// método que possibilita a reparação manual de um carro numa ET
void reparacaoManual(estacoes*& estacao, carrosReparados*& raiz) {

	string marcaCarro; // variavel para receber a marca do carro
	string modeloCarro; // variavel para receber o modelo do carro

	getline(cin, marcaCarro);
	cout << "\nIntroduza a marca do carro a remover: "; // pergunta ao usuario a marca
	getline(cin, marcaCarro);
	cout << "\nIntroduza também o modelo do carro: "; // pergunta ao usuario o modelo
	getline(cin, modeloCarro);

	estacoes* atual = estacao; // recebe a estação atual
	while (atual != nullptr) { // enquanto a estação for diferente de nul
		carro* carroNaET = atual->primeiroCarro; // recebe o primeiro carro da ET
		carro* anterior = nullptr; // e o anterior a null

		while (carroNaET != nullptr) { // enquanto o carro na estação for diferente de null
			if (marcaCarro == carroNaET->marca && modeloCarro == carroNaET->modelo) { // compara a marca e o modelo dada pelo usuario a ver se existe na estação

				cout << "\nO carro com ID " << carroNaET->idCarro << " foi removido da estação " << atual->idET << ".\n"; // se houver esse carro faz o display desta mensagem
				atual->faturacao += (carroNaET->dias * 50); // adiciona à faturação da estação

				carro* copia = new carro(*carroNaET);

				inserirNaArvore(raiz, atual->idET, copia); // insere o carro na arvore da estação

				if (anterior == nullptr) { // se não tiver carro antes do que vai ser removido
					atual->primeiroCarro = carroNaET->proximoCarro; // o primeiro carro da ET passa a ser o seguinte do atual
				}
				else {
					anterior->proximoCarro = carroNaET->proximoCarro; // caso contrario o seguinte do carro anterior é o proximo ao carro que está a ser removido
				}

				delete carroNaET; // remove o atual
				carroNaET = anterior == nullptr ? atual->primeiroCarro : anterior->proximoCarro; // se o anterior for um null o atual é o primeiro da estação caso contrário é o proximo do anterior
				atual->quantidadeCarros--; // diminui a quantidade de carros atual na ET
			}
			else { // caso as condições não sejam cumpridas
				anterior = carroNaET;
				carroNaET = carroNaET->proximoCarro;
			}
		}
		atual = atual->proximaEstacao;
	}
}

// método para alterar o tempo de reparação de um carro na fila
void tempoReparacao(carro*& carros, int& numEstacoes, estacoes*& estacao) {

	string marcaCarro; // variável que recebe a marca do carro
	string modeloCarro; // variável que recebe o modelo do carro
	int tempoReparacao = 0; // e a que recebe o tempo 

	getline(cin, marcaCarro);
	cout << "Introduza a marca do carro: ";
	getline(cin, marcaCarro);

	cout << "Introduza o modelo do carro: ";
	getline(cin, modeloCarro);

	cout << "\nIntroduza o tempo de reparação atualizado: ";
	cin >> tempoReparacao; // faz os outputs para o usuario

	carro* atual = carros; // recebe o carro atual
	carro* anterior = nullptr; // e o anterior a null

	while (atual != nullptr) { // enquanto houver carros
		if ((atual->marca == marcaCarro) && (atual->modelo == modeloCarro)) { // vê se a marca e modelo são os mesmos

			carro* novoCarro = new carro(); // cria um novo carro com tudo igual ao que vai ser mudado
			novoCarro->idCarro = atual->idCarro; 
			novoCarro->tempoMax = tempoReparacao; // altera apenas o tempo de reparação para o inserido pelo usuario
			novoCarro->dias = atual->dias;
			novoCarro->prioritario = atual->prioritario;
			novoCarro->marca = atual->marca;
			novoCarro->modelo = atual->modelo;
			novoCarro->proximoCarro = nullptr;

			if (anterior == nullptr) { // se o anterior for null
				carros = atual->proximoCarro; 
			}
			else {
				anterior->proximoCarro = atual->proximoCarro; // o proximo carro ao anterior é igual ao proximo do atual
			}
			delete atual; // apaga o atual

			if (carros == nullptr) { // se não houver carros 
				carros = novoCarro; // adiciona o novo carro à lista de carros
			}
			else {
				carro* ultimoCarro = carros; // variavel para receber o ultimo carro
				while (ultimoCarro->proximoCarro != nullptr) { // enquanto o proximo carro é diferente de null
					ultimoCarro = ultimoCarro->proximoCarro; // percorre até chegar ao ultimo carro
				}
				ultimoCarro->proximoCarro = novoCarro; // e o proximo carro depois do ultimo vai ser o novo criado
			}
			break;
		}
		anterior = atual; // atualiza o anterior
		atual = atual->proximoCarro; // vai para o próximo carro
	}
	organizaListaEspera(carros); // organiza os carros (lista de espera)
	printETs(estacao); // dá print das ETS
	printCars(carros); // dá print dos carros
}

// método para adicionar prioridadea um carro na fila
void adicionarPrioridade(carro*& carros, int& numEstacoes, estacoes*& estacao) {

	int IDcarro = 0; // variavel que recebe o ID do carro

	cout << "\nIntroduza o ID do carro que quer alterar a prioridade: ";
	cin >> IDcarro; // pede ao usuário o ID e atualiza a variavel
	 
	carro* atual = carros; // recebe o carro atual
	carro* anterior = nullptr; // variavel de carro anterior a null

	while (atual != nullptr) { // enquanto o atual for diferente de null
		if (atual->idCarro == IDcarro) { // compara o id dado pelo usuário com o do carro atual
			if (atual->prioritario == false) { // se a prioridade desse carro for falsa

				carro* novoCarro = new carro();
				novoCarro->idCarro = atual->idCarro;
				novoCarro->tempoMax = atual->tempoMax;
				novoCarro->dias = atual->dias;
				novoCarro->prioritario = true;
				novoCarro->marca = atual->marca;
				novoCarro->modelo = atual->modelo;
				novoCarro->proximoCarro = nullptr; // cria um carro com as mesmas caracteristicas e prioridade diferente

				if (anterior == nullptr) { // se o anterior for null
					carros = atual->proximoCarro;
				}
				else {
					anterior->proximoCarro = atual->proximoCarro; // se não o proximo carro ao anterior é o proximo do atual
				}
				delete atual; // apaga o atual

				if (carros == nullptr) { // se carros tiver vazia 
					carros = novoCarro; // adiciona o carro ao inicio da lista
				}
				else {
					carro* ultimoCarro = carros; // variavel para armazenar o ultimo carro a lista
					while (ultimoCarro->proximoCarro != nullptr) { // enquanto houver proximo carro faz o loop
						ultimoCarro = ultimoCarro->proximoCarro;
					}
					ultimoCarro->proximoCarro = novoCarro; // quando chega ao ultimo carro adiciona esse novo carro ao seguinte
				}
				break;
			}
			else { // se já for prioritário dá output desta mensagem
				cout << "\nEste carro já é prioritário\n";
				break;
			}
		}
		anterior = atual;
		atual = atual->proximoCarro;
	}
	organizaListaEspera(carros); // organiza a lista de carros
	printETs(estacao); // print das estações
	printCars(carros); // print dos carros
}

// método que remove o mecânico
void removerMecanico(estacoes*& estacao, marcas*& marca, carro*& carros, int& numEstacoes, carrosReparados* raiz) {

	string nomeMecanico; // váriavel para receber o input do nome do mecânico

	getline(cin, nomeMecanico);
	cout << "Introduza o nome do mecânico a remover: ";
	getline(cin, nomeMecanico); // faz output da mensagem e atualiza a variável

	estacoes* atual = estacao; // recebe a estação atual
	estacoes* anterior = nullptr; // variavel de estação anterior a null
	string novoMecanico; // variavel para receber o nome do novo mecânico

	while (atual != nullptr) { // enquanto a atual for diferente de null
		if (atual->mecanico == nomeMecanico) { // se o nome introduzido pelo usuario dor igual ao da estação
			
			removerTodosCarros(estacao, nomeMecanico); // invoca a função removerTodosCarros
			removerNodo(raiz, atual->idET);

			cout << "\nDê um nome ao mecânico: ";
			getline(cin, novoMecanico); // pede ao usuario o nome do novo mecanico e atualiza a variavel
			cout << "\n";

			string marcaRandom = escolhePalavraRandomMarcas(marca); // recebe uma marca random

			estacoes* novaEstacao = new estacoes(); // cria uma nova estação

			novaEstacao->idET = atual->idET;
			novaEstacao->capacidade = rand() % 4 + 2;
			novaEstacao->quantidadeCarros = 0;
			novaEstacao->faturacao = 0;
			novaEstacao->mecanico = novoMecanico;
			novaEstacao->marcaEspecializada = marcaRandom;
			novaEstacao->proximaEstacao = nullptr;
			novaEstacao->primeiroCarro = nullptr; // atribui às variáveis os valores

			if (anterior == nullptr) { // se o anterior for igual a null
				estacao = atual->proximaEstacao; // a estação vai ser igual à próxima da atual
			}
			else {
				anterior->proximaEstacao = atual->proximaEstacao; // a proxima estação da anterior é igual à próxima da atual
			}
			delete atual; // apaga a atual

			if (estacao == nullptr) { // se não tiver estações na lista ligada
				estacao = novaEstacao; // adiciona ao inicio a nova estação
			}
			else {
				estacoes* ultimaEstacao = estacao; // variável que armazena a ultima estação
				while (ultimaEstacao->proximaEstacao != nullptr) { // faz o loop enquanto houver estações
					ultimaEstacao = ultimaEstacao->proximaEstacao;
				}
				ultimaEstacao->proximaEstacao = novaEstacao; // adiciona ao final da lista ligada a nova estação
			}
			break;
		}
		anterior = atual; // põe a anterior como a atual
		atual = atual->proximaEstacao; // passa para a próxima estação
	}
	organizaETs(estacao); // organiza as estações
	printETs(estacao); // print das ETs
	printCars(carros); // print dos carros
}

// método que remove todos os carros de uma estação
void removerTodosCarros(estacoes*& estacao, const string& nomeMecanico) {

	estacoes* atual = estacao; // recebe a estação atual

	while (atual != nullptr) { // enquanto a atual for diferente de null
		if (atual->mecanico == nomeMecanico) { // vê se o mecânico é o mesmo
			carro* atualCarro = atual->primeiroCarro; // recebe o primeiro carro da estação
			while (atualCarro != nullptr) { // enquanto for diferente de null

				cout << "O carro com ID " << atualCarro->idCarro << " foi removido da estação " << atual->idET << ".\n"; // display da mensagem

				carro* proximoCarro = atualCarro->proximoCarro; // proximo carro recebe o proximo carro do atual
				delete atualCarro; // dá delete do carro atual
				atualCarro = proximoCarro; // atualiza o atual para o próximo
				atual->quantidadeCarros--; // diminui a quantidade de carros da estação
			}
			atual->primeiroCarro = nullptr; // a estação fica sem carros 
		}
		atual = atual->proximaEstacao; // passa para a póxima estação 
	}
}

// método para remover todos os carros da arvore de carros arranjados
carrosReparados* removerNodo(carrosReparados* raiz, carrosReparados* noRem, carrosReparados* ant) {
	if (ant == NULL) {
		if (noRem->esquerda == NULL && noRem->direita == NULL) {
			raiz = NULL;
		}
		else if (raiz->direita == NULL) {
			raiz = raiz->esquerda;
		}
		else if (raiz->esquerda == NULL) {
			raiz = raiz->direita;
		}
		else {
			carrosReparados* aux = noRem->esquerda;
			while (aux->direita != NULL) {
				aux = aux->direita;
			}
			aux->direita = noRem->direita;

			raiz = noRem->esquerda;
		}
	}
	else {
		if (noRem->direita == NULL) {
			if (ant->idET > noRem->idET) {
				ant->esquerda = noRem->esquerda;
			}
			else {
				ant->direita = noRem->esquerda;
			}
		}
		else if (noRem->esquerda == NULL) {
			if (ant->idET > noRem->idET) {
				ant->esquerda = noRem->direita;
			}
			else {
				ant->direita = noRem->direita;
			}
		}
		else {
			carrosReparados* temp = noRem->esquerda;
			while (temp->direita != NULL) {
				temp = temp->direita;
			}
			temp->direita = noRem->direita;
			if (ant->idET > noRem->idET) {
				ant->esquerda = noRem->esquerda;
			}
			else {
				ant->direita = noRem->esquerda;
			}
		}
	}
	delete noRem;
	return raiz;
}

// método para remover todos os carros da arvore de carros arranjados
carrosReparados* removerNodo(carrosReparados* raiz, int num) {
	carrosReparados* noRem = raiz;
	carrosReparados* ant = NULL;
	while (noRem != NULL) {
		if (noRem->idET == num) {
			//while(existeCarrosComId(raiz, num))
			raiz = removerNodo(raiz, noRem, ant);
			noRem = raiz;
			ant = NULL;
		}
		else if (num < noRem->idET) {
			ant = noRem;
			noRem = noRem->esquerda;
		}
		else {
			ant = noRem;
			noRem = noRem->direita;
		}
	}
	return raiz;
}

// método para adicionar uma estação nova
void adicionaET(estacoes*& estacao, marcas*& marca) {

	int numEstacoes = 0; // váriavel que armazena o número de estações

	estacoes* temp = estacao; // variavel para receber as estações para poder contar quantas existem

	while (temp != nullptr) { // enquanto houver estações
		numEstacoes++; // incrementa a variavel
		temp = temp->proximaEstacao; // e passa para a próxima
	}

	string nomeMecanico; // variável para receber o nome do mecânico
	string marcaCarro; // varável para receber a marca do carro

	getline(cin, nomeMecanico);
	cout << "\nIntroduza o nome do mecânico: ";
	getline(cin, nomeMecanico);

	cout << "\nIntroduza a marca: ";
	getline(cin, marcaCarro); // faz os outputs para o usuário e atualiza as variáveis

	estacoes* novaET = new estacoes(); // cria uma nova estação

	novaET->idET = numEstacoes + 1;
	novaET->capacidade = rand() % 4 + 2;
	novaET->quantidadeCarros = 0;
	novaET->faturacao = 0;
	novaET->mecanico = nomeMecanico;
	novaET->marcaEspecializada = marcaCarro;
	novaET->proximaEstacao = nullptr;
	novaET->primeiroCarro = nullptr; // adiciona aos atributos os valores

	novaET->proximaEstacao = estacao; 
	estacao = novaET; // adiciona à lista ligada das estações

	ofstream marcasAdd; // abre o ficheiro das marcas para escrita
	marcasAdd.open("marcas.txt", ios::app); // permite a escrita no final do ficheiro

	if (marcasAdd.is_open()) { // se tiver aberto 
		marcasAdd << "\n" << marcaCarro; // adiciona essa marca ao ficheiro. caso seja introduzida uma marca nova
		marcasAdd.close(); // fecha o ficheiro
	}

	lerMarcas(marca); // invoca o método que lê o ficheiro das marcas, para adicionar ao ficheiro das marcas a nova introduzida
}

//método que grava as oficinas assim como os carros dentro dela
void gravarEstacao(estacoes*& estacao, int& numEstacoes) {

	estacoes* copia = estacao; // recebe a estação atual

	ofstream oficinas("oficinas.txt"); //cria um ficheiro de escrita

	oficinas.clear(); //faz clear para limpar o ficheiro e não ficar guardado informações antigas

	if (oficinas.is_open()) { //verifica se consegue abrir o ficheiro para guardar a informação
		oficinas << "-\n";
		oficinas << numEstacoes << "\n";
		oficinas << "-\n";
		while(copia != nullptr) { //percorre todas as ETs 
			oficinas << copia->idET << "\n";
			oficinas << copia->capacidade << "\n";
			oficinas << copia->quantidadeCarros << "\n";
			oficinas << copia->faturacao << "\n";
			oficinas << copia->mecanico << "\n";
			oficinas << copia->marcaEspecializada << "\n"; // adiciona a cada linha do ficheiro a informação

			for (carro* carroAtual = copia->primeiroCarro; carroAtual != nullptr; carroAtual = carroAtual->proximoCarro) { //percorre todos os carros dentro da ET
				oficinas << "--\n";
				oficinas << carroAtual->idCarro << "\n";
				oficinas << carroAtual->tempoMax << "\n";
				oficinas << carroAtual->dias << "\n";
				oficinas << carroAtual->prioritario << "\n";
				oficinas << carroAtual->marca << "\n";
				oficinas << carroAtual->modelo << "\n"; // adiciona a cada linha do ficheiro a informação do carro
			} oficinas << "-\n";

			copia = copia->proximaEstacao;
		}
	}
	else {
		cout << "Não foi possivel abrir o ficheiro oficinas\n";
	}
}

//método que grava a fila de espera
void gravarFilaDeEspera(carro*& carros) {

	int numCarros = 0;

	carro* copiaCarros = carros; // recebe o carro atual

	ofstream fila("filaEspera.txt"); //cria um ficheiro de escrita

	fila.clear(); //faz o clear do ficheiro

	if (fila.is_open()) { //verifica se o ficheiro pode ser aberto para ser guardada as informações
		fila << "-\n";
		fila << numCarros << "\n";
		while(copiaCarros != nullptr) { //faz o loop pela quantidade de carros na fila de espera
			fila << "-\n";
			fila << copiaCarros->idCarro << "\n";
			fila << copiaCarros->tempoMax << "\n";
			fila << copiaCarros->dias << "\n";
			fila << copiaCarros->prioritario << "\n";
			fila << copiaCarros->marca << "\n";
			fila << copiaCarros->modelo << "\n"; // adiciona a cada linha do ficheiro as informações do carro

			copiaCarros = copiaCarros->proximoCarro;
		}
	}
	else {
		cout << "Não foi possivel abrir o ficheiro filaEspera\n";
	}
}

// método para dar o nome ao ficheiro e invocar o método para gravar a arvore
void nomeFicheiro(carrosReparados* raiz) {
	ofstream arvore("arvores.txt");

	arvore.clear();

	gravarArvores(raiz, arvore);
}

// método para gravar as arvores
void gravarArvores(carrosReparados* raiz, ofstream& arvore) {

	if (raiz == nullptr) {
		return;
	}

	gravarArvores(raiz->direita, arvore);

	arvore << "-\n";
	arvore << raiz->idET << "\n";
	arvore << raiz->carros->idCarro << "\n";
	arvore << raiz->carros->tempoMax << "\n";
	arvore << raiz->carros->dias << "\n";
	arvore << raiz->carros->prioritario << "\n";
	arvore << raiz->carros->marca << "\n";
	arvore << raiz->carros->modelo << "\n"; // adiciona a cada linha as informações do carro e a et que pertence

	gravarArvores(raiz->esquerda, arvore);
}

// método para carregar as estações
void uploadEstacao(estacoes*& estacao, int& numEstacoes, int& numCarrosTotal) {
	estacoes* primeiraEstacao = nullptr; // variavel de primeira estção a null
	estacoes* ultimaEstacao = nullptr; // variavel de ultima estação a ull

	ifstream oficinas("oficinas.txt"); // abre o ficheiro para leitura

	if (oficinas.is_open()) {
		string linha;
		getline(oficinas, linha); // lê a primeira linha do arquivo (deve ser "-")
		getline(oficinas, linha); // lê a segunda linha do arquivo (número de estações)
		numEstacoes = stoi(linha);
		getline(oficinas, linha); // lê a próxima linha (deve ser "-")

		while (getline(oficinas, linha)) {
			estacoes* novaEstacao = new estacoes(); // cria uma nova estação
			novaEstacao->idET = stoi(linha);
			getline(oficinas, linha);
			novaEstacao->capacidade = stoi(linha);
			getline(oficinas, linha);
			novaEstacao->quantidadeCarros = stoi(linha);
			getline(oficinas, linha);
			novaEstacao->faturacao = stoi(linha);
			getline(oficinas, linha);
			novaEstacao->mecanico = linha;
			getline(oficinas, linha);
			novaEstacao->marcaEspecializada = linha;
			novaEstacao->proximaEstacao = nullptr;
			novaEstacao->primeiroCarro = nullptr; // dá os valores do ficheiro aosatributos da estação

			ofstream marcasAdd;
			marcasAdd.open("marcas.txt", ios::app); // adiciona ao ficheiro da marcas a marca da estação caso seja guardada de uma utilização do programa anterior 
												   // uma marca não existente no ficheiro original
			if (marcasAdd.is_open()) {
				marcasAdd << "\n" << linha;
				marcasAdd.close();
			}

			int index = 0;

			while (getline(oficinas, linha) && linha != "-" && index < novaEstacao->quantidadeCarros) {
				carro* novoCarro = new carro(); // cria um carro novo
				getline(oficinas, linha);
				novoCarro->idCarro = stoi(linha);
				getline(oficinas, linha);
				novoCarro->tempoMax = stoi(linha);
				getline(oficinas, linha);
				novoCarro->dias = stoi(linha);
				getline(oficinas, linha);
				novoCarro->prioritario = stoi(linha);
				getline(oficinas, linha);
				novoCarro->marca = linha;
				getline(oficinas, linha);
				novoCarro->modelo = linha;
				novoCarro->proximoCarro = nullptr; // dá os valores do ficheiro aos atributos do carro

				if (novaEstacao->primeiroCarro == nullptr) { // se a lista ligada da estação estier vazia 
					novaEstacao->primeiroCarro = novoCarro; // adiciona esse novo carro ao inicio da estação
				}
				else {
					carro* ultimoCarro = novaEstacao->primeiroCarro; // se não percorre até ao final da lista 
					while (ultimoCarro->proximoCarro != nullptr) {
						ultimoCarro = ultimoCarro->proximoCarro;
					}
					ultimoCarro->proximoCarro = novoCarro; // e adiciona o novo carro ao final da lista 
				}

				index++;
			}

			if (primeiraEstacao == nullptr) { // se a primeira estação for null
				primeiraEstacao = novaEstacao; 
				ultimaEstacao = novaEstacao; // adicona a estas listas ligadas a nova estação
			}
			else {
				ultimaEstacao->proximaEstacao = novaEstacao; // se não adiciona depois da ultima estação a nova estação
				ultimaEstacao = novaEstacao; // e a ultima estação passa a ser esta nova
			}
		}
	}
	oficinas.close(); // fecha o ficheiro

	estacao = primeiraEstacao; // atualiza a lista ligada de estações para esta nova

}

// método para carregar a fila de espera
void uploadFilaDeEspera(carro*& carros, int& numCarrosTotal) {

	carro* primeiroCarro = nullptr; // variavel do tipo carro a null
	carro* ultimoCarro = nullptr; // variavel do tipo carro a null

	ifstream filaEspera("filaEspera.txt"); // abre o ficheiro da fila de espera para leitura

	if (filaEspera.is_open()) {
		string linha;
		getline(filaEspera, linha); // próximo "-"
		getline(filaEspera, linha);
		getline(filaEspera, linha); // próximo "-"

		while (getline(filaEspera, linha)) {
			carro* novoCarro = new carro(); // cria um novo carro
			novoCarro->idCarro = stoi(linha);
			getline(filaEspera, linha);
			novoCarro->tempoMax = stoi(linha);
			getline(filaEspera, linha);
			novoCarro->dias = stoi(linha);
			getline(filaEspera, linha);
			novoCarro->prioritario = stoi(linha);
			getline(filaEspera, linha);
			novoCarro->marca = linha;
			getline(filaEspera, linha);
			novoCarro->modelo = linha;
			getline(filaEspera, linha);

			novoCarro->proximoCarro = nullptr; // adiciona ao novo carro as informações do ficheiro aos devidos atributos

			if (primeiroCarro == nullptr) { // se o primeiro carro for nulll
				primeiroCarro = novoCarro; // adiciona o novo carro ao inicio da lista 
				ultimoCarro = novoCarro; // e atualiza a variável de ultimo carro
				numCarrosTotal = primeiroCarro->idCarro; // atualiza a variável numero de carros totais, para ser usada na criação de novos carros
			}
			else { // se não
				ultimoCarro->proximoCarro = novoCarro; // o proximo carro ao ultimo vai ser o novo criado
				ultimoCarro = novoCarro; // e atualiza a variavél de ultimo carro
				if (ultimoCarro->idCarro > numCarrosTotal) { // se o id do ultimo carro adicionado for maior que a variável numero de carros atual
					numCarrosTotal = ultimoCarro->idCarro; // é atualizada essa váriavel para um número mais alto
				}
				
			}
		}
	}
	filaEspera.close(); // fecha o ficheiro

	carros = primeiroCarro; // atualiza a lista ligada de carros, para a nova criada
}

// método para inserir os carros arranjados na arvore a partir de um ficheiro
void uploadArvores(carrosReparados*& raiz, const string ficheiro) {

	ifstream arvores(ficheiro);

	if (arvores.is_open()) {

		string linha;
		int idET = 0;

		while (getline(arvores, linha)) {
			getline(arvores, linha);
			idET = stoi(linha);
			carro* novoCarro = new carro(); // cria um carro novo
			getline(arvores, linha);
			novoCarro->idCarro = stoi(linha);
			getline(arvores, linha);
			novoCarro->tempoMax = stoi(linha);
			getline(arvores, linha);
			novoCarro->dias = stoi(linha);
			getline(arvores, linha);
			novoCarro->prioritario = stoi(linha);
			getline(arvores, linha);
			novoCarro->marca = linha;
			getline(arvores, linha);
			novoCarro->modelo = linha; // dá os valores ao carro

			inserirNaArvore(raiz, idET, novoCarro); // insere na arvore
		}
	}
	else {
		cout << "\nFalha ao abrir o ficheiro\n";
	}

	arvores.close(); // fecha o ficheiro
}

// método para remover marcas duplicadas do ficheiro de marcas
void removeMarcasDuplicadas(marcas* marca) {

	marcas* atual = marca; // recebe a marca atual

	while (atual != nullptr) { // se a marca atual for diferente de null
		marcas* duplicado = atual; // copia da marca atual
		while (duplicado->proximaMarca != nullptr) // se a próxima marca à duplicada for null
		{
			if (duplicado->proximaMarca->marca == atual->marca) { // se a marca for a mesma das duas
				marcas* novaLista = duplicado->proximaMarca; // cria uma nova lista e adiciona a proxima marca 
				duplicado->proximaMarca = novaLista->proximaMarca; // proxima marca da duplicada é a proxima da nova lista
				delete novaLista; // apaga a nova lista
			}
			else {
				duplicado = duplicado->proximaMarca; // se não passa para a próxima marca
			}
		}
		atual = atual->proximaMarca; // passa para a próxima marca
	}

	ofstream marcas("marcas.txt"); // abre o ficheiro para escrita
	if (marcas.is_open()) {
		atual = marca; // recebe a marca atual
		while (atual != nullptr) { // enquanto a marca for diferente de null
			marcas << atual->marca << "\n"; // adiciona a marca ao ficheiro
			atual = atual->proximaMarca;
		}
		marcas.close(); // fecha o ficheiro
	}
	else {
		cout << "\nErro ao tentar escrever no ficheiro\n";
	}
}

// método para invocar o método que printa a arvore certa
void imprimirArvorePorEstacao(carrosReparados* raiz) {

	int idDaET = 0;
	cout << "\nIntroduza o ID da ET que deseja imprimir os carros reparados: ";
	cin >> idDaET;
	cout << "\n";

	imprimeArvore(raiz, 0, idDaET);
}

// método que faz o print da arvore da estação
void imprimeArvore(carrosReparados* raiz, int nivel, int idET) {

	if (raiz == nullptr) {
		cout << "\n";
		return;
	}

	imprimeArvore(raiz->direita, nivel + 1, idET);

	// parte onde verifica se é a ET selecionada pelo user
	if (raiz->idET == idET) {
		for (int i = 0; i < nivel; i++) {
			cout << "\t";
		}

		cout << "idET: " << raiz->idET << " | ID: " << raiz->carros->idCarro << " | Modelo: " << raiz->carros->modelo << "\n";
	}

	imprimeArvore(raiz->esquerda, nivel + 1, idET);
}

void gestao(estacoes*& estacao, int numEstacoes, marcas*& marca, int numCarros, carro*& carros, int& numCarrosTotal, carrosReparados*& raiz) {


	int options;

	cout << "\n***** Bem vindo gestor *****\n";
	cout << "(1).Reparação Manual\n";
	cout << "(2).Atualizar tempo de reparação\n";
	cout << "(3).Adicionar Prioridade\n";
	cout << "(4).Remover Mecânico\n";
	cout << "(5).Adicionar Estação\n";
	cout << "(6).Gravar Oficina\n";
	cout << "(7).Carregar Oficina\n";
	cout << "(8).Imprimir Oficina\n";
	cout << "Selecione a sua opção:\n";
	cin >> options;

	switch (options)
	{
	case 1:

		reparacaoManual(estacao, raiz);
		organizaETs(estacao);
		printETs(estacao);
		organizaListaEspera(carros);
		printCars(carros);

		break;

	case 2:

		tempoReparacao(carros, numEstacoes, estacao);

		break;

	case 3:

		adicionarPrioridade(carros, numEstacoes, estacao);

		break;

	case 4:

		removerMecanico(estacao, marca, carros, numEstacoes, raiz);

		break;

	case 5:

		adicionaET(estacao, marca);
		organizaETs(estacao);
		printETs(estacao);
		organizaListaEspera(carros);
		printCars(carros);

		break;

	case 6:

		gravarEstacao(estacao, numEstacoes);
		gravarFilaDeEspera(carros);
		nomeFicheiro(raiz);
		cout << "\nOficina gravada com sucesso\n";

		break;

	case 7:

		uploadEstacao(estacao, numEstacoes, numCarrosTotal);
		uploadFilaDeEspera(carros, numCarrosTotal);
		uploadArvores(raiz, "arvores.txt");
		printETs(estacao);
		printCars(carros);
		removeMarcasDuplicadas(marca);

		break;

	case 8:

		imprimirArvorePorEstacao(raiz);

		break;

	default:

		cout << "Não selecionou uma opção válida\n";

		break;
	}
}