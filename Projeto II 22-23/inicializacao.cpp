#include <iostream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "structs.h"
#include "inicializacao.h"
#include "gestao.h"

using namespace std;

//método para ler os ficheiros
void lerMarcas(const string& nomeFicheiro, marcas*& marca) {
    ifstream ficheiro(nomeFicheiro);
    string marcaCarro;

    if (ficheiro.is_open()) {
        while (getline(ficheiro, marcaCarro)) {
            marcas* novaMarca = new marcas();
            novaMarca->marca = marcaCarro;
            novaMarca->proximaMarca = nullptr;

            if (marca == nullptr) {
                marca = novaMarca;
            }
            else {
                marcas* ultima = marca;
                while (ultima->proximaMarca != nullptr) {
                    ultima = ultima->proximaMarca;
                }
                ultima->proximaMarca = novaMarca;
            }
        }
        ficheiro.close();
    }
}


void lerModelos(const string& nomeFicheiro, modelos*& modelo){
    ifstream ficheiro(nomeFicheiro);
    string modeloCarro;

    if (ficheiro.is_open()) {
        while (getline(ficheiro, modeloCarro)) {
            modelos* novaMarca = new modelos();
            novaMarca->modelo = modeloCarro;
            novaMarca->proximoModelo = nullptr;

            if (modelo == nullptr) {
                modelo = novaMarca;
            }
            else {
                modelos* ultima = modelo;
                while (ultima->proximoModelo != nullptr) {
                    ultima = ultima->proximoModelo;
                }
                ultima->proximoModelo = novaMarca;
            }
        }
        ficheiro.close();
    }
}

string escolhePalavraRandomMarcas(marcas* marca) {
    marcas* temp = marca;
    int numeroPalavras = 0;

    while (temp != nullptr) {
        numeroPalavras++;
        temp = temp->proximaMarca;
    }

    if (marca == nullptr) {
        return "";
    }

    int numeroRandom = rand() % numeroPalavras;

    marcas* atual = marca;
    int indexAtual = 0;

    while (atual != nullptr) {
        if (indexAtual == numeroRandom) {
            return atual->marca;
        }
        atual = atual->proximaMarca;
        indexAtual++;
    }
    return "";
}

//metodo para escolher uma palavra random
string escolhePalavraRandomModelos(modelos* modelo) {

    modelos* temp = modelo;
    int numeroPalavras = 0;

    while (temp != nullptr) {
        numeroPalavras++;
        temp = temp->proximoModelo;
    }

    if (modelo == nullptr) {
        return "";
    }

    int numeroRandom = rand() % numeroPalavras;

    modelos* atual = modelo;
    int indexAtual = 0;

    while (atual != nullptr) {
        if (indexAtual == numeroRandom) {
            return atual->modelo;
        }
        atual = atual->proximoModelo;
        indexAtual++;
    }
    return "";
}

//void inserirCarroPorModelo(arvoreReparados*& raiz, int idDaET,carro* atual) {
//    if (raiz == nullptr) {
//        raiz = new arvoreReparados();
//        raiz->idDaET = idDaET;
//        raiz->marca = atual->marca;
//        raiz->modelo = atual->modelo;
//        raiz->esquerda = nullptr;
//        raiz->direita = nullptr;
//    }
//    else if (atual->modelo < raiz->modelo) {
//        inserirCarroPorModelo(raiz->esquerda, idDaET, atual);
//    }
//    else {
//        inserirCarroPorModelo(raiz->direita, idDaET, atual);
//    }
//}


//void inserirCarro(arvoreReparados*& raiz, estacoes* estacao, carro* atual) {
//    
//
//}

