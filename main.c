#include "funcoesEstrutura.h"

#define COMANDOS 18

int main()
{
    Diretorio *unidade;
	DArquivos *aberto = NULL;
	Posicao p = {NULL, 0};
	XY xy = {0, 28, 0 , 0};
	char sair = 1, cont_comandos, cont_letras, flag, str_aux[50], dir[21], set_deleted = 1, cmd;
	char comando[50], op, comandos[COMANDOS][15] = {"SET DEFAULT TO", "SET DELETED", "CREATE", "DIR", "QUIT", "USE",
                                                    "LIST STRUCTURE", "APPEND", "LIST", "CLEAR", "LOCATE", "GOTO",
                                                    "DISPLAY", "EDIT", "DELETE", "RECALL", "PACK", "ZAP"};
	createUnidades(&unidade);
    cmdDbase('T', unidade->und, aberto, 0);
	do{
        x = 1;
        fflush(stdin);
		gotoxy(x, y++), printf(". "), gets(comando);
		cmdDbase('T', unidade->und, aberto, p.posi);
		strcpy(str_aux, "");
        cont_comandos = 0;
        while(cont_comandos < COMANDOS && strstr(comando, comandos[cont_comandos]) == NULL)
            cont_comandos++;

        if(cont_comandos < COMANDOS)
        {
            flag = strlen(comandos[cont_comandos]);
            cont_letras = 0;
            while(flag < strlen(comando))
            {
                str_aux[cont_letras] = comando[flag + 1];
                flag++; cont_letras++;
            }

            switch(cont_comandos)
            {
                case 0:
                    setDefaultTo(&unidade, str_aux);
                    p.pos = NULL;
                    p.posi = 0;
                    aberto = NULL;
                    break;
                case 2:
                    fCreate(&unidade, str_aux, &aberto);
                    p.pos = NULL;
                    p.posi = 0;
                    break;
                case 3:
                    fDir(unidade->und, aberto);
                    p.pos = NULL;
                    p.posi = 0;
                    break;
                case 4:
                    sair = 0;
                    break;
                case 5:
                    fUse(str_aux, &aberto, &unidade);
                    strcpy(dir, unidade->und);
                    strcat(dir, "/");
                    strcat(dir, str_aux);
                    p.pos = NULL;
                    p.posi = 0;
                    break;
                case 6:
                    if(aberto == NULL)
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    else
                        fListStructure(unidade->und, aberto);
                    p.pos = NULL;
                    p.posi = 0;
                    break;
                case 7:
                    if(aberto == NULL)
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    else
                        fAppend(aberto, dir, unidade);
                    p.pos = NULL;
                    p.posi = 0;
                    break;
                case 8:
                    if(aberto == NULL)
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    else
                    {
                        if(strstr(str_aux, "FOR"))
                            fListFor(aberto, str_aux, set_deleted, unidade->und);
                        else
                            fList(aberto, set_deleted, unidade->und);
                      p.pos = NULL;
                        p.posi = 0;
                    }
                    break;
                case 9:
                    fClear();
                    break;
                case 10:
                    if(aberto != NULL)
                        fLocate(aberto, str_aux, set_deleted, unidade->und);
                    else
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    p.pos = NULL;
                    p.posi = 0;
                    break;
                case 11:
                    if(aberto != NULL)
                    {
                        p.pos = NULL;
                        p.posi = 0;
                        p.posi = atoi(str_aux);
                        if(p.posi < 1)
                            gotoxy(x, y++),printf("INVALID INDEX!");
                        else
                            fGoto(aberto, &p, set_deleted, unidade->und);
                    }
                    else
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    break;
                case 12:

                    if(p.pos == NULL)
                        gotoxy(x, y++),printf("NO ITEMS SELECTED");
                    else
                        fDisplay(aberto->Campos, p, unidade->und, aberto);
                    break;
                case 13:
                    if(aberto != NULL)
                    {
                        if(p.pos == NULL)
                            gotoxy(x, y++),printf("NO ITEMS SELECTED");
                        else
                        {
                            fEdit(aberto, dir, p, unidade->und);
                            gravaDadosAtualizados(aberto, unidade->und);
                        }
                        p.pos = NULL;
                        p.posi = 0;
                    }
                    else
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    break;
                case 14:
                    if(aberto != NULL)
                    {
                        if(strcmp(str_aux, "ALL") == 0)
                        {
                            fDeleteAll(aberto->Status, unidade->und, aberto);
                            gravaDadosAtualizados(aberto, unidade->und);
                        }
                        else
                        {
                            if(p.pos != NULL)
                            {
                                fDelete(p, unidade->und, aberto);
                                gravaDadosAtualizados(aberto, unidade->und);
                            }
                            else
                                gotoxy(x, y++),printf("NO ITEMS SELECTED");
                        }
                        p.pos = NULL;
                        p.posi = 0;
                    }
                    else
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    break;
                case 15:
                    if(aberto != NULL)
                    {
                        if(strcmp(str_aux, "ALL") == 0)
                        {
                            fRecallAll(aberto->Status, unidade->und, aberto);
                            gravaDadosAtualizados(aberto, unidade->und);
                        }
                        else
                        {
                            if(p.pos != NULL)
                            {
                                fRecall(p, unidade->und, aberto);
                                gravaDadosAtualizados(aberto, unidade->und);
                            }
                            else
                                gotoxy(x, y++),printf("NO ITEMS SELECTED");
                        }
                        p.pos = NULL;
                        p.posi = 0;
                    }
                    else
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    break;
                case 1:
                    setDeleted(&set_deleted, str_aux, unidade->und, aberto);
                    break;
                case 16:
                    if(aberto != NULL)
                    {
                        fPack(aberto, unidade->und);
                        gravaDadosAtualizados(aberto, unidade->und);
                    }
                    else
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    break;
                case 17:
                    if(aberto != NULL)
                        fZap(&aberto, &unidade);
                    else
                        gotoxy(x, y++),printf("NO FILE OPENED");
                    break;
            }
        }
        else
            gotoxy(x, y++),printf("COMMAND NOT FOUND");
        cmdDbase('T', unidade->und, aberto, p.posi);

	}while(sair);
	return 0;
}
