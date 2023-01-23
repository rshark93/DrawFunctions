#include "main.h"

#include <iostream>
#include "calculate_function.h"

// 10^tg(x)
float math_fun_0(const float x) {
	return static_cast<float>(pow(10, tan(x)));
}

// 10^sin(x)
float math_fun_1(const float x) {
	return static_cast<float>(pow(10, sin(x)));
}

int main(const int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "default usage: " << argv[0] << " fonts: "<< default_font_path.substr(default_font_path.find_last_of("/\\") + 1) << std::endl;
		argv[1] = default_font_path.data();
	}

	const auto calculate_obj = std::make_unique<calculate_function>();

	constexpr auto x_end = 6.f;
	constexpr auto max_y = 60.f;

	constexpr char* first_fun_name = "10^tg(x)";
	constexpr char* second_fun_name = "10^sin(x)";

	calculate_obj->init_plot_setting(680, 680, "Draw Functions", argv[1], 18, "X", "Y",
		1, 5, x_end, max_y);

	calculate_obj->calculate(0, first_fun_name, 0x000FF, 0.f, x_end, math_fun_0);
	calculate_obj->calculate(1, second_fun_name, 0xFF00FF, 0.f, x_end, math_fun_1);

	calculate_obj->print_result(0, first_fun_name);
	calculate_obj->print_result(1, second_fun_name);

	return calculate_obj->plot_graphics();
}