void criarArvores(arvoreReparados*& arvores, estacoes*& estacao) {

    while (estacao != nullptr) {
        arvoreReparados* novaArvore = new arvoreReparados();

        novaArvore->idDaET = estacao->idET;
        novaArvore->marca = estacao->marcaEspecializada;
        novaArvore->modelo = "";
        novaArvore->direita = nullptr;
        novaArvore->esquerda = nullptr;
        novaArvore->proximaArvore = nullptr;

        novaArvore->proximaArvore = arvores;
        arvores = novaArvore;

        estacao = estacao->proximaEstacao;
    }
}

void organizaETs(estacoes*& estacao) {

    estacoes* atual = estacao->proximaEstacao;
    estacao->proximaEstacao = nullptr;

    while (atual != nullptr) {

        estacoes* proxima = atual->proximaEstacao;

        if (atual->idET < estacao->idET) {
            atual->proximaEstacao = estacao;
            estacao = atual;
        }
        else {
            estacoes* anterior = estacao;

            while (anterior->proximaEstacao != nullptr && (atual->idET > anterior->proximaEstacao->idET)) {
                anterior = anterior->proximaEstacao;
            }
            atual->proximaEstacao = anterior->proximaEstacao;
            anterior->proximaEstacao = atual;
        }
        atual = proxima;
    }
}

// método que organiza a lista de espera
void organizaListaEspera(carro*& carros) {

    carro* atual = carros->proximoCarro; // cria uma nova lista ligada de carros
    carros->proximoCarro = nullptr; // o próximo é null

    while (atual != nullptr) { // percorre todos os carros
        carro* proximo = atual->proximoCarro; // novo carro que recebe o próximo do carro atual

        if (atual->prioritario > carros->prioritario || (atual->prioritario == carros->prioritario && atual->idCarro < carros->idCarro)) { // faz as comparações necessárias
            atual->proximoCarro = carros; // mete esse carro na primeira posição da lista ligada
            carros = atual; // faz a ligação
        }
        else { // caso não seja cumprida a condição
            carro* anterior = carros; // cria uma nova lista ligada

            while (anterior->proximoCarro != nullptr && (atual->prioritario < anterior->proximoCarro->prioritario || (atual->prioritario == anterior->proximoCarro->prioritario && atual->idCarro > anterior->proximoCarro->idCarro))) {
                anterior = anterior->proximoCarro;
            }
            atual->proximoCarro = anterior->proximoCarro;
            anterior->proximoCarro = atual;
        }
        atual = proximo; // vai para o próximo carro na lista ligada
    }
}


// método que remove o carro da estação
void removeCarros(estacoes* estacao) {
    while (estacao != nullptr) { // percorre todas as estações da lista ligada
        carro* atual = estacao->primeiroCarro; // recebe a lista ligada de carros da estacao
        carro* anterior = nullptr; // cria um anterior a null

        while (atual != nullptr) { // vê todos os carros nessa ET
            atual->dias++; // incrementa os dias com o ciclo

            if ((atual->dias == atual->tempoMax) || (rand() % 20 <= 2)) // se os dias forem iguais ao maximo ou se a probabilidade for cumprida
            {
                cout << "O carro com ID " << atual->idCarro << " foi removido da estação " << estacao->idET << ".\n";
                estacao->faturacao += (atual->dias * 50); //adiciona ao valor da faturação da estação

                //invocar a funcao que poe o carro na arvore certa

                if (anterior == nullptr) { // se não tiver carro antes do que vai ser removido
                    estacao->primeiroCarro = atual->proximoCarro; // o primeiro carro da ET passa a ser o seguinte do atual
                }
                else {
                    anterior->proximoCarro = atual->proximoCarro; // caso contrario o seguinte do carro anterior é o proximo ao carro que está a ser removido
                }
                delete atual; // remove o atual
                atual = anterior == nullptr ? estacao->primeiroCarro : anterior->proximoCarro; // 
                estacao->quantidadeCarros--; // diminui a quantidade de carros atual na ET
            }
            else { // caso as condições não sejam cumpridas
                anterior = atual;
                atual = atual->proximoCarro;
            }
        }
        estacao = estacao->proximaEstacao; // passa para a próxima estação
    }
}

