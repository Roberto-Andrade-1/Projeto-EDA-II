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

//m�todo para ler os ficheiros
string* lerFicheiro(const string& nomeFicheiro, int& numeroPalavras) {
    ifstream ficheiro(nomeFicheiro); // d� um nome ao ficheiro aberto
    string* palavras = nullptr; //um apontador a null para obter as palavras
    string linha; //variavel para as linhas
    numeroPalavras = 0; //variavel a zero do numero de palavras lidas

    while (getline(ficheiro, linha)) { //equanto tiver linhas faz estas opera��es
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
    int randomIndex = rand() % numeroPalavras; //obtem o resto da divisao do rand a dividir pelo n�mero de palavras do ficheiro
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

// m�todo que organiza a lista de espera
void organizaListaEspera(carro*& carros) {

    carro* atual = carros->proximoCarro; // cria uma nova lista ligada de carros
    carros->proximoCarro = nullptr; // o pr�ximo � null

    while (atual != nullptr) { // percorre todos os carros
        carro* proximo = atual->proximoCarro; // novo carro que recebe o pr�ximo do carro atual

        if (atual->prioritario > carros->prioritario || (atual->prioritario == carros->prioritario && atual->idCarro < carros->idCarro)) { // faz as compara��es necess�rias
            atual->proximoCarro = carros; // mete esse carro na primeira posi��o da lista ligada
            carros = atual; // faz a liga��o
        }
        else { // caso n�o seja cumprida a condi��o
            carro* anterior = carros; // cria uma nova lista ligada

            while (anterior->proximoCarro != nullptr && (atual->prioritario < anterior->proximoCarro->prioritario || (atual->prioritario == anterior->proximoCarro->prioritario && atual->idCarro > anterior->proximoCarro->idCarro))) {
                anterior = anterior->proximoCarro;
            }
            atual->proximoCarro = anterior->proximoCarro;
            anterior->proximoCarro = atual;
        }
        atual = proximo; // vai para o pr�ximo carro na lista ligada
    }
}


// m�todo que remove o carro da esta��o
void removeCarros(estacoes* estacao) {
    while (estacao != nullptr) { // percorre todas as esta��es da lista ligada
        carro* atual = estacao->primeiroCarro; // recebe a lista ligada de carros da estacao
        carro* anterior = nullptr; // cria um anterior a null

        while (atual != nullptr) { // v� todos os carros nessa ET
            atual->dias++; // incrementa os dias com o ciclo

            if ((atual->dias == atual->tempoMax) || (rand() % 20 <= 2)) // se os dias forem iguais ao maximo ou se a probabilidade for cumprida
            {
                cout << "O carro com ID " << atual->idCarro << " foi removido da esta��o " << estacao->idET << ".\n";
                estacao->faturacao += (atual->dias * 50); //adiciona ao valor da fatura��o da esta��o

                if (anterior == nullptr) { // se n�o tiver carro antes do que vai ser removido
                    estacao->primeiroCarro = atual->proximoCarro; // o primeiro carro da ET passa a ser o seguinte do atual
                }
                else {
                    anterior->proximoCarro = atual->proximoCarro; // caso contrario o seguinte do carro anterior � o proximo ao carro que est� a ser removido
                }
                delete atual; // remove o atual
                atual = anterior == nullptr ? estacao->primeiroCarro : anterior->proximoCarro; // 
                estacao->quantidadeCarros--; // diminui a quantidade de carros atual na ET
            }
            else { // caso as condi��es n�o sejam cumpridas
                anterior = atual;
                atual = atual->proximoCarro;
            }
        }
        estacao = estacao->proximaEstacao; // passa para a pr�xima esta��o
    }
}

// m�todo para criar os carros do ciclo
void criarCarro(carro*& carros, int& numCarrosTotal, int& numeroPalavrasMarcas, int& numeroPalavrasModelos, estacoes*& estacao, string marcas[], string modelos[]) {

    int numCarrosCriados = 0;

    while(numCarrosCriados < 10) // s� cria 10 carros
    {
        string marcaRandom = escolhePalavraRandom(marcas, numeroPalavrasMarcas); // vai buscar uma marca random do ficheiro
        string modeloRandom = escolhePalavraRandom(modelos, numeroPalavrasModelos); // vai buscar um modelo random do ficheiro

        estacoes* tempEstacao = estacao; // lista ligada temporaria para conseguir percorrer todas as esta��es, sem isto ap�s a itera��o por todas as esta��es pela primeira vez no for loop ia ficar a null

        for (int j = 0; tempEstacao != nullptr; j++) { // percorre as esta��es todas
            if (tempEstacao->marcaEspecializada == marcaRandom) { // v� se a marca random � igual � da esta��o atual

                carro* novoCarro = new carro(); // cria um novo carro 

                novoCarro->idCarro = numCarrosTotal + 1; // d� o ID
                novoCarro->tempoMax = rand() % 4 + 2; // o seu tempo m�ximo
                novoCarro->dias = 0; // dias na oficina
                novoCarro->prioritario = rand() % 100 < 5; // prioridade
                novoCarro->marca = marcaRandom; // marca
                novoCarro->modelo = modeloRandom; // modelo
                novoCarro->proximoCarro = nullptr; // pr�ximo carro a null

                novoCarro->proximoCarro = carros;
                carros = novoCarro;

                numCarrosCriados++; // atualiza a variavel de carros criados
                numCarrosTotal++; //atualiza a variavel numCarrosTotal
            }
            tempEstacao = tempEstacao->proximaEstacao; // vai para a proxima esta��o
        }
    }
}

// m�todo para remover os carros da lista de espera que v�o ser adicionados na ET
carro* removeDaListaEspera(carro* carroNaET, carro* carros) {
    if (carroNaET == carros) { // se o carro for o mesmo
        carros = carros->proximoCarro; // � passado para o proximo carro
    }
    else { // caso contr�rio
        carro* carroAtual = carros; // cria a lista ligada com o resto dos carros que n�o v�o ser adicionados
        while (carroAtual->proximoCarro != carroNaET) { // v� se s�o diferentes
            carroAtual = carroAtual->proximoCarro; 
        }
        carroAtual->proximoCarro = carroNaET->proximoCarro; // pr�ximo carro do carro que n�o vai ser removido � o pr�ximo do que vai ser removido
    }
    delete carroNaET; // deleta o carro que vai ser adicionado na ET
    return carros; // d� o return da lista ligada de carros
}

// m�todo para adicionar um carro na ET
void adicionaCarroET(carro*& carros, estacoes*& estacao) {

    int carroAdicionado = 0; //variavel para contar quantos carros foram adicionados
    carro* carroAAdicionar = carros; //recebe a carro de carros

    while (carroAdicionado < 8 && carroAAdicionar != nullptr) { //enquanto o numero de carros adiconados n�o for 8 nem percorrer a lista ligada de carros toda

        estacoes* estacaoAtual = estacao; // lista ligada temporaria para receber as esta��es

        while (estacaoAtual != nullptr){ // enquanto houver esta��es
            if((carroAAdicionar->marca == estacaoAtual->marcaEspecializada) && (estacaoAtual->quantidadeCarros < estacaoAtual->capacidade)){ // se tiver a mesma marca e capacidade
                
                carro* carroET = new carro(); // cria um novo carro
                *carroET = *carroAAdicionar; // igual� ao carro que vai ser adicionado
                carroET->proximoCarro = nullptr; // pr�ximo carro a esse � null

                if (estacaoAtual->primeiroCarro == nullptr) { // se n�o tiver nenhum carro na primeira posi��o
                    estacaoAtual->primeiroCarro = carroET; // p�e esse carro nessa posi��o
                }
                else { // caso contr�rio
                    carro* ultimoCarro = estacaoAtual->primeiroCarro; //lista ligada que recebe os carros da esta��o
                    while (ultimoCarro->proximoCarro != nullptr) { // enquanto o pr�ximo carro for diferente de null
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
            estacaoAtual = estacaoAtual->proximaEstacao; //pr�xima esta��o
        }
        if (estacaoAtual == nullptr) { //ve se j� percorreu as esta��es todas 
            carroAAdicionar = carroAAdicionar->proximoCarro; //passa para o pr�ximo carro
        }
    }
}

// m�todo para criar os 10 primeiros carros totalmente aleatorios
void primeirosCarros(carro*& carros, string marcas[], string modelos[], int& numeroPalavrasMarcas, int& numeroPalavrasModelos, int& numCarrosTotal) {
    for (int i = 0; i < 10; i++) // faz um loop de 10 itera��es
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

        cout << "D� um nome ao mec�nico " << i + 1 << ": \n"; // p�e na consola a mensagem para dar um nome
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
             << "Priorit�rio: " << (carros->prioritario ? "Sim" : "N�o") << " | "
             << "Tempo de repara��o: " << carros->tempoMax << " | "
             << "Dias na oficina: " << carros->dias << "\n";
        cout << "\n";
        carros = carros->proximoCarro;
    }
}

void printETs(estacoes* estacao) {

    while (estacao != nullptr) {
        cout << "\nEsta��o ";
        cout << "ID: " << estacao->idET << " | "
            << "Mec�nico: " << estacao->mecanico << " | "
            << "Capacidade: " << estacao->capacidade << " | "
            << "Carros: " << estacao->quantidadeCarros << " | "
            << "Marca: " << estacao->marcaEspecializada << " | "
            << "Total Fatura��o: " << estacao->faturacao << "$\n\n";

        for (carro* carroAtual = estacao->primeiroCarro; carroAtual != nullptr; carroAtual = carroAtual->proximoCarro) {

            cout << "Carro ID: " << carroAtual->idCarro << " | "
                << "Marca: " << carroAtual->marca << " | "
                << "Modelo: " << carroAtual->modelo << " | "
                << "Priorit�rio: " << (carroAtual->prioritario ? "Sim" : "N�o") << " | "
                << "Tempo de repara��o: " << carroAtual->tempoMax << " | "
                << "Dias na oficina: " << carroAtual->dias << "\n";
        }
        cout << "-------------------------- \n";

        estacao = estacao->proximaEstacao;
    }
}

// m�todo que faz o output inicial a pedir os ficheiros, e pede os inputs para fazer os ciclos ou a gestao
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

            cout << "Escolha uma das op��es, s ou n\n";

            break;
        }
    }
    while (!sair) {
        char opcao;

        cout << "\n\nDia (s)eguinte ********* (g)est�o\n"
            << "       (e)scapar �s contas\n"
            << "Selecione a sua op��o:\n";
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

            cout << "N�o selecionou uma op��o valida\n";

            break;
        }
    }
}