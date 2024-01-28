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

    marcas* marca = nullptr;

    modelos* modelo = nullptr;

    lerMarcas(marca);
    lerModelos(modelo);

    carro* carros = nullptr;

    estacoes* estacao = nullptr;

    carrosReparados* raiz = nullptr;

    menu(marca, modelo, carros, estacao, raiz);

    limpaMarcas();

    return 0;
}