#ifndef HASH_C
#define HASH_C

#include "../entities/entidades.c"

#define TAMANHO_TABELA 100

typedef struct estr
{
    int codigo;
} TStruct1;

typedef struct cel
{
    TStruct1 id;
    struct cel *prox;
} TCel;

typedef struct
{
    TCel *frente;
    TCel *tras;
    int tamanho;
} TFila1;

void FFVazia1(TFila1 *Fila)
{
    Fila->frente = (TCel *)malloc(sizeof(TCel));
    Fila->tras = Fila->frente;
    Fila->frente->prox = NULL;
    Fila->tamanho = 0;
}

int Vazia1(TFila1 Fila)
{
    return (Fila.frente == Fila.tras);
}

void Enfileirar1(TStruct1 id, TFila1 *Fila)
{
    Fila->tras->prox = (TCel *)malloc(sizeof(TCel));
    Fila->tras = Fila->tras->prox;
    Fila->tras->id = id;
    Fila->tras->prox = NULL;
    Fila->tamanho++;
}

void Desenfileirar1(TFila1 *fila, TStruct1 *id)
{
    TCel *aux;
    if (!Vazia1(*fila))
    {
        aux = fila->frente->prox;
        fila->frente->prox = aux->prox;
        *id = aux->id;
        id->codigo = aux->id.codigo;
        free(aux);
        if (fila->frente->prox == NULL)
        {
            fila->tras = fila->frente;
        }
        fila->tamanho--;
    }
}

TFila1 Fila;

TFila1 inicializa_fila()
{
    TFila1 fila;
    FFVazia1(&fila);
    return fila;
}

typedef struct TPosicao
{
    int posicao;
} Posicao;

int tamanho_registro_hash()
{
    return sizeof(int);
}

int tamanho_arquivo_hash(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tamanho = trunc(ftell(arq) / tamanho_registro_hash());
    return tamanho;
}

Posicao *criar_posicao(int posicao)
{
    Posicao *pos = (Posicao *)malloc(sizeof(Posicao));
    if (pos)
        memset(pos, 0, sizeof(Posicao));
    pos->posicao = posicao;
    return pos;
}

Posicao *ler_posicao(FILE *arq)
{
    Posicao *pos = (Posicao *)malloc(sizeof(Posicao));
    fread(&pos->posicao, sizeof(int), 1, arq);
    return pos;
}

void salvar_posicao(Posicao *pos, FILE *arq)
{
    fwrite(&pos->posicao, sizeof(int), 1, arq);
}

//Função hash com base no x mod 100
int funcao_hash(Cliente *cliente)
{
    return cliente->id % TAMANHO_TABELA;
}

//Cria a tabela hash vazia
FILE *criar_tabela_hash()
{
    FILE *tabela_hash = fopen("tabela_hash.dat", "w+b");
    if (tabela_hash == NULL)
    {
        printf("Erro ao criar arquivo\n");
        return NULL;
    }
    Posicao *pos;
    while (tamanho_arquivo_hash(tabela_hash) < TAMANHO_TABELA)
    {
        pos = criar_posicao(-1);
        salvar_posicao(pos, tabela_hash);
    }
    return tabela_hash;
}

void atualizar_prox(FILE *clientes_arq, int posicao, int novo_prox)
{
    fseek(clientes_arq, posicao * tamanho_registro_cliente(), SEEK_SET);
    Cliente *cliente = ler_cliente(clientes_arq);
    if (cliente == NULL)
    {
        printf("Erro ao ler cliente na posição %d\n", posicao);
        return;
    }

    cliente->prox = novo_prox;

    fseek(clientes_arq, posicao * tamanho_registro_cliente(), SEEK_SET);
    salvar_cliente(cliente, clientes_arq);

    free(cliente);
}

