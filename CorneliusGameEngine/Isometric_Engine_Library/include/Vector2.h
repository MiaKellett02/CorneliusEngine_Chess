#pragma once

//Libary includes.
#include <cmath>

//Struct definition.
struct Vector2 {
	float x, y;

	Vector2() { x = 0; y = 0; }
	Vector2(const Vector2& copy) {
		x = copy.GetX();
		y = copy.GetY();
	}

	Vector2(float a_x, float a_y) { x = a_x; y = a_y; }

	const float GetX() const { return x; }
	const float GetY() const { return y; }

	void Normalize() {
		//Calculate magnitude.
		float magnitude = std::sqrt(x * x + y * y);

		//Zero check.
		if (magnitude > 0) {
			x /= magnitude;
			y /= magnitude;
		}
	}

	float Magnitude() {
		return std::sqrt(x * x + y * y);
	}

	float SqrMagnitude() {
		return x * x + y * y;
	}

	//Operator overrides.
	Vector2 operator*(int multiplier) const {
		return Vector2(x * multiplier, y * multiplier);
	}

	Vector2 operator*(float multiplier) const {
		return Vector2(x * multiplier, y * multiplier);
	}

	Vector2 operator/(int division) const {
		return Vector2(x / division, y / division);
	}

	Vector2 operator/(float division) const {
		return Vector2(x / division, y / division);
	}

	Vector2 operator+(Vector2 other) const {
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(Vector2 other) const {
		return Vector2(x - other.x, y - other.y);
	}
};

struct Vector2Int {
	int x, y;

	Vector2Int() { x = 0; y = 0; }
	Vector2Int(const Vector2Int& copy) {
		x = copy.GetX();
		y = copy.GetY();
	}

	Vector2Int(int a_x, int a_y) { x = a_x; y = a_y; }

	const float GetX() const { return x; }
	const float GetY() const { return y; }

	void Normalize() {
		//Calculate magnitude.
		float magnitude = std::sqrt((float)(x * x + y * y));

		//Zero check.
		if (magnitude > 0) {
			//Convert to float.
			float xFloat = (float)x;
			float yFloat = (float)y;

			xFloat /= magnitude;
			yFloat /= magnitude;

			//Convert back to int by rounding.
			x = std::round(xFloat);
			y = std::round(yFloat);
		}
	}

	int Magnitude() {
		return std::round(std::sqrt((float)(x * x + y * y)));
	}

	float MagnitudeFloat() {
		return std::sqrt((float)(x * x + y * y));
	}

	int SqrMagnitude() {
		return x * x + y * y;
	}

	float SqrMagnitudeFloat() {
		return (float)(x * x + y * y);
	}

	//Operator overrides.
	Vector2Int operator*(int multiplier) const {
		return Vector2Int(x * multiplier, y * multiplier);
	}

	Vector2Int operator*(float multiplier) const {
		return Vector2Int(x * multiplier, y * multiplier);
	}

	Vector2Int operator/(int division) const {
		return Vector2Int(x / division, y / division);
	}

	Vector2Int operator/(float division) const {
		return Vector2Int(x / division, y / division);
	}

	Vector2Int operator+(Vector2Int other) const {
		return Vector2Int(x + other.x, y + other.y);
	}

	Vector2Int operator-(Vector2Int other) const {
		return Vector2Int(x - other.x, y - other.y);
	}

	bool operator==(const Vector2Int& other) const {
		return x == other.x && y == other.y;
	}
};