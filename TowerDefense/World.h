#pragma once
#include <functional>
#include <iostream>
#include "Gun.h"
#include "Monster.h"

const float pi = acosf(-1);

//���� ����
enum class word_types
{
	normal, infinity
};

//����� ����, ������� ����� �������������� ����� � �������
class World final : public sf::Drawable
{
private:
	//����� ������ �����
	unsigned long long* wave_ = new unsigned long long(0);

	//������������ ����� �� ���������
	unsigned long long wave_max_ = 5;

	//������ ��������
	std::vector<Monster>* monsters_ = new std::vector<Monster>;
	//������ �����
	std::vector<Gun>* guns_ = new std::vector<Gun>;
	// �������� ��������� �������
	int spawn_speed_ = 1;
	// ���������� �������� �� ������ �����
	int mob_list_[5] = { 3, 7, 12, 16, 20 };
	//int mob_list_sum_spawns_[5] = {1, 3, 6, 8, 10};
	sf::Window* window_;
	sf::Texture world_texture_;

	sf::Vector2f draw_scale_;





	
	// �����, �� ������� ���������� ������
	sf::Vector2f move_zone_[6] =
	{
		{ 430, 15},
		{430, 230},
		{790, 230},
		{790, 615},
		{610, 615},
		{610, 680}
	};






	
	// ���������� ����� ���������������, ���� ������ ������� ����� �.� ������
	std::vector<sf::Vector2<sf::Vector2f>>* busy_zone_ = 
		new  std::vector<sf::Vector2<sf::Vector2f>>(
		{
			{{370, 0}, {500, 300}},
			{{500, 170}, {850, 300}},
			{{730, 300}, {845, 575}},
			{{550, 575}, {865, 720}},
		});

public:

	// ���������� ��� ������
	explicit World(const word_types type, sf::Window* window)
	{
		if (type == word_types::infinity)
		{
			wave_max_ = ULLONG_MAX;
		}

		window_ = window;

		world_texture_.loadFromFile("data/textures/map.png");

		draw_scale_ = { F(window_->getSize().x) / F(world_texture_.getSize().x), F(window_->getSize().y) / F(world_texture_.getSize().y) };

		newWave();
	}

	// ��������� ��� �������� ����� ����� �����
	~World()
	{
		delete wave_;
		delete monsters_;
		delete guns_;
	}

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		mobMove();

		// ������ ������� ��������
		std::vector<bool> is_all_dead;
		for (Monster const& monster : *monsters_)
		{
			is_all_dead.push_back(monster.isDead());
		}

		std::vector<bool> is_all_dead_unique;

		std::unique_copy(is_all_dead.begin(), is_all_dead.end(), std::back_inserter(is_all_dead_unique));

		// ���� ��� ������, �� ��������� �� ������ �����
		if (is_all_dead_unique.size() == 1)
		{
			*wave_ += 1;
		}

		// ���������� ����� �� ����� �������
		for (Gun const& gun : *guns_)
		{
			if (!gun.is_fixed())
			{
				gun.setPosition( sf::Mouse::getPosition(*window_).x, sf::Mouse::getPosition(*window_).y);

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (std::function<bool()>([=, &gun]()->bool
						{
							// ������ �� ���������� �� ������, ��� �������� �������
							for (sf::Vector2<sf::Vector2f> const& zone : *busy_zone_)
							{
								if (gun.getPosition().x - gun.getSize().x / 2.F > zone.x.x && gun.getPosition().x - gun.getSize().x / 2.F < zone.y.x && gun.getPosition().y - gun.getSize().y / 2.F > zone.x.y && gun.getPosition().y - gun.getSize().y / 2.F < zone.y.y)
								{
									return false;
								}

								if (gun.getPosition().x - gun.getSize().x / 2.F + gun.getSize().x > zone.x.x && gun.getPosition().x - gun.getSize().x / 2.F + gun.getSize().x < zone.y.x && gun.getPosition().y - gun.getSize().y / 2.F > zone.x.y && gun.getPosition().y - gun.getSize().y / 2.F < zone.y.y)
								{
									return false;
								}

								if (gun.getPosition().x - gun.getSize().x / 2.F > zone.x.x && gun.getPosition().x - gun.getSize().x / 2.F < zone.y.x && gun.getPosition().y + gun.getSize().y - gun.getSize().y / 2.F > zone.x.y && gun.getPosition().y - gun.getSize().y / 2.F + gun.getSize().y < zone.y.y)
								{
									return false;
								}

								if (gun.getPosition().x - gun.getSize().x / 2.F + gun.getSize().x > zone.x.x && gun.getPosition().x - gun.getSize().x / 2.F + gun.getSize().x < zone.y.x && gun.getPosition().y - gun.getSize().y / 2.F + gun.getSize().y > zone.x.y && gun.getPosition().y - gun.getSize().y / 2.F + gun.getSize().y < zone.y.y)
								{
									return false;
								}

							}
							return true;

						})())
					{
						gun.is_fixed() = true;
						busy_zone_->push_back(sf::Vector2<sf::Vector2f>
							(gun.getPosition(),
								sf::Vector2f(gun.getPosition().x
									+ gun.getSize().x, gun.getPosition().y
									+ gun.getSize().y)));

					}
				}
			}
			else
			{
				for (Monster const& monster : *monsters_)
				{
					if (fabs(monster.getPosition().x - gun.getPosition().x) < gun.getRadius() && fabs(monster.getPosition().y - gun.getPosition().y) < gun.getRadius())
					{
						gun.setRotation(atan2(monster.getPosition().y - gun.getPosition().y, monster.getPosition().x - gun.getPosition().x) * 180.F / pi);
						gun.shot(monster);
						break;
					}
				}
			}
		}

		sf::Sprite world_sprite;
		world_sprite.setTexture(world_texture_);
		world_sprite.setScale(F(target.getSize().x) / F(world_texture_.getSize().x), F(target.getSize().y) / F(world_texture_.getSize().y));
		target.draw(world_sprite);

		
		// ��������� ����� � �����
		for (Gun const& gun : *guns_)
		{
			target.draw(gun);
		}
		
		for (Monster const& monster : *monsters_)
		{
			target.draw(monster);
		}

	}


	// �������� ����� �����
	void newWave() const
	{
		monsters_->clear();
		if (*wave_ < wave_max_)
		{
			std::cout << *wave_ << std::endl;
			for (auto i = 0; i < *wave_ + 1; i++)
			{
				monsters_->push_back(Monster(monster_type::green, move_zone_[0]));
			}
		}

	}

	// ������������ �������� �� �����
	void mobMove() const
	{
		for (Monster const& monster : *monsters_)
		{
			if (!monster.isDead())
			{
				if (*monster.move_enum < 5)
				{
					monster.move((move_zone_[*monster.move_enum + 1].x - move_zone_[*monster.move_enum].x) * (monster.levels.speed / 1000) * draw_scale_.x, (move_zone_[*monster.move_enum + 1].y - move_zone_[*monster.move_enum].y) * (monster.levels.speed / 1000) * draw_scale_.y);
					*monster.steps += 1;
					if (*monster.steps > 1000 / monster.levels.speed)
					{
						*monster.move_enum += 1;
						*monster.steps = 0;
					}
				}
				else
				{
					monster.isDead() = true;
				}
			}
		}
	}

	// �������� ����� �����
	void newGun() const
	{
		guns_->push_back(Gun({ 100, 100 }, 300, 1.5f));
	}
};