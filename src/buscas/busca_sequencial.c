#include "../entities/entidades.c"

FILE *logs;

void salvar_informacoes_logs(char *descricao, int numero_comparacoes, double tempo) {
    logs = fopen("logs.txt", "a");
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs.\n");
        return;
    }

    fprintf(logs, "*****************************************\n%s: \nNumero de comparacoes: %d\n Tempo de execucao: %.2f segundos\n\n",descricao, numero_comparacoes, tempo);
    fclose(logs);
}

Cliente *buscar_cliente_sequencial(int chave, FILE *arq)
{
    Cliente *cliente;

    rewind(arq);
    int contador = 0, achou = 0;
    double inicio = clock(), fim = 0;

        while ((cliente = ler_cliente(arq)) != NULL)
        {
            contador++;
            if (cliente->id == chave)
            {
                fim = clock();
                achou = 1;
                break;
            }
        }
    

    double tempo = (fim - inicio) / CLOCKS_PER_SEC;
    salvar_informacoes_logs("Busca sequencial - CLIENTE", contador, tempo);

    if (achou == 1)
        return cliente;
    else
        return NULL;

    free(cliente);
}

Livro *buscar_livro_sequencial(int chave, FILE *arq)
{
    Livro *livro;
    rewind(arq);
    int contador = 0, achou = 0;
    double inicio = clock(), fim = 0;

    
        while ((livro = ler_livro(arq)) != NULL)
        {
            contador++;
            if (livro->id == chave)
            {
                fim = clock();
                achou = 1;
                break;
            }
        }
    

    double tempo = (fim - inicio) / CLOCKS_PER_SEC;
    salvar_informacoes_logs("Busca sequencial - LIVRO", contador, tempo);

    if (achou == 1) {
        return livro;
    } else {
        free(livro);    
        return NULL;
    }
}

Emprestimo *buscar_emprestimo_sequencial(int chave, FILE *arq)
{
    Emprestimo *emprestimo;
    rewind(arq);
    int contador = 0, achou = 0;
    double inicio = clock(), fim = 0;

    
        while ((emprestimo = ler_emprestimo(arq)) != NULL)
        {
            contador++;
            if (emprestimo->id == chave)
            {
                fim = clock();
                achou = 1;
                break;
            }
        }
    

    double tempo = (fim - inicio) / CLOCKS_PER_SEC;
    salvar_informacoes_logs("Busca sequencial - EMPRESTIMO", contador, tempo);

    if (achou == 1) {
        return emprestimo;
    } else {
        free(emprestimo);    
        return NULL;
    }
}