#ifndef QUICK_SORT_C
#define QUICK_SORT_C
#include "../entities/entidades.h"

void salvar_informacoes_logs_ordenacao(char *descricao, int numero_comparacoes, int numero_trocas, double tempo) {
    FILE* logs = fopen("logs/logs_ordenacao.txt", "a");
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs.\n");
        return;
    }

    fprintf(logs, "*****************************************\n%s: \nNumero de comparacoes: %d\nNumero de trocas: %d\nTempo de execucao: %.2f segundos\n\n",descricao, numero_comparacoes, numero_trocas, tempo);
    fclose(logs);
}

int particao_cliente(FILE *arq, int inicio, int final, int* n_comparacoes, int* n_trocas)
{
    Cliente *aux = NULL, *cliente_j;

    fseek(arq, tamanho_registro_cliente() * final, SEEK_SET);
    Cliente *x = ler_cliente(arq);

    int i = inicio - 1;
    for (int j = inicio; j < final; j++)
    {
        int contagemExtra = 1;
        fseek(arq, tamanho_registro_cliente() * j, SEEK_SET);
        cliente_j = ler_cliente(arq);
        if (cliente_j->id <= x->id)
        {
            i++;
            fseek(arq, tamanho_registro_cliente() * i, SEEK_SET);
            aux = ler_cliente(arq);
            fseek(arq, tamanho_registro_cliente() * i, SEEK_SET);
            salvar_cliente(cliente_j, arq);
            fseek(arq, tamanho_registro_cliente() * j, SEEK_SET);
            salvar_cliente(aux, arq);

            contagemExtra = 0;
            (*n_comparacoes) += 1;
            (*n_trocas) += 1;
        } 
        if (contagemExtra == 1)
        {
            (*n_comparacoes) += 1;
        }  
    }
    fseek(arq, tamanho_registro_cliente() * (i + 1), SEEK_SET);
    aux = ler_cliente(arq);
    fseek(arq, tamanho_registro_cliente() * (i + 1), SEEK_SET);
    salvar_cliente(x, arq);
    fseek(arq, tamanho_registro_cliente() * final, SEEK_SET);
    salvar_cliente(aux, arq);
    (*n_trocas) += 1;
    return i + 1;
}

int particao_livro(FILE *arq, int inicio, int final, int* n_comparacoes, int* n_trocas)
{
    Livro *aux = NULL, *livro_j;

    fseek(arq, tamanho_registro_livro() * final, SEEK_SET);
    Livro *x = ler_livro(arq);

    int i = inicio - 1;
    for (int j = inicio; j < final; j++)
    {
        int contagemExtra = 1;
        fseek(arq, tamanho_registro_livro() * j, SEEK_SET);
        livro_j = ler_livro(arq);
        if (livro_j->id <= x->id)
        {
            i++;
            fseek(arq, tamanho_registro_livro() * i, SEEK_SET);
            aux = ler_livro(arq);
            fseek(arq, tamanho_registro_livro() * i, SEEK_SET);
            salvar_livro(livro_j, arq);
            fseek(arq, tamanho_registro_livro() * j, SEEK_SET);
            salvar_livro(aux, arq);

            contagemExtra = 0;
            (*n_comparacoes) += 1;
            (*n_trocas) += 1;
        } 
        if (contagemExtra == 1)
        {
            (*n_comparacoes) += 1;
        }  
    }
    fseek(arq, tamanho_registro_livro() * (i + 1), SEEK_SET);
    aux = ler_livro(arq);
    fseek(arq, tamanho_registro_livro() * (i + 1), SEEK_SET);
    salvar_livro(x, arq);
    fseek(arq, tamanho_registro_livro() * final, SEEK_SET);
    salvar_livro(aux, arq);
    (*n_trocas) += 1;
    return i + 1;
}

int particao_emprestimo(FILE *arq, int inicio, int final, int* n_comparacoes, int* n_trocas)
{
    Emprestimo *aux = NULL, *emprestimo_j;

    fseek(arq, tamanho_registro_emprestimo() * final, SEEK_SET);
    Emprestimo *x = ler_emprestimo(arq);

    int i = inicio - 1;
    for (int j = inicio; j < final; j++)
    {
        int contagemExtra = 1;
        fseek(arq, tamanho_registro_emprestimo() * j, SEEK_SET);
        emprestimo_j = ler_emprestimo(arq);
        if (emprestimo_j->id <= x->id)
        {
            i++;
            fseek(arq, tamanho_registro_emprestimo() * i, SEEK_SET);
            aux = ler_emprestimo(arq);
            fseek(arq, tamanho_registro_emprestimo() * i, SEEK_SET);
            salvar_emprestimo(emprestimo_j, arq);
            fseek(arq, tamanho_registro_emprestimo() * j, SEEK_SET);
            salvar_emprestimo(aux, arq);

            //Incrementa informações para salvar no arquivo de LOGS de ordenação
            contagemExtra = 0;
            (*n_comparacoes) += 1;
            (*n_trocas) += 1;
        } 
        if (contagemExtra == 1)
        {
            (*n_comparacoes) += 1;
        }   
    }

    fseek(arq, tamanho_registro_emprestimo() * (i + 1), SEEK_SET);
    aux = ler_emprestimo(arq);
    fseek(arq, tamanho_registro_emprestimo() * (i + 1), SEEK_SET);
    salvar_emprestimo(x, arq);
    fseek(arq, tamanho_registro_emprestimo() * final, SEEK_SET);
    salvar_emprestimo(aux, arq);
    (*n_trocas) += 1;
    return i + 1;
}

void quick_sort(FILE *arq, int inicio, int final, int tipo, double tempoInicial, int* n_comparacoes, int* n_trocas)  
{
    if (inicio < final)
    {
        int meio;
        if (tipo == 1)
        {
            meio = particao_cliente(arq, inicio, final, n_comparacoes, n_trocas);
        }
        else if (tipo == 2)
        {
            meio = particao_livro(arq, inicio, final, n_comparacoes, n_trocas);
        }
        else
        {
            meio = particao_emprestimo(arq, inicio, final, n_comparacoes, n_trocas);
        }
        quick_sort(arq, inicio, meio - 1, tipo, tempoInicial, n_comparacoes, n_trocas);
        quick_sort(arq, meio + 1, final, tipo, tempoInicial, n_comparacoes, n_trocas);
    }
}

void quick_sort_com_logs(FILE *arq, int inicio, int final, int tipo) {

    double tempoInicial = clock();
    int n_comparacoes = 0, n_trocas = 0;

    printf("Iniciando quick....\n");
    quick_sort(arq, inicio, final, tipo, tempoInicial, &n_comparacoes, &n_trocas);

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    if (tipo == 1)
    {
        salvar_informacoes_logs_ordenacao("Quick Sort - CLIENTE", n_comparacoes, n_trocas, tempoDeExecucao);
    }
    else if (tipo == 2)
    {
        salvar_informacoes_logs_ordenacao("Quick Sort - LIVRO", n_comparacoes, n_trocas, tempoDeExecucao);
    }
    else
    {
        salvar_informacoes_logs_ordenacao("Quick Sort - EMPRESTIMO", n_comparacoes, n_trocas, tempoDeExecucao);
    }

}

#endif
