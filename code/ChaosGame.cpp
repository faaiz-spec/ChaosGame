// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib> 
#include <cmath>
#include <ctime>

//Make the code easier to type with "using namespace"
using namespace sf;
using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game!!", Style::Default);
	
	vector<Vector2f> vertices;
	vector<Vector2f> points;

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
		    if (event.type == Event::Closed)
		    {
					// Quit the game when the window is closed
					window.close();
		    }
		    if (event.type == sf::Event::MouseButtonPressed)
		    {
			if (event.mouseButton.button == sf::Mouse::Left)
			{
			    std::cout << "the left button was pressed" << std::endl;
			    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
			    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
	
			    if(vertices.size() < 3)
			    {
				vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
			    }
			    else if(points.size() == 0)
			    {
				///fourth click
				///push back to points vector
					points.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
			    }
			}
		    }
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == Keyboard::R)
				{
					// Reset the game
					vertices.clear();
					points.clear();
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/*
		****************************************
		Update
		****************************************
		*/
	
		if(points.size() > 0)
		{
		    ///generate more point(s)
		    ///select random vertex
		    ///calculate midpoint between random vertex and the last point in the vector
		    ///push back the newly generated coord.
			for (int i = 0; i < 5; ++i) // Generate 5 points per frame for faster visualization
			{
				// Select a random vertex
				int randomIndex = rand() % vertices.size();
				// Calculate the midpoint between the random vertex and the last point
				Vector2f lastPoint = points.back();
				Vector2f randomVertex = vertices[randomIndex];
				Vector2f newPoint((lastPoint.x + randomVertex.x) / 2, (lastPoint.y + randomVertex.y) / 2);
				// Push back the newly generated point
				points.push_back(newPoint);
			}
		}
	
		/*
		****************************************
		Draw
		****************************************
		*/
		window.clear();
		for(int i = 0; i < vertices.size(); i++)
		{
		    RectangleShape rect(Vector2f(10,10));
		    rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
			//rect.setPosition(vertices[i]);
			rect.setFillColor(Color::Blue);
		    window.draw(rect);
		}
		for (const auto& point : points)
		{
			RectangleShape rect(Vector2f(2, 2)); // Smaller points
			rect.setPosition(point);
			//rect.setFillColor(Color::Red);
			//window.draw(rect);
			float minDistance = std::numeric_limits<float>::max();
			for (const auto& vertex : vertices)
			{
				float distance = sqrt(pow(point.x - vertex.x, 2) + pow(point.y - vertex.y, 2));
				if (distance < minDistance)
				{
					minDistance = distance;
				}
			}

			// Change color based on distance
			if (minDistance < 50)
				rect.setFillColor(Color::Red);
			else if (minDistance < 100)
				rect.setFillColor(Color::Yellow);
			else
				rect.setFillColor(Color::Green);

			window.draw(rect);
		}

		window.display();
	}
}
