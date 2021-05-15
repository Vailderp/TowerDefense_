#pragma once
#include <functional>
#include <iostream>
#include "Gun.h"
#include "Monster.h"

const float pi = acosf(-1);

//Типы мира
enum class word_types
{
	normal, infinity
};

//Класс мира, который будет контролировать башни и монстры
class World final : public sf::Drawable
{
private:
	//Какая сейчас волна
	unsigned long long* wave_ = new unsigned long long(0);

	//Максимальная волна по умолчанию
	unsigned long long wave_max_ = 5;

	//Массив монстров
	std::vector<Monster>* monsters_ = new std::vector<Monster>;
	
	//Массив башен
	std::vector<Gun>* guns_ = new std::vector<Gun>;
	
	// скорость появления монстра
	float spawn_speed_per_second = 1.01F;
	
	// количество монстров на каждую волну
	int mob_list_[5] = { 3, 7, 12, 16, 20 };
	
	//int mob_list_sum_spawns_[5] = {1, 3, 6, 8, 10};
	sf::Window* window_;
	
	sf::Texture world_texture_;

	sf::Vector2f draw_scale_;

	word_types type_;

	std::vector<Monster> *monsters_in_spawn_process_ = new std::vector<Monster>;

	Timer timer_;

	bool stopped_ = false;

	std::function<void(Monster*)> on_monster_remove_ =
	[](Monster* monster)
	{

	};
	
	std::function<void(Monster)> on_monster_out_ = 
	[](Monster monster)
	{
		
	};

	std::function<void()> on_win_ =
		[]()
	{

	};

	// точки, по которым двигаюется монстр
	sf::Vector2f move_zone_[6] =
	{
		{ 430, 15},
		{430, 230},
		{790, 230},
		{790, 615},
		{610, 615},
		{610, 680}
	};

	// координаты точек прямоугольников, куда нельзя ставить пушку т.е дорога
	std::vector<sf::Vector2<sf::Vector2f>>* busy_zone_ = 
		new  std::vector<sf::Vector2<sf::Vector2f>>(
		{
			{{370, 0}, {500, 300}},
			{{500, 170}, {850, 300}},
			{{730, 300}, {845, 575}},
			{{550, 575}, {865, 720}},
		});

public:
	World() = default;
	// конструтор для класса
	explicit World(const word_types type, sf::Window* window) :
		type_(type)
	{
		if (type == word_types::infinity)
		{
			wave_max_ = ULLONG_MAX;
		}

		window_ = window;

		world_texture_.loadFromFile("data/textures/map.png");

		draw_scale_ = {
			F(window_->getSize().x) / F(world_texture_.getSize().x),
			F(window_->getSize().y) / F(world_texture_.getSize().y)
		};

		newWave(0);
	}

	// деструтор для удаления всего после волны
	~World()
	{
		delete wave_;
		delete monsters_;
		delete guns_;
	}

public:

