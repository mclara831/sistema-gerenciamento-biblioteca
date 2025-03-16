#include "cli.h"
#include "../buscas/busca_binaria.c"
#include "../metodos_ordenacao/quick_sort.c"
#include "../metodos_ordenacao/selecao_natural.c"
#include "../metodos_ordenacao/intercalacao_otima2.c"

void menu_principal()
{
    printf("\n==================================================");
    printf("\n                  MENU DE OPCOES                 ");
    printf("\n==================================================");

    printf("\n\nClientes");
    printf("\n--------------------------------------------------");
    printf("\n [1] Inserir um novo cliente");
    printf("\n [2] Atualizar dados de um cliente");
    printf("\n [3] Buscar cliente");
    printf("\n [4] Imprimir base de dados de clientes");

    printf("\n\nLivros");
    printf("\n--------------------------------------------------");
    printf("\n [5] Inserir um novo livro");
    printf("\n [6] Atualizar dados de um livro");
    printf("\n [7] Buscar livro");
    printf("\n [8] Imprimir base de dados de livros");

    printf("\n\nEmprestimos");
    printf("\n--------------------------------------------------");
    printf("\n [9] Registrar um novo emprestimo");
    printf("\n [10] Realizar devolucao");
    printf("\n [11] Renovar emprestimo");
    printf("\n [12] Buscar emprestimo");
    printf("\n [13] Imprimir base de dados de emprestimos");

    printf("\n\nBase de Dados");
    printf("\n--------------------------------------------------");
    printf("\n [14] Gerar base de dados ordenada");
    printf("\n [15] Gerar base de dados desordenada");
    printf("\n [16] Ordenar bases de dados");

    printf("\n\n[0] Sair");
    printf("\n==================================================");
    printf("\nSelecione uma opcao: ");
}

void menu_tipo_busca()
{
    printf("\n\n[1] Busca sequencial\n");
    printf("[2] Busca binaria\n");
    printf("Selecione o tipo de busca: ");
}

/**********************************************************
                 FUNÇÕES DO CLIENTE
***********************************************************/

void registrar_novo_cliente(FILE *arq)
{
    char nome[20], cpf[15];
    fflush(stdin);
    printf("Digite o nome: ");
    scanf("%[^\n]", nome);
    // fgets(nome, 20, stdin);
    fflush(stdin);
    printf("Digite o cpf(XXX.XXX.XXX-XX): ");
    scanf("%[^\n]", cpf);
    // fgets(cpf, 15, stdin);

    while (cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-')
    {
        printf("\n\n[AVISO]: Dados digitados incorretamente! Tente novamente!\n");
        fflush(stdin);
        printf("Digite o cpf(XXX.XXX.XXX-XX): ");
        fgets(cpf, 14, stdin);
    }

    Cliente *c = criar_cliente(tamanho_arquivo_clientes(arq) + 1, nome, cpf);
    fseek(arq, 0, SEEK_END);
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
        printf("[AVISO]: Opcao invalida! Digite novamente!");
        menu_tipo_busca();
        scanf("%d", &opcao);
    }

    if (opcao == 2 && ler_status_base() == 0) {
        printf("\n[ERRO]: A base de dados esta desordenada. Utilize a busca sequencial!\n");
        return;
    }

    (opcao == 1) ? (c = buscar_cliente_sequencial(codigo, arq)) : (c = buscar_cliente_binaria(codigo, arq));

    printf("\nDados do cliente encontrado:\n");
    (c != NULL) ? imprimir_cliente(c) : printf("[AVISO]: Cliente nao econtrado na base de dados!\n");
}

