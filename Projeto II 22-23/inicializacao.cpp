#include <iostream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include "structs.h"
#include "inicializacao.h"
#include "gestao.h"

using namespace std;

//método para ler o ficheiro e adicionar a marca à lista ligada das marcas
void lerMarcas(marcas*& marca) {
    ifstream ficheiro("marcas.txt"); // recebe o ficheiro para leitura
    string marcaCarro; // váriavel para guardar a marca

    if (ficheiro.is_open()) { // se tiver aberto
        while (getline(ficheiro, marcaCarro)) { // enquanto houver linhas para ler, atribui à string marcaCarro o valor 
            marcas* novaMarca = new marcas(); // cria uma lista ligada de marcas
            novaMarca->marca = marcaCarro; // dá o valor marcaCarro ao atributo marca da struct marcas
            novaMarca->proximaMarca = nullptr; // e a proxima marca é null

            if (marca == nullptr) { // se a lista ligada estiver vazia
                marca = novaMarca; // adiciona a nova marca ao inicio da lista
            }
            else { // caso contrário
                marcas* ultima = marca; // cria uma nova variável do tipo marca para ser o seguinte na lista
                while (ultima->proximaMarca != nullptr) { // enquanto houver marcas
                    ultima = ultima->proximaMarca; // vai passando 
                }
                ultima->proximaMarca = novaMarca; // para adicionar essa nova marca ao final da lista
            }
        }
        ficheiro.close(); // e fecha o ficheiro
    }
}

// método para ler o ficheiro e adiconar o modelo à lista ligada de modelos
void lerModelos(modelos*& modelo){
    ifstream ficheiro("modelos.txt"); // recebe o ficheiro para leitura
    string modeloCarro; // váriavel para guardar o modelo

    if (ficheiro.is_open()) { // se tiver aberto
        while (getline(ficheiro, modeloCarro)) { // enquanto houver linhas para ler, atribui à string modeloCarro o valor 
            modelos* novoModelo = new modelos(); // cria uma lista ligada de modelos 
            novoModelo->modelo = modeloCarro; // dá o valor modeloCarro ao atributo modelo da struct modelos
            novoModelo->proximoModelo = nullptr; // e o proximo modelo é null

            if (modelo == nullptr) { // se a lista ligada estiver vazia
                modelo = novoModelo; // adiciona o novo modelo ao inicio da lista
            }
            else { // caso contrário
                modelos* ultima = modelo; // cria uma nova variável do tipo modelo para ser o seguinte na lista
                while (ultima->proximoModelo != nullptr) { // enquanto houver modelos
                    ultima = ultima->proximoModelo; // vai passando 
                }
                ultima->proximoModelo = novoModelo; // para adicionar esse novo modelo ao final da lista
            }
        }
        ficheiro.close(); // e fecha o ficheiro
    }
}

// método para escolher um marca random
string escolhePalavraRandomMarcas(marcas* marca) {
    marcas* temp = marca; // recebe as marcas
    int numeroPalavras = 0; // variável para armazenar o número de palavras

    while (temp != nullptr) { // enquanto houver marcas
        numeroPalavras++; // incrementa o número de palavras
        temp = temp->proximaMarca; // passa para a próxima marca
    }

    if (marca == nullptr) { // se for null
        return ""; // retorna nada
    }

    int numeroRandom = rand() % numeroPalavras; // atribui à váriavel um valor aleatório entre 0 e o número de palavras

    marcas* atual = marca; // recebe a marca atual
    int indexAtual = 0; // váriavel para o index

    while (atual != nullptr) { // enquanto houver marcas
        if (indexAtual == numeroRandom) { // se o index for igual ao número random
            return atual->marca; // retorna a marca
        }
        atual = atual->proximaMarca; // passa para a próxima marca
        indexAtual++; // incrementa o index
    }
    return "";
}

//metodo para escolher uma palavra random
string escolhePalavraRandomModelos(modelos* modelo) {

    modelos* temp = modelo; // cria uma lista ligada temporaria dos modelos
    int numeroPalavras = 0; // variável para armazenar o número de palavras

    while (temp != nullptr) { // enquanto ouver modelos
        numeroPalavras++; // acrescenta o número de palavras
        temp = temp->proximoModelo; // passa para o próximo modelo
    }

    if (modelo == nullptr) { // se o modelo for null
        return ""; 
    }

    int numeroRandom = rand() % numeroPalavras; // atribui à váriavel um valor aleatório entre 0 e o número de palavras

    modelos* atual = modelo; // recebe o modelo atual
    int indexAtual = 0; // e o index atual

    while (atual != nullptr) { // enquanto houver modelos
        if (indexAtual == numeroRandom) { // se o index for igual ao número random
            return atual->modelo; // dá return do modelo
        }
        atual = atual->proximoModelo; // passa para o próximo modelo
        indexAtual++; // incrementa o index
    }
    return "";
}

