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

void tempoReparacao(carro*& carros, int& numEstacoes, estacoes*& estacao) {

	string marcaCarro;
	string modeloCarro;
	int tempoReparacao = 0;

	getline(cin, marcaCarro);
	cout << "Introduza a marca do carro: ";
	getline(cin, marcaCarro);

	getline(cin, modeloCarro);
	cout << "Introduza o modelo do carro: ";
	getline(cin, modeloCarro);

	cout << "\nIntroduza o tempo de reparação atualizado: ";
	cin >> tempoReparacao;

	carro* atual = carros;
	carro* anterior = nullptr;

	while (atual != nullptr) {
		if ((atual->marca == marcaCarro) && (atual->modelo == modeloCarro)) {

			carro* novoCarro = new carro();
			novoCarro->idCarro = atual->idCarro;
			novoCarro->tempoMax = tempoReparacao;
			novoCarro->dias = atual->dias;
			novoCarro->prioritario = atual->prioritario;
			novoCarro->marca = atual->marca;
			novoCarro->modelo = atual->modelo;
			novoCarro->proximoCarro = nullptr;

			if (anterior == nullptr) {
				carros = atual->proximoCarro;
			}
			else {
				anterior->proximoCarro = atual->proximoCarro;
			}
			delete atual;

			if (carros == nullptr) {
				carros = novoCarro;
			}
			else {
				carro* ultimoCarro = carros;
				while (ultimoCarro->proximoCarro != nullptr) {
					ultimoCarro = ultimoCarro->proximoCarro;
				}
				ultimoCarro->proximoCarro = novoCarro;
			}
			break;
		}
		anterior = atual;
		atual = atual->proximoCarro;
	}
	organizaListaEspera(carros);
	printETs(estacao);
	printCars(carros);
}

void adicionarPrioridade(carro*& carros, int& numEstacoes, estacoes*& estacao) {

	int IDcarro = 0;

	cout << "\nIntroduza o ID do carro que quer alterar a prioridade: ";
	cin >> IDcarro;

	carro* atual = carros;
	carro* anterior = nullptr;

	while (atual != nullptr) {
		if (atual->idCarro == IDcarro) {
			if (atual->prioritario == false) {

				carro* novoCarro = new carro();
				novoCarro->idCarro = atual->idCarro;
				novoCarro->tempoMax = atual->tempoMax;
				novoCarro->dias = atual->dias;
				novoCarro->prioritario = true;
				novoCarro->marca = atual->marca;
				novoCarro->modelo = atual->modelo;
				novoCarro->proximoCarro = nullptr;

				if (anterior == nullptr) {
					carros = atual->proximoCarro;
				}
				else {
					anterior->proximoCarro = atual->proximoCarro;
				}
				delete atual;

				if (carros == nullptr) {
					carros = novoCarro;
				}
				else {
					carro* ultimoCarro = carros;
					while (ultimoCarro->proximoCarro != nullptr) {
						ultimoCarro = ultimoCarro->proximoCarro;
					}
					ultimoCarro->proximoCarro = novoCarro;
				}
				break;
			}
			else {
				cout << "\nEste carro já é prioritário\n";
				break;
			}
		}
		anterior = atual;
		atual = atual->proximoCarro;
	}
	organizaListaEspera(carros);
	printETs(estacao);
	printCars(carros);
}


void removerTodosCarros(estacoes*& estacao, const string& nomeMecanico) {
	estacoes* atual = estacao;
	while (atual != nullptr) {
		if (atual->mecanico == nomeMecanico) {
			carro* atualCarro = atual->primeiroCarro;
			while (atualCarro != nullptr) {

				cout << "O carro com ID " << atualCarro->idCarro << " foi removido da estação " << atual->idET << ".\n";

				carro* proximoCarro = atualCarro->proximoCarro;
				delete atualCarro;
				atualCarro = proximoCarro;
				atual->quantidadeCarros--;
			}
			atual->primeiroCarro = nullptr;
		}
		atual = atual->proximaEstacao;
	}
}

