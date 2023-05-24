#pragma once
#include <stdlib.h>
#include "structs.h"
#include "inicializacao.h"

void reparacaoManual(estacoes*& estacao, carrosReparados*& raiz);

void tempoReparacao(carro*& carros, int& numEstacoes, estacoes*& estacao);

void adicionarPrioridade(carro*& carros, int& numEstacoes, estacoes*& estacao);

void removerMecanico(estacoes*& estacao, marcas*& marca, carro*& carros, int& numEstacoes, carrosReparados* raiz);

void removerTodosCarros(estacoes*& estacao, const string& nomeMecanico);

carrosReparados* removerNodo(carrosReparados* raiz, carrosReparados* noRem, carrosReparados* ant);

carrosReparados* removerNodo(carrosReparados* raiz, int num);

void adicionaET(estacoes*& estacao, marcas*& marca);

void gravarEstacao(estacoes*& estacao, int& numEstacoes);

void gravarFilaDeEspera(carro*& carros);

void nomeFicheiro(carrosReparados* raiz);

void gravarArvores(carrosReparados* raiz, ofstream& arvore);

void uploadEstacao(estacoes*& estacao, int& numEstacoes, int& numCarrosTotal);

void uploadFilaDeEspera(carro*& carros, int& numCarrosTotal);

void uploadArvores(carrosReparados*& raiz, const string ficheiro);

void removeMarcasDuplicadas(marcas* marca);

void imprimirArvorePorEstacao(carrosReparados* raiz);

void imprimeArvore(carrosReparados* raiz, int nivel, int idET);

void imprimirModelo(carrosReparados* raiz, int idET);

void gestao(estacoes*& estacao, int numEstacoes, marcas*& marca, int numCarros, carro*& carros, int& numCarrosTotal, carrosReparados*& raiz);
