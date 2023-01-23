#pragma once

#include "plot_graph.h"
#include <iostream>

class calculate_function {

public:

	void init_plot_setting() const;

	void init_plot_setting(const int width, const int height, char* plot_window_title, char* font_text_path, const int font_text_size, 
	                       char* caption_text_x, char* caption_text_y,
	                       const int scale_x, const int scale_y, const double x_end_value, const double max_y) const;

	void calculate(const int id, char* caption_text, const int color,
	               const float x_start_value, const float x_end_value, float (*math_func)(float)) const;

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
	double max_y_ = 0.;
};
