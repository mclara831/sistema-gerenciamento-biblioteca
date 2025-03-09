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

    fprintf(logs, "*****************************************\n%s: \nNumero de particoes criadas: %d\nTempo de execucao: %.2f segundos\n\n", descricao, particoes, tempo);
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

    if (is_array_empty_cli(registros, tam_particao) == 0)
    {
        printf("entrou no final\n");
        sprintf(nomeParticao, "cli/partition%i.dat", qtdParticoes);

        FILE *part;
        if ((part = fopen(nomeParticao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        while (is_array_empty_cli(registros, tam_particao) == 0)
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

int selecao_natural_livros(FILE *arq, int tam_particao)
{
    double tempoInicial = clock();

    int qtdParticoes = 0;
    Livro *registros[tam_particao];
    char nomeParticao[30];

    int reg = 0;
    int tam = tamanho_arquivo_livros(arq);

    Livro *anterior = NULL;

    rewind(arq);

    FILE *reservatorio;
    if ((reservatorio = fopen("livro/reservatorio.dat", "wb+")) == NULL)
    {
        printf("Erro criar arquivo de saida\n");
        return -1;
    }

    for (int i = 0; i < tam_particao; i++)
    {
        registros[i] = ler_livro(arq);
        if (registros[i] == NULL)
            break;
        reg++;
    }

    while (reg < tam)
    {
        sprintf(nomeParticao, "livro/partition%i.dat", qtdParticoes);

        FILE *part;
        if ((part = fopen(nomeParticao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
            return -1;
        }

        while (tamanho_arquivo_livros(reservatorio) < tam_particao && reg < tam)
        {
            Livro *menor = NULL;
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
            salvar_livro(menor, part);

            Livro *novo = ler_livro(arq);

            if (novo == NULL)
            {
                registros[posicao_menor] = NULL;
            }
            else if (novo->id < anterior->id)
            {
                salvar_livro(novo, reservatorio);
                registros[posicao_menor] = NULL;
            }
            else
            {
                registros[posicao_menor] = novo;
                reg++;
            }
        }

        qtdParticoes++;
        fclose(part);

        if (tamanho_arquivo_livros(reservatorio) == 0)
            break;

        rewind(reservatorio);
        for (int i = 0; i < tam_particao; i++)
        {
            registros[i] = ler_livro(reservatorio);
        }

        fclose(reservatorio);
        reservatorio = fopen("livro/reservatorio.dat", "wb+");
    }

    if (is_array_empty_liv(registros, tam_particao) == 0)
    {
        sprintf(nomeParticao, "livro/partition%i.dat", qtdParticoes);

        FILE *part;
        if ((part = fopen(nomeParticao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        while (is_array_empty_liv(registros, tam_particao) == 0)
        {

            int posicao_menor;
            Livro *menor = (Livro *)malloc(sizeof(Livro));
            menor->id = INT_MAX;

            for (int i = 0; i < tam_particao; i++)
            {
                if (registros[i] != NULL && registros[i]->id < menor->id)
                {
                    menor = registros[i];
                    posicao_menor = i;
                }
            }
            salvar_livro(menor, part);
            registros[posicao_menor] = NULL;
        }
        qtdParticoes++;
        fclose(part);
    }

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_sn("Seleção natural - LIVRO", qtdParticoes, tempoDeExecucao);
    fclose(reservatorio);
    return qtdParticoes;
}

int selecao_natural_emp(FILE *arq, int tam_particao)
{
    double tempoInicial = clock();

    int qtdParticoes = 0;
    Emprestimo *registros[tam_particao];
    char nomeParticao[30];

    int reg = 0;
    int tam = tamanho_arquivo_emprestimos(arq);

    Emprestimo *anterior = NULL;

    rewind(arq);

    FILE *reservatorio;
    if ((reservatorio = fopen("emp/reservatorio.dat", "wb+")) == NULL)
    {
        printf("Erro criar arquivo de saida\n");
        return -1;
    }

    for (int i = 0; i < tam_particao; i++)
    {
        registros[i] = ler_emprestimo(arq);
        if (registros[i] == NULL)
            break;
        reg++;
    }

    while (reg < tam)
    {
        sprintf(nomeParticao, "emp/partition%i.dat", qtdParticoes);

        FILE *part;
        if ((part = fopen(nomeParticao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
            return -1;
        }

        while (tamanho_arquivo_emprestimos(reservatorio) < tam_particao && reg < tam)
        {
            Emprestimo *menor = NULL;
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
            salvar_emprestimo(menor, part);

            Emprestimo *novo = ler_emprestimo(arq);

            if (novo == NULL)
            {
                registros[posicao_menor] = NULL;
            }
            else if (novo->id < anterior->id)
            {
                salvar_emprestimo(novo, reservatorio);
                registros[posicao_menor] = NULL;
            }
            else
            {
                registros[posicao_menor] = novo;
                reg++;
            }
        }

        qtdParticoes++;
        fclose(part);

        if (tamanho_arquivo_emprestimos(reservatorio) == 0)
            break;

        rewind(reservatorio);
        for (int i = 0; i < tam_particao; i++)
        {
            registros[i] = ler_emprestimo(reservatorio);
        }

        fclose(reservatorio);
        reservatorio = fopen("emp/reservatorio.dat", "wb+");
    }

    if (is_array_empty_emp(registros, tam_particao) == 0)
    {
        sprintf(nomeParticao, "emp/partition%i.dat", qtdParticoes);

        FILE *part;
        if ((part = fopen(nomeParticao, "wb+")) == NULL)
        {
            printf("Erro criar arquivo de saida\n");
        }
        while (is_array_empty_emp(registros, tam_particao) == 0)
        {

            int posicao_menor;
            Emprestimo *menor = (Emprestimo *)malloc(sizeof(Emprestimo));
            menor->id = INT_MAX;

            for (int i = 0; i < tam_particao; i++)
            {
                if (registros[i] != NULL && registros[i]->id < menor->id)
                {
                    menor = registros[i];
                    posicao_menor = i;
                }
            }
            salvar_emprestimo(menor, part);
            registros[posicao_menor] = NULL;
        }
        qtdParticoes++;
        fclose(part);
    }

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_sn("Seleção natural - EMPRESTIMO", qtdParticoes, tempoDeExecucao);
    fclose(reservatorio);
    return qtdParticoes;
}

#endif