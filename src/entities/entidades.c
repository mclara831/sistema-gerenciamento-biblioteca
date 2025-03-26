#ifndef ENTIDADES_C
#define ENTIDADES_C
#include "entidades.h"

/**********************************************************
                 FUNÇÕES GERAIS
***********************************************************/
void shuffle(int *vet, int tamanho_maximo)
{
    srand(time(NULL));
    for (int i = tamanho_maximo - 1; i > 0; i--)
    {
        int j = rand() % (i);
        int tmp = vet[j];
        vet[j] = vet[i];
        vet[i] = tmp;
    }
}

void salvar_status_base(int ordenada)
{
    FILE *status_arq = fopen("status.txt", "w");
    if (status_arq == NULL)
    {
        printf("Erro ao abrir o arquivo de status!\n");
        return;
    }
    fprintf(status_arq, "%d", ordenada);
    fclose(status_arq);
}

int ler_status_base()
{
    FILE *status_arq = fopen("status.txt", "r");
    if (status_arq == NULL)
    {
        return 0; // Se o arquivo não existir, consideramos desordenada
    }
    int ordenada;
    fscanf(status_arq, "%d", &ordenada);
    fclose(status_arq);
    return ordenada;
}

void criar_bases_ordenadas(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq, int tamanho)
{
    salvar_status_base(1);
    criar_base_ordenada_clientes(clientes_arq, tamanho);
    criar_base_ordenada_livros(livros_arq, tamanho);
    criar_base_ordenada_emprestimos(emp_arq, tamanho);
}

void criar_bases_desordenadas(FILE *clientes_arq, FILE *livros_arq, FILE *emp_arq, int tamanho)
{
    salvar_status_base(0);
    criar_base_desordenada_clientes(clientes_arq, tamanho);
    criar_base_desordenada_livros(livros_arq, tamanho);
    criar_base_desordenada_emprestimos(emp_arq, tamanho);
}

struct tm criar_data()
{
    time_t t = time(NULL);
    struct tm data_emprestimo = *localtime(&t);
    return data_emprestimo;
}

struct tm *adicionar_dias(struct tm *data, int dias)
{
    data->tm_mday += dias;
    mktime(data);
    return data;
}

void imprimir_data(struct tm *data)
{
    printf("%02d/%02d/%04d", data->tm_mday, data->tm_mon + 1, data->tm_year + 1900);
}

/**********************************************************
                 FUNÇÕES DO CLIENTE
***********************************************************/
int tamanho_registro_cliente()
{
    return sizeof(int) + sizeof(char) * 50 + sizeof(char) * 15 + sizeof(int) + sizeof(int);
}

int tamanho_arquivo_clientes(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tamanho = trunc(ftell(arq) / tamanho_registro_cliente());
    return tamanho;
}

Cliente *criar_cliente(int id, char *nome, char *cpf, int prox, int ocupado)
{
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
    if (cliente)
        memset(cliente, 0, sizeof(Cliente));

    cliente->id = id;
    strcpy(cliente->nome, nome);
    strcpy(cliente->cpf, cpf);
    cliente->prox = prox;
    cliente->ocupado = ocupado;
    return cliente;
}

void salvar_cliente(Cliente *cliente, FILE *arq)
{
    fwrite(&cliente->id, sizeof(int), 1, arq);
    fwrite(cliente->nome, sizeof(char), 50, arq);
    fwrite(cliente->cpf, sizeof(char), 15, arq);
    fwrite(&cliente->prox, sizeof(int), 1, arq);
    fwrite(&cliente->ocupado, sizeof(int), 1, arq);
}

Cliente *ler_cliente(FILE *arq)
{
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
    if (0 >= fread(&cliente->id, sizeof(int), 1, arq))
    {
        free(cliente);
        return NULL;
    }
    fread(cliente->nome, sizeof(char), 50, arq);
    fread(cliente->cpf, sizeof(char), 15, arq);
    fread(&cliente->prox, sizeof(int), 1, arq);
    fread(&cliente->ocupado, sizeof(int), 1, arq);
    return cliente;
}

