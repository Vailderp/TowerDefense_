#include "Monster.h"
#define F static_cast<float>

sf::Vector2f operator / (const sf::Vector2f v, const float n)
{
	return {v.x / n, v.y / n};
}

void Monster::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite sprite;

	sprite.setPosition(*position_);
	
	sprite.setTexture(texture_);
	
	sprite.setScale(size_.x / F(texture_.getSize().x), size_.y / F(texture_.getSize().y));

	sprite.setOrigin(F(texture_.getSize().x) / 2, F(texture_.getSize().y) / 2);
	
	target.draw(sprite);
}

void Monster::setTexture(const std::string& path)
{
	texture_.loadFromFile(path);
}

void Monster::setSize(const sf::Vector2f size)
{
	size_ = size;
}

sf::Vector2f Monster::getSize() const
{
	return size_;
}

void Monster::setPosition(const sf::Vector2f position) const
{
	*position_ = position;
}

sf::Vector2f Monster::getPosition() const
{
	return *position_;
}
