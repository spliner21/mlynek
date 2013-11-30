#include "functions.h"

void ladujgrafike(char *path,char *filename,SDL_Surface **gfx)
{
    //funkcja ładuje grafikę ze ścieżki path+filename do płaszczyzny gfx
    char str[30];
    strcpy(str,path);
    strcat(str,filename);
    *gfx=IMG_Load(str);
}

void param_react(int argc, char *args[], char *gfx_path, SDL_Color *text_color, bool *ismenu, bool *nosplash)
{
    //reakcja na wywołanie gry z parametrami (zwraca, czy ma być menu czy nie)
    int i=1, j=0;
    FILE *cfg;                  //wskaźnik do pliku konfiguracyjnego
    char cfg_s[3]=" ", c=' ';   //zmienne pomocnicze do odczytu z pliku
    int x[3];
    while(i<argc)
    {
        if(strcmp("-c",args[i])==0)     //parametr -c
        {
            cfg=fopen(args[i+1],"r");   //otwarcie pliku konfiguracyjnego
            if(cfg!=NULL)
            {
                while(c!=EOF)
                {
                    c=fscanf(cfg,"%3c",cfg_s);
                    fscanf(cfg,"%c",&c);
                    if(strcmp("gfx",cfg_s)==0)  //odczytujemy ścieżkę do grafiki
                    {
                        for(j=0; j<20; j++)     //zerowanie aktualnej ścieżki
                            gfx_path[j]='\0';
                        j=0;
                        while((c=fgetc(cfg)) != '\n')   //przepisanie nowej ścieżki
                        {
                            gfx_path[j]=c;
                            j++;
                        }

                    }
                    else if(strcmp("txt",cfg_s)==0)     //zmiana koloru tekstu
                    {
                        c=fgetc(cfg);
                        printf("%s",&c);
                        switch(c)
                        {
                        case '1':
                            text_color->r = 250;
                            text_color->g = 250;
                            text_color->b = 250;
                            break;
                        case '2':
                            text_color->r = 250;
                            text_color->g = 200;
                            text_color->b = 50;
                            break;
                        case '3':
                            text_color->r = 150;
                            text_color->g = 150;
                            text_color->b = 250;
                            break;
                        default:
                            break;
                        }
                    }
                    else if(strcmp("tcl",cfg_s)==0)     //zmiana koloru tekstu 2
                    {
                        fscanf(cfg,"%3d,%3d,%3d",x,(x+1),(x+2));
                            text_color->r = x[0];
                            text_color->g = x[1];
                            text_color->b = x[2];
                    }
                }
                fclose(cfg);
            }
        }
        else if(strcmp("-x",args[i])==0)    //parametr -x => brak menu
        {
            *ismenu=false;
        }
        else if(strcmp("-s",args[i])==0)    //parametr -x => brak menu
        {
            *nosplash=true;
        }
        i++;
    }
}

void inicjuj_gre(SDL_Surface **okno,gl_okno *w_gfx, gl_pion *p_gfx, gl_menu *m_gfx, char *gfx_path, FILE **fp)		//inicjalizacja środowiska (ekran, plansza, pionki)
{
    //inicjalizacja wszystkiego co potrzebne w grze
    SDL_Surface* icon = SDL_LoadBMP("icon_s.bmp");      //załadowanie ikony gry
    SDL_Rect wy;
    wy.x=wth;
    wy.y=wth-50;
    time_t *strtime=NULL;       //zmienne na potrzeby wygenerowania losowej nazwy pliku
    struct tm *ftime;
    char filename[25],date[16];

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    *okno = SDL_SetVideoMode((wth+200),wth,0,SDL_HWSURFACE);	//załadowanie ekranu,
    SDL_WM_SetCaption("Młynek v1.0",NULL);                  //tekst belki tytułowej
    SDL_WM_SetIcon(icon, NULL);
    SDL_FreeSurface(icon);
    w_gfx->gray=IMG_Load("gfx/gray.png");
    ladujgrafike(gfx_path,"plansza.bmp",&(w_gfx->plansza));        //załadowanie grafik
    ladujgrafike(gfx_path,"plansza1.png",&(w_gfx->plansza1));
    ladujgrafike(gfx_path,"licznik_p.bmp",&(m_gfx->licznik));
    ladujgrafike(gfx_path,"menu_bg.bmp",&(m_gfx->menubg));
    ladujgrafike(gfx_path,"pion_bg.bmp",&(m_gfx->pionbg));
    ladujgrafike(gfx_path,"wy.bmp",&(m_gfx->wyjscie));
    ladujgrafike(gfx_path,"select.png",&(p_gfx->select));
    ladujgrafike(gfx_path,"cross.png",&(p_gfx->cross));
    ladujgrafike(gfx_path,"r.png",&(p_gfx->red));
    ladujgrafike(gfx_path,"b.png",&(p_gfx->blue));

    SDL_BlitSurface(w_gfx->plansza,NULL,*okno,NULL);      //wyświetlenie początkowej zawartości okna
    SDL_BlitSurface(m_gfx->wyjscie,NULL,*okno,&wy);
    SDL_Flip(*okno);
    strtime=malloc(sizeof(time_t));     //wygenerowanie losowej nazwy pliku
    *strtime=time(NULL);
    ftime=localtime(strtime);
    strftime(date,15*sizeof(char),"%d%m%y%H%M%S",ftime);
    free(strtime);
    free(ftime);
    sprintf(filename,"saves/%s",date);
    *fp=fopen(filename,"w");     //otwarcie pliku
}

