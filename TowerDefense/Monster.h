#pragma once
#include <SFML/Graphics.hpp>
// ���� ��������
enum class monster_type
{
	green, red, yellow
};

// ����� �������
class Monster : public sf::Drawable
{
private:
	sf::Vector2f size_;
	sf::Vector2f *position_ = new sf::Vector2f;
	sf::Texture texture_;
	
	// ��� �� ������
	bool *is_dead_ = new bool(false);

public:
	// ������� ������� � ������������� ��� �������, ��������, ��������
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
	
	// ����������� ��� �������� �������
	explicit Monster(const monster_type type, const sf::Vector2f position = {})
	{
		// �������� �������� � ����������� �� ���� �������
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

