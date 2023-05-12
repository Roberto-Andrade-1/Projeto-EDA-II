#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include "structs.h"
#include "gestao.h"

using namespace std;

void lerMarcas(const string& nomeFicheiro, marcas*& marca);

void lerModelos(const string& nomeFicheiro, modelos*& modelo);

string escolhePalavraRandomMarcas(marcas* marca);

string escolhePalavraRandomModelos(modelos* modelo);

void organizaETs(estacoes*& estacao);

void organizaListaEspera(carro*& carros);

void removeCarros(estacoes* estacao);

void criarCarro(carro*& carros, int& numCarrosTotal, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, estacoes*& estacao, marcas*& marca, modelos*& modelo);

carro* removeDaListaEspera(carro* carroNaET, carro* carros);

void adicionaCarroET(carro*& carros, estacoes*& estacao);

void primeirosCarros(carro*& carros, marcas*& marca, modelos*& modelo, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, int& numCarrosTotal);

void estacaoTrabalho(estacoes*& estacao, int& numET, int& numEstacoes, int& numeroPalavrasMarcas, marcas*& marca);

void printCars(carro* carros);

void printETs(estacoes* estacao);

void menu(int& numeroPalavrasMarcas, int& numeroPalavrasModelos, marcas*& marca, modelos*& modelo, carro*& carros, estacoes*& estacao);

void limpaMarcas();