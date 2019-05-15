#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

//dimensions of window
int num_vertBox = 30;
int num_horzBox = 30;
int pixSize = 16;

//dimensions of window in pixels
int width = pixSize * num_horzBox;
int height = pixSize * num_vertBox;

//pad struct and instantiation
struct Pad{
	int x, y, padDirection;
}pad1;

Pad pad2;

//pad movement function
void padMove(Pad& pad)
{
	//moves pad by two (16x16) boxes
	if (pad.padDirection == 0)
		pad.x -= 2;
	if (pad.padDirection == 1)
		pad.x += 2;

	//boundary check for pad (in coordinates(16x16) not pixels)
	if (pad.x > num_horzBox - 2)
		pad.x = num_horzBox - 2;
	if (pad.x < 2)
		pad.x = 2;
}

//ball struct and instantiation
struct Ball
{
	int x, y;
}ball;

//velocity of ball held here
sf::Vector2i ballVelocity(0, 0);

//variables for starting ball and checking what side it went out of bounds from
bool startBall = false;
bool outFromBottom = false;

//ball movement function, uses ballVelocity to update itself
void ballMove()
{
	//starts or restarts ball from middle
	if (startBall && ballVelocity.x == 0 && ballVelocity.y == 0)
	{
		if (outFromBottom)
		{
			ballVelocity.x = 1;
			ballVelocity.y = 1;
		}
		else
		{
			ballVelocity.x = -1;
			ballVelocity.y = -1;
		}
	}

	//boundary checks for walls
	if (ball.x > num_horzBox - 1)
	{
		ball.x = num_horzBox - 1;
		ballVelocity.x = -1;
	}
	if (ball.x < 1)
	{
		ball.x = 1;
		ballVelocity.x = 1;
	}

	//resets ball after going out of bounds
	if (ball.y > 29 || ball.y < 2)
	{
		//checks which side it went out of bounds from
		if (ball.y > 29)
			outFromBottom = true;
		else
			false;

		ball.y = 15;
		ball.x = 15;
		ballVelocity.x = 0;
		ballVelocity.y = 0;
	}

	//ball movement x and y coordinates updated
	ball.x += ballVelocity.x;
	ball.y += ballVelocity.y;
}


//checks if paddles and ball collide
bool collides(sf::RectangleShape& padShape, sf::RectangleShape& ballShape, Pad& pad)
{
	sf::Vector2f padPos = padShape.getPosition();
	sf::Vector2f ballPos = ballShape.getPosition();

	float deltaX = abs(padPos.x - ballPos.x);
	float deltaY = abs(padPos.y - ballPos.y);

	sf::Vector2f padHalfSize(padShape.getLocalBounds().width/ 2.0f, padShape.getLocalBounds().height / 2.0f);
	sf::Vector2f ballHalfSize(ballShape.getLocalBounds().width / 2.0f, ballShape.getLocalBounds().height / 2.0f);

	float collisionX = deltaX - (padHalfSize.x + ballHalfSize.x);
	float collisionY = deltaY - (padHalfSize.y + ballHalfSize.y);

	//checks what direction pad is going at the time of collision to determine what side ball should go to
	if (collisionX < 0 && collisionY < 0)
	{
		if (pad.padDirection == 0)
			ballVelocity.x = -2;
		if (pad.padDirection == 1)
			ballVelocity.x = 2;
		return true;
	}
	
	return false;
}

int main()
{
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(width, height), "Pong");

	//background sprite initialized
	sf::Texture t1;
	t1.loadFromFile("image/black.png");
	sf::Sprite bgSprite(t1);

	//Pad shape instantiations and new origin sets (Pad1)
	sf::RectangleShape padRect;
	padRect.setSize(sf::Vector2f(64.0f, 8.0f));
	padRect.setFillColor(sf::Color::Cyan);

	float padMidX = (padRect.getLocalBounds().width) / 2.0f;
	float padMidY = (padRect.getLocalBounds().height) / 2.0f;
	padRect.setOrigin(padMidX, padMidY);

	//Pad2
	sf::RectangleShape padRect2;
	padRect2.setSize(sf::Vector2f(64.0f, 8.0f));
	padRect2.setFillColor(sf::Color::Green);

	float padMidX2 = (padRect2.getLocalBounds().width) / 2.0f;
	float padMidY2 = (padRect2.getLocalBounds().height) / 2.0f;
	padRect2.setOrigin(padMidX2, padMidY2);

	//ball shape instantiation and new origin set
	sf::RectangleShape ballRect;
	ballRect.setSize(sf::Vector2f(8.0f, 4.0f));
	ballRect.setFillColor(sf::Color::White);

	float ballMidX = (ballRect.getLocalBounds().left + ballRect.getLocalBounds().width) / 2.0f;
	float ballMidY = (ballRect.getLocalBounds().top + ballRect.getLocalBounds().height) / 2.0f;
	ballRect.setOrigin(ballMidX, ballMidY);

	//initial pad positions
	pad1.x = 15;
	pad1.y = 29;

	pad2.x = 15;
	pad2.y = 3;

	//ball initial position
	ball.x = 15;
	ball.y = 15;

	//timer variables for function calls
	sf::Clock clock;
	float timer = 0.0f;
	float delay = 0.065f;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		//checks for closing of window
		sf::Event e;
		while (window.pollEvent(e))
			if (e.type == sf::Event::Closed)
				window.close();

		//pad1 Controls
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			pad1.padDirection = 0;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			pad1.padDirection = 1;
		else
			pad1.padDirection = -1;

		//pad2 Controls
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			pad2.padDirection = 0;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			pad2.padDirection = 1;
		else
			pad2.padDirection = -1;

		//start ball controls
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			startBall = true;
		else
			startBall = false;

		if (timer > delay)
		{
			timer = 0;

			//pad movement func call
			padMove(pad1);
			padMove(pad2);

			//checks collision of ball with either of the two pads
			if (collides(padRect, ballRect, pad1))
				ballVelocity.y = -2;
			if (collides(padRect2, ballRect, pad2))
				ballVelocity.y = 2;
			
			ballMove();
		}

		window.clear();

		//draws background
		for (int i = 0; i < num_horzBox; i++)
		{
			for (int j = 0; j < num_vertBox; j++)
			{
				bgSprite.setPosition(i * pixSize, j * pixSize);
				window.draw(bgSprite);
			}
		}

		//Draws ball
		ballRect.setPosition(ball.x * pixSize, ball.y * pixSize);
		window.draw(ballRect);

		//Draws both pads
		padRect.setPosition(pad1.x * pixSize, pad1.y * pixSize);
		window.draw(padRect);

		padRect2.setPosition(pad2.x * pixSize, pad2.y * pixSize);
		window.draw(padRect2);

		window.display();

	}

	return 0;
}