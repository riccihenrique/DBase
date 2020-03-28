#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

int x = 1, y = 1;

struct coords{
    int xprin, yprin, xlin, ylin; //Posições da tela e das linhas
};
typedef struct coords XY;

struct diretorio{
	struct diretorio *top, *bottom;
	char und[21];
	struct darquivos *arqs;
};
typedef struct diretorio Diretorio; // Informações sobre Diretorios

union dados {
		char ValorL, *ValorC, *ValorM, *ValorD;
		float ValorN;
};

struct tdados{
	union dados d;
	struct infos *Prox;
};
typedef struct tdados TDados; // Tipos de Dados

struct carquivos{
    struct carquivos *Prox;
    char FieldName[11], Type, Dec;
    int Width;
    struct tdados *Patual, *Pdados;
};
typedef struct carquivos CArquivos; // Campos do Arquivo

struct darquivos{
    struct darquivos *Ant, *Prox;
    char NomeDBF[21], Data[11], Hora[6];
    struct tdados *Status;
    struct carquivos *Campos;
};
typedef struct darquivos DArquivos; // Dados do Arquivo

struct posicao{
    TDados *pos;
    int posi;
};
typedef struct posicao Posicao;

TDados *criaDados(TDados d, char t);
void insereTrueFalse(TDados **tipo, TDados *dado);
void insereDados(CArquivos **tipo, TDados *dado);
void leArquivo(FILE *arq, DArquivos **aberto);

void setDefaultTo(Diretorio **estrutura, char diretorio[3])
{
    char aux[5];
    strcpy(aux, diretorio);
    strcat(aux, "/");
    if(strcmp((*estrutura)->und, aux) != 0)
    {
        if(strcmp(diretorio, "C:") == 0 && strcmp((*estrutura)->und, "D:/") == 0)
            (*estrutura) = (*estrutura)->bottom;
        else if(strcmp(diretorio, "D:") == 0 && strcmp((*estrutura)->und, "C:/") == 0)
            (*estrutura) = (*estrutura)->top;
        else
            gotoxy(35, y + 4), printf("DIRETORY %s NOT FOUND", diretorio);
    }
}

void fUse(char nome_arq[21], DArquivos **aberto, Diretorio **estru)
{
    FILE *arq;
    char caminh[21];
    DArquivos *aux;
    strcpy(caminh, (*estru)->und);
    strcat(caminh,"/");
    strcat(caminh, nome_arq);

    if((*estru)->arqs != NULL)
    {
        aux = (*estru)->arqs;
        while(aux->Prox != NULL && strcmp(aux->NomeDBF, nome_arq) != 0) //Verifiva se Arquivo já está carregado na memória
            aux = aux->Prox;
        if(strcmp(aux->NomeDBF, nome_arq) != 0)
        {
            arq = fopen(caminh, "rb");
            if(arq == NULL)
                 gotoxy(35, y + 4), printf("FILE NOT FOUND");
            else
            {
                leArquivo(arq, &*aberto); // Le arquivo para a memória
                aux->Prox = *aberto;
                (*aberto)->Ant = aux;
            }
        }
        else
            *aberto = aux;
    }
    else // Caso não haja nenhum arquivo na memória, já se realiza a leitura
    {
        arq = fopen(caminh, "rb");
        if(arq == NULL)
           gotoxy(35, y + 4), printf("FILE NOT FOUND");
        else
        {
            leArquivo(arq, &*aberto);
            (*estru)->arqs = *aberto;
        }
    }
}

void fCreate(Diretorio **estrutura, char nome_arq[21], DArquivos **aberto)
{
    FILE *arq;
    char caminho_arquivo[21];

    strcpy(caminho_arquivo, (*estrutura)->und);
    strcat(caminho_arquivo, "/");
    strcat(caminho_arquivo, nome_arq); //Trata o nome e diretorio da criaçao do arquivo

    arq = fopen(caminho_arquivo, "rb");

    if(arq == NULL)
        createArquivo(&*estrutura, nome_arq, arq, caminho_arquivo);
    else
    {
        gotoxy(x, y++),printf("EXISTING FILE! DO YOU WANT TO OVERWRITE THIS FILE? <Y/N>");
        cmdDbase('C', (*estrutura)->und, NULL);
        if(gotoxy(x, y),toupper(getch()) == 'Y')
        {
            fclose(arq);
            createArquivo(&*estrutura, nome_arq, arq, caminho_arquivo);
        }
    }
    fUse(nome_arq, aberto, estrutura);
}