void removerMecanico(estacoes*& estacao, marcas*& marca, carro*& carros, int& numeroPalavrasMarcas, int& numEstacoes) {

	string nomeMecanico;

	getline(cin, nomeMecanico);
	cout << "Introduza o nome do mecânico a remover: ";
	getline(cin, nomeMecanico);

	estacoes* atual = estacao;
	estacoes* anterior = nullptr;
	string novoMecanico;

	while (atual != nullptr) {
		if (atual->mecanico == nomeMecanico) {
			
			removerTodosCarros(estacao, nomeMecanico);

			cout << "\nDê um nome ao mecânico: ";
			getline(cin, novoMecanico);
			cout << "\n";

			string marcaRandom = escolhePalavraRandomMarcas(marca);

			estacoes* novaEstacao = new estacoes();

			novaEstacao->idET = atual->idET;
			novaEstacao->capacidade = rand() % 4 + 2;
			novaEstacao->quantidadeCarros = 0;
			novaEstacao->faturacao = 0;
			novaEstacao->mecanico = novoMecanico;
			novaEstacao->marcaEspecializada = marcaRandom;
			novaEstacao->proximaEstacao = nullptr;
			novaEstacao->primeiroCarro = nullptr;

			if (anterior == nullptr) {
				estacao = atual->proximaEstacao;
			}
			else {
				anterior->proximaEstacao = atual->proximaEstacao;
			}
			delete atual;

			if (estacao == nullptr) {
				estacao = novaEstacao;
			}
			else {
				estacoes* ultimaEstacao = estacao;
				while (ultimaEstacao->proximaEstacao != nullptr) {
					ultimaEstacao = ultimaEstacao->proximaEstacao;
				}
				ultimaEstacao->proximaEstacao = novaEstacao;
			}
			break;
		}
		anterior = atual;
		atual = atual->proximaEstacao;
	}
	organizaETs(estacao);
	printETs(estacao);
	printCars(carros);
}

void adicionaET(estacoes*& estacao, marcas*& marca) {

	int numEstacoes = 0;

	estacoes* temp = estacao;

	while (temp != nullptr) {
		numEstacoes++;
		temp = temp->proximaEstacao;
	}

	string nomeMecanico;
	string marcaCarro;

	getline(cin, nomeMecanico);
	cout << "\nIntroduza o nome do mecânico: ";
	getline(cin, nomeMecanico);

	cout << "\nIntroduza a marca: ";
	getline(cin, marcaCarro);

	estacoes* novaET = new estacoes();

	novaET->idET = numEstacoes + 1;
	novaET->capacidade = rand() % 4 + 2;
	novaET->quantidadeCarros = 0;
	novaET->faturacao = 0;
	novaET->mecanico = nomeMecanico;
	novaET->marcaEspecializada = marcaCarro;
	novaET->proximaEstacao = nullptr;
	novaET->primeiroCarro = nullptr;

	novaET->proximaEstacao = estacao;
	estacao = novaET;

	ofstream marcasAdd;
	marcasAdd.open("marcas.txt", ios::app);

	if (marcasAdd.is_open()) {
		marcasAdd << "\n" << marcaCarro;
		marcasAdd.close();
	}

	lerMarcas("marcas.txt", marca);
}

//método que grava as oficinas assim como os carros dentro dela
void gravarEstacao(estacoes*& estacao, int& numEstacoes) {

	ofstream oficinas("oficinas.txt"); //cria um ficheiro com o nome

	oficinas.clear(); //faz clear para limpar o ficheiro e não ficar guardado informações antigas

	if (oficinas.is_open()) { //verifica se consegue abrir o ficheiro para guardar a informação
		oficinas << "-\n";
		oficinas << numEstacoes << "\n";
		oficinas << "-\n";
		while(estacao != nullptr) { //percorre todas as ETs 
			oficinas << estacao->idET << "\n";
			oficinas << estacao->capacidade << "\n";
			oficinas << estacao->quantidadeCarros << "\n";
			oficinas << estacao->faturacao << "\n";
			oficinas << estacao->mecanico << "\n";
			oficinas << estacao->marcaEspecializada << "\n";

			for (carro* carroAtual = estacao->primeiroCarro; carroAtual != nullptr; carroAtual = carroAtual->proximoCarro) { //percorre todos os carros dentro da ET
				oficinas << "--\n";
				oficinas << carroAtual->idCarro << "\n";
				oficinas << carroAtual->tempoMax << "\n";
				oficinas << carroAtual->dias << "\n";
				oficinas << carroAtual->prioritario << "\n";
				oficinas << carroAtual->marca << "\n";
				oficinas << carroAtual->modelo << "\n";
			} oficinas << "-\n";

			estacao = estacao->proximaEstacao;
		}
	}
	else {
		cout << "Não foi possivel abrir o ficheiro oficinas\n";
	}
}

//método que grava a fila de espera
void gravarFilaDeEspera(carro*& carros) {

	int numCarros = 0;

	carro* contaCarros = carros;

	while (contaCarros != nullptr) {
		numCarros++;
		contaCarros = contaCarros->proximoCarro;
	}

	ofstream fila("filaEspera.txt"); //cria um ficheiro com o nome

	fila.clear(); //faz o clear do ficheiro


	if (fila.is_open()) { //verifica se o ficheiro pode ser aberto para ser guardada as informações
		fila << "-\n";
		fila << numCarros << "\n";
		while(carros != nullptr) { //faz o loop pela quantidade de carros na fila de espera
			fila << "-\n";
			fila << carros->idCarro << "\n";
			fila << carros->tempoMax << "\n";
			fila << carros->dias << "\n";
			fila << carros->prioritario << "\n";
			fila << carros->marca << "\n";
			fila << carros->modelo << "\n";

			carros = carros->proximoCarro;
		}
	}
	else {
		cout << "Não foi possivel abrir o ficheiro filaEspera\n";
	}
}

