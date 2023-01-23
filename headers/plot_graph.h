#pragma once

#include "SDL_ttf.h"
#include "linked_list.h"

enum {
	dot_radius = 4,
	caption_offset_circle_to_text = 10,
	caption_offset_delimiter = 40,
	graduation_height = 10,
	caption_y_label_offset = 30,
	caption_margin = 30
};

typedef struct plot_settings_struct {
	int screen_width;
	int screen_height;
	char *plot_window_title;
	char *font_text_path;
	int font_text_size;
	char *caption_text_x;
	char *caption_text_y;
	caption_list caption_list;
	mutable coord_list coordinate_list;
	double scale_x;
	double scale_y;
	double max_x;
	double max_y;
} plot_settings;

typedef struct plot_struct {
	TTF_Font *font;
	SDL_Window *screen;
	SDL_Surface *plot_surface;
	SDL_Surface *plot_mask_surface;
	SDL_Surface *caption_surface;
	SDL_Surface *caption_mask_surface;
	SDL_Surface *caption_x;
	SDL_Surface *caption_y;
	SDL_Texture *texture_x;
	SDL_Texture *texture_y;
	SDL_Renderer *renderer;
} plot_struct;

int plot_graph(plot_settings *params);

void draw_scale_graduation(SDL_Renderer * renderer, const plot_settings *params, plot_struct *plot, float plot_width, float plot_height,
	SDL_Rect plot_mask_position, TTF_Font *font, SDL_Color font_color, surface_list *surface_list, point_2d plot_position);

void draw_points(SDL_Renderer* renderer, const caption_item* caption_item, const plot_settings *params, float plot_width, float plot_height,
	SDL_Rect plot_mask_position);

void draw_plot(plot_struct *plot, const plot_settings *params, surface_list *surface_list);

void wait_for_sdl_event();

void clean_plot(const plot_struct *plot, plot_settings *params, surface_list *surface_list);

void draw_circle(SDL_Renderer *renderer, int n_cx, int n_cy, int radius);

void fill_circle(SDL_Renderer *renderer, int cx, int cy, int radius);