// método para criar um carro para a arvore
carrosReparados* novoCarro(int idET, carro* carroReparado) { // recebe a et e o carro que foi arranjado
    carrosReparados* novoCarro = new carrosReparados;
    novoCarro->idET = idET;
    novoCarro->carros = carroReparado;
    novoCarro->esquerda = NULL;
    novoCarro->direita = NULL; // cria o reparado com as variaveis certas
    return novoCarro;
}

// método para inserir o carro na arvore pela ordem correta
void inserirNaArvore(carrosReparados*& raiz, int idET, carro* carroReparado) {

    if (raiz == nullptr) {
        raiz = novoCarro(idET, carroReparado);
        return;
    }
    if (carroReparado->marca < raiz->carros->marca) {
        inserirNaArvore(raiz->esquerda, idET, carroReparado);
    }
    else if (carroReparado->marca > raiz->carros->marca) {
        inserirNaArvore(raiz->direita, idET, carroReparado);
    }
    else {
        if (carroReparado->modelo < raiz->carros->modelo) {
            inserirNaArvore(raiz->esquerda, idET, carroReparado);
        }
        else {
            inserirNaArvore(raiz->direita, idET, carroReparado);
        }
    }
}

// método para organizar as estações
void organizaETs(estacoes*& estacao) {

    estacoes* atual = estacao->proximaEstacao; // recebe as estações 
    estacao->proximaEstacao = nullptr; // proxima estação é null

    while (atual != nullptr) { // enquanto tiver estações

        estacoes* proxima = atual->proximaEstacao; // recebe a proxima da estação atual

        if (atual->idET < estacao->idET) { // se o id da atual for menor que a primeira 
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

            while (anterior->proximoCarro != nullptr && (atual->prioritario < anterior->proximoCarro->prioritario || (atual->prioritario == anterior->proximoCarro->prioritario && atual->idCarro > anterior->proximoCarro->idCarro))) { // verifica as condições necessárias se for prioritário ou por id e prioridade
                anterior = anterior->proximoCarro; 
            }
            atual->proximoCarro = anterior->proximoCarro;
            anterior->proximoCarro = atual;
        }
        atual = proximo; // vai para o próximo carro na lista ligada
    }
}


// método que remove o carro da estação
void removeCarros(estacoes*& estacao, carrosReparados*& raiz) {

    estacoes* temp = estacao; // cópia das estações

    while (temp != nullptr) { // percorre todas as estações da lista ligada
        carro* atual = temp->primeiroCarro; // recebe a lista ligada de carros da estacao
        carro* anterior = nullptr; // cria um anterior a null

        while (atual != nullptr) { // vê todos os carros nessa ET
            atual->dias++; // incrementa os dias com o ciclo

            if ((atual->dias == atual->tempoMax) || (rand() % 20 <= 2)) // se os dias forem iguais ao maximo ou se a probabilidade for cumprida
            {
                cout << "O carro com ID " << atual->idCarro << " foi removido da estação " << temp->idET << ".\n";
                temp->faturacao += (atual->dias * 50); //adiciona ao valor da faturação da estação

                carro* copia = new carro(*atual);

                inserirNaArvore(raiz, temp->idET, copia);

                if (anterior == nullptr) { // se não tiver carro antes do que vai ser removido
                    temp->primeiroCarro = atual->proximoCarro; // o primeiro carro da ET passa a ser o seguinte do atual
                }
                else {
                    anterior->proximoCarro = atual->proximoCarro; // caso contrario o seguinte do carro anterior é o proximo ao carro que está a ser removido
                }
                delete atual; // remove o atual
                atual = anterior == nullptr ? temp->primeiroCarro : anterior->proximoCarro; // se o anterior for um null o atual é o primeiro da estação caso contrário é o proximo do anterior
                temp->quantidadeCarros--; // diminui a quantidade de carros atual na ET
            }
            else { // caso as condições não sejam cumpridas
                anterior = atual;
                atual = atual->proximoCarro;
            }
        }
        temp = temp->proximaEstacao; // passa para a próxima estação
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
                carros = novoCarro; // adiciona à fila de espera (lista ligada)

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
        carros = carros->proximoCarro; // esse carro passa a ser o proximo carro
    }
    else { // caso contrário
        carro* carroAtual = carros; // cria a lista ligada com o resto dos carros que não vão ser adicionados
        while (carroAtual->proximoCarro != carroNaET) { // vê se são diferentes
            carroAtual = carroAtual->proximoCarro; // percorre todos os carros diferentes
        }
        carroAtual->proximoCarro = carroNaET->proximoCarro; // próximo carro do carro que não vai ser removido é o próximo do que vai ser removido
    }
    delete carroNaET; // deleta o carro que vai ser adicionado na ET
    return carros; // dá o return da lista ligada de carros
}

