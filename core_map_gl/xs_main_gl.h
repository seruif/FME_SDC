#include "../xseruif_mapi.h"
#include <GL/glut.h> 

#define XSGL_LEVEL_DEBUG 2

int xsgl_level;

//void xsgl_convert_(int off)
//{
//	for (map<int, XSBuild>::iterator i = xs_map.builds.begin(); i != xs_map.builds.end(); i++)
//	{
//		XSBuild* b = &(i->second);
//		b->coord.Y += off;
//		for (map<int, XSFloor>::iterator j = b->floors.begin(); j != b->floors.end(); j++)
//		{
//			XSFloor* f = &(j->second);
//		}
//	}
//}

bool debug_mode;
bool debug_show_name;

void xsgl_debug_mode(bool name)
{
	debug_mode = true;
	debug_show_name = name;
}

float* xsgl_Color3f(int color)
{
	int r, g, b;
	b = color % 128;
	color = color / 128;
	g = color % 128;
	color = color / 128;
	r = color;

	float* f = new float[3];
	f[0] = r / 127.0;
	f[1] = g / 127.0;
	f[2] = b / 127.0;

	return f;
}

void xsgl_Color3(int color)
{
	GLbyte r, g, b;
	b = color % 128;
	color = color / 128;
	g = color % 128;
	color = color / 128;
	r = color;
	glColor3b(r, g, b);
}

float xsgl_map_scale() { return xs_map_types.map_scale; }

void xsgl_draw_backarea(XSBackgroundArea area, XSVectorF build_pos)
{
	glBegin(GL_QUADS);
	//glColor3d(1.0, 0, 0);
	xsgl_Color3(area.type->color);
	glVertex2d(area.coord1.X, area.coord1.Y);
	glVertex2d(area.coord2.X, area.coord1.Y);
	glVertex2d(area.coord2.X, area.coord2.Y);
	glVertex2d(area.coord1.X, area.coord2.Y);
	glEnd();
}

void xsgl_draw_backareas(XSFloor floor, XSVectorF build_pos)
{
	XSBackgroundArea area;
	for (int i = 0; i < floor.Zs.size(); i++)
	{
		for (int k = 0; k < floor.backgrounds.size(); k++)
			if (floor.Zs[i] == floor.backgrounds[k].z)
			{
				area = floor.backgrounds[k];
				xsgl_draw_backarea(area, build_pos);
			}
	}
}

void xsgl_draw_walls(XSFloor floor, XSVectorF build_pos)
{
	XSWall wall;
	for (int k = 0; k < floor.walls.size(); k++)
	{
		XSWall* wl = &(floor.walls[k]);
		glLineWidth(floor.walls[k].width);
		if (wl->type->strip)
		{
			glLineStipple(wl->type->strip_factor, wl->type->strip_pattern);
			glEnable(GL_LINE_STIPPLE);
		}
		else
			glDisable(GL_LINE_STIPPLE);

		glBegin(GL_LINES);

		wall = floor.walls[k];
		xsgl_Color3(wall.type->background_type->color);
		glVertex2d(wall.coord1.X, wall.coord1.Y);
		glVertex2d(wall.coord2.X, wall.coord2.Y);

		glEnd();
	}
}

void xsgl_draw_stairs(XSFloor floor, XSVectorF build_pos)
{
	for (int i = 0; i < floor.stairs.size(); i++)
	{
		XSStair s = floor.stairs[i];
		XSWallType* wt = s.wall_type;
		string ori = s.orientation;
		XSVectorF p1 = s.coord1;
		XSVectorF p2 = s.coord2;

		xsgl_Color3(wt->background_type->color);

		if (!wt->strip)
			throw runtime_error("Wall for stairs is not strip");

		if (ori.compare(XS_PRM_HORIZONTAL) == 0)
		{
			float w = p2.Y - p1.Y;
			glLineWidth(w);
			p1.Y += (p2.Y - p1.Y) / 2;
			p2.Y = p1.Y;
			w = p2.X - p1.X;
			if (!s.full)
				w /= 4;
			p1.X += w;
			p2.X -= w;
		}
		if (ori.compare(XS_PRM_VERTICAL) == 0)
		{
			float w = p2.X - p1.X;
			glLineWidth(w);
			p1.X += (p2.X - p1.X) / 2;
			p2.X = p1.X;
			w = p2.Y - p1.Y;
			if (!s.full)
				w /= 4;
			p1.Y += w;
			p2.Y -= w;
		}

		glLineStipple(wt->strip_factor, wt->strip_pattern);
		glEnable(GL_LINE_STIPPLE);

		glBegin(GL_LINES);
		glVertex2d(p1.X, p1.Y);
		glVertex2d(p2.X, p2.Y);
		glEnd();

		//glDisable(GL_LINE_STIPPLE);

		//glLineWidth(wt->width*xsgl_map_scale());
		//glBegin(GL_LINES);
		//glVertex2d(p1.X, p1.Y);
		//glVertex2d(p2.X, p2.Y);
		//glEnd();
	}
}


