#ifndef SELECAO_NATURAL_C
#define SELECAO_NATURAL_C
#include "../entities/entidades.h"
#include "intercalacao_otima.c"
#include "limits.h"

void salvar_informacoes_logs_sn(char *descricao, int particoes, double tempo) {
    FILE* logs = fopen("logs/logs_classificacao.txt", "a");
    if (logs == NULL)
    {
        printf("Erro ao abrir o ficheiro de logs.\n");
        return;
    }

    fprintf(logs, "*****************************************\n%s: \nNumero de comparacoes: %d\nTempo de execucao: %.2f segundos\n\n", descricao, particoes, tempo);
    fclose(logs);
}

int selecao_natural_clientes(FILE *arq, int tam_particao)
{
    double tempoInicial = clock();

    int qtdParticoes = 0;
    Cliente *registros[tam_particao];
    char nomeParticao[30];

    int reg = 0;
    int tam = tamanho_arquivo_clientes(arq);

    Cliente *anterior = NULL;

    rewind(arq);

    FILE *reservatorio;
    if ((reservatorio = fopen("cli/reservatorio.dat", "wb+")) == NULL)
    {
        printf("Erro criar arquivo de saida\n");
        return -1;
    }

    for (int i = 0; i < tam_particao; i++)
    {
        registros[i] = ler_cliente(arq);
        if (registros[i] == NULL)
            break;
        reg++;
    }

    while (reg < tam)
    {
        sprintf(nomeParticao, "cli/partition%i.dat", qtdParticoes);

        FILE *part;
        if ((part = fopen(nomeParticao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
            return -1;
        }

        while (tamanho_arquivo_clientes(reservatorio) < tam_particao && reg < tam)
        {
            Cliente *menor = NULL;
            int posicao_menor = -1;

            for (int j = 0; j < tam_particao; j++)
            {
                if (registros[j] != NULL)
                {
                    if (menor == NULL || registros[j]->id < menor->id)
                    {
                        menor = registros[j];
                        posicao_menor = j;
                    }
                }
            }

            if (menor == NULL)
            {
                break;
            }
            
            anterior = menor;
            salvar_cliente(menor, part);

            Cliente *novo = ler_cliente(arq);

            if (novo == NULL)
            {
                registros[posicao_menor] = NULL;
            }
            else if (novo->id < anterior->id)
            {
                salvar_cliente(novo, reservatorio);
                registros[posicao_menor] = NULL;
            }
            else
            {
                registros[posicao_menor] = novo;
                reg++;
            }
        }

        printf("\nParticao %d\n", qtdParticoes);
        qtdParticoes++;
        fclose(part);

        if (tamanho_arquivo_clientes(reservatorio) == 0)
            break;

        rewind(reservatorio);
        for (int i = 0; i < tam_particao; i++)
        {
            registros[i] = ler_cliente(reservatorio);
        }

        fclose(reservatorio);
        reservatorio = fopen("cli/reservatorio.dat", "wb+");
    }

    if (is_array_empty(registros, tam_particao) == 0)
    {
        printf("entrou no final\n");
        sprintf(nomeParticao, "cli/partition%i.dat", qtdParticoes);

        FILE *part;
        if ((part = fopen(nomeParticao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        while (is_array_empty(registros, tam_particao) == 0)
        {

            int posicao_menor;
            Cliente *menor = (Cliente *)malloc(sizeof(Cliente));
            menor->id = INT_MAX;

            for (int i = 0; i < tam_particao; i++)
            {
                if (registros[i] != NULL && registros[i]->id < menor->id)
                {
                    menor = registros[i];
                    posicao_menor = i;
                }
            }

            salvar_cliente(menor, part);
            registros[posicao_menor] = NULL;
        }
        qtdParticoes++;
        fclose(part);
    }
    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_sn("Seleção natural - CLIENTE", qtdParticoes, tempoDeExecucao);
    fclose(reservatorio);
    return qtdParticoes;
}

#endif