void criar_base_ordenada_clientes(FILE *arq, int tamanho)
{
    fclose(arq);
    if ((arq = fopen("arq_clientes.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de clientes!");
        exit(1);
    }
    int vetor[tamanho];
    Cliente *cliente;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    for (int i = 0; i < tamanho; i++)
    {
        cliente = criar_cliente(vetor[i], "ANONIMO", "000.000.000-00", -1, 1);
        salvar_cliente(cliente, arq);
    }
    free(cliente);
}

void criar_base_desordenada_clientes(FILE *arq, int tamanho)
{
    fclose(arq);
    if ((arq = fopen("arq_clientes.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de clientes!");
        exit(1);
    }
    int vetor[tamanho];
    Cliente *cliente;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    shuffle(vetor, tamanho);

    for (int i = 0; i < tamanho; i++)
    {
        cliente = criar_cliente(vetor[i], "ANONIMO", "000.000.000-00", -1, 1);
        salvar_cliente(cliente, arq);
    }
    free(cliente);
}

void imprimir_cliente(Cliente *cliente)
{
    printf("**********************************************");
    printf("\nCliente de codigo ");
    printf("%d", cliente->id);
    printf("\nNome: ");
    printf("%s", cliente->nome);
    printf("\nCPF: ");
    printf("%s", cliente->cpf);
    printf("\nProx: ");
    printf("%d", cliente->prox);
    printf("\n**********************************************\n");
}

void imprimir_base_clientes(FILE *arq)
{
    rewind(arq);
    Cliente *cliente = ler_cliente(arq);

    while (cliente != NULL)
    {
        imprimir_cliente(cliente);
        cliente = ler_cliente(arq);
    }
    free(cliente);
}

int posicao_cliente(Cliente *cliente, FILE *arq)
{
    rewind(arq);
    Cliente *c;
    int posicao = 0;
    while ((c = ler_cliente(arq)) != NULL)
    {
        if (cliente->id == c->id)
        {
            break;
        }
        posicao++;
    }

    return posicao;
}

/**********************************************************
                 FUNÇÕES DO LIVRO
***********************************************************/
int tamanho_registro_livro()
{
    return sizeof(int) + sizeof(char) * 50 + sizeof(char) * 50 + sizeof(char) * 10 + sizeof(int) + sizeof(char);
}

int tamanho_arquivo_livros(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tamanho = trunc(ftell(arq) / tamanho_registro_livro());
    return tamanho;
}

Livro *criar_livro(int id, char *titulo, char *autor, char *genero, int anoPublicacao, char disponibilidade)
{
    Livro *livro = (Livro *)malloc(sizeof(Livro));
    if (livro)
    {
        memset(livro, 0, sizeof(Livro));
    }
    livro->id = id;
    strcpy(livro->titulo, titulo);
    strcpy(livro->autor, autor);
    strcpy(livro->genero, genero);
    livro->anoPublicacao = anoPublicacao;
    livro->disponibilidade = disponibilidade;
    return livro;
}

void salvar_livro(Livro *livro, FILE *arq)
{
    fwrite(&livro->id, sizeof(int), 1, arq);
    fwrite(livro->titulo, sizeof(char), 50, arq);
    fwrite(livro->autor, sizeof(char), 50, arq);
    fwrite(livro->genero, sizeof(char), 10, arq);
    fwrite(&livro->anoPublicacao, sizeof(int), 1, arq);
    fwrite(&livro->disponibilidade, sizeof(char), 1, arq);
}

Livro *ler_livro(FILE *arq)
{
    Livro *livro = (Livro *)malloc(sizeof(Livro));
    if (0 >= fread(&livro->id, sizeof(int), 1, arq))
    {
        free(livro);
        return NULL;
    }
    fread(livro->titulo, sizeof(char), 50, arq);
    fread(livro->autor, sizeof(char), 50, arq);
    fread(livro->genero, sizeof(char), 10, arq);
    fread(&livro->anoPublicacao, sizeof(int), 1, arq);
    fread(&livro->disponibilidade, sizeof(char), 1, arq);
    return livro;
}

void criar_base_ordenada_livros(FILE *arq, int tamanho)
{
    fclose(arq);
    if ((arq = fopen("arq_livros.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de livros!");
        exit(1);
    }
    int vetor[tamanho];
    Livro *livro;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    for (int i = 0; i < tamanho; i++)
    {
        livro = criar_livro(vetor[i], "Linguagem C", "Desconhecido", "Cientifico", 1996, 's');
        strcpy(livro->genero, "Cientifico");
        salvar_livro(livro, arq);
    }
    free(livro);
}

void criar_base_desordenada_livros(FILE *arq, int tamanho)
{
    fclose(arq);
    if ((arq = fopen("arq_livros.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de livros!");
        exit(1);
    }
    int vetor[tamanho];
    Livro *livro;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    shuffle(vetor, tamanho);

    for (int i = 0; i < tamanho; i++)
    {
        livro = criar_livro(vetor[i], "Linguagem C", "Desconhecido", "Cientifico", 1996, 's');
        strcpy(livro->genero, "Cientifico");
        salvar_livro(livro, arq);
    }
    free(livro);
}

void imprimir_livro(Livro *livro)
{
    printf("**********************************************");
    printf("\nLivro de codigo ");
    printf("%d", livro->id);
    printf("\nTitulo: ");
    printf("%s", livro->titulo);
    printf("\nAutor: ");
    printf("%s", livro->autor);
    printf("\nGenero: ");
    printf("%s", livro->genero);
    printf("\nAno de publicacao: ");
    printf("%d", livro->anoPublicacao);
    printf("\nDisponibilidade: ");
    printf("%c", livro->disponibilidade);
    printf("\n**********************************************\n");
}

void imprimir_base_livros(FILE *arq)
{
    rewind(arq);
    Livro *livro;
    while ((livro = ler_livro(arq)) != NULL)
    {
        imprimir_livro(livro);
    }
    free(livro);
}

int posicao_livro(Livro *livro, FILE *arq)
{
    rewind(arq);
    Livro *l;
    int posicao = 0;
    while ((l = ler_livro(arq)) != NULL)
    {
        if (livro->id == l->id)
        {
            break;
        }
        posicao++;
    }

    return posicao;
}

/**********************************************************
                 FUNÇÕES DO EMPRÉSTIMO
***********************************************************/
int tamanho_registro_emprestimo()
{
    return sizeof(int) + sizeof(Livro) + sizeof(Cliente) + sizeof(struct tm) + sizeof(struct tm) + sizeof(double) + sizeof(char) + +sizeof(double) + sizeof(char);
}

int tamanho_arquivo_emprestimos(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tamanho = trunc(ftell(arq) / tamanho_registro_emprestimo());
    return tamanho;
}

void liberar_emprestimo(Emprestimo *emp)
{
    free(emp->livro);
    free(emp->cliente);
    free(emp);
}

Emprestimo *criar_emprestimo(int id, Livro *livro, Cliente *cliente, struct tm data_emprestimo, struct tm data_prevista, double valor, char devolvido, double multa, char atraso)
{
    Emprestimo *emprestimo = (Emprestimo *)malloc(sizeof(Emprestimo));
    emprestimo->id = id;
    emprestimo->livro = livro;
    emprestimo->cliente = cliente;
    emprestimo->data_emprestimo = data_emprestimo;
    emprestimo->data_prevista = data_prevista;
    emprestimo->valor = valor;
    emprestimo->devolvido = devolvido;
    emprestimo->multa = multa;
    emprestimo->atraso = atraso;
    return emprestimo;
}

void salvar_emprestimo(Emprestimo *emprestimo, FILE *arq)
{
    fwrite(&emprestimo->id, sizeof(int), 1, arq);
    fwrite(emprestimo->livro, sizeof(Livro), 1, arq);
    fwrite(emprestimo->cliente, sizeof(Cliente), 1, arq);
    fwrite(&emprestimo->data_emprestimo, sizeof(struct tm), 1, arq);
    fwrite(&emprestimo->data_prevista, sizeof(struct tm), 1, arq);
    fwrite(&emprestimo->valor, sizeof(double), 1, arq);
    fwrite(&emprestimo->devolvido, sizeof(char), sizeof(char), arq);
    fwrite(&emprestimo->multa, sizeof(double), 1, arq);
    fwrite(&emprestimo->atraso, sizeof(char), sizeof(char), arq);
}

Emprestimo *ler_emprestimo(FILE *arq)
{
    Emprestimo *emprestimo = (Emprestimo *)malloc(sizeof(Emprestimo));
    if (0 >= fread(&emprestimo->id, sizeof(int), 1, arq))
    {
        liberar_emprestimo(emprestimo);
        return NULL;
    }
    emprestimo->livro = malloc(sizeof(Livro));
    fread(emprestimo->livro, sizeof(Livro), 1, arq);
    emprestimo->cliente = malloc(sizeof(Cliente));
    fread(emprestimo->cliente, sizeof(Cliente), 1, arq);
    fread(&emprestimo->data_emprestimo, sizeof(struct tm), 1, arq);
    fread(&emprestimo->data_prevista, sizeof(struct tm), 1, arq);
    fread(&emprestimo->valor, sizeof(double), 1, arq);
    fread(&emprestimo->devolvido, sizeof(char), sizeof(char), arq);
    fread(&emprestimo->multa, sizeof(double), 1, arq);
    fread(&emprestimo->atraso, sizeof(char), sizeof(char), arq);
    return emprestimo;
}

void criar_base_ordenada_emprestimos(FILE *arq, int tamanho)
{
    fclose(arq);
    if ((arq = fopen("arq_emprestimos.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de livros!");
        exit(1);
    }
    int vetor[tamanho];
    Emprestimo *emprestimo;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    struct tm data_emp = criar_data();
    struct tm data_prev = criar_data();
    struct tm *data_prevista = adicionar_dias(&data_prev, 7);

    for (int i = 0; i < tamanho; i++)
    {
        Cliente *cliente = criar_cliente(1, "ANONIMO", "000.000.000-00", -1, 1);
        Livro *livro = criar_livro(1, "Linguagem C", "Desconhecido", "Cientifico", 1996, 's');
        emprestimo = criar_emprestimo(vetor[i], livro, cliente, data_emp, *data_prevista, 5.00, 'n', 0.00, 'n');
        salvar_emprestimo(emprestimo, arq);
        free(livro);
        free(cliente);
        liberar_emprestimo(emprestimo);
    }
}

void criar_base_desordenada_emprestimos(FILE *arq, int tamanho)
{
    fclose(arq);
    if ((arq = fopen("arq_emprestimos.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de livros!");
        exit(1);
    }
    int vetor[tamanho];
    Emprestimo *emprestimo;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    shuffle(vetor, tamanho);

    struct tm data_emp = criar_data();
    struct tm data_prev = criar_data();
    struct tm *data_prevista = adicionar_dias(&data_prev, 7);

    for (int i = 0; i < tamanho; i++)
    {
        Cliente *cliente = criar_cliente(1, "ANONIMO", "000.000.000-00", -1, 1);
        Livro *livro = criar_livro(1, "Linguagem C", "Desconhecido", "Cientifico", 1996, 's');
        emprestimo = criar_emprestimo(vetor[i], livro, cliente, data_emp, *data_prevista, 5.00, 'n', 0.00, 'n');
        salvar_emprestimo(emprestimo, arq);
        free(livro);
        free(cliente);
        liberar_emprestimo(emprestimo);
    }
}

void imprimir_emprestimo(Emprestimo *emprestimo)
{
    printf("**********************************************");
    printf("\nCodigo do Emprestimo: ");
    printf("%d", emprestimo->id);
    printf("\nTitulo: ");
    printf("%s", emprestimo->livro->titulo);
    printf("\nCliente: ");
    printf("%s", emprestimo->cliente->nome);
    printf("\nData de emprestimo: ");
    imprimir_data(&emprestimo->data_emprestimo);
    printf("\nData de prevista de devolucao: ");
    imprimir_data(&emprestimo->data_prevista);
    printf("\nValor do emprestimo: ");
    printf("R$%.2f", emprestimo->valor);
    printf("\nDevolvido: ");
    printf("%c", emprestimo->devolvido);
    if (emprestimo->multa > 0)
    {
        printf("\nValor da multa: ");
        printf("R$%.2f", emprestimo->multa);
    }
    printf("\nDevolvido com atraso: ");
    printf("%c", emprestimo->atraso);
    printf("\n**********************************************\n");
}

void imprimir_base_emprestimos(FILE *arq)
{
    rewind(arq);
    Emprestimo *emprestimo;
    while ((emprestimo = ler_emprestimo(arq)) != NULL)
    {
        imprimir_emprestimo(emprestimo);
        liberar_emprestimo(emprestimo);
    }
}

int posicao_emprestimo(Emprestimo *emprestimo, FILE *arq)
{
    rewind(arq);
    Emprestimo *e;
    int posicao = 0;
    while ((e = ler_emprestimo(arq)) != NULL)
    {
        if (emprestimo->id == e->id)
        {
            break;
        }
        posicao++;
    }
    return posicao;
}

double calcular_multa(Emprestimo *emprestimo)
{
    struct tm data_prev = emprestimo->data_prevista;
    struct tm data_atual = criar_data();
    // struct tm *data_atual2 = adicionar_dias(&data_atual, 1);

    time_t t_prevista = mktime(&data_prev);
    time_t t_atual = mktime(&data_atual);

    double diferenca = difftime(t_atual, t_prevista) / 86400;
    printf("%d", (int)diferenca);

    if (diferenca > 0)
    {
        return (int)diferenca;
    }
    return 0;
}

#endif