#ifndef INTERCALACAO_OTIMA2_C
#define INTERCALACAO_OTIMA2_C
#include "../entities/entidades.h"
#include "intercalacao_otima.c"

void salvar_informacoes_logs_io2(char *descricao, int contador, double tempo)
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
typedef struct estrutura
{
    char particao[40];
} TStruct;

typedef struct celula
{
    TStruct particao;
    struct celula *prox;
} TCelula;

typedef struct fila
{
    TCelula *frente;
    TCelula *tras;
    int tamanho;
} TFila;

void FFVazia(TFila *Fila)
{
    Fila->frente = (TCelula *)malloc(sizeof(TCelula));
    Fila->tras = Fila->frente;
    Fila->frente->prox = NULL;
    Fila->tamanho = 0;
}

int Vazia(TFila fila)
{
    return (fila.frente == fila.tras);
}

void Enfileirar(TStruct particao, TFila *Fila)
{
    Fila->tras->prox = (TCelula *)malloc(sizeof(TCelula));
    Fila->tras = Fila->tras->prox;
    Fila->tras->particao = particao;
    Fila->tras->prox = NULL;
    Fila->tamanho++;
}

void Desenfileirar(TFila *fila, TStruct *particao)
{
    TCelula *aux;
    if (!Vazia(*fila))
    {
        aux = fila->frente->prox;
        fila->frente->prox = aux->prox;
        *particao = aux->particao;
        strcpy(particao->particao, aux->particao.particao);
        free(aux);
        if (fila->frente->prox == NULL)
        {
            fila->tras = fila->frente;
        }
        fila->tamanho--;
    }
}

void intercalacao_otima_clientes2(int num, int total_particoes, FILE *arq)
{
    char nomeParticao[40];
    double tempoInicial = clock();
    int n_part_inicial = total_particoes;

    // Inicializa a fila de partições
    TFila fila;
    FFVazia(&fila);
    FILE **estruturas = (FILE **)malloc(num * sizeof(FILE *));
    Cliente *clientes[num]; 

    // Abre as partições iniciais e as enfileira
    for (int i = 0; i < total_particoes; i++)
    {
        sprintf(nomeParticao, "cli/partition%i.dat", i);
        TStruct particao;
        strcpy(particao.particao, nomeParticao);
        Enfileirar(particao, &fila);
    }

    FILE *particao;
    while (fila.tamanho > 1)
    {
        for (int i = 0; i < num; i++)
        {
            if (fila.tamanho == 0)
            {
                clientes[i] = NULL;
                continue;
            }
            
            TStruct aux;
            Desenfileirar(&fila, &aux);
            particao = fopen(aux.particao, "rb+");
            if (particao == NULL)
            {
                printf("Erro ao abrir arquivo de entrada: %s\n", nomeParticao);
                continue;
            }
            estruturas[i] = particao;
            clientes[i] = ler_cliente(particao);
        }

        sprintf(nomeParticao, "cli/partition%i.dat", total_particoes);
        FILE *novaParticao = fopen(nomeParticao, "wb+");
        if (novaParticao == NULL)
        {
            printf("Erro ao criar arquivo de saída\n");
            return;
        }

        while (is_array_empty_cli(clientes, num) == 0)
        {
            Cliente *menor = NULL;
            int menor_indice;
            for (int j = 0; j < num; j++)
            {
                // contador++;
                if (clientes[j] == NULL)
                    continue;

                if (menor == NULL || clientes[j]->id < menor->id)
                {
                    menor_indice = j;
                    menor = clientes[j];
                }
            }

            if (menor != NULL)
            {   
                salvar_cliente(menor, novaParticao);
                clientes[menor_indice] = ler_cliente(estruturas[menor_indice]);
            }
        }

        fflush(novaParticao);
        fclose(novaParticao);

        TStruct nova;
        strcpy(nova.particao, nomeParticao);
        
        Enfileirar(nova, &fila);
        total_particoes++;
    }

    sprintf(nomeParticao, "cli/partition%i.dat", (total_particoes - 1));
    // Reabrir arquivo antes de comparar tamanho
    particao = fopen(nomeParticao, "rb+");
    if (particao == NULL)
    {
        printf("Erro ao abrir arquivo para verificação\n");
        exit(EXIT_FAILURE);
    }

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_io("Intercalacao Otima - CLIENTE", (total_particoes - n_part_inicial), tempoDeExecucao);

    rewind(particao);
    copiar_dados(particao, arq, 1);
    fclose(particao);
}


