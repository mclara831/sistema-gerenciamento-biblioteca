#include "interface/cli.c"

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

    criar_bases_ordenadas(clientes_arq, livros_arq, emprestimos_arq, 10);

    int opcao = 0, continuar = 1, tamanho;
   
    printf("**************** BEM VINDO AO SISTEMA DE GERENCIAMENTO DE BIBLIOTECA ************************");

    while (continuar)
    {
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
            buscar_cliente(clientes_arq);
            break;
        case 3:
            imprimir_base_clientes(clientes_arq);
            break;
        case 4:
            registrar_novo_livro(livros_arq);
            break;
        case 5:
            buscar_livro(livros_arq);
            break;
        case 6:
            imprimir_base_livros(livros_arq);
            break;
        case 7:
            registrar_novo_emprestimo(clientes_arq, livros_arq, emprestimos_arq);
            break;
        case 8:
            realizar_devolucao(emprestimos_arq, livros_arq);
            break;
        case 9:
            renovar_emprestimo(emprestimos_arq);
            break;
        case 10:
            imprimir_base_emprestimos(emprestimos_arq);
            break;
        case 11:
            printf("Digite o tamanho da base de dados que deseja criar: ");
            scanf("%d", &tamanho);
            criar_bases_ordenadas(clientes_arq, livros_arq, emprestimos_arq, tamanho);
            break;
        case 12:
            printf("Digite o tamanho da base de dados que deseja criar: ");
            scanf("%d", &tamanho);
            criar_bases_desordenadas(clientes_arq, livros_arq, emprestimos_arq, tamanho);
            printf("\n[AVISO]: AO CRIAR A BASE DE DADOS DESORDENADA E POSSIVEL QUE A BUSCA BINARIA NAO RETORNA DE DE FORMA CORRETA OS DADOS DA ENTIDADE PROCURADA!\n\n");
            break;
        default:
            printf("Opcao invalida! Digite novamente!\n");
            break;
        }
    }

    printf("**************** OBRIGADO POR UTILIZAR NOSSO SISTEMA ! ************************\n");

    fclose(clientes_arq);
    fclose(livros_arq);
    fclose(emprestimos_arq);
    return (0);
}