void editar_dados_cliente(FILE *arq) {
    
    int codigo;
    printf("\nDigite o codigo do cliente que voce deseja atualizar: ");
    scanf("%d", &codigo);
    Cliente *c = buscar_cliente_sequencial(codigo, arq);

    if (c == NULL)
    {
        printf("[AVISO]: Cliente nao encontrado!");
        return;
    }
    
    char nome[50], cpf[15];
    fflush(stdin);
    printf("Digite o nome: ");
    scanf("%[^\n]", nome);
    // fgets(nome, 20, stdin);
    fflush(stdin);
    printf("Digite o cpf(XXX.XXX.XXX-XX): ");
    scanf("%[^\n]", cpf);
    // fgets(cpf, 15, stdin);

    while (cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-')
    {
        printf("\n\n[AVISO]: Dados digitados incorretamente! Tente novamente!\n");
        fflush(stdin);
        printf("Digite o cpf(XXX.XXX.XXX-XX): ");
        fgets(cpf, 14, stdin);
    }

    int posicao = posicao_cliente(c, arq);
    Cliente *cliente = criar_cliente(c->id, nome, cpf);
    fseek(arq, tamanho_registro_cliente() * posicao, SEEK_SET);
    salvar_cliente(cliente, arq);

    printf("Cliente atualizado com sucesso!\n");
    imprimir_cliente(cliente);
    free(c);
    fflush(stdin);
}

/**********************************************************
                 FUNÇÕES DO LIVRO
***********************************************************/

void registrar_novo_livro(FILE *arq)
{
    char titulo[50], autor[50], genero[10], disponibilidade;
    int anoPublicacao;

    printf("\nDigite o titulo do livro: ");
    fflush(stdin);
    scanf("%[^\n]", titulo);
    // fgets(titulo, 50, stdin);
    printf("\nDigite o autor do livro: ");
    fflush(stdin);
    scanf("%[^\n]", autor);
    // fgets(autor, 50, stdin);
    printf("\nDigite o genero: ");
    fflush(stdin);
    scanf("%[^\n]", genero);
    // fgets(genero, 10, stdin);
    printf("Digite o ano de publicacao: ");
    scanf("%d", &anoPublicacao);
    printf("Disponivel(s/n): ");
    fflush(stdin);
    scanf("%c", &disponibilidade);
    while (disponibilidade != 's' && disponibilidade != 'n')
    {
        printf("\n\n[AVISO]: Dados digitados incorretamente! Tente novamente!\n");
        printf("Disponivel(s/n): ");
        fflush(stdin);
        scanf("%c", &disponibilidade);
    }

    Livro *l = criar_livro(tamanho_arquivo_livros(arq) + 1, titulo, autor, genero, anoPublicacao, disponibilidade);
    fseek(arq, 0, SEEK_END);
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
        printf("[AVISO]: Opcao invalida! Digite novamente!");
        menu_tipo_busca();
        scanf("%d", &opcao);
    }

    if (opcao == 2 && ler_status_base() == 0) {
        printf("\n[ERRO]: A base de dados esta desordenada. Utilize a busca sequencial!\n");
        return;
    }

    (opcao == 1) ? (l = buscar_livro_sequencial(codigo, arq)) : (l = buscar_livro_binaria(codigo, arq));

    printf("\nDados do livro encontrado:\n");
    (l != NULL) ? imprimir_livro(l) : printf("[AVISO]: Livro nao encontrado na base de dados!\n");
}

