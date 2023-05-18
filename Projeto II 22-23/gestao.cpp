#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "structs.h"
#include "inicializacao.h"
#include "gestao.h"

// m�todo que possibilita a repara��o manual de um carro numa ET
void reparacaoManual(estacoes*& estacao, carrosReparados*& raiz) {

	string marcaCarro; // variavel para receber a marca do carro
	string modeloCarro; // variavel para receber o modelo do carro

	getline(cin, marcaCarro);
	cout << "\nIntroduza a marca do carro a remover: "; // pergunta ao usuario a marca
	getline(cin, marcaCarro);
	cout << "\nIntroduza tamb�m o modelo do carro: "; // pergunta ao usuario o modelo
	getline(cin, modeloCarro);

	estacoes* atual = estacao; // recebe a esta��o atual
	while (atual != nullptr) { // enquanto a esta��o for diferente de nul
		carro* carroNaET = atual->primeiroCarro; // recebe o primeiro carro da ET
		carro* anterior = nullptr; // e o anterior a null

		while (carroNaET != nullptr) { // enquanto o carro na esta��o for diferente de null
			if (marcaCarro == carroNaET->marca && modeloCarro == carroNaET->modelo) { // compara a marca e o modelo dada pelo usuario a ver se existe na esta��o

				cout << "\nO carro com ID " << carroNaET->idCarro << " foi removido da esta��o " << atual->idET << ".\n"; // se houver esse carro faz o display desta mensagem
				atual->faturacao += (carroNaET->dias * 50); // adiciona � fatura��o da esta��o

				carro* copia = new carro(*carroNaET);

				inserirNaArvore(raiz, atual->idET, copia); // insere o carro na arvore da esta��o

				if (anterior == nullptr) { // se n�o tiver carro antes do que vai ser removido
					atual->primeiroCarro = carroNaET->proximoCarro; // o primeiro carro da ET passa a ser o seguinte do atual
				}
				else {
					anterior->proximoCarro = carroNaET->proximoCarro; // caso contrario o seguinte do carro anterior � o proximo ao carro que est� a ser removido
				}

				delete carroNaET; // remove o atual
				carroNaET = anterior == nullptr ? atual->primeiroCarro : anterior->proximoCarro; // se o anterior for um null o atual � o primeiro da esta��o caso contr�rio � o proximo do anterior
				atual->quantidadeCarros--; // diminui a quantidade de carros atual na ET
			}
			else { // caso as condi��es n�o sejam cumpridas
				anterior = carroNaET;
				carroNaET = carroNaET->proximoCarro;
			}
		}
		atual = atual->proximaEstacao;
	}
}

// m�todo para alterar o tempo de repara��o de um carro na fila
void tempoReparacao(carro*& carros, int& numEstacoes, estacoes*& estacao) {

	string marcaCarro; // vari�vel que recebe a marca do carro
	string modeloCarro; // vari�vel que recebe o modelo do carro
	int tempoReparacao = 0; // e a que recebe o tempo 

	getline(cin, marcaCarro);
	cout << "Introduza a marca do carro: ";
	getline(cin, marcaCarro);

	cout << "Introduza o modelo do carro: ";
	getline(cin, modeloCarro);

	cout << "\nIntroduza o tempo de repara��o atualizado: ";
	cin >> tempoReparacao; // faz os outputs para o usuario

	carro* atual = carros; // recebe o carro atual
	carro* anterior = nullptr; // e o anterior a null

	while (atual != nullptr) { // enquanto houver carros
		if ((atual->marca == marcaCarro) && (atual->modelo == modeloCarro)) { // v� se a marca e modelo s�o os mesmos

			carro* novoCarro = new carro(); // cria um novo carro com tudo igual ao que vai ser mudado
			novoCarro->idCarro = atual->idCarro; 
			novoCarro->tempoMax = tempoReparacao; // altera apenas o tempo de repara��o para o inserido pelo usuario
			novoCarro->dias = atual->dias;
			novoCarro->prioritario = atual->prioritario;
			novoCarro->marca = atual->marca;
			novoCarro->modelo = atual->modelo;
			novoCarro->proximoCarro = nullptr;

			if (anterior == nullptr) { // se o anterior for null
				carros = atual->proximoCarro; 
			}
			else {
				anterior->proximoCarro = atual->proximoCarro; // o proximo carro ao anterior � igual ao proximo do atual
			}
			delete atual; // apaga o atual

			if (carros == nullptr) { // se n�o houver carros 
				carros = novoCarro; // adiciona o novo carro � lista de carros
			}
			else {
				carro* ultimoCarro = carros; // variavel para receber o ultimo carro
				while (ultimoCarro->proximoCarro != nullptr) { // enquanto o proximo carro � diferente de null
					ultimoCarro = ultimoCarro->proximoCarro; // percorre at� chegar ao ultimo carro
				}
				ultimoCarro->proximoCarro = novoCarro; // e o proximo carro depois do ultimo vai ser o novo criado
			}
			break;
		}
		anterior = atual; // atualiza o anterior
		atual = atual->proximoCarro; // vai para o pr�ximo carro
	}
	organizaListaEspera(carros); // organiza os carros (lista de espera)
	printETs(estacao); // d� print das ETS
	printCars(carros); // d� print dos carros
}

