#pragma once
#include <cstdarg>
#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>

#include "Math.h"

namespace gui
{
	class GuiElement;

	class Drawable;

	class Events
	{
	public:
		bool mouse_move;
		bool mouse_down;

		sf::Vector2f position;
		sf::Vector2f inter_position;

		explicit Events(const bool is_mouse_move = false,
			const bool is_mouse_clicked = false,
			const sf::Vector2f position = {},
			const sf::Vector2f inter_position = {}) :
			mouse_move(is_mouse_move),
			mouse_down(is_mouse_clicked),
			position(position),
			inter_position(inter_position)
		{

		}

		bool operator == (const Events events) const
		{
			if (this->mouse_down == events.mouse_down &&
				this->mouse_move == events.mouse_move)
			{
				return true;
			}
			return false;
		}

		bool operator != (const Events events) const
		{
			return !(*this == events);
		}

		friend class Drawable;

	private:
		void release(GuiElement* element) const;
	};

	class GuiElement :
		public sf::Drawable
	{

		friend class gui::Drawable;

	protected:
		sf::Vector2f size_;
		sf::Vector2f position_;
		sf::Vector2f scale_;
		sf::Vector2<sf::Vector2f> q_position_;

		std::function<void()> on_click_function_ = []()->void
		{

		};

		std::function<void()> on_move_function_ = []()->void
		{

		};

		std::function<void()> on_out_function_ = []()->void
		{

		};

	public:

		GuiElement() = default;

		GuiElement(const float x, const float y,
			const float width, const float height) :
			size_(width, height),
			position_(x, y),
			q_position_(sf::Vector2f(x, y),
				sf::Vector2f(x + width, y + height))
		{

		}

		GuiElement(const sf::Vector2f position, const sf::Vector2f size) :
			size_(size),
			position_(position),
			q_position_(sf::Vector2f(position.x, position.y),
				sf::Vector2f(position.x + size.x, position.y + size.y))
		{

		}

		void setPosition(const float x, const float y)
		{
			position_.x = x;
			position_.y = y;
		}

		void setPosition(const sf::Vector2f position)
		{
			position_ = position;
		}

		[[nodiscard]] sf::Vector2f getPosition() const
		{
			return position_;
		}

		void setSize(const float x, const float y)
		{
			size_.x = x;
			size_.y = y;
		}

		void setSize(const sf::Vector2f size)
		{
			size_ = size;
		}

		sf::Vector2f getSize() const
		{
			return size_;
		}

		void onDown(const std::function<void()>& function)
		{
			this->on_click_function_ = function;
		}

		void onMove(const std::function<void()>& function)
		{
			this->on_move_function_ = function;
		}

		void onOut(const std::function<void()>& function)
		{
			this->on_out_function_ = function;
		}

		virtual void events(const Events events) = 0;

	protected:

		void draw(sf::RenderTarget& target,
			sf::RenderStates states) const override = 0;

	};