void fDir(char *und, DArquivos *aberto)
{
    DArquivos aux;
    DIR *dir;
    FILE *arq;
    char name[21], ext[5], caminho[21];
    int cont_arq = 0, total = 0;
    struct dirent *lsdir;

    strcpy(ext, ".DBF");
    dir = opendir(und);

    gotoxy(x, y),printf("Database Files"); x += 20;
    gotoxy(x, y),printf("Last Update"); x += 20;
    gotoxy(x, y++),printf("Size");
    x = 1;
    cmdDbase('f', und, aberto);
    while((lsdir = readdir(dir)) != NULL) // Percorre o diretório
    {
        x = 1;
        strcpy(name, lsdir->d_name);
        if(strstr(name, ext) != NULL)
        {
            cont_arq++;
            strcpy(caminho, und);
            strcat(caminho, "/");
            strcat(caminho, name);
            arq = fopen(caminho, "rb");
            fread(&aux, sizeof(DArquivos), 1, arq);
            fseek(arq, 0, 2);
            gotoxy(x, y), printf("%s", name); x += 20;
            gotoxy(x, y), printf("%s", aux.Data); x += 20;
            total += ftell(arq);
            gotoxy(x, y++), printf("%d", ftell(arq));
            x = 1;
            cmdDbase('f', und, aberto);
            fclose(arq);
        }
    }
    gotoxy(x, y++),printf("    %d bytes in      %d files.", total, cont_arq);
    cmdDbase('f', und, aberto);
}

void fListStructure(char *und, DArquivos *aberto)
{
    CArquivos *aux_campo = aberto->Campos;
    TDados *aux_dados = aberto->Status;
    int tam = 0;
    unsigned char i = 1, c = 0;

    while(aux_dados != NULL)
    {
        c++;
        aux_dados = aux_dados->Prox; // Conta o numero de registros
    }

    gotoxy(x, y++),printf("Structure for Database: %s%s", und, aberto->NomeDBF); cmdDbase('d', und, aberto);
    gotoxy(x, y++),printf("Number of Data records: %d", c); cmdDbase('d', und, aberto);
    gotoxy(x, y++),printf("Date of last update: %s", aberto->Data); cmdDbase('d', und, aberto);
    gotoxy(x, y),printf("Field"); x += 9;
    gotoxy(x, y),printf("Field Name"); x += 11;
    gotoxy(x, y),printf("Type"); x += 10;
    gotoxy(x, y),printf("Width"); x += 9;
    gotoxy(x, y++),printf("Dec"); x = 1; cmdDbase('d', und, aberto);
    while(aux_campo != NULL) // Percorre os Campos do Arquivo
    {
         gotoxy(x, y), printf("%d", i); x += 9;
         gotoxy(x, y), printf("%s", aux_campo->FieldName); x += 11;
         gotoxy(x, y);
         switch(aux_campo->Type)
         {
            case 'D':
                printf("Date");
                break;
            case 'M':
                printf("Memo");
                break;
            case 'C':
                printf("Character");
                break;
            case 'L':
                printf("Logical");
                break;
            case 'N':
                printf("Numeric");
         }
         x += 10;
         gotoxy(x, y), printf("%d", aux_campo->Width);x += 9;
         gotoxy(x, y++), printf("%c", aux_campo->Dec); x = 1; cmdDbase('d', und, aberto);

        i++;
        tam += aux_campo->Width; // Soma o tamanho de bytes da estrutura

        aux_campo = aux_campo->Prox;
    }
    gotoxy(x, y++),printf("** Total **\t\t      %d", tam + 1); cmdDbase('d', und, aberto);
}