// m�todo para adicionar prioridadea um carro na fila
void adicionarPrioridade(carro*& carros, int& numEstacoes, estacoes*& estacao) {

	int IDcarro = 0; // variavel que recebe o ID do carro

	cout << "\nIntroduza o ID do carro que quer alterar a prioridade: ";
	cin >> IDcarro; // pede ao usu�rio o ID e atualiza a variavel
	 
	carro* atual = carros; // recebe o carro atual
	carro* anterior = nullptr; // variavel de carro anterior a null

	while (atual != nullptr) { // enquanto o atual for diferente de null
		if (atual->idCarro == IDcarro) { // compara o id dado pelo usu�rio com o do carro atual
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
					anterior->proximoCarro = atual->proximoCarro; // se n�o o proximo carro ao anterior � o proximo do atual
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
			else { // se j� for priorit�rio d� output desta mensagem
				cout << "\nEste carro j� � priorit�rio\n";
				break;
			}
		}
		anterior = atual;
		atual = atual->proximoCarro;
	}
	organizaListaEspera(carros); // organiza a lista de carros
	printETs(estacao); // print das esta��es
	printCars(carros); // print dos carros
}

// m�todo que remove todos os carros de uma esta��o
void removerTodosCarros(estacoes*& estacao, const string& nomeMecanico) {

	estacoes* atual = estacao; // recebe a esta��o atual

	while (atual != nullptr) { // enquanto a atual for diferente de null
		if (atual->mecanico == nomeMecanico) { // v� se o mec�nico � o mesmo
			carro* atualCarro = atual->primeiroCarro; // recebe o primeiro carro da esta��o
			while (atualCarro != nullptr) { // enquanto for diferente de null

				cout << "O carro com ID " << atualCarro->idCarro << " foi removido da esta��o " << atual->idET << ".\n"; // display da mensagem

				carro* proximoCarro = atualCarro->proximoCarro; // proximo carro recebe o proximo carro do atual
				delete atualCarro; // d� delete do carro atual
				atualCarro = proximoCarro; // atualiza o atual para o pr�ximo
				atual->quantidadeCarros--; // diminui a quantidade de carros da esta��o
			}
			atual->primeiroCarro = nullptr; // a esta��o fica sem carros 
		}
		atual = atual->proximaEstacao; // passa para a p�xima esta��o 
	}
}

