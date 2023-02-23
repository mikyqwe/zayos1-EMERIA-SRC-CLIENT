#pragma once

namespace ikgl{
	namespace __private {
		template <class widget_type>
		class __template_selector : public ikgl::window {
		public:
			static ikgl::widget_type_t type() { return ikgl::widget_type_t::selector; }

		private:
			using widget_deque_t = std::deque<widget_type*>;
			using position_t = size_t;
			static const position_t nullpos = (position_t)(-1);

		private:
			enum class state_t {
				unselected,
				selecting,
				selected,
			};


		private:
			widget_deque_t			_elements;
			ikgl::scrollbar*		_scrollbar;
			window*					_clicker;
			ikgl::box*				_box;
			ikgl::coord_t			_view_height;
			ikgl::coord_t			_blackspace;
			position_t				_selected_pos;
			position_t				_default_pos;
			ikgl::widget_event_t	_on_select;
			state_t					_state;

		protected:
			void _on_click_box() {
				if (this->_gui) {
					auto mouse = this->_gui->get_mouse_position();
					position_t pos = 0;
					for (auto& elm : this->_elements) {
						if (elm->is_in(mouse)) {
							this->select_element(pos);
							break;
						} pos++;
					}
				}
			}

			void _on_click_clicker() {
				this->_box->show();
				this->_state = state_t::selecting;
				this->_box->show();
				this->_box->move_to_bottom();
				this->_box->move_bottom(this->_view_height);
				this->_update_view();
			}

			void _on_scroll_list() {
				this->_update_view();
			}

			void _setup_widgets() {
				auto _click_box = [this] __ikgl_wevent_lambda_type_func{
					this->_on_click_box();
					return widget_event_return_t::KEEP_CALLBACK;
				};
				auto _click_clicker = [this] __ikgl_wevent_lambda_type_func{
					this->_on_click_clicker();
					return widget_event_return_t::KEEP_CALLBACK;
				};
				auto _on_scroll = [this] __ikgl_wevent_lambda_type_func{
					this->_on_scroll_list();
					return widget_event_return_t::KEEP_CALLBACK;
				};

				this->_box = this->create_element<ikgl::box>();
				this->_scrollbar = this->create_element<ikgl::scrollbar>();
				this->_clicker = this->create_element<ikgl::window>();

				this->_box->set_on_mouseleft_up(_click_box);
				this->_clicker->set_on_mouseleft_up(_click_clicker);
				this->_scrollbar->set_on_scroll(_on_scroll);

				this->_scrollbar->set_size({4, 0});
				this->_scrollbar->hide();

				auto button1 = this->_scrollbar->get_button1_ptr();
				auto button2 = this->_scrollbar->get_button2_ptr();
				auto buttonm = this->_scrollbar->get_middle_button_ptr();

				button1->set_size(4,4);
				button2->set_size(4,4);
				buttonm->set_size(4,4);
			}

			bool_t _is_shown_above(const position_t pos) {
				if (this->_selected_pos == pos)
					return true;
				if (this->_state == state_t::unselected &&
					this->_default_pos == pos)
					return true;
				return false;
			};

			void _update_clicker_size() {
				this->_clicker->set_size(this->_size);
				this->_clicker->move_to_top();
				this->_clicker->move_to_left();
			}

			void _update_box_size() {
				this->_box->set_size({ this->_size.width, this->_view_height });
			}

			void _update_scrollbar_size() {
				this->_scrollbar->set_height(this->_view_height);
				this->_scrollbar->move_to_right();
				this->_scrollbar->move_to_bottom();
				this->_scrollbar->move_bottom(this->_view_height);
			}

			ikgl::coord_t _get_total_view_height() {
				ikgl::coord_t result = 0;
				position_t pos = 0;

				auto it = this->_elements.begin();
				auto end = this->_elements.end();

				for (; it != end; ++it, ++pos) {
					if (this->_is_shown_above(pos)) {
						continue;
					} result += (*it)->get_height() + this->_blackspace;
				}return result;
			}

			void _update_elements() {
				auto total_height = utilities::to<float_t>(this->_get_total_view_height());
				auto scrollval = this->_scrollbar->get_value();
				auto start_y = -utilities::to<coord_t>(total_height * scrollval);

				if (total_height < this->_view_height) {
					this->_scrollbar->set_value(0.0f);
					this->_scrollbar->hide();
					start_y = 0;
				}

				else {
					if (!this->_scrollbar->is_shown()) {
						this->_scrollbar->show();
					}
				}

				position_t pos = 0;
				for (auto& elm : this->_elements) {
					if (this->_is_shown_above(pos))
						continue;

					elm->set_position(4, start_y);
					start_y += elm->get_height() + this->_blackspace;
				}
			}

			void _update_selected() {
				auto selected = this->get_selected();
				if (selected && this->_state == state_t::selected) {
					if (selected->get_parent() != this) {
						selected->set_parent(this);
					} selected->set_position(4, this->_blackspace);
				}
				if (selected && this->_state == state_t::unselected) {
					if (selected->get_parent() != this->_box) {
						selected->set_parent(this->_box);
					}
				}
			}

			void _update_default() {
				auto _default = this->get_element(this->_default_pos);
				if (_default && this->_state == state_t::unselected) {
					if (_default->get_parent() != this) {
						_default->set_parent(this);
					} _default->set_position(4, this->_blackspace);
				}
				if (_default && this->_state == state_t::selected) {
					if (_default->get_parent() != this->_box) {
						_default->set_parent(this->_box);
					}
				}
			}

