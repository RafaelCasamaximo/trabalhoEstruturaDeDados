#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "trataString.h"
#include "leituraGeo.h"
#include "desenhaSvg.h"
#include "leituraQry.h"
#include "padrao.h"

int main(int argc, char *argv[])
{
    int i = 1;

    No* lista = NULL;
    No* listaQry = NULL;

    char *entryPath = NULL;
    char *geoPath = NULL;
    char *queryPath = NULL;
    char *outPath = NULL;
    char *caminhoConcatenado = NULL;
    char *caminhoConcatenadoSaida = NULL;
    char *caminhoConcatenadoQuery = NULL;
    char *caminhoConcatenadoGeoQry = NULL;
    char *caminhoConcatenadoLogQry = NULL;
    char *nomeArquivoGeo = NULL;
    char *nomeArquivo = NULL;
    char *nomeArquivoQuery = NULL;
    char *nomeArquivoQuerySvg = NULL;
    char *nomeArquivoLogTxt = NULL;

    //Lê os argumentos passados para o executável
    while (i < argc){
        //siguel [-e path] -f arq.geo [-q consulta.qry] -o dir
        printf("(%d) -> %s", i, argv[i]);

        //Vê se o usuário inseriu um parametro para especificar o local da entrada de arquivos
        if (strcmp("-e", argv[i]) == 0){
            i++;

            if (argv[i] == NULL)
            { //Encerra o programa se '-e' não for seguido do local do arquivo.
                printf("\n!Erro! Sem parametros em -e");
                printf("\n '-e' sem diretorio.");
                exit(1);
            }
            printf(" O parâmetro foi inserido. (%s)\n", argv[i]);
            entryPath = (char *)malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(entryPath, argv[i]);
        }

        //Vê se o usuário inseriu um parametro para especificar o arquivo da geografia
        else if (strcmp("-f", argv[i]) == 0){
            i++;

            if (argv[i] == NULL){
                //Encerra o programa se '-f' não for seguido do local do arquivo.
                printf("\n!Erro! Sem parametros em -f");
                printf("\n'-f' sem arquivo");
                exit(1);
            }
            printf(" O parâmetro foi inserido. (%s)\n", argv[i]);
            geoPath = (char *)malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(geoPath, argv[i]);
        }

        //Vê se o usuário inseriu um parametro para especificar o arquivo de consulta
        else if (strcmp("-q", argv[i]) == 0){
            i++;

            if (argv[i] == NULL){
                //Encerra o programa se '-q' não for seguido do local do arquivo.
                printf("\n!Erro! Sem parametros em -q");
                printf("\n'-q' sem arquivo");
                exit(1);
            }
            printf(" O parâmetro foi inserido. (%s)\n", argv[i]);
            queryPath = (char *)malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(queryPath, argv[i]);
        }

        //Vê se o usuário inseriu um parametro para especificar o local dos arquivos de saída
        else if (strcmp("-o", argv[i]) == 0){
            i++;

            if (argv[i] == NULL){
                //Encerra o programa se '-o' não for seguido do diretorio.
                printf("\n!Erro! Sem parametros em -o");
                printf("\n'-o' sem diretorio");
                exit(1);
            }
            printf(" O parâmetro foi inserido. (%s)\n", argv[i]);
            outPath = (char *)malloc((strlen(argv[i]) + 1) * sizeof(char));
            strcpy(outPath, argv[i]);
        }
        i++;
    }

    if (geoPath == NULL || outPath == NULL){
        printf("\n!Erro! Nao existe -f ou -o. Esses parâmetros são essenciais.");
        exit(1);
    }

    //Se ele chegar até aqui significa que existe os parametros essenciais.
    //Le o .geo e monta uma lista com ele
    concatenaCaminhos(entryPath, geoPath, &caminhoConcatenado);
    lista = pegaDadosGeo(lista, caminhoConcatenado);

    // //Constroi o svg com base na lista feita na func anteiror
    getNomeConcatExtension(geoPath, ".svg", &nomeArquivo);
    concatenaCaminhos(outPath, nomeArquivo, &caminhoConcatenadoSaida);
    lista = desenhaSvg(lista, caminhoConcatenadoSaida);

    if(queryPath != NULL){
        concatenaCaminhos(entryPath, queryPath, &caminhoConcatenadoQuery);

        extraiNome(queryPath, &nomeArquivoQuery);
        extraiNome(geoPath, &nomeArquivoGeo);
        concatenaNomeGeoQry(nomeArquivoGeo, nomeArquivoQuery, ".svg", &nomeArquivoQuerySvg);
        concatenaNomeGeoQry(nomeArquivoGeo, nomeArquivoQuery, ".txt", &nomeArquivoLogTxt);
        printf("%s", nomeArquivoQuerySvg);
        concatenaCaminhos(outPath, nomeArquivoQuerySvg, &caminhoConcatenadoGeoQry);
        concatenaCaminhos(outPath, nomeArquivoLogTxt, &caminhoConcatenadoLogQry);
        listaQry = pegaDadoQry(listaQry, &lista, caminhoConcatenadoQuery, caminhoConcatenadoLogQry);
        listaQry = desenhaSvgQry(lista, listaQry, caminhoConcatenadoGeoQry);
    }


    if(entryPath != NULL){
        free(entryPath);
    }
    if(geoPath != NULL){
       free(geoPath);
    }
    if(queryPath != NULL){
        free(queryPath);
    }
    if(outPath != NULL){
        free(outPath);
    }
    if(caminhoConcatenado != NULL){
        free(caminhoConcatenado);
    }
    if(nomeArquivo != NULL){
       free(nomeArquivo); 
    }
    if(caminhoConcatenadoSaida != NULL){
        free(caminhoConcatenadoSaida);
    }
    if(caminhoConcatenadoQuery != NULL){
        free(caminhoConcatenadoGeoQry);
    }
    if(nomeArquivoGeo != NULL){
        free(nomeArquivoGeo);
    }
    if(nomeArquivoQuery != NULL){
        free(nomeArquivoQuery);
    }
    if(caminhoConcatenadoQuery != NULL){
        free(caminhoConcatenadoQuery);
    }
    if(nomeArquivoQuerySvg != NULL){
        free(nomeArquivoQuerySvg);
    }
    if(caminhoConcatenadoLogQry != NULL){
        free(caminhoConcatenadoLogQry);
    }
    if(nomeArquivoLogTxt != NULL){
        free(nomeArquivoLogTxt);
    }
    listaQry = delLista(listaQry);
    lista = delLista(lista);

    return 0;
}