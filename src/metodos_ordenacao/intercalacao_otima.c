#ifndef INTERCALACAO_OTIMA_C
#define INTERCALACAO_OTIMA_C
#include "../entities/entidades.h"

void salvar_informacoes_logs_io(char *descricao, int contador, double tempo)
{
    FILE *logs = fopen("logs/logs_intercalcacao.txt", "a");
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs.\n");
        return;
    }

    fprintf(logs, "*****************************************\n%s: \nNumero de comparacoes: %d\nTempo de execucao: %.2f segundos\n\n", descricao, contador, tempo);
    fclose(logs);
}

void copiar_dados(FILE *origem, FILE *destino, int tipo)
{

    rewind(origem);
    rewind(destino);

    if (tipo == 1)
    {
        Cliente *dado;
        while((dado = ler_cliente(origem)) != NULL)
        {
            salvar_cliente(dado, destino);
            free(dado);
        }
    }
    else
    {
        Livro *dado;
        while((dado = ler_livro(origem)) != NULL)
        {
            salvar_livro(dado, destino);
        }
    }
}

int is_array_empty(Cliente **clientes, int fim)
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

    while (is_array_empty(clientes, total_particoes) == 0)
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
        printf("Reg: %d\n", reg);
        reg++;
    }

    // printf("\nParticao %d\n", total_particoes);
    // imprimir_base_clientes(arq_final);
    printf("Acabou de imprimir tudo\n");
    fclose(arq_final);

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
}

#endif