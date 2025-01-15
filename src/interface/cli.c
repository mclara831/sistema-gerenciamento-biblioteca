#include "cli.h"
#include "../buscas/busca_binaria.c"

void menu_principal()
{
    printf("\n******************* MENU DE OPCOES *******************");
    printf("\n\n Clientes\n");
    printf("\t1. Registrar um novo cliente\n");
    printf("\t2. Buscar cliente\n");
    printf("\t3. Imprimir base de dados de clientes");

    printf("\nLivros\n");
    printf("\t4. Registrar um novo livro\n");
    printf("\t5. Buscar livro\n");
    printf("\t6. Imprimir base de dados de livros");

    printf("\nEmprestimo\n");
    printf("\t7. Registrar um novo emprestimo\n");
    printf("\t8. Realizar devolucao\n");
    printf("\t9. Renovar emprestimo\n");
    printf("\t10. Imprimir base de dados de emprestimos\n");

    printf("\nBase de dados\n");
    printf("\t11. Gerar base de dados ordenada\n");
    printf("\t12. Gerar base de dados desordenada\n");
    printf("\n\t0. Sair\n");

    printf("\nSelecione uma opcao: ");
}

void menu_tipo_busca()
{
    printf("\n\n1. Busca sequencial\n");
    printf("2. Busca binaria\n");
    printf("Selecione o tipo de busca: ");
}

