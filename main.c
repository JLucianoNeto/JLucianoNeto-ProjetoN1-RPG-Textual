#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#define nulo -1
// Teste biuld 0.5 jogo inicio 03/03/2023
// primeiro teste será a criação do primeiro esboço do inventário
// vida, precisa ser uma variável global

// Variaveis globais:
int dano_inm, dealed_dmg, pos_atual, batalha_on;
const char *rosa_v;
//Variáveis globais:

// Estruturas:
struct dados_per
{
    int mylife;
    int mypotion;
    char myname[25];
};
struct dados_per s1 = {100, 4};

struct per_sword
{
    int swr_dmg;
};
struct per_sword s2 = {10};
// Estruturas:

// declaração de funções e matrizes:
int matriz[6][6], vetor_apoio[1][6];
int  mov_salas(), acoe_per(), toupper(int c);
void batalha(int Dmg, int inm),rosa_ventos(int coluna), ver_nulidade(int indice, int in_vtp),caminhada(), tomar_pocao(),geren_vida(int lp),delay(int seconds);
// declaração de funções e matrizes:
    




/**
* ... As linhas dessa matriz dizem respeito aos dados que cada sala possui, a primeira coluna é o id da sala, 
* as quatro seguintes são referentes as direções disponíveis, a coluna 6 se refere a possibilidade da sala
* estar vazia[0], possuir um monstro[1] ou possuir uma poção[2].
* A coluna 7 diz respeito a salas especiais como: sala da chave[1],sala do baú[2] ou sala do boss[3]  ...
* 
*/
int matriz[6][6] = {
    {0, 2, nulo, nulo, 1, 0},
    {1, nulo, nulo, 0, 4, 1},
    {2, nulo, 0, nulo, nulo, 0},
    {3, 5, 4, nulo, nulo, 0},
    {4, 3, nulo, 1, nulo, 0},
    {5, nulo, 3, nulo, nulo, 1},
};
pos_atual = 0;

int main(void)
{

    printf("Digite seu nome : \n");
    scanf("%s", s1.myname);

    mov_salas();
}

/**
 * @brief Essa função é um timer que aguarda, em segundos, o valor de seu parametro para posteriormente continuar o código
*/
void delay(int seconds)
{ // Função de timer do João
    time_t now_time, end_time;

    time(&now_time);
    end_time = now_time + seconds;

    while (now_time < end_time)
        time(&now_time);
}

/**
 *@brief Essa função diz respeito a ação de tomar poção e adiciona um valor fixo ao hp do personagem até que as poções acabem 
*/
void tomar_pocao()
{
    if (s1.mypotion > 0)
    {
        printf("\nVocê recuperou 15 de vida !\n");
        geren_vida(15);
        printf("\n═══════════════════════════════════════");
        printf("\n %s - HP: %d\n", s1.myname, s1.mylife);
        printf("═══════════════════════════════════════\n");
        delay(4);
        s1.mypotion -= 1;
    }
    else
    {
        printf("Suas poções acabaram :/ ");
    }
}

/**
 * @brief Gerencia os paradigmas da vida do personagem: a morte, a adição de vida e a subtração de vida
 * @param lp O quanto de vida será adicionado ou subtraido da vida do personagem
*/
void geren_vida(int lp)
{
    int newLife = s1.mylife + lp;
    if (newLife <= 0)
    {
        delay(5);
        printf("\e[1;1H\e[2J");
        printf("\rVocê morreu :( ");
        exit(0);
    }
    else
    {
        if (newLife >= 100)
        { 
            s1.mylife = 100;
        }
        else
        {
            if (newLife > 0 && newLife < 100)
            {
                s1.mylife = newLife;
            }
        }
    }
}

/**
 *  @brief  Essa função diz respeito as possíveis ações que o personagem pode tomar
 *  @return int pode retornar 1 para a validação de uma condicional contida na função batalha().
 */
int acoe_per()
{
    int resposta;
    printf("\nVocê ataca [1] ou se cura [2] ? ");
    scanf("%i", &resposta);
    getchar();
    switch (resposta)
    {
    case 1:
        return 1;
        break;
    case 2:
        tomar_pocao();
        break;
    default:
        printf("\n╟ !Comando inválido! ╢\n");
        acoe_per();
        break;
    }
    return 0;
}

