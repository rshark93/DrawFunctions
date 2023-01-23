#include "plot_graph.h"
#include <cmath>
#include <iostream>

struct rect {
	SDL_Rect sdl_rect{};

	rect(const int x, const int y, const int width, const int height) {
		sdl_rect.x = x;
		sdl_rect.y = y;
		sdl_rect.w = width;
		sdl_rect.h = height;
	}

	~rect() = default;
};

int plot_graph(plot_settings *params) {
	setvbuf(stdout, nullptr, _IONBF, 0);

	surface_list surface_list = nullptr;

	if(TTF_Init() == -1) {
		std::cerr << stderr <<  "Error SDL TTF_Init error : %s\n" << TTF_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		std::cerr << stderr << "Error SDL init failure : %s\n" << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	plot_struct plot;
	plot.font = nullptr;
	plot.screen = nullptr;
	plot.plot_surface = nullptr;
	plot.plot_mask_surface = nullptr;
	plot.caption_surface = nullptr;
	plot.caption_mask_surface = nullptr;
	plot.caption_x = nullptr;
	plot.caption_y = nullptr;
	plot.texture_x = nullptr;
	plot.texture_y = nullptr;
	plot.renderer = nullptr;

	//font specs
	plot.font = TTF_OpenFont(params->font_text_path, params->font_text_size);

	if (plot.font == nullptr) {
		std::cout << "Error font file read failure, check your font file\n";
		clean_plot(&plot,params,&surface_list);

		return EXIT_FAILURE;
	}

	plot.screen = SDL_CreateWindow(
		params->plot_window_title,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		params->screen_width, 
		params->screen_height,
		SDL_WINDOW_SHOWN);

	draw_plot(&plot,params, &surface_list);
	clean_plot(&plot,params, &surface_list);

	return EXIT_SUCCESS;
}		

void clean_plot(const plot_struct *plot, plot_settings *params, surface_list *surface_list)
{
	SDL_DestroyTexture(plot->texture_x);
	SDL_DestroyTexture(plot->texture_y);
	SDL_FreeSurface (plot->plot_mask_surface);
	SDL_FreeSurface (plot->plot_surface);
	SDL_FreeSurface (plot->caption_mask_surface);
	SDL_FreeSurface (plot->caption_surface);
	SDL_FreeSurface (plot->caption_x);
	SDL_FreeSurface (plot->caption_y);

	params->caption_list = clear_caption(params->caption_list);
	params->coordinate_list = clear_coord(params->coordinate_list);

	*surface_list = clear_surface(*surface_list);

	SDL_DestroyRenderer(plot->renderer);
	SDL_DestroyWindow  (plot->screen);

	SDL_Quit();
	TTF_CloseFont(plot->font);
	TTF_Quit();
}

void draw_plot(plot_struct *plot, const plot_settings *params, surface_list *surface_list)
{
	plot->renderer = SDL_CreateRenderer(plot->screen, 0, 0);

	if (plot->screen != nullptr) {
		constexpr int stroke_width = 2;
		constexpr SDL_Color font_color = {0, 0, 0};

		plot->caption_x = TTF_RenderText_Blended(plot->font, params->caption_text_x, font_color);
		plot->caption_y = TTF_RenderText_Blended(plot->font, params->caption_text_y, font_color);

		//------------ background-----------------------
		SDL_SetRenderDrawColor(plot->renderer,255,255,255,255);
		const auto screen = rect(0, 0, params->screen_width, params->screen_height);

		SDL_RenderFillRect( plot->renderer, &screen.sdl_rect );
		//---------------------------------------------

		const float plot_width = static_cast<float>(params->screen_width) * 0.8f;
		const float plot_height = static_cast<float>(params->screen_height) * 0.8f;
		const float plot_caption_height = static_cast<float>(params->screen_height) * 0.05f;

		const auto plot_position = rect(
			params->screen_width / 2 - plot_width * 0.47,
			params->screen_height * 0.50 - plot_height / 2, 
			plot_width,
			plot_height);

		const auto plot_mask_position = rect(
			plot_position.sdl_rect.x - stroke_width, 
			plot_position.sdl_rect.y - stroke_width,
			plot_width + stroke_width * 2,
			plot_height + stroke_width * 2);

		const auto plot_caption_position = rect(
			plot_position.sdl_rect.x,
			plot_position.sdl_rect.y - 20 - plot_caption_height,
			plot_width,
			plot_caption_height);

		const auto plot_caption_mask_position = rect(
			plot_caption_position.sdl_rect.x - stroke_width,
			plot_caption_position.sdl_rect.y - stroke_width,
			plot_width + stroke_width * 2,
			plot_caption_height + stroke_width * 2);

		SDL_SetRenderDrawColor(plot->renderer,0, 0, 0,255);
		SDL_RenderFillRect(plot->renderer, &plot_mask_position.sdl_rect);

		SDL_SetRenderDrawColor(plot->renderer,255, 255, 255,255);
		SDL_RenderFillRect(plot->renderer, &plot_position.sdl_rect);

		SDL_SetRenderDrawColor(plot->renderer,0, 0, 0,255);
		SDL_RenderFillRect(plot->renderer, &plot_caption_mask_position.sdl_rect);

		SDL_SetRenderDrawColor(plot->renderer,255, 255, 255,255);
		SDL_RenderFillRect(plot->renderer, &plot_caption_position.sdl_rect);

		draw_scale_graduation(plot->renderer,
		                      params,
		                      plot,
		                      plot_width,
		                      plot_height,
		                      plot_mask_position.sdl_rect,
		                      plot->font,
		                      font_color,
		                      surface_list,
		                      point_2d {
		                      	static_cast<float>(plot_position.sdl_rect.x),
		                      	static_cast<float>(plot_position.sdl_rect.y)
		                      });

		if (params->caption_list != nullptr) {
			if (const caption_item *tmp = params->caption_list; tmp != nullptr) {
				int caption_offset = caption_margin;

				while (tmp != nullptr) {
					//plot circle1
					const int circle_x1 = plot_caption_mask_position.sdl_rect.x + caption_offset;
					const int circle_y1 = plot_caption_mask_position.sdl_rect.y + plot_caption_height / 2 + stroke_width;

					SDL_SetRenderDrawColor(plot->renderer,0,0,0,255);
					fill_circle(plot->renderer,circle_x1,circle_y1,dot_radius);

					SDL_SetRenderDrawColor(plot->renderer,(tmp->caption_color & 0xFF0000) >> 16, 
						(tmp->caption_color & 0x00FF00) >> 8,
						tmp->caption_color & 0x0000FF,255);

					fill_circle(plot->renderer,circle_x1,circle_y1,dot_radius - 2);

					//plot circle2
					caption_offset += 40;

					const int circle_x2 = plot_caption_mask_position.sdl_rect.x + caption_offset;
					const int circle_y2 = circle_y1;

					SDL_SetRenderDrawColor(plot->renderer,0,0,0,255);
					fill_circle(plot->renderer,circle_x2,circle_y2,dot_radius);

					SDL_SetRenderDrawColor(plot->renderer,(tmp->caption_color & 0xFF0000) >> 16, 
						(tmp->caption_color & 0x00FF00) >> 8,
						tmp->caption_color & 0x0000FF,255);

					fill_circle(plot->renderer,circle_x2,circle_y2,dot_radius - 2);

					//draw line between two circles
					SDL_RenderDrawLine(plot->renderer,circle_x1 + dot_radius + 1,circle_y1,circle_x2 - dot_radius - 1,circle_y2);

					//display caption
					SDL_Surface *caption_text_surface = TTF_RenderText_Blended(plot->font, tmp->caption_txt, font_color);
					SDL_Rect caption_text;
					SDL_Texture * texture_text = SDL_CreateTextureFromSurface(plot->renderer, caption_text_surface);
					SDL_QueryTexture(texture_text, nullptr, nullptr, &caption_text.w, &caption_text.h);
					caption_text.x = circle_x2 + dot_radius + caption_offset_circle_to_text;
					caption_text.y = circle_y2 - caption_text.h / 2;
					SDL_RenderCopy(plot->renderer, texture_text, nullptr, &caption_text);

					*surface_list = push_back_surface(*surface_list, caption_text_surface);

					caption_offset += caption_text.w + dot_radius + caption_offset_circle_to_text + caption_offset_delimiter;

					draw_points(plot->renderer,
						tmp,
						params,
						plot_width,
						plot_height,
						plot_mask_position.sdl_rect);

					tmp = tmp->nxt;
				}
			}
		}

		SDL_RenderPresent(plot->renderer);
		wait_for_sdl_event();
	} else {
		std::cerr << stderr << "Error cant allocate memory for screen : \n" << SDL_GetError() << std::endl;
	}
}

void draw_points(
	SDL_Renderer *renderer,
	const caption_item *caption_item,
	const plot_settings *params,
	const float plot_width,
	const float plot_height,
	const SDL_Rect plot_mask_position)
{
	const coordinate_item* tmp = params->coordinate_list;

	const float scale_x_num = plot_width / (params->max_x / params->scale_x);
	const float scale_y_num = plot_height / (params->max_y / params->scale_y);

	auto is_first = true;

	float previous_x = 0;
	float previous_y = 0;

	while (tmp != nullptr) {
		if (tmp->caption_id == caption_item->caption_id) {
			const int circle_x1 = plot_mask_position.x + 1 + tmp->p.x / params->scale_x * scale_x_num;
			const int circle_y1 = plot_mask_position.y + plot_height - tmp->p.y / params->scale_y * scale_y_num;

			SDL_SetRenderDrawColor(renderer,0,0,0,255);

			fill_circle(renderer,circle_x1,circle_y1,dot_radius);

			SDL_SetRenderDrawColor(renderer,(caption_item->caption_color & 0xFF0000) >> 16, 
				(caption_item->caption_color & 0x00FF00) >> 8,
				caption_item->caption_color & 0x0000FF,255);

			fill_circle(renderer,circle_x1,circle_y1,dot_radius - 2);

			if (!is_first)
				SDL_RenderDrawLine(renderer,previous_x,previous_y,circle_x1,circle_y1);

			previous_x = circle_x1;
			previous_y = circle_y1;

			if (is_first)
				is_first = false;
		}

		tmp = tmp->nxt;
	}
}

void draw_scale_graduation(SDL_Renderer * renderer,
						   const plot_settings *params,
                           plot_struct *plot,
                           const float plot_width,
                           const float plot_height,
                           const SDL_Rect plot_mask_position,
						   TTF_Font *font,
                           const SDL_Color font_color,
                           surface_list *surface_list,
                           const point_2d plot_position
) {
	const int scale_x_num = plot_width / (params->max_x / params->scale_x);
	const int scale_y_num = plot_height / (params->max_y / params->scale_y);

	int init_pos_x = plot_mask_position.x + 1;
	int init_pos_y = plot_mask_position.y + plot_height + 1;

	auto current_scale = 0;

	const int point_number_x = params->max_x / params->scale_x;

	int regular_caption_text_height = 0;
	int regular_caption_text_width = 0;

	for (auto i = 0; i < point_number_x + 1; ++i) {
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderDrawLine(renderer,init_pos_x,init_pos_y,init_pos_x,init_pos_y - graduation_height);

		char text[10];
		sprintf_s(text,"%d",current_scale);

		SDL_Surface *caption_text_surface = TTF_RenderText_Blended(font, text, font_color);
		SDL_Rect caption_text;
		SDL_Texture * texture_text = SDL_CreateTextureFromSurface(renderer, caption_text_surface);
		SDL_QueryTexture(texture_text, nullptr, nullptr, &caption_text.w, &caption_text.h);
		caption_text.x = init_pos_x-caption_text.w / 2;
		caption_text.y = init_pos_y + 5;
		SDL_RenderCopy(renderer, texture_text, nullptr, &caption_text);

		*surface_list = push_back_surface(*surface_list,caption_text_surface);

		init_pos_x += scale_x_num;
		current_scale += params->scale_x;

		regular_caption_text_height = caption_text.h;
	}

	current_scale = 0;
	init_pos_x = plot_mask_position.x + 1;
	init_pos_y = plot_mask_position.y + plot_height;

	for (auto i = 0; i < params->max_y / params->scale_y + 1; ++i) {
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderDrawLine(renderer,init_pos_x,init_pos_y,init_pos_x+graduation_height,init_pos_y);

		char text[10];
		sprintf_s(text,"%d", current_scale);

		SDL_Surface *caption_text_surface = TTF_RenderText_Blended(font, text, font_color);
		SDL_Rect caption_text;
		SDL_Texture * texture_text = SDL_CreateTextureFromSurface(renderer, caption_text_surface);
		SDL_QueryTexture(texture_text, nullptr, nullptr, &caption_text.w, &caption_text.h);
		caption_text.x = init_pos_x-caption_text.w - 10;
		caption_text.y = init_pos_y-caption_text.h / 2;
		SDL_RenderCopy(renderer, texture_text, nullptr, &caption_text);

		*surface_list = push_back_surface(*surface_list,caption_text_surface);

		init_pos_y -= scale_y_num;
		current_scale += params->scale_y;
		regular_caption_text_width = caption_text.w;
	}

	// caption y
	SDL_Rect text_caption_y;
	plot->texture_y = SDL_CreateTextureFromSurface(plot->renderer, plot->caption_y);
	SDL_QueryTexture( plot->texture_y, nullptr, nullptr, &text_caption_y.w, &text_caption_y.h);
	text_caption_y.x = -1 * regular_caption_text_width;
	text_caption_y.y = plot_mask_position.y + plot_height / 2 + text_caption_y.w / 4.;

	// rotate caption y
	const SDL_Point caption_center= { static_cast<int>(plot_position.x) - caption_y_label_offset,0 };
	SDL_RenderCopyEx(plot->renderer, plot->texture_y, nullptr, &text_caption_y, -90, &caption_center, SDL_FLIP_NONE);

	//caption x
	SDL_Rect text_caption_x;
	plot->texture_x = SDL_CreateTextureFromSurface(plot->renderer, plot->caption_x);
	SDL_QueryTexture(plot->texture_x, nullptr, nullptr, &text_caption_x.w, &text_caption_x.h);
	text_caption_x.x = params->screen_width / 2 - text_caption_x.w / 2;
	text_caption_x.y = plot_position.y + plot_height + regular_caption_text_height;
	SDL_RenderCopy(plot->renderer, plot->texture_x, nullptr, &text_caption_x);
}

void wait_for_sdl_event() {
	SDL_Event event;

	while (true) {
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
			break;
	}
}

void draw_circle(SDL_Renderer *renderer, const int n_cx, const int n_cy, const int radius)
{
	auto error = static_cast<double>(-radius);
	const double x = static_cast<double>(radius) - 0.5;
	constexpr auto y = 0.5;
	const double cx = n_cx - 0.5;
	const double cy = n_cy - 0.5;
 
	while (x >= y) {
		SDL_RenderDrawPoint(renderer, static_cast<int>(cx + x), static_cast<int>(cy + y));
		SDL_RenderDrawPoint(renderer, static_cast<int>(cx + y), static_cast<int>(cy + x));
 
		if (x != 0.) {
			SDL_RenderDrawPoint(renderer, static_cast<int>(cx - x), static_cast<int>(cy + y));
			SDL_RenderDrawPoint(renderer, static_cast<int>(cx + y), static_cast<int>(cy - x));
		}
 
		if constexpr (y != 0.) {
			SDL_RenderDrawPoint(renderer, static_cast<int>(cx + x), static_cast<int>(cy - y));
			SDL_RenderDrawPoint(renderer, static_cast<int>(cx - y), static_cast<int>(cy + x));
		}
 
		if (x != 0. && y != 0.) {
			SDL_RenderDrawPoint(renderer, static_cast<int>(cx - x), static_cast<int>(cy - y));
			SDL_RenderDrawPoint(renderer, static_cast<int>(cx - y), static_cast<int>(cy - x));
		}
	}
}

void fill_circle(SDL_Renderer *renderer, const int cx, const int cy, const int radius) {
	static constexpr int bpp = 4;
	const auto r = static_cast<double>(radius);

	for (double dy = 1; dy <= r; dy += 1.0) {
		const double dx = floor(sqrt(2.0 * r * dy - dy * dy));

		for (int x = cx - dx; x <= cx + dx; x++) {
			SDL_RenderDrawPoint(renderer, x,static_cast<int>(cy + r - dy));
			SDL_RenderDrawPoint(renderer, x,static_cast<int>(cy - r + dy));
		}
	}
}