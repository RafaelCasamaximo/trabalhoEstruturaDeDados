#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "leituraQry.h"
#include "list.h"
#include "padrao.h"

No* pegaDadoQry(No* listaQry, No* lista, char* path){
    FILE* qry = NULL;
    qry = fopen(path, "r");
    if(qry == NULL){
        printf("ERRO! Não foi possivel abrir o arquivo! O QRY não será considerado!");
        exit(1);
    }

    int j = 0, k = 0, id = 0;
    float x = 0, y = 0, w = 0, h = 0, xc = 0, yc = 0;
    char cb[22], cp[22], comando[6];
    int resultado = 0, resultadoPegaCoordenadas = 0, resultadoCentroDeMassa = 0;

    while(1){
        fscanf(qry, "%s", comando); //vai pra proxima linha automaticamente?
        if(feof(qry)){
            break;
        }
        if(strcmp(comando, "o?")){
            fscanf(qry, "%d %d", &j, &k);
            resultado = sobrepoe(lista, j, k);
            //Add elem
            listaQry = addElem(listaQry, id, 'd');
            //Aqui falta eu pegar as coordenadas para desenhar o retangulo
            resultadoPegaCoordenadas = pegaCoordanadas(lista, j, k, &x, &y, &w, &h);
            if(resultado == 1){
                //Ret cheio
                listaQry = addRT(listaQry, id, x, y, w, h, 1);
            }
            else{
                //Ret tracejado
                listaQry = addRT(listaQry, id, x, y, w, h, 0);
            }
        }
        if(strcmp(comando, "i?")){
            fscanf(qry, "%d %f %f", &j, &x, &y);
            resultado = contem(lista, j, x, y);
            //add Elem Ponto
            listaQry = addElem(listaQry, id, 'p');
            //Soma id++
            id++;
            //add Elem Linha
            listaQry = addElem(listaQry, id, 'l');
            //Busca o centro de massa do elemento J
            resultadoCentroDeMassa = pegaCentro(lista, j, &xc, &yc);
            if(resultado == 1){
                //Ponto azul
                listaQry = addL(listaQry, id-1, xc, yc, x, y, 1);
                //Linha azul
                listaQry = addP(listaQry, id, xc, yc, 1);
            }
            else{
                //Ponto magenta
                listaQry = addL(listaQry, id-1, xc, yc, x, y, 0);
                //Linha magenta
                listaQry = addP(listaQry, id, xc, yc, 0);
            }
        }
        if(strcmp(comando, "pnt")){
            fscanf(qry, "%d %s %s", &j, cb, cp);
            resultado = paint(lista, j, cb, cp);
        }
        if(strcmp(comando, "pnt*")){
            fscanf(qry, "%d %d %s %s", &j, &k, cb, cp);
            resultado = paintN(lista, j, k, cb, cp);
        }
        if(strcmp(comando, "delf")){
            fscanf(qry, "%d", &j);
            resultado = dElem(lista, j);
        }
        if(strcmp(comando, "delf*")){
            fscanf(qry, "%d %d", &j, &k);
            resultado = dElemN(lista, j, k);
        }
        id++;
    }
    fclose(qry);
    return listaQry;
}

