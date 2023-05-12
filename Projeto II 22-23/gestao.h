#pragma once
#include <stdlib.h>
#include "structs.h"
#include "gestao.h"

void uploadEstacao(estacoes*& estacao, int& numEstacoes, int& numCarrosTotal);

void uploadFilaDeEspera(carro*& carros, int& numCarrosTotal);

void gestao(estacoes*& estacao, int numEstacoes, marcas*& marca, int& numeroPalavrasMarcas, int numCarros, carro*& carros, int& numCarrosTotal);