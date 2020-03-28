#ifndef FUNCOESESTRUTURA_H_INCLUDED
#define FUNCOESESTRUTURA_H_INCLUDED

#include "meuconio.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <cType.h>
#include <dirent.h>

#include "funcoesDBase.h"

void cmdDbase(char cmd, char *und, DArquivos *aberto, int rec)
{
    int xa = x, ya = y + 2;
    char aux[15], nome[21];
    if(aberto == NULL)
        strcpy(nome, "NONE");
    else
        strcpy(nome, aberto->NomeDBF);

    if(cmd == 'A')
        strcpy(aux, "APPEND");
    else if(cmd == 'C')
        strcpy(aux, "CREATE");
    else if(cmd == 'E')
        strcpy(aux, "EDIT");
    else
        strcpy(aux, "Command Line");

    gotoxy(xa, ya - 1), printf("                                                                                                                      \n");
    gotoxy(xa, ya), printf("                                                                                                                      ");
    gotoxy(xa, ya+1), printf("                                                                                                                      ");
    textbackground(7);
    textcolor(0);
    gotoxy(xa, ya);
    printf("%s\t\t||<%s>||%s\t\t|REC: %d\t\t\t  ", aux, und, nome, rec);
    textbackground(0);
    textcolor(7);
    gotoxy(45, ya + 1), printf("Enter a dBASE III PLUS command.");
}

void setArqInfo(char nome_arq[], DArquivos *info_arq)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);    //Pega Hora do sistema

    strcpy(info_arq->NomeDBF, nome_arq);
    sprintf(info_arq->Hora,"%d:%d", timeinfo->tm_hour, timeinfo->tm_min);
    sprintf(info_arq->Data,"%d/%d/20%d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year - 100);
}

DArquivos *criaNDArq(DArquivos infos)
{
    DArquivos *nova_caixa = (DArquivos*)malloc(sizeof(DArquivos));

    nova_caixa->Ant = nova_caixa->Prox = NULL;
    nova_caixa->Campos = NULL;
    nova_caixa->Status = NULL;
    strcpy(nova_caixa->Data, infos.Data);
    strcpy(nova_caixa->Hora, infos.Hora);
    strcpy(nova_caixa->NomeDBF, infos.NomeDBF);

    return nova_caixa;
}

CArquivos *criaNCArq(CArquivos infos)
{
    CArquivos *nova_caixa = (CArquivos*)malloc(sizeof(CArquivos));

    nova_caixa->Prox = NULL;
    nova_caixa->Patual = NULL;
    nova_caixa->Pdados = NULL;
    strcpy(nova_caixa->FieldName, infos.FieldName);
    nova_caixa->Type = infos.Type;
    nova_caixa->Width = infos.Width;
    nova_caixa->Dec = infos.Dec;

    return nova_caixa;
}

TDados *criaDados(TDados d, char t)
{
    TDados *nova_caixa = (TDados*)malloc(sizeof(TDados));
    switch(t)
        {
            case 'N':
                nova_caixa->d.ValorN = d.d.ValorN;
            break;
            case 'D':
                nova_caixa->d.ValorD = (char*)malloc(sizeof(char) * 10);
                strcpy(nova_caixa->d.ValorD, d.d.ValorD);
            break;
            case 'L':
                nova_caixa->d.ValorL = d.d.ValorL;
            break;
            case 'C':
                nova_caixa->d.ValorC = (char*)malloc(sizeof(char) * 50);
                nova_caixa->d.ValorC = d.d.ValorC;
            break;
            case 'M':
                nova_caixa->d.ValorM = (char*)malloc(sizeof(char) * 50);
                strcpy(nova_caixa->d.ValorM, d.d.ValorM);
            break;
        }
    nova_caixa->Prox = NULL;
    return nova_caixa;
}

void insereTrueFalse(TDados **tipo, TDados *dado)
{
    TDados *aux;
    if(*tipo == NULL)
        *tipo = dado;
    else
    {
        aux = *tipo;
        while(aux->Prox != NULL)
            aux = aux->Prox;
        aux->Prox = dado;
    }
}