void registrar_novo_cliente(FILE *arq)
{
    char nome[20], cpf[15];
    fflush(stdin);
    printf("Digite o nome: ");
    fgets(nome, 20, stdin);
    fflush(stdin);
    printf("Digite o cpf(XXX.XXX.XXX-XX): ");
    fgets(cpf, 15, stdin);

    while (cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-')
    {
        printf("\n\nDados digitados incorretamente! Tente novamente!\n");
        fflush(stdin);
        printf("Digite o cpf(XXX.XXX.XXX-XX): ");
        fgets(cpf, 14, stdin);
    }

    Cliente *c = criar_cliente(tamanho_arquivo_clientes(arq) + 1, nome, cpf);
    salvar_cliente(c, arq);

    printf("Cliente cadastrado com sucesso!\n");
    imprimir_cliente(c);
    free(c);
    fflush(stdin);
}

void buscar_cliente(FILE *arq)
{
    Cliente *c;
    int opcao, codigo;
    printf("\nDigite o codigo do cliente buscado: ");
    scanf("%d", &codigo);
    menu_tipo_busca();
    scanf("%d", &opcao);

    while (opcao != 1 && opcao != 2)
    {
        printf("Opcao invalida! Digite novamente!");
        menu_tipo_busca();
        scanf("%d", &opcao);
    }

    (opcao == 1) ? (c = buscar_cliente_sequencial(codigo, arq)) : (c = buscar_cliente_binaria(codigo, arq));

    printf("\nDados do cliente encontrado:\n");
    (c != NULL) ? imprimir_cliente(c) : printf("Cliente nao econtrado na base de dados!\n");
}

void registrar_novo_livro(FILE *arq)
{
    char titulo[20], autor[20], genero[10], disponibilidade;
    int anoPublicacao;

    printf("\nDigite o titulo do livro: ");
    fflush(stdin);
    fgets(titulo, 20, stdin);
    printf("\nDigite o autor do livro: ");
    fflush(stdin);
    fgets(autor, 20, stdin);
    printf("\nDigite o genero: ");
    fflush(stdin);
    fgets(genero, 10, stdin);
    printf("Digite o ano de publicacao: ");
    scanf("%d", &anoPublicacao);
    printf("Disponivel(s/n): ");
    fflush(stdin);
    scanf("%c", &disponibilidade);
    while (disponibilidade != 's' && disponibilidade != 'n')
    {
        printf("\n\nDados digitados incorretamente! Tente novamente!\n");
        printf("Disponivel(s/n): ");
        fflush(stdin);
        scanf("%c", &disponibilidade);
    }

    Livro *l = criar_livro(tamanho_arquivo_livros(arq) + 1, titulo, autor, genero, anoPublicacao, disponibilidade);
    salvar_livro(l, arq);

    printf("Livro cadastrado com sucesso!\n");
    imprimir_livro(l);
    free(l);
}

void buscar_livro(FILE *arq)
{
    Livro *l;
    int opcao, codigo;
    printf("\nDigite o codigo do livro buscado: ");
    scanf("%d", &codigo);
    menu_tipo_busca();
    scanf("%d", &opcao);

    while (opcao != 1 && opcao != 2)
    {
        printf("Opcao invalida! Digite novamente!");
        menu_tipo_busca();
        scanf("%d", &opcao);
    }

    (opcao == 1) ? (l = buscar_livro_sequencial(codigo, arq)) : (l = buscar_livro_binaria(codigo, arq));

    printf("\nDados do livro encontrado:\n");
    (l != NULL) ? imprimir_livro(l) : printf("Livro nao econtrado na base de dados!\n");
}

void registrar_novo_emprestimo(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq)
{
    int cod_cliente, cod_livro;
    printf("\nDigite o codigo do livro buscado: ");
    scanf("%d", &cod_livro);
    printf("\nDigite o codigo do cliente buscado: ");
    scanf("%d", &cod_cliente);

    Cliente *c = buscar_cliente_sequencial(cod_cliente, clientes_arq);
    Livro *l = buscar_livro_sequencial(cod_livro, livros_arq);

    if (l == NULL || c == NULL)
    {
        printf("Livro e/ou cliente nao encontrado.\n");
        return;
    }

    if (l->disponibilidade == 'n')
    {
        printf("O livro escolhido nao esta disponivel! Escolha outro livro ou volte depois!\n");
        return;
    }

    struct tm data_emp = criar_data();
    struct tm data_prev = criar_data();
    struct tm *data_prevista = adicionar_dias(&data_prev, 7);

    Livro *novoLivro = criar_livro(l->id, l->titulo, l->autor, l->genero, l->anoPublicacao, 'n');
    fseek(livros_arq, tamanho_registro_livro() * (l->id - 1), SEEK_SET);
    salvar_livro(novoLivro, livros_arq);

    Emprestimo *e = criar_emprestimo(tamanho_arquivo_emprestimos(emp_arq) + 1, l, c, data_emp, *data_prevista, 5.00, 'n', 'n');
    imprimir_emprestimo(e);
    fseek(emp_arq, 0, SEEK_END);
    salvar_emprestimo(e, emp_arq);

    free(e->livro);
    free(e->cliente);
    free(e);
    free(l);
    free(novoLivro);
}

void realizar_devolucao(FILE *emp_arq, FILE *livro_arq)
{
    int codigo;
    char atraso;

    printf("Digite o codigo do emprestimo: ");
    scanf("%d", &codigo);
    Emprestimo *e = buscar_emprestimo_sequencial(codigo, emp_arq);

    if (e == NULL)
    {
        printf("Codigo de emprestimo nao encontrado! Tente novamente!\n");
        return;
    }

    Livro *l = buscar_livro_sequencial(e->livro->id, livro_arq);

    Livro *novoLivro = criar_livro(l->id, l->titulo, l->autor, l->genero, l->anoPublicacao, 's');
    fseek(livro_arq, tamanho_registro_livro() * (l->id - 1), SEEK_SET);
    salvar_livro(novoLivro, livro_arq);

    struct tm data_dev = criar_data();
    struct tm data_prev = e->data_prevista;
    time_t t1 = mktime(&data_dev);
    time_t t2 = mktime(&data_prev);

    (t1 <= t2) ? (atraso = 'n') : (atraso = 's');

    Emprestimo *emp = criar_emprestimo(e->id, e->livro, e->cliente, e->data_emprestimo, e->data_prevista, e->valor, 's', atraso);
    fseek(emp_arq, tamanho_registro_emprestimo() * (e->id - 1), SEEK_SET);
    salvar_emprestimo(emp, emp_arq);

    imprimir_emprestimo(emp);
    printf("Livro devolvido!\n");

    liberar_emprestimo(e);
    liberar_emprestimo(emp);
}

void renovar_emprestimo(FILE *emp_arq)
{
    int codigo;
    printf("Digite o codigo do emprestimo: ");
    scanf("%d", &codigo);
    Emprestimo *e = buscar_emprestimo_sequencial(codigo, emp_arq);

    if (e == NULL)
    {
        printf("Codigo de emprestimo nao encontrado! Tente novamente!\n");
        return;
    }

    struct tm *data_prev = adicionar_dias(&e->data_prevista, 7);

    Emprestimo *emp = criar_emprestimo(e->id, e->livro, e->cliente, e->data_emprestimo, *data_prev, e->valor, e->devolvido, e->atraso);
    fseek(emp_arq, tamanho_registro_emprestimo() * (e->id - 1), SEEK_SET);
    salvar_emprestimo(emp, emp_arq);

    printf("Emprestimo %d renovado com sucesso!\n", e->id);
    imprimir_emprestimo(emp);

    liberar_emprestimo(e);
    liberar_emprestimo(emp);
}
