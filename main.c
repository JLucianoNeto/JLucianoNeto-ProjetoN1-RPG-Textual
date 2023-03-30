#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#define nulo -1


//Variáveis globais:

int dano_inm, dealed_dmg, pos_atual = 0, batalha_on,chave;
const char *rosa_v;

//Variáveis globais:

// Estruturas:

struct dados_per
{
    int mylife;
    int mypotion;
    char myname[25];
    int prot_am;
};
struct dados_per s1 = {100, 3,"",0};

struct per_sword
{
    int swr_dmg;
    int esp_buff;
};
struct per_sword s2 = {10,0};

// Estruturas:

// declaração de funções e matrizes:

int matriz[18][7], vetor_apoio[1][6];
int  mov_salas(), acoe_per(),enigma_chave();
void batalha(int Dmg, int inm),rosa_ventos(int coluna), ver_nulidade(int indice, int in_vtp),caminhada(), tomar_pocao(),geren_vida(int lp);
void abrir_bau(),delay(int seconds),desc_salas(int id_sala);
// declaração de funções e matrizes:
    

/**
* ... As linhas dessa matriz dizem respeito aos dados que cada sala possui, a primeira coluna é o id da sala, 
* as quatro seguintes são referentes as direções disponíveis, a coluna 6 se refere a possibilidade da sala
* estar vazia[0] ou possuir um monstro[1] .
* Na coluna 6 ainda esxistem as salas especiais como: sala da chave[2],sala do baú[3], sala do boss[4] ou evento especial[5]  ...
* Na coluna 7 diz repeito a ter poção ou não como loot
*/
int matriz[18][7] = {
    {0, 1, nulo, nulo, nulo, 0,0},
    {1, 2, 0, nulo, nulo, 0,0},
    {2, 4, 1, nulo, 3, 0,0},
    {3, nulo, nulo, 2, 5, 0,0},
    {4, 7, 2, 11, nulo, 0,0},
    {5, 6, nulo, 3, nulo, 0,0},
    {6, 9, 5, nulo, nulo, 1,0},
    {7, 15, 4, nulo, 8, 0,1},
    {8, nulo, nulo, 7, 9, 0,0},
    {9, nulo, 6, 8, 14, 1,0},
    {10, nulo, nulo, nulo, 12, 2,0},//sala da chave
    {11, nulo, 13, nulo, 4, 1,1},
    {12, nulo, nulo, 10, 13, 1,0},
    {13,11, nulo, 12,nulo, 0,0},
    {14, nulo, nulo, 9, nulo, 3,0},// Sala do baú
    {15, 16, 7, nulo, nulo, 5,0},
    {16, 17, 15, nulo, nulo, 1,0},
    {17, nulo, 16, 18, nulo, 0,1},
    {18, nulo, 17, nulo, nulo, 4,0},// Sala do boss final
};

int main(void)
{
    printf("\n");
    printf("Digite seu nome : ");
    scanf("%s", s1.myname);
    delay(4);
    mov_salas();
    return 0;
}

/**
 * @brief Essa função é um timer que aguarda, em segundos, o valor de seu parametro para posteriormente continuar o código
*/
void delay(int seconds)
{ 
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
        printf("	  Poções : %i\n", s1.mypotion);
        printf("	  Bônus de dano : %i\n", s2.esp_buff);
        printf("	  Defesa : %i", s1.prot_am);
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
            inm -= (dealed_dmg+s2.esp_buff);
            printf("\n═══════════════════════════════════════\n");
            printf("\n     ⚔ Você causou %i de dano ! ⚔   \n", dealed_dmg);
        }

        dano_inm = rand() % Dmg;
        dano_inm -= s1.prot_am;
        geren_vida(dano_inm * -1);

        printf(" \n     ☠  Você tomou %i de dano ! ☠  \n", dano_inm);
        printf("\n═══════════════════════════════════════");

        delay(4);

        if (inm <= 0)
        {
			printf("\n");
            printf("\n═══════════════════════════════════════");
            printf("\n ᕙ( ͡❛ ͜ʖ ͡❛)ᕗ Você matou o esqueleto ");
            printf("\n═══════════════════════════════════════\n");
            delay(4);
            batalha_on = 0;
        }
    } while (batalha_on == 1);
}

