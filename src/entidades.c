#include "entidades.h"

/**********************************************************
                 FUNÇÕES GERAIS
***********************************************************/
void shuffle(int *vet, int tamanho_maximo, int tamanho_minimo) {
    srand(time(NULL));
    for (int i = tamanho_maximo - 1; i > 0; i--)
    {
        int j = rand() % (i);
        int tmp = vet[j];
        vet[j] = vet[i];
        vet[i] = tmp;
    }
}

/**********************************************************
                 FUNÇÕES DO CLIENTE
***********************************************************/
int tamanho_registro_cliente() {
    return sizeof(int)
            + sizeof(char) * 20
            + sizeof(char) * 16;
}

int tamanho_arquivo_clientes(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tamanho = trunc(ftell(arq) / tamanho_registro_cliente(arq));
    return tamanho;
} 

Cliente* criar_cliente(int id, char *nome, char *cpf) {
    Cliente* cliente = (Cliente*) malloc(sizeof(Cliente));
    cliente->id = id;
    strcpy(cliente->nome, nome);
    strcpy(cliente->cpf, cpf);
    return cliente;
}

void salvar_cliente(Cliente *cliente, FILE *arq) {
    fwrite(&cliente->id, sizeof(int), 1, arq);
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), arq);
    fwrite(cliente->cpf, sizeof(char), sizeof(cliente->cpf), arq);
}

Cliente* ler_cliente(FILE *arq) {
    Cliente* cliente = (Cliente*) malloc(sizeof(Cliente));
    fread(&cliente->id, sizeof(int), 1, arq);
    fread(cliente->nome, sizeof(char), sizeof(cliente->nome), arq);
    fread(cliente->cpf, sizeof(char), sizeof(cliente->cpf), arq);
    return cliente;
}

void criar_base_ordenada_clientes(FILE *arq, int tamanho) {
    int vetor[tamanho];
    Cliente *cliente;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;
    
    for (int i = 0; i < tamanho; i++) {
        cliente = criar_cliente(vetor[i], "ANONIMO", "000.000.000-00");
        salvar_cliente(cliente, arq);
    }
    free(cliente);
}

