#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include "structs.h"
#include "gestao.h"

using namespace std;

string* lerFicheiro(const string& nomeFicheiro, int& numeroPalavras);

string escolhePalavraRandom(const string* palavras, int numeroPalavras);

void removeCarros(estacoes* estacao);

void criarCarro(carro*& carros, int& numCarros, int& numCarrosTotal, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, estacoes*& estacao, string marcas[], string modelos[]);

carro* removeDaListaEspera(carro* carroNaET, carro* carros);

void adicionaCarroET(carro*& carros, estacoes*& estacao);

void primeirosCarros(carro*& carros, string marcas[], string modelos[], int& numeroPalavrasMarcas, int& numeroPalavrasModelos, int& numCarros, int& numCarrosTotal);

void estacaoTrabalho(estacoes*& estacao, int& numET, int& numEstacoes, int& numeroPalavrasMarcas, string marcas[]);

void printCars(carro* carros);

void printETs(estacoes* estacao, int& numEstacoes);

void menu(int& numeroPalavrasMarcas, int& numeroPalavrasModelos, string marcas[], string modelos[], carro*& carros, estacoes*& estacao);
