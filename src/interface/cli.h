#ifndef CLI_H
#define CLI_H

#include <stdio.h>

void menu_principal();
void menu_tipo_busca();

/* CLI - CLIENTE */
void registrar_novo_cliente(FILE *arq);
void buscar_cliente(FILE *arq);
void editar_dados_cliente(FILE *arq);

/* CLI - LIVRO */
void registrar_novo_livro(FILE *arq);
void buscar_livro(FILE *arq);
void editar_dados_livro(FILE *arq);

/* CLI - EMPRÉSTIMO */
void registrar_novo_emprestimo(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq);
void realizar_devolucao(FILE *emp_arq, FILE *livro_arq);
void renovar_emprestimo(FILE *emp_arq);

/* CLI - FUNÇÕES DA BASE DE DADOS */
void classificao_e_intercalacao(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq, int tam_particao);
void iniciar_ordenacao(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq);

/* CLI - HASH */
void menu_opcoes_hash();
void criar_novo_cli_hash(FILE *tabela_hash, FILE *clientes_arq);
void buscar_cliente_hash(FILE *tabela_hash, FILE *clientes_arq);

#endif