/**
 * @brief Essa função diz respeito a mecânica da batalha, um loop que só acaba quando o monstro morre
 * @param dmg Diz respeito ao dano máximo do inimigo
 * @param inm Diz respeito a vida do inimigo
*/
void batalha(int Dmg, int inm)
{
    batalha_on = 1;
    srand(time(NULL)); // Função que reinicia a função rand()
    do
    {

        printf("\n");
        printf("\e[1;1H\e[2J"); // limpa tela

        printf("\n════════════════════════════════════════");
        printf("\n	  %s - HP: %d\n", s1.myname, s1.mylife);
        printf("	  Poções : %i", s1.mypotion);
        printf("\n════════════════════════════════════════");

        printf("\n _                                    _\n");
        printf("(_'----------------------------------'_)\n");
        printf("(_.==================================._)\n");
        printf("\n 	💀Esqueleto - HP: : %i", inm);
        printf("\n _                                    _\n");
        printf("(_'----------------------------------'_)\n");
        printf("(_.==================================._)");

        if (acoe_per() == 1)
        {
            dealed_dmg = rand() % (s2.swr_dmg + 1);
            inm -= dealed_dmg;
            printf("\n═══════════════════════════════════════\n");
            printf("\n     ⚔ Você causou %i de dano ! ⚔   \n", dealed_dmg);
        }

        dano_inm = rand() % Dmg;
        geren_vida(dano_inm * -1);

        printf(" \n     ☠  Você tomou %i de dano ! ☠  \n", dano_inm);
        printf("\n═══════════════════════════════════════");

        delay(4);

        if (inm <= 0)
        {
			printf("\n");
            printf("\n═══════════════════════════════════════");
            printf("\n ᕙ( ͡❛ ͜ʖ ͡❛)ᕗ Você matou o esqueleto ᕙ( ͡❛ ͜ʖ ͡❛)ᕗ ");
            printf("\n═══════════════════════════════════════\n");
            delay(4);
            batalha_on = 0;
        }
    } while (batalha_on == 1);
}

// Daqui para baixo são as mecânicas da salas
/**
 * @brief Aqui será a forma pela qual o jogador se "movimentará" pela matriz
*/
int mov_salas()
{
    do
    {
		/**
		 * Essa primeira cadeia de ifs diz respeito a 6 coluna da matriz onde cada um deles categoriza um evento diferente 
        */
		if (matriz[pos_atual][5] == 1)
        {
            batalha(5, 50);
            if (batalha_on == 0)
            {
                matriz[pos_atual][5] = 0;
            }

            // }else{
            // 	 if (matriz[pos_atual][5] == 2)
            // 	 {
            // Nesse espaço vai entrar o puzzle para pegar a chave;
            // 	 }
        }
        for (int i = 1; i < 5; i++)
        {
            if ((matriz[pos_atual][i] != nulo))
            {
                rosa_ventos(i);
                printf("Você pode ir para o %s [ %i ] \n", rosa_v, matriz[pos_atual][i]);
            }
            vetor_apoio[0][i] = matriz[pos_atual][i];
        }
        caminhada();
        printf("\n__________________________________________\n");
    } while (1);
}

/**
 * @brief Serve para saber para qual direção o personagem quer ir
 * 
*/
void caminhada()
{
    char resposta;
    printf("\n\r Você quer ir para qual direção ? \n");
    scanf(" %c", &resposta);
    resposta = toupper(resposta);

    switch (resposta)
    {
    case 'N':
        ver_nulidade(vetor_apoio[0][1], 1);
        break;
    case 'S':
        ver_nulidade(vetor_apoio[0][2], 2);
        break;
    case 'L':
        ver_nulidade(vetor_apoio[0][3], 3);
        break;
    case 'O':
        ver_nulidade(vetor_apoio[0][4], 4);
        break;
    default:
        caminhada();
        break;
    }

}

/**
 * @brief Essa função verifica a presença do nulo na escolha e se não houver nulo ele salva a nova posição
 * @param indice Vai capturar o valor dentro do vetor de apoio na posição relacionada com a coluna
 * @param in_vtp "in vector position" diz respeito a posição da coluna no vetor_apoio
 */
void ver_nulidade(int indice, int in_vtp)
{
    if (indice == nulo)
    {
        printf("\n╟ !Nenhum caminho disponível, digite novamente! ╢\n");
        caminhada();
    }
    else
    {
        pos_atual = vetor_apoio[0][in_vtp];
    }
}

/**
 * @brief Vai traduzir qual a direção representada pelo inteiro na matriz e salvar seu nome em rosa_v
 * @param coluna Valor que representa a coluna na matriz 
 */
void rosa_ventos(int coluna)
{

    switch (coluna)
    {
    case 1:
        rosa_v = "Norte[N]";
        break;
    case 2:
        rosa_v = "Sul[S]";
        break;
    case 3:
        rosa_v = "Leste[L]";
        break;
    case 4:
        rosa_v = "Oeste[O]";
    default:
        break;
    }
}
