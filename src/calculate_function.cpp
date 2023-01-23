#include "calculate_function.h"

#include <iomanip>
#include <vector>

void calculate_function::init_plot_setting() const {
	plot_settings_->screen_height = 680;
	plot_settings_->screen_width = 680;
	plot_settings_->plot_window_title = "Draw Functions";
	plot_settings_->font_text_path = default_font_path_;
	plot_settings_->font_text_size = 18;
	plot_settings_->caption_text_x = "X";
	plot_settings_->caption_text_y = "Y";
	plot_settings_->scale_x = 1;
	plot_settings_->scale_y = 5;
	plot_settings_->max_x = x_end_value_;
	plot_settings_->max_y = 100.f;
}

void calculate_function::init_plot_setting(const int width, const int height, char* plot_window_title,
                                           char* font_text_path, const int font_text_size, char* caption_text_x, char* caption_text_y, const int scale_x,
                                           const int scale_y, const double x_end_value, const double max_y) const {
	plot_settings_->screen_height = height;
	plot_settings_->screen_width = width;
	plot_settings_->plot_window_title = plot_window_title;
	plot_settings_->font_text_path = font_text_path;
	plot_settings_->font_text_size = font_text_size;
	plot_settings_->caption_text_x = caption_text_x;
	plot_settings_->caption_text_y = caption_text_y;
	plot_settings_->scale_x = scale_x;
	plot_settings_->scale_y = scale_y;
	plot_settings_->max_x = x_end_value;
	plot_settings_->max_y = max_y;
}

void calculate_function::calculate(const int id, char* caption_text, const int color, const float x_start_value,
                                   const float x_end_value, float(* math_func)(float)) const {
	plot_settings_->caption_list = push_back_caption(plot_settings_->caption_list, caption_text, id, color);

	const auto step = (abs(x_start_value) + abs(x_end_value)) / graph_max_points;

	for (auto x = x_start_value; x < x_end_value; x += step) {
		auto y = math_func(x);

		if (y > plot_settings_->max_y) y = plot_settings_->max_y;

		plot_settings_->coordinate_list = push_back_coord(plot_settings_->coordinate_list, id, point_2d{ x, y });
	}
}

void calculate_function::print_result(int graph_id, std::string graph_name) const {
	std::vector<point_2d> points;

	const auto x = plot_settings_->coordinate_list;
	points.push_back(x->p);

	auto x_next = x->nxt;
	points.push_back(x_next->p);

	while (true) {
		x_next = x_next->nxt;

		if (x_next == nullptr)
			break;

		points.push_back(x_next->p);
	}


	/// <summary>
	/// TODO id's and print each graph
	/// </summary>
	/// <param name="graph_id"></param>

	auto total_plots = points.size() / graph_max_points;

	std::cout << "\n" << std::setw(20) << "PLOT: " << graph_name << std::endl;

	std::cout << std::setw(5) << " " << "  ";
	std::cout << std::setw(10) << "X:";
	std::cout << std::setw(20) << "Y:" << std::endl;

	auto s = 0;
	graph_id > 0 ? s = graph_max_points + 1 : 0;

	for (auto i = s; i < graph_max_points + graph_id * s; ++i) {
		std::cout << std::setw(5) << i + 1 - graph_id * s << ". ";
		std::cout << std::setw(10) << points[i].x;
		std::cout << std::setw(20) << points[i].y << std::endl;
	}
}

int calculate_function::plot_graphics() const {
	if (const int ret = plot_graph(plot_settings_.get()); ret == EXIT_FAILURE) {
		std::cout << "plot_graph return with status \n" << ret << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

calculate_function::calculate_function() {
	std::cout << "calculate function constructor" << std::endl;

	plot_settings_ = std::make_shared<plot_settings>();
}

calculate_function::~calculate_function() {
	std::cout << "calculate_function destructor" << std::endl;
}
