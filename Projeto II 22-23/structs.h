#pragma once
#include <string>
#include <stdlib.h>

using namespace std;

struct modelos 
{
    string modelo;
    modelos* proximoModelo;
};

struct marcas 
{
    string marca;
    marcas* proximaMarca;
};

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

struct carrosReparados 
{
    int idET;
    carro* carros;
    carrosReparados* esquerda;
    carrosReparados* direita;
};