void fAppend(DArquivos *aberto, char dir[], Diretorio *estrutura)
{
    FILE *arq;
    DArquivos altera_Data, *nova_arq;
    CArquivos *aux = aberto->Campos;
    TDados d, *novo_dado;
    char leitura[51], i;
    int yaux1 = y, yaux2 = y;

    arq = fopen(dir, "rb+");
    fseek(arq, 0, SEEK_END); // Adiciona novos dados no fim do arquivo
    while(aux != NULL)
    {
        gotoxy(x, y++), printf("%s", aux->FieldName);
        textbackground(7);
        textcolor(0);
        for(i = 0; i < aux->Width; i++)
            gotoxy(x + i + 11, y - 1), printf(" ");

        textbackground(0);
        textcolor(7);
        cmdDbase('A', estrutura->und, aberto);
        aux = aux->Prox;
    }
    textbackground(7);
    textcolor(0);
    x += 11;

    gotoxy(x, yaux1++), gets(leitura);
    while(strcmp(leitura, "") != 0)
    {
        d.d.ValorL = 1;
        novo_dado = criaDados(d, 'L');
        insereTrueFalse(&aberto->Status, novo_dado);
        fwrite(&d.d.ValorL, sizeof(char), 1, arq);
        aux = aberto->Campos;
        while(aux != NULL)
        {
            switch(aux->Type) // Le dado referente ao tipo, adiciona na estrutura da memória e salva no arquivo
            {
                case 'N':
                    d.d.ValorN = atof(leitura);
                    fwrite(&d.d.ValorN, sizeof(float), 1, arq);
                break;
                case 'D':
                    d.d.ValorD = (char*)malloc(sizeof(char) * 11);
                    strcpy(d.d.ValorD, leitura);
                    fwrite(&leitura[0], sizeof(char), 11, arq);
                break;
                case 'L':
                    d.d.ValorL = leitura[0];
                    fwrite(&d.d.ValorL, sizeof(char), 1, arq);
                break;
                case 'C':
                    d.d.ValorC = (char*)malloc(sizeof(char) * (aux->Width + 1));
                    while(strlen(leitura) > aux->Width)
                    {
                        gotoxy(35, y + 4), printf("Length must be less than %d ", aux->Width);
                        gets(leitura);
                    }
                    strcpy(d.d.ValorC, leitura);
                    fwrite(&leitura[0], sizeof(char), aux->Width + 1, arq);
                break;
                case 'M':
                    d.d.ValorM = (char*)malloc(sizeof(char) * 51);
                    strcpy(d.d.ValorM, leitura);
                    fwrite(&leitura[0], sizeof(char), 51, arq);
                break;
            }
            novo_dado = criaDados(d, aux->Type);
            insereDados(&aux, novo_dado);
            aux = aux->Prox;
            if(aux != NULL)
                gotoxy(x, yaux1++), gets(leitura);
        }
        yaux1 = yaux2;
        aux = aberto->Campos;
        while(yaux1 < y)
        {
            for(i = 0; i < aux->Width; i++)
                gotoxy(x + i, yaux1), printf(" ");
            aux = aux->Prox;
            yaux1++;
        }
        yaux1 = yaux2;
        gotoxy(x, yaux1++), gets(leitura);
    }
    textbackground(0);
    textcolor(7);

    fseek(arq, 0, SEEK_SET); //Altera informações como data e hora do arquivo
    setArqInfo(aberto->NomeDBF, aberto);
    fwrite(aberto, sizeof(DArquivos), 1, arq);
    x = 1;
    fclose(arq);
}