void criar_base_desordenada_clientes(FILE *arq, int tamanho) {
    int vetor[tamanho];
    Cliente *cliente;

    for(int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    shuffle(vetor, tamanho, 0);
    
    for(int i = 0; i < tamanho; i++) {
        cliente = criar_cliente(vetor[i], "ANONIMO", "000.000.000-00");
        salvar_cliente(cliente, arq);
    }
    free(cliente);
}

void imprimir_cliente(Cliente *cliente) {
    printf("**********************************************");
    printf("\nCliente de codigo ");
    printf("%d", cliente->id);
    printf("\nNome: ");
    printf("%s", cliente->nome);
    printf("\nCPF: ");
    printf("%s", cliente->cpf);
    printf("\n**********************************************\n");
}

void imprimir_base_clientes(FILE *arq) {

    rewind(arq);

    Cliente *cliente;
    while ((cliente = ler_cliente(arq)) != NULL)
        imprime(cliente);

    free(cliente);
}

/**********************************************************
                 FUNÇÕES DO LIVRO
***********************************************************/
int tamanho_registro_livro() {
    return sizeof(int)
            + sizeof(char) * 20
            + sizeof(char) * 20
            + sizeof(char) * 10
            + sizeof(int)
            + sizeof(char);

}

int tamanho_arquivo_livros(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tamanho = trunc(ftell(arq) / tamanho_registro_livro());
    return tamanho;
}

Livro* criar_livro(int id, char *titulo, char *autor, char *gênero, int anoPublicacao, char disponibilidade) {
    Livro* livro = (Livro*) malloc(sizeof(Livro));
    livro->id = id;
    strcpy(livro->titulo, titulo);
    strcpy(livro->autor, autor);
    strcpy(livro->gênero, gênero);
    livro->anoPublicacao =  anoPublicacao;

    if (disponibilidade == 'S'|| disponibilidade =='s')
        livro->disponibilidade = 'S';
    else
        livro->disponibilidade = 'N';

    return livro;
}

void salvar_livro(Livro *livro, FILE *arq) {
    fwrite(&livro->id, sizeof(int), 1, arq);
    fwrite(livro->titulo, sizeof(char), sizeof(livro->titulo), arq);
    fwrite(livro->autor, sizeof(char), sizeof(livro->autor), arq);
    fwrite(livro->gênero, sizeof(char), sizeof(livro->gênero), arq);
    fwrite(&livro->anoPublicacao, sizeof(int), 1, arq);
    fwrite(livro->disponibilidade, sizeof(char), 1, arq);
}

Livro* ler_livro(FILE *arq) {
    Livro *livro = (Livro*)malloc(sizeof(Livro));
    fread(&livro->id, sizeof(int), 1, arq);
    fread(livro->titulo, sizeof(char), sizeof(livro->titulo), arq);
    fread(livro->autor, sizeof(char), sizeof(livro->autor), arq);
    fread(livro->gênero, sizeof(char), sizeof(livro->gênero), arq);
    fread(&livro->anoPublicacao, sizeof(int), 1, arq);
    fread(livro->disponibilidade, sizeof(char), sizeof(char), arq);

    return livro;
}

void criar_base_ordenada_livros(FILE *arq, int tamanho) {
    int vetor[tamanho];
    Livro *livro;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;
    
    for (int i = 0; i < tamanho; i++) {
        livro = criar_livro(vetor[i], "Linguagem C", "Desconhecido", "Científico", 1996, 's');
        salvar_livro(livro, arq);
    }
    free(livro);
}

void criar_base_desordenada_livros(FILE *arq, int tamanho) {
    int vetor[tamanho];
    Livro *livro;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    shuffle(vetor, tamanho, 1);
    
    for (int i = 0; i < tamanho; i++) {
        livro = criar_livro(vetor[i], "Linguagem C", "Desconhecido", "Científico", 1996, 's');
        salvar_livro(livro, arq);
    }
    free(livro);
}

void imprimir_livro(Livro *livro) {
    printf("**********************************************");
    printf("\nLivro de codigo ");
    printf("%d", livro->id);
    printf("\nTitulo: ");
    printf("%s", livro->titulo);
    printf("\nAutor: ");
    printf("%s", livro->autor);
    printf("\nGenero: ");
    printf("%s", livro->gênero);
    printf("\nAno de publicacao: ");
    printf("%d", livro->anoPublicacao);
    printf("\nDisponibilidade: ");
    printf("%c", livro->disponibilidade);   
    printf("\n**********************************************\n");
}

void imprimir_base_livros(FILE *arq) {
    rewind(arq);
    Livro *livro;
    while ((livro = ler_livro(arq)) != NULL) {
        imprimir_livro(livro);
    }

    free(livro);
}

/**********************************************************
                 FUNÇÕES DO EMPRÉSTIMO
***********************************************************/
int tamanho_registro_emprestimo() {
    return sizeof(int)
            + sizeof(Livro)
            + sizeof(Cliente)
            + sizeof(char) * 11
            + sizeof(char) * 11
            + sizeof(char) * 11
            + sizeof(char)
            + sizeof(char);
}

int tamanho_arquivo_emprestimos(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tamanho = trunc(ftell(arq) / tamanho_registro_emprestimo());
    return tamanho;
}

Emprestimo* criar_emprestimo(int id, Livro *livro, Cliente *cliente, char *data_emprestimo, char *data_prevista, char *data_devolucao, char devolvido, char atraso) {
    Emprestimo *emprestimo = (Emprestimo*)malloc(sizeof(Emprestimo));
    emprestimo->id = id;
    emprestimo->livro = livro;
    emprestimo->cliente = cliente;
    strcpy(emprestimo->data_emprestimo, data_emprestimo);
    strcpy(emprestimo->data_prevista , data_prevista);
    strcpy(emprestimo->data_devolucao , data_devolucao); 
    emprestimo->devolvido = devolvido;
    emprestimo->atraso = atraso;
    return emprestimo;
}

void salvar_emprestimo(Emprestimo *emprestimo, FILE *arq) {
    fwrite(&emprestimo->id, sizeof(int), 1, arq);
    fwrite(emprestimo->livro, sizeof(Livro), 1, arq);
    fwrite(emprestimo->cliente, sizeof(Livro), 1, arq);
    fwrite(emprestimo->data_emprestimo, sizeof(char), sizeof(emprestimo->data_emprestimo), arq);
    fwrite(emprestimo->data_prevista, sizeof(char), sizeof(emprestimo->data_prevista), arq);
    fwrite(emprestimo->data_devolucao, sizeof(char), sizeof(emprestimo->data_devolucao), arq);
    fwrite(emprestimo->devolvido, sizeof(char), sizeof(char), arq);
    fwrite(emprestimo->data_devolucao, sizeof(char), sizeof(char), arq);

}
Emprestimo* ler_emprestimo(FILE *arq) {
    Emprestimo *emprestimo = (Emprestimo*)malloc(sizeof(Emprestimo));
    fread(&emprestimo->id, sizeof(int), 1, arq);
    fread(emprestimo->livro, sizeof(Livro), 1, arq);
    fread(emprestimo->cliente, sizeof(Livro), 1, arq);
    fread(emprestimo->data_emprestimo, sizeof(char), sizeof(emprestimo->data_emprestimo), arq);
    fread(emprestimo->data_prevista, sizeof(char), sizeof(emprestimo->data_prevista), arq);
    fread(emprestimo->data_devolucao, sizeof(char), sizeof(emprestimo->data_devolucao), arq);
    fread(emprestimo->devolvido, sizeof(char), sizeof(char), arq);
    fread(emprestimo->data_devolucao, sizeof(char), sizeof(char), arq);
}

void criar_base_ordenada_emprestimos(FILE *arq, int tamanho) {
    int vetor[tamanho];
    Emprestimo *emprestimo;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    
    for (int i = 0; i < tamanho; i++) {
        emprestimo = criar_emprestimo(vetor[i], NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        salvar_livro(emprestimo, arq);
    }
    free(emprestimo);
}

void criar_base_desordenada_emprestimos(FILE *arq, int tamanho) {
    int vetor[tamanho];
    Emprestimo *emprestimo;

    for (int i = 0; i < tamanho; i++)
        vetor[i] = i + 1;

    shuffle(vetor, tamanho, 1);

    for (int i = 0; i < tamanho; i++) {
        emprestimo = criar_emprestimo(vetor[i], NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        salvar_livro(emprestimo, arq);
    }

    free(emprestimo);
}

void imprimir_emprestimo(Emprestimo *emprestimo) {
    printf("**********************************************");
    printf("\nCódigo do Emprestimo: ");
    printf("%d", emprestimo->id);
    printf("\nTitulo: ");
    printf("%s", emprestimo->livro->titulo);
    printf("\nCliente: ");
    printf("%s", emprestimo->cliente->nome);
    printf("\nData de emprestimo: ");
    printf("%s", emprestimo->data_emprestimo);
    printf("\nData de prevista de devolucao: ");
    printf("%s", emprestimo->data_devolucao);
    printf("\nData de devolucao: ");
    printf("%s", emprestimo->data_devolucao);
    printf("\nDevolvido: ");
    printf("%c", emprestimo->devolvido);
    printf("\nDevolvido com atraso: ");
    printf("%c", emprestimo->atraso);   
    printf("\n**********************************************\n");
}

void imprimir_base_emprestimos(FILE *arq) {
    rewind(arq);

    Emprestimo *emprestimo;
    while ((emprestimo = ler_emprestimo(arq)) != NULL)
    {
        imprimir_emprestimo(emprestimo);
    }
    free(emprestimo);    
}
