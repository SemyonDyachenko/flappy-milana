#include <iostream>

#include <SFML/Graphics.hpp>
#include <time.h>

using namespace std;
using namespace sf;

const unsigned win_size_x = 800,win_size_y = 600;


class WallPair
{
public:
	float posX, posY;
	float speed;
	int randOffset;

	WallPair(float x, float y)
	{
		posX = x;
		posY = y;
		speed = 0.2f;
		randOffset = rand() % 250;
	}

	void Update(const float& deltatime)
	{
		posX -= speed * deltatime;

		if(posX <-50)
		{
			posX = 800;
			randOffset = rand() % 50;
		}
	}
};



class Truba
{
public:
	WallPair* obs[4];
	Texture t;
	RectangleShape shp;
	int betweenDist;



	Truba()
	{
		t.loadFromFile("wall.png");
		shp.setSize(Vector2f(t.getSize()));
		shp.setTexture(&t);

		
		
		int startPosX = 450;
		
		for(size_t i = 0;i < 4;i++)
		{
			obs[i] = new WallPair(startPosX, 600-t.getSize().y);
			startPosX += 220;
		}
		
		betweenDist = 250;
	

		
	}

	void Update(const float& deltatime)
	{
		for(size_t i = 0; i < 4;i++)
		{
			obs[i]->Update(deltatime);
		}

	}

	void Render(RenderTarget * target)
	{
		for(size_t i = 0; i < 4;i++)
		{
			shp.setPosition(obs[i]->posX, obs[i]->posY);
			target->draw(shp);
			shp.setPosition(obs[i]->posX, (-betweenDist)+obs[i]->randOffset);
			target->draw(shp);
		}
	}
};


class Background
{
public:
	
	Background(std::string path)
	{
		t[0].loadFromFile(path);
		t[1].loadFromFile(path);

		shape[0].setSize(Vector2f(t[0].getSize()));
		shape[1].setSize(Vector2f(t[1].getSize()));

		shape[0].setTexture(&t[0]);
		shape[1].setTexture(&t[1]);

		posX1 = 0;
		posX2 = 800;
		
		shape[0].setPosition(posX1, 0);
		shape[1].setPosition(posX2, 0);
	}

	void Update(const float& deltatime)
	{
		posX1 -= 0.1*deltatime;
		posX2 -= 0.1*deltatime;
		if(posX1 < -800)
		{
			posX1 = 0;
			posX2 = 800;
		}

	
		shape[0].setPosition(posX1, 0);
		shape[1].setPosition(posX2, 0);
	
		
	}
	
	void Render(RenderTarget *target)
	{
		for(size_t i = 0; i <= 1;i++ )
		{
			target->draw(shape[i]);
		}
	}

private:
	Texture t[2];
	float posX1,posX2;

	RectangleShape shape[2];
};


class Bird
{
public:
	Bird(float x,float y,Texture & texture)
	{
		posX = x;
		posY = y;

		gravity = 0.001f;
		vy = 0;

		isDeath = false;

		m_Shape.setSize(Vector2f(texture.getSize()));
		m_Shape.setTexture(&texture);

		m_Shape.setPosition(posX, posY);
	}

	virtual ~Bird()
	{
		
	}

	void setdeath(bool death)
	{
		isDeath = death;
	}

	const sf::FloatRect& GetBounds() const
	{
		return m_Shape.getGlobalBounds();
	}

	void SetPosition(float x,float y)
	{
		m_Shape.setPosition(x, y);
	}
	
	virtual void Update(const float& deltatime)
	{
	
		
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			vy = -0.3f;
		}
		vy += gravity;
		posY += vy;

		if(posY > 600)
		{
			isDeath = true;
		}
		
		m_Shape.setPosition(posX, posY);
	}

	bool GetDeath()
	{
		return isDeath;
	}

	virtual void Render(RenderTarget * target) {

		target->draw(m_Shape);
	} // here target = window

private:
	float posX, posY;
	sf::RectangleShape m_Shape;

	bool isDeath;
	
	float gravity;
	float vy;
};


int main(int argc,char** argv)
{
	srand(time(0));
	
	RenderWindow * window = new RenderWindow(VideoMode(win_size_x, win_size_y), "Flappy Game");  // non resizeable

	Truba * truba = new Truba();
	
	Texture texture;
	texture.loadFromFile("flappy.png");

	Background back("background.png");
	
	Bird *bird = new Bird(100, 100, texture);

	Clock clock;

	Texture m;
	m.loadFromFile("milana.png");

	Sprite milana(m);
	milana.setPosition(0, 0);
	
	
	while(window->isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		time = time / 800;
		clock.restart();
;		
		Event event;
		while (window->pollEvent(event))
		{
			if(event.type == Event::Closed)
			{
				window->close();
			}
		}


		window->clear(Color(10, 46, 67, 0));
		
		if (!bird->GetDeath())
		{
			bird->Update(time);
		}
		else
		{
			bird->setdeath(true);
			window->draw(milana);
		}
		back.Update(time);
		truba->Update(time);


		
		
		
		
		if (!bird->GetDeath())
		{
			back.Render(window);
			truba->Render(window);
			bird->Render(window);
		}
	
		window->display();
	}
	
}