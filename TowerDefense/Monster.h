#pragma once
#include <SFML/Graphics.hpp>
// типы монстров
enum class monster_type
{
	green, red, yellow
};

// класс монстра
class Monster : public sf::Drawable
{
private:
	sf::Vector2f size_;
	sf::Vector2f *position_ = new sf::Vector2f;
	sf::Texture texture_;
	
	// жив ли монстр
	bool *is_dead_ = new bool(false);

public:
	// уровень монстра и соотвественно его уровень, скорость, здоровье
	class Levels
	{
	public:
		float level;
		float speed;
		float health;
		
		void setLevel(const float level)
		{
			this->level = level;
			speed = level;
			health = level;
		}
		
		
	} levels = {1, 5, 1};

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	int *move_enum = new int(0);
	int *steps = new int(0);
	
	// конструктор для создания монстра
	explicit Monster(const monster_type type, const sf::Vector2f position = {})
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

		setSize({100, 100});
		
	}
	
	void setTexture(const std::string& path);
	void setSize(const sf::Vector2f size);
	sf::Vector2f getSize() const;
	void setPosition(const sf::Vector2f position) const;
	sf::Vector2f getPosition() const;

	bool& isDead() const
	{
		return *is_dead_;
	}

	void move(const float x, const float y) const
	{
		position_->x += x;
		position_->y += y;
	}
	
};