int sobrepoe(No* lista, int j, int k){
    No* auxJ = NULL;
    No* auxK = NULL;

    //Verificações
    auxJ = buscaElem(lista, j);
    if(auxJ == NULL){
        printf("\nNão é possivel aplicar o algoritmo de sobreposição.\nO Elemento com o ID J não foi encontrato e/ou não existe!\n");
        return 0;
    }
    auxK = buscaElem(lista, k);
    if(auxK == NULL){
        printf("\nNão é possivel aplicar o algoritmo de sobreposição.\nO Elemento com o ID K não foi encontrato e/ou não existe!\n");
        return 0;
    }
    //Verifica se nenhum dos dos elementos são circulos e/ou retangulos
    if((auxJ->tipo != 'c' && auxJ->tipo != 'r') || (auxK->tipo != 'c' && auxK->tipo != 'r')){
        printf("A verificação de sobreposição só é possível para círculos e retangulos!\n");
        return 0;
    }

    //retangulo e retangulo
    if(auxJ->tipo == 'r' && auxK->tipo == 'r'){
        //ret a
        float jx = auxJ->fig->r.x;
        float jy = auxJ->fig->r.y;
        float jw = auxJ->fig->r.w;
        float jh = auxJ->fig->r.h;

        float kx = auxK->fig->r.x;
        float ky = auxK->fig->r.y;
        float kw = auxK->fig->r.w;
        float kh = auxK->fig->r.h;

        //x,y
        if(jx <= kx && kx <= (jx+jw)){
            if(jy <= ky && ky <= (jy+jh)){
                return 1;
            }
        }
        //x+w, y
        if(jx <= kx+kw && kx+kw <= (jx+jw)){
            if(jy <= ky && ky <= (jy+jh)){
                return 1;
            }
        }
        //x, y+h
        if(jx <= kx && kx <= (jx+jw)){
            if(jy <= ky+kh && ky+kh <= (jy+jh)){
                return 1;
            }
        }
        if(jx <= kx+kw && kx+kw <= (jx+jw)){
            if(jy <= ky+kh && ky+kh <= (jy+jh)){
                return 1;
            }
        }
        return 0;
    }

    //circulo e circulo
    if(auxJ->tipo == 'c' && auxK->tipo == 'c'){
        if(distanciaQuadrada(auxJ->fig->c.x, auxJ->fig->c.y, auxK->fig->c.x, auxK->fig->c.y) <= ((auxJ->fig->c.r + auxK->fig->c.r) * (auxJ->fig->c.r + auxK->fig->c.r))){
            return 1;
        }
        return 0;
    }

    //retangulo e circulo
    if((auxJ->tipo == 'r' && auxK->tipo == 'c') || (auxJ->tipo == 'c' && auxK->tipo == 'r')){
        No* circulo = NULL;
        No* retangulo = NULL;
        if(auxJ->tipo == 'r'){
            circulo = auxJ;
            retangulo = auxK;
        }
        else{
            circulo = auxK;
            retangulo = auxJ;
        }
        float dX = circulo->fig->c.x - max(retangulo->fig->r.x, min(circulo->fig->c.x, retangulo->fig->r.x + retangulo->fig->r.w));
        float dY = circulo->fig->c.y - max(retangulo->fig->r.y, min(circulo->fig->c.y, retangulo->fig->r.y + retangulo->fig->r.h));
        if((dX * dX + dY * dY) <= (circulo->fig->c.r * circulo->fig->c.y)){
            return 1;
        } 
        return 0;
    }
    return 0;
}

int contem(No* lista, int j, float x, float y){
    No* auxJ = NULL;
    auxJ = buscaElem(lista, j);
    if(auxJ == NULL){
        printf("\nNão é possivel aplicar o algoritmo de intersecção.\nO Elemento com o ID J não foi encontrato e/ou não existe!\n");
        return 0;
    }
    if(auxJ->tipo == 'r'){
        float xr = auxJ->fig->r.x;
        float yr = auxJ->fig->r.y;
        float wr = auxJ->fig->r.w;
        float hr = auxJ->fig->r.h;
        if(((xr <= x) && (x <= (xr + wr))) && ((yr <= y) && (y <= (yr + hr)))){
            return 1;
        }
    }
    if(auxJ->tipo == 'c'){
        float dX = auxJ->fig->c.x - x;
        float dY = auxJ->fig->c.y - y;
        if(((dX * dX) + (dY * dY)) <= (auxJ->fig->c.r * auxJ->fig->c.r)){
            return 1;
        }
    }
    else{
        printf("O tipo de elemento é invalido na comparação!");
    }
    return 0;
}