// método para criar os carros do ciclo
void criarCarro(carro*& carros, int& numCarrosTotal, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, estacoes*& estacao, marcas*& marca, modelos*& modelo) {

    int numCarrosCriados = 0;

    while(numCarrosCriados < 10) // só cria 10 carros
    {
        string marcaRandom = escolhePalavraRandomMarcas(marca); // vai buscar uma marca random do ficheiro
        string modeloRandom = escolhePalavraRandomModelos(modelo); // vai buscar um modelo random do ficheiro

        estacoes* tempEstacao = estacao; // lista ligada temporaria para conseguir percorrer todas as estações, sem isto após a iteração por todas as estações pela primeira vez no for loop ia ficar a null

        for (int j = 0; tempEstacao != nullptr; j++) { // percorre as estações todas
            if (tempEstacao->marcaEspecializada == marcaRandom) { // vê se a marca random é igual à da estação atual

                carro* novoCarro = new carro(); // cria um novo carro 

                novoCarro->idCarro = numCarrosTotal + 1; // dá o ID
                novoCarro->tempoMax = rand() % 4 + 2; // o seu tempo máximo
                novoCarro->dias = 0; // dias na oficina
                novoCarro->prioritario = rand() % 100 < 5; // prioridade
                novoCarro->marca = marcaRandom; // marca
                novoCarro->modelo = modeloRandom; // modelo
                novoCarro->proximoCarro = nullptr; // próximo carro a null

                novoCarro->proximoCarro = carros;
                carros = novoCarro;

                numCarrosCriados++; // atualiza a variavel de carros criados
                numCarrosTotal++; //atualiza a variavel numCarrosTotal
            }
            tempEstacao = tempEstacao->proximaEstacao; // vai para a proxima estação
        }
    }
}

// método para remover os carros da lista de espera que vão ser adicionados na ET
carro* removeDaListaEspera(carro* carroNaET, carro* carros) {
    if (carroNaET == carros) { // se o carro for o mesmo
        carros = carros->proximoCarro; // é passado para o proximo carro
    }
    else { // caso contrário
        carro* carroAtual = carros; // cria a lista ligada com o resto dos carros que não vão ser adicionados
        while (carroAtual->proximoCarro != carroNaET) { // vê se são diferentes
            carroAtual = carroAtual->proximoCarro; 
        }
        carroAtual->proximoCarro = carroNaET->proximoCarro; // próximo carro do carro que não vai ser removido é o próximo do que vai ser removido
    }
    delete carroNaET; // deleta o carro que vai ser adicionado na ET
    return carros; // dá o return da lista ligada de carros
}

// método para adicionar um carro na ET
void adicionaCarroET(carro*& carros, estacoes*& estacao) {

    int carroAdicionado = 0; //variavel para contar quantos carros foram adicionados
    carro* carroAAdicionar = carros; //recebe a carro de carros

    while (carroAdicionado < 8 && carroAAdicionar != nullptr) { //enquanto o numero de carros adiconados não for 8 nem percorrer a lista ligada de carros toda

        estacoes* estacaoAtual = estacao; // lista ligada temporaria para receber as estações

        while (estacaoAtual != nullptr){ // enquanto houver estações
            if((carroAAdicionar->marca == estacaoAtual->marcaEspecializada) && (estacaoAtual->quantidadeCarros < estacaoAtual->capacidade)){ // se tiver a mesma marca e capacidade
                
                carro* carroET = new carro(); // cria um novo carro
                *carroET = *carroAAdicionar; // igualá ao carro que vai ser adicionado
                carroET->proximoCarro = nullptr; // próximo carro a esse é null

                if (estacaoAtual->primeiroCarro == nullptr) { // se não tiver nenhum carro na primeira posição
                    estacaoAtual->primeiroCarro = carroET; // põe esse carro nessa posição
                }
                else { // caso contrário
                    carro* ultimoCarro = estacaoAtual->primeiroCarro; //lista ligada que recebe os carros da estação
                    while (ultimoCarro->proximoCarro != nullptr) { // enquanto o próximo carro for diferente de null
                        ultimoCarro = ultimoCarro->proximoCarro; 
                    }
                    ultimoCarro->proximoCarro = carroET;
                }
                estacaoAtual->quantidadeCarros++;
                carroAdicionado++;
                carros = removeDaListaEspera(carroAAdicionar, carros);
                carroAAdicionar = carros;
                break;
            }
            estacaoAtual = estacaoAtual->proximaEstacao; //próxima estação
        }
        if (estacaoAtual == nullptr) { //ve se já percorreu as estações todas 
            carroAAdicionar = carroAAdicionar->proximoCarro; //passa para o próximo carro
        }
    }
}