void insereDados(CArquivos **tipo, TDados *dado)
{
    TDados *aux;
    if((*tipo)->Pdados == NULL)
    {
        (*tipo)->Pdados = dado;
        (*tipo)->Patual = dado;
    }
    else
    {
        aux = (*tipo)->Pdados;
        while(aux->Prox != NULL)
            aux = aux->Prox;
        aux->Prox = dado;
    }
}

void insereDArq(DArquivos **arq, DArquivos *nova_da)
{
    DArquivos *aux_busca;
    if(*arq == NULL)
        *arq = nova_da; // Verifica se é o primeiro arquivo do diretorio
    else
    {
         // caso nao seja o primeiro, vai ate o fim e insere la
        aux_busca = *arq;
        while(aux_busca->Prox != NULL)
            aux_busca = aux_busca->Prox;
        aux_busca->Prox = nova_da;
        nova_da->Ant = aux_busca;
    }
}

void insereCArq(CArquivos **arq, CArquivos *nova_ca)
{
    CArquivos *aux_busca;
    if(*arq == NULL)
        *arq = nova_ca;
    else
    {
        aux_busca = *arq;
        while(aux_busca->Prox != NULL)
            aux_busca = aux_busca->Prox;
        aux_busca->Prox = nova_ca;
    }
}

void excluiDados(TDados **d)
{
    if(*d != NULL)
    {
        excluiDados(&(*d)->Prox);
        free(*d);
        *d = NULL;
    }
}

void excluiCArq(CArquivos **camp)
{
    if(*camp != NULL)
    {
        excluiCArq(&(*camp)->Prox);
        excluiDados(&(*camp)->Pdados);
        free(*camp);
        *camp = NULL;
    }
}

void createUnidades(Diretorio **Unidade)
{
	Diretorio *NewUnidade = (Diretorio*)malloc(sizeof(Diretorio));
	//Cria a unidade D e já atribui seus valores
	NewUnidade->top = (Diretorio*)malloc(sizeof(Diretorio));
	NewUnidade->top->arqs = NULL;
	NewUnidade->top->bottom = NewUnidade;
	NewUnidade->top->top = NULL;
	strcpy(NewUnidade->top->und,"D:/");
	//Atribui valores para a unidade C
	NewUnidade->arqs = NULL;
	NewUnidade->bottom= NULL;
	strcpy(NewUnidade->und,"C:/");

	(*Unidade) = NewUnidade;
}

