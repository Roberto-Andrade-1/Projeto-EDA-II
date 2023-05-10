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


void removerTodosCarros(estacoes*& estacao) {

	carro* atual = estacao->primeiroCarro;
	carro* anterior = nullptr;

	while (atual != nullptr) {

		cout << "O carro com ID " << atual->idCarro << " foi removido da estação " << estacao->idET << ".\n";
		estacao->faturacao += (atual->dias * 50);

		if (anterior == nullptr) {
			estacao->primeiroCarro = atual->proximoCarro;
		}
		else {
			anterior->proximoCarro = atual->proximoCarro;
		}
		carro* temp = atual->proximoCarro;
		delete atual;
		atual = temp;
		estacao->quantidadeCarros--;
		anterior = atual;
	}
}

void removerMecanico(estacoes*& estacao, string marcas[], carro*& carros, int& numeroPalavrasMarcas, int& numEstacoes) {

	string nomeMecanico;

	getline(cin, nomeMecanico);
	cout << "Introduza o nome do mecânico a remover: ";
	getline(cin, nomeMecanico);

	estacoes* atual = estacao;
	estacoes* anterior = nullptr;
	string novoMecanico;

	while (atual != nullptr) {
		if (atual->mecanico == nomeMecanico) {
			
			removerTodosCarros(estacao);

			cout << "\nDê um nome ao mecânico: ";
			getline(cin, novoMecanico);
			cout << "\n";

			string marcaRandom = escolhePalavraRandom(marcas, numeroPalavrasMarcas);

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

void adicionaET(estacoes*& estacao) {

	string nomeMecanico;
	string marca;

	cout << "\nIntroduza o nome do mecânico: ";
	getline(cin, nomeMecanico);

	cout << "\nIntroduza a marca: ";
	getline(cin, marca);



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

void gestao(estacoes*& estacao, int numEstacoes, string marcas[], int& numeroPalavrasMarcas, int numCarros, carro*& carros, int& numCarrosTotal) {

	int options;

	cout << "\n***** Bem vindo gestor *****\n";
	cout << "(1).Reparação Manual\n";
	cout << "(2).Atualizar tempo de reparação\n";
	cout << "(3).Adicionar Prioridade\n";
	cout << "(4).Remover Mecânico\n";
	cout << "(5).Gravar Oficina\n";
	cout << "(6).Carregar Oficina\n";
	cout << "(7).Imprimir Oficina\n";
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

		removerMecanico(estacao, marcas, carros, numeroPalavrasMarcas, numEstacoes);

		break;

	case 5:

		adicionaET(estacao);

		break;

	case 6:

		gravarEstacao(estacao, numEstacoes);
		gravarFilaDeEspera(carros);
		cout << "\nOficina gravada com sucesso\n";

		break;

	case 7:

		//uploadEstacao(estacao, numEstacoes, numCarrosTotal);
		//uploadFilaDeEspera(numCarros, carros, numCarrosTotal);
		printETs(estacao);
		printCars(carros);

		break;

	case 8:

		//imprimirCarrosReparados();

		break;

	default:

		cout << "Não selecionou uma opção válida\n";

		break;
	}
}