void editar_dados_livro(FILE *arq)
{
    int codigo;
    printf("\nDigite o codigo do livro que voce deseja atualizar: ");
    scanf("%d", &codigo);
    Livro *l = buscar_livro_sequencial(codigo, arq);

    if (l == NULL)  
    {
        printf("[AVISO]: Livro nao encontrado!");
        return;
    }
    
    char titulo[50], autor[50], genero[10], disponibilidade;
    int anoPublicacao;

    printf("\nDigite o titulo do livro: ");
    fflush(stdin);
    scanf("%[^\n]", titulo);
    // fgets(titulo, 50, stdin);
    printf("\nDigite o autor do livro: ");
    fflush(stdin);
    scanf("%[^\n]", autor);
    // fgets(autor, 50, stdin);
    printf("\nDigite o genero: ");
    fflush(stdin);
    scanf("%[^\n]", genero);
    // fgets(genero, 10, stdin);
    printf("Digite o ano de publicacao: ");
    scanf("%d", &anoPublicacao);
    printf("Disponivel(s/n): ");
    fflush(stdin);
    scanf("%c", &disponibilidade);
    while (disponibilidade != 's' && disponibilidade != 'n')
    {
        printf("\n\n[AVISO]: Dados digitados incorretamente! Tente novamente!\n");
        printf("Disponivel(s/n): ");
        fflush(stdin);
        scanf("%c", &disponibilidade);
    }

    int posicao = posicao_livro(l, arq);
    Livro *livro = criar_livro(l->id, titulo, autor, genero, anoPublicacao, disponibilidade);
    fseek(arq, tamanho_registro_livro() * posicao, SEEK_SET);
    salvar_livro(livro, arq);

    printf("Livro atualizado com sucesso!\n");
    imprimir_livro(livro);
    free(l);
}

/**********************************************************
                FUNÇÕES DO EMPRÉSTIMO
***********************************************************/

void registrar_novo_emprestimo(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq)
{
    int cod_cliente, cod_livro;
    printf("\nDigite o codigo do livro buscado: ");
    scanf("%d", &cod_livro);
    Livro *l = buscar_livro_sequencial(cod_livro, livros_arq);

    if (l == NULL)
    {
        printf("[AVISO]: Livro nao encontrado.\n");
        return;
    }

    printf("\nDigite o codigo do cliente buscado: ");
    scanf("%d", &cod_cliente);

    Cliente *c = buscar_cliente_sequencial(cod_cliente, clientes_arq);

    if (c == NULL)
    {
        printf("[AVISO]: Cliente nao encontrado.\n");
        return;
    }

    int posicao = posicao_livro(l, livros_arq);

    if (l->disponibilidade == 'n')
    {
        printf("[AVISO]: O livro escolhido nao esta disponivel! Escolha outro livro ou volte depois!\n");
        return;
    }

    struct tm data_emp = criar_data();
    struct tm data_prev = criar_data();
    struct tm *data_prevista = adicionar_dias(&data_prev, 7);

    Livro *novoLivro = criar_livro(l->id, l->titulo, l->autor, l->genero, l->anoPublicacao, 'n');
    fseek(livros_arq, tamanho_registro_livro() * (posicao), SEEK_SET);
    salvar_livro(novoLivro, livros_arq);

    Emprestimo *e = criar_emprestimo(tamanho_arquivo_emprestimos(emp_arq) + 1, l, c, data_emp, *data_prevista, 5.00, 'n', 0.00, 'n');
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
        printf("[AVISO]: Codigo de emprestimo nao encontrado! Tente novamente!\n");
        return;
    }

    if (e->devolvido == 's')
    {
        printf("\n[AVISO]: Emprestimo ja finalizado!\n");
        return;
    }

    Livro *l = buscar_livro_sequencial(e->livro->id, livro_arq);

    int posicao_liv = posicao_livro(l, livro_arq);
    int posicao_emp = posicao_emprestimo(e, emp_arq);

    Livro *novoLivro = criar_livro(l->id, l->titulo, l->autor, l->genero, l->anoPublicacao, 's');
    fseek(livro_arq, tamanho_registro_livro() * (posicao_liv), SEEK_SET);
    salvar_livro(novoLivro, livro_arq);

    struct tm data_dev = criar_data();
    struct tm data_prev = e->data_prevista;
    time_t t1 = mktime(&data_dev);
    time_t t2 = mktime(&data_prev);

    (t1 <= t2) ? (atraso = 'n') : (atraso = 's');

    int dias_em_atraso = calcular_multa(e);
    double multa = 1.50;

    Emprestimo *emp = criar_emprestimo(e->id, e->livro, e->cliente, e->data_emprestimo, e->data_prevista, e->valor, 's', dias_em_atraso * multa, atraso);
    fseek(emp_arq, tamanho_registro_emprestimo() * (posicao_emp), SEEK_SET);
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
        printf("[AVISO]: Codigo de emprestimo nao encontrado! Tente novamente!\n");
        return;
    }

    if (e->devolvido == 's')
    {
        printf("\n[AVISO]: Emprestimo ja finalizado!\n");
        return;
    }

    struct tm *data_prev = adicionar_dias(&e->data_prevista, 7);
    int posicao_emp = posicao_emprestimo(e, emp_arq);

    Emprestimo *emp = criar_emprestimo(e->id, e->livro, e->cliente, e->data_emprestimo, *data_prev, e->valor, e->devolvido, e->multa, e->atraso);
    fseek(emp_arq, tamanho_registro_emprestimo() * (posicao_emp), SEEK_SET);
    salvar_emprestimo(emp, emp_arq);

    printf("Emprestimo %d renovado com sucesso!\n", e->id);
    imprimir_emprestimo(emp);

    liberar_emprestimo(e);
    liberar_emprestimo(emp);
}