void xsgl_draw_doors(XSFloor floor, XSVectorF build_pos)
{
	XSDoor door;
	for (int k = 0; k < floor.doors.size(); k++)
	{
		door = floor.doors[k];
		glBegin(GL_QUADS);
		//glColor3d(1.0, 0, 0);
		xsgl_Color3(door.type->background_type->color);
		glVertex2d(door.coord1.X, door.coord1.Y);
		glVertex2d(door.coord2.X, door.coord1.Y);
		glVertex2d(door.coord2.X, door.coord2.Y);
		glVertex2d(door.coord1.X, door.coord2.Y);
		glEnd();
	}
}

void xsgl_draw_text(string s, void* font, XSVectorF pos, XSVectorF build_pos)
{
	const char *cc;
	cc = s.c_str();
	char* c;
	c = const_cast<char*>(cc);

	glRasterPos2f(pos.X, pos.Y);

	for (c = c; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
}

void xsgl_draw_text_of_room(XSRoom r, void* font, XSVectorF build_pos)
{
	float w=13, h=8;
	//if (font == GLUT_BITMAP_8_BY_13)
	//{
	//	w = 13;
	//	h = 8;
	//}
	bool showinfo = false;
	if (font == GLUT_BITMAP_9_BY_15)
	{
		w = 15;
		h = 9;
		showinfo = true;
	}

	string s;
	XSVectorF pos;

	s = "";
	if (debug_show_name)
		s = r.name;
	if (s == "" || debug_mode)
		s = s.append(" [").append(to_string(r.id)).append("]");

	//pos = r.coord1 + (r.coord2 - r.coord1)*r.room_text_coord + build_pos;
	pos = r.real_text_coord;
	pos.X -= (float)strlen(s.c_str()) / 2 * h;// / xsgl_map_scale();
	pos.Y -= w / 3;// / xsgl_map_scale();

	glColor3f(0, 0, 0);
	xsgl_draw_text(s, font, pos, build_pos);

	if (showinfo)
	{
		s = r.info;

		XSBackgroundArea a;
		a.type = new XSBackgroundType;
		a.type->color = xs_map_types.selectroom_color;
		a.coord1 = r.coord1;
		a.coord2 = a.coord1;
		a.coord1.Y -= h/xsgl_map_scale()*2;
		a.coord2.X += (float)strlen(s.c_str())*(w / xsgl_map_scale())*0.6;

		xsgl_draw_backarea(a, build_pos);
		glColor3f(0, 0, 0);
		a.coord1.Y += 4 / xsgl_map_scale();
		xsgl_draw_text(s, font, a.coord1+build_pos, build_pos);
	}
}

void xsgl_draw_text_of_rooms(XSFloor floor, XSVectorF build_pos)
{
	XSRoom r;

	glColor3f(0, 0, 0);
	for (map<int, XSRoom>::iterator j = floor.rooms.begin(); j != floor.rooms.end(); j++)
	{
		r = j->second;
		xsgl_draw_text_of_room(r, GLUT_BITMAP_8_BY_13, build_pos);
	}
}

void xsgl_draw_selected_room(XSRoom room, XSVectorF build_pos)
{
	XSVectorF c1, c2, c;
	c1 = room.coord1;
	c2 = room.coord2;
	c = c1+(c2 - c1)/2;

	float w, h;
	w = c2.X - c1.X;
	h = c2.Y - c1.Y;
	w *= xs_map_types.selectroom_zoom;
	h *= xs_map_types.selectroom_zoom;

	c1.X = c.X - w/2;
	c1.Y = c.Y - h/2;

	c2.X = c.X + w/2;
	c2.Y = c.Y + h/2;

	room.coord1 = c1;
	room.coord2 = c2;

	//XSBackgroundArea a = *room.back_area;
	XSBackgroundArea a = *room.back_area;
	a.coord1 = c1;
	a.coord2 = c2;
	room.coord1 = c1;
	room.coord2 = c2;
	//room.back_area = &a;
	room.back_area = &a;

	xsgl_draw_backarea(a, build_pos);
	glColor3f(0, 0, 0);
	xsgl_draw_text_of_room(room, GLUT_BITMAP_9_BY_15, build_pos);
}

XSVectorF xsgl_moutuon_m;
XSVectorF xsgl_moution_dx;
XSVectorF xsgl_moution_d;
bool xsgl_moution;
bool xsgl_cam_changed;

bool xsgl_zoom_changed;
float xsgl_zoom_d;
XSVectorF xsgl_window_size;

XSRoom  xsgl_selected_xsroom;
bool  xsgl_selected_room;



XSBuild bbb;
XSVectorF mousem;

void xsgl_set_winsow_size_var(float w, float h)
{
	xsgl_window_size.X = w;
	xsgl_window_size.Y = h;
}

void xsgl_init_vars()
{
	xsgl_moutuon_m.X = INT32_MIN;
	xsgl_moutuon_m.Y = INT32_MIN;
	xsgl_level = XSGL_LEVEL_DEBUG;
	xsgl_moution_dx.X = 0;
	xsgl_moution_dx.Y = 0;
	xsgl_cam_changed = false;
	xsgl_zoom_changed = false;
	xsgl_selected_room = false;
	xsgl_moution = false;

	//glScalef(xs_map_types.map_scale, xs_map_types.map_scale, 1);

	mousem.X = 0;
	mousem.Y = 0;

	debug_mode = false;
	debug_show_name = true;
}

void xsgl_motion(int x, int y)
{
	if (!xsgl_moution) return;

	if (xsgl_moutuon_m.X == INT32_MIN)
		xsgl_moutuon_m.X = x;
	if (xsgl_moutuon_m.Y == INT32_MIN)
		xsgl_moutuon_m.Y = y;
	xsgl_moution_dx.X = -(xsgl_moutuon_m.X - x);// / xs_map_types.map_scale;
	xsgl_moution_dx.Y = (xsgl_moutuon_m.Y - y);// / xs_map_types.map_scale;
	xsgl_moutuon_m.X = x;
	xsgl_moutuon_m.Y = y;
	xsgl_moution_d.X += xsgl_moution_dx.X;// *xs_map_types.map_scale;
	xsgl_moution_d.Y += xsgl_moution_dx.Y;//*xs_map_types.map_scale;

	xsgl_cam_changed = true;
	//xsgl_moution = false;
	glutPostRedisplay();
}

void xsgl_mouse(int button, int state, int x, int y)
{
	if (button == 0 && state == GLUT_UP)
		xsgl_moution = false;

	if (button == 0 && state == GLUT_DOWN)
	{
		xsgl_moutuon_m.X = INT32_MIN;
		xsgl_moutuon_m.Y = INT32_MIN;
		xsgl_moution = true;
		//xsgl_selected_room = false;
	}
	if (button == 2 && state == GLUT_DOWN)
	{
		xsgl_selected_room = false;

		float xx = (float)x / xsgl_map_scale() - bbb.coord.X - xsgl_moution_d.X;
		float yy = (xsgl_window_size.Y - (float)y) / xsgl_map_scale() - bbb.coord.Y - xsgl_moution_d.Y;

		XSFloor f = bbb.floors[1];
		XSRoom r;
		for (map<int, XSRoom>::iterator i = f.rooms.begin(); i != f.rooms.end(); i++)
		{
			r = i->second;
			if (r.coord1.X<xx&&r.coord2.X>xx&&
				r.coord1.Y<yy&&r.coord2.Y>yy)
			{
				xsgl_selected_xsroom = r;
				xsgl_selected_room = true;
				break;
			}
		}
	}
}

void xsgl_zoom_event(unsigned char key, int x, int y)
{
	if (key == 114 || key == 234)
	{
		xs_map_types.read();
		xs_map.read();
		glutPostRedisplay();
		return;
	}

	if (key == 100 || key == 226)
	{
		debug_mode = !debug_mode;
		glutPostRedisplay();
		return;
	}

	if (key == 110 || key == 242)
	{
		debug_show_name = !debug_show_name;
		glutPostRedisplay();
		return;
	}

	int level = key - 48;
	if (bbb.floors.find(level) == bbb.floors.end())
		return;

	xsgl_level = level;
	float c = 1;
	if (key == 43)//+
	{
		c = xs_map_types.map_scale / (xs_map_types.map_scale - xs_map_types.zoom_step);
	}
	if (key == 45)//-
	{
		c = xs_map_types.map_scale / (xs_map_types.map_scale + xs_map_types.zoom_step);
	}
	if (key == 45 || key == 43)
	{
		xsgl_zoom_d = c;
		xsgl_zoom_changed = true;

		float nw = xsgl_window_size.X / (xsgl_map_scale() * c);
		float nh = xsgl_window_size.Y / (xsgl_map_scale() * c);
		float ow = xsgl_window_size.X / xsgl_map_scale();
		float oh = xsgl_window_size.Y / xsgl_map_scale();

		xs_map_types.map_scale *= c;

		//xsgl_moution_dx.X = (ow - nw) / xsgl_map_scale();
		//xsgl_moution_dx.Y = (oh - nh) / xsgl_map_scale();

		xsgl_moution_dx.X = (nw - ow) / 2;
		xsgl_moution_dx.Y = (nh - oh) / 2;
	}
	glutPostRedisplay();
}

void mouse_pass_moution(int x, int y)
{
	mousem.X = ((float)x - xsgl_moution_d.X) / xsgl_map_scale() - 4;// -bbb.coord.X;
	mousem.Y = (xsgl_window_size.Y - (float)y - xsgl_moution_d.Y) / xsgl_map_scale() - 4;// -bbb.coord.Y;
	glutPostRedisplay();
}

void drawBitmapTextMouse(float x, float y)
{
	string s = to_string(mousem.X).append(" : ").append(to_string(mousem.Y));
	const char *cc;
	cc = s.c_str();
	char* c;
	c = const_cast<char*>(cc);
	glRasterPos2f(x, y);

	for (c = c; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
}

void xsgl_draw_test()
{
	//gluLookAt(-20, -20, 1, -20, -20, 0, 0, 1, 0);
	//gluLookAt(xsgl_cam_x, xsgl_cam_y, 1, xsgl_cam_x, xsgl_cam_y, 0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	//glTranslatef(xsgl_cam_x, xsgl_cam_y, 0);
	if (xsgl_cam_changed)
	{
		glTranslatef(xsgl_moution_dx.X, xsgl_moution_dx.Y, 0);
		//glTranslatef(xsgl_mx, xsgl_my, 0);
		xsgl_cam_changed = false;
		//xsgl_mouse(0, 0, 0, 0);
	}
	//if (xsgl_zoom_changed)
	//{
	//	glScalef(xsgl_zoom_d, xsgl_zoom_d, 1);
	//	glTranslatef(xsgl_moution_dx.X, xsgl_moution_dx.Y, 0);
	//	//glTranslatef(0, 0, 0);
	//	xsgl_moution_d += xsgl_moution_dx;
	//	xsgl_zoom_changed = false;
	//}
	//for (map<int, XSBuild>::iterator i = xs_map.builds.begin(); i != xs_map.builds.end(); i++)
	//{
	XSBuild b = xs_map.builds[7];
	bbb = b;
	//bbb.coord.X = 0;
	//bbb.coord.Y = 0;
	XSVectorF tp = xsgl_window_size;
	tp.X = 10;
	tp.Y -= 15;
	tp -= xsgl_moution_d;
	//for (map<int, XSFloor>::iterator j = b.floors.begin(); j != b.floors.end(); j++)
	//{
	XSFloor f = b.floors[xsgl_level];
	xsgl_draw_backareas(f, b.coord);
	xsgl_draw_walls(f, b.coord);
	xsgl_draw_doors(f, b.coord);
	xsgl_draw_stairs(f, b.coord);
	xsgl_draw_text_of_rooms(f, b.coord);
	if (xsgl_selected_room)
		xsgl_draw_selected_room(xsgl_selected_xsroom, b.coord);

	string t = b.name;
	t = t.append(" : ").append(f.name).append(" [LEVEL ")
		.append(to_string(xsgl_level)).append("]");
	xsgl_draw_text(t, GLUT_BITMAP_9_BY_15, tp, tp);
	//	}
	//}
	glColor3f(0, 0, 0);
	drawBitmapTextMouse(-xsgl_moution_d.X + 10.0 / xsgl_map_scale(),
		-xsgl_moution_d.Y + 10.0 / xsgl_map_scale());
	glFlush();
	glutSwapBuffers();
}