	inline void Events::release(GuiElement* element) const
	{
		element->events(*this);
	}


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
			button_sprite_.setScale(math::scale(this->size_.x, button_on_out_texture_.getSize().x), math::scale(this->size_.y, button_on_out_texture_.getSize().y));

		}

		void setOnDownTexture(const std::string& texture_path)
		{
			button_on_down_texture_.loadFromFile(texture_path);
		}

		void setOnMoveTexture(const std::string& texture_path)
		{
			button_on_move_texture_.loadFromFile(texture_path);
		}

		void setOnOutTexture(const std::string& texture_path)
		{
			button_on_out_texture_.loadFromFile(texture_path);
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
					this->on_click_function_();
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

		void draw(sf::RenderTarget& target,
			const sf::RenderStates states) const override
		{
			target.draw(button_sprite_, states);
		}

	};

	class Range final : public GuiElement
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

		float value_ = 0;

	public:

		void draw(sf::RenderTarget& target,
			const sf::RenderStates states) const override
		{

			target.draw(range_hor_rect_, states);
			target.draw(range_ver_rect_, states);
		}

		void events(const Events events) override
		{
			if (events.mouse_down)
			{
				range_ver_rect_.setSize({ this->size_.x / 60.f,
					this->size_.y });
				range_ver_rect_.setPosition(this->position_.x +
					events.inter_position.x, position_.y);

				range_hor_rect_.setSize({ this->size_.x,
					this->size_.y / 10.f });
				range_hor_rect_.setPosition(this->position_.x,
					this->position_.y + this->size_.y / 2.f);

			}
		}

	};



	class Drawable final
	{
	private:
		std::vector<GuiElement*> gui_elements_;
		sf::Vector2f size_ = {};
		sf::RenderWindow* render_window_;
		sf::Texture background_texture_;

		bool background_bool_ = false;

	public:

		void setWindow(sf::RenderWindow* render_window)
		{
			render_window_ = render_window;
		}

		sf::RenderWindow* getRenderWindow() const
		{
			return render_window_;
		}

		explicit Drawable(const sf::Vector2f size,
			const std::string& background_texture_path = {},
			sf::RenderWindow* render_window = nullptr) :

			size_(size),
			render_window_(render_window)
		{
			if (!background_texture_path.empty())
			{
				background_texture_.loadFromFile(background_texture_path);
				background_bool_ = true;
			}
		}

		Drawable(const float size_x, const float size_y,
			const std::string& background_texture_path = {},
			sf::RenderWindow* render_window = nullptr) :

			size_(size_x, size_y),
			render_window_(render_window)
		{
			if (!background_texture_path.empty())
			{
				background_texture_.loadFromFile(background_texture_path);
				background_bool_ = true;
			}
		}

		void addGuiElement(GuiElement* const gui_element)
		{
			gui_elements_.emplace_back(gui_element);
		}

		void addGuiElement(std::initializer_list<GuiElement* const> list)
		{
			for (GuiElement* const& gui_element : list)
			{
				gui_elements_.emplace_back(gui_element);
			}
		}

		void operator + (GuiElement* const gui_element)
		{
			addGuiElement(gui_element);
		}

		void operator + (const std::initializer_list<GuiElement* const> list)
		{
			addGuiElement(list);
		}

		gui::Drawable& operator << (GuiElement* const gui_element)
		{
			addGuiElement(gui_element);
			return *this;
		}

		gui::Drawable& operator << (const std::initializer_list<GuiElement* const> list)
		{
			addGuiElement(list);
			return *this;
		}

		void setBackgroundTexture(const std::string& background_texture_path)
		{
			background_texture_.loadFromFile(background_texture_path);
		}

		void setSize(const sf::Vector2f size)
		{
			size_ = size;
		}

		void setSize(const float size_x, const float size_y)
		{
			size_.x = size_x;
			size_.y = size_y;
		}

		sf::Vector2f getSize() const
		{
			return size_;
		}

		void draw() const
		{
			const sf::Vector2f scale(
				math::scale(render_window_->getSize().x, size_.x),
				math::scale(render_window_->getSize().y, size_.y));

			sf::Transform transform;
			
			transform.scale(scale.x, scale.y);

			sf::Sprite background_sprite;
			
			background_sprite.setTexture(background_texture_);
			
			background_sprite.setScale(math::scale(
				size_.x, background_texture_.getSize().x),
				math::scale(size_.y,
					background_texture_.getSize().y));
			
			render_window_->draw(background_sprite, transform);

			for (GuiElement* const& it : gui_elements_)
			{
				Events events{};

				events.position.x =
					static_cast<float>(sf::Mouse::getPosition(*render_window_).x);
				
				events.position.y =
					static_cast<float>(sf::Mouse::getPosition(*render_window_).y);

				if (events.position.x > it->q_position_.x.x * scale.x)
				{
					if (events.position.y < it->q_position_.y.y * scale.y)
					{
						if (events.position.y > it->q_position_.x.y * scale.y)
						{
							if (events.position.x < it->q_position_.y.x * scale.x)
							{
								
								events.mouse_move = true;

								events.inter_position.x = (events.position.x -
									it->q_position_.x.x * scale.x) / scale.x;

								events.inter_position.y = (events.position.y -
									it->q_position_.x.y * scale.y) / scale.y;

								if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
								{
									events.mouse_down = true;
								}
								
							}
						}
					}
				}

				events.release(it);
				
				render_window_->draw(*it, transform);
			}
		}
	};
};