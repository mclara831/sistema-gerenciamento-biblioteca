#ifndef ENTIDADES_H
#define ENTIDADES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct TCliente
{
    int id;
    char nome[20];
    char cpf[15];
} Cliente;

typedef struct TLivro
{
    int id;
    char titulo[20];
    char autor[20];
    char genero[10];
    int anoPublicacao;
    char disponibilidade;
} Livro;

typedef struct TEmprestimo
{
    int id;
    Livro *livro;
    Cliente *cliente;
    struct tm data_emprestimo;
    struct tm data_prevista;
    double valor;
    char devolvido;
    char atraso;
} Emprestimo;

/**********************************************************
                 FUNÇÕES GERAIS
***********************************************************/
void shuffle(int *vet, int tamanho_maximo);
void criar_bases_ordenadas(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq, int tamanho);
void criar_bases_desordenadas(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq, int tamanho);
struct tm criar_data();
struct tm *adicionar_dias(struct tm *data, int dias);
void imprimir_data(struct tm *data);

/**********************************************************
                 FUNÇÕES DO CLIENTE
***********************************************************/
int tamanho_registro_cliente();
int tamanho_arquivo_clientes(FILE *arq);
Cliente *criar_cliente(int id, char *nome, char *cpf);
void salvar_cliente(Cliente *cliente, FILE *arq);
Cliente *ler_cliente(FILE *arq);
void criar_base_ordenada_clientes(FILE *arq, int tamanho);
void criar_base_desordenada_clientes(FILE *arq, int tamanho);
void imprimir_cliente(Cliente *cliente);
void imprimir_base_clientes(FILE *arq);

/**********************************************************
                 FUNÇÕES DO LIVRO
***********************************************************/
int tamanho_registro_livro();
int tamanho_arquivo_livros(FILE *arq);
Livro *criar_livro(int id, char *titulo, char *autor, char *genero, int anoPublicacao, char disponibilidade);
void salvar_livro(Livro *livro, FILE *arq);
Livro *ler_livro(FILE *arq);
void imprimir_livro(Livro *livro);
void criar_base_ordenada_livros(FILE *arq, int tamanho);
void criar_base_desordenada_livros(FILE *arq, int tamanho);
void imprimir_base_livros(FILE *arq);

/**********************************************************
                 FUNÇÕES DO EMPRÉSTIMO
***********************************************************/
int tamanho_registro_emprestimo();
void liberar_emprestimo(Emprestimo *emp);
int tamanho_arquivo_emprestimos(FILE *arq);
Emprestimo *criar_emprestimo(int id, Livro *livro, Cliente *cliente, struct tm data_emprestimo, struct tm data_prevista, double valor, char devolvido, char atraso);
void salvar_emprestimo(Emprestimo *emprestimo, FILE *arq);
Emprestimo *ler_emprestimo(FILE *arq);
void imprimir_emprestimo(Emprestimo *emprestimo);
void criar_base_ordenada_emprestimos(FILE *arq, int tamanho);
void criar_base_desordenada_emprestimos(FILE *arq, int tamanho);
void imprimir_base_emprestimos(FILE *arq);

#endif