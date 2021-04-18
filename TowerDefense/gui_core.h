#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "Math.h"
#include "Time.h"

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
		
		friend class gui::Events;
	
	protected:
		sf::Vector2f size_;
		sf::Vector2f position_;
		sf::Vector2f scale_;
		sf::Vector2<sf::Vector2f> q_position_;
		
		bool press_delay_enable_ = false;
		long double press_delay_= 1000;

		bool is_active_ = true;

		std::function<void()> on_down_function_ = []()->void
		{

		};

		std::function<void()> on_move_function_ = []()->void
		{

		};

		std::function<void()> on_out_function_ = []()->void
		{

		};

	public:

		GuiElement(const float x, const float y,
			const float width, const float height) :
			size_(width, height),
			position_(x, y),
			q_position_(sf::Vector2f(x, y),
				sf::Vector2f(x + width, y + height))
		{

		}

		explicit GuiElement(const sf::Vector2f position = {}, const sf::Vector2f size = {}) :
			size_(size),
			position_(position),
			q_position_(sf::Vector2f(position.x, position.y),
				sf::Vector2f(position.x + size.x, position.y + size.y))
		{

		}

		GuiElement& setPosition(const float x, const float y)
		{
			position_.x = x;
			position_.y = y;
			return *this;
		}

		GuiElement& setPosition(const sf::Vector2f position)
		{
			position_ = position;
			return *this;
		}

		[[nodiscard]]
		sf::Vector2f getPosition() const
		{
			return position_;
		}

		GuiElement& setSize(const float x, const float y)
		{
			size_.x = x;
			size_.y = y;
			return *this;
		}

		GuiElement& setSize(const sf::Vector2f size)
		{
			size_ = size;
			return *this;
		}

		GuiElement& setPressDelayEnable(const bool enable)
		{
			press_delay_enable_ = enable;
			return *this;
		}

		bool getPressDelayEnable() const
		{
			return press_delay_enable_;
		}

		GuiElement& setPressDelay(const long double delay)
		{
			press_delay_ = delay;
			return *this;
		}

		long double getPressDelay() const
		{
			return press_delay_;
		}
		
		[[nodiscard]]
		sf::Vector2f getSize() const
		{
			return size_;
		}

		GuiElement& setActive(const bool enable)
		{
			is_active_ = enable;
			return *this;
		}

		bool getActive() const
		{
			return  is_active_;
		}

		GuiElement& setOnDownFunction(const std::function<void()>& function)
		{
			this->on_down_function_ = function;
			return *this;
		}

		GuiElement& setOnMoveFunction(const std::function<void()>& function)
		{
			this->on_move_function_ = function;
			return *this;
		}

		GuiElement& setOnOutFunction(const std::function<void()>& function)
		{
			this->on_out_function_ = function;
			return *this;
		}

		std::function<void()> getOnDownFunction() const
		{
			return this->on_down_function_;
		}

		std::function<void()> getOnMoveFunction() const
		{
			return this->on_move_function_;
		}

		std::function<void()> getOnOutFunction() const
		{
			return this->on_out_function_;
		}
	
	private:
		
		virtual void events(const Events events) = 0;

	protected:

		void draw(sf::RenderTarget& target,
			sf::RenderStates states) const override = 0;

	};

	inline void Events::release(GuiElement* element) const
	{
		element->events(*this);
	}

	class Drawable final
	{
	private:
		std::vector<GuiElement*> gui_elements_;
		sf::Vector2f size_ = {};
		sf::RenderWindow* render_window_;
		sf::Texture background_texture_;
		bool background_bool_ = false;
	private:
		Timer timer_;

	public:

		compl Drawable()
		{
			
		}

		Drawable& setWindow(sf::RenderWindow* render_window)
		{
			render_window_ = render_window;
			return *this;
		}

		[[nodiscard]]
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

		Drawable& setSize(const sf::Vector2f size)
		{
			size_ = size;
			return *this;
		}

		Drawable& setSize(const float size_x, const float size_y)
		{
			size_.x = size_x;
			size_.y = size_y;
			return *this;
		}

		[[nodiscard]]
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

				if (it->is_active_)
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
										if (it->press_delay_enable_)
										{
											if (timer_.milliseconds() > it->press_delay_)
											{
												events.mouse_down = true;
												timer_.clear();
											}
										}
										else
										{
											events.mouse_down = true;
										}
									}
								}
							}
						}
					}

					events.release(it);
					render_window_->draw(*it, transform);
				}
			}
		}
		
	};
}
