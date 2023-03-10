#pragma once

#include "plot_graph.h"
#include <iostream>

class calculate_function {

public:
	int graph_max_points = 21;

	void init_plot_setting() const;

	void init_plot_setting(int width, int height, char* plot_window_title, char* font_text_path, int font_text_size, 
	                       char* caption_text_x, char* caption_text_y,
	                       int scale_x, int scale_y, double x_end_value, double max_y) const;

	void calculate(int id, char* caption_text, int color,
	               float x_start_value, float x_end_value, float (*math_func)(float)) const;

	void print_result(int graph_id, std::string graph_name) const;

	[[nodiscard]] int plot_graphics() const;

	calculate_function();
	~calculate_function();
private:
	//calculate_function(const calculate_function&); // Prevent copy-construction
	//calculate_function& operator=(const calculate_function&); // Prevent assignment

	std::shared_ptr<plot_settings> plot_settings_ = nullptr;

	char* default_font_path_ = "../../fonts/Philosopher.ttf";

	double x_start_value_ = 0.;
	double x_end_value_ = 10.;
};