void uploadFilaDeEspera(carro*& carros, int& numCarrosTotal) {
	carro* primeiroCarro = nullptr;
	carro* ultimoCarro = nullptr;

	ifstream filaEspera("filaEspera.txt");

	if (filaEspera.is_open()) {
		string linha;
		getline(filaEspera, linha); // próximo "-"
		getline(filaEspera, linha);
		getline(filaEspera, linha); // próximo "-"

		while (getline(filaEspera, linha)) {
			carro* novoCarro = new carro();
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

			novoCarro->proximoCarro = nullptr;

			if (primeiroCarro == nullptr) {
				primeiroCarro = novoCarro;
				ultimoCarro = novoCarro;
				numCarrosTotal = primeiroCarro->idCarro;
			}
			else {
				ultimoCarro->proximoCarro = novoCarro;
				ultimoCarro = novoCarro;
				if (ultimoCarro->idCarro > numCarrosTotal) {
					numCarrosTotal = ultimoCarro->idCarro;
				}
				
			}
		}
	}
	filaEspera.close();

	carros = primeiroCarro;
}

void uploadEstacao(estacoes*& estacao, int& numEstacoes, int& numCarrosTotal) {
	estacoes* primeiraEstacao = nullptr;
	estacoes* ultimaEstacao = nullptr;

	ifstream oficinas("oficinas.txt");

	if (oficinas.is_open()) {
		string linha;
		getline(oficinas, linha); // lê a primeira linha do arquivo (deve ser "-")
		getline(oficinas, linha); // lê a segunda linha do arquivo (número de estações)
		numEstacoes = stoi(linha);
		getline(oficinas, linha); // lê a próxima linha (deve ser "-")

		while (getline(oficinas, linha)) {
			estacoes* novaEstacao = new estacoes();
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
			novaEstacao->primeiroCarro = nullptr;

			int index = 0;

			while (getline(oficinas, linha) && linha != "-" && index < novaEstacao->quantidadeCarros) {
				carro* novoCarro = new carro();
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
				novoCarro->proximoCarro = nullptr;

				if (novaEstacao->primeiroCarro == nullptr) {
					novaEstacao->primeiroCarro = novoCarro;
				}
				else {
					carro* ultimoCarro = novaEstacao->primeiroCarro;
					while (ultimoCarro->proximoCarro != nullptr) {
						ultimoCarro = ultimoCarro->proximoCarro;
					}
					ultimoCarro->proximoCarro = novoCarro;
				}

				index++;
			}

			if (primeiraEstacao == nullptr) {
				primeiraEstacao = novaEstacao;
				ultimaEstacao = novaEstacao;
			}
			else {
				ultimaEstacao->proximaEstacao = novaEstacao;
				ultimaEstacao = novaEstacao;
			}
		}
	}
	oficinas.close();

	estacao = primeiraEstacao;

}

//arvoreReparados* encontrarArvorePorEstacao(arvoreReparados* arvore, int idDaET) {
//
//	
//}

//void imprimeArvore(arvoreReparados* raiz, int nivel) {
//	if (raiz == nullptr) {
//		return;
//	}
//
//	imprimeArvore(raiz->direita, nivel + 1);
//
//	for (int i = 0; i < nivel; i++) {
//		cout << "\t";
//	}
//
//	cout << "Marca: " << raiz->marca << " - Modelo: " << raiz->modelo << endl;
//
//	imprimeArvore(raiz->esquerda, nivel + 1);
//}

//void imprimirArvorePorEstacao(arvoreReparados* arvore, int idDaET) {
//	arvoreReparados* estacaoTree = encontrarArvorePorEstacao(arvore, idDaET);
//	if (estacaoTree != nullptr) {
//		cout << "Cars for Station " << idDaET << ":" << endl;
//		imprimeArvore(estacaoTree, 0);
//	}
//	else {
//		cout << "Station " << idDaET << " not found." << endl;
//	}
//}

void gestao(estacoes*& estacao, int numEstacoes, marcas*& marca, int& numeroPalavrasMarcas, int numCarros, carro*& carros, int& numCarrosTotal) {

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

		//reparacaoManual(estacao, numEstacoes, numCarros, carros);

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
		cout << "\nOficina gravada com sucesso\n";

		break;

	case 7:

		uploadEstacao(estacao, numEstacoes, numCarrosTotal);
		uploadFilaDeEspera(carros, numCarrosTotal);
		printETs(estacao);
		printCars(carros);

		break;

	case 8:

		//imprimirCarrosReparados(raiz, 0);

		break;

	default:

		cout << "Não selecionou uma opção válida\n";

		break;
	}
}