void fList(DArquivos *aberto, char set_deleted, char *und)
{
    CArquivos *aux_campo = aberto->Campos;
    TDados *ativo = aberto->Status;
    char i = 1;
    gotoxy(x, y), printf("Records#");
    x += 9;
    while(aux_campo != NULL) // Lista campos do arquivo
    {
        aux_campo->Patual = aux_campo->Pdados;
        gotoxy(x, y),printf("%s", aux_campo->FieldName);
        if(aux_campo->Type == 'N')
            x += 5;
        x += aux_campo->Width;
        aux_campo = aux_campo->Prox;
    }
    x = 1;
    y++;
    cmdDbase('s', und, aberto);
    aux_campo = aberto->Campos;
    if(set_deleted)
    {
        while(ativo != NULL)
        {
            gotoxy(x, y), printf("%d", i);
                x += 9;
            while(aux_campo != NULL)
            {
                gotoxy(x, y);
                exibeCampos(aux_campo);
                if(aux_campo->Type == 'N')
                    x += 5;
                x += aux_campo->Width;
                aux_campo->Patual = aux_campo->Patual->Prox;
                aux_campo = aux_campo->Prox;
            }
            i++;
            x = 1;
            y++;
            cmdDbase('d', und, aberto);
            aux_campo = aberto->Campos;
            ativo = ativo->Prox;
        }
    }
    else
    {
        while(ativo != NULL)
        {
            if(ativo->d.ValorL)
            {
                gotoxy(x, y), printf("%d", i);
                x += 9;
            }
            while(aux_campo != NULL)
            {
                gotoxy(x, y);
                if(ativo->d.ValorL)
                {
                    exibeCampos(aux_campo);
                    if(aux_campo->Type == 'N')
                        x += 5;
                    x += aux_campo->Width;
                }
                aux_campo->Patual = aux_campo->Patual->Prox;
                aux_campo = aux_campo->Prox;
            }

            if(ativo->d.ValorL)
            {
                i++;
                x = 1;
                y++;
                cmdDbase('d', und, aberto);
            }
            aux_campo = aberto->Campos;
            ativo = ativo->Prox;
        }
    }
    x = 1;
}