			void _update_view() {
				if (this->_selected_pos != nullpos) {
					this->_update_selected();
				}

				switch (this->_state) {
				case state_t::selecting:
					this->_update_elements();
					break;

				case state_t::selected:
					this->_scrollbar->set_value(0.0f);
					this->_update_default();
					break;

				case state_t::unselected:
					if (this->_default_pos != nullpos) {
						this->_update_default();
					}break;

				default:
					break;
				}
			}

		public:

			__template_selector<widget_type>(gui* ptr) : window(ptr) {
				this->_elements = {};
				this->_view_height = 0;
				this->_selected_pos = nullpos;
				this->_default_pos = nullpos;
				this->_state = state_t::unselected;
				this->_on_select = {};
				this->_blackspace = 0;
				this->_widget_type = widget_type_t::selector;
				this->_setup_widgets();
			}

			void clear_elements() {
				for (auto& elm : this->_elements) {
					this->remove_child(elm);
				}

				this->_selected_pos = nullpos;
				this->_elements.clear();
			}

			widget_type* add_element() {
				auto ptr = this->_box->create_element<widget_type>();
				this->_elements.push_back(ptr);
				return ptr;
			}

			void remove_element(const position_t pos, ikgl::bool_t update_view = true) {
				if (pos < this->_elements.size()) {
					auto it = this->_elements.begin() + pos;
					this->remove_child(*it);
					this->_elements.erase(it);

					if (this->_selected_pos == pos) {
						this->_selected_pos = nullpos;
						this->_state = state_t::unselected;
					}
				}

				if (update_view) {
					this->_update_view();
				}
			}

			widget_type* get_element(const position_t pos) {
				if (pos < this->_elements.size()) {
					return this->_elements[pos];
				} return nullptr;
			}

			void select_element(const position_t pos) {
				if (pos < this->_elements.size()) {
					this->_selected_pos = pos;
					this->_state = state_t::selected;
					this->_box->hide();
					this->_update_view();
				}

				if (this->_on_select) {
					this->_on_select(this);
				}
			}

			void select_element(const widget_type* ptr) {
				position_t pos = 0;
				for (auto& elm : this->_elements) {
					if (elm == ptr) {
						this->select_element(pos);
						return;
					} pos++;
				}
			}

			widget_type* get_selected() {
				if (this->_selected_pos != nullpos) {
					return this->_elements[this->_selected_pos];
				} return nullptr;
			}

			position_t get_selected_position() {
				return this->_selected_pos;
			}

			void unselect() {
				if (this->_selected_pos == nullpos)
					return;

				this->_selected_pos = nullpos;
				this->_state = state_t::unselected;
				this->_update_view();
			}

			void set_view_height(const ikgl::coord_t height, ikgl::bool_t update_view = true) {
				this->_view_height = height;
				if (update_view)
					this->_update_view();
			}

			ikgl::coord_t get_view_height() {
				return this->_view_height;
			}

			void set_blankspace(const ikgl::coord_t bl, ikgl::bool_t update_view = true) {
				this->_blackspace = bl;
				if (update_view)
					this->_update_view();
			}

			ikgl::coord_t get_blankspace() {
				return this->_blackspace;
			}

			void set_default(const position_t pos, bool_t update_view = true) {
				this->_default_pos = pos;
				if (update_view)
					this->_update_view();
			}

			position_t get_default() {
				return this->_default_pos;
			}


			void set_on_select(const widget_event_t& f) {
				this->_on_select = f;
			}

			virtual ikgl::void_t remove_children() {
				auto _is_reserved = [this](window* ptr) {
					if (ptr == this->_scrollbar ||
						ptr == this->_clicker) {
						return true;
					}
					auto it = this->_elements.begin();
					auto end = this->_elements.end();
					for (; it != end; ++it) {
						if ((window*)*it == ptr) {
							return true;
						}
					}
					return false;
				};

				if (this->_children_deque.empty() == false) {
					auto reserved_list = window::children_deque_t{};
				
					for(auto& ptr : this->_children_deque){
						if (_is_reserved(ptr)) {
							reserved_list.push_back(ptr);
							continue;
						}

						if (this->_gui) {
							this->_gui->__check_for_peeked(ptr);
							this->_gui->__register_destroyed(ptr);
						}

						else {
							delete(ptr);
						}
					}
				
					this->_children_deque.clear();
					this->_children_deque.swap(reserved_list);
					this->call_render();
				}
			}

			virtual ikgl::void_t set_size(const ikgl::widget_size_t& size) {
				this->ikgl::window::set_size(size);
				this->_update_clicker_size();
				this->_update_box_size();
				this->_update_scrollbar_size();
			}

			virtual ikgl::void_t set_size(const widget_size_t* size) {
				this->set_size(*size);
			}

			virtual ikgl::void_t set_size(const coord_t w, const coord_t h){
				this->set_size({ w,h });
			}

			ikgl::scrollbar* get_scrollbar() {
				return this->_scrollbar;
			}

			ikgl::box* get_box() {
				return this->_box;
			}

			ikgl::window* get_clicker() {
				return this->_clicker();
			}

		};
	}
}

namespace ikgl {
	class text_selector : public __private::__template_selector<ikgl::text> {

		text_selector(gui* gptr) : __private::__template_selector<ikgl::text>(gptr){
		}


	};
	
}
