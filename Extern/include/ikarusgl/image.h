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
#ifndef __INCLUDE_HEADER_IMAGE_IKARUSGL__
#define __INCLUDE_HEADER_IMAGE_IKARUSGL__	

namespace ikgl
{
	class image : public window
	{
	public:
		image(gui* guiptr);
		~image();

		/**
		* Function used to apply a color to the image.
		* @param color is the color to be applied to the image.
		*/
		void_t apply_color(const color_rgba_t& color);

		/**
		* Function used to reset the image object.
		*/
		void_t clear();
		
		/**
		* Function used to draw the image. It's automatically used by the #ikgl::gui object. This allow the use of the widget into a customized rendering function.
		* @param hdc is a HANDLE to the DC object. You can find more about DC object in the Microsoft Documentation.
		* @param graphicsptr is a pointer to the Gdiplus::Graphics object. You can find more about Gdiplus::Graphics object in the Microsoft Documentation.
		*/
		void_t draw(hdc_t& hdc,  gdiplus_graphics_t* graphicsptr);

		/**
		* Function used to obtain STATICALLY the empty name of the widget.
		* \return STATICALLY the empty name of the widget.
		*/
		static wstring_t empty_name() { return L"NONAME_IMAGE"; }
		
		/**
		* Function used to obtain the recting information of the image. The recting information are used to draw the image in percentage or multiple times (it depends how they are set)
		* \return recting information of the image.
		*/
		rect_scale_t get_rect_scale();

		/**
		* Function used to obtain the recting information of the image. The recting information are used to draw the image in percentage or multiple times (it depends how they are set)
		* @param rScale will contian the recting information of the image.
		*/
		void_t get_rect_scale(rect_scale_t& rScale);

		/**
		* Function used to obtain the recting information of the image. The recting information are used to draw the image in percentage or multiple times (it depends how they are set)
		* @param rLeft will contian the left recting information of the image.
		* @param rTop will contian the top recting information of the image.
		* @param rRight will contian the right recting information of the image.
		* @param rBottom will contian the bottom recting information of the image.
		*/
		void_t get_rect_scale(float_t& rLeft, float_t& rTop, float_t& rRight, float_t& rBottom);

		/**
		* Function used to obtain the original size of the image.
		* \return the original size of the image.
		*/
		widget_size_t get_real_image_size();

		/**
		* Function used to know the mode used to draw the image. 
		* \return true if the image is drawed statically, otherwise false.
		*/
		bool_t is_static_draw();

		/**
		* Function used to to load the image from file.
		* @param path is the path to be used to load the image.
		* \return true fi the image is loaded sucessfully, otherwise false.
		*/
		bool_t load_image(string_t path);

		/**
		* Function used to to load the image from file.
		* @param path is the path to be used to load the image.
		* \return true fi the image is loaded sucessfully, otherwise false.
		*/
		bool_t load_image(wstring_t path);

		/**
		* Function used to to load the image from buffer.
		* @param data is the pointer to the bytes of the image.
		* @param len is buffer lenght.
		* \return true fi the image is loaded sucessfully, otherwise false.
		*/
		bool_t load_image(void_t* data, dword_t len);

		/**
		* Function used to load the image from an existring BITMAP object. You can find more about BITMAP object in Microsoft Documentation
		* @param hbitmap is a HANDLE to the BITMAP object to be used to laod the image.
		* @param pixel_format is the color format to use to load the image.
		* \return true if the image is loaded successfully, otherwise false.
		*/
		bool_t load_image(hbitmap_t hbitmap, gdiplus_pixel_format_t pixel_format);

		/**
		* Function used to run the callback event 'on_render' set on the image and to receive the rendering requesting answer from the widget and its children.
		* \return true if at least one of the children or the widget needed the rendering.
		*/
		bool_t on_render();

		/**
		* Function used to set the size to the image. This method reset the rect scale.
		* @param width is the width to set to the image
		* @param height is the height to set to the image
		*/
		void_t set_size(coord_t width, coord_t height);

		/**
		* Function used to set the size to the image. This method reset the rect scale.
		* @param size is the destination size to set.
		*/
		void_t set_size(const widget_size_t& size);

		/**
		* Function used to set the recting scale to the image. This method can repeate the image while rendering or truncate it for value < 1.0 
		* @param rLeft is the left recting scale to apply to the image
		* @param rTop is the top recting scale to apply to the image
		* @param rRight is the right recting scale to apply to the image
		* @param rBottom is the bottom recting scale to apply to the image
		*/
		void_t set_rect_scale(float_t rLeft, float_t rTop, float_t rRight, float_t rBottom);

		/**
		* Function used to set the recting scale to the image. This method can repeate the image while rendering or truncate it for value < 1.0
		* @param rScale is the recting scale to apply to the image.
		*/
		void_t set_rect_scale(rect_scale_t& rScale);

		/**
		* Function used to return statically the widget type
		* \return statically the widget type
		*/
		static widget_type_t type() { return widget_type_t::image; }
		
	protected:
		void_t __clear_image();
		void_t __update_size_translating();

	protected:
		rect_scale_t			_rect_scale;
		widget_size_t			_image_size;
		color_rgba_t			_apply_color;
		gdiplus_image_t*		_gdi_image;

	};
}

#endif //__INCLUDE_HEADER_IMAGE_IKARUSGL__