void fListFor(DArquivos *aberto, char cmd[], char set_deleted, char *und)
{
    CArquivos *aux_campo = aberto->Campos, *aux_campo2 = aberto->Campos;
    TDados *aux_dado = aberto->Status, *aux_dado2;
    char aux_cmd[21], i, j, ValorC[51], ValorL;
    float ValorN;
    i = 0;
    while(cmd[i + 4] != ' ')
    {
        aux_cmd[i] = cmd[i + 4];
        i++;
    }
    aux_cmd[i] = '\0';
    i += 4;
    aux_campo->Patual = aux_campo->Pdados;
    while(aux_campo != NULL && strcmp(aux_campo->FieldName, aux_cmd) != 0) // Verifica se o campo existe e ja seta o patual para a primeira pos
    {
        aux_campo = aux_campo->Prox;
        aux_campo->Patual = aux_campo->Pdados;
    }
    if(aux_campo == NULL)
    {
        gotoxy(x, y++), printf("%d", i);
        x += 9;
        while(aux_campo2 != NULL)
        {
            gotoxy(x, y);
            exibeCampos(aux_campo2);
            if(aux_campo->Type == 'N')
                x += 5;
            x += aux_campo2->Width;("FIELD NOT FOUND"); cmdDbase('d', und, aberto);
        }
    }
    else
    {
        while(cmd[i] == '=' || cmd[i] == ' ')
            i++;
        j = 0;
        while(cmd[i] != '\0')
            aux_cmd[j++] = cmd[i++];
        aux_cmd[j] = '\0';
        i = 1;
        gotoxy(x, y), printf("RECORD#");
        x += 9;
        while(aux_campo2 != NULL)
        {
            gotoxy(x, y), printf("%s", aux_campo2->FieldName);
            if(aux_campo->Type == 'N')
                x += 5;
            x += aux_campo2->Width;
            aux_campo2->Patual = aux_campo2->Pdados;
            aux_campo2 = aux_campo2->Prox;
        }
        y++; x = 1;
        cmdDbase('d', und, aberto);
        switch(aux_campo->Type)
        {
            case 'C':
            case 'M':
            case 'D':
                strcpy(ValorC, aux_cmd);

                while(aux_dado != NULL)
                {
                    aux_dado2 = aux_campo->Patual;
                    if(set_deleted || aux_dado->d.ValorL) //Todos os gares com || referem-se ao set deleted
                        if(strstr(aux_cmd, aux_campo->Patual->d.ValorC))
                        {
                            aux_campo2 = aberto->Campos;
                            gotoxy(x, y), printf("%d", i);
                            x += 9;
                            while(aux_campo2 != NULL)
                            {
                                gotoxy(x, y);
                                exibeCampos(aux_campo2);
                                if(aux_campo->Type == 'N')
                                    x += 5;
                                x += aux_campo2->Width;
                                aux_campo2->Patual = aux_campo2->Patual->Prox;
                                aux_campo2 = aux_campo2->Prox;
                            }
                            y++; x = 1;
                            cmdDbase('d', und, aberto);
                        }
                        else
                        {
                            aux_campo2 = aberto->Campos;
                            while(aux_campo2 != NULL)
                            {
                                aux_campo2->Patual = aux_campo2->Patual->Prox;
                                aux_campo2 = aux_campo2->Prox;
                            }
                        }
                    aux_campo->Patual = aux_dado2;
                    aux_campo->Patual = aux_campo->Patual->Prox;
                    aux_dado = aux_dado->Prox;
                    i++;
                }
            break;
            case 'N':
                ValorN = atof(aux_cmd);
                while(aux_dado != NULL)
                {
                    aux_dado2 = aux_campo->Patual;
                    if(set_deleted || aux_dado->d.ValorL)
                        if(ValorN == aux_campo->Patual->d.ValorN)
                        {
                            aux_campo2 = aberto->Campos;
                            gotoxy(x, y), printf("%d", i);
                            x += 9;
                            while(aux_campo2 != NULL)
                            {
                                gotoxy(x, y);
                                exibeCampos(aux_campo2);
                                if(aux_campo->Type == 'N')
                                    x += 5;
                                x += aux_campo2->Width;
                                aux_campo2->Patual = aux_campo2->Patual->Prox;
                                aux_campo2 = aux_campo2->Prox;
                            }
                            y++; x = 1;
                            cmdDbase('d', und, aberto);
                        }
                        else
                        {
                            aux_campo2 = aberto->Campos;
                            while(aux_campo2 != NULL)
                            {
                                aux_campo2->Patual = aux_campo2->Patual->Prox;
                                aux_campo2 = aux_campo2->Prox;
                            }
                        }
                    aux_campo->Patual = aux_dado2;
                    aux_campo->Patual = aux_campo->Patual->Prox;
                    aux_dado = aux_dado->Prox;
                    i++;
                }
            break;
            case 'L':
                ValorL = aux_cmd[0];
                while(aux_dado != NULL)
                {
                    aux_dado2 = aux_campo->Patual;
                    if(set_deleted || aux_dado->d.ValorL)
                        if(ValorL == aux_campo->Patual->d.ValorL)
                        {
                            aux_campo2 = aberto->Campos;
                            gotoxy(x, y), printf("%d", i);
                            x += 9;
                            while(aux_campo2 != NULL)
                            {
                                gotoxy(x, y);
                                exibeCampos(aux_campo2);
                                if(aux_campo->Type == 'N')
                                    x += 5;
                                x += aux_campo2->Width;
                                aux_campo2->Patual = aux_campo2->Patual->Prox;
                                aux_campo2 = aux_campo2->Prox;
                            }
                            y++; x = 1;
                            cmdDbase('d', und, aberto);
                        }
                        else
                        {
                            aux_campo2 = aberto->Campos;
                            while(aux_campo2 != NULL)
                            {
                                aux_campo2->Patual = aux_campo2->Patual->Prox;
                                aux_campo2 = aux_campo2->Prox;
                            }
                        }
                    aux_campo->Patual = aux_dado2;
                    aux_campo->Patual = aux_campo->Patual->Prox;
                    aux_dado = aux_dado->Prox;
                    i++;
                }
            break;
        }
    }
    x = 1;
}

void fClear()
{
    clrscr();
    y = 1;
}

