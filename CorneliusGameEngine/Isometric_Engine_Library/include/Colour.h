#pragma once

//Library includes.
#include <cstdint>

//Struct definition.
class Colour {
public:
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Colour() {
		r = 255;
		g = 255;
		b = 255;
	}

	Colour(uint8_t a_r, uint8_t a_g, uint8_t a_b) {
		r = a_r;
		g = a_g;
		b = a_b;
	}

	/// <summary>
	/// Clamped between 0-1
	/// </summary>
	/// <param name="a_multiplier"></param>
	void Darken(float a_multiplier) {
		//Validate input.
		if (a_multiplier < 0) {
			a_multiplier = 0;
		}
		if (a_multiplier > 1) {
			a_multiplier = 1;
		}

		//Apply the darkening.
		r = (float)r * a_multiplier;
		g = (float)g * a_multiplier;
		b = (float)b * a_multiplier;
	}
};