



#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
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
        for (int i = 0; i < strlen(respostaUsuario); i++) {
            respostaUsuario[i] = tolower(respostaUsuario[i]);
        }

        if (strcmp(respostaUsuario, respostaCorreta) == 0) {
            printf("========================================================================\n");
            printf("Se em voz alta a palavra chave recitou, uma chave para ti, te retornou!\n");
            printf("========================================================================\n");

            return 0;
        } else {
            numTentativas--;
            if (numTentativas > 0) {
                printf("Resposta incorreta. Você tem %d tentativas restantes.\n", numTentativas);
            }
        }
    } while (numTentativas > 0);

    printf("Uma maldição caiu sobre você.\n");
    return 0;
}



