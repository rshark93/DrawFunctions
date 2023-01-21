#include "main.h"

#include <iostream>
#include "plot_graph.h"

int main(const int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "usage: " << argv[0] << " font file name(.ttf)\n" << std::endl;
		argv[1] = const_cast<char*>(default_font_path);
	}

	//populate caption list
	caption_list caption_list = nullptr;

	caption_list = push_back_caption(caption_list, "10^tg(x)", 0, 0x000FF);
	caption_list = push_back_caption(caption_list, "10^sin(x)", 1, 0xFF00FF);

	//populate coordinate list
	coord_list coordinate_list = nullptr;

	constexpr auto x_start_value = 0.;
	constexpr auto x_end_value = 10.;

	auto max_y = 0.0;

	for (auto x = x_start_value; x < x_end_value; x += 0.2) {
		auto y = pow(10, tan(x)); // 10^tg(x)
		const auto y_1 = pow(10, sin(x)); // 10^sin(x)

		if (y > 60.) y = 60.;
		if (y > max_y) max_y = y;

		coordinate_list = push_back_coord(coordinate_list, 0, x, y);
		coordinate_list = push_back_coord(coordinate_list, 1, x, y_1);
	}

	//populate plot parameter object
	plot_params params;

	params.screen_height = 680;
	params.screen_width = 680;
	params.plot_window_title = "Draw Functions";
	params.font_text_path = argv[1];
	params.font_text_size = 18;
	params.caption_text_x = "X";
	params.caption_text_y = "Y";
	params.caption_list = caption_list;
	params.coordinate_list = coordinate_list;
	params.scale_x = 1;
	params.scale_y = 5;
	params.max_x = x_end_value;
	params.max_y = max_y;

	if (const int ret = plot_graph(&params); ret == EXIT_FAILURE) {
		std::cout << "plot_graph return with status %d\n" << ret << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