// m�todo que remove o mec�nico
void removerMecanico(estacoes*& estacao, marcas*& marca, carro*& carros, int& numeroPalavrasMarcas, int& numEstacoes) {

	string nomeMecanico; // v�riavel para receber o input do nome do mec�nico

	getline(cin, nomeMecanico);
	cout << "Introduza o nome do mec�nico a remover: ";
	getline(cin, nomeMecanico); // faz output da mensagem e atualiza a vari�vel

	estacoes* atual = estacao; // recebe a esta��o atual
	estacoes* anterior = nullptr; // variavel de esta��o anterior a null
	string novoMecanico; // variavel para receber o nome do novo mec�nico

	while (atual != nullptr) { // enquanto a atual for diferente de null
		if (atual->mecanico == nomeMecanico) { // se o nome introduzido pelo usuario dor igual ao da esta��o
			
			removerTodosCarros(estacao, nomeMecanico); // invoca a fun��o removerTodosCarros

			cout << "\nD� um nome ao mec�nico: ";
			getline(cin, novoMecanico); // pede ao usuario o nome do novo mecanico e atualiza a variavel
			cout << "\n";

			string marcaRandom = escolhePalavraRandomMarcas(marca); // recebe uma marca random

			estacoes* novaEstacao = new estacoes(); // cria uma nova esta��o

			novaEstacao->idET = atual->idET;
			novaEstacao->capacidade = rand() % 4 + 2;
			novaEstacao->quantidadeCarros = 0;
			novaEstacao->faturacao = 0;
			novaEstacao->mecanico = novoMecanico;
			novaEstacao->marcaEspecializada = marcaRandom;
			novaEstacao->proximaEstacao = nullptr;
			novaEstacao->primeiroCarro = nullptr; // atribui �s vari�veis os valores

			if (anterior == nullptr) { // se o anterior for igual a null
				estacao = atual->proximaEstacao; // a esta��o vai ser igual � pr�xima da atual
			}
			else {
				anterior->proximaEstacao = atual->proximaEstacao; // a proxima esta��o da anterior � igual � pr�xima da atual
			}
			delete atual; // apaga a atual

			if (estacao == nullptr) { // se n�o tiver esta��es na lista ligada
				estacao = novaEstacao; // adiciona ao inicio a nova esta��o
			}
			else {
				estacoes* ultimaEstacao = estacao; // vari�vel que armazena a ultima esta��o
				while (ultimaEstacao->proximaEstacao != nullptr) { // faz o loop enquanto houver esta��es
					ultimaEstacao = ultimaEstacao->proximaEstacao;
				}
				ultimaEstacao->proximaEstacao = novaEstacao; // adiciona ao final da lista ligada a nova esta��o
			}
			break;
		}
		anterior = atual; // p�e a anterior como a atual
		atual = atual->proximaEstacao; // passa para a pr�xima esta��o
	}
	organizaETs(estacao); // organiza as esta��es
	printETs(estacao); // print das ETs
	printCars(carros); // print dos carros
}

// m�todo para adicionar uma esta��o nova
void adicionaET(estacoes*& estacao, marcas*& marca) {

	int numEstacoes = 0; // v�riavel que armazena o n�mero de esta��es

	estacoes* temp = estacao; // variavel para receber as esta��es para poder contar quantas existem

	while (temp != nullptr) { // enquanto houver esta��es
		numEstacoes++; // incrementa a variavel
		temp = temp->proximaEstacao; // e passa para a pr�xima
	}

	string nomeMecanico; // vari�vel para receber o nome do mec�nico
	string marcaCarro; // var�vel para receber a marca do carro

	getline(cin, nomeMecanico);
	cout << "\nIntroduza o nome do mec�nico: ";
	getline(cin, nomeMecanico);

	cout << "\nIntroduza a marca: ";
	getline(cin, marcaCarro); // faz os outputs para o usu�rio e atualiza as vari�veis

	estacoes* novaET = new estacoes(); // cria uma nova esta��o

	novaET->idET = numEstacoes + 1;
	novaET->capacidade = rand() % 4 + 2;
	novaET->quantidadeCarros = 0;
	novaET->faturacao = 0;
	novaET->mecanico = nomeMecanico;
	novaET->marcaEspecializada = marcaCarro;
	novaET->proximaEstacao = nullptr;
	novaET->primeiroCarro = nullptr; // adiciona aos atributos os valores

	novaET->proximaEstacao = estacao; 
	estacao = novaET; // adiciona � lista ligada das esta��es

	ofstream marcasAdd; // abre o ficheiro das marcas para escrita
	marcasAdd.open("marcas.txt", ios::app); // permite a escrita no final do ficheiro

	if (marcasAdd.is_open()) { // se tiver aberto 
		marcasAdd << "\n" << marcaCarro; // adiciona essa marca ao ficheiro. caso seja introduzida uma marca nova
		marcasAdd.close(); // fecha o ficheiro
	}

	lerMarcas(marca); // invoca o m�todo que l� o ficheiro das marcas, para adicionar ao ficheiro das marcas a nova introduzida
}