	void setMode(const word_types type)
	{
		if (type == word_types::infinity)
		{
			wave_max_ = ULLONG_MAX;
		}
		else
		{
			wave_max_ = 5;
		}
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (!stopped_)
		{
			mobMove();

			if (!monsters_in_spawn_process_->empty())
			{
				if (timer_.milliseconds() > 1000.0F / spawn_speed_per_second)
				{
					monsters_->push_back(monsters_in_spawn_process_->at(0));
					monsters_in_spawn_process_->erase(monsters_in_spawn_process_->begin());
					timer_.clear();
				}
			}

			// массив умерших монстров
			std::vector<bool> is_all_dead;
			for (Monster const& monster : *monsters_)
			{
				is_all_dead.push_back(monster.isDead());
			}

			std::vector<bool> is_all_dead_unique;

			std::unique_copy(is_all_dead.begin(), is_all_dead.end(), std::back_inserter(is_all_dead_unique));

			// постановка пушки на место курсора
			for (Gun const& gun : *guns_)
			{
				if (!gun.is_fixed())
				{
					gun.setPosition(sf::Mouse::getPosition(*window_).x, sf::Mouse::getPosition(*window_).y);

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						if (std::function<bool()>([=, &gun]()->bool
							{
								// защита от постановки на дорогу, где движутся монстры
								for (sf::Vector2<sf::Vector2f> const& zone : *busy_zone_)
								{
									if (gun.getPosition().x - gun.getSize().x / 2.F >
										zone.x.x && gun.getPosition().x - gun.getSize().x / 2.F <
										zone.y.x && gun.getPosition().y - gun.getSize().y / 2.F >
										zone.x.y && gun.getPosition().y - gun.getSize().y / 2.F < zone.y.y)
									{
										return false;
									}

									if (gun.getPosition().x - gun.getSize().x / 2.0F + gun.getSize().x >
										zone.x.x && gun.getPosition().x - gun.getSize().x / 2.F + gun.getSize().x <
										zone.y.x && gun.getPosition().y - gun.getSize().y / 2.F >
										zone.x.y && gun.getPosition().y - gun.getSize().y / 2.F < zone.y.y)
									{
										return false;
									}

									if (gun.getPosition().x - gun.getSize().x / 2.0F >
										zone.x.x && gun.getPosition().x - gun.getSize().x / 2.0F <
										zone.y.x && gun.getPosition().y + gun.getSize().y - gun.getSize().y / 2.0F >
										zone.x.y && gun.getPosition().y - gun.getSize().y / 2.F + gun.getSize().y < zone.y.y)
									{
										return false;
									}

									if (gun.getPosition().x - gun.getSize().x / 2.F + gun.getSize().x >
										zone.x.x && gun.getPosition().x - gun.getSize().x / 2.0F + gun.getSize().x <
										zone.y.x && gun.getPosition().y - gun.getSize().y / 2.0F + gun.getSize().y >
										zone.x.y && gun.getPosition().y - gun.getSize().y / 2.0F + gun.getSize().y < zone.y.y)
									{
										return false;
									}

									if (gun.getPosition().x - gun.getSize().x / 2.F + gun.getSize().x > 0.80F * 1280)
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
					for (auto i = 0; i < monsters_->size(); i++)
					{

						if (!monsters_->at(i).isDead())
						{
							if (fabs(monsters_->at(i).getPosition().x - gun.getPosition().x) < gun.getRadius() &&
								fabs(monsters_->at(i).getPosition().y - gun.getPosition().y) < gun.getRadius())
							{
								gun.setRotation(atan2f(monsters_->at(i).getPosition().y - gun.getPosition().y,
									monsters_->at(i).getPosition().x - gun.getPosition().x) * 180.F / pi);
								gun.shot(&monsters_->at(i));
								break;
							}
						}
						else
						{
							on_monster_remove_(&monsters_->at(i));
							monsters_->erase(monsters_->begin() + i);
						}

					}
				}
			}

			sf::Sprite world_sprite;
			world_sprite.setTexture(world_texture_);
			world_sprite.setScale(F(target.getSize().x) / F(world_texture_.getSize().x),
				F(target.getSize().y) / F(world_texture_.getSize().y));
			target.draw(world_sprite);


			// отрисовка мобов и пушек
			for (Gun const& gun : *guns_)
			{
				target.draw(gun);
			}

			bool win = true;
			for (Monster const& monster : *monsters_)
			{
				if (!monster.isDead())
				{
					win = false;
				}
				target.draw(monster);
			}
			
			if (win && (*wave_ >= wave_max_))
			{
				on_win_();
			}

			
		}
	}


public:
	// создание новой волны
	void newWave(const unsigned long long wave) const
	{
		if (*wave_ < wave_max_)
		{
			*wave_ = wave;
			for (auto i = 0; i < *wave_ * 1.4; i++)
			{

				const int rand = math::rand(0, 2);

				if (rand == 0)
				{
					if (type_ == word_types::normal)
					{
						monsters_in_spawn_process_->push_back(Monster(monster_type::green, move_zone_[0], 1.0F * wave, 50.0F * wave));
					}
					else
					{
						monsters_in_spawn_process_->push_back(Monster(monster_type::green, move_zone_[0], 0.50F * sqrtf(wave), 10.0F * sqrtf(wave)));
					}
				}
				else if (rand == 1)
				{
					if (type_ == word_types::normal)
					{
						monsters_in_spawn_process_->push_back(Monster(monster_type::red, move_zone_[0], 1.0F * wave, 50.0F * wave));
					}
					else
					{
						monsters_in_spawn_process_->push_back(Monster(monster_type::red, move_zone_[0], 0.50F * sqrtf(wave), 10.0F * sqrtf(wave)));
					}
				}
				else
				{
					if (type_ == word_types::normal)
					{
						monsters_in_spawn_process_->push_back(Monster(monster_type::yellow, move_zone_[0], 1.0F * wave, 50.0F * wave));
					}
					else
					{
						monsters_in_spawn_process_->push_back(Monster(monster_type::yellow, move_zone_[0], 0.50F * sqrtf(wave), 10.0F * sqrtf(wave)));
					}
				}
			}
		}

	}

	unsigned long long getWave() const
	{
		return *wave_;
	}
	
	void newMob(const monster_type type, const float speed, const float all_health) const
	{
		monsters_->push_back(Monster(type, move_zone_[0], speed, all_health));
	}

	
	// передвидение монстров по карте
	void mobMove() const
	{
		for (Monster const& monster : *monsters_)
		{
			if (!monster.isDead())
			{
				if (*monster.move_enum < 5)
				{
					monster.move(
						(move_zone_[*monster.move_enum + 1].x - 
							move_zone_[*monster.move_enum].x) * (monster.
							speed / 1000.0F) * draw_scale_.x,
						(move_zone_[*monster.move_enum + 1].y -
							move_zone_[*monster.move_enum].y) * (monster.
							speed / 1000.0F) * draw_scale_.y);
					*monster.steps += 1;
					if (*monster.steps > 1000.0F / monster.speed)
					{
						*monster.move_enum += 1;
						*monster.steps = 0;
					}
				}
				else
				{
					on_monster_out_(monster);
					monster.isDead() = true;
				}
			}
		}
	}

	// создание новой пушки
	void newGun(const float level) const
	{
		guns_->push_back(Gun({ 100.0F, 100.0F }, 200.0F * level, 1.50F * level, 10, 10));
	}

	void onMonsterRemove(const std::function<void(Monster*)>& on_monster_remove)
	{
		on_monster_remove_ = on_monster_remove;
	}

	void onMonsterOut(const std::function<void(Monster)>& on_monster_out)
	{
		on_monster_out_ = on_monster_out;
	}

	void onWin(const std::function<void()>& on_win)
	{
		on_win_ = on_win;
	}

	Gun* parseClick() const
	{
		const float x = F(sf::Mouse::getPosition(*window_).x);
		const float y = F(sf::Mouse::getPosition(*window_).y);
		for (Gun &gun : *guns_)
		{
			if (x > gun.getPosition().x - gun.getSize().x / 2)
			{
				if (x < gun.getPosition().x + gun.getSize().x - gun.getSize().x / 2)
				{
					if (y > gun.getPosition().y - gun.getSize().y / 2)
					{
						if (y < gun.getPosition().y + gun.getSize().y - gun.getSize().y / 2)
						{
							return &gun;
						}
					}
				}
			}
		}
		return nullptr;
	}

	void stop(const bool stop)
	{
		stopped_ = stop;
	}
};