void fLocate(DArquivos *aberto, char cmd[], char set_deleted, char *und)
{
    CArquivos *aux_campo = aberto->Campos;
    TDados *aux_dado = aberto->Status;
    char aux_cmd[21], i, j, ValorC[51], ValorL;
    float ValorN;
    i = 0;
    while(cmd[i + 4] != ' ')
    {
        aux_cmd[i] = cmd[i + 4];
        i++;
    }
    aux_cmd[i] = '\0';
    i += 4;
    aux_campo->Patual = aux_campo->Pdados;
    while(aux_campo != NULL && strcmp(aux_campo->FieldName, aux_cmd) != 0) // Verifica se o campo existe e ja seta o patual para a primeira pos
    {
        aux_campo->Patual = aux_campo->Pdados;
        aux_campo = aux_campo->Prox;
    }
    if(aux_campo == NULL)
    {
        gotoxy(x, y++), printf("FIELD NOT FOUND"); cmdDbase('d', und, aberto);
    }
    else
    {
        while(cmd[i] == '=' || cmd[i] == ' ')
            i++;
        j = 0;
        while(cmd[i] != '\0')
            aux_cmd[j++] = cmd[i++];
        aux_cmd[j] = '\0';
        i = 1;
        gotoxy(x, y++), printf("RECORD# %s", aux_campo->FieldName); cmdDbase('d', und, aberto);
        switch(aux_campo->Type)
        {
            case 'C':
            case 'M':
            case 'D':
                strcpy(ValorC, aux_cmd);

                while(aux_campo->Patual != NULL)
                {
                    if(set_deleted || aux_dado->d.ValorL) //Todos os gares com || referem-se ao set deleted
                        if(strstr(ValorC, aux_campo->Patual->d.ValorM))
                        {
                            gotoxy(x, y++), printf("%d\t %s", i, aux_campo->Patual->d.ValorC); cmdDbase('d', und, aberto);
                        }

                    aux_campo->Patual = aux_campo->Patual->Prox;
                    aux_dado = aux_dado->Prox;
                    i++;
                }
            break;
            case 'N':
                ValorN = atof(aux_cmd);
                while(aux_campo->Patual != NULL)
                {
                    if(set_deleted || aux_dado->d.ValorL)
                        if(ValorN == aux_campo->Patual->d.ValorN)
                        {
                            gotoxy(x, y++), printf("%d\t %f", i, aux_campo->Patual->d.ValorN); cmdDbase('d', und, aberto);
                        }

                    aux_campo->Patual = aux_campo->Patual->Prox;
                    aux_dado = aux_dado->Prox;
                    i++;
                }
            break;
            case 'L':
                ValorL = aux_cmd[0];
                while(aux_campo->Patual != NULL)
                {
                    if(set_deleted || aux_dado->d.ValorL)
                        if(ValorL == aux_campo->Patual->d.ValorL)
                        {
                            gotoxy(x, y++),printf("%d\t %c", i, aux_campo->Patual->d.ValorL); cmdDbase('d', und, aberto);
                        }

                    aux_campo->Patual = aux_campo->Patual->Prox;
                    aux_dado = aux_dado->Prox;
                    i++;
                }
            break;
        }
    }
}

void fGoto(DArquivos *aberto, Posicao *pos, char set_deleted, char *und)
{
    CArquivos *aux_campo = aberto->Campos;
    pos->pos = aberto->Status;
    int i = 0, j = 0;

    while(pos->pos != NULL && i < pos->posi)
    {
        if(set_deleted || pos->pos->d.ValorL)
            i++;
        j++;
        if(i < pos->posi)
            pos->pos = pos->pos->Prox;
    }
    if(pos->pos == NULL)
    {
        gotoxy(x, y++), printf("POSITION NOT FOUND"); cmdDbase('d', und, aberto);
    }
    else
    {
        while(aux_campo != NULL)
        {
            aux_campo->Patual = aux_campo->Pdados;
            i = 0;
            while(i < j - 1)
            {
                aux_campo->Patual = aux_campo->Patual->Prox;
                i++;
            }
            aux_campo = aux_campo->Prox;
        }
    }
}