int paint(No* lista, int j, char cb[], char cp[]){
    No *aux = buscaElem(lista, j);
    if(aux == NULL){
        printf("\n\tNao foi possivel encontrar o elemento J. ID: %d!", j);
        return 0;
    }

    if(aux->tipo == 'c'){
        strcpy(aux->fig->c.corb, cb);
        strcpy(aux->fig->c.corp, cp);
    }

    else if(aux->tipo == 'r'){
        strcpy(aux->fig->r.corb, cb);
        strcpy(aux->fig->r.corp, cp);
    }

    else if(aux->tipo == 't'){
        strcpy(aux->fig->t.corb, cb);
        strcpy(aux->fig->t.corp, cp);
    }
    
    return 1;
}

int paintN(No* lista, int j, int k, char cb[], char cp[]){
    int menor, maior;
    maior = max(j, k);
    menor = min(j, k);

    for(int i = menor; i <= maior; i++){
        paint(lista, i, cb, cp);
    }
    return 1;
}

int dElem(No* lista, int j){
    lista = delElem(lista, j);
    return 1;
}

int dElemN(No* lista, int j, int k){
    int menor, maior;
    maior = max(j, k);
    menor = min(j, k);

    for(int i = menor; i <= maior; i++){
        dElem(lista, i);
    }
    return 1;
}

int pegaCoordanadas(No* lista, int j, int k, float* x, float* y, float* w, float* h){
    float vX[4], vY[4];
    No* auxJ = NULL;
    No* auxK = NULL;
    auxJ = buscaElem(lista, j);
    if(auxJ == NULL){
        printf("Não foi possivel encontrar o elemento J para extrair coordenadas!");
        return 0;
    }
    auxK = buscaElem(lista, k);
    if(auxJ == NULL){
        printf("Não foi possivel encontrar o elemento K para extrair coordenadas!");
        return 0;
    }
    if(auxJ->tipo == 'r'){
        vX[0] = auxJ->fig->r.x;
        vX[1] = auxJ->fig->r.x + auxJ->fig->r.w;
        vY[0] = auxJ->fig->r.y;
        vY[1] = auxJ->fig->r.y + auxJ->fig->r.h;;
    }
    if(auxJ->tipo == 'c'){
        vX[0] = auxJ->fig->c.r + auxJ->fig->c.x;
        vX[1] = auxJ->fig->c.r - auxJ->fig->c.x;
        vY[0] = auxJ->fig->c.r + auxJ->fig->c.y;
        vY[1] = auxJ->fig->c.r - auxJ->fig->c.y;
    }
    else{
        printf("O tipo de elemento do J não é válido! (%c)", auxJ->tipo);
        return 0;
    }

    if(auxK->tipo == 'r'){
        vX[2] = auxK->fig->r.x;
        vX[3] = auxK->fig->r.x + auxK->fig->r.w;
        vY[2] = auxK->fig->r.y;
        vY[3] = auxK->fig->r.y + auxK->fig->r.h;
    }
    if(auxK->tipo == 'c'){
        vX[2] = auxK->fig->c.r + auxK->fig->c.x;
        vX[3] = auxK->fig->c.r - auxK->fig->c.x;
        vY[2] = auxK->fig->c.r + auxK->fig->c.y;
        vY[3] = auxK->fig->c.r - auxK->fig->c.y;
    }
    else{
        printf("O tipo de elemento do K não é válido! (%c)", auxK->tipo);
        return 0;
    }

    *x = minV(vX, 4);
    *y = minV(vY, 4);
    *w = maxV(vX, 4) - *x;
    *h = maxV(vY, 4) - *y;
    return 1;
}

int pegaCentro(No* lista, int j, float* x, float* y){
    No* aux = NULL;
    aux = buscaElem(lista, j);
    if(aux == NULL){
        printf("Não é possivel encontrar o elemento para calcular o centro de massa! (%c)", aux->tipo);
        return 0;
    }
    if(aux->tipo == 'c'){
        *x = aux->fig->c.x;
        *y = aux->fig->c.y;
    }
    if(aux->tipo == 'r'){
        *x = aux->fig->r.x + (aux->fig->r.w / 2);
        *y = aux->fig->r.y + (aux->fig->r.h / 2);
    }
    else{
        printf("O tipo do elemento é inválido! Não é possivel calcular o centro de massa desse elemento!");
        return 0;
    }
    return 1;
}