// método para criar os 10 primeiros carros totalmente aleatorios
void primeirosCarros(carro*& carros, marcas*& marca, modelos*& modelo, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, int& numCarrosTotal) {
    for (int i = 0; i < 10; i++) // faz um loop de 10 iterações
    {
        string marcaRandom = escolhePalavraRandomMarcas(marca); // seleciona uma marca random do ficheiro
        string modeloRandom = escolhePalavraRandomModelos(modelo);// seleciona um modelo random do ficheiro

        carro* novoCarro = new carro();

        novoCarro->idCarro = numCarrosTotal + 1;
        novoCarro->tempoMax = rand() % 4 + 2;
        novoCarro->dias = 0;
        novoCarro->prioritario = rand() % 100 < 5;
        novoCarro->marca = marcaRandom;
        novoCarro->modelo = modeloRandom;
        novoCarro->proximoCarro = nullptr;
        
        novoCarro->proximoCarro = carros;
        carros = novoCarro;

        numCarrosTotal++; // atualiza a variavel numCarrosTotal
    }
}


void estacaoTrabalho(estacoes*& estacao, int& numET, int& numEstacoes, int& numeroPalavrasMarcas, marcas*& marca) {

    string nome; // variavel para guardar o nome do mecanico
    getline(cin, nome); // recebe o input do usuario

    for (int i = 0; i < numET; i++) {

        string marcaRandom = escolhePalavraRandomMarcas(marca);

        cout << "Dê um nome ao mecânico " << i + 1 << ": \n"; // põe na consola a mensagem para dar um nome
        getline(cin, nome); // recebe o input do usuario

        estacoes* novaET = new estacoes();

        novaET->idET = numEstacoes + 1;
        novaET->capacidade = rand() % 4 + 2;
        novaET->quantidadeCarros = 0;
        novaET->faturacao = 0;
        novaET->mecanico = nome;
        novaET->marcaEspecializada = marcaRandom;
        novaET->proximaEstacao = nullptr;
        novaET->primeiroCarro = nullptr;

        novaET->proximaEstacao = estacao;
        estacao = novaET;

        numEstacoes++;
    }
}


void printCars(carro* carros) {

    cout << "\nLista de espera: \n";
    for (int i = 1; carros != nullptr; i++) {
        cout << "Carro ID: " << carros->idCarro << " | "
             << "Marca: " << carros->marca << " | "
             << "Modelo: " << carros->modelo << " | "
             << "Prioritário: " << (carros->prioritario ? "Sim" : "Não") << " | "
             << "Tempo de reparação: " << carros->tempoMax << " | "
             << "Dias na oficina: " << carros->dias << "\n";
        cout << "\n";
        carros = carros->proximoCarro;
    }
}

