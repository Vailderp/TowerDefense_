#pragma once
#include <SFML/Graphics.hpp>

#include "Gun.h"

#define F static_cast<float>

// типы монстров
enum class monster_type
{
	green, red, yellow
};

// класс монстра
class Monster final: public sf::Drawable
{
private:
	sf::Vector2f size_;
	sf::Vector2f *position_ = new sf::Vector2f;
	sf::Texture texture_;
	
	// жив ли монстр
	bool *is_dead_ = new bool(false);

public:

	float speed;
	
	float health;
	
	float all_health;

	int* move_enum = new int(0);
	
	int* steps = new int(0);

	// конструктор для создания монстра
	explicit Monster(const monster_type type, const sf::Vector2f position = {}, const float speed = 0, const float all_health = 0) :
		speed(speed),
		all_health(all_health),
		health(all_health)
	{
		// загрузка текстуры в зависимости от типа монстра
		switch (type) {
		case monster_type::green:
			setTexture("data/textures/green.png");
			break;

		case monster_type::red:
			setTexture("data/textures/red.png");
			break;

		case monster_type::yellow:
			setTexture("data/textures/yellow.png");
			break;


		default:
			break;
		}

		*position_ = position;


		setSize({ 100, 100 });

	}

	compl Monster()
	{

	}
	
	
	// уровень монстра и соотвественно его уровень, скорость, здоровье

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (!*is_dead_)
		{

			
			sf::RectangleShape green;
			sf::RectangleShape gray;
			
			green.setFillColor(sf::Color::Green);
			gray.setFillColor(sf::Color(200, 200, 200));
			
			green.setPosition(position_->x - size_.x / 2, position_->y - size_.y / 2 - 20);
			green.setSize({ (health) * (size_.x / all_health), 15.0F });
			
			gray.setPosition(position_->x - size_.x / 2 + (health) * (size_.x / all_health),
			                 position_->y - size_.y / 2 - 20);
			gray.setSize({ (all_health - health) * (size_.x / all_health) , 15.0F});
			
			target.draw(green);
			target.draw(gray);
			
			sf::Sprite sprite;

			sprite.setPosition(*position_);

			sprite.setTexture(texture_);

			sprite.setScale(size_.x / F(texture_.getSize().x), size_.y / F(texture_.getSize().y));

			sprite.setOrigin(F(texture_.getSize().x) / 2.0F, F(texture_.getSize().y) / 2.0F);

			if (health <= 0)
			{
				*is_dead_ = true;
			}

			target.draw(sprite);
		}
	}

public:
	
	void setTexture(const std::string& path)
	{
		texture_.loadFromFile(path);
	}
	
	void setSize(const sf::Vector2f size)
	{
		size_ = size;
	}
	
	sf::Vector2f getSize() const
	{
		return size_;
	}
	
	void setPosition(const sf::Vector2f position) const
	{
		*position_ = position;
	}
	
	sf::Vector2f getPosition() const
	{
		return *position_;
	}

	bool& isDead() const
	{
		return *is_dead_;
	}

	void move(const float x, const float y) const
	{
		position_->x += x;
		position_->y += y;
	}

	void damage(const float damage)
	{
		this->health -= damage;
	}

	
	
};

