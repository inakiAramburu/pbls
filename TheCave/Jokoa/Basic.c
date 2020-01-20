#include "Basic.h"
#include "Image.h"
#include "Event.h"
#include "Motion.h"
#include "SDL_net.h"
#include <stdio.h>
#include "Sound.h"
#include <stdlib.h>

PERTSONAIA pertsonaia;
ETSAIA etsaia[ETSAI_KOPURUA + 1];		//+1 Bossarako

PONG easteregg;

extern IMGPERTSONAIA spriteak[7];

SDL_Window* leihoa;
extern SDL_Renderer* render;
extern TIROAK jaurtigai[30];
extern int kont;


int LeihoaEtaRenderHasi()
{
	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Ezin izan da SDL hasieratu: %s", SDL_GetError());
		return 1;
	}
	// Create an application window with the following settings:
	leihoa = SDL_CreateWindow(
		"The Cave",                  			// Izenburua
		SDL_WINDOWPOS_CENTERED,           // Leihoa erdian jarri
		SDL_WINDOWPOS_CENTERED,           // Leihoa erdian jarri
		PANTAILA_ZABALERA,                // Sortzeko lehioaren zabalera
		PANTAILA_ALTUERA,                 // Sortzeko leihoaren altuera
		SDL_WINDOW_OPENGL                // Erabilitako teknologia grafikoa
	);

	if (leihoa == NULL) { 		// Leihoa sortu dela konprobatzen du

		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Ezin izan da leihoa sortu: %s\n", SDL_GetError());		//Errorea
		return 1;
	}

	SDL_Surface* ikonoa = SDL_LoadBMP(".\\media\\Ikonoa.bmp");
	SDL_SetWindowIcon(leihoa, ikonoa);
	SDL_FreeSurface(ikonoa);

	render = SDL_CreateRenderer(leihoa, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);		//Renderizatua hasi

	Musikaabiarazi();
	Soinuak[MUSIKA_JOKUA] = Mix_LoadWAV(".\\media\\sound\\MusikaGeneral\\MusikaJolasa.wav");
	Soinuak[KEA_EFEKTUA] = Mix_LoadWAV(".\\media\\sound\\effect\\Pertsonaia\\Kea.wav");
	Soinuak[EZPATA_EFEKTUA] = Mix_LoadWAV(".\\media\\sound\\effect\\Pertsonaia\\Ezpata.wav");
	Soinuak[IRABAZI_EFEKTUA] = Mix_LoadWAV(".\\media\\sound\\IrabaziGaldu\\Irabazi.wav");
	Soinuak[GALDU_EFEKTUA] = Mix_LoadWAV(".\\media\\sound\\IrabaziGaldu\\GameOver.wav");

	Soinuak[TIROA_EFEKTUA] = Mix_LoadWAV(".\\media\\sound\\effect\\boss\\Tiroak.wav");
	Soinuak[MusikaBOSS] = Mix_LoadWAV(".\\media\\sound\\boss\\MusikaBoss.wav");
	Soinuak[BOOS_KOLPE_EFEKTUA] = Mix_LoadWAV(".\\media\\sound\\boss\\hit.wav");

	Soinuak[PONG_PALA] = Mix_LoadWAV(".\\media\\sound\\pong\\pala.wav");
	Soinuak[PONG_PARETA] = Mix_LoadWAV(".\\media\\sound\\pong\\pareta.wav");
	Soinuak[PONG_IRABAZI] = Mix_LoadWAV(".\\media\\sound\\pong\\puntua.wav");

	return 0;
}

void Amaitu(SDL_bool* Jokatzen, PANTAILAK* Pantaila)
{
	if (*Pantaila == MINIJOKOA )
	{
		Konexioaitxi();
	}
	extern int aukera;
	SDL_DestroyWindow(leihoa);
	SDL_DestroyRenderer(render);
	*Jokatzen = SDL_FALSE;
	*Pantaila = ATERA;

}
void PertsonaiaHasieratu()
{
	JokalariaKargatu(".\\media\\player\\Idle.bmp", 0);
	spriteak[0].kop = 6;

	JokalariaKargatu(".\\media\\player\\Run.bmp", 1);
	spriteak[1].kop = 10;

	JokalariaKargatu(".\\media\\player\\Salto.bmp", 2);
	spriteak[2].kop = 8;

	JokalariaKargatu(".\\media\\player\\Caida.bmp", 3);
	spriteak[3].kop = 6;

	JokalariaKargatu(".\\media\\player\\Attack.bmp", 4);
	spriteak[4].kop = 8;

	JokalariaKargatu(".\\media\\player\\Dead.bmp", 5);
	spriteak[5].kop = 6;

	JokalariaKargatu(".\\media\\player\\Humo.bmp", 6);
	spriteak[6].kop = 4;
}

void EtsaiakHasieratu()
{
	int j, tmp = 0, kopurua = 5;
	for (j = 0; j < kopurua; j++)
	{
		EtsaiaKargatu(".\\media\\enemies\\Mamua.bmp", j);
		RectEraikitzailea(&etsaia[j].SrcSprite, 0,0,44,33);
		etsaia[j].DestSprite.w = 33;
		etsaia[j].DestSprite.h = 44;
	}
	tmp = j;
	for (j = tmp; j < tmp + kopurua; j++)
	{
		EtsaiaKargatu(".\\media\\enemies\\Mukitxua.bmp", j);
		RectEraikitzailea(&etsaia[j].SrcSprite, 0, 0, 44, 33);
		etsaia[j].DestSprite.w = 33;
		etsaia[j].DestSprite.h = 44;
	}
	tmp = j;
}

void GuztiakHil()
{
	for (int i = 0; i < ETSAI_KOPURUA; i++)
	{
		etsaia[i].bizirik = SDL_FALSE;
	}
}

void PertsonaiaHil()
{
	pertsonaia.bizirik = SDL_FALSE;
	if (BOSS.bizirik)
	{
		BOSS.bizirik = SDL_FALSE;
		for (int i = 0; i < 30; i++)
		{
			jaurtigai[i].pantailan = SDL_FALSE;
		}
	}
	if (kont == 3)
	{
		IrabaziGaldu(IRABAZI_EFEKTUA);
		ImgKargatu(".\\media\\menu\\IrabaziDuzu.bmp", 845, 560, 208, 76);		//Zabalera, altuera, x, y
	}
	else
	{
		IrabaziGaldu(GALDU_EFEKTUA);
		ImgKargatu(".\\media\\menu\\GalduDuzu.bmp", 840, 562, 208, 76);		//Zabalera, altuera, x, y
	}
	GuztiakHil();
	
	RenderPrestatu(0, 0, 0);
	Irudikatu();
}

void EtsaiaKendu(int pos, int BizirikDaudenEtsaiak[], int BizirikKopurua)
{
	etsaia[BizirikDaudenEtsaiak[pos]].bizirik = SDL_FALSE;
	for (int j = pos; j < BizirikKopurua - 1; j++)
	{
		BizirikDaudenEtsaiak[j] = BizirikDaudenEtsaiak[j + 1];
	}
}

void RectEraikitzailea(SDL_Rect* dest, int x, int y, int h, int w)
{
	dest->w = w;
	dest->h = h;
	dest->x = x;
	dest->y = y;
}

void BerrizJaurti()
{
	RectEraikitzailea(&easteregg.pilota, 628, 348, 25, 25);
	do
	{
		easteregg.angelua = (rand() % 90) - 45;
	} while (easteregg.angelua < 15 && easteregg.angelua > -15);
	easteregg.abiadurax = 7;
	easteregg.abiaduray = 7;
}
