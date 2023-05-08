#pragma once
#include <string>
#include <stdlib.h>

using namespace std;

struct carro
{
    int idCarro;
    int tempoMax;
    int dias;
    bool prioritario;
    string marca;
    string modelo;
    carro* proximoCarro;
};

struct estacoes
{
    int idET;
    int capacidade;
    int quantidadeCarros;
    int faturacao;
    string mecanico;
    string marcaEspecializada;
    estacoes* proximaEstacao;
    carro* primeiroCarro;
};

struct arvoreReparados
{
    string marca;
    string modelo;
    arvoreReparados* esquerda;
    arvoreReparados* direita;
};