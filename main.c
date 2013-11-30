#include "main.h"
int main(int argc, char *args[])
{
    /*DEKLARACJA ZMIENNYCH */
    int t_plansza[24]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},	//tablica pionów na planszy
        team[2]={0,0},		//tablica druzyn, zlicza stracone pionki
        i=1,				//licznik pętli
        t=1,				//identyfikator druzyny
        p=0,				//licznik ruchów
        temp_ruch=0,        //tymczasowe przetrzymywanie ruchu
        mtest=0,            //zmienna do testu młynka
        mlynki=0,			//licznik znalezionych młynków
        myszkaX=0,
        myszkaY=0;          //współrzędne myszki
    bool wyjscie=false,	    //zmienna warunkująca zamknięcie programu
         wybor=false,	    //Jeśli "true" to znaczy, że pionek został podniesiony.
         bylmlynek=false,   //jesli true, to nie zmieniac wartości w tablicy rozgrywka->ruch[x][3]
         finish=false,      //Wartość true oznacza, że któraś z drużyn wygrała lub jest remis.
         ismenu=true,       //true-> jesteśmy w menu, false->trwa rozgrywka
         nosplash=false,    //jeżeli true-> to nie pojawi się grafika końcowa
         test=false;        //zmienna testowa dla wyszukiwania młynków
    SDL_Event zdarzenie;
    SDL_Surface *okno = NULL,   //płaszczyzna okna
                *text = NULL;   //płaszczyzna tekstowa
    gl_okno *w_gfx=malloc(sizeof(gl_okno));     //grafiki duże - plansza, tło, itp.
    gl_pion *p_gfx=malloc(sizeof(gl_pion));     //grafiki małe - pionki, zaznaczenia, krzyżyki
    gl_menu *m_gfx=malloc(sizeof(gl_menu));     //grafiki menu - przyciski, tła, itp.
    SDL_Rect team1[7],      //współrzędne pól dla krzyżyków
             team2[7],
             pole[24],		//tablica współrzędnych pól na planszy
             menu[20];      //pozycje elementów menu
    rozgr *rozgrywka;       //struktura tablicowa przechowująca przebieg rozgrywki
    FILE *fp=NULL;          //wskaźnik na plik przechowujący przebieg rozgrywki
    char l_pion[2],         //zmienna przechowująca ilość pionków zbitych (liczba punktów) dla obu drużyn
         gfx_path[30]="gfx/classic/";       //ścieżka do grafik
    TTF_Font *font, *font_s;                //czcionka duża i mała
    SDL_Color text_color = {250, 250, 250}, //color tekstu
              end_color = {0, 255, 50};     //kolor dla ukończonych etapów


    /*KONIEC DEKLARACJI ZMIENNYCH, POCZĄTEK PROGRAMU */

    if(argc>1)
        param_react(argc,args,gfx_path, &text_color,&ismenu,&nosplash);
    inicjuj_gre(&okno,w_gfx,p_gfx,m_gfx,gfx_path,&fp);
    oblicz_pola(team1,team2,pole,menu);           //zapis wszystkich współrzędnych potrzebnych w grze
    font = TTF_OpenFont("FreeSans.ttf", 24);            //ładowanie dużej i małej czcionki
    font_s = TTF_OpenFont("FreeSans.ttf", 16);
    if(ismenu==true)        //sprawdzenie, czy gra ma wyświetlić menu, czy od razu przejść do gry
    {
        SDL_BlitSurface(m_gfx->menubg,NULL,okno,&menu[5]);
        laduj_menu(okno,text_color,font,menu,1);          //wyświetlenie menu
        SDL_BlitSurface(w_gfx->gray,NULL,okno,NULL);           //zaszarzenie planszy
    }
    else        //jeśli nie ma być menu, to rysujemy planszę rozgrywki
    {
        SDL_FreeSurface(m_gfx->menubg);
        ladujgrafike(gfx_path,"menu_clean.bmp",&(m_gfx->menubg));
        rozstaw_gre(&okno,w_gfx->plansza,m_gfx,p_gfx->red,menu,l_pion,font,font_s,text_color);
    }
    rozgrywka=(rozgr*)malloc(sizeof(rozgr));            //inicjalizacja tablicy przechowującej rozgrywkę
    rozgrywka->ruch=(int**)malloc(sizeof(int*));
    rozgrywka->ruch[0]=(int*)malloc(4*sizeof(int));
    text = TTF_RenderUTF8_Blended(font,"Młynek v1.0",text_color); //wyświetlenie nagłówka gry
    SDL_BlitSurface(text, NULL, okno, &menu[14]);
        SDL_Flip(okno);


    while(!wyjscie)	//pętla główna programu - ustalenie zmiennej wyjscie=true wewnątrz pętli powoduje zamknięcie gry
    {
        while(SDL_PollEvent(&zdarzenie))
        {
            if(zdarzenie.type == SDL_MOUSEMOTION) //pobranie współrzędnych myszki
            {
                myszkaX = zdarzenie.motion.x;
                myszkaY = zdarzenie.motion.y;
            }
            if(zdarzenie.type == SDL_MOUSEBUTTONDOWN && zdarzenie.button.button == SDL_BUTTON_LEFT)   //reakcja na LPM
            {
                if(ismenu==true)        //MENU
                {
                    switch(i)
                    {
                    case 1:         //menu główne
                        if(myszkaX>=wth)
                        {
                            if(myszkaY>=250 && myszkaY<280) //Nowa gra
                            {
                                rozstaw_gre(&okno,w_gfx->plansza,m_gfx,p_gfx->red,menu,l_pion,font,font_s,text_color);
                                SDL_FreeSurface(m_gfx->menubg);
                                ladujgrafike(gfx_path,"menu_clean.bmp",&(m_gfx->menubg));
                                ismenu=false;       //opuszczenie menu
                                i=-1;               //reinicjalizacja i
                                SDL_Flip(okno);
                            }
                            else if(myszkaY>=280 && myszkaY<310)    //Wybór schematu
                            {
                                i=2;
                                SDL_BlitSurface(m_gfx->menubg,NULL,okno,&menu[5]);
                                laduj_menu(okno,text_color,font,menu,2);
                                SDL_Flip(okno);
                            }
                            else if(myszkaY>=310 && myszkaY<340)    //Wybór schematu
                            {
                                i=3;
                                text=IMG_Load("gfx/info.bmp");
                                SDL_BlitSurface(text,NULL,okno,NULL);
                                SDL_FreeSurface(text);
                                SDL_Flip(okno);
                            }
                        }
                        break;
                    case 2: //wybór schematu
                        if(myszkaX>=wth)
                        {
                            if(myszkaY>=250 && myszkaY<280)         //opcja1: classic
                            {
                                strcpy(gfx_path,"gfx/classic/");
                                text_color.r = 250;
                                text_color.g = 250;
                                text_color.b = 250;
                            }
                            else if(myszkaY>=280 && myszkaY<310)    //opcja 2: sand
                            {
                                strcpy(gfx_path,"gfx/sand/");
                                text_color.r = 250;
                                text_color.g = 200;
                                text_color.b = 50;
                            }
                            else if(myszkaY>=310 && myszkaY<340)    //opcja 3: ice
                            {
                                strcpy(gfx_path,"gfx/ice/");
                                text_color.r = 150;
                                text_color.g = 150;
                                text_color.b = 250;
                            }
                            else if(myszkaY>=340 && myszkaY<370)
                            {
                                i=3;
                            }
                        }
                        if(i!=3)    //jeśli wybrano nowy schemat, to przeładowujemy grafikę
                        {
                            przeladuj_gfx(&okno,w_gfx,p_gfx,m_gfx,gfx_path);
                            text = TTF_RenderUTF8_Blended(font,"Młynek v1.0",text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[14]);
                            SDL_BlitSurface(w_gfx->gray,NULL,okno,NULL);
                        }
                        SDL_BlitSurface(m_gfx->menubg,NULL,okno,(menu+5));    //wyświetlenie głównego menu
                        laduj_menu(okno,text_color,font,menu,1);
                        SDL_Flip(okno);
                        i=1;
                        break;
                    case 3:
                        i=1;
                        SDL_BlitSurface(w_gfx->plansza,NULL,okno,NULL);
                        SDL_BlitSurface(m_gfx->menubg,NULL,okno,(menu+5));    //wyświetlenie głównego menu
                        laduj_menu(okno,text_color,font,menu,1);          //wyświetlenie menu
                        SDL_BlitSurface(w_gfx->gray,NULL,okno,NULL);    //zaszarzenie planszy
                        text = TTF_RenderUTF8_Blended(font,"Młynek 1.0",text_color); //wyświetlenie nagłówka gry
                        SDL_BlitSurface(text, NULL, okno, &menu[14]);
                        SDL_BlitSurface(m_gfx->wyjscie, NULL, okno, &menu[19]);
                        SDL_Flip(okno);
                        break;
                    }
                    if(myszkaX>=wth && myszkaY>=(wth-50))   //wyjście z gry
                    {
                        wyjscie=wyjscie_b(&okno,gfx_path);
                    }
                }
                else if(!finish)
                {

                    i=znajdz_pole(myszkaX,myszkaY,pole);        //jeżeli kliknięto w pole to zwróci jego numer, w innym przypadku zwróci -1
                    if(i==-2)
                    {
                        wyjscie=wyjscie_b(&okno,gfx_path);
                    }
                    if(mlynki>0 && t_plansza[i]==t)             //reakcja na młynek z poprz. ruchu
                    {
                        mtest=znajdz_mlynek(i,t,t_plansza);     //sprawdzenie, czy wybrany pionek nie należy do młynka
                        if((mtest>0 && znajdz_niemlynek(t, t_plansza, m_gfx->menubg, p_gfx->select, &okno, pole, menu,font_s,text_color)==false)||mtest==0)
                        {
                            //jeżeli wszystkie pionki przeciwnika należą do młynka lub wybrany pionek nie należy
                            rozgrywka->ruch[p-1][3]=i;
                            usun_pion(i, t, t_plansza, team, p_gfx->cross, &okno, team1, team2); //usuwamy wybrany pionek
                            przeladuj(p_gfx,&okno, w_gfx->plansza1,t_plansza,pole);    //przeładowanie zawartości okna
                            mlynki--;           //dekrementacja licznika młynków
                            bylmlynek=true;
                            SDL_BlitSurface(m_gfx->menubg, NULL, okno, &menu[5]);
                            if(mlynki==1)       //wyświetlenie komunikatu o młynku
                            {
                                text=TTF_RenderUTF8_Blended(font_s,"Wybierz pionek do zbicia",text_color);
                                SDL_BlitSurface(text,NULL,okno,&menu[7]);
                            }
                        }
                            SDL_BlitSurface(m_gfx->licznik, NULL, okno, &menu[12]);
                        sprintf(l_pion,"%d",(team[0]));
                        text = TTF_RenderText_Blended(font,l_pion,text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[3]);
                        sprintf(l_pion,"%d",(team[1]));
                        text = TTF_RenderText_Blended(font,l_pion,text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[4]);
                        i=-1;

                        if(team[0]==7)  //komunikat o zwycięstwie drużyny czerwonych
                        {
                            text = TTF_RenderUTF8_Blended(font_s,"Etap 2: rozgrywka",end_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[1]);
                            rozgrywka->victory_t=1;
                            text = TTF_RenderUTF8_Blended(font_s,"Wygrały czerwone!",text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[6]);
                            text = TTF_RenderUTF8_Blended(font_s,"Naciśnij enter...",text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[7]);
                            SDL_BlitSurface(w_gfx->gray,NULL,okno,NULL);
                            finish=true;
                        }
                        else if(team[1]==7)  //komunikat o zwycięstwie drużyny niebieskich
                        {
                            text = TTF_RenderUTF8_Blended(font_s,"Etap 2: rozgrywka",end_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[1]);
                            rozgrywka->victory_t=-1;
                            text = TTF_RenderUTF8_Blended(font_s,"Wygrały niebieskie!",text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[6]);
                            text = TTF_RenderUTF8_Blended(font_s,"Naciśnij enter...",text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[7]);
                            SDL_BlitSurface(w_gfx->gray,NULL,okno,NULL);
                            finish=true;
                        }
                    }
                    else if(mlynki==0 && p<=17)     //etap ustawiania pionków na planszy
                    {
                        if(i>=0 && t_plansza[i]==0) //spr. czy wybrano wolne pole
                        {
                            t_plansza[i]=t;
                            p++;        //inkrementacja licznika ruchów
                            dodaj_ruch(i,-1,p,t,rozgrywka);   //uaktualnienie tabeli przebiegu rozgrywki
                            if(t<0)                             //ustawienie pionka na planszy
                            {
                                SDL_BlitSurface(p_gfx->blue,NULL,okno,&pole[i]);

                            }
                            else
                            {
                                SDL_BlitSurface(p_gfx->red,NULL,okno,&pole[i]);
                            }
                            SDL_BlitSurface(m_gfx->licznik, NULL, okno, &menu[12]); //wyświetlenie licznika
                            sprintf(l_pion,"%d",(team[0]));
                            text = TTF_RenderText_Blended(font,l_pion,text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[3]);
                            sprintf(l_pion,"%d",(team[1]));
                            text = TTF_RenderText_Blended(font,l_pion,text_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[4]);
                            mlynki=znajdz_mlynek(i,t,t_plansza);    //sprawdzenie czy nie powstał młynek
                            switch(mlynki)  //komunikaty o znalezionych młynkach
                            {
                            case 1:
                                text = TTF_RenderUTF8_Blended(font_s,"Znaleziono młynek!",text_color);
                                SDL_BlitSurface(text, NULL, okno, &menu[6]);
                                text = TTF_RenderUTF8_Blended(font_s,"Wybierz pionek do zbicia",text_color);
                                SDL_BlitSurface(text, NULL, okno, &menu[7]);
                                break;
                            case 2:
                                text = TTF_RenderUTF8_Blended(font_s,"Znaleziono 2 młynki!",text_color);
                                SDL_BlitSurface(text, NULL, okno, &menu[6]);
                                text = TTF_RenderUTF8_Blended(font_s,"Wybierz pionki do zbicia",text_color);
                                SDL_BlitSurface(text, NULL, okno, &menu[7]);
                                break;
                            default:
                                break;
                            }
                            t=t*(-1);   //zmiana drużyny
                        }
                        i=-1;
                        if(p==18)
                        {
                            text = TTF_RenderUTF8_Blended(font_s,"Etap 1: rozstawienie",end_color);
                            SDL_BlitSurface(text, NULL, okno, &menu[0]);
                        }

                    }
                    else if(i>=0 && p>17)   //rozgrywka - przesuwanie pionków
                    {
                        if(t_plansza[i]==t && wybor==false) //"podniesienie" pionka
                        {
                            switch(t)
                            {
                            case 1:
                                if((sprawdz_mozliwosc(i,t_plansza)==true)||(team[1]==6))    //jeżeli jest możliwość,
                                {
                                    //lub dana drużyna ma na planszy tylko 3 pionki
                                    temp_ruch=i;
                                    wybor=true;
                                    SDL_BlitSurface(p_gfx->select,NULL,okno,&pole[i]);     //narysowanie zaznaczenia podniesionego pionka
                                }
                                break;
                            case -1:
                                if((sprawdz_mozliwosc(i,t_plansza)==true)||(team[0]==6))    //j.w. tylko dla przeciwnej drużyny
                                {
                                    temp_ruch=i;
                                    wybor=true;
                                    SDL_BlitSurface(p_gfx->select,NULL,okno,&pole[i]);
                                }
                                break;
                            }
                        }
                        else if(wybor==true && temp_ruch!=i && t_plansza[i]==0)
                        {
                            test=przestaw_pion(temp_ruch,i,t,team, t_plansza,p_gfx,&okno,w_gfx->plansza1,pole); //przestawienie pionka
                            SDL_Flip(okno);
                            if(test==true)
                            {
                                test=sprawdz_powtorzenia(temp_ruch,i,rozgrywka,p);    //sprawdzenie, czy nie nastąpiły 3x te same ruchy po kolei
                                if(test==true) //komunikat o remisie
                                {
                                    text = TTF_RenderUTF8_Blended(font_s,"Powtarzasz ruchy!",text_color);
                                    SDL_BlitSurface(text, NULL, okno, &menu[6]);
                                    text = TTF_RenderUTF8_Blended(font_s,"Remis!",text_color);
                                    SDL_BlitSurface(text, NULL, okno, &menu[7]);
                                    text = TTF_RenderUTF8_Blended(font_s,"Naciśnij enter",text_color);
                                    SDL_BlitSurface(text, NULL, okno, &menu[8]);
                                    finish=true;                //teraz gra czeka na naciśnięcie [Enter]
                                    rozgrywka->victory_t=0;     //dzięki temu gra wpisze 0 w miejscu drużyny zwycięskiej.
                                    SDL_BlitSurface(w_gfx->gray,NULL,okno,NULL);
                                }
                                else    //wykonanie ruchu
                                {
                                    p++;
                                    dodaj_ruch(temp_ruch,i,p,t,rozgrywka);
                                    mlynki=znajdz_mlynek(i,t,t_plansza);
                                    if(mlynki)  //komunikat o znalezieniu młynka
                                    {   //(w rozgrywce może wystąpić tylko pojedynczy młynek)
                                        text = TTF_RenderUTF8_Blended(font_s,"Znaleziono młynek!",text_color);
                                        SDL_BlitSurface(text, NULL, okno, &menu[6]);
                                        text = TTF_RenderUTF8_Blended(font_s,"Wybierz pionek do zbicia",text_color);
                                        SDL_BlitSurface(text, NULL, okno, &menu[7]);
                                    }
                                    t=t*(-1);
                                    wybor=false;
                                }
                            }
                        }
                        else if(wybor==true && temp_ruch==i) //odznaczanie pionka
                        {
                            wybor=false;
                            przeladuj(p_gfx,&okno,w_gfx->plansza1,t_plansza,pole);
                        }
                        i=-1;
                    }
                    if(mlynki==0)   //zmiana pionka w polu "aktualny ruch"
                    {
                        SDL_BlitSurface(m_gfx->pionbg,NULL,okno,&menu[13]);
                        switch(t)
                        {
                        case 1:
                                SDL_BlitSurface(p_gfx->red,NULL,okno,&menu[11]);
                            break;
                        case -1:
                                SDL_BlitSurface(p_gfx->blue,NULL,okno,&menu[11]);
                            break;
                        }
                    }
                    SDL_Flip(okno);
                }
            }
            if(zdarzenie.type == SDL_QUIT|| ((zdarzenie.type == SDL_KEYDOWN) && (zdarzenie.key.keysym.sym==SDLK_ESCAPE)))
            {   //reakcja na [Esc] - wyjście z gry
                wyjscie = true;
                nosplash = true;
            }
            if(zdarzenie.type == SDL_KEYDOWN && zdarzenie.key.keysym.sym ==SDLK_RETURN)
            {   //jw tylko dla [Enter] w sytuacji, gdy gra została zakończona - przejście do menu
                if(finish)
                {
                    for(i=0; i<p; i++)
                        fprintf(fp,"%4d%4d%4d\n",rozgrywka->ruch[i][0],rozgrywka->ruch[i][1],rozgrywka->ruch[i][2]);
                    fprintf(fp,"%5d\n",p);
                    for(i=0; i<24; i++)
                        fprintf(fp,"%3d",t_plansza[i]);
                    fprintf(fp,"\n");
                    fprintf(fp,"%5d",mlynki);
                    fprintf(fp,"\n\n---------------------\n\n");
                    przeladuj_gfx(&okno,w_gfx,p_gfx,m_gfx,gfx_path);
                    text = TTF_RenderUTF8_Blended(font,"Młynek v1.0",text_color);
                    SDL_BlitSurface(text, NULL, okno, &menu[14]);
                    zeruj_gre(t_plansza,team,p,rozgrywka);
                    laduj_menu(okno,text_color,font,menu,1);
                    SDL_BlitSurface(w_gfx->gray,NULL,okno,NULL);
                    i=1;
                    p=0;
                    t=1;
                    wybor=false;
                    ismenu=true;
                    finish=false;
                    SDL_Flip(okno);
                }
            }
        }
    }
    if(p>0)
    {
        for(i=0; i<p; i++)
            fprintf(fp,"%4d%4d%4d\n",rozgrywka->ruch[i][0],rozgrywka->ruch[i][1],rozgrywka->ruch[i][2]);
        fprintf(fp,"%5d\n",p);
        for(i=0; i<24; i++)
            fprintf(fp,"%3d",t_plansza[i]);
        fprintf(fp,"\n");
        fprintf(fp,"%5d",mlynki);
    }
    if(nosplash==false)
    {
        SDL_FreeSurface(w_gfx->plansza);
        w_gfx->plansza=IMG_Load("gfx/endsplash.bmp");
        SDL_BlitSurface(w_gfx->plansza,NULL,okno,NULL);
        SDL_Flip(okno);
        SDL_Delay(5000);
    }
    zwolnij(w_gfx,p_gfx,m_gfx,&text,rozgrywka,p);
    TTF_CloseFont(font);
    TTF_CloseFont(font_s);
    TTF_Quit();
    SDL_Quit();
    fclose(fp);
    exit(0);
}