//m�todo que grava as oficinas assim como os carros dentro dela
void gravarEstacao(estacoes*& estacao, int& numEstacoes) {

	estacoes* copia = estacao; // recebe a esta��o atual

	ofstream oficinas("oficinas.txt"); //cria um ficheiro de escrita

	oficinas.clear(); //faz clear para limpar o ficheiro e n�o ficar guardado informa��es antigas

	if (oficinas.is_open()) { //verifica se consegue abrir o ficheiro para guardar a informa��o
		oficinas << "-\n";
		oficinas << numEstacoes << "\n";
		oficinas << "-\n";
		while(copia != nullptr) { //percorre todas as ETs 
			oficinas << copia->idET << "\n";
			oficinas << copia->capacidade << "\n";
			oficinas << copia->quantidadeCarros << "\n";
			oficinas << copia->faturacao << "\n";
			oficinas << copia->mecanico << "\n";
			oficinas << copia->marcaEspecializada << "\n"; // adiciona a cada linha do ficheiro a informa��o

			for (carro* carroAtual = copia->primeiroCarro; carroAtual != nullptr; carroAtual = carroAtual->proximoCarro) { //percorre todos os carros dentro da ET
				oficinas << "--\n";
				oficinas << carroAtual->idCarro << "\n";
				oficinas << carroAtual->tempoMax << "\n";
				oficinas << carroAtual->dias << "\n";
				oficinas << carroAtual->prioritario << "\n";
				oficinas << carroAtual->marca << "\n";
				oficinas << carroAtual->modelo << "\n"; // adiciona a cada linha do ficheiro a informa��o do carro
			} oficinas << "-\n";

			copia = copia->proximaEstacao;
		}
	}
	else {
		cout << "N�o foi possivel abrir o ficheiro oficinas\n";
	}
}

//m�todo que grava a fila de espera
void gravarFilaDeEspera(carro*& carros) {

	int numCarros = 0;

	carro* copiaCarros = carros; // recebe o carro atual

	ofstream fila("filaEspera.txt"); //cria um ficheiro de escrita

	fila.clear(); //faz o clear do ficheiro

	if (fila.is_open()) { //verifica se o ficheiro pode ser aberto para ser guardada as informa��es
		fila << "-\n";
		fila << numCarros << "\n";
		while(copiaCarros != nullptr) { //faz o loop pela quantidade de carros na fila de espera
			fila << "-\n";
			fila << copiaCarros->idCarro << "\n";
			fila << copiaCarros->tempoMax << "\n";
			fila << copiaCarros->dias << "\n";
			fila << copiaCarros->prioritario << "\n";
			fila << copiaCarros->marca << "\n";
			fila << copiaCarros->modelo << "\n"; // adiciona a cada linha do ficheiro as informa��es do carro

			copiaCarros = copiaCarros->proximoCarro;
		}
	}
	else {
		cout << "N�o foi possivel abrir o ficheiro filaEspera\n";
	}
}

// m�todo para gravar as arvores
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
	arvore << raiz->carros->modelo << "\n"; // adiciona a cada linha as informa��es do carro e a et que pertence

	gravarArvores(raiz->esquerda, arvore);
}

