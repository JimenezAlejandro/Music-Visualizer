#pragma once
#include "SignalProcessor.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>

class Visualizer {
public:
	Visualizer(int width, int height, std::string musicName);
	~Visualizer();
private:
	std::string songName;
	sf::Music music;
	sf::InputSoundFile file;
	bool fileOpened;
	sf::Int16* samples;
	sf::RenderWindow window;
	dsp::SignalProcessor sp;
	void start();
	void toggle();
	void stop();
	void update();
	int width();
	int height();
	void appendPoint(sf::VertexArray& va, int i, double magnitude);
	void appendLineStrip(sf::VertexArray& va, int i, double magnitude);
	void appendQuad(sf::VertexArray& va, int i, double magnitude);
};