void intercalacao_otima_livros2(int num, int total_particoes, FILE *arq)
{

    char nomeParticao[40];
    double tempoInicial = clock();
    int n_part_inicial = total_particoes;

    // Inicializa a fila de partições
    TFila fila;
    FFVazia(&fila);
    FILE **estruturas = (FILE **)malloc(num * sizeof(FILE *));//Abre  os arquivos
    Livro *livros[num]; 

    // Abre as partições iniciais e as enfileira
    for (int i = 0; i < total_particoes; i++)
    {
        sprintf(nomeParticao, "livro/partition%i.dat", i);
        TStruct particao;
        strcpy(particao.particao, nomeParticao);
        Enfileirar(particao, &fila);
    }

    FILE *particao;
    while (fila.tamanho > 1)
    {
        //Desenfileira os primeiros n-1 registros, os coloca nos vetores
        for (int i = 0; i < num; i++)
        {
            if (fila.tamanho == 0)
            {
                livros[i] = NULL;
                continue;
            }
            
            TStruct aux;
            Desenfileirar(&fila, &aux);
            particao = fopen(aux.particao, "rb+");
            if (particao == NULL)
            {
                printf("Erro ao abrir arquivo de entrada: %s\n", nomeParticao);
                continue;
            }
            estruturas[i] = particao;
            livros[i] = ler_livro(particao);
        }

        sprintf(nomeParticao, "livro/partition%i.dat", total_particoes);
        FILE *novaParticao = fopen(nomeParticao, "wb+");
        if (novaParticao == NULL)
        {
            printf("Erro ao criar arquivo de saída\n");
            return;
        }

        while (is_array_empty_liv(livros, num) == 0)
        {
            Livro *menor = NULL;
            int menor_indice;
            for (int j = 0; j < num; j++)
            {
                // contador++;
                if (livros[j] == NULL)
                    continue;

                if (menor == NULL || livros[j]->id < menor->id)
                {
                    menor_indice = j;
                    menor = livros[j];
                }
            }

            if (menor != NULL)
            {   
                salvar_livro(menor, novaParticao);
                livros[menor_indice] = ler_livro(estruturas[menor_indice]);
            }
        }

        fflush(novaParticao);
        fclose(novaParticao);

        TStruct nova;
        strcpy(nova.particao, nomeParticao);
        
        Enfileirar(nova, &fila);
        total_particoes++;
    }

    sprintf(nomeParticao, "livro/partition%i.dat", (total_particoes - 1));
    // Reabrir arquivo antes de comparar tamanho
    particao = fopen(nomeParticao, "rb+");
    if (particao == NULL)
    {
        printf("Erro ao abrir arquivo para verificação\n");
        exit(EXIT_FAILURE);
    }

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_io("Intercalacao Otima - LIVRO", (total_particoes - n_part_inicial), tempoDeExecucao);

    rewind(particao);
    copiar_dados(particao, arq, 2);
    fclose(particao);
}

void intercalacao_otima_emprestimos2(int num, int total_particoes, FILE *arq)
{
    char nomeParticao[40];
    double tempoInicial = clock();
    int n_part_inicial = total_particoes;

    // Inicializa a fila de partições
    TFila fila;
    FFVazia(&fila);
    FILE **estruturas = (FILE **)malloc(num * sizeof(FILE *));
    Emprestimo *emps[num]; 

    // Abre as partições iniciais e as enfileira
    for (int i = 0; i < total_particoes; i++)
    {
        sprintf(nomeParticao, "emp/partition%i.dat", i);
        TStruct particao;
        strcpy(particao.particao, nomeParticao);
        Enfileirar(particao, &fila);
    }

    FILE *particao;
    while (fila.tamanho > 1)
    {
        for (int i = 0; i < num; i++)
        {
            if (fila.tamanho == 0)
            {
                emps[i] = NULL;
                continue;
            }
            
            TStruct aux;
            Desenfileirar(&fila, &aux);
            particao = fopen(aux.particao, "rb+");
            if (particao == NULL)
            {
                printf("Erro ao abrir arquivo de entrada: %s\n", nomeParticao);
                continue;
            }
            estruturas[i] = particao;
            emps[i] = ler_emprestimo(particao);
        }

        sprintf(nomeParticao, "emp/partition%i.dat", total_particoes);
        FILE *novaParticao = fopen(nomeParticao, "wb+");
        if (novaParticao == NULL)
        {
            printf("Erro ao criar arquivo de saída\n");
            return;
        }

        while (is_array_empty_emp(emps, num) == 0)
        {
            Emprestimo *menor = NULL;
            int menor_indice;
            for (int j = 0; j < num; j++)
            {
                // contador++;
                if (emps[j] == NULL)
                    continue;

                if (menor == NULL || emps[j]->id < menor->id)
                {
                    menor_indice = j;
                    menor = emps[j];
                }
            }

            if (menor != NULL)
            {   
                salvar_emprestimo(menor, novaParticao);
                emps[menor_indice] = ler_emprestimo(estruturas[menor_indice]);
            }
        }

        fflush(novaParticao);
        fclose(novaParticao);

        TStruct nova;
        strcpy(nova.particao, nomeParticao);
        
        Enfileirar(nova, &fila);
        total_particoes++;
    }

    sprintf(nomeParticao, "emp/partition%i.dat", (total_particoes - 1));
    // Reabrir arquivo antes de comparar tamanho
    particao = fopen(nomeParticao, "rb+");
    if (particao == NULL)
    {
        printf("Erro ao abrir arquivo para verificação\n");
        exit(EXIT_FAILURE);
    }

    double tempoFinal = clock();
    double tempoDeExecucao = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC;

    salvar_informacoes_logs_io("Intercalacao Otima - EMPRESTIMO", (total_particoes - n_part_inicial), tempoDeExecucao);

    rewind(particao);
    copiar_dados(particao, arq, 3);
    fclose(particao);
}
#endif