void printETs(estacoes* estacao) {

    while (estacao != nullptr) {
        cout << "\nEstação ";
        cout << "ID: " << estacao->idET << " | "
            << "Mecânico: " << estacao->mecanico << " | "
            << "Capacidade: " << estacao->capacidade << " | "
            << "Carros: " << estacao->quantidadeCarros << " | "
            << "Marca: " << estacao->marcaEspecializada << " | "
            << "Total Faturação: " << estacao->faturacao << "$\n\n";

        for (carro* carroAtual = estacao->primeiroCarro; carroAtual != nullptr; carroAtual = carroAtual->proximoCarro) {

            cout << "Carro ID: " << carroAtual->idCarro << " | "
                << "Marca: " << carroAtual->marca << " | "
                << "Modelo: " << carroAtual->modelo << " | "
                << "Prioritário: " << (carroAtual->prioritario ? "Sim" : "Não") << " | "
                << "Tempo de reparação: " << carroAtual->tempoMax << " | "
                << "Dias na oficina: " << carroAtual->dias << "\n";
        }
        cout << "-------------------------- \n";

        estacao = estacao->proximaEstacao;
    }
}

void limpaMarcas() {

    ifstream marcas("marcas.txt");

    if (marcas.is_open()) {
        ofstream marcasTemp("temp.txt");

        if (marcasTemp.is_open()) {
            string linha;
            int numeroLinha = 1;
            int comecaLinha = 46;

            while (getline(marcas, linha)) {
                if (numeroLinha <= comecaLinha) {
                    marcasTemp << linha << "\n";
                }
                numeroLinha++;
            }

            marcas.close();
            marcasTemp.close();

            remove("marcas.txt");
            rename("temp.txt", "marcas.txt");
        }
    }
}


// método que faz o output inicial a pedir os ficheiros, e pede os inputs para fazer os ciclos ou a gestao
void menu(int& numeroPalavrasMarcas, int& numeroPalavrasModelos, marcas*& marca, modelos*& modelo, carro*& carros, estacoes*& estacao) {

    char escolha;
    int numCarros = 0;
    int numCarrosTotal = 0;
    int numEstacoes = 0;
    int numET = rand() % 6 + 3;
    bool ficheiros = true;
    bool sair = false;

    while (ficheiros)
    {
        cout << "Pretende inserir dados de ET's e carros guardados em ficheiros? (s/n)\n";
        cin >> escolha;

        switch (escolha)
        {
        case 's':

            uploadEstacao(estacao, numEstacoes, numCarrosTotal);
            uploadFilaDeEspera(carros, numCarrosTotal);
            //upload das arvores, senão não dá para imprimir
            printETs(estacao);
            printCars(carros);
            ficheiros = false;

            break;

        case 'n':

            ficheiros = false;

            primeirosCarros(carros, marca, modelo, numeroPalavrasMarcas, numeroPalavrasModelos, numCarrosTotal);

            estacaoTrabalho(estacao, numET, numEstacoes, numeroPalavrasMarcas, marca);

            organizaETs(estacao);

            printETs(estacao);

            organizaListaEspera(carros);

            printCars(carros);

            break;

        default:

            cout << "Escolha uma das opções, s ou n\n";

            break;
        }
    }
    while (!sair) {
        char opcao;

        cout << "\n\nDia (s)eguinte ********* (g)estão\n"
            << "       (e)scapar às contas\n"
            << "Selecione a sua opção:\n";
        cin >> opcao;

        switch (opcao)
        {
        case 's':

            adicionaCarroET(carros, estacao);

            criarCarro(carros, numCarrosTotal, numeroPalavrasMarcas, numeroPalavrasModelos, estacao, marca, modelo);

            removeCarros(estacao);

            organizaETs(estacao);

            printETs(estacao);

            organizaListaEspera(carros);

            printCars(carros);

            break;

        case 'g':

            gestao(estacao, numEstacoes, marca, numeroPalavrasMarcas, numCarros, carros, numCarrosTotal);

            break;

        case 'e':

            sair = true;
            break;

        default:

            cout << "Não selecionou uma opção valida\n";

            break;
        }
    }
}