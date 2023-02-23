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
#ifndef __INCLUDE_HEADER_DRAW_IKARUSGL__
#define __INCLUDE_HEADER_DRAW_IKARUSGL__
namespace ikgl{

	inline void calculate_path(gdiplus_graphics_path_t* path, const gdiplus_rect_t& rect, int dia)
	{
		// diameter can't exceed width or height
		if (dia > rect.Width)    dia = rect.Width;
		if (dia > rect.Height)   dia = rect.Height;
		// define a corner 
		gdiplus_rect_t corner(rect.X, rect.Y, dia, dia);
		// begin path
		path->Reset();
		// top left
		path->AddArc(corner, 180, 90);
		// top right
		corner.X += (rect.Width - dia - 1);
		path->AddArc(corner, 270, 90);
		// bottom right
		corner.Y += (rect.Height - dia - 1);
		path->AddArc(corner, 0, 90);
		// bottom left
		corner.X -= (rect.Width - dia - 1);
		path->AddArc(corner, 90, 90);
		// end path
		path->CloseFigure();
	}



	inline void_t draw_rect(gdiplus_graphics_t* grptr, const gdiplus_rect_t& rect, const color_rgba_t& color) {
		grptr->FillRectangle(&gdiplus_solid_brush_t(color.to_gdiplus_argb()), rect);
	}

	inline void_t draw_round_rect(gdiplus_graphics_t* grptr, const gdiplus_rect_t& rect, const color_rgba_t& color) {
		gdiplus_graphics_path_t path;
		gdiplus_int_t radius = utilities::minof(rect.Width, rect.Height) / 50;
		if (radius == 0)
			radius = 1;

		calculate_path(&path, rect, radius*2);
		grptr->FillPath(&gdiplus_solid_brush_t(color.to_gdiplus_argb()), &path);
	}

	inline void_t draw_round_rect(gdiplus_graphics_t* grptr, const gdiplus_rect_t& rect, const color_rgba_t& color, uint_t border_radius) {
		gdiplus_graphics_path_t path;
		gdiplus_int_t radius = border_radius;

		calculate_path(&path, rect, radius * 2);
		grptr->FillPath(&gdiplus_solid_brush_t(color.to_gdiplus_argb()), &path);
	}


	inline void_t draw_round_rect_bordered(gdiplus_graphics_t* grptr, const gdiplus_rect_t& rect, const color_rgba_t& color, const color_rgba_t& border_color, uint_t bordersize) {
		gdiplus_graphics_path_t path;
		gdiplus_int_t radius = utilities::minof(rect.Width, rect.Height) / 50;
		if (radius == 0)
			radius = 1;

		calculate_path(&path, rect, radius * 2);

		//draw fill
		grptr->FillPath(&gdiplus_solid_brush_t(color.to_gdiplus_argb()), &path);

		//draw border
		gdiplus_solid_brush_t borderBrush(border_color.to_gdiplus_argb());
		grptr->DrawPath(&gdiplus_pen_t(&borderBrush, gdiplus_real_t(bordersize)), &path);
	}

	inline void_t draw_round_rect_bordered(gdiplus_graphics_t* grptr, const gdiplus_rect_t& rect, const color_rgba_t& color, const color_rgba_t& border_color, uint_t bordersize, uint_t border_radius) {
		gdiplus_graphics_path_t path;
		gdiplus_int_t radius = border_radius;

		calculate_path(&path, rect, radius * 2);

		//draw fill
		grptr->FillPath(&gdiplus_solid_brush_t(color.to_gdiplus_argb()), &path);

		//draw border
		gdiplus_solid_brush_t borderBrush(border_color.to_gdiplus_argb());
		grptr->DrawPath(&gdiplus_pen_t(&borderBrush, gdiplus_real_t(bordersize)), &path);
	}


	inline void_t draw_rect_bordered(gdiplus_graphics_t* grptr, const gdiplus_rect_t& rect, const color_rgba_t& color, const color_rgba_t& border_color, uint_t bordersize) {
		draw_rect(grptr, rect, color);

		gdiplus_solid_brush_t borderBrush(border_color.to_gdiplus_argb());
		grptr->DrawRectangle(&gdiplus_pen_t(&borderBrush, gdiplus_real_t(bordersize)), rect);
	}



}
#endif //__INCLUDE_HEADER_DRAW_IKARUSGL__
