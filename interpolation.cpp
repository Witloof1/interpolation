#include <SFML/Graphics.hpp>
#include <math.h>

constexpr int nSCALE = 100;
constexpr int nWIDTH = 800;
constexpr int nHEIGHT = 600;
constexpr int nNUMBER_OF_POINTS = nWIDTH / nSCALE + 1;
constexpr int nNUMBER_OF_INTERP = 50;

enum class Mode { lerp, lerp2, cerp } mode;

Mode& operator++(Mode& m)
{
	switch (m)
	{
		case Mode::lerp: 	return m = Mode::lerp2;
		case Mode::lerp2: 	return m = Mode::cerp;
		case Mode::cerp: 	return m = Mode::lerp;
	}
}

// linear interpolation
float lerp(const sf::Vector2f& p1, const sf::Vector2f& p2, float x)
{
	return p1.y + (p2.y - p1.y) * (x - p1.x) / (p2.x - p1.x);
}

// linear interpolation
float lerp2(const sf::Vector2f& p1, const sf::Vector2f& p2, float x)
{
	// This method expects (int)x to be the index in the array of points.
	x /= nSCALE;

	float a = x - (int)x;

	return ((1 - a) * p1.y + a * p2.y);
}

// cosine interpolation
float cerp(const sf::Vector2f& p1, const sf::Vector2f& p2, float x)
{
	// This method expects (int)x to be the index in the array of points.
	x /= nSCALE;

	float a = x - (int)x;
	float g = (1 - cos(3.14159265 * a)) / 2;

	return (1 - g) * p1.y + a * p2.y;
}

int main(int argc, char const *argv[])
{
	sf::RenderWindow window(sf::VideoMode(nWIDTH, nHEIGHT), "interpolation");
	window.setFramerateLimit(60);

	sf::Font font;
	font.loadFromFile("/System/Library/Fonts/Avenir.ttc");

	sf::Text text("lerp", font);
	mode = Mode::lerp;

	float (*interpolation)(const sf::Vector2f&, const sf::Vector2f&, float);

	sf::Vector2f points[nNUMBER_OF_POINTS];
	srand(1000);
	for (int i = 0; i != nNUMBER_OF_POINTS; ++i)
		points[i] = sf::Vector2f(i * nSCALE, rand() % nHEIGHT);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				++mode;
		}

		window.clear();

		sf::CircleShape point(2, 6);
		std::for_each(std::begin(points), std::end(points), [&](const sf::Vector2f& p)
		{
			point.setPosition(p);
			window.draw(point);
		});

		if (mode == Mode::lerp) {
			interpolation = &lerp;
			text.setString("lerp");
		}
		else if (mode == Mode::lerp2) {
			interpolation = &lerp2;
			text.setString("lerp2");	
		}
		else {
			interpolation = &cerp;
			text.setString("cerp");
		}

		sf::CircleShape interpPoint(1, 4);
		sf::Vector2f pos;
		for (int i = 0; i != nNUMBER_OF_POINTS - 1; ++i)
			for (int x = 0; x != nNUMBER_OF_INTERP; ++x)
			{
				pos.x = i * nSCALE + (nSCALE / nNUMBER_OF_INTERP) * x;
				
				pos.y = interpolation(points[i], points[i+1], pos.x);
				
				interpPoint.setPosition(pos);
				window.draw(interpPoint);
			}

		window.draw(text);
		window.display();
	}

	return 0;
}