/**
 * @brief Funciona de forma similar a função batalha, mas é a última batalha do jogo.
*/
void batalha_boss(int Dmg, int inm){
     batalha_on = 1;
    srand(time(NULL)); // Função que reinicia a função rand()
    do
    {

        printf("\n");
        printf("\e[1;1H\e[2J"); // limpa tela

        printf("\n════════════════════════════════════════");
        printf("\n	  %s - HP: %d\n", s1.myname, s1.mylife);
        printf("	  Poções : %i\n", s1.mypotion);
        printf("	  Bônus de dano : %i\n", s2.esp_buff);
        printf("	  Defesa : %i", s1.prot_am);
        printf("\n════════════════════════════════════════");

        printf("\n _                                    _\n");
        printf("(_'----------------------------------'_)\n");
        printf("(_.==================================._)\n");
        printf("\n 	💀Armadura viva - HP: : %i", inm);
        printf("\n _                                    _\n");
        printf("(_'----------------------------------'_)\n");
        printf("(_.==================================._)");

        if (acoe_per() == 1)
        {
            dealed_dmg = rand() % (s2.swr_dmg + 1);
            inm -= (dealed_dmg+s2.esp_buff);
            printf("\n═══════════════════════════════════════\n");
            printf("\n     ⚔ Você causou %i de dano ! ⚔   \n", dealed_dmg);
        }

        dano_inm = rand() % Dmg;
        dano_inm -= s1.prot_am;
        geren_vida(dano_inm * -1);

        printf(" \n     ☠  Você tomou %i de dano ! ☠  \n", dano_inm);
        printf("\n═══════════════════════════════════════");

        delay(4);

        if (inm <= 0)
        {
			printf("\n");
            printf("\n═══════════════════════════════════════");
            printf("\n ᕙ( ͡❛ ͜ʖ ͡❛)ᕗ Você matou A armadura viva! ");
            printf("\n                  Você venceu !                    ");
            printf("\n═══════════════════════════════════════\n");
            delay(4);
            exit(0);
        }
    } while (batalha_on == 1);
}

// Daqui para baixo são as mecânicas da salas