void createArquivo(Diretorio **estrutura, char nome_arq[21], FILE *arq, char caminho_arq[])
{
    DArquivos *nova_da, *aux_busca, info_arq;
    CArquivos *nova_ca, *aux_inserir, info_campo;
    int i = 0;
    char carac, aux_str[12];

    arq = fopen(caminho_arq, "wb");
    if(arq == NULL)
    {
        gotoxy(30, y + 4),printf("ERROR\n");
        y += 1;
        cmdDbase('C', &(*estrutura)->und, NULL, 0);
    }

    else
    {
        setArqInfo(nome_arq, &info_arq);
        nova_da = criaNDArq(info_arq);

        fwrite(nova_da, sizeof(DArquivos), 1, arq); //Grava Estrutura de informações do arquivo
        insereDArq(&(*estrutura)->arqs, nova_da);

        x+=5;
        gotoxy(x, y),printf("Field Name"); x += 12;
        gotoxy(x, y),printf("Type"); x += 10;
        gotoxy(x, y),printf("Width"); x += 10;
        gotoxy(x, y++),printf("Dec"); x = 1;
        cmdDbase('C', &(*estrutura)->und, NULL, 0);

        for(x = 6; x < 41; x++)
            gotoxy(x, y), printf("%c", 205);

        x = 1; y++;
        cmdDbase('C', &(*estrutura)->und, NULL, 0);

        i++;
        gotoxy(x, y),printf("%d", i);x += 5;
        fflush(stdin); gotoxy(x, y), gets(info_campo.FieldName);
        x += 12;
        do{
            do
            {
                gotoxy(x, y), carac = getch();
                switch(carac)
                {
                    case 'C':
                        gotoxy(x, y), printf("              ");
                        gotoxy(x, y);
                        printf("Character");
                        info_campo.Type = 'C';
                        break;
                    case 'D':
                        gotoxy(x, y), printf("              ");
                        gotoxy(x, y);
                        printf("Date");
                        info_campo.Type = 'D';
                        break;
                    case 'M':
                        gotoxy(x, y), printf("              ");
                        gotoxy(x, y);
                        printf("Memo");
                        info_campo.Type = 'M';
                        break;
                    case 'N':
                        gotoxy(x, y), printf("              ");
                        gotoxy(x, y);
                        printf("Numeric");
                        info_campo.Type = 'N';
                        break;
                    case 'L':
                        gotoxy(x, y), printf("              ");
                        gotoxy(x, y);
                        printf("Logical");
                        info_campo.Type = 'L';
                        break;
                    case 8:
                        gotoxy(x, y), printf("              ");
                        info_campo.Type = ' ';
                        break;
                    case 13:
                        break;
                    default:
                        gotoxy(x, y), printf("              ");
                        info_campo.Type = ' ';
                        carac = ' ';

                }
                if(info_campo.Type == ' ')
                    carac = ' ';
            }while(carac != 13);
            x += 10;
            while(gotoxy(x, y),scanf("%s", &aux_str), !atoi(aux_str))
            {
                gotoxy(35, y + 4),printf("                                                            ");
                gotoxy(35, y + 4),printf("Width can't be negative, null or greater than 50! Try again ");
                gotoxy(x, y), printf("          ");
            }
            gotoxy(35, y + 4),printf("                                                            ");
            info_campo.Width = atoi(aux_str);
            x += 10;
            gotoxy(x, y), info_campo.Dec = getche();
            while(info_campo.Dec != 13 && (info_campo.Dec < 48 || info_campo.Dec > 57))
            {
                gotoxy(x, y), printf(" ");
                gotoxy(35, y + 4),printf("Only numbers.");
                gotoxy(x, y), info_campo.Dec = getche();
            }
            if(info_campo.Dec == 13)
                info_campo.Dec = ' ';
            gotoxy(35, y + 4),printf("                                                            ");

            y++;
            x = 1;
            cmdDbase('C', &(*estrutura)->und, NULL, 0);

            nova_ca = criaNCArq(info_campo);
            insereCArq(&nova_da->Campos, nova_ca);

            fwrite(nova_ca, sizeof(CArquivos), 1, arq);
            i++;
            gotoxy(x, y),printf("%d", i); x += 5;
            fflush(stdin); gotoxy(x, y), gets(info_campo.FieldName);
            x += 12;
        }while(strcmp(info_campo.FieldName, "") != 0);
        nova_ca = criaNCArq(info_campo);
        fwrite(nova_ca, sizeof(CArquivos), 1, arq);
        fclose(arq);
        x = 1;
    }
}

