// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

//Make the code easier to type with "using namespace"
using namespace sf;
using namespace std;

int main()
{
	/***PRE LOOP VARIABLES***/

	// Create a video mode object
	VideoMode vm(1500, 900);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game :D", Style::Default);
	//////////////////////////////////////////


	// loads a font, end program if not found
	Font lol;
	if (!lol.loadFromFile("Mojangles.ttf"))
	{
		cout << "poopoo caca" << endl;
		return 0;
	}
	//////////////////////////////////////////


	// create text objects and set their font and font size
	Text text, customRatioText;
	text.setFont(lol);
	text.setString("Left click to place your vertices,\nRight click to start drawing.\n\nTo select a custom ratio, press r, type a float, and enter.");
	text.setCharacterSize(32);

	customRatioText.setFont(lol);
	customRatioText.setCharacterSize(32);
	//////////////////////////////////////////


	// create and initialize variables for while loop
	int maxDistance = -1;
	float ratio = -1;
	bool RMBPressed = false;
	string input;

	vector<Vector2f> vertices;
	vector<Vector2f> points;
	//////////////////////////////////////////


	/*
	****************************************
	Game loop
	****************************************
	*/
	while (window.isOpen())
	{
		/*
		****************************************
		Input handling
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			// Quit the game when the window is closed
			if (event.type == Event::Closed)
			{

				window.close();
			}


			// mouse buttons
			else if (event.type == Event::MouseButtonPressed)
			{
				input = "";
				// left click
				if (event.mouseButton.button == Mouse::Left)
				{
					//cout << "LMB: " << event.mouseButton.x << " " << event.mouseButton.y << endl;

					// only set vertices before right mouse button is pressed
					if (!RMBPressed)
					{
						vertices.push_back(Vector2f(event.mouseButton.x, event.mouseButton.y));
					}
				}


				// right click
				else if (event.mouseButton.button == Mouse::Right && vertices.size() > 0)
				{
					// only does this stuff the first time right mouse button is pressed
					if (RMBPressed == false)
					{
						// push back the last placed vertix
						points.push_back(vertices[vertices.size() - 1]);

						// defaults ratio values depending on vertix count (only sets if a custom value was not set)
						if (ratio == -1)
						{
							ratio = 0.8;
							switch (vertices.size())
							{
							case 3:
								ratio = 0.5;
								break;
							case 4:
								ratio = 0.6666667;
								break;
							case 5:
								ratio = 0.618;
								break;
							case 6:
								ratio = 0.667;
								break;
							case 7:
								ratio = 0.692;
								break;
							case 8:
								ratio = 0.707;
								break;
							case 9:
								ratio = 0.742;
								break;
							case 10:
								ratio = 0.764;
								break;
							}
						}


						// finds the distance between the two farthest points
						int distance;
						for (size_t i = 0; i < vertices.size(); ++i)
						{
							for (size_t j = 0; j < vertices.size(); ++j)
							{
								distance = sqrt(pow((vertices[i].x - vertices[j].x), 2) + pow((vertices[i].y - vertices[j].y), 2));
								if (maxDistance < distance)
									maxDistance = distance;
							}
						}
						//cout << "Max distance: " << maxDistance << endl;

					}

					RMBPressed = true; // once right mouse button is pressed, game moves on to drawing phase and you can no longer edit things
					//cout << "RMB: " << event.mouseButton.x << " " << event.mouseButton.y << endl;

				}
			}

			// takes in and parses text input
			if (event.type == Event::TextEntered)
			{
				if (event.text.unicode < 128)
				{
					input += static_cast<char>(event.text.unicode);
					cout << input << "                 " << input.size() << " \"" << input[input.size() - 1] << "\"" << endl; // will comment out later, ignore for now
					string str = "\n\n\n\nEnter float: " + input.substr(1);
					if (tolower(input[0]) == 'r') customRatioText.setString(str);
				}
			}
		}

		// close when esc hit
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Tilde)) // i chose the ~ key because backspace seems to have some issues
		{
			maxDistance = -1;
			ratio = -1;
			RMBPressed = false;
			customRatioText.setString("");
			input = "";
			vertices.clear();
			points.clear();
		}

		// sets custom ratio when enter is hit
		if (Keyboard::isKeyPressed(Keyboard::Enter) && !RMBPressed)
		{
			if (tolower(input[0]) == 'r' && (isdigit(input[1]) || (input[1] == '.' && isdigit(input[2]))))
			{
				ratio = stof(input.substr(1));
				//cout << ratio << endl;
			}
			input = "";
			string temp = "\n\n\n\n" + to_string(ratio);
			customRatioText.setString(temp);
		}
		//////////////////////////////////////////



		/*
		****************************************
		Update
		****************************************
		*/

		// set points if right mouse button is pressed
		if (RMBPressed)
		{
			for (size_t i = 0; i < vertices.size() * 3; ++i)
			{
				int randomPoint = rand() % vertices.size();

				Vector2f newpoint = { (vertices[randomPoint].x - points[points.size() - 1].x) * ratio + points[points.size() - 1].x,
									  (vertices[randomPoint].y - points[points.size() - 1].y) * ratio + points[points.size() - 1].y };
				points.push_back(newpoint);
			}
		}



		/*
		****************************************
		Draw
		****************************************
		*/
		window.clear();

		// draw vertices
		for (size_t i = 0; i < vertices.size() && !RMBPressed; ++i)
		{
			RectangleShape rect(Vector2f(10, 10));
			rect.setPosition(Vector2f(vertices[i].x, vertices[i].y));
			rect.setFillColor(Color::Blue);
			window.draw(rect);
		}

		// draw points
		for (const auto& point : points)
		{
			RectangleShape rect(Vector2f(2, 2));
			rect.setPosition(point);
			rect.setFillColor(Color::Red);
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
			if (minDistance < maxDistance / 7)
				rect.setFillColor(Color::Red);
			else if (minDistance < maxDistance / 4)
				rect.setFillColor(Color::Yellow);
			else
				rect.setFillColor(Color::Green);

			window.draw(rect);
		}

		// draw text
		if (!RMBPressed)
		{
			window.draw(text);
			window.draw(customRatioText);
		}

		window.display();
	}
}