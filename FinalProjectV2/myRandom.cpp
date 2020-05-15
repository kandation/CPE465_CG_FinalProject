#include "MyRandom.h"
#include <cmath>

MyRandom::MyRandom()
{
}

float MyRandom::Rand01() {
	return (float)(rand() % 100 + 1) / 100;
}

int MyRandom::Rand(int end) {
	return Rand(0, end);
}

int MyRandom::Rand(int start, int end) {
	int random_integer;
	int range = (end - start) + 1;

	random_integer = start + (range * rand() / (RAND_MAX + 1.0));
	return random_integer;
}
float MyRandom::Rand(float end) {
	return Rand(0.0f, end);
}

float MyRandom::Rand(float start, float end) {
	return start + (std::abs(end - start) * Rand01());
}

void MyRandom::AllRand() {
	m_size = Rand01() * Rand01() + 0.3;
	m_posX = Rand(-10, 10);
	m_r = Rand(1.0f, 3.0f);
	m_posY = Rand(-10, 10);
	m_red = Rand01();
	m_green = Rand01();
	m_blue = Rand01();
	m_width = Rand01() * Rand01() + 0.3;
	m_height = Rand01() * Rand01() + 0.3;
}

float MyRandom::GetColorRed()
{
	return m_red;
}

float MyRandom::GetColorGreen()
{
	return m_green;
}

float MyRandom::GetColorBlue()
{
	return m_blue;
}