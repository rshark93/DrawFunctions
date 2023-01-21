#include "linked_list.h"
#include "SDL.h"
#include <iostream>

caption_list clear_caption(caption_list list) {
	if (list!= nullptr) {
		caption_item * current = list;

		while (current != nullptr) {	
			caption_item* next = current->nxt;
			free(current);
			current = next;
		}

		list = nullptr;
	}

	return list;
}

caption_list push_back_caption(const caption_list list, char * caption_txt, const int caption_id, const int color) {
	const auto caption_new_item = new caption_item[sizeof(caption_item)];
	caption_new_item->caption_txt = caption_txt;
	caption_new_item->caption_id = caption_id;
	caption_new_item->caption_color = color;

	caption_new_item->nxt = nullptr;

	if (list == nullptr)
		return caption_new_item;

	caption_item* temp = list;
	while(temp->nxt != nullptr) {
		temp = temp->nxt;
	}

	temp->nxt = caption_new_item;
	return list;
}

void print_list_caption(const caption_list list) {
	if (const caption_item *tmp = list; tmp != nullptr)
	{
		while(tmp != nullptr) {
			printf("%s => %d;", tmp->caption_txt, tmp->caption_id);
			tmp = tmp->nxt;
		}
		printf("\n");
	}

}

coord_list clear_coord(coord_list list) {

	if (list != nullptr) {
		coordinate_item * current = list;

		while(current != nullptr) {	
			coordinate_item* next = current->nxt;
			free(current);
			current=next;
		}

		list = nullptr;
	}

	return list;
}

coord_list push_back_coord(const coord_list list, const int caption_id, const float x, const float y) {
	const auto coord_new_item = new coordinate_item[sizeof(coordinate_item)];
	coord_new_item->x = x;
	coord_new_item->y = y;
	coord_new_item->caption_id = caption_id;
	coord_new_item->nxt = nullptr;

	if(list == nullptr)
		return coord_new_item;

	coordinate_item* temp=list;
	while(temp->nxt != nullptr) {
		temp = temp->nxt;
	}

	temp->nxt = coord_new_item;
	return list;
}

void print_list_coord(const coord_list list) {
	if (const coordinate_item *tmp = list; tmp != nullptr) {
		while(tmp != nullptr) {
			printf("(%f,%f) ", tmp->x,tmp->y);
			tmp = tmp->nxt;
		}

		printf("\n");
	}

}

surface_list clear_surface(surface_list list) {

	if (list != nullptr) {
		surface_item * current = list;

		while(current != nullptr) {	
			surface_item* next = current->nxt;
			SDL_FreeSurface(current->surface);
			free(current);
			current=next;
		}

		list = nullptr;
	}

	return list;
}

surface_list push_back_surface(const surface_list list, SDL_Surface* surface) {
	const auto surface_new_item = new surface_item[sizeof(surface_item)];
	surface_new_item->surface = surface;
	surface_new_item->nxt = nullptr;

	if (list == nullptr)
		return surface_new_item;

	surface_item* temp = list;

	while(temp->nxt != nullptr) {
		temp = temp->nxt;
	}

	temp->nxt = surface_new_item;
	return list;
}
