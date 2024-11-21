// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

// make the code easier to type with "using namespace"
using namespace sf;
using namespace std;

// ticks on a separate thread to not be affected by lag
void runTick(int& tick, int& max, bool& life)
{
	int lol = 1;
	while (life)
	{
		if (abs(tick += lol) > max) lol = -lol;
		//cout << tick << "\n";
		this_thread::sleep_for(chrono::nanoseconds(250));
	}
}

int main()
{
	/*
	****************************************
	Pre loop initializations
	****************************************
	*/

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
	Text text, enterInput;
	text.setFont(lol);
	text.setString("Left click to place your vertices,\nRight click to start drawing.\n\nCustom options: (r)atio, (s)peed, (p)oint size.");
	text.setCharacterSize(32);

	enterInput.setFont(lol);
	enterInput.setCharacterSize(32);
	//////////////////////////////////////////


	// create and initialize variables for while loop
	int maxDistance = -1, ticks = 0, pointSize = 2, max = 0;
	size_t  generationSpeed = 3;
	float fractalRatio = -1;
	bool RMBPressed = false, life = true;
	string input;

	vector<Vector2f> vertices;
	vector<Vector2f> points;

	thread ticker(runTick, ref(ticks), ref(max), ref(life));
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
						if (fractalRatio == -1)
						{
							fractalRatio = 0.8;
							switch (vertices.size())
							{
							case 3:
								fractalRatio = 0.5;
								break;
							case 4:
								fractalRatio = 0.6666667;
								break;
							case 5:
								fractalRatio = 0.618;
								break;
							case 6:
								fractalRatio = 0.667;
								break;
							case 7:
								fractalRatio = 0.692;
								break;
							case 8:
								fractalRatio = 0.707;
								break;
							case 9:
								fractalRatio = 0.742;
								break;
							case 10:
								fractalRatio = 0.764;
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
						max = maxDistance * 4 - 2;
						//cout << "Max distance: " << maxDistance << endl;

					}

					RMBPressed = true; // once right mouse button is pressed, game moves on to drawing phase and you can no longer edit things
					//cout << "RMB: " << event.mouseButton.x << " " << event.mouseButton.y << endl;

				}
			}

			// takes in and parses text input
			if (event.type == Event::TextEntered)
			{
				if (Keyboard::isKeyPressed(Keyboard::Backspace))
				{
					if (input.size() > 1)
						input.pop_back();
				}

				else if (event.text.unicode < 128)
					input += static_cast<char>(event.text.unicode);

				if (tolower(input[0]) == 'r')
				{
					string str = "\n\n\n\nEnter float for ratio: " + input.substr(1);
					enterInput.setString(str);
				}
				else if (tolower(input[0]) == 's')
				{
					string str = "\n\n\n\nEnter int for speed: " + input.substr(1);
					enterInput.setString(str);
				}
				else if (tolower(input[0]) == 'p')
				{
					string str = "\n\n\n\nEnter int for point size: " + input.substr(1);
					enterInput.setString(str);
				}
				else if (input.size() == 1) // if not an option, pop off char to not block option
					input.pop_back();

			}
		}

		// close when esc hit
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// reset program to allow new inputs and whatnot
		if (Keyboard::isKeyPressed(Keyboard::Tilde)) // i chose the ~ key because backspace seems to have some issues
		{
			maxDistance = -1;
			fractalRatio = -1;
			generationSpeed = 3;
			pointSize = 2;
			RMBPressed = false;
			enterInput.setString("");
			input = "";
			vertices.clear();
			points.clear();
		}

		// sets custom config values when enter is hit
		if (Keyboard::isKeyPressed(Keyboard::Enter) && !RMBPressed)
		{
			static string temp;
			if (tolower(input[0]) == 'r' && (isdigit(input[1]) || (input[1] == '.' && isdigit(input[2]))))
			{
				fractalRatio = stof(input.substr(1));
				temp = "\n\n\n\nFractal ratio: " + to_string(fractalRatio);
				//cout << ratio << endl;
			}
			else if (tolower(input[0]) == 's' && (isdigit(input[1])))
			{
				generationSpeed = stoi(input.substr(1));
				temp = "\n\n\n\nPoint generation speed: " + to_string(generationSpeed);
				//cout << ratio << endl;
			}
			else if (tolower(input[0]) == 'p' && (isdigit(input[1])))
			{
				pointSize = stoi(input.substr(1));
				temp = "\n\n\n\nPoint size: " + to_string(pointSize);
				//cout << ratio << endl;
			}
			input = "";
			enterInput.setString(temp);
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
			for (size_t i = 0; i < generationSpeed; ++i)
			{
				int randomPoint = rand() % vertices.size();

				Vector2f newpoint = { (vertices[randomPoint].x - points[points.size() - 1].x) * fractalRatio + points[points.size() - 1].x,
									  (vertices[randomPoint].y - points[points.size() - 1].y) * fractalRatio + points[points.size() - 1].y };
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
			RectangleShape rect(Vector2f(pointSize, pointSize));
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

			// change color based on distance
			if (minDistance < ticks * 0.166667)
				rect.setFillColor(Color::Red);
			else if (minDistance < ticks * 0.166667 + maxDistance * 0.166667)
				rect.setFillColor(Color::Yellow);
			else if (minDistance < ticks * 0.166667 + maxDistance * 0.333333)
				rect.setFillColor(Color::Green);
			else if (minDistance < ticks * 0.166667 + maxDistance * 0.5)
				rect.setFillColor(Color::Cyan);
			else if (minDistance < ticks * 0.166667 + maxDistance * 0.666667)
				rect.setFillColor(Color::Blue);
			else
				rect.setFillColor(Color::Magenta);


			window.draw(rect);
		}

		// draw text
		if (!RMBPressed)
		{
			window.draw(text);
			window.draw(enterInput);
		}

		window.display();
	}

	life = false; // murder the thread to death
	ticker.join();
}