// m�todo para dar o nome ao ficheiro e invocar o m�todo para gravar a arvore
void nomeFicheiro(carrosReparados* raiz) {
	ofstream arvore("arvores.txt");

	arvore.clear();

	gravarArvores(raiz, arvore);
}

// m�todo para carregar a fila de espera
void uploadFilaDeEspera(carro*& carros, int& numCarrosTotal) {

	carro* primeiroCarro = nullptr; // variavel do tipo carro a null
	carro* ultimoCarro = nullptr; // variavel do tipo carro a null

	ifstream filaEspera("filaEspera.txt"); // abre o ficheiro da fila de espera para leitura

	if (filaEspera.is_open()) {
		string linha;
		getline(filaEspera, linha); // pr�ximo "-"
		getline(filaEspera, linha);
		getline(filaEspera, linha); // pr�ximo "-"

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

			novoCarro->proximoCarro = nullptr; // adiciona ao novo carro as informa��es do ficheiro aos devidos atributos

			if (primeiroCarro == nullptr) { // se o primeiro carro for nulll
				primeiroCarro = novoCarro; // adiciona o novo carro ao inicio da lista 
				ultimoCarro = novoCarro; // e atualiza a vari�vel de ultimo carro
				numCarrosTotal = primeiroCarro->idCarro; // atualiza a vari�vel numero de carros totais, para ser usada na cria��o de novos carros
			}
			else { // se n�o
				ultimoCarro->proximoCarro = novoCarro; // o proximo carro ao ultimo vai ser o novo criado
				ultimoCarro = novoCarro; // e atualiza a variav�l de ultimo carro
				if (ultimoCarro->idCarro > numCarrosTotal) { // se o id do ultimo carro adicionado for maior que a vari�vel numero de carros atual
					numCarrosTotal = ultimoCarro->idCarro; // � atualizada essa v�riavel para um n�mero mais alto
				}
				
			}
		}
	}
	filaEspera.close(); // fecha o ficheiro

	carros = primeiroCarro; // atualiza a lista ligada de carros, para a nova criada
}

void uploadEstacao(estacoes*& estacao, int& numEstacoes, int& numCarrosTotal) {
	estacoes* primeiraEstacao = nullptr; // variavel de primeira est��o a null
	estacoes* ultimaEstacao = nullptr; // variavel de ultima esta��o a ull

	ifstream oficinas("oficinas.txt"); // abre o ficheiro para leitura

	if (oficinas.is_open()) {
		string linha;
		getline(oficinas, linha); // l� a primeira linha do arquivo (deve ser "-")
		getline(oficinas, linha); // l� a segunda linha do arquivo (n�mero de esta��es)
		numEstacoes = stoi(linha);
		getline(oficinas, linha); // l� a pr�xima linha (deve ser "-")

		while (getline(oficinas, linha)) {
			estacoes* novaEstacao = new estacoes(); // cria uma nova esta��o
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
			novaEstacao->primeiroCarro = nullptr; // d� os valores do ficheiro aosatributos da esta��o

			ofstream marcasAdd;
			marcasAdd.open("marcas.txt", ios::app); // adiciona ao ficheiro da marcas a marca da esta��o caso seja guardada de uma utiliza��o do programa anterior 
												   // uma marca n�o existente no ficheiro original
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
				novoCarro->proximoCarro = nullptr; // d� os valores do ficheiro aos atributos do carro

				if (novaEstacao->primeiroCarro == nullptr) { // se a lista ligada da esta��o estier vazia 
					novaEstacao->primeiroCarro = novoCarro; // adiciona esse novo carro ao inicio da esta��o
				}
				else {
					carro* ultimoCarro = novaEstacao->primeiroCarro; // se n�o percorre at� ao final da lista 
					while (ultimoCarro->proximoCarro != nullptr) {
						ultimoCarro = ultimoCarro->proximoCarro;
					}
					ultimoCarro->proximoCarro = novoCarro; // e adiciona o novo carro ao final da lista 
				}

				index++;
			}

			if (primeiraEstacao == nullptr) { // se a primeira esta��o for null
				primeiraEstacao = novaEstacao; 
				ultimaEstacao = novaEstacao; // adicona a estas listas ligadas a nova esta��o
			}
			else {
				ultimaEstacao->proximaEstacao = novaEstacao; // se n�o adiciona depois da ultima esta��o a nova esta��o
				ultimaEstacao = novaEstacao; // e a ultima esta��o passa a ser esta nova
			}
		}
	}
	oficinas.close(); // fecha o ficheiro

	estacao = primeiraEstacao; // atualiza a lista ligada de esta��es para esta nova

}

