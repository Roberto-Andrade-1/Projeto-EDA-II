#include <iostream>
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
string* lerFicheiro(const string& nomeFicheiro, int& numeroPalavras) {
    ifstream ficheiro(nomeFicheiro); // dá um nome ao ficheiro aberto
    string* palavras = nullptr; //um apontador a null para obter as palavras
    string linha; //variavel para as linhas
    numeroPalavras = 0; //variavel a zero do numero de palavras lidas

    while (getline(ficheiro, linha)) { //equanto tiver linhas faz estas operações
        istringstream linhaStream(linha); //se for uma linha 
        string palavra;
        while (linhaStream >> palavra) {
            string* temp = new string[numeroPalavras + 1];
            for (int i = 0; i < numeroPalavras; i++) {
                temp[i] = palavras[i];
            }
            temp[numeroPalavras] = palavra;
            delete[] palavras;
            palavras = temp;
            numeroPalavras++;
        }
    }
    return palavras;
}

//metodo para escolher uma palavra random
string escolhePalavraRandom(const string* palavras, int numeroPalavras) {
    int randomIndex = rand() % numeroPalavras; //obtem o resto da divisao do rand a dividir pelo número de palavras do ficheiro
    return palavras[randomIndex]; //faz o return da palavra nesse index 
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
void criarCarro(carro*& carros, int& numCarrosTotal, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, estacoes*& estacao, string marcas[], string modelos[]) {

    int numCarrosCriados = 0;

    while(numCarrosCriados < 10) // só cria 10 carros
    {
        string marcaRandom = escolhePalavraRandom(marcas, numeroPalavrasMarcas); // vai buscar uma marca random do ficheiro
        string modeloRandom = escolhePalavraRandom(modelos, numeroPalavrasModelos); // vai buscar um modelo random do ficheiro

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
void primeirosCarros(carro*& carros, string marcas[], string modelos[], int& numeroPalavrasMarcas, int& numeroPalavrasModelos, int& numCarrosTotal) {
    for (int i = 0; i < 10; i++) // faz um loop de 10 iterações
    {
        string marcaRandom = escolhePalavraRandom(marcas, numeroPalavrasMarcas); // seleciona uma marca random do ficheiro
        string modeloRandom = escolhePalavraRandom(modelos, numeroPalavrasModelos);// seleciona um modelo random do ficheiro

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


void estacaoTrabalho(estacoes*& estacao, int& numET, int& numEstacoes, int& numeroPalavrasMarcas, string marcas[]) {

    string nome; // variavel para guardar o nome do mecanico
    getline(cin, nome); // recebe o input do usuario

    for (int i = 0; i < numET; i++) {

        string marcaRandom = escolhePalavraRandom(marcas, numeroPalavrasMarcas);

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

// método que faz o output inicial a pedir os ficheiros, e pede os inputs para fazer os ciclos ou a gestao
void menu(int& numeroPalavrasMarcas, int& numeroPalavrasModelos, string marcas[], string modelos[], carro*& carros, estacoes*& estacao) {

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

            //uploadEstacao(estacao, numEstacoes, numCarrosTotal);
            //uploadFilaDeEspera(numCarros, carros, numCarrosTotal);
            printETs(estacao);
            printCars(carros);
            ficheiros = false;

            break;

        case 'n':

            ficheiros = false;

            primeirosCarros(carros, marcas, modelos, numeroPalavrasMarcas, numeroPalavrasModelos, numCarrosTotal);

            estacaoTrabalho(estacao, numET, numEstacoes, numeroPalavrasMarcas, marcas);

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

            criarCarro(carros, numCarrosTotal, numeroPalavrasMarcas, numeroPalavrasModelos, estacao, marcas, modelos);

            removeCarros(estacao);

            organizaETs(estacao);

            printETs(estacao);

            organizaListaEspera(carros);

            printCars(carros);

            break;

        case 'g':

            gestao(estacao, numEstacoes, marcas, numeroPalavrasMarcas, numCarros, carros, numCarrosTotal);

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