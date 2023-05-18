#pragma once
#include <stdlib.h>
#include "structs.h"
#include "gestao.h"

void reparacaoManual(estacoes*& estacao, carrosReparados*& raiz);

void tempoReparacao(carro*& carros, int& numEstacoes, estacoes*& estacao);

void adicionarPrioridade(carro*& carros, int& numEstacoes, estacoes*& estacao);

void removerTodosCarros(estacoes*& estacao, const string& nomeMecanico);

void removerMecanico(estacoes*& estacao, marcas*& marca, carro*& carros, int& numeroPalavrasMarcas, int& numEstacoes);

void adicionaET(estacoes*& estacao, marcas*& marca);

void gravarEstacao(estacoes*& estacao, int& numEstacoes);

void gravarFilaDeEspera(carro*& carros);

void uploadFilaDeEspera(carro*& carros, int& numCarrosTotal);

void uploadEstacao(estacoes*& estacao, int& numEstacoes, int& numCarrosTotal);

void removeMarcasDuplicadas(marcas* marca);

void imprimeArvore(carrosReparados* raiz, int nivel, int idET);

void imprimirArvorePorEstacao(carrosReparados* raiz);

void gestao(estacoes*& estacao, int numEstacoes, marcas*& marca, int& numeroPalavrasMarcas, int numCarros, carro*& carros, int& numCarrosTotal, carrosReparados*& raiz);