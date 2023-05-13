#include <string>
#include <stdlib.h>
#include <time.h>
#include <locale>
#include "structs.h"
#include "inicializacao.h"
#include "gestao.h"

using namespace std;

int main() {

    srand(time(NULL));
    locale::global(locale(""));

    int numeroPalavrasMarcas = 0;
    int numeroPalavrasModelos = 0;

    marcas* marca = nullptr;

    modelos* modelo = nullptr;

    lerMarcas("marcas.txt", marca);
    lerModelos("modelos.txt", modelo);

    carro* carros = nullptr;

    estacoes* estacao = nullptr;

    arvoreReparados* arvores = nullptr;

    menu(numeroPalavrasMarcas, numeroPalavrasModelos, marca, modelo, carros, estacao, arvores);

    limpaMarcas();

    return 0;
}