// método para adicionar um carro na ET
void adicionaCarroET(carro*& carros, estacoes*& estacao) {

    int carroAdicionado = 0; //variavel para contar quantos carros foram adicionados
    carro* carroAAdicionar = carros; //copia da lista ligada de carros

    while (carroAdicionado < 8 && carroAAdicionar != nullptr) { //enquanto o numero de carros adiconados não for 8 nem percorrer a lista ligada de carros toda

        estacoes* estacaoAtual = estacao; // lista ligada temporaria para receber as estações existentes

        while (estacaoAtual != nullptr){ // enquanto houver estações
            if((carroAAdicionar->marca == estacaoAtual->marcaEspecializada) && (estacaoAtual->quantidadeCarros < estacaoAtual->capacidade)){ // se tiver a mesma marca e tiver capacidade
                
                carro* carroET = new carro(); // cria um novo carro
                *carroET = *carroAAdicionar; // igualá ao carro que vai ser adicionado
                carroET->proximoCarro = nullptr; // próximo carro a esse é null

                if (estacaoAtual->primeiroCarro == nullptr) { // se não tiver nenhum carro na primeira posição
                    estacaoAtual->primeiroCarro = carroET; // põe esse carro nessa posição
                }
                else { // caso contrário
                    carro* ultimoCarro = estacaoAtual->primeiroCarro; //lista ligada que recebe os carros da estação
                    while (ultimoCarro->proximoCarro != nullptr) { // enquanto o próximo carro for diferente de null
                        ultimoCarro = ultimoCarro->proximoCarro; // percorre até ao final da lista ligada
                    }
                    ultimoCarro->proximoCarro = carroET; // adicona no final da lista o novo carro
                }
                estacaoAtual->quantidadeCarros++; // aumenta a quantidade de carros na estação
                carroAdicionado++; // incrementa a variável de carros que já foram adicionados
                carros = removeDaListaEspera(carroAAdicionar, carros); // retira da lista ligada de carros (fila de espera) esse carro que foi adicionado à estação
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
        string marcaRandom = escolhePalavraRandomMarcas(marca); // seleciona uma marca random da lista ligada de marcas
        string modeloRandom = escolhePalavraRandomModelos(modelo);// seleciona um modelo random da lista ligada de modelos

        carro* novoCarro = new carro(); // cria um novo carro

        novoCarro->idCarro = numCarrosTotal + 1;
        novoCarro->tempoMax = rand() % 4 + 2;
        novoCarro->dias = 0;
        novoCarro->prioritario = rand() % 100 < 5;
        novoCarro->marca = marcaRandom;
        novoCarro->modelo = modeloRandom;
        novoCarro->proximoCarro = nullptr; // adiciona os valores necessários aos atributos do carro
        
        novoCarro->proximoCarro = carros;
        carros = novoCarro; // adiciona o carro criado à lista ligada da fila de espera

        numCarrosTotal++; // atualiza a variavel numCarrosTotal
    }
}

// método que cria as estações de trabalho
void estacaoTrabalho(estacoes*& estacao, int& numET, int& numEstacoes, int& numeroPalavrasMarcas, marcas*& marca) {

    string nome; // variavel para guardar o nome do mecanico
    getline(cin, nome); // recebe o input do usuario

    for (int i = 0; i < numET; i++) { // ciclo que cria a quantidade de estações baseado na variável que tem o valor random

        string marcaRandom = escolhePalavraRandomMarcas(marca); // retira uma marca random da lista de marcas

        cout << "Dê um nome ao mecânico " << i + 1 << ": \n"; // põe na consola a mensagem para dar um nome
        getline(cin, nome); // recebe o input do usuario

        estacoes* novaET = new estacoes(); // cria uma nova estação

        novaET->idET = numEstacoes + 1; 
        novaET->capacidade = rand() % 4 + 2;
        novaET->quantidadeCarros = 0;
        novaET->faturacao = 0;
        novaET->mecanico = nome;
        novaET->marcaEspecializada = marcaRandom;
        novaET->proximaEstacao = nullptr;
        novaET->primeiroCarro = nullptr; // adiciona aos atributos da estação os valores necessários

        novaET->proximaEstacao = estacao;
        estacao = novaET; // adiciona a estacao à lista ligada

        numEstacoes++; // incrementa o numero de estações
    }
}

// método para fazer output dos carros na fila de espera
void printCars(carro* carros) {

    cout << "\nLista de espera: \n";
    while (carros != nullptr) { // percorre os carros 
        cout << "Carro ID: " << carros->idCarro << " | "
             << "Marca: " << carros->marca << " | "
             << "Modelo: " << carros->modelo << " | "
             << "Prioritário: " << (carros->prioritario ? "Sim" : "Não") << " | "
             << "Tempo de reparação: " << carros->tempoMax << " | "
             << "Dias na oficina: " << carros->dias << "\n"; // faz o output das informações necessárias de cada carro
        cout << "\n";
        carros = carros->proximoCarro; // passa para o próximo carro
    }
}

// método para fazer output das estações
void printETs(estacoes* estacao) {

    while (estacao != nullptr) { // percorre todas as estações
        cout << "\nEstação ";
        cout << "ID: " << estacao->idET << " | "
            << "Mecânico: " << estacao->mecanico << " | "
            << "Capacidade: " << estacao->capacidade << " | "
            << "Carros: " << estacao->quantidadeCarros << " | "
            << "Marca: " << estacao->marcaEspecializada << " | "
            << "Total Faturação: " << estacao->faturacao << "$\n\n"; // faz os outputs necessários, e busca a informação desejada

        for (carro* carroAtual = estacao->primeiroCarro; carroAtual != nullptr; carroAtual = carroAtual->proximoCarro) { // output dos carros na estação

            cout << "Carro ID: " << carroAtual->idCarro << " | "
                << "Marca: " << carroAtual->marca << " | "
                << "Modelo: " << carroAtual->modelo << " | "
                << "Prioritário: " << (carroAtual->prioritario ? "Sim" : "Não") << " | "
                << "Tempo de reparação: " << carroAtual->tempoMax << " | "
                << "Dias na oficina: " << carroAtual->dias << "\n"; // faz os output das informações dos carros
        }
        cout << "-------------------------- \n";

        estacao = estacao->proximaEstacao; // passa para a estação seguinte
    }
}

// método para retirar as marcas que foram adicionadas ao ficheiro das marcas
void limpaMarcas() {

    ifstream marcas("marcas.txt"); // recebe o ficheiro marcas para leitura

    if (marcas.is_open()) {
        ofstream marcasTemp("temp.txt"); // cria um ficheiro temporario de escrita

        if (marcasTemp.is_open()) {
            string linha; // variável para ler as linhas
            int numeroLinha = 1; // onde vai começar a ler
            int comecaLinha = 46; // até onde lê sem eliminar

            while (getline(marcas, linha)) { // vai lendo cada linha do ficheiro marcas
                if (numeroLinha <= comecaLinha) { // se tiver entre as 46 primeiras linhas adiciona ao ficheiro temporario
                    marcasTemp << linha << "\n";
                }
                numeroLinha++; // incrementa o número das linhas
            }

            marcas.close(); // fecha os ficheiros
            marcasTemp.close(); 

            remove("marcas.txt"); // remove o ficheiro marcas 
            rename("temp.txt", "marcas.txt"); // substitui o nome do ficheiro temporario para marcas.txt
        }
    }
}

// método que faz o output inicial a pedir os ficheiros, e pede os inputs para fazer os ciclos ou a gestao
void menu(int& numeroPalavrasMarcas, int& numeroPalavrasModelos, marcas*& marca, modelos*& modelo, carro*& carros, estacoes*& estacao, carrosReparados*& raiz) {

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
            uploadArvores(raiz, "arvores.txt");
            printETs(estacao);
            printCars(carros);
            removeMarcasDuplicadas(marca);

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

            removeCarros(estacao, raiz);

            organizaETs(estacao);

            printETs(estacao);

            organizaListaEspera(carros);

            printCars(carros);

            break;

        case 'g':

            gestao(estacao, numEstacoes, marca, numeroPalavrasMarcas, numCarros, carros, numCarrosTotal, raiz);

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