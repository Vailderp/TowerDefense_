#pragma once
#include <cstdarg>
#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>
#include "gui_core.h"

namespace gui
{
	class Button final : public GuiElement
	{
	private:

		sf::Sprite button_sprite_;
		sf::Texture button_on_down_texture_;
		sf::Texture button_on_move_texture_;
		sf::Texture button_on_out_texture_;
		Events last_events_;

	public:

		explicit Button(const float x = 0,
		                const float y = 0,
		                const float width = 0,
		                const float height = 0,
			const std::string& on_down_texture_path = {},
			const std::string& on_move_texture_path = {},
			const std::string& on_out_texture_path = {}) :
	
			GuiElement(x, y, width, height)
		
		{
			if (!on_down_texture_path.empty())
			{
				button_on_down_texture_.loadFromFile(on_down_texture_path);
			}
			
			if (!on_move_texture_path.empty())
			{
				button_on_move_texture_.loadFromFile(on_move_texture_path);
			}
			
			if (!on_out_texture_path.empty())
			{
				button_on_out_texture_.loadFromFile(on_out_texture_path);
			}

			button_sprite_.setTexture(button_on_out_texture_);
			button_sprite_.setScale(
				math::scale(this->size_.x,
					button_on_out_texture_.getSize().x),
				math::scale(this->size_.y,
					button_on_out_texture_.getSize().y));

		}

		Button& setOnDownTexture(const std::string& texture_path)
		{
			button_on_down_texture_.loadFromFile(texture_path);
			return *this;
		}

		Button& setOnMoveTexture(const std::string& texture_path)
		{
			button_on_move_texture_.loadFromFile(texture_path);
			return *this;
		}

		Button& setOnOutTexture(const std::string& texture_path)
		{
			button_on_out_texture_.loadFromFile(texture_path);
			return *this;
		}

		Button& setOnDownTexture(const sf::Texture& texture)
		{
			button_on_down_texture_ = texture;
			return *this;
		}

		Button& setOnMoveTexture(const sf::Texture& texture)
		{
			button_on_move_texture_ = texture;
			return *this;
		}

		Button& setOnOutTexture(const sf::Texture& texture)
		{
			button_on_out_texture_ = texture;
			return *this;
		}


	protected:
		void events(const Events events) override
		{
			
			button_sprite_.setPosition(this->position_);
			
			if (last_events_ != events)
			{
				last_events_ = events;
				
				if (events.mouse_down)
				{
					this->on_down_function_();
					button_sprite_.setTexture(button_on_down_texture_);
					button_sprite_.setScale(math::scale(this->size_.x,
						button_on_down_texture_.getSize().x), 
						math::scale(this->size_.y,
							button_on_down_texture_.getSize().y));
				}
				else
				{
					if (events.mouse_move)
					{
						this->on_move_function_();
						button_sprite_.setTexture(button_on_move_texture_);
						button_sprite_.setScale(math::scale(this->size_.x,
							button_on_move_texture_.getSize().x),
							math::scale(this->size_.y,
								button_on_move_texture_.getSize().y));
					}
					else
					{
						this->on_out_function_();
						button_sprite_.setTexture(button_on_out_texture_);
						button_sprite_.setScale(math::scale(this->size_.x,
							button_on_out_texture_.getSize().x), 
							math::scale(this->size_.y,
								button_on_out_texture_.getSize().y));
					}
				}
			}
		}


	public:

		static Button* build()
		{
			return new Button;
		}

	protected:
		void draw(sf::RenderTarget& target, 
			const sf::RenderStates states) const override
		{
			target.draw(button_sprite_, states);
		}

	};

	class Range final: public GuiElement
	{
	public:

		explicit Range(const float x = 0,
			const float y = 0,
			const float width = 0,
			const float height = 0) :
		GuiElement(x, y, width, height)
		{
			range_hor_rect_.setFillColor(sf::Color::Blue);
			range_ver_rect_.setFillColor(sf::Color::White);
			
			range_ver_rect_.setSize({ this->size_.x / 60.f, this->size_.y });
			range_ver_rect_.setOrigin(this->size_.x / 60.f / 2.f, 0);
			range_ver_rect_.setPosition(this->position_.x, position_.y);

			range_hor_rect_.setSize({ this->size_.x, this->size_.y / 10.f });
			range_hor_rect_.setOrigin(0, this->size_.y / 10.f / 2.f);
			range_hor_rect_.setPosition(this->position_.x, this->position_.y + this->size_.y / 2.f);
		}

	private:
		sf::RectangleShape range_ver_rect_;
		sf::RectangleShape range_hor_rect_;

		float max_value_ = 0;
		float min_value_ = 0;

		float value_ = 0;

		std::function<void(const float)> on_change_function_ = [](const float) -> void
		{
			
		};

	public:

		Range& setOnChangeFunction(const std::function<void(const float value)>& on_change_function)
		{
			on_change_function_ = on_change_function;
			return *this;
		}

		std::function<void(const float)> getOnChangeFunction() const
		{
			return on_change_function_;
		}

		Range& setMinValue(const float min_value)
		{
			min_value_ = min_value;
			return *this;
		}

		float getMinValue() const
		{
			return min_value_;
		}

		Range& setMaxValue(const float max_value)
		{
			max_value_ = max_value;
			return *this;
		}

		float getMaxValue() const
		{
			return max_value_;
		}
		
		Range& setValue(const float value)
		{
			value_ = value;
			return *this;
		}
		
		float getValue() const
		{
			return value_;
		}
	
	protected:
		void draw(sf::RenderTarget& target,
		          const sf::RenderStates states) const override
		{
			
			target.draw(range_hor_rect_, states);
			target.draw(range_ver_rect_, states);
		}
		
		void events(const gui::Events events) override
		{
			if (events.mouse_down)
			{
				range_ver_rect_.setSize({ this->size_.x / 60.f,
					this->size_.y});
				range_ver_rect_.setPosition(this->position_.x +
					events.inter_position.x, position_.y);

				range_hor_rect_.setSize({ this->size_.x,
					this->size_.y / 10.f});
				range_hor_rect_.setPosition(this->position_.x,
					this->position_.y + this->size_.y / 2.f);

				value_ = min_value_ + events.inter_position.x * math::scale(max_value_ - min_value_, this->size_.x);

				on_change_function_(value_);
				
				std::cout << value_ << std::endl;
				
			}
		}
		
	};
	
	class Text final : public gui::GuiElement
	{
	private:

		sf::Font* font_ = new sf::Font;

	public:

		sf::Text* text = new sf::Text;

		explicit Text(const sf::Font& font = sf::Font())
		{
			*font_ = font;
		}

		compl Text()
		{
			delete font_;
			delete text;
		}

	public:
		Text& loadFontFromFile(const std::string& path) 
		{
			font_->loadFromFile(path);
			text->setFont(*font_);
			return *this;
		}

	protected:
		void draw(sf::RenderTarget& target,
			const sf::RenderStates states) const override
		{
			text->setPosition(this->position_);
			target.draw(*text);
		}

		void events(const gui::Events events) override
		{

		}
	};
	
};
