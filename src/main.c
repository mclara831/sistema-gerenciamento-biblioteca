#include "interface/cli.c"
#include "metodos_ordenacao/quick_sort.c"
#include "metodos_ordenacao/selecao_natural.c"
#include "metodos_ordenacao/intercalacao_otima.c"
#include "metodos_ordenacao/intercalacao_otima2.c"
#include "hashing/hash.c"

int main(void)
{
    FILE *clientes_arq, *livros_arq, *emprestimos_arq;

    if ((clientes_arq = fopen("arq_clientes.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de clientes!");
        exit(1);
    }

    if ((livros_arq = fopen("arq_livros.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de livros!");
        exit(1);
    }

    if ((emprestimos_arq = fopen("arq_emprestimos.dat", "w+b")) == NULL)
    {
        printf("[ERRO]: Não foi possivel abrir o arquivo de emprestimos!");
        exit(1);
    }

    /**************************************************************************
                TESTES DAS BASES DAS ENTIDADES COM DIFERENTES TAMANHOS
    ***************************************************************************/
    /*int tam = 10;
    for (int i = 0; i < 4; i++)
    {
        if ((clientes_arq = fopen("arq_clientes.dat", "w+b")) == NULL)
        {
            printf("[ERRO]: Não foi possivel abrir o arquivo de clientes!");
            exit(1);
        }
        criar_base_desordenada_clientes(clientes_arq, tam);
        // quick_sort_com_logs(clientes_arq, 0, tamanho_arquivo_clientes(clientes_arq) - 1, 1);
        srand(time(NULL));
        int tam_particao = rand() % (tam/2);
        printf("%d", tam_particao);
        int n_particoes = selecao_natural_clientes(clientes_arq, tam_particao);
        intercalacao_otima_clientes2(n_particoes/2, n_particoes, clientes_arq);
        fclose(clientes_arq);
        tam = tam * 10;
    }

    tam = 10;
    for (int i = 0; i < 4; i++)
    {
        if ((livros_arq = fopen("arq_livros.dat", "w+b")) == NULL)
        {
            printf("[ERRO]: Não foi possivel abrir o arquivo de livros!");
            exit(1);
        }
        criar_base_desordenada_livros(livros_arq, tam);
        // quick_sort_com_logs(livros_arq, 0, tamanho_arquivo_livros(livros_arq) - 1, 2);
        srand(time(NULL));
        int tam_particao = rand() % (tam/2);
        int n_particoes = selecao_natural_livros(livros_arq, tam_particao);
        intercalacao_otima_livros2(n_particoes/2, n_particoes, livros_arq);
        fclose(livros_arq);
        tam = tam * 10;
    }

    tam = 10;
    for (int i = 0; i < 4; i++)
    {
        if ((emprestimos_arq = fopen("arq_emprestimos.dat", "w+b")) == NULL)
        {
            printf("[ERRO]: Não foi possivel abrir o arquivo de emprestimos!");
            exit(1);
        }
        criar_base_desordenada_emprestimos(livros_arq, tam);
        // quick_sort_com_logs(emprestimos_arq, 0, tamanho_arquivo_emprestimos(emprestimos_arq) - 1, 3);
        srand(time(NULL));
        int tam_particao = rand() % (tam/2);
        int n_particoes = selecao_natural_emp(emprestimos_arq, tam_particao);
        intercalacao_otima_emprestimos2(n_particoes/2, n_particoes, emprestimos_arq);
        fclose(emprestimos_arq);
        tam = tam * 10;
    }
    system("pause");*/

    /*********************CRIAÇÃO PADRÃO DA BASE DE DADOS PARA INICIALIZAÇÃO DO SISTEMA ********************/
    criar_bases_desordenadas(clientes_arq, livros_arq, emprestimos_arq, 300);

    FILE *tabela_hash = ordenar_arquivos_hash(clientes_arq);
    imprimir_tabela_hash(tabela_hash);

    int opcao = 0, continuar = 1, tamanho;
    while (continuar)
    {
        menu_opcoes_hash(clientes_arq, tabela_hash);
        scanf("%d", &opcao);
        fflush(stdin);

        switch (opcao)
        {
        case 0:
            continuar = 0;
            fclose(clientes_arq);
            fclose(tabela_hash);
            break;
        case 1:
            criar_novo_cli_hash(tabela_hash, clientes_arq);
            break;
        case 2:
            buscar_cliente_hash(tabela_hash, clientes_arq);
            break;
        case 3:
            excluir_cliente_hash(tabela_hash, clientes_arq);
            break;
        case 4:
            imprimir_tabela_hash(tabela_hash);
            break;
        case 5:
            imprimir_lista_encadeada_tabela(tabela_hash, clientes_arq);
            break;
        case 6:
            imprimir_base_clientes(clientes_arq);
            break;
        default:
            printf("[ERRO] Opcao invalida!\n");
            break;
        }
    }

    return 0;

    printf("**************** BEM VINDO AO SISTEMA DE GERENCIAMENTO DE BIBLIOTECA ************************");

    while (continuar)
    {
        // system("cls");
        menu_principal();
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 0:
            continuar = 0;
            break;
        case 1:
            registrar_novo_cliente(clientes_arq);
            break;
        case 2:
            editar_dados_cliente(clientes_arq);
            break;
        case 3:
            buscar_cliente(clientes_arq);
            break;
        case 4:
            imprimir_base_clientes(clientes_arq);
            break;
        case 5:
            registrar_novo_livro(livros_arq);
            break;
        case 6:
            editar_dados_livro(livros_arq);
            break;
        case 7:
            buscar_livro(livros_arq);
            break;
        case 8:
            imprimir_base_livros(livros_arq);
            break;
        case 9:
            registrar_novo_emprestimo(clientes_arq, livros_arq, emprestimos_arq);
            break;
        case 10:
            realizar_devolucao(emprestimos_arq, livros_arq);
            break;
        case 11:
            renovar_emprestimo(emprestimos_arq);
            break;
        case 12:
            buscar_emprestimo(emprestimos_arq);
            break;
        case 13:
            imprimir_base_emprestimos(emprestimos_arq);
            break;
        case 14:
            printf("Digite o tamanho da base de dados que deseja criar: ");
            scanf("%d", &tamanho);
            criar_bases_ordenadas(clientes_arq, livros_arq, emprestimos_arq, tamanho);
            break;
        case 15:
            printf("Digite o tamanho da base de dados que deseja criar: ");
            scanf("%d", &tamanho);
            criar_bases_desordenadas(clientes_arq, livros_arq, emprestimos_arq, tamanho);
            break;
        case 16:
            iniciar_ordenacao(clientes_arq, livros_arq, emprestimos_arq);
            break;
        default:
            printf("[AVISO]: Opcao invalida! Digite novamente!\n");
            break;
        }
    }

    printf("**************** OBRIGADO POR UTILIZAR NOSSO SISTEMA ! ************************\n");

    fclose(clientes_arq);
    fclose(livros_arq);
    fclose(emprestimos_arq);
    return (0);
}
