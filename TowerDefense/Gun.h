#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Monster.h"
#include "Time.h"
#define F static_cast<float>

// класс башни, который описывает ее свойства

class Bullet : public sf::Drawable
{
private:
	float *step_ = new float(0);
	const float steps_;
	const sf::Vector2f speed_;
	const sf::Vector2f position_;
	const float rotation_;
	sf::Texture texture_;
	bool* deleted_ = new bool{ false };

public:
	compl Bullet()
	{
		delete step_;
		delete deleted_;
	}
	
	explicit Bullet(const float steps, const sf::Vector2f speed, const float rotation, const sf::Vector2f position) :
		steps_(steps),
		speed_(speed),
		position_(position),
		rotation_(rotation)
	{
		if(!texture_.loadFromFile("data/textures/shot.png"))
		{
			std::cout << "Error >> Bullet >> shot >> texture >> not loaded" << std::endl;
		}
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (*step_ < steps_)
		{
			*step_ += 1;
			sf::Sprite sprite;
			sprite.setTexture(texture_);
			sprite.setPosition({ position_.x + speed_.x * *step_, position_.y + speed_.y * *step_ });
			sprite.setOrigin(F(texture_.getSize().x) / 2.0F, F(texture_.getSize().y) / 2.0F);
			sprite.setRotation(rotation_);
			target.draw(sprite);
		}
		else
		{
			*deleted_ = true;
		}
	}

	bool isComplete() const
	{
		if (*step_ >= steps_)
		{
			return true;
		}
		return false;
	}
};

class Gun final : public sf::Drawable
{
private:
	// радиус стрельбы
	float radius_ = 100;
	
	// скорость стрельбы
	float firing_speed_per_sec_;

	float bullet_speed_ = 8;
	
	// цена для улучшения
	float price_upgrade_;

	sf::Vector2f *position_ = new sf::Vector2f;
	sf::Vector2f size_;
	sf::Texture *texture_;

	sf::SoundBuffer *sound_buffer_ = new sf::SoundBuffer;
	sf::Sound *shot_ = new sf::Sound;
	
	float *rotation_ = new float(0.F);
	
	Timer *timer_ = new Timer;

	bool *position_fixed_ = new bool(false);

	std::vector<Bullet*>* bullets_ = new std::vector<Bullet*>;

	float damage_;

	int level_ = 1;

protected:
	// отрисовка пушки
	void draw(sf::RenderTarget& target,
		sf::RenderStates states) const override
	{

		for (auto i = 0; i < bullets()->size(); i++)
		{
			if (!bullets_->at(i)->isComplete())
			{
				target.draw(*(bullets_->at(i)));
			}
			else
			{
				bullets_->erase(bullets_->begin() + i);
			}
		}
		
		sf::Sprite sprite;
		sprite.setTexture(*texture_);
		sprite.setPosition(*position_);
		sprite.setScale(size_.x / F(texture_->getSize().x),
			size_.y / F(texture_->getSize().y));
		sprite.setOrigin(F(texture_->getSize().x) / 2,
			F(texture_->getSize().y) / 2);
		sprite.setRotation(*rotation_);
		target.draw(sprite);
	}

public:
	
	// конструтор для создания пушки
	explicit Gun(const sf::Vector2f size = {},
		const float radius = 0, 
		const float firing_speed_per_sec = 0,
		const float price_upgrade = 0,
		const float damage = 0,
		const std::string& texture_path = "data/textures/cannon1.png") :
		radius_(radius),
		firing_speed_per_sec_(firing_speed_per_sec),
		price_upgrade_(price_upgrade),
		size_(size), texture_(new sf::Texture),
		damage_(damage)
	{
		texture_->loadFromFile(texture_path);
		if (!sound_buffer_->loadFromFile("data/sounds/shot.wav"))
		{
			std::cout << "Error >> Gun >> shot >> music >> not loaded" << std::endl;
		}
		shot_->setBuffer(*sound_buffer_);
	}

	Gun();

	// геттеры и сеттеры

	void setLevel(
		const int level, 
		const float radius = 0,
		const float firing_speed_per_sec = 0,
		const float damage = 0) 
	{
		if (!texture_->loadFromFile("data/textures/cannon" + std::to_string(level) + ".png"))
		{
			std::cout << "Error >> Gun >> texture >> not loaded" << std::endl;
		}

		radius_ = radius;
		firing_speed_per_sec_ = firing_speed_per_sec;
		damage_ = damage;
		level_ = level;
		
	}

	int getLevel() const
	{
		return level_;
	}

	void setRadius(const float radius)
	{
		radius_ = radius;
	}

	float getRadius() const
	{
		return radius_;
	}

	void setFiringSpeed(const float firing_speed)
	{
		firing_speed_per_sec_ = firing_speed;
	}

	float getFiringSpeed() const
	{
		return firing_speed_per_sec_;
	}

	void setPriceUpgrade(const float price_upgrade)
	{
		price_upgrade_ = price_upgrade;
	}

	float getBulletSpeed() const
	{
		return bullet_speed_;
	}

	void setBulletSpeed(const float bullet_speed)
	{
		bullet_speed_ = bullet_speed;
	}
	
	float getPriceUpgrade() const
	{
		return price_upgrade_;
	}

	void setTexture(const sf::Texture& texture) const
	{
		*texture_ = texture;
	}

	void setPosition(const sf::Vector2i position) const
	{
		position_->x = position.x;
		position_->y = position.y;
	}
	void setPosition(const float x, const float y) const
	{
		position_->x = x;
		position_->y = y;
	}

	sf::Vector2f getPosition() const
	{
		return *position_;
	}

	void setSize(const sf::Vector2f size)
	{
		size_ = size;
	}

	sf::Vector2f getSize() const
	{
		return size_;
	}

	float getRotation() const
	{
		return *rotation_;
	}

	void setRotation(const float rotation) const
	{
		*rotation_ = rotation;
	}

	bool& is_fixed() const
	{
		return *position_fixed_;
	}

	void shot(Monster* monster) const
	{
		if (timer_->milliseconds() > 1000.0F / firing_speed_per_sec_)
		{
			bullets_->push_back(new Bullet(bullet_speed_,
				sf::Vector2f((monster->getPosition().x  - position_->x)
					/ bullet_speed_, (monster->getPosition().y - position_->y)
					/ bullet_speed_),
				*rotation_, *position_));
			shot_->play();
			timer_->clear();
			monster->damage(damage_);
		}
	}

	std::vector<Bullet*>* bullets() const
	{
		return bullets_;
	}
	
	void damageMonster(Monster *monster) const
	{
		monster->damage(damage_);
	}

};


