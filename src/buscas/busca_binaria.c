#include "busca_sequencial.c"

Cliente *buscar_cliente_binaria(int chave, FILE *arq)
{
    int inicio = 0, contador = 0, bandeira = 0;
    int fim = tamanho_arquivo_clientes(arq) - 1;
    Cliente *cliente = NULL;

    clock_t initial_time = clock();

    while (inicio <= fim)
    {
        int meio = inicio + (fim - inicio) / 2;
        fseek(arq, meio * tamanho_registro_cliente(), SEEK_SET);

        cliente = ler_cliente(arq);

        if (cliente->id == chave)
        {
            bandeira = 1;
            break;
        }
        else if (cliente->id < chave)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
        contador++;
    }

    clock_t final_time = clock();
    double total_time = (double)(final_time - initial_time) / CLOCKS_PER_SEC;

    salvar_informacoes_logs("Busca binaria - CLIENTE", contador, total_time);

    if (bandeira == 1)
    {
        return cliente;
    }
    else
    {
        free(cliente);
        return NULL;
    }
}

Livro *buscar_livro_binaria(int chave, FILE *arq)
{
    int inicio = 0, contador = 0, bandeira = 0;
    int fim = tamanho_arquivo_livros(arq) - 1;
    Livro *livro = NULL;

    clock_t initial_time = clock();

    while (inicio <= fim)
    {
        int meio = inicio + (fim - inicio) / 2;
        fseek(arq, meio * tamanho_registro_livro(), SEEK_SET);

        livro = ler_livro(arq);

        if (livro->id == chave)
        {
            bandeira = 1;
            break;
        }
        else if (livro->id < chave)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
        contador++;
    }

    clock_t final_time = clock();
    double total_time = (double)(final_time - initial_time) / CLOCKS_PER_SEC;

    salvar_informacoes_logs("Busca binaria - LIVRO", contador, total_time);

    if (bandeira == 1)
    {
        return livro;
    }
    else
    {
        free(livro);
        return NULL;
    }
}

Emprestimo *buscar_emprestimo_binaria(int chave, FILE *arq)
{
    int inicio = 0, contador = 0, bandeira = 0;
    int fim = tamanho_arquivo_emprestimos(arq) - 1;
    Emprestimo *emprestimo = NULL;

    clock_t initial_time = clock();

    while (inicio <= fim)
    {
        int meio = inicio + (fim - inicio) / 2;
        fseek(arq, meio * tamanho_registro_emprestimo(), SEEK_SET);

        emprestimo = ler_emprestimo(arq);

        if (emprestimo->id == chave)
        {
            bandeira = 1;
            break;
        }
        else if (emprestimo->id < chave)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }
        contador++;
    }

    clock_t final_time = clock();
    double total_time = (double)(final_time - initial_time) / CLOCKS_PER_SEC;

    salvar_informacoes_logs("Busca binaria - EMPRESTIMO", contador, total_time);

    if (bandeira == 1)
    {
        return emprestimo;
    }
    else
    {
        free(emprestimo);
        return NULL;
    }
}