void buscar_emprestimo(FILE *arq)
{
    Emprestimo *e;
    int opcao, codigo;
    printf("\nDigite o codigo do emprestimo buscado: ");
    scanf("%d", &codigo);
    menu_tipo_busca();
    scanf("%d", &opcao);

    while (opcao != 1 && opcao != 2)
    {
        printf("[AVISO]: Opcao invalida! Digite novamente!");
        menu_tipo_busca();
        scanf("%d", &opcao);
    }

    if (opcao == 2 && ler_status_base() == 0) {
        printf("\n[ERRO]: A base de dados esta desordenada. Utilize a busca sequencial!\n");
        return;
    }

    (opcao == 1) ? (e = buscar_emprestimo_sequencial(codigo, arq)) : (e = buscar_emprestimo_binaria(codigo, arq));

    printf("\nDados do livro encontrado:\n");
    (e != NULL) ? imprimir_emprestimo(e) : printf("[AVISO]: Emprestimo nao encontrado na base de dados!\n");
}

/**********************************************************
                FUNÇÕES DA BASE DE DADOS
***********************************************************/
void classificao_e_intercalacao(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq, int tam_particao)
{
    // Clientes
    int n_particoes = selecao_natural_clientes(clientes_arq, tam_particao);
    printf("Digite o numero de arquivo que você deseja manipular(deve ser menor que %d): ", n_particoes);
    int num;
    scanf("%d", &num);
    if (num > n_particoes)
    {
        printf("O numero digitado foi maior que o numero de particoes existentes!");
        return;
    }
    intercalacao_otima_clientes2(num, n_particoes, clientes_arq);
    // Livros
    n_particoes = selecao_natural_livros(livros_arq, tam_particao);
    intercalacao_otima_livros2(num, n_particoes, livros_arq);
    //Emprestimos
    n_particoes = selecao_natural_emp(emp_arq, tam_particao);
    intercalacao_otima_emprestimos2(num, n_particoes, emp_arq);
    salvar_status_base(1);
}

void iniciar_ordenacao(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq) {
    printf("\n\n[1] Ordenar pelo QuickSort\n");
    printf("[2] Ordenar pela classificacao e intercalacao\n");
    printf("Selecione o tipo de ordenacao: ");
    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        quick_sort_todas_entidades(clientes_arq, livros_arq, emp_arq);
        break;
    case 2:
        printf("\nDigite o tamanho do reservatorio: ");
        int tam;
        scanf("%d", &tam);
        classificao_e_intercalacao(clientes_arq, livros_arq, emp_arq, tam);
        break;
    default:
        printf("[AVISO]: Opcao invalida!\n");
        break;
    }
    
    printf("[SUCESSO] Ordenação realizada com sucesso!\n");
}