#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "stdbool.h"  //biblioteka załadowana na potrzeby typu boolean

#define wth 636

typedef struct _rozgr
{
    int il_ruchow;      //po zakończeniu gry tu będzie ilość ruchów które wykonano w ciągu rozgrywki
    int **ruch;         //przebieg gry
    int victory_t;      //po zakończeniu gry tu będzie id wygranej drużyny (inicjalizowane zerem)
} rozgr;

typedef struct _gl_okno    //okno główne i jego grafiki
{
    SDL_Surface *plansza,   //tło planszy
                *plansza1,  //nakładka zasłaniająca pola po przeładowaniu
                *gray;      //szara nakładka na planszę
} gl_okno;

typedef struct _gl_pionki  //grafiki pionków i kształty podobne
{
    SDL_Surface *red,       //czerwony/biały
                *blue,      //niebieski/czarny
                *select,    //zaznaczenie
                *cross;     //skreślenie
} gl_pion;
typedef struct _gl_menu //grafiki menu
{
    SDL_Surface *text,              //płaszczyzna tekstu
                *licznik,
                *menubg,            //tło menu
                *pionbg,	        //tło pionka
                *wyjscie;           //przycisk wyjscie
} gl_menu;

void ladujgrafike(char *path,char *filename,SDL_Surface **gfx);
void param_react(int argc, char *args[], char *gfx_path, SDL_Color *text_color, bool *ismenu, bool *nosplash);
void inicjuj_gre(SDL_Surface **okno,gl_okno *w_gfx, gl_pion *p_gfx, gl_menu *m_gfx, char *gfx_path, FILE **fp);
void przeladuj_gfx(SDL_Surface **okno,gl_okno *w_gfx, gl_pion *p_gfx, gl_menu *m_gfx,char *gfx_path);
void rozstaw_gre(SDL_Surface **okno, SDL_Surface *plansza, gl_menu *m_gfx, SDL_Surface *red, SDL_Rect *menu, char *l_pion, TTF_Font *font,TTF_Font *font_s, SDL_Color text_color);
void oblicz_pola(SDL_Rect *team1, SDL_Rect *team2, SDL_Rect *pole, SDL_Rect *menu);
void laduj_menu(SDL_Surface *okno, SDL_Color text_color, TTF_Font *font, SDL_Rect *menu, int i);
void przeladuj(gl_pion *p_gfx, SDL_Surface **okno, SDL_Surface *plansza1,int *t_plansza, SDL_Rect *pole);
int znajdz_pole(int myszkaX, int myszkaY, SDL_Rect *pole);
int znajdz_mlynek(int miejsce, int team, int *t_plansza);
bool znajdz_niemlynek(int team, int *t_plansza, SDL_Surface *menubg, SDL_Surface *select, SDL_Surface **okno, SDL_Rect *pole, SDL_Rect *menu, TTF_Font *font, SDL_Color text_color);
bool sprawdz_mozliwosc(int p, int *t_plansza);
bool sprawdz_ruch(int p1, int p2);
bool przestaw_pion(int p1, int p2,int t, int *team, int *t_plansza,gl_pion *p_gfx,SDL_Surface **okno, SDL_Surface *plansza1, SDL_Rect *pole);
void dodaj_ruch(int p1, int p2, int p, int t, rozgr *rozgrywka);
bool sprawdz_powtorzenia(int p1, int p2, rozgr *rozgrywka, int p);
void usun_pion(int p, int t, int *t_plansza, int *team, SDL_Surface *cross, SDL_Surface **okno, SDL_Rect *team1, SDL_Rect *team2);
bool wyjscie_b(SDL_Surface **okno, char *gfx_path);
void zeruj_gre(int *t_plansza, int *team, int p, rozgr *rozgrywka);
void zwolnij(gl_okno *w_gfx, gl_pion *p_gfx, gl_menu *m_gfx, SDL_Surface **text, rozgr *rozgrywka, int p);

#endif