/**
 * @brief Aqui será a forma pela qual o jogador se "movimentará" pela matriz
*/
int mov_salas(){

    do
    {
    printf("\e[1;1H\e[2J");
    printf(" ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ _  \n");
    printf("_|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|/\n");
    printf("\n");
    desc_salas(pos_atual);
    printf("\n");
    printf(" ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ _  \n");
    printf("_|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|___|/\n");
    printf("\n");
    delay(7);
		
        /**
		 * Essa primeira cadeia de ifs diz respeito a 6 coluna da matriz onde cada um deles categoriza um evento diferente 
        */

		if (matriz[pos_atual][5] == 1){
            batalha(5, 50);
            if (batalha_on == 0)
            {
                matriz[pos_atual][5] = 0;
            }
            }else{
            if (matriz[pos_atual][5] == 2){
             
                if(enigma_chave() == 1){
                    chave = 1;
                }
            
            }else{
                if(matriz[pos_atual][5] == 3){
                    abrir_bau();
                    }
                }
            }

       printf("\n");

        for (int i = 1; i < 5; i++){
            if ((matriz[pos_atual][i] != nulo)){
                rosa_ventos(i);
                printf("Você pode ir para o %s  \n", rosa_v);
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
    printf("\n\rVocê quer ir para qual direção ? \n");
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

/**
 * @brief Essa função verifica se o personagem tem a chave, se ele tiver ele consegue abrir o baú
*/
void abrir_bau(){
    if (chave == 1)
    {
        printf("\n══════════════════════════════════════════════════════════════\n");
        printf("Usando a chave que você coletou, você conseguiu abrir o baú\n");
        printf("e nele tinha uma espada melhor e um amuleto\n");
        printf("ao colocar o amuleto você tem a sensação de que está mais resistente (+3 defesa)");
        printf("\n══════════════════════════════════════════════════════════════");
        s2.esp_buff = 5;
        s1.prot_am = 3;
    }
    
}

/**
 * @brief Nessa função está contido todo o enigma que é um jogo de pergunta e resposta simples 
*/
int enigma_chave(){
    
    const char respostaCorreta[] = "silencio";
    char respostaUsuario[10];
    int numTentativas = 5;
    
    printf("====================================================\n");
    printf("É tão frágil que, só de mencioná-lo, ele se quebra.\n");
    printf("====================================================\n");
    printf("Você tem 5 tentativas para acertar a palavra correta.\n");

    do {
        printf("Digite a palavra (sem acentuação): ");
        scanf("%9s", respostaUsuario);
        for (unsigned int i = 0; i < strlen(respostaUsuario); i++) {
            respostaUsuario[i] = tolower(respostaUsuario[i]);
        }

        if (strcmp(respostaUsuario, respostaCorreta) == 0) {
            printf("\e[1;1H\e[2J");
            delay(3);
            printf("========================================================================\n");
            printf("Se em voz alta a palavra chave recitou, uma chave para ti,se revelou!\n");
            printf("========================================================================\n");
            printf("\n════════════════════════════════════════");
            printf("\n	  %s você recebeu uma chave ! \n", s1.myname);
            printf("\n════════════════════════════════════════");

            return 1;
        } else {
            numTentativas--;
            if (numTentativas > 0) {
                printf("Resposta incorreta. Você tem %d tentativas restantes.\n", numTentativas);
                
            }
        }
    } while (numTentativas > 0);
    printf("Uma maldição caiu sobre você. Você está com 1 de HP, mas você recebeu a chave\n");
        s1.mylife = 1;
        chave = 1;
    printf("\n════════════════════════════════════════");
    printf("\n	  %s - HP: %d\n", s1.myname, s1.mylife);
    printf("\n════════════════════════════════════════");
    return 0;
}

/**
 * @brief Essa função descreve o conteúdo de todas as 19 salas do jogo
 * @param id_sala referente a primeira coluna da matriz
*/
void desc_salas(int id_sala){
    switch (id_sala)
{
case 0:
    printf("Ao abrir a porta da entrada dessa masmorra nas profundezas de uma caverna, você se depara com um velho salão de entrada tomado pelo tempo \n");
    printf("com teias de aranha cobrindo móveis que outrora poderiam dar brilho a aquele lugar e agora fazem parte de um cenário sombrio e silencioso \n");
    break;

case 1:
    printf("O chão aqui é irregular e quebradiço, tornando difícil caminhar com segurança.\n");
    printf("Além da penumbra você tem a impressão de que existe uma presença opressiva e sombria lhe observando.\n");
    break;

case 2:
    printf("Ainda envolto pela penumbra, você começa a olhar com mais atencção ao redor da sala.\n");
    printf("Você percebe que há alguns objetos espalhados pelo chão, como pedaços de madeira, restos de tecidos e ossos. Alguns deles cobertos de mofo e parecem estar ali a muito tempo.\n");
    break;

case 3:
    printf("Enquanto você examinava os objetos, é possível ouvir ruídos de ratos vindo de algum lugar .\n");
    printf("Depois de passar um tempo nessa sala escura, você tem a sensação de que algo o observa na penumbra, deixando-o tenso e alerta.\n");
    break;

case 4:
    printf("Diferente da sala anterior, essa é iluminada por tochas e velas, que revelam os detalhes do local.\n");
    printf("existe um pequeno altar a oeste da sala completamente destruido e pilhas de cinzas estão espalhadas por todo o lugar\n");  
    printf("O ar é mais quente e úmido, e o cheiro de mofo e podridão dá lugar a um forte odor de cinzas.\n");
    break;

case 5:
    printf("Ao entrar você percebe a existência de diversas estantes, ao investigar mais de perto, percebe que \n");
    printf("Os vários mantimentos guardados ali já estão podres a muito tempo\n");
    break;

case 6:
    printf("Essa sala possui apenas algumas mesas e cadeiras velhas, provavelmente era uma zona de repouso \n");
    if(matriz[pos_atual][5] == 1){
        printf("Assim que entra, uma pilha de ossos começa a se levantar e ganhar forma... O esqueleto parece ter sido um aventureiro, pois está usando equipamentos e armaduras antigas.\n");
        printf("Você se prepara para a batalha!\n"); 
    }
    break;

case 7:
    
    printf("Nesta sala, encontra-se uma pequena poço de água no canto da sala. A água está meio turva e reflete a fraca luz emitida pelas tochas.\n");
    if(matriz[pos_atual][6] == 1){
        printf("Você decide investigar a poça e percebe que há um fraco brilho no fundo dela.\n");
        delay(4);
        printf("Você encontrou uma poção!");
        printf("\n═══════════════════════════════════════");
        printf("\n              Porção +1");
        printf("\n═══════════════════════════════════════\n");
        s1.mypotion += 1;
        matriz[pos_atual][6] = 0;
        }
    break;

case 8:
    printf("Chegando aqui, você se depara com uma sala ampla e escura, iluminada apenas por tochas espalhadas pelas paredes.\n");
    printf("Há um corredor estreito que leva a uma porta de madeira maciça no final da sala.\n");
    break;

case 9:
    printf("Ao entrar você percebe que é apenas uma sala vazia com madeiras velhas jogadas em locais pelo chão \n");
    if(matriz[pos_atual][5] == 1){
        printf("Andando um pouco na sala, você é surpreendido por um barulho alto vindo de uma das laterais.\n");
        printf("Você se vira e vê um esqueleto furioso, armado com uma lança, correndo em sua direção!");
    }
    break;

case 10:
    if(chave == 0){
    printf("Neste lugar, você percebe que atras de você se fechou sozinha. Em pânico você busca uma saída, mas a única porta disponível está trancada\n");
    printf("Desesperado, você nota uma inscrição \"Só sairá da sala ao resolver o enigma\"\n");
    printf("Gravado na parede existe o texto: É tão frágil que, só de mencioná-lo, ele se quebra.\n");
    }else{
        printf("Após resolvido o enigma agora é apenas uma sala vazia \n");
    }
    break;
case 11:
    // printf("Após ter solucionado o enigma, você consegue avançar para a próxima sala. A sensação de estar sendo observado é maior aqui...");
    printf("A sala é iluminada por tochas e que reluzem a luz de cristais em um pequeno altar\n"); 
    if(matriz[pos_atual][6] == 1){
        
        printf("ao examina-lo você encontra uma poção.");
        printf("\n═══════════════════════════════════════");
        printf("\n              Porção +1");
        printf("\n═══════════════════════════════════════\n");
        printf("Entretanto ao pegar a poção um esqueleto surge de um ponto cego da sala !");
        delay(1);
        s1.mypotion += 1;
        matriz[pos_atual][6] = 0;
    }
    break;

case 12:
    printf("A sala é escura e mal iluminada, e há um cheiro forte e desagradável no ar.\n");
    if(matriz[pos_atual][5] == 1){
        printf("Ao examinar a sala você se depara com uma imagem grotesca, é um esqueleto ainda com partes da velha carcaça humana! \n");
    }
    break;

case 13:
    printf("Você se depara com uma ponte antiga que te levará até o outro lado. \n");
    break;

case 14:// Sala do baú
    printf("Nessa sala, há um baú no canto coberto de poeira e teias de aranha. \n");
    if(chave == 0){
        printf("O baú está trancado.\n");
    }else{
        printf("Você já o abriu ");
    }
    break;

case 15:
    if(matriz[pos_atual][5] == 5){    
        printf("Você sente uma presença de imponência ameaçadora. \n");
        printf("A atmosfera se torna tensa e se pode ouvir ruídos estranhos vindos da porta como se algo estivesse movendo lá dentro.\n");
        printf("Você cria coragem e destemidamente adentra a sala. E....\n");
        delay(4);
        printf("Ela está vazia...\n");
    matriz[pos_atual][5] = 0;
    }    
    break;

case 16:
    printf("As paredes do local possuem velhas correntes, cheias de ferrugem com uma pilha de ossos abaixo.\n");
    printf("Você tem a impressão de que elas deveriam estar acorrentando algum prisioneiro\n");
    if(matriz[pos_atual][5] == 1){ 
        printf("Dessa pilha surge um esqueleto que avança na sua direção !\n");
    }
    break;

case 17:
    if(matriz[pos_atual][6] == 1){
        printf("A sala possui uma espécie de \"Quebra-Cabeças\" antigo. \n");
        printf("Rapidamento você o monta e um mecanismo o libera uma poção.");
        printf("\n═══════════════════════════════════════");
        printf("\n              Porção +1");
        printf("\n═══════════════════════════════════════\n");
        s1.mypotion += 1;
        matriz[pos_atual][6] = 0;
    }else{
        printf("Agora que o \"Quebra-Cabeças\" está montado não resta nada de interessante nessa sala\n");
    }
    break;
case 18:
    printf("Ao abrir as grandes portas dessa sala, você se depara com um grande salão vazio com um trono em seu fim, \n");
    printf("Sentado ao trono você observa uma armadura em uma posição relaxada completamente imóvel\n");
    printf("Repentinamente surge uma centelha de coloração aroxeada bem escura partindo de dentro do elmo da armadura \n");
    printf("Que logo toma a forma de uma chama de mesma cor que ocupa as articulações da armadura");
    printf("Após esse rápido acontecimento a porta atrás de você fecha-se rapidamente e você vê aquela armadura, agora \"viva\", de pé olhando no fundo da sua alma\n");
    printf("Ela corta o ar e dele começa a surgir uma lamina completamente escura que ela agarra e parte em sua direção.\n"); 
    printf("Não existe mais volta, morrendo aqui niguém ouvirá seu último suspiro .\n");
    delay(10);
    batalha_boss(10,150);
default:
    break;
}
}


