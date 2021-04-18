#pragma once
#include "Gui.h"
#include "World.h"
#include <Windows.h>

class Menu;

class TypeGame : public sf::Drawable
{

	friend class Menu;
	
private:
	bool *active_ = new bool(false);

	std::function<void(word_types)> on_mode_ = [](word_types type) {};
	
	gui::Drawable drawable_{ 1, 1,
		"data/textures/main2.png" };

	gui::Button button_wave5_{ 0.40F, 0.40F, 0.30F, 0.15F,
		"data/textures/buttons/5wave_click.png",
		"data/textures/buttons/5wave_on.png",
		"data/textures/buttons/5wave.png" };

	gui::Button button_infinity_{ 0.40F, 0.60F, 0.30F, 0.15F,
		"data/textures/buttons/infinite_click.png",
		"data/textures/buttons/infinite_on.png",
		"data/textures/buttons/infinite.png" };

public:


	explicit TypeGame(sf::RenderWindow* window)
	{
		
		drawable_
		<< &button_wave5_.setPressDelayEnable(true).setPressDelay(1.0_sec)
		<< &button_infinity_.setPressDelayEnable(true).setPressDelay(1.0_sec);

		drawable_.setWindow(window);
		
		button_wave5_.setOnDownFunction(
			[&]() -> void
			{
				*active_ = false;
				on_mode_(word_types::normal);
			});

		button_infinity_.setOnDownFunction(
			[&]() -> void
			{
				*active_ = false;
				on_mode_(word_types::infinity);
			});
		
	}

	void setActive(const bool active) const
	{
		*active_ = active;
	}

	bool* active() const
	{
		return active_;
	}


	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (*active_)
		{
			drawable_.draw();
		}
	}

	void onMode(const std::function<void(word_types)>& on_mode)
	{
		on_mode_ = on_mode;
	}

	
};

class Menu : public sf::Drawable
{
private:

	sf::SoundBuffer sound_buffer_;
	
	sf::Sound *sound_ = new sf::Sound;
	
	TypeGame type_game_;
	
	bool *active_ = new bool(true);
	
	gui::Drawable drawable_{ 1, 1,
		"data/textures/main2.png" };

	gui::Button button_start_{ 0.40F, 0.40F, 0.30F, 0.15F,
		"data/textures/buttons/start_click.png",
		"data/textures/buttons/start_on.png",
		"data/textures/buttons/start.png" };

	gui::Button button_exit_{ 0.40F, 0.60F, 0.30F, 0.15F,
		"data/textures/buttons/exit_click.png",
		"data/textures/buttons/exit_on.png",
		"data/textures/buttons/exit.png" };

public:

	
	explicit Menu(sf::RenderWindow* window) :
	type_game_(window)
	{

		button_exit_.setOnDownFunction([]() -> void
		{
			exit(1);
		});

		button_start_.setOnDownFunction(
		[&]() -> void
		{
			*type_game_.active_ = true;
			*active_ = false;
			Sleep(1000);
		});
		
		drawable_
		<< &button_start_.setPressDelayEnable(true).setPressDelay(1.0_sec)
		<< &button_exit_.setPressDelayEnable(true).setPressDelay(button_start_.getPressDelay());

		drawable_.setWindow(window);


		if (!sound_buffer_.loadFromFile("data/sounds/menu.wav"))
		{
			std::cout << "ERROR >> LOAD MAIN.WAV" << std::endl;
		}
		sound_->setBuffer(sound_buffer_);
		sound_->setLoop(true);
		sound_->play();
	}
	
	void setActive(const bool active) const
	{
		*active_ = active;
	}

	bool* active() const
	{
		return active_;
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(type_game_);
		if (*active_)
		{
			drawable_.draw();
			
		}
		if (!*active_ && !*type_game_.active_ )
		{
			sound_->stop();
		}
	}

	bool gameShouldBeStarted() const
	{
		if (!*active_ && !*type_game_.active_)
		{
			return true;
		}
		return false;
	}

	void onMode(const std::function<void(word_types)>& on_mode)
	{
		type_game_.onMode(on_mode);
	}

	
};
