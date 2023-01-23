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

	calculate_obj->init_plot_setting(680, 680, "Draw Functions", argv[1], 18, "X", "Y",
		1, 5, 10.f, 60.f);

	calculate_obj->calculate(0, "10^tg(x)", 0x000FF, 0.f, 10.f, math_fun_0);
	calculate_obj->calculate(1, "10^sin(x)", 0xFF00FF, 0.f, 10.f, math_fun_1);

	calculate_obj->print_result(1);

	return calculate_obj->plot_graphics();
}
