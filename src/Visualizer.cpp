#include <iostream>
#include <climits>
#include "Visualizer.h"

Visualizer::Visualizer(int width, int height, std::string musicName)
	: window()
	, music()
	, samples(nullptr)
	, songName(musicName)
	, fileOpened(music.openFromFile("resources/" + musicName) && file.openFromFile("resources/" + musicName))
	, sp(music.getSampleRate())
{
	if (fileOpened)
	{
		sf::VideoMode mode(width, height, sf::VideoMode::getDesktopMode().bitsPerPixel);
		window.create(mode, "Maravilla (" + musicName + ")", sf::Style::Titlebar | sf::Style::Close);
		samples = new sf::Int16[sp.getSize()];
		start();
	}
}

Visualizer::~Visualizer()
{
	delete[] samples;
}

void Visualizer::start()
{
	window.display();
	window.setFramerateLimit(30);
	music.play();

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					stop();
					break;
				case sf::Event::KeyPressed:
					switch (event.key.code)
					{
						case sf::Keyboard::Space:
							toggle();
							break;
						case sf::Keyboard::Escape:
							
							break;
					}
					break;
			}
		}

		if (music.getStatus() == sf::Music::Playing) 
		{
			update();
		}
		else if (music.getStatus() == sf::Music::Stopped)
		{
			stop();
		}
	}
}

void Visualizer::toggle()
{
	if (music.getStatus() == sf::Music::Playing)
	{
		music.pause();
	}
	else if (music.getStatus() == sf::Music::Paused)
	{
		music.play();
	}
}

void Visualizer::stop()
{
	music.stop();
	window.close();
}

void Visualizer::update()
{
	sf::VertexArray va(sf::Quads);

	float seconds = music.getPlayingOffset().asSeconds();
	file.seek(seconds * music.getSampleRate() * music.getChannelCount());
	file.read(samples, sp.getSize());

	sp.process(samples);

	for (int i = 0; i < sp.getSize() / 2; i++)
	{
		appendQuad(va, i, sp.getMagnitude(i));
	}

	window.clear();
	window.draw(va);
	window.display();
}

int Visualizer::width()
{
	return window.getSize().x;
}

int Visualizer::height()
{
	return window.getSize().y;
}

void Visualizer::appendPoint(sf::VertexArray& va, int i, double magnitude)
{
	double factor = width() / (sp.getSize() / double(2));
	int x = i * factor;
	int y = i > 1 ? height() - magnitude : height();
	if (i < va.getVertexCount())
	{
		va[i].position.y = (va[i].position.y + y) / 2;
	}
	else
	{
		va.append(sf::Vector2f(x, y));
	}
}

void Visualizer::appendLineStrip(sf::VertexArray& va, int i, double magnitude)
{
	appendPoint(va, i, magnitude);
}

void Visualizer::appendQuad(sf::VertexArray& va, int i, double magnitude)
{
	double factor = width() / (sp.getSize() / double(2));
	int mod = (i + 1) % int(1 / (factor - int(factor))) ? 0 : 1;
	int x = i * factor;
	int y = i > 1 ? height() - magnitude : height() - 1;
	if (i * 4 < va.getVertexCount())
	{
		va[i * 4 + 1].position.y = (va[i * 4 + 1].position.y + y) / 2;
		va[i * 4 + 2].position.y = (va[i * 4 + 2].position.y + y) / 2;
	}
	else
	{
		va.append(sf::Vector2f(x, height()));
		va.append(sf::Vector2f(x, y));
		va.append(sf::Vector2f(x + factor + mod - 1, y));
		va.append(sf::Vector2f(x + factor + mod - 1, height()));
	}
}
