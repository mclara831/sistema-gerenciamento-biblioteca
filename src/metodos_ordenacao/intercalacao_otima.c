#ifndef INTERCALACAO_OTIMA_C
#define INTERCALACAO_OTIMA_C
#include "../entities/entidades.h"

void salvar_informacoes_logs_io(char *descricao, int part, double tempo)
{
    FILE *logs = fopen("logs/logs_intercalcacao.txt", "a");
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs.\n");
        return;
    }

    fprintf(logs, "*****************************************\n%s: \nNumero de particoes criadas: %d\nTempo de execucao: %.2f segundos\n\n", descricao, part, tempo);
    fclose(logs);
}

void copiar_dados(FILE *origem, FILE *destino, int tipo)
{

    rewind(origem);
    rewind(destino);

    if (tipo == 1)
    {
        Cliente *dado;
        while ((dado = ler_cliente(origem)) != NULL)
        {
            salvar_cliente(dado, destino);
            free(dado);
        }
    }
    else if (tipo == 2)
    {
        Livro *dado;
        while ((dado = ler_livro(origem)) != NULL)
        {
            salvar_livro(dado, destino);
        }
    }
    else
    {
        Emprestimo *dado;
        while ((dado = ler_emprestimo(origem)) != NULL)
        {
            salvar_emprestimo(dado, destino);
        }
    }
}

int is_array_empty_cli(Cliente **clientes, int fim)
{
    for (int i = 0; i < fim; i++)
    {
        if (clientes[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}

int is_array_empty_liv(Livro **livros, int fim)
{
    for (int i = 0; i < fim; i++)
    {
        if (livros[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}

int is_array_empty_emp(Emprestimo **emps, int fim)
{
    for (int i = 0; i < fim; i++)
    {
        if (emps[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}

void intercalacao_otima_clientes(int total_particoes, FILE *arq)
{
    double tempoInicial = clock();

    int i = 0;
    int reg = 0;
    int contador = 0;
    char nomeParticao[40];

    int menorIndice;
    Cliente *menor;

    Cliente **clientes = (Cliente **)malloc(total_particoes * sizeof(Cliente *));
    FILE **particoes = (FILE **)malloc(total_particoes * sizeof(FILE *));

    while (i < total_particoes)
    {
        sprintf(nomeParticao, "cli/partition%i.dat", i);
        if ((particoes[i] = fopen(nomeParticao, "rb")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        clientes[i] = ler_cliente(particoes[i]);
        i++;
    }

    FILE *arq_final;
    if ((arq_final = fopen("cli/arq_final.dat", "wb+")) == NULL)
    {
        printf("Erro criar arquivo de saida\n");
    }

    while (is_array_empty_cli(clientes, total_particoes) == 0)
    {
        menor = NULL;
        for (int j = 0; j < total_particoes; j++)
        {
            contador++;
            if (clientes[j] == NULL)
                continue;

            if (menor == NULL || clientes[j]->id < menor->id)
            {
                menorIndice = j;
                menor = clientes[j];
            }
        }

        salvar_cliente(menor, arq_final);
        clientes[menorIndice] = ler_cliente(particoes[menorIndice]);
        reg++;
    }
    
    for (i = 0; i < total_particoes; i++)
    {
        fclose(particoes[i]);
    }
    free(clientes);
    free(particoes);

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_io("Intercalacao Otima - CLIENTE", contador, tempoDeExecucao);
    copiar_dados(arq_final, arq, 1);
    fclose(arq_final);
}

void intercalacao_otima_livros(int total_particoes, FILE *arq)
{
    double tempoInicial = clock();

    int i = 0;
    int reg = 0;
    int contador = 0;
    char nomeParticao[40];

    int menorIndice;
    Livro *menor;

    Livro **livros = (Livro **)malloc(total_particoes * sizeof(Livro *));
    FILE **particoes = (FILE **)malloc(total_particoes * sizeof(FILE *));

    while (i < total_particoes)
    {
        sprintf(nomeParticao, "livro/partition%i.dat", i);
        if ((particoes[i] = fopen(nomeParticao, "rb")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        livros[i] = ler_livro(particoes[i]);
        i++;
    }

    FILE *arq_final;
    if ((arq_final = fopen("livro/arq_final.dat", "wb+")) == NULL)
    {
        printf("Erro criar arquivo de saida\n");
    }

    while (is_array_empty_liv(livros, total_particoes) == 0)
    {
        menor = NULL;
        for (int j = 0; j < total_particoes; j++)
        {
            contador++;
            if (livros[j] == NULL)
                continue;

            if (menor == NULL || livros[j]->id < menor->id)
            {
                menorIndice = j;
                menor = livros[j];
            }
        }

        salvar_livro(menor, arq_final);
        livros[menorIndice] = ler_livro(particoes[menorIndice]);
        reg++;
    }

    for (i = 0; i < total_particoes; i++)
    {
        fclose(particoes[i]);
    }
    free(livros);
    free(particoes);

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_io("Intercalacao Otima - LIVRO", contador, tempoDeExecucao);
    copiar_dados(arq_final, arq, 2);
    fclose(arq_final);
}

void intercalacao_otima_emprestimos(int total_particoes, FILE *arq)
{
    double tempoInicial = clock();

    int i = 0;
    int reg = 0;
    int contador = 0;
    char nomeParticao[40];

    int menorIndice;
    Emprestimo *menor;

    Emprestimo **emprestimos = (Emprestimo **)malloc(total_particoes * sizeof(Emprestimo *));
    FILE **particoes = (FILE **)malloc(total_particoes * sizeof(FILE *));

    while (i < total_particoes)
    {
        sprintf(nomeParticao, "emp/partition%i.dat", i);
        if ((particoes[i] = fopen(nomeParticao, "rb")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        emprestimos[i] = ler_emprestimo(particoes[i]);
        i++;
    }

    FILE *arq_final;
    if ((arq_final = fopen("emp/arq_final.dat", "wb+")) == NULL)
    {
        printf("Erro criar arquivo de saida\n");
    }

    while (is_array_empty_emp(emprestimos, total_particoes) == 0)
    {
        menor = NULL;
        for (int j = 0; j < total_particoes; j++)
        {
            contador++;
            if (emprestimos[j] == NULL)
                continue;

            if (menor == NULL || emprestimos[j]->id < menor->id)
            {
                menorIndice = j;
                menor = emprestimos[j];
            }
        }

        salvar_emprestimo(menor, arq_final);
        emprestimos[menorIndice] = ler_emprestimo(particoes[menorIndice]);
        reg++;
    }

    for (i = 0; i < total_particoes; i++)
    {
        fclose(particoes[i]);
    }
    free(emprestimos);
    free(particoes);

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_io("Intercalacao Otima - EMPRESTIMO", contador, tempoDeExecucao);
    copiar_dados(arq_final, arq, 3);
    fclose(arq_final);
}

#endif