#pragma once
#include <stdlib.h>
#include "structs.h"
#include "gestao.h"

void lerMarcas(marcas*& marca);

void lerModelos(modelos*& modelo);

string escolhePalavraRandomMarcas(marcas* marca);

string escolhePalavraRandomModelos(modelos* modelo);

carrosReparados* novoCarro(int idET, carro* carroReparado);

void inserirNaArvore(carrosReparados*& raiz, int idET, carro* carroReparado);

void organizaETs(estacoes*& estacao);

void organizaListaEspera(carro*& carros);

void removeCarros(estacoes*& estacao, carrosReparados*& raiz);

void criarCarro(carro*& carros, int& numCarrosTotal, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, estacoes*& estacao, marcas*& marca, modelos*& modelo);

carro* removeDaListaEspera(carro* carroNaET, carro* carros);

void adicionaCarroET(carro*& carros, estacoes*& estacao);

void primeirosCarros(carro*& carros, marcas*& marca, modelos*& modelo, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, int& numCarrosTotal);

void estacaoTrabalho(estacoes*& estacao, int& numET, int& numEstacoes, int& numeroPalavrasMarcas, marcas*& marca);

void printCars(carro* carros);

void printETs(estacoes* estacao);

void limpaMarcas();

void menu(int& numeroPalavrasMarcas, int& numeroPalavrasModelos, marcas*& marca, modelos*& modelo, carro*& carros, estacoes*& estacao, carrosReparados*& raiz);