void leArquivo(FILE *arq, DArquivos **aberto)
{
    CArquivos *novo_campo, info_campo, *aux;
    DArquivos *novo_arq, info_arq;
    TDados *novo_dado, info_dado;
    char aux_str[51];

    fread(&info_arq, sizeof(DArquivos), 1, arq);
    novo_arq = criaNDArq(info_arq);
    *aberto = novo_arq;
    fread(&info_campo, sizeof(CArquivos), 1, arq);
    while(strcmp(info_campo.FieldName, "") != 0)
    {
        novo_campo = criaNCArq(info_campo);
        insereCArq(&novo_arq->Campos, novo_campo);
        fread(&info_campo, sizeof(CArquivos), 1, arq);
    }
    fread(&info_dado.d.ValorL, sizeof(char), 1, arq);
    while(!feof(arq))
    {
        novo_dado = criaDados(info_dado, 'L');
        insereTrueFalse(&novo_arq->Status, novo_dado);
        aux = novo_arq->Campos;
        while(aux != NULL)
        {
            switch(aux->Type)
            {
                case 'N':
                    fread(&info_dado.d.ValorN, sizeof(float), 1, arq);
                break;
                case 'L':
                    fread(&info_dado.d.ValorL, sizeof(char), 1, arq);
                break;
                case 'D':
                    fread(&aux_str[0], sizeof(char), 11, arq);
                    info_dado.d.ValorD = (char*)malloc(sizeof(char) * 11);
                    strcpy(info_dado.d.ValorD, aux_str);
                break;
                case 'C':
                    fread(&aux_str[0], sizeof(char), aux->Width + 1, arq);
                    info_dado.d.ValorC = (char*)malloc(sizeof(char) * 51);
                    strcpy(info_dado.d.ValorC, aux_str);
                break;
                case 'M':
                    fread(&aux_str[0], sizeof(char), 51, arq);
                    info_dado.d.ValorM = (char*)malloc(sizeof(char) * 51);
                    strcpy(info_dado.d.ValorM, aux_str);
                break;
            }
            novo_dado = criaDados(info_dado, aux->Type);
            insereDados(&aux, novo_dado);
            aux = aux->Prox;
        }
        fread(&info_dado.d.ValorL, sizeof(char), 1, arq);
    }
    fclose(arq);
}

void exibeCampos(CArquivos *campo)
{
    char aux[11];
    switch(campo->Type)
    {
        case 'N':
            strcpy(aux, "%.");
            aux[2] = campo->Dec;
            aux[3] = 'f';
            aux[4] = '\0';
            printf(aux, campo->Patual->d.ValorN);
        break;
        case 'L':
            printf("%c", campo->Patual->d.ValorL);
        break;
        case 'M':
            printf("%s", campo->Patual->d.ValorM);
            break;
        case 'C':
            printf("%s", campo->Patual->d.ValorC);
            break;
        case 'D':
            printf("%s", campo->Patual->d.ValorD);
    }
}

void gravaDadosAtualizados(DArquivos *aberto, char *und)
{
    char dir[21], aux_str[51];
    FILE *arq;
    CArquivos *aux_campo, ultimo_campo = {NULL, ""};
    TDados *aux_dado = aberto->Status;

    strcpy(dir, und);
    strcat(dir, "/");
    strcat(dir, aberto->NomeDBF);

    arq = fopen(dir, "wb");
    fwrite(aberto, sizeof(DArquivos), 1, arq);
    aux_campo = aberto->Campos;
    while(aux_campo != NULL)
    {
        aux_campo->Patual = aux_campo->Pdados;
        fwrite(aux_campo, sizeof(CArquivos), 1, arq);
        aux_campo = aux_campo->Prox;
    }
    fwrite(&ultimo_campo, sizeof(CArquivos), 1, arq);
    while(aux_dado != NULL)
    {
        fwrite(&aux_dado->d.ValorL, sizeof(char), 1, arq);
        aux_campo = aberto->Campos;
        while(aux_campo != NULL)
        {
            switch(aux_campo->Type)
            {
                case 'N':
                    fwrite(&aux_campo->Patual->d.ValorN, sizeof(float), 1, arq);
                break;
                case 'L':
                    fwrite(&aux_campo->Patual->d.ValorL, sizeof(char), 1, arq);
                break;
                case 'D':
                    strcpy(aux_str, aux_campo->Patual->d.ValorD);
                    fwrite(&aux_str[0], sizeof(char), 11, arq);
                break;
                case 'M':
                    strcpy(aux_str, aux_campo->Patual->d.ValorM);
                    fwrite(&aux_str[0], sizeof(char), 51, arq);
                break;
                case 'C':
                    strcpy(aux_str, aux_campo->Patual->d.ValorC);
                    fwrite(&aux_str[0], sizeof(char), aux_campo->Width + 1, arq);
                break;
            }
            aux_campo->Patual = aux_campo->Patual->Prox;
            aux_campo = aux_campo->Prox;
        }
        aux_dado = aux_dado->Prox;
    }
    fclose(arq);

}

#endif // FUNCOESESTRUTURA_H_INCLUDED