FILE *inserir_registro_hash(FILE *tabela_hash, FILE *clientes_arq, Cliente *cliente)
{
    if (!Vazia1(Fila))
    {
        TStruct1 id;
        Desenfileirar1(&Fila, &id);

        cliente->id = id.codigo;
        cliente->ocupado = 1;

        int posicao = posicao_cliente(cliente, clientes_arq);
        fseek(clientes_arq, posicao * tamanho_registro_cliente(), SEEK_SET);
        salvar_cliente(cliente, clientes_arq);
    }
    else
    {
        cliente->id = (tamanho_arquivo_clientes(clientes_arq) + 1);
        fseek(clientes_arq, 0, SEEK_END);
        salvar_cliente(cliente, clientes_arq);
    }

    int reg = posicao_cliente(cliente, clientes_arq);
    int hash = funcao_hash(cliente);
    fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
    Posicao *pos = ler_posicao(tabela_hash);
    if (pos->posicao == -1)
    {
        fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
        pos = criar_posicao(reg);
        salvar_posicao(pos, tabela_hash);
    }
    else
    {
        fseek(clientes_arq, pos->posicao * tamanho_registro_cliente(), SEEK_SET);
        Cliente *ultimo = ler_cliente(clientes_arq);
        int pos_ultimo = pos->posicao;

        // Encontrar o último da lista encadeada
        while (ultimo->prox != -1)
        {
            pos_ultimo = ultimo->prox;
            fseek(clientes_arq, ultimo->prox * tamanho_registro_cliente(), SEEK_SET);
            ultimo = ler_cliente(clientes_arq);
        }
        ultimo->prox = reg; // 'reg' deve ser a posição física do novo cliente

        // Salva a atualização do último cliente
        fseek(clientes_arq, pos_ultimo * tamanho_registro_cliente(), SEEK_SET);
        salvar_cliente(ultimo, clientes_arq);

        // Libera a memória do cliente lido
        free(ultimo);
    }

    free(pos);
    return tabela_hash;
}

Cliente *buscar_registro_hash(int cliente_id, FILE *clientes_arq, FILE *tabela_hash)
{
    int hash = cliente_id % TAMANHO_TABELA;
    fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
    Posicao *pos = ler_posicao(tabela_hash);
    fseek(clientes_arq, pos->posicao * tamanho_registro_cliente(), SEEK_SET);
    Cliente *cliente = ler_cliente(clientes_arq);
    if (cliente->id == cliente_id && cliente->ocupado == 1)
    {
        return cliente;
    }
    else if (cliente->prox == -1)
    {
        return NULL;
    }
    else
    {
        while (cliente->prox != -1 && cliente->id != cliente_id)
        {
            fseek(clientes_arq, cliente->prox * tamanho_registro_cliente(), SEEK_SET);
            cliente = ler_cliente(clientes_arq);
        }

        if (cliente->id == cliente_id && cliente->ocupado == 1)
        {
            return cliente;
        }
        else
        {
            return NULL;
        }
    }
}

void excluir_registro_hash(int cliente_id, FILE *clientes_arq, FILE *tabela_hash)
{
    int hash = cliente_id % TAMANHO_TABELA;

    fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
    Posicao *pos = ler_posicao(tabela_hash);

    if (pos->posicao == -1)
    {
        printf("[ERRO]: Cliente não encontrado!\n");
        free(pos);
        return;
    }

    fseek(clientes_arq, pos->posicao * tamanho_registro_cliente(), SEEK_SET);
    Cliente *cliente = ler_cliente(clientes_arq);
    Cliente *anterior = NULL;
    int pos_anterior = -1;
    int pos_atual = pos->posicao;

    //O cliente é o único na lista encadeada
    if (cliente->id == cliente_id && cliente->ocupado == 1 && cliente->prox == -1)
    {
        pos = criar_posicao(-1);
        fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
        salvar_posicao(pos, tabela_hash);
    }

    // Busca pelo cliente na lista encadeada
    while (cliente->prox != -1 && cliente->id != cliente_id)
    {
        pos_anterior = pos_atual;
        anterior = cliente;
        pos_atual = cliente->prox;

        if (pos_atual == -1)
            break;

        fseek(clientes_arq, pos_atual * tamanho_registro_cliente(), SEEK_SET);
        cliente = ler_cliente(clientes_arq);
    }

    if (cliente == NULL || cliente->id != cliente_id)
    {
        printf("[ERRO]: Cliente não encontrado!\n");
        if (anterior)
            free(anterior);
        free(cliente);
        free(pos);
        return;
    }

    // Caso 1: Remoção do primeiro elemento
    if (pos_anterior == -1)
    {
        fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
        if (cliente->prox != -1)
        {
            // Atualiza tabela hash para apontar para o próximo
            Posicao *nova_pos = criar_posicao(cliente->prox);
            salvar_posicao(nova_pos, tabela_hash);
            free(nova_pos);
        }
        else
        {
            // Lista ficará vazia (compartimento)
            Posicao *nova_pos = criar_posicao(-1);
            salvar_posicao(nova_pos, tabela_hash);
            free(nova_pos);
        }
    }
    // Caso 2: Remoção do meio/fim da lista
    else
    {
        // Atualiza o anterior para apontar para o próximo do removido
        atualizar_prox(clientes_arq, pos_anterior, cliente->prox);
        // anterior->prox = cliente->prox;
        // fseek(clientes_arq, pos_anterior * tamanho_registro_cliente(), SEEK_SET);
        // salvar_cliente(anterior, clientes_arq);
    }

    // Marca cliente como removido
    cliente->ocupado = 0;
    cliente->prox = -1;
    fseek(clientes_arq, pos_atual * tamanho_registro_cliente(), SEEK_SET);
    salvar_cliente(cliente, clientes_arq);

    TStruct1 id;
    id.codigo = cliente_id;
    Enfileirar1(id, &Fila);

    // Libera memória
    if (anterior)
        free(anterior);
    free(cliente);
    free(pos);
}