void fDisplay(CArquivos *Campos, Posicao pos, char *und, DArquivos *aberto)
{
    CArquivos *aux_campo = Campos;
    gotoxy(x, y), printf("RECORD#");
    x+= 9;
    while(aux_campo != NULL) // Exibe todos os campos
    {
        gotoxy(x, y), printf("%s", aux_campo->FieldName);
        if(aux_campo->Type == 'N')
            x += 5;
        x += aux_campo->Width;
        aux_campo = aux_campo->Prox;

    }
    x = 1;
    y++;
    cmdDbase('d', und, aberto);
    aux_campo = Campos;
    gotoxy(x, y), printf("%d", pos.posi);
    x += 9;
    while(aux_campo != NULL) // Exibe os dados da posição atual
    {
        gotoxy(x, y);
        exibeCampos(aux_campo);
        if(aux_campo->Type == 'N')
            x += 5;
        x += aux_campo->Width;
        aux_campo = aux_campo->Prox;
    }
    x = 1;
    y++;
    cmdDbase('d', und, aberto);
}

void fEdit(DArquivos *aberto, char dir[], Posicao pos, char *und)
{
    CArquivos *aux_campo = aberto->Campos;
    TDados novo_dado;
    char leitura[51], carac, aux[51], i;
    int yaux1 = y, yaux2 = y;

    while(aux_campo != NULL)
    {
        gotoxy(x, y), printf("%s", aux_campo->FieldName); x += 10;
        textbackground(7);
        textcolor(0);
        for(i = 0; i < aux_campo->Width; i++)
            gotoxy(x + i, y), printf(" ");
        gotoxy(x, y);
        exibeCampos(aux_campo);
        x = 1;
        y++;
        textbackground(0);
        textcolor(7);
        cmdDbase('E', und, aberto);
        aux_campo = aux_campo->Prox;
    }
    x += 10;
    aux_campo = aberto->Campos;
    while(aux_campo != NULL)
    {
        gotoxy(x, yaux1), carac = getch();
        if(carac != 13)
        {
            gotoxy(x, yaux1), printf("                                   ");
            textbackground(7);
            textcolor(0);
            for(i = 0; i < aux_campo->Width; i++)
                gotoxy(x + i, yaux1), printf(" ");

            gotoxy(x, yaux1), printf("%c", carac);
            strcpy(leitura, " ");
            leitura[0] = carac;
            gotoxy(x+1, yaux1), gets(aux);
            strcat(leitura, aux);
            textbackground(0);
            textcolor(7);
            switch(aux_campo->Type)
            {
                case 'N':
                    aux_campo->Patual->d.ValorN = atof(leitura);
                break;
                case 'D':
                    novo_dado.d.ValorD = (char*)malloc(sizeof(char) * 11);
                    strcpy(aux_campo->Patual->d.ValorD, leitura);
                break;
                case 'L':
                    aux_campo->Patual->d.ValorL = leitura[0];
                break;
                case 'C':
                    novo_dado.d.ValorC = (char*)malloc(sizeof(char) * 51);
                    strcpy(aux_campo->Patual->d.ValorC, leitura);
                break;
                case 'M':
                    novo_dado.d.ValorM = (char*)malloc(sizeof(char) * 51);
                    strcpy(&aux_campo->Patual->d.ValorM, leitura);
                break;
            }
        }
        aux_campo = aux_campo->Prox;
        yaux1++;
    }

    x = 1;
    setArqInfo(aberto->NomeDBF, aberto);
    gravaDadosAtualizados(aberto, und);
}

void fDelete(Posicao pos, char *und, DArquivos *aberto)
{
    if(pos.pos->d.ValorL != 0) // Caso não esteja já excluido, exclui
    {
        gotoxy(x, y++), printf("\t1 record deleted\n"); cmdDbase('d', und, aberto);
        pos.pos->d.ValorL = 0;
    }
    else
    {
        gotoxy(x, y++), printf("\t0 redord deleted\n"); cmdDbase('d', und, aberto);
    }
}

void fDeleteAll(TDados *Status, char *und, DArquivos *aberto)
{
    TDados *aux_dado = Status;
    int i = 0;
    while(aux_dado != NULL)
    {
        if(aux_dado->d.ValorL)
        {
            i++;
            aux_dado->d.ValorL = 0;
        }
        aux_dado = aux_dado->Prox;
    }
    gotoxy(x, y++), printf("\t%d FILES DELETED", i); cmdDbase('d', und, aberto);
}

