#ifndef MYRANDOM_H
#define MYRANDOM_H

class MyRandom
{
private:
	float m_size, m_posX, m_posY, m_red, m_green, m_blue, m_r;
	float m_width, m_height;
public:
	MyRandom();
	float Rand01();
	float Rand(float end);
	float Rand(float start, float end);
	int Rand(int end);
	int Rand(int start, int end);

	void AllRand();

	float GetColorRed();
	float GetColorGreen();
	float GetColorBlue();
};

#endif