// m�todo para inserir os carros na arvore
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
			novoCarro->modelo = linha; // d� os valores ao carro

			inserirNaArvore(raiz, idET, novoCarro); // insere na arvore
		}
	}
	else {
		cout << "\nFalha ao abrir o ficheiro\n";
	}

	arvores.close(); // fecha o ficheiro
}

// m�todo para remover marcas duplicadas do ficheiro de marcas
void removeMarcasDuplicadas(marcas* marca) {

	marcas* atual = marca; // recebe a marca atual

	while (atual != nullptr) { // se a marca atual for diferente de null
		marcas* duplicado = atual; // copia da marca atual
		while (duplicado->proximaMarca != nullptr) // se a pr�xima marca � duplicada for null
		{
			if (duplicado->proximaMarca->marca == atual->marca) { // se a marca for a mesma das duas
				marcas* novaLista = duplicado->proximaMarca; // cria uma nova lista e adiciona a proxima marca 
				duplicado->proximaMarca = novaLista->proximaMarca; // proxima marca da duplicada � a proxima da nova lista
				delete novaLista; // apaga a nova lista
			}
			else {
				duplicado = duplicado->proximaMarca; // se n�o passa para a pr�xima marca
			}
		}
		atual = atual->proximaMarca; // passa para a pr�xima marca
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

// m�todo que faz o print da arvore da esta��o
void imprimeArvore(carrosReparados* raiz, int nivel, int idET) {

	if (raiz == nullptr) {
		cout << "\n";
		return;
	}

	imprimeArvore(raiz->direita, nivel + 1, idET);

	// parte onde verifica se � a ET selecionada pelo user
	if (raiz->idET == idET) {
		for (int i = 0; i < nivel; i++) {
			cout << "\t";
		}

		cout <<"ID: " << raiz->carros->idCarro << " | Modelo: " << raiz->carros->modelo << "\n";
	}

	imprimeArvore(raiz->esquerda, nivel + 1, idET);
}

// m�todo para invocar o m�todo que printa a arvore certa
void imprimirArvorePorEstacao(carrosReparados* raiz) {

	int idDaET = 0;
	cout << "\nIntroduza o ID da ET que deseja imprimir os carros reparados: ";
	cin >> idDaET;
	cout << "\n";

	imprimeArvore(raiz, 0, idDaET);
}


void gestao(estacoes*& estacao, int numEstacoes, marcas*& marca, int& numeroPalavrasMarcas, int numCarros, carro*& carros, int& numCarrosTotal, carrosReparados*& raiz) {


	int options;

	cout << "\n***** Bem vindo gestor *****\n";
	cout << "(1).Repara��o Manual\n";
	cout << "(2).Atualizar tempo de repara��o\n";
	cout << "(3).Adicionar Prioridade\n";
	cout << "(4).Remover Mec�nico\n";
	cout << "(5).Adicionar Esta��o\n";
	cout << "(6).Gravar Oficina\n";
	cout << "(7).Carregar Oficina\n";
	cout << "(8).Imprimir Oficina\n";
	cout << "Selecione a sua op��o:\n";
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

		removerMecanico(estacao, marca, carros, numeroPalavrasMarcas, numEstacoes);

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

		cout << "N�o selecionou uma op��o v�lida\n";

		break;
	}
}