void przeladuj_gfx(SDL_Surface **okno,gl_okno *w_gfx, gl_pion *p_gfx, gl_menu *m_gfx,char *gfx_path)		//inicjalizacja środowiska (ekran, plansza, pionki)
{
    SDL_Rect wy;
    wy.x=wth;
    wy.y=wth-50;
    SDL_FreeSurface(w_gfx->plansza);      //zwolnienie starych grafik
    SDL_FreeSurface(w_gfx->plansza1);
    SDL_FreeSurface(p_gfx->select);
    SDL_FreeSurface(p_gfx->cross);
    SDL_FreeSurface(p_gfx->red);
    SDL_FreeSurface(p_gfx->blue);
    SDL_FreeSurface(m_gfx->licznik);
    SDL_FreeSurface(m_gfx->menubg);
    SDL_FreeSurface(m_gfx->pionbg);
    SDL_FreeSurface(m_gfx->wyjscie);
    ladujgrafike(gfx_path,"plansza.bmp",&(w_gfx->plansza));        //załadowanie grafik
    ladujgrafike(gfx_path,"plansza1.png",&(w_gfx->plansza1));
    ladujgrafike(gfx_path,"licznik_p.bmp",&(m_gfx->licznik));
    ladujgrafike(gfx_path,"menu_bg.bmp",&(m_gfx->menubg));
    ladujgrafike(gfx_path,"pion_bg.bmp",&(m_gfx->pionbg));
    ladujgrafike(gfx_path,"wy.bmp",&(m_gfx->wyjscie));
    ladujgrafike(gfx_path,"select.png",&(p_gfx->select));
    ladujgrafike(gfx_path,"cross.png",&(p_gfx->cross));
    ladujgrafike(gfx_path,"r.png",&(p_gfx->red));
    ladujgrafike(gfx_path,"b.png",&(p_gfx->blue));

    SDL_BlitSurface(w_gfx->plansza,NULL,*okno,NULL);  //wyświetlenie stanu początkowego ekranu
    SDL_BlitSurface(m_gfx->wyjscie,NULL,*okno,&wy);
    SDL_Flip(*okno);
}
void rozstaw_gre(SDL_Surface **okno, SDL_Surface *plansza, gl_menu *m_gfx, SDL_Surface *red, SDL_Rect *menu, char *l_pion, TTF_Font *font,TTF_Font *font_s, SDL_Color text_color)		//rozstawienie grafiki w stanie początkowym
{
    SDL_Surface *text=NULL;
    SDL_Rect wy;
    wy.x=wth;
    wy.y=wth-50;

    SDL_BlitSurface(plansza,NULL,*okno,NULL);    //przeładowanie zawartości okna
    text = TTF_RenderUTF8_Blended(font,"Młynek v1.0",text_color);
    SDL_BlitSurface(text, NULL, *okno, &menu[14]);
    text = TTF_RenderUTF8_Blended(font_s,"Etap 1: rozstawienie",text_color);
    SDL_BlitSurface(text, NULL, *okno, &menu[0]);
    text = TTF_RenderUTF8_Blended(font_s,"Etap 2: rozgrywka",text_color);
    SDL_BlitSurface(text, NULL, *okno, &menu[1]);
    text = TTF_RenderUTF8_Blended(font_s,"Czerwoni/Niebiescy",text_color);
    SDL_BlitSurface(text, NULL, *okno, &menu[2]);
    text = TTF_RenderUTF8_Blended(font_s,"Obecny ruch",text_color);
    SDL_BlitSurface(text, NULL, *okno, &menu[10]);
    SDL_BlitSurface(m_gfx->pionbg,NULL,*okno,&menu[13]);
    SDL_BlitSurface(red, NULL, *okno, &menu[11]);
    SDL_BlitSurface(m_gfx->licznik, NULL, *okno, &menu[12]);
    sprintf(l_pion,"%d",0);
    text = TTF_RenderText_Blended(font,l_pion,text_color);
    SDL_BlitSurface(text, NULL, *okno, &menu[3]);
    sprintf(l_pion,"%d",0);
    text = TTF_RenderText_Blended(font,l_pion,text_color);
    SDL_BlitSurface(text, NULL, *okno, &menu[4]);


    SDL_BlitSurface(m_gfx->wyjscie,NULL,*okno,&wy);
    SDL_Flip(*okno);
}
void oblicz_pola(SDL_Rect *team1,SDL_Rect *team2, SDL_Rect *pole, SDL_Rect *menu)		//obliczenie współrzędnych pionków na planszy i pozycji menu
{
    int i,x1,x2,half;
    for(i=0; i<7; i++)
    {
        team1[i].x = i*side;		//wymiary pionu
        team1[i].y = 0;
        team1[i].w = 0;
        team1[i].h = 0;
        team2[i].x = wth-(i+1)*side;		//wymiary pionu
        team2[i].y = d_dist;
        team2[i].w = 0;
        team2[i].h = 0;
    }
    half=(wth-side)/2;

    for(i=0; i<=2; i++) //pętla obliczająca współrzędne
    {
        x1=offside+i*dist;
        x2=wth-(x1+side);

        (pole+i*8+0)->x = x1;
        (pole+i*8+0)->y = x1;
        (pole+i*8+1)->x = half;
        (pole+i*8+1)->y = x1;
        (pole+i*8+2)->x = x2;
        (pole+i*8+2)->y = x1;
        (pole+i*8+3)->x = x2;
        (pole+i*8+3)->y = half;
        (pole+i*8+4)->x = x2;
        (pole+i*8+4)->y = x2;
        (pole+i*8+5)->x = half;
        (pole+i*8+5)->y = x2;
        (pole+i*8+6)->x = x1;
        (pole+i*8+6)->y = x2;
        (pole+i*8+7)->x = x1;
        (pole+i*8+7)->y = half;
    }

    (menu+0)->x=wth+20;     //etap1
    (menu+0)->y=50;
    (menu+1)->x=wth+20;     //etap2
    (menu+1)->y=70;
    (menu+2)->x=wth+30;     //red/blue
    (menu+2)->y=170;
    (menu+3)->x=wth+50;     //wynik red
    (menu+3)->y=205;
    (menu+4)->x=wth+150;    //wynik blue
    (menu+4)->y=205;
    (menu+5)->x=wth;        //grafika zasłaniająca menu
    (menu+5)->y=250;
    (menu+6)->x=wth+10;     //linie komunikatów (1)
    (menu+6)->y=400;
    (menu+7)->x=wth+10;     //(2)
    (menu+7)->y=415;
    (menu+8)->x=wth+10;     //(3)
    (menu+8)->y=430;
    (menu+9)->x=wth+10;     //(4)
    (menu+9)->y=445;
    (menu+10)->x=wth+50;    //obecny ruch (tekst)
    (menu+10)->y=100;
    (menu+11)->x=wth+70;    //obecny ruch (pion)
    (menu+11)->y=120;
    (menu+12)->x=wth;       //tło licznika
    (menu+12)->y=200;
    (menu+13)->x=wth;       //tło piona (obecny ruch)
    (menu+13)->y=120;
    (menu+14)->x=wth+20;    //Belka tytułowa
    (menu+14)->y=0;
    (menu+15)->x=wth;       //Linie menu (1)
    (menu+15)->y=250;
    (menu+16)->x=wth;       //(2)
    (menu+16)->y=280;
    (menu+17)->x=wth;       //(3)
    (menu+17)->y=310;
    (menu+18)->x=wth;       //(4)
    (menu+18)->y=340;
    (menu+19)->x=wth;       //wyjscie
    (menu+19)->y=wth-50;
}
void laduj_menu(SDL_Surface *okno, SDL_Color text_color, TTF_Font *font, SDL_Rect *menu, int i)
{
    //funkcja rysuje napisy menu
    SDL_Surface *text=NULL;
    switch(i)
    {
        case 1:
            text = TTF_RenderUTF8_Blended(font,"Nowa Gra",text_color);
            SDL_BlitSurface(text,NULL,okno,(menu+15));
            text = TTF_RenderUTF8_Blended(font,"Wybierz schemat",text_color);
            SDL_BlitSurface(text,NULL,okno,(menu+16));
            text = TTF_RenderUTF8_Blended(font,"Instrukcja",text_color);
            SDL_BlitSurface(text,NULL,okno,(menu+17));
            break;
        case 2:
            text = TTF_RenderUTF8_Blended(font,"CLASSIC",text_color);
            SDL_BlitSurface(text, NULL, okno, &menu[15]);
            text = TTF_RenderUTF8_Blended(font,"SAND",text_color);
            SDL_BlitSurface(text, NULL, okno, &menu[16]);
            text = TTF_RenderUTF8_Blended(font,"ICE",text_color);
            SDL_BlitSurface(text, NULL, okno, &menu[17]);
            text = TTF_RenderUTF8_Blended(font,"Powrót",text_color);
            SDL_BlitSurface(text, NULL, okno, &menu[18]);
            break;
    }
    SDL_FreeSurface(text);
}
void przeladuj(gl_pion *p_gfx, SDL_Surface **okno, SDL_Surface *plansza1,int *t_plansza, SDL_Rect *pole)	//rysowanie pionków na planszy
{
    int i;
    SDL_BlitSurface(plansza1,NULL,*okno,NULL);
    for(i=0; i<=23; i++)
    {
        switch(t_plansza[i])
        {
        case 1:
            SDL_BlitSurface(p_gfx->red,NULL,*okno,&pole[i]);		//rysuj pion czerwony
            break;
        case -1:
            SDL_BlitSurface(p_gfx->blue,NULL,*okno,&pole[i]);		//rysuj pion niebieski
            break;
        default:
            break;
        }
    }
    SDL_Flip(*okno);
}
int znajdz_pole(int myszkaX, int myszkaY, SDL_Rect *pole)		//zwraca numer pola w tablicy t_tablica na podst. współrzędnych myszki
{
    int i=-1;
    if(myszkaX>=wth)
    {
        if(myszkaY>=(wth-50))
            i=-2;   //przycisk wyjście
    }
    if(myszkaX >= (pole+1)->x && myszkaX <= (pole+1)->x + side)
    {
        if(myszkaY >= (pole+1)->y && myszkaY <= (pole+0)->y + side)
            i=1;
        else if(myszkaY >= (pole+9)->y && myszkaY <= (pole+9)->y + side)
            i=9;
        else if(myszkaY >= (pole+17)->y && myszkaY <= (pole+17)->y + side)
            i=17;
        else if(myszkaY >= (pole+5)->y && myszkaY <= (pole+5)->y + side)
            i=5;
        else if(myszkaY >= (pole+13)->y && myszkaY <= (pole+13)->y + side)
            i=13;
        else if(myszkaY >= (pole+21)->y && myszkaY <= (pole+21)->y + side)
            i=21;
    }
    else if(myszkaY >= (pole+3)->y && myszkaY <= (pole+3)->y + side)
    {
        if(myszkaX >= (pole+7)->x && myszkaX <= (pole+7)->x + side)
            i=7;
        else if(myszkaX >= (pole+15)->x && myszkaX <= (pole+15)->x + side)
            i=15;
        else if(myszkaX >= (pole+23)->x && myszkaX <= (pole+23)->x + side)
            i=23;
        else if(myszkaX >= (pole+3)->x && myszkaX <= (pole+3)->x + side)
            i=3;
        else if(myszkaX >= (pole+11)->x && myszkaX <= (pole+11)->x + side)
            i=11;
        else if(myszkaX >= (pole+19)->x && myszkaX <= (pole+19)->x + side)
            i=19;
    }
    else if(myszkaX >= (pole+0)->x && myszkaX <= (pole+0)->x + side)
    {
        if(myszkaY >= (pole+0)->y && myszkaY <= (pole+0)->y + side)
            i=0;
        else if(myszkaY >= (pole+6)->y && myszkaY <= (pole+6)->y + side)
            i=6;
    }
    else if(myszkaX >= (pole+8)->x && myszkaX <= (pole+8)->x + side)
    {
        if(myszkaY >= (pole+8)->y && myszkaY <= (pole+8)->y + side)
            i=8;
        else if(myszkaY >= (pole+14)->y && myszkaY <= (pole+14)->y + side)
            i=14;
    }
    else if(myszkaX >= (pole+16)->x && myszkaX <= (pole+16)->x + side)
    {
        if(myszkaY >= (pole+16)->y && myszkaY <= (pole+16)->y + side)
            i=16;
        else if(myszkaY >= (pole+22)->y && myszkaY <= (pole+22)->y + side)
            i=22;
    }
    else if(myszkaX >= (pole+2)->x && myszkaX <= (pole+2)->x + side)
    {
        if(myszkaY >= offside && myszkaY <= offside + side)
            i=2;
        else if(myszkaY >= (pole+4)->y && myszkaY <= (pole+4)->y + side)
            i=4;
    }
    else if(myszkaX >= (pole+10)->x && myszkaX <= (pole+10)->x + side)
    {
        if(myszkaY >= (pole+10)->y && myszkaY <= (pole+10)->y + side)
            i=10;
        else if(myszkaY >= (pole+12)->y && myszkaY <= (pole+12)->y + side)
            i=12;
    }
    else if(myszkaX >= (pole+18)->x && myszkaX <= (pole+18)->x + side)
    {
        if(myszkaY >= (pole+18)->y && myszkaY <= (pole+18)->y + side)
            i=18;
        else if(myszkaY >= (pole+20)->y && myszkaY <= (pole+20)->y + side)
            i=20;
    }
    return i;
}
int znajdz_mlynek(int miejsce, int team, int *t_plansza)
{
    //sprawdza czy w polu "miejsce" i sąsiednich polach nie powstał młynek z pionków drużyny "team"
    int ilosc=0;
    if(miejsce<=7)
    {
        switch(miejsce)
        {
        case 0:
            if(*(t_plansza+1)==team && *(t_plansza+2)==team)
                ilosc++;
            if(*(t_plansza+7)==team && *(t_plansza+6)==team)
                ilosc++;
            break;
        default:
            if((miejsce % 2)==0)
            {
                if(*(t_plansza+miejsce+1)==team && *(t_plansza+(miejsce+2)%8)==team)
                    ilosc++;
                if(*(t_plansza+miejsce-1)==team && *(t_plansza+miejsce-2)==team)
                    ilosc++;

            }
            else
            {
                if(*(t_plansza+(miejsce+1)%8)==team && *(t_plansza+miejsce-1)==team)
                    ilosc++;
                if(*(t_plansza+miejsce+8)==team && *(t_plansza+miejsce+16)==team)
                    ilosc++;

            }
            break;

        }

    }
    else if(miejsce>=16)
    {
        switch(miejsce)
        {
        case 16:
            if(*(t_plansza+17)==team && *(t_plansza+18)==team)
                ilosc++;
            if(*(t_plansza+22)==team && *(t_plansza+23)==team)
                ilosc++;
            break;
        default:
            if((miejsce % 2)==0)
            {
                if(*(t_plansza+miejsce+1)==team && *(t_plansza+(miejsce-6)%8+16)==team)
                    ilosc++;
                if(*(t_plansza+miejsce-1)==team && *(t_plansza+miejsce-2)==team)
                    ilosc++;

            }
            else
            {
                if(*(t_plansza+(miejsce-7)%8+16)==team && *(t_plansza+miejsce-1)==team)
                    ilosc++;
                if(*(t_plansza+miejsce-8)==team && *(t_plansza+miejsce-16)==team)
                    ilosc++;

            }
            break;

        }

    }
    else
    {
        switch(miejsce)
        {
        case 8:
            if(*(t_plansza+15)==team && *(t_plansza+14)==team)
                ilosc++;
            if(*(t_plansza+9)==team && *(t_plansza+10)==team)
                ilosc++;
            break;
        default:
            if((miejsce % 2)==0)
            {
                if(*(t_plansza+miejsce+1)==team && *(t_plansza+(miejsce-6)%8+8)==team)
                    ilosc++;
                if(*(t_plansza+miejsce-1)==team && *(t_plansza+miejsce-2)==team)
                    ilosc++;

            }
            else
            {
                if(*(t_plansza+(miejsce-7)%8+8)==team && *(t_plansza+miejsce-1)==team)
                    ilosc++;
                if(*(t_plansza+miejsce-8)==team && *(t_plansza+miejsce+8)==team)
                    ilosc++;

            }
            break;

        }

    }
    return ilosc;
}
bool znajdz_niemlynek(int team, int *t_plansza, SDL_Surface *menubg, SDL_Surface *select, SDL_Surface **okno, SDL_Rect *pole, SDL_Rect *menu, TTF_Font *font, SDL_Color text_color)
{
    //sprawdza, czy wybrany przez gracza pionek nie należy do młynka. Jeśli należy wyświetla stosowny komunikat
    int i;

    SDL_Surface *text=NULL;
    bool rezultat=false;

    for(i=0; i<=23; i++)
    {
        if((t_plansza[i]==team)&&(znajdz_mlynek(i,team,t_plansza)==0))
        {
            char komunikat[4][23]= {"Wybrany pionek","należy do młynka!","Wybierz pionek","spośród zaznaczonych"};
            rezultat=true;
            SDL_BlitSurface(select,NULL,*okno,&pole[i]);
            SDL_BlitSurface(menubg, NULL, *okno, &menu[5]);
            text = TTF_RenderUTF8_Blended(font,komunikat[0],text_color);
            SDL_BlitSurface(text, NULL, *okno, &menu[6]);
            text = TTF_RenderUTF8_Blended(font,komunikat[1],text_color);
            SDL_BlitSurface(text, NULL, *okno, &menu[7]);
            text = TTF_RenderUTF8_Blended(font,komunikat[2],text_color);
            SDL_BlitSurface(text, NULL, *okno, &menu[8]);
            text = TTF_RenderUTF8_Blended(font,komunikat[3],text_color);
            SDL_BlitSurface(text, NULL, *okno, &menu[9]);
        }
    }
    SDL_FreeSurface(text);
    return rezultat;
}
bool sprawdz_mozliwosc(int p, int *t_plansza)
{
    //sprawdza, czy pionek na polu p może się ruszyć (czy nie jest zablokowany)
    bool rezultat=false;
    switch(p)   //sytuacje 0,7,8,15,16,23 są wyjątkowe (narożniki, pola poprzednie/następne nie różnią się numerami o 1)
    {
    case 0:
        if(*(t_plansza+7)==0 || *(t_plansza+1)==0)
            rezultat=true;
        break;
    case 8:
        if(*(t_plansza+15)==0 || *(t_plansza+9)==0)
            rezultat=true;
        break;
    case 16:
        if(*(t_plansza+23)==0 || *(t_plansza+17)==0)
            rezultat=true;
        break;
    case 7:
        if(*(t_plansza+6)==0 || *(t_plansza)==0 || *(t_plansza+15)==0)
            rezultat=true;
        break;
    case 15:
        if(*(t_plansza+14)==0 || *(t_plansza+8)==0 || *(t_plansza+7)==0 || *(t_plansza+23)==0)
            rezultat=true;
        break;
    case 23:
        if(*(t_plansza+22)==0 || *(t_plansza+16)==0 || *(t_plansza+15)==0)
            rezultat=true;
        break;
    default:    //tutaj rozbijamy sytuację na 2 przypadki : parzyste/nieparzyste.
        if((p%2==0)&&(*(t_plansza+(p+1))==0 || *(t_plansza+(p-1))==0))
            rezultat=true;
        else if(p%2==1) //nieparzyste rozbijamy na 3 przypadki (0-7,8-15,16-23)
        {
            if((p<8)&&(*(t_plansza+(p+1))==0 || *(t_plansza+(p-1))==0 || *(t_plansza+(p+8))==0))
                rezultat=true;
            else if((p>16)&&(*(t_plansza+(p+1))==0 || *(t_plansza+(p-1))==0 || *(t_plansza+(p-8))==0))
                rezultat=true;
            else if((p<16)&&(p>8)&&(*(t_plansza+(p+1))==0 || *(t_plansza+(p-1))==0 || *(t_plansza+(p-8))==0 || *(t_plansza+(p+8))==0))
                rezultat=true;
        }
    }
    return rezultat;
}
bool sprawdz_ruch(int p1, int p2)
{
    //sprawdza, czy pionek wykonuje ruch na uprawnione pole (tzn. czy pole p2 jest połączone z p1 linią)
    bool rezultat=false;
    if(p1<8)    //rozbijamy na 3 przypadki (0-7),
    {
        if(p1==0 && (p2==7 || p2==1))
            rezultat=true;
        else if(p1==7 && (p2==0 || p2==6 || p2==15))    //a następnie na przypadki specyficzne
            rezultat=true;
        else if(p1!=0 && p1!=7)
        {
            if((p1%2==0)&&((p2==p1+1)||(p2==p1-1)))     //oraz na parzyste/nieparzyste
                rezultat=true;
            else if((p1%2==1)&&((p2==p1+1)||(p2==p1-1)||(p2==p1+8)))
                rezultat=true;
        }
    }
    else if(p1>15) //(16-23), reszta analogicznie
    {
        if(p1==16 && (p2==23 || p2==17))
            rezultat=true;
        else if(p1==23 && (p2==16 || p2==22 || p2==15))
            rezultat=true;
        else if(p1!=16 && p1!=23)
        {
            if((p1%2==0)&&((p2==p1+1)||(p2==p1-1)||(p2==p1+8)))
                rezultat=true;
            else if((p1%2==1)&&((p2==p1+1)||(p2==p1-1)||(p2==p1-8)))
                rezultat=true;
        }
    }
    else    //(8-15)
    {
        if(p1==8 && (p2==15 || p2==9))
            rezultat=true;
        else if(p1==15 && (p2==8 || p2==14 || p2==7 || p2==23))
            rezultat=true;
        else if(p1!=8 && p1!=15)
        {
            if((p1%2==0)&&((p2==p1+1)||(p2==p1-1)||(p2==p1+8)))
                rezultat=true;
            else if((p1%2==1)&&((p2==p1+1)||(p2==p1-1)||(p2==p1+8)||(p2==p1-8)))
                rezultat=true;
        }

    }
    return rezultat;
}
void dodaj_ruch(int p1, int p2, int p, int t, rozgr *rozgrywka)
{
    //powiększa tablicę rozgrywka oraz dodaje nowy ruch
    rozgrywka->ruch[p-1][0]=p1;
    rozgrywka->ruch[p-1][1]=p2;
    rozgrywka->ruch[p-1][2]=t;
    rozgrywka->il_ruchow=p;
    rozgrywka->ruch=(int**)realloc(rozgrywka->ruch,((p+1)*sizeof(int*)));
    rozgrywka->ruch[p]=(int*)malloc(4*sizeof(int));
    rozgrywka->ruch[p][3]=-1;
}
bool sprawdz_powtorzenia(int p1, int p2, rozgr *rozgrywka, int p)
{
    //sprawdza, czy w przebiegu rozgrywki nie znajdują się 3 powtarzające się z rzędu sytuacje
    bool rezultat=false;
    if(((rozgrywka->ruch[p-2][0])==(rozgrywka->ruch[p-6][0]))&&((rozgrywka->ruch[p-2][0])==(rozgrywka->ruch[p-10][0]))&&((rozgrywka->ruch[p-2][1])==(rozgrywka->ruch[p-6][1]))&&((rozgrywka->ruch[p-2][1])==(rozgrywka->ruch[p-10][1])))
    {
        if((p1==(rozgrywka->ruch[p-4][0]))&&(p1==(rozgrywka->ruch[p-8][0]))&&(p2==(rozgrywka->ruch[p-4][1]))&&(p2==(rozgrywka->ruch[p-8][1])))
            rezultat=true;
    }
    return rezultat;
}
bool przestaw_pion(int p1, int p2,int t, int *team, int *t_plansza,gl_pion *p_gfx,SDL_Surface **okno, SDL_Surface *plansza1, SDL_Rect *pole)
{
    //operacja przestawienia pionka z pola p1 na pole p2
    bool test=false;
    switch(t)
    {
        //sprawdzenie, czy pionek nie jest zablokowany (tylko, jeżeli drużyna ma więcej niż 3 pionki)
    case 1:
        if(team[1]<6)
            test=sprawdz_ruch(p1,p2);
        else test=true;
        break;
    case -1:
        if(team[0]<6)
            test=sprawdz_ruch(p1,p2);
        else test=true;
        break;
    default:               //bo kompilator wyrzucał warning...
        break;
    }
    if(test==true)
    {
        t_plansza[p1]=0;    //zamiana pól w tablicy planszy
        t_plansza[p2]=t;
        przeladuj(p_gfx,okno, plansza1,t_plansza,pole); //rozstawienie planszy od nowa (dla gracza jak przestawienie pionka)
    }
    return test;
}
void usun_pion(int p, int t, int *t_plansza, int *team, SDL_Surface *cross, SDL_Surface **okno, SDL_Rect *team1, SDL_Rect *team2)
{
    //operacja usuwania pionka z planszy
    t_plansza[p]=0;
    switch(t)
    {
        //krzyżyk u góry/dołu planszy - po stronie drużyny, która straciła pion
    case 1:
        SDL_BlitSurface(cross,NULL,*okno,&team1[team[1]]);
        team[1]++;
        break;
    case -1:
        SDL_BlitSurface(cross,NULL,*okno,&team2[team[0]]);
        team[0]++;
        break;
    }
}
bool wyjscie_b(SDL_Surface **okno,char *gfx_path)
{
    SDL_Surface *wyjscie_s=NULL;    //podświetlenie przycisku (czysto efektowne, zero sensu :P)
    SDL_Rect wy;
    wy.x=wth;
    wy.y=wth-50;
    strcat(gfx_path,"wy_sel.bmp");
    wyjscie_s=SDL_LoadBMP(gfx_path);
    SDL_BlitSurface(wyjscie_s,NULL,*okno,&wy);
    SDL_Flip(*okno);
    SDL_Delay(100);     //do tej linijki etap czysto efektowny,
    SDL_FreeSurface(wyjscie_s);
    return true;       //dwie następne to etap wyjścia :P
}
void zeruj_gre(int *t_plansza, int *team, int p, rozgr *rozgrywka)
{
    //funkcja czyści pamięć starej rozgrywki - tworzy pustą planszę
    int i;
    for(i=0; i<24; i++)
        t_plansza[i]=0; //zerowanie pól planszy
    team[0]=0;
    team[1]=0;
    for(i=0; i<p; i++)
        free(rozgrywka->ruch[i]);
    free(rozgrywka->ruch);
    rozgrywka->ruch=(int**)malloc(sizeof(int*));
    rozgrywka->ruch[0]=(int*)malloc(4*sizeof(int));
}
void zwolnij(gl_okno *w_gfx, gl_pion *p_gfx, gl_menu *m_gfx, SDL_Surface **text, rozgr *rozgrywka, int p)
{
    int i=0;
    //zwolnienie pamięci
    SDL_FreeSurface(w_gfx->plansza);
    SDL_FreeSurface(w_gfx->plansza1);
    SDL_FreeSurface(w_gfx->gray);
    SDL_FreeSurface(p_gfx->red);
    SDL_FreeSurface(p_gfx->blue);
    SDL_FreeSurface(p_gfx->select);
    SDL_FreeSurface(p_gfx->cross);
    SDL_FreeSurface(m_gfx->licznik);
    SDL_FreeSurface(m_gfx->menubg);
    SDL_FreeSurface(m_gfx->pionbg);
    SDL_FreeSurface(*text);
    free(w_gfx);
    free(m_gfx);
    free(p_gfx);
    for(i=0; i<p; i++)
        free(rozgrywka->ruch[i]);
    free(rozgrywka->ruch);
    free(rozgrywka);
}
