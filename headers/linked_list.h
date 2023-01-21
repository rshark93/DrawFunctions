#pragma once
#include "SDL.h"

typedef struct caption_item caption_item;

struct caption_item
{
	char * caption_txt;
	int    caption_id;
	int    caption_color;
	caption_item *nxt;
};

typedef caption_item* caption_list;

typedef struct coordinate_item coordinate_item;

struct coordinate_item
{
	float  x;
	float  y;
	int    caption_id;
	coordinate_item *nxt;
};

typedef coordinate_item* coord_list;

typedef struct surface_item surface_item;

struct surface_item
{
	SDL_Surface * surface;
	surface_item *nxt;
};

typedef surface_item* surface_list;

coord_list push_back_coord(coord_list list,int caption_id, float x,float y);

void print_list_coord(coord_list list);

coord_list clear_coord(coord_list list);

caption_list push_back_caption(caption_list list, char* caption_txt,int caption_id,int color);

void print_list_caption(caption_list list);

caption_list clear_caption(caption_list list);

surface_list push_back_surface(surface_list list, SDL_Surface* surface);

surface_list clear_surface(surface_list list);