void imprimir_lista_encadeada_tabela(FILE *tabela_hash, FILE *clientes_arq)
{
    rewind(tabela_hash);
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        Posicao *pos = ler_posicao(tabela_hash);
        fseek(clientes_arq, pos->posicao * tamanho_registro_cliente(), SEEK_SET);
        if (pos->posicao == -1)
        {
            printf("\n[%d]: VAZIO", i);
            continue;
        }

        Cliente *cliente = ler_cliente(clientes_arq);

        printf("\n[%d]: %d ", i, cliente->id);

        while (cliente->prox != -1)
        {
            fseek(clientes_arq, cliente->prox * tamanho_registro_cliente(), SEEK_SET);
            cliente = ler_cliente(clientes_arq);
            printf("-> %d", cliente->id);
        }

        free(cliente);
        free(pos);
    }
}

void imprimir_tabela_hash(FILE *tabela_hash)
{
    rewind(tabela_hash);
    printf("Indice \tPosicao\n");
    for (int i = 0; i < TAMANHO_TABELA; i++)
    {
        Posicao *pos = ler_posicao(tabela_hash);
        printf(" %d:\t%d\n", i, pos->posicao);
        free(pos);
    }
}

FILE *ordenar_arquivos_hash(FILE *clientes_arq)
{
    Fila = inicializa_fila();
    FILE *tabela_hash = criar_tabela_hash();

    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));

    int reg = 0;
    int tamanho_arq = tamanho_arquivo_clientes(clientes_arq);

    rewind(clientes_arq);

    while ((cliente = ler_cliente(clientes_arq)) != NULL && reg < tamanho_arq)
    {
        int hash = funcao_hash(cliente);
        fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
        Posicao *pos = ler_posicao(tabela_hash);

        if (pos->posicao == -1)
        {
            fseek(tabela_hash, hash * tamanho_registro_hash(), SEEK_SET);
            pos = criar_posicao(reg);
            salvar_posicao(pos, tabela_hash);
        }
        else
        {
            fseek(clientes_arq, pos->posicao * tamanho_registro_cliente(), SEEK_SET);
            cliente = ler_cliente(clientes_arq);
            int anterior = pos->posicao;
            
            // Percorre a lista encadeada até encontrar o último nó
            while (cliente->prox != -1)
            {
                fseek(clientes_arq, cliente->prox * tamanho_registro_cliente(), SEEK_SET);
                anterior = cliente->prox;
                cliente = ler_cliente(clientes_arq);
            }
            atualizar_prox(clientes_arq, anterior, reg);
            
            fseek(clientes_arq, (reg+1) * tamanho_registro_cliente(), SEEK_SET);
        }
        free(cliente);
        reg++;
    }
    return tabela_hash;
}

#endif