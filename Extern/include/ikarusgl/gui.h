/*
Copyright (c) 2019 IkarusDeveloper

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __INCLUDE_HEADER_GUI_IKARUSGL__
#define __INCLUDE_HEADER_GUI_IKARUSGL__
#define __critical_scope() std::lock_guard<mutex_t> lock(this->_mutex)

namespace ikgl
{

	inline void_t __set_defaults_on_graphic(gdiplus_graphics_t& gd) {
		gd.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	}

	/**
	* Function used to set the basics informations recommended.
	* @param hinstance a HANDLE to INSTANCE object. (You can find more information about INSTANCE object in Micosoft Documentation )
	* @param pwinfo is a pointer to an exists object #ikgl::create_gui_infow_t that will contain the basics setting.
	*/
	void_t make_default_create_info(hinstance_t hinstance, create_gui_infow_t* pwinfo);

	/**
	* Function used to set the basics informations recommended.
	* @param hinstance a HANDLE to INSTANCE object. (You can find more information about INSTANCE object in Micosoft Documentation )
	* @param painfo is a pointer to an exists object #ikgl::create_gui_infoa_t that will contain the basics setting.
	*/
	void_t make_default_create_info(hinstance_t hinstance, create_gui_infoa_t* painfo);



	/**
	* The Main class of the library. It's a manager for the application window, it owns the layers, the widgets, and manage the messages of the application. 
	*/
	class gui
	{
		friend class window;
		friend class button;
		
		template<class widget_type>
		friend class __private::__template_selector;

		friend class scrollbar;
		friend class layer;

	public:
		typedef  std::list<window*> rootlist;
		typedef  std::list<window*> pickedlist;
		typedef  std::map<uint_t,std::vector<message_event_t>> message_map;
		typedef  std::unordered_map<string_t,layer> layer_map;

		/**
		* Structure used to store the fps tracing informations
		*/
		typedef struct SFPSTraceData{
			double_t start_time, last_time;
			uint_t   fps;
		} fps_trace_data_t;

	public:
		gui();
		~gui();

	//* public templates methods
	public:

		/**
		* Function used to perform a function without causing concurrency. Functions like all the callbacks settable to widgets and to the gui object are already safe and performed by locking the mutex.
		* @param t is a function (or a lambda) to be perform in safer conditions.
		* \return the function return true if the t parameter is performed successfully.
		*/
		template <class T>
		bool_t execute_lock(std::function<T> t){
			if(!this->__is_window_initialized())
				return false;

			__critical_scope();
			t();
			return true;
		}


		/**
		* Function used to perform a function without causing concurrency and by assigning to s the output of the function.  Functions like all the callbacks settable to widgets and to the gui object are already safe and performed by locking the mutex.
		* @param t is the function to be performed in safer conditions.
		* @param s is the out argument to receive the output of t.
		* \return The function return true if the function is performed sucessfully
		*/
		template <class T, class S>
		bool_t execute_lock_out(std::function<T> t, S& s){
			if(!this->__is_window_initialized())
				return false;

			__critical_scope();
			s = t();
			return true;
		}


		/**
		* Function used to obtain a widget by passing its id.
		* @param id is the id to find.
		* @param obj is a reference to a pointer that it will contain the result of the function.
		* \return the function return true if the id is found successfully.
		*/
		template <class T=window>
		bool_t get_widget_by_id(int_t id , T*& obj){
			window* win = nullptr;
			obj		 = nullptr;

			for(auto& lit : this->_layer_map)
				for(auto& child : lit.second.get_children_ref())
					if(child->get_child(id, &win))
 						break;

			obj = (T*) win;
			return obj != nullptr;
		}


		/**
		* Function used to obtain a widget by passing its id.
		* @param id is the id to find.
		* \return the function return a pointer of T object. It may return nullptr if the id is not found or if the widget is not an object of type T.
		*/
		template <class T=window>
		T* get_widget_by_id(int_t id){
			window* win = nullptr;

			for(auto& lit : this->_layer_map)
				for(auto& child : lit.second.get_children_ref())
					if(child->get_child(id, &win))
 						break;

			if (!win || win->get_widget_type() != T::type())
				return nullptr;

			else
				return static_cast<T*>(win);
		}



		/**
		* Function used to find a widget by its name. To set a name to the widget you can use #ikgl::window::set_window_name function.
		* @param name is the name to find.
		* \return The function may return the pointer to a object of type T or a nullptr if the name is not found or the object found is not an object of type T.
		*/
		template <class T = window>
		T* get_widget_by_name(string_t name) {
			window* win = this->get_window_by_name(name);

			if (!win || T::type() != win->get_widget_type()) {
				return nullptr;
			} else {
				return static_cast<T*>(win);
			}
		}


		/**
		* Function used to find a widget by its name. To set a name to the widget you can use #ikgl::window::set_window_name function.
		* @param name is the name to find.
		* \return The function may return the pointer to a object of type T or a nullptr if the name is not found or the object found is not an object of type T.
		*/
		template <class T = window>
		T* get_widget_by_name(wstring_t name) {
			window* win = this->get_window_by_name(name);

			if (!win || T::type() != win->get_widget_type()) {
				return nullptr;
			}
			else {
				return static_cast<T*>(win);
			}
		}

	//* private templates methods
	private:
		template <class T>
		void_t __check_new_picked(const T& func) {
			if (!this->_peeked_window_left && !this->_peeked_window_right) {
				//* store the result of picked on the mouse position
				auto picked = this->get_picked(this->get_mouse_position());
				func();

				//* get again the result of picked on the mouse position
				auto new_picked = this->get_picked(this->get_mouse_position());
				if (picked != new_picked) {
					 if (picked) picked->on_mouseover_out();
					 if (new_picked) new_picked->on_mouseover_in();
				}
			}

			else {
				func();
			}
		}

	//* non-template public methods
	public:

		/**
		* Function used to add a new layer to the layer list. The name to give to the layer must be unique. The function may return nullptr if the layer name is already used.
		* @param layername is a unique name to set to the layer.
		* \return This function return a pointer that points the newone layer created.
		*/
		layer* add_layer(string_t layername);

		/**
		* Function used to create the window to be manager by the gui object. You can set the parameters of the application window by setting them into the wInfo object to pass as argument.
		* @param wInfo is a pointer to a #ikgl::create_gui_infow_t object that exists and it contain the setting to use to create the application window.
		* \return the function return true if the create operation is performed successfully.
		*/
		bool_t create(create_gui_infow_t* wInfo);

		/**
		* Function used to create the window to be manager by the gui object. You can set the parameters of the application window by setting them into the wInfo object to pass as argument.
		* @param aInfo is a pointer to a #ikgl::create_gui_infoa_t object that exists and it contain the setting to use to create the application window.
		* \return the function return true if the create operation is performed successfully.
		*/
		//bool_t create(create_gui_infoa_t* aInfo);

		/**
		* Function used to force the gui to redraw the widgets.
		*/
		void_t call_render();

		/**
		* Function used to clear the gui object before to destroy it or to exit from the program.
		*/
		void_t clear();

		/**
		* Function used to disable the tracing of the fps (it's disabled by default, You can use #ikgl::gui::enable_trace_fps to start the tracing )
		*/
		void_t disable_trace_fps();

		/**
		* Function used to disable the rendering performed without check the render request from the widgets (It's disabled by default, You can enable the always render option by using #ikgl::gui::enable_always_render ). The always render option is usefull to increase performance when it is practically obvious that at least one of the widgets will require rendering. This avoids controlling all the widgets and directly renders the interface, saving nanoseconds.
		*/
		void_t disable_always_render();

		/**
		* Function used to disable the layer mode usefull to increase efficiency and add the possibility of new transparency effects that are not possible without enabling layer mode. The layer mode is disabled by default , You can enable it by using #ikgl::gui::enable_layer_mode function
		*/
		void_t disable_layer_mode();

		/**
		* This is a version of #ikgl::gui::disable_layer_mode that locks the mutex before to be performed.  To know when is it necessary to lock take a look to #ikgl::gui::execute_lock description.
		*/
		void_t disable_layer_mode_lock();

		/**
		* Function used to disable the transparent colorkey used for layer mode. This option is used just when the layer mode is enabled. It is disable by default, you can enable it by using #ikgl::gui::enable_transparent_colorkey. 
		*/
		void_t disable_transparent_colorkey();

		/**
		* This is a version of #ikgl::gui::disable_transparent_colorkey that locks the mutex before to be performed.  To know when is it necessary to lock take a look to #ikgl::gui::execute_lock description.
		*/
		void_t disable_transparent_colorkey_lock();

		/**
		* Function used to enable the tracing of the fps count. It's default disabled, You can disable the tracing when it's no longer needed by using #ikgl::gui::disable_trace_fps .
		*/
		void_t enable_trace_fps();

		/**
		* Function used to make the rendering performed without check the render request from the widgets (It's disabled by default, You can disable it when it's no longer needed by using #ikgl::gui::disable_always_render ). The always render option is usefull to increase performance when it is practically obvious that at least one of the widgets will require rendering. This avoids controlling all the widgets and directly renders the interface, saving nanoseconds.
		*/
		void_t enable_always_render();

		/**
		* Function used to enable the layer mode, which is usefull to increase the performace and to make available some transparency effect otherwise unusable. By using the transparent colorkey is possible to add transparency part to the window. Layer mode is disabled by default, You can enable it and disable it when it's no longer needed by using #ikgl::gui::disable_layer_mode .
		*/
		void_t enable_layer_mode();

		/**
		* Function used to enable the colorkey option. The color key option is usefull to make transparency part of the application window. To use this option is needed to enable the layer mode by using #ikgl::gui::enable_layer_mode. It's possible to disable color key option by using #ikgl::gui::disable_transparent_colorkey . Color key option is disable by default.
		*/
		void_t enable_transparent_colorkey();

		/**
		* This is a version of #ikgl::gui::enable_layer_mode to be used out of safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		*/
		void_t enable_layer_mode_lock();

		/**
		* This is a version of #ikgl::gui::enable_transparent_colorkey to be used out of safe context.  You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		*/
		void_t enable_transparent_colorkey_lock();

		/**
		* Function used to obtain a pointer to a layer previuosly allocated. The layer name is to be unique.
		* @param layername is a name to find in layermap.
		* \return this function return a pointer to a layer previusly allocated, or nullptr if the layername is not found into the map.
		*/
		layer* get_layer(string_t layername);

		/**
		* Function used to get the picked window by the position.
		* @param x is the horizontal axis coordinate to use to obtain the picked window.
		* @param y is the vertical axis coordinate to use to obtain the picked window.
		* \return The function return the widget picked in the input position. The function may return nullptr if there arent widget that it contains the input position.
		*/
		window* get_picked(coord_t x, coord_t y);

		/**
		* Function used to get the picked window by the position.
		* @param pos is the input position to be checked.
		* \return The function return the widget picked in the input position. The function may return nullptr if there arent widget that it contains the input position.
		*/
		window* get_picked(widget_pos_t pos);

		/**
		* This is a version of #ikgl::gui::get_picked to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param x is the horizontal axis coordinate to use to obtain the picked window.
		* @param y is the vertical axis coordinate to use to obtain the picked window.
		* \return The function return the widget picked in the input position. The function may return nullptr if there arent widget that it contains the input position.
		*/
		window* get_picked_lock(coord_t x, coord_t y);

		/**
		* This is a version of #ikgl::gui::get_picked to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param pos is the input position to be checked.
		* \return The function return the widget picked in the input position. The function may return nullptr if there arent widget that it contains the input position.
		*/
		window* get_picked_lock(widget_pos_t pos);

		/**
		* Function used to obtain the focused widget. The function may return nullptr if there aren't any focused widget.
		* \return this function return the focused widget. The function may return nullptr if there aren't any focused widget.
		*/
		window* get_focused();

		/**
		* This is a version of #ikgl::gui::get_focused to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return this function return the focused widget. The function may return nullptr if there aren't any focused widget.
		*/
		window*	get_focused_lock();

		/**
		* Function used to obtain a #ikgl::window pointer that it's the widget that own the input name. You can use #ikgl::window::set_window_name to set the name to the widget. The name to be set have to be unique.
		* @param name is the name to be find.
		* \return The function return a pointer to the window instance of the widget that it own the input name. The function may return nullptr if the name is not found.
		*/
		window* get_window_by_name(const wstring_t& name);

		/**
		* Function used to obtain a #ikgl::window pointer that it's the widget that own the input name. You can use #ikgl::window::set_window_name to set the name to the widget. The name to be set have to be unique. The overload that it use the wstring_t parameter is a bit more efficient because doesn't need to convert the string into a wstring.
		* @param name is the name to be find.
		* \return The function return a pointer to the window instance of the widget that it own the input name. The function may return nullptr if the name is not found.
		*/
		window* get_window_by_name(const string_t& name);


		/**
		* Function used to obtain the min resize size set to the application window. The return maybe a zero object if the gui didn't set any min size.
		* \return This function return a const reference to the ikgl::gui object member.
		*/
		const widget_size_t& get_window_min_resize() const;

		/**
		* Function used to obtain the maximum resize size set to the application window. The return maybe a zero object if the gui didn't set any maximum size.
		* \return This function return a const reference to the ikgl::gui object member.
		*/
		const widget_size_t& get_window_max_resize() const;

		/**
		* This is a version of #ikgl::gui::get_window_min_resize to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return This function return a copy of the ikgl::gui object member.
		*/
		const widget_size_t get_window_min_resize_lock();

		/**
		* This is a version of #ikgl::gui::get_window_max_resize to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return This function return a copy of the ikgl::gui object member.
		*/
		const widget_size_t get_window_max_resize_lock();

		/**
		* Function used to obtain the last rendering timestamp.
		* \return the last rendering timestamp.
		*/
		double_t get_last_rendering_time();

		/**
		* This is a version of #ikgl::gui::get_last_rendering_time to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return the last rendering timestamp.
		*/
		double_t get_last_rendering_time_lock();

		/**
		* Function used to obtain the last update timestamp.
		* \return the last update timestamp.
		*/
		double_t get_last_updating_time();

		/**
		* This is a version of #ikgl::gui::get_last_updating_time to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return the last update timestamp.
		*/
		double_t get_last_updating_time_lock();

		/**
		* Function used to obtain the set value for maximum fps. By default the value of maximum fps is #ikgl::DEFAULT_FPS. You can set the maximum value for fps by using #ikgl::gui::set_fps
		* \return the set value for maximum fps.
		*/
		uint_t get_fps() const;

		/**
		* This is a version of #ikgl::gui::get_fps to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return the set value for maximum fps.
		*/
		uint_t get_fps_lock();

		/**
		* Function used to obtain the set value for maximum ups. By default the value of maximum ups is #ikgl::DEFAULT_UPS. You can set the maximum value for fps by using #ikgl::gui::set_ups .
		* \return the set value for maximum ups.
		*/
		uint_t get_ups() const;

		/**
		* This is a version of #ikgl::gui::get_ups to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return the set value for maximum ups.
		*/
		uint_t get_ups_lock();

		/**
		* Function used to obtain the HANDLE to the DC object that it's own by the aplication window. You can find more information about DC object in Microsoft Documentation.
		* \return This function the HANDLE to the DC object that it's own by the aplication window.
		*/
		hdc_t get_hdc();

		/**
		* Function used to obtain the HANDLE to the WND object that it's own by the aplication window. You can find more information about WND object in Microsoft Documentation.
		* \return This function the HANDLE to the WND object that it's own by the aplication window.
		*/
		hwnd_t get_hwnd();

		/**
		* Function used to obtain the HANDLE to the INSTANCE object that it's own by the aplication window. You can find more information about INSTANCE object in Microsoft Documentation.
		* \return This function the HANDLE to the INSTANCE object that it's own by the aplication window.
		*/
		hinstance_t get_hinstance();

		/**
		* This is a version of #ikgl::gui::get_hwnd to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return This function the HANDLE to the WND object that it's own by the aplication window.
		*/
		hwnd_t get_hwnd_lock();

		/**
		* This is a version of #ikgl::gui::get_hinstance to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return This function the HANDLE to the INSTANCE object that it's own by the aplication window.
		*/
		hinstance_t get_hinstance_lock();

		/**
		* Function used to obtain a constant reference to the member that it contains the mouse position.
		* \return This function returns a constant reference to the member that it contains the mouse position.
		*/
		const widget_pos_t& get_mouse_position();

		/**
		* This is a version of #ikgl::gui::get_mouse_position to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return This function returns a constant reference to the member that it contains the mouse position.
		*/
		const widget_pos_t& get_mouse_position_lock();

		/**
		* Function used to know the count of fps made in the last second. To obtain a real fps count you must enable the fps tracing by using #ikgl::gui::enable_trace_fps 
		* \return This function return the count of fps made in the last second if the fps tracing is enabled, otherwise it returns 0 or any invalid value.
		*/
		uint_t get_real_fps();

		/**
		* This is a version of #ikgl::gui::get_real_fps to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return This function return the count of fps made in the last second if the fps tracing is enabled, otherwise it returns 0 or any invalid value.
		*/
		uint_t get_real_fps_lock();

		/**
		* Function used to obtain a HANDLE to the CURSOR object that it's own by the application window.  You can find more information about CURSOR object in Microsoft Documentation.
		* \return this function returns a HANDLE to the CURSOR object that it's own by the application window.
		*/
		hcursor_t get_cursor();

		/**
		* This is a version of #ikgl::gui::get_cursor to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return this function returns a HANDLE to the CURSOR object that it's own by the application window.
		*/
		hcursor_t get_cursor_lock();

		/**
		* Function used to obtain an object RECT that it contains the rectangle information of the application window. You can find more information about RECT object in Microsoft Documentation.
		* \return this function returns an object RECT that it contains the rectangle information of the application window.
		*/
		mc_rect_t get_mc_rect();

		/**
		* Function used to obtain a #ikgl::widget_rect_t object that contains rectangle information of the application window. 
		* \return this function returns a #ikgl::widget_rect_t object that contains rectangle information of the application window. 
		*/
		widget_rect_t get_rect();

		/**
		* Function used to obtain a #ikgl::widget_size_t object that contains size information of the application window.
		* \return this function returns a #ikgl::widget_size_t object that contains size information of the application window.
		*/
		widget_size_t get_size();

		/**
		* Function used to obtain a #ikgl::widget_pos_t object that contains position information of the application window.
		* \return this function returns a #ikgl::widget_pos_t object that contains position information of the application window.
		*/
		widget_pos_t get_position();

		/**
		* Function used to obtain an object POINT that it contains the position information of the application window. You can find more information about POINT object in Microsoft Documentation.
		* \return this function returns a POINT object that contains position information of the application window.
		*/
		point_t get_position_as_point();

		/**
		* Function used to obtain the alpha factor set to the application window. The alpha factor is to be used only by enabling the layer mode and the transparent colorkey by using #ikgl::gui::enable_layer_mode and #ikgl::gui::enable_transparent_colorkey .
		* \return the alpha factor set to the application window.
		*/
		float_t get_alpha();

		/**
		* Function used to obtain the alpha color set as color to be transparent in the application window. The alpha color is by default equal to #ikgl::DEFAULT_ALPHA_COLOR . The alpha factor is to be used only by enabling the layer mode and the transparent colorkey by using #ikgl::gui::enable_layer_mode and #ikgl::gui::enable_transparent_colorkey .
		* \return the alpha color set as color to be transparent in the application window. 
		*/
		mc_colorref_t get_alpha_color();

		/**
		* This is a version of #ikgl::gui::get_alpha_color to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return the alpha color set as color to be transparent in the application window.
		*/
		mc_colorref_t get_alpha_color_lock();

		/**
		* Function used to obtain the value set to the force redraw flag of the object gui. The gui object may redraw itself even if the flag to redraw itself is set to false, if there are at least one widget that request the rendering.
		* \return This function returns the value set to the force redraw flag of the object gui.
		*/
		bool_t is_to_render();

		/**
		* Function used to know if the gui object is tracing the fps count. It's disable by default, You can enable the fps tracing by using #ikgl::gui::enable_trace_fps and disable it by using #ikgl::gui::disable_trace_fps .
		* \return This function returns true if the gui object is tracing the fps count.
		*/
		bool_t is_trace_fps_enabled();

		/**
		* Function used to know if it's set the flag to redraw the gui object without check the widget rendering requests. It's disable by default, You can enable the option by using #ikgl::gui::enable_always_render and disable it by using #ikgl::gui::disable_always_render . The always render option is usefull to increase performance when it is practically obvious that at least one of the widgets will require rendering. This avoids controlling all the widgets and directly renders the interface, saving nanoseconds.
		* \return This function returns true if it's set the flag to redraw the gui object without check the widget rendering requests.
		*/
		bool_t is_always_render();

		/**
		* Function used to know if the mouse is outside of the application window.
		* \return This function returns true if the mouse is outside of the application window.
		*/
		bool_t is_mouse_outside();

		/**
		* Function used to know if the application window is actually minimized.
		* \return This function returns true if the application window is actually minimized.
		*/
		bool_t is_minimized();

		/**
		* Function used to know if the layer mode is enabled. You can find more information about this flag to the #ikgl::gui::enable_layer_mode description.
		* \return the value of the flag that enable the layer mode.
		*/
		bool_t is_layer_mode_enabled();

		/**
		* This is a version of #ikgl::gui::is_layer_mode_enabled to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* \return the value of the flag that enable the layer mode.
		*/
		bool_t is_layer_mode_enabled_lock();

		/**
		* Function used to maximize the application window.
		* \return true if the application window is maximized correctly.
		*/
		bool_t maximize();

		/**
		* Function used to minimize the application window.
		* \return true if the application window is minimized correctly.
		*/
		bool_t minimize();

		/**
		* Function used to move the application window to the destination position.
		* @param pos is the destination position.
		* \return true if the window is moved correctly.
		*/
		bool_t move_window(const widget_pos_t& pos);

		/**
		* Function used to start the message loop. It's recommended to start the message when the gui object is correctly create, initialize, set.
		* @param ms is the amount of ms to sleep when the message queue is empty.
		* \return false if the gui object is not ready to use the message loop.
		*/
		bool_t message_loop(const dword_t ms);

		/**
		* Function used to start the message loop and running an event to every cycle. It's recommended to start the message when the gui object is correctly create, initialize, set.
		* \return false if the gui object is not ready to use the message loop.
		*/
		bool_t message_loop(loop_message_event_t event);

		/**
		* Function used to call the message procedure. It's automatically used into the message loop but it's a public methods to allow the use in customized loop.
		* @param hWnd is the handle to the window that receiving the message.
		* @param message is the message received.
		* @param wParam is the wparam sent with the message.
		* @param lParam is the lparam sent with the message.
		* \return the procedure result.
		*/
		wnd_procedure_result_t message_procedure(hwnd_t hWnd, uint_t message, wparam_t wParam, lparam_t lParam);

		/**
		* Function used to obtain the result of the interrogate the object and its widgets on the need to be rendered. The function also calls the callback of the 'on_render' event on all widgets of the gui object.
		* \return The function return true if at least one widget or the gui object request the rendering.
		*/
		bool_t on_render();

		/**
		* Function used to post a quit message to the message queue (this is used to solve the problem of posting a quit message from a different thread).
		*/
		void_t quit();

		/**
		* Function used to remove the layer from the layer map. The removing of the layer will destroy it and its widgets.
		* @param layername is the name to identify the layer to be removed.
		* \return The function return true if the layer is successfully found and removed.
		*/
		bool_t remove_layer(string_t layername);

		/**
		* Function used to call the render action into the hdc passed as argument.
		* @param hdc is a HANDLE to a DC object. You can find more about the DC object in the Microsoft Documentation.
		*/
		void_t render(hdc_t& hdc);

		/**
		* Function used to resize the application window. The function also calls the callback of the 'on_resize_gui' event on all the widgets owns by the gui object.
		* @param size is the destination size to be set.
		* \return true if the application window is resized correctly 
		*/
		bool_t resize_window(const widget_size_t& size);

		/**
		* Function used to register a new message handler. The message handler can prevend the default handle by return #ikgl::message_return_t::PREVENT_DEFAULT . The message event window are called just when the message is sent to the application window.
		* @param type is the message to be handled.
		* @param event is the handler to the message.
		*/
		void_t register_message_event_window(uint_t type, message_event_t event);

		/**
		* Function used to register a new message handler. The message handler can prevend the default handle by return #ikgl::message_return_t::PREVENT_DEFAULT . Differently from the event window, the event general are called without check the handle to the window that owns the message.
		* @param type is the message to be handled.
		* @param event is the handler to the message.
		*/
		void_t register_message_event_general(uint_t type, message_event_t event);

		/**
		* Function used to register a table of #ikgl::message_event_info_t .  The message event window are called just when the message is sent to the application window.
		* @param event a pointer to the first element of the table (table also called array)
		* @param uiCount the table (array) size.
		*/
		void_t register_message_event_window_table(message_event_info_t* event, uint_t uiCount);

		/**
		* Function used to register a table of #ikgl::message_event_info_t . Differently from the event window, the event general are called without check the handle to the window that owns the message.
		* @param event a pointer to the first element of the table (table also called array)
		* @param uiCount the table (array) size.
		*/
		void_t register_message_event_general_table(message_event_info_t* event, uint_t uiCount);

		/**
		* Function used to set the button icon cursor.
		*/
		void_t set_button_cursor();

		/**
		* Function used to set the normal icon cursor.
		*/
		void_t set_normal_cursor();

		/**
		* Function used to set the fps maximum value. You can obtain the maximum set fps value by using #ikgl::gui::get_fps . By default the fps maximum value is set to #ikgl::DEFAULT_FPS .
		* @param fps is the value to set as maximum fps value.
		*/
		void_t set_fps(uint_t fps);

		/**
		* This is a version of #ikgl::gui::set_fps to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param fps is the value to set as maximum fps value.
		*/
		void_t set_fps_lock(uint_t fps);

		/**
		* Function used to set the ups maximum value. You can obtain the maximum set ups value by using #ikgl::gui::get_ups . By default the ups maximum value is set to #ikgl::DEFAULT_UPS .
		* @param ups is the value to set as maximum ups value.
		*/
		void_t set_ups(uint_t ups);

		/**
		* This is a version of #ikgl::gui::set_ups to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param ups is the value to set as maximum ups value.
		*/
		void_t set_ups_lock(uint_t ups);

		/**
		* Function used to set a custom HCURSOR to the application window. You can find more about the HCURSOR by looking into Microsoft Documentation.
		* @param hCursor is a HANDLE to the CURSOR object to set.
		*/
		void_t set_cursor(hcursor_t hCursor);

		/**
		* This is a version of #ikgl::gui::set_cursor to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param hCursor is a HANDLE to the CURSOR object to set.
		*/
		void_t set_cursor_lock(hcursor_t hCursor);

		/**
		* Function used to set the alpha factor to the application window. to be used the alpha factor need to enable the layer mode and the transparency color key. Read more about in #ikgl::gui::enable_transparent_colorkey description.
		* @param alpha is the value to set as alpha factor. (0.0 ~ 1.0)
		*/
		void_t set_alpha(float_t alpha);

		/**
		* Function used to set the alpha color to be used to draw the transparent part of the application window. You can see more about by looking the #ikgl::gui::get_alpha_color description.
		* @param color is the color to be set as alpha color.
		*/
		void_t set_alpha_color(mc_colorref_t color);

		/**
		* This is a version of #ikgl::gui::set_alpha_color to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param color is the color to be set as alpha color.
		*/
		void_t set_alpha_color_lock(mc_colorref_t color);

		/**
		* Function used to set a minimum size to the application window.
		* @param size is the value to set as minimum size.
		*/
		void_t set_window_min_resize(const widget_size_t& size);

		/**
		* Function used to set a maximum size to the application window.
		* @param size is the value to set as maximum size.
		*/
		void_t set_window_max_resize(const widget_size_t& size);

		/**
		* This is a version of #ikgl::gui::set_window_min_resize to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param size is the value to set as minimum size.
		*/
		void_t set_window_min_resize_lock(const widget_size_t& size);

		/**
		* This is a version of #ikgl::gui::set_window_max_resize to be used out of the safe context. You can know better when it is really needed to lock the mutex by reading the #ikgl::gui::execute_lock description.
		* @param size is the value to set as maximum size.
		*/
		void_t set_window_max_resize_lock(const widget_size_t& size);

		/**
		* Function used to start the rendering thread. After the start of the rendering thread is important to use locked methods to prevent concurrency. Read more about the locked methods in #ikgl::gui::execute_lock description.
		* \return true if the rendering is started successfully.
		*/
		bool_t start_rendering_thread();

		/**
		* Function used to start the update thread. After the start of the update thread is important to use locked methods to prevent concurrency. Read more about the locked methods in #ikgl::gui::execute_lock description.
		* \return true if the update is started successfully.
		*/
		bool_t start_updating_thread();

	private:
		
		void_t __clear_rendering_buffer(hdc_t hdcMem, hbitmap_t hBitmap, hbitmap_t hBitmapOld);		
		bool_t __is_window_initialized();
		hbitmap_t __initialize_rendering_buffer(hdc_t hdcDest, hdc_t& hdcMem, hbitmap_t& hBitmap);
		int_t __mt_render();
		int_t __mt_update();
		bool_t __mt_update_ex();
		bool_t __mt_render_ex();
		void_t __minimize_window();
		void_t __on_mousebutton(uint_t message, point_t p);
		void_t __on_mousemove(const widget_pos_t& newpos);
		void_t __on_mouseleave();
		void_t __on_mousewheel(wparam_t wParam, lparam_t lParam);
		void_t __on_key_down(const key_message_info_t& message_info);
		void_t __on_key_up(const key_message_info_t& message_info);
		void_t __restore_window();
		void_t __register_window_name(window* win, const wstring_t& new_name);
		void_t __shut();
		void_t __set_last_rendering_time(double_t dTime);
		void_t __set_last_update_time(double_t dTime);
		void_t __set_render_made();
		void_t __swap_rendering_buffer(hdc_t hdcDest, hdc_t hdcMem, hbitmap_t hBitmap);
		void_t __unregister_window_name(const wstring_t& name);
		void_t __update_fps_counter();
		void_t __check_for_peeked(ikgl::window* ptr);
		void_t __register_destroyed(ikgl::window* ptr);

	private:
		message_map _message_event_map_window;
		message_map _message_event_map_general;
		layer_map _layer_map;
		window_name_map_t _window_name_map;

		window* _peeked_window_left;
		window* _peeked_window_right;
		window* _focused_window;

		widget_size_t _size;
		widget_size_t _min_size;
		widget_size_t _max_size;
		widget_pos_t _mouse_pos;
		fps_trace_data_t _fps_data;

		hwnd_t _hwnd;
		hinstance_t _hinstance;
		hdc_t _hdc;
		hcursor_t _hcursor;

		mutex_t _mutex;
		thread_t* _rendering_thread;
		thread_t* _updating_thread;
			
		atomic_bool_t _is_mouse_outside_window;
		atomic_bool_t _is_to_render;
		atomic_bool_t _is_trace_fps;
		atomic_bool_t _is_always_render;
		atomic_bool_t _is_minimized;
		atomic_bool_t _is_use_colorkey;
		atomic_bool_t _is_over_button;
		atomic_bool_t _is_quit;
		bool_t _is_start_mouse_tracking;
		bool_t _is_shut;

		atomic_float_t _alpha;
		mc_colorref_t _alpha_color;

		uint_t _fps;
		uint_t _ups;
		
		double_t _last_refresh;
		double_t _last_update;
		ulong_ptr_t _gdiplus_token;

		std::deque<ikgl::window*> _destroyed;
	};

	/**
	* Function used to cast the widget pointer. The assert prevent the wrong castings.
	* @param in_ is the pointer input to be casted into a T pointer.
	* \return the function return a pointer to the casted object.
	*/
	template <class T>
	T* widget_cast(window* in_) {
		assert(in_ && in_->get_widget_type() == T::type());
		return static_cast<T*>(in_);
	}

}


#endif //__INCLUDE_HEADER_GUI_IKARUSGL__


#undef __critical_scope