void fRecall(Posicao pos, char *und, DArquivos *aberto)
{
    if(pos.pos->d.ValorL != 1) // Caso não esteja já excluido, exclui
    {
        gotoxy(x, y++), printf("\t1 record recalled"); cmdDbase('d', und, aberto);
        pos.pos->d.ValorL = 1;
    }
    else
    {
        gotoxy(x, y++), printf("\t0 record recalled"); cmdDbase('d', und, aberto);
    }
}

void fRecallAll(TDados *Status, char *und, DArquivos *aberto)
{
    TDados *aux_dado = Status;
    int i = 0;
    while(aux_dado != NULL)
    {
        if(!aux_dado->d.ValorL)
        {
            i++;
            aux_dado->d.ValorL = 1;
        }
        aux_dado = aux_dado->Prox;
    }
    gotoxy(x, y++), printf("\t%d FILES RECALLED", i); cmdDbase('d', und, aberto);
}

void setDeleted(char *set_deleted, char cmd[],char *und, DArquivos *aberto)
{
    if(strcmp(cmd, "ON") == 0)
        *set_deleted = 0;
    else if(strcmp(cmd, "OFF") == 0)
        *set_deleted = 1;
    else
    {
        gotoxy(x, y++), printf("INVALID PARAMETER. TRY 'ON' OR 'OFF'"); cmdDbase('d', und, aberto);
    }

}

void fPack(DArquivos *aberto, char *und)
{
    CArquivos *aux_campo = aberto->Campos;
    TDados *aux_dado = aberto->Status, *aux_excl;
    int i = 0;

    while(aux_campo != NULL)
    {
        aux_campo->Patual = aux_campo->Pdados;
        aux_campo = aux_campo->Prox;
    }


    while(aux_dado != NULL && !aux_dado->d.ValorL)
    {
        aberto->Status = aux_dado->Prox;
        free(aux_dado);
        aux_dado = aberto->Status;
        aux_campo = aberto->Campos;

        while(aux_campo != NULL)
        {
            aux_campo->Pdados = aux_campo->Patual->Prox;
            free(aux_campo->Patual);
            aux_campo->Patual = aux_campo->Pdados;
            aux_campo = aux_campo->Prox;
        }
        i++;
    }
    if(aux_dado != NULL)
    {
        while(aux_dado->Prox != NULL)
        {
            aux_excl = aux_dado->Prox;
            if(!aux_excl->d.ValorL)
            {
                aux_dado->Prox = aux_excl->Prox;
                free(aux_excl);
                aux_campo = aberto->Campos;
                while(aux_campo != NULL)
                {
                    aux_excl = aux_campo->Patual->Prox;
                    aux_campo->Patual->Prox = aux_excl->Prox;
                    free(aux_excl);
                    aux_campo = aux_campo->Prox;
                }
                i++;
            }
            else
            {
                 aux_campo = aberto->Campos;
                 while(aux_campo != NULL)
                 {
                    aux_campo->Patual = aux_campo->Patual->Prox;
                    aux_campo = aux_campo->Prox;
                 }
                 if(aux_dado->Prox != NULL)
                    aux_dado = aux_dado->Prox;
            }
        }
    }
    gotoxy(x, y++), printf("\t%d FILES DELETED", i);cmdDbase('d', und, aberto);
}

void fZap(DArquivos **aberto, Diretorio **unidade)
{
    excluiDados(&(*aberto)->Status);
    excluiCArq(&(*aberto)->Campos);
    if((*unidade)->arqs == *aberto)
    {
        (*unidade)->arqs = (*aberto)->Prox;
        if((*unidade)->arqs != NULL)
            (*unidade)->arqs->Ant = NULL;
    }
    else
        if((*aberto)->Prox == NULL)
            (*aberto)->Ant->Prox = (*aberto)->Prox;
        else
        {
            (*aberto)->Prox->Ant = (*aberto)->Ant;
            (*aberto)->Ant->Prox = (*aberto)->Prox;
        }
    free(*aberto);
    *aberto = NULL;
}

#endif // FUNCOES_H_INCLUDED
