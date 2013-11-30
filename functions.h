#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "stdbool.h"  //biblioteka załadowana na potrzeby typu boolean
#define wth 636
#define side 59
#define offside 46
#define dist 80
#define d_dist 580

typedef struct _rozgr
{
    int il_ruchow;      //po zakończeniu gry tu będzie ilość ruchów które wykonano w ciągu rozgrywki
    int **ruch;         //ostatnie 5 ruchów
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

#endif // FUNCTIONS_H_INCLUDED
