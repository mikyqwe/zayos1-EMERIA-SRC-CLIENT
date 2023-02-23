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
#pragma once

namespace ikgl {
	
	/**
	* the animation widget is intended as a multi-frame image, to which you can add images as new frames and adjust the delay with which the frame will be changed.
	*/
	class animation : public window {

	public:

		/**
		* the class constructor will be used by the #create_element function to allocate the object and assign the pointer of the gui object that owns the widget
		* @param guiptr is a pointer to the #ikgl::gui object that owns the widget
		*/
		animation(gui* guiptr);
		~animation();

		/**
		* Function used to apply a color to be superimposed on images when rendered
		* @param color is the color to apply to the images.
		*/
		void_t apply_color(const color_rgba_t& color);

		/**
		* Function used to append an image as frame to the object animation
		* @param path is the file path used to load the image from file.
		* \return the function return false if it fails to find or to load the image from the path.
		*/
		bool_t append_image(const string_t& path);

		/**
		* Function used to append an image as frame to the object animation
		* @param path is the file path used to load the image from file.
		* \return the function return false if it fails to find or to load the image from the path.
		*/
		bool_t append_image(const wstring_t& path);

		/**
		* Function used to append an image as frame to the object animation
		* @param data is a pointer to the bytes of the image to load it from stream.
		* @param len is the amount of bytes of the image used to load it from stream.
		* \return the function return false if it fails to load the image from the memory buffer
		*/
		bool_t append_image(const void_t* data, const dword_t len);

		/**
		* Function used to append an image as frame to the object animation
		* @param hbitmap is an handle to the bitmap object used to load the image (Microsoft Docs explain about how to obtain an HBITMAP)
		* @param pixel_format is the gdiplus pixel format used to load the image
		* \return the function return false if it fails to load the image from the bitmap handle
		*/
		bool_t append_image(hbitmap_t hbitmap, gdiplus_pixel_format_t pixel_format);

		/**
		* Function used to reset the object (for example if you want to reuse it as a new object but without deleting it from memory)
		*/
		void_t clear();


		/**
		* function used to draw the widget (used internally by the gui object but also usable if you are using a custom rendering method)
		* @param hdc is and handle to a DC object (Microsoft docs contains the information about the DC/HDC)
		* @param graphicsptr is a pointer to a gdiplus graphic object used to draw the widget (Micosoft docs contains the information about Gdiplus::Graphics)
		*/
		void_t draw(hdc_t& hdc, gdiplus_graphics_t* graphicsptr);


		/**
		* Function used to obtain the default name (of an unnamed widget)
		* \return the function return a wstring_t object contains the default name of the widget
		*/
		static wstring_t empty_name() { return L"NONAME_ANIMATION"; }
		

		/**
		* Function used to know if the widget will use a static or dinamyc draw 
		* \return the function return true if the widget options don't needs to use a dynamic draw
		*/
		bool_t is_static_draw();
		
		/**
		* Function used to obtain the recting scale set to the widget
		* \return the function return the recting scale set to the widget
		*/
		rect_scale_t get_rect_scale();

		/**
		* Function used to obtain the recting scale set to the widget
		* @param rScale [out] is used to contain the rect scale of the widget
		*/
		void_t get_rect_scale(rect_scale_t& rScale);

		/**
		* Function used to obtain the recting scale set to the widget
		* @param rLeft [out] is used to contain the left recting scale of the widget
		* @param rTop [out] is used to contain the top recting scale of the widget
		* @param rRight [out] is used to contain the right recting scale of the widget
		* @param rBottom [out] is used to contain the bottom recting scale of the widget
		*/
		void_t get_rect_scale(float_t& rLeft, float_t& rTop, float_t& rRight, float_t& rBottom);

		/**
		* Function used to obtain the delay value set to the widget to switch the frames
		* \return the function return the delay value set to the widget to switch the frames
		*/
		double_t get_delay();

		/**
		* Function used to know if the widget needs to be redrawed (or if one of its children needs it) and to trigger the specific event "on render event" for which it is possible to set a callback via #ikgl::window::set_on_render
		* \return the function return true if the widget or a child needs to be redrawed
		*/
		bool_t on_render();

		//* the set size operation reset the rectscale 
		//* so warning to use it before to set the scale if you need it

		/**
		* Function used to set the size of the image. The set size operation reset the rectscale 
		* @param width to set to the image
		* @param height to set to the image
		*/
		void_t set_size(const coord_t width, const coord_t height);

		/**
		* Function used to set the size of the image. The set size operation reset the rectscale 
		* @param size contains the width and height to set to the image
		*/
		void_t set_size(const widget_size_t& size);

		/**
		* Function used to assign a recting scale to the image.
		* @param rLeft contains the left scale to rect the image
		* @param rTop contains the top scale to rect the image
		* @param rRight contains the right scale to rect the image
		* @param rBottom contains the bottom scale to rect image
		*/
		void_t set_rect_scale(const float_t rLeft, const  float_t rTop, const float_t rRight, const float_t rBottom);

		/**
		* Function used to assign a recting scale to the image.
		* @param rScale contains the scale to use to rect the image
		*/
		void_t set_rect_scale(const rect_scale_t& rScale);

		/**
		* Function used to set the delay to switch the frames.
		* @param seconds contains the value of seconds to switch delayed the frames (value in seconds e.g. 0.1s to set 10fps)
		*/
		void_t set_delay(double_t seconds);


		/**
		* Function used obtain the type index of the widget type
		* \return The function return the constant #ikgl::widget_type_t::animation
		*/
		static widget_type_t type() { return widget_type_t::animation; }
		
		
	private:
		/**
		* Function used to clear the animation members (it is called in #clear)
		*/
		void_t __clear_animation();

		/**
		* Function used to obtain the frame image pointer to render (it is called in #draw method)
		* \return the function return the frame image pointer to render
		*/
		gdiplus_image_t* __get_frame();


	private:
		std::vector<gdiplus_image_t*> _images;
		rect_scale_t _rect_scale;
		widget_size_t _image_size;
		color_rgba_t _apply_color;
		int_t _cur_frame;
		double_t _next_frame_change;
		double_t _delay;
	};


}