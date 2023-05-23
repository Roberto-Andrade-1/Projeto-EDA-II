#pragma once
#include <stdlib.h>
#include "structs.h"
#include "gestao.h"

using namespace std;

void lerMarcas(marcas*& marca);

void lerModelos(modelos*& modelo);

string escolhePalavraRandomMarcas(marcas* marca);

string escolhePalavraRandomModelos(modelos* modelo);

void estacaoTrabalho(estacoes*& estacao, int& numET, int& numEstacoes, marcas*& marca);

void primeirosCarros(carro*& carros, marcas*& marca, modelos*& modelo, int& numCarrosTotal);

void adicionaCarroET(carro*& carros, estacoes*& estacao);

carro* removeDaListaEspera(carro* carroNaET, carro* carros);

void removeCarros(estacoes*& estacao, carrosReparados*& raiz);

carrosReparados* novoCarroArvore(int idET, carro* carroReparado);

void inserirNaArvore(carrosReparados*& raiz, int idET, carro* carroReparado);

void criarCarro(carro*& carros, int& numCarrosTotal, estacoes*& estacao, marcas*& marca, modelos*& modelo);

void organizaETs(estacoes*& estacao);

void organizaListaEspera(carro*& carros);

void printETs(estacoes* estacao);

void printCars(carro* carros);

void limpaMarcas();

void menu(marcas*& marca, modelos*& modelo, carro*& carros, estacoes*& estacao, carrosReparados*& raiz);