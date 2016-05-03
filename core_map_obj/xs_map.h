#include "xs_map_types.h"

//door
#define XS_DOOR_ORIENTAION_VERTICAL 1
#define XS_DOOR_ORIENTAION_HORIZONTAL 2
#define XS_DOOR_ORIENTAION_ANGLE 3

struct XSMap;
struct XSBuild;
struct XSRoom;
struct XSFloor;
struct XSWall;
struct XSDoor;
struct XSBackgroundArea;

XSMapTypes xs_map_types;

struct XSStair
{
	XSWallType* wall_type;
	int wall_type_id;
	XSVectorF coord1;
	XSVectorF coord2;
	string orientation;
	bool full;
};

struct XSWall
{
public:
	int type_id;
	XSWallType* type;
	XSVectorF coord1;
	XSVectorF coord2;
	float width;
};

struct XSDoor
{
public:
	int type_id;
	XSDoorType* type;
	XSVectorF coord1;
	XSVectorF coord2;

	float width;
	float heigh;

	string orientation;
	float angle;
};

struct XSRoomLogo
{
	int type_id;
	XSLogoType* type;
	int room_id;
	XSVectorF room_coord;
	XSVectorF real_coord;
	float scale;
};

struct XSBackgroundArea
{
public:
	int type_id;
	XSBackgroundType* type;
	XSVectorF coord1;
	XSVectorF coord2;
	int render_type;
	int z;
	float scale;
};

struct XSRoom
{
public:
	int id;
	string name;
	string info;
	XSVectorF coord1;
	XSVectorF coord2;
	//unsigned int back_id;
	XSBackgroundArea* back_area;
	XSVectorF room_text_coord;
	XSVectorF real_text_coord;
};

struct XSBuild;

struct XSFloor
{
	XSWall add_wall(XSWallType* tp, XSVectorF coord1, XSVectorF coord2, float width)
	{
		XSWall w;
		w.type_id = tp->id;
		w.type = tp;
		w.width = width;
		w.coord1 = coord1;
		w.coord2 = coord2;
		walls.push_back(w);
		return w;
	}
	void add_room_4walls(XSVectorF coord1, XSVectorF coord2, XSWallType* tp)
	{
		add_wall(tp, coord1, coord1.set_x(coord2.X), xs_map_types.room_wall->width);
		add_wall(tp, coord1, coord1.set_y(coord2.Y), xs_map_types.room_wall->width);
		add_wall(tp, coord1.set_x(coord2.X), coord2, xs_map_types.room_wall->width);
		add_wall(tp, coord1.set_y(coord2.Y), coord2, xs_map_types.room_wall->width);
	}
	void optimization_wall()
	{
		//LATER
	}

	XSDoorType* get_door_type(int id)
	{
		XSDoorType* dt;
		try { dt = &xs_map_types.doors[id]; }
		catch (const exception&) { throw xs_error_wall_id(id); }
		return dt;
	}

	XSDoor* after_door(XSDoor* d, string ori, float angle, float def_width_wall)
	{
		//d->type_id = xs_fileline_read_next_value_i();
		d->angle = -1;
		d->type = get_door_type(d->type_id);
		if (def_width_wall == -1)
			def_width_wall = d->type->heigh;
		d->width = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, d->type->width);
		d->heigh = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, def_width_wall);

		if (ori.compare("\0") == 0)
			ori = xs_fileline_read_next_value_s();

		if (ori.compare(XS_PRM_HORIZONTAL) == 0)
		{
			d->coord1.X -= d->width / 2;
			d->coord1.Y -= d->heigh / 2;
			d->coord2 = d->coord1.offset_x(d->width);
			d->coord2.Y += d->heigh;
		}
		else if (ori.compare(XS_PRM_VERTICAL) == 0)
		{
			d->coord1.Y -= d->width / 2;
			d->coord1.X -= d->heigh / 2;
			d->coord2 = d->coord1.offset_y(d->width);
			d->coord2.X += d->heigh;
		}
		else if (ori.compare(XS_PRM_ANGLE) == 0)
			throw runtime_error("Door angle not support yet.");
		else
			throw xs_error_invalid_key(ori);

		d->orientation = ori;
		doors.push_back(*d);
		return d;
	}

	void read_door_after_wall(XSWall w, string ori, float angle)
	{
		string tpr = xs_fileline_read_next_value_s();
		if (tpr.compare(XS_PRM_DOORNEXT) == 0)
		{
			vector<float> poss;
			while (xs_fileline_next_value_exist())
				poss.push_back(xs_fileline_read_next_value_f());

			for (int i = 0; i < poss.size(); i++)
			{
				if (!xs_file_readline_prm())
					throw runtime_error("Unexpected end. Expected doornext");
				if (!xs_fileline_prm_is(XS_PRM_DOORNEXT))
					throw xs_error_invalid_prm(xs_fileline_prm);
				XSDoor d;
				d.type_id = xs_fileline_read_next_value_i();
				d.type = get_door_type(d.type_id);
				d.coord1 = w.coord1;
				d.coord2 = w.coord1;

				//XSVectorF c;
				//c.X = w.coord1.X + poss[i];
				//c.Y = w.coord1.Y + poss[i];
				if (ori.compare(XS_PRM_HORIZONTAL) == 0)
				{
					//d.coord1 = c.offset_x(-d.width / 2);
					d.coord1.X += poss[i];
					d.coord2.X += poss[i];
				}
				else if (ori.compare(XS_PRM_VERTICAL) == 0)
				{
					//d.coord1 = c.offset_y(-d.width / 2);
					d.coord1.Y += poss[i];
					d.coord2.Y += poss[i];
				}
				else if (ori.compare(XS_PRM_ANGLE) == 0)
					throw runtime_error("Wall angle not support yet.");
				else
					throw xs_error_invalid_key(ori);

				after_door(&d, ori, angle, w.width);
			}
		}
	}

	void read_plan()
	{
		xs_openfile(xs_build_path_to_file_fx(XS_FOLDER_BUILDS, XS_FILE_PLAN, build_id, floor_id));
		while (xs_file_readline_prm())
		{
			int id = xs_fileline_read_next_value_i();
			if (xs_fileline_prm_is(XS_PRM_ROOMLIST))
			{
				XSVectorF coord1 = xs_fileline_read_next_value_vectorf();
				float len = xs_fileline_read_next_value_f();
				string ori = xs_fileline_read_next_value_s();
				//int c = xs_fileline_read_next_value_i();

				XSVectorF coord2;
				if (ori.compare(XS_PRM_HORIZONTAL) == 0)
					coord2 = coord1.offset_y(len);
				else if (ori.compare(XS_PRM_VERTICAL) == 0)
					coord2 = coord1.offset_x(len);
				else
					throw xs_error_invalid_key(ori);

				float width;
				XSRoom r;
				while (xs_file_readline_prm())
				{
					if (xs_fileline_prm_is(XS_PRM_ROOMNEXT))
					{
						id = xs_fileline_read_next_value_i();
						width = xs_fileline_read_next_value_f();

						if (ori.compare(XS_PRM_HORIZONTAL) == 0)
							coord2.X += width;
						else if (ori.compare(XS_PRM_VERTICAL) == 0)
							coord2.Y += width;

						r.id = id;
						r.coord1 = coord1;
						r.coord2 = coord2;
						r.name = "";
						r.room_text_coord.X = 0.5;
						r.room_text_coord.Y = 0.5;
						r.back_area = NULL;

						if (xs_fileline_next_value_exist())
						{
							string tpr = xs_fileline_read_next_value_s();
							if (tpr.compare(XS_PRM_FIXTEXT) == 0)
								r.room_text_coord = xs_fileline_read_next_value_vectorf(XS_PRM_DEFAULT, 0.5, 0.5);
							else
								throw xs_error_invalid_key(tpr);
						}

						rooms.insert(pair<int, XSRoom>(r.id, r));

						if (ori.compare(XS_PRM_HORIZONTAL) == 0)
							coord1.X += width;
						else if (ori.compare(XS_PRM_VERTICAL) == 0)
							coord1.Y += width;
					}
					else if (xs_fileline_prm_is(XS_PRM_SKIP))
					{
						width = xs_fileline_read_next_value_f();
						if (ori.compare(XS_PRM_HORIZONTAL) == 0)
						{
							coord2.X += width;
							coord1.X += width;
						}
						else if (ori.compare(XS_PRM_VERTICAL) == 0)
						{
							coord2.Y += width;
							coord1.Y += width;
						}
					}
					else if (xs_fileline_prm_is(XS_PRM_ENDNEXT))
					{
						break;
					}
					else
						throw xs_error_invalid_prm(xs_fileline_prm);
				}
			}
			else if (xs_fileline_prm_is(XS_PRM_ROOM))
			{
				string tpr = xs_fileline_read_next_value_s();
				XSVectorF coord1 = xs_fileline_read_next_value_vectorf();
				XSVectorF coord2 = xs_fileline_read_next_value_vectorf();

				if (tpr.compare(XS_PRM_RECTANGLE) == 0)
				{
					XSWallType* wt = NULL;
					int wid = xs_fileline_read_next_value_i(XS_PRM_DEFAULT, xs_map_types.room_wall_id);

					try { wt = &xs_map_types.walls[wid]; }
					catch (const exception&) { throw xs_error_wall_id(wid); }

					add_room_4walls(coord1, coord2, wt);
				}
				else if (tpr.compare(XS_PRM_LAREA) == 0)
					coord2 = coord1 + coord2;
				else if (tpr.compare(XS_PRM_AREA) != 0)
					throw xs_error_invalid_key(tpr);

				XSRoom r;
				//r.back_area = NULL;
				r.name = "";
				r.id = id;
				r.coord1 = coord1;
				r.coord2 = coord2;
				r.room_text_coord.set(0.5f, 0.5f);

				if (xs_fileline_next_value_exist())
				{
					tpr = xs_fileline_read_next_value_s();
					if (tpr.compare(XS_PRM_FIXTEXT) == 0)
						r.room_text_coord = xs_fileline_read_next_value_vectorf(XS_PRM_DEFAULT, 0.5, 0.5);
					else
						throw xs_error_invalid_key(tpr);
				}

				rooms.insert(pair<int, XSRoom>(id, r));
			}
			else if (xs_fileline_prm_is(XS_PRM_PWALL) ||
				xs_fileline_prm_is(XS_PRM_CWALL) ||
				xs_fileline_prm_is(XS_PRM_LWALL) ||
				xs_fileline_prm_is(XS_PRM_WALLLIST) ||
				xs_fileline_prm_is(XS_PRM_PLWALL))
			{
				XSWallType* wt;
				try { wt = &xs_map_types.walls[id]; }
				catch (const exception&) { throw xs_error_wall_id(id); }

				float wall_width = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, wt->width);

				if (xs_fileline_prm_is(XS_PRM_CWALL))
				{
					XSVectorF coord1 = xs_fileline_read_next_value_vectorf();
					XSVectorF coord2 = xs_fileline_read_next_value_vectorf();

					//read_door_after_wall(add_wall(wt, coord1, coord2, width));
					add_wall(wt, coord1, coord2, wall_width);
				}
				else if (xs_fileline_prm_is(XS_PRM_LWALL))
				{
					XSVectorF coord1 = xs_fileline_read_next_value_vectorf();
					string ori = xs_fileline_read_next_value_s();
					float length = xs_fileline_read_next_value_f();

					XSVectorF coord2;
					if (ori.compare(XS_PRM_HORIZONTAL) == 0)
					{
						coord2 = coord1.offset_x(length);
					}
					else if (ori.compare(XS_PRM_VERTICAL) == 0)
					{
						coord2 = coord1.offset_y(length);
					}
					else if (ori.compare(XS_PRM_ANGLE) == 0)
						throw runtime_error("Wall angle not support yet.");
					else
						throw xs_error_invalid_key(ori);

					if (xs_fileline_next_value_exist())
						read_door_after_wall(add_wall(wt, coord1, coord2, wall_width), ori, 0);
					else
						add_wall(wt, coord1, coord2, wall_width);
				}
				else if (xs_fileline_prm_is(XS_PRM_PWALL))
				{
					XSVectorF c1, c2;
					c1 = xs_fileline_read_next_value_vectorf();
					while (xs_fileline_next_value_exist())
					{
						c2 = xs_fileline_read_next_value_vectorf();
						add_wall(wt, c1, c2, wall_width);
						c1 = c2;
					}
				}
				else if (xs_fileline_prm_is(XS_PRM_PLWALL))
				{
					XSVectorF c1, c2;
					string ori;
					float len;
					c1 = xs_fileline_read_next_value_vectorf();
					while (xs_fileline_next_value_exist())
					{
						ori = xs_fileline_read_next_value_s();
						len = xs_fileline_read_next_value_f();

						if (ori.compare(XS_PRM_HORIZONTAL) == 0)
						{
							c2 = c1.offset_x(len);
						}
						else if (ori.compare(XS_PRM_VERTICAL) == 0)
						{
							c2 = c1.offset_y(len);
						}
						else if (ori.compare(XS_PRM_ANGLE) == 0)
							throw runtime_error("Wall angle not support yet.");
						else
							throw xs_error_invalid_key(ori);

						add_wall(wt, c1, c2, wall_width);
						c1 = c2;
					}
				}
				else if (xs_fileline_prm_is(XS_PRM_WALLLIST))
				{
					XSVectorF c1, c2;
					string ori;
					float off, len;
					c1 = xs_fileline_read_next_value_vectorf();
					ori = xs_fileline_read_next_value_s();
					len = xs_fileline_read_next_value_f();
					while (xs_fileline_next_value_exist())
					{
						off = xs_fileline_read_next_value_f();

						if (ori.compare(XS_PRM_HORIZONTAL) == 0)
						{
							c1 = c1.offset_y(off);
							c2 = c1.offset_x(len);
						}
						else if (ori.compare(XS_PRM_VERTICAL) == 0)
						{
							c1 = c1.offset_x(off);
							c2 = c1.offset_y(len);
						}
						else if (ori.compare(XS_PRM_ANGLE) == 0)
							throw runtime_error("Wall angle not support yet.");
						else
							throw xs_error_invalid_key(ori);

						add_wall(wt, c1, c2, wall_width);
						//c1 = c2;
					}
				}
			}
			else if (xs_fileline_prm_is(XS_PRM_DOOR))
			{
				XSDoor d;
				d.type_id = id;
				d.coord1 = xs_fileline_read_next_value_vectorf();
				string ori = xs_fileline_read_next_value_s();

				after_door(&d, ori, 0, -1);

				doors.push_back(d);
			}
			else if (xs_fileline_prm_is(XS_PRM_STAIR))
			{
				XSWallType* wt;
				try { wt = &xs_map_types.walls[id]; }
				catch (const exception&) { throw xs_error_wall_id(id); }
				XSVectorF coord1 = xs_fileline_read_next_value_vectorf();
				XSVectorF coord2 = xs_fileline_read_next_value_vectorf();
				string ori = xs_fileline_read_next_value_s();

				if (ori.compare(XS_PRM_VERTICAL)!=0&& ori.compare(XS_PRM_HORIZONTAL)!=0)
					throw xs_error_invalid_key(ori);

				XSStair s;
				s.wall_type = wt;
				s.wall_type_id = wt->id;
				s.coord1 = coord1;
				s.coord2 = coord2;
				s.orientation = ori;

				s.full = false;

				if (xs_fileline_next_value_exist())
				{
					string full = xs_fileline_read_next_value_s();
					if (full.compare(XS_PRM_FULL) == 0)
						s.full = true;
					else
						throw xs_error_invalid_key(full);
				}

				stairs.push_back(s);
			}
			else
				throw xs_error_invalid_prm(xs_fileline_prm);
		}
		xs_closefile();
		optimization_wall();
	}

	void after_read_back(XSBackgroundArea* a)
	{
		XSBackgroundType* bt;

		try { bt = &xs_map_types.backgrounds[a->type_id]; }
		catch (const exception&) { throw xs_error_backgroind_id(a->type_id); }

		a->type = bt;

		if (xs_fileline_next_value_exist())
		{
			string rt = xs_fileline_read_next_value_s();
			if (rt.compare(XS_PRM_MOSAIC) == 0)
				a->render_type = XS_BACK_RENDER_MOSAIC;
			else if (rt.compare(XS_PRM_STRETCH) == 0)
				a->render_type = XS_BACK_RENDER_STRETCH;
			else
				throw xs_error_invalid_key(rt);

			//a->z = xs_fileline_read_next_value_i();;
			a->scale = 1;

			if (xs_fileline_next_value_exist())
				a->scale = xs_fileline_read_next_value_f();
		}
	}

	void read_backs()
	{
		xs_openfile(xs_build_path_to_file_fx(XS_FOLDER_BUILDS, XS_FILE_BACKS, build_id, floor_id));
		int bid;
		Zs.push_back(0);
		while (xs_file_readline_prm())
		{
			bid = xs_fileline_read_next_value_i();

			XSBackgroundArea* a = new XSBackgroundArea[1];
			a->type_id = bid;
			a->z = 0;
			a->scale = 1;
			a->render_type = -1;

			if (xs_fileline_prm_is(XS_PRM_ROOM))
			{
				int rid = xs_fileline_read_next_value_i();

				XSRoom* r = &(rooms[rid]);

				a->coord1 = r->coord1;
				a->coord2 = r->coord2;

				//try { a.background = &xs_map_types.backgrounds[bid]; }
				//catch (const exception&) { throw xs_error_backgroind_id(bid); }

				r->back_area = a;
			}
			else if (xs_fileline_prm_is(XS_PRM_AREA) || xs_fileline_prm_is(XS_PRM_LAREA))
			{
				a->coord1 = xs_fileline_read_next_value_vectorf();
				a->coord2 = xs_fileline_read_next_value_vectorf();

				if (xs_fileline_prm_is(XS_PRM_LAREA))
					a->coord2 = a->coord1 + a->coord2;
			}
			else throw xs_error_invalid_prm(xs_fileline_prm);

			if (xs_fileline_next_value_exist())
			{
				int z = xs_fileline_read_next_value_i();
				a->z = z;
				bool zf = false;
				int ip = 0;
				for (int i = 0; i < Zs.size(); i++)
				{
					if (z == Zs[i]) { zf = true; break; }
					if (z < Zs[i]) ip = i;
				}
				if (!zf)
					Zs.insert(Zs.begin() + ip, z);
			}

			after_read_back(a);
			backgrounds.push_back(*a);
		}
		xs_closefile();
	}

	void read_rooms()
	{
		xs_openfile(xs_build_path_to_file_fx(XS_FOLDER_BUILDS, XS_FILE_ROOMS, build_id, floor_id));
		int rid;
		while (xs_file_readline_prm(2))
		{
			rid = xs_fileline_read_next_value_i();
			if (xs_fileline_prm_is(XS_PRM_ROOM)|| xs_fileline_prm_is(XS_PRM_INFO))
			{

				string name = xs_fileline_read_next_value_s();
				rooms[rid].info = "N/A";
				if (xs_fileline_prm_is(XS_PRM_ROOM))
					rooms[rid].name = name;
				else
					rooms[rid].info = name;
			}
			//else if (xs_fileline_prm_is(XS_PRM_LOGO))
			//{
			//	XSRoomLogo l;

			//	l.type_id = xs_fileline_read_next_value_i();
			//	float x = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, 0.5f);
			//	float y = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, 0.5f);

			//	l.coord.X = x;
			//	l.coord.Y = y;

			//	l.scale = 1;
			//	if (xs_fileline_next_value_exist())
			//		l.scale = xs_fileline_read_next_value_f();

			//	logos.push_back(l);
			//}
			else throw xs_error_invalid_prm(xs_fileline_prm);
		}
		xs_closefile();
	}

	void aftercalc()
	{
		XSVectorF build_pos = build_coord;
		for (vector<XSBackgroundArea>::iterator i = backgrounds.begin(); i != backgrounds.end(); i++)
		{
			i->coord1 += build_pos;
			i->coord2 += build_pos;

			i->coord1 *= map_scale;
			i->coord2 *= map_scale;
		}
		XSVectorF floor_size_c1;
		XSVectorF floor_size_c2;
		floor_size_c1.set(INT32_MAX, INT32_MAX);
		floor_size_c2.set(INT32_MIN, INT32_MIN);
		for (vector<XSWall>::iterator i = walls.begin(); i != walls.end(); i++)
		{
			i->coord1 += build_pos;
			i->coord2 += build_pos;

			i->coord1 *= map_scale;
			i->coord2 *= map_scale;

			if (i->coord1 < floor_size_c1)
				floor_size_c1 = i->coord1;

			if (i->coord1 > floor_size_c2)
				floor_size_c2 = i->coord1;

			i->width *= map_scale;
		}
		floor_size = floor_size_c2 - floor_size_c1;

		for (vector<XSDoor>::iterator i = doors.begin(); i != doors.end(); i++)
		{
			i->coord1 += build_pos;
			i->coord2 += build_pos;

			i->coord1 *= map_scale;
			i->coord2 *= map_scale;
			i->heigh *= map_scale;
			i->width *= map_scale;
		}
		for (vector<XSStair>::iterator i = stairs.begin(); i != stairs.end(); i++)
		{
			i->coord1 += build_pos;
			i->coord2 += build_pos;

			i->coord1 *= map_scale;
			i->coord2 *= map_scale;
		}
		for (map<int,XSRoom>::iterator i = rooms.begin(); i != rooms.end(); i++)
		{
			XSRoom* r = &(i->second);
			r->coord1 += build_pos;
			r->coord2 += build_pos;

			r->coord1 *= map_scale;
			r->coord2 *= map_scale;

			r->real_text_coord = r->room_text_coord.convert_to_real_coord(r->coord1, r->coord2);
		}
		for (vector<XSRoomLogo>::iterator i = logos.begin(); i != logos.end(); i++)
		{
			XSRoom* r = &rooms[i->room_id];
			i->real_coord = i->room_coord.convert_to_real_coord(r->coord1,r->coord2);
		}
	}

	XSVectorF build_coord;
	float map_scale;
public:
	vector<int> Zs;
	XSVectorF floor_size;

	int build_id;
	//XSBuild* build;
	int floor_id;//num of floor = id
	int level;
	string name;

	vector<XSBackgroundArea> backgrounds;
	vector<XSWall> walls;
	map<int, XSRoom> rooms;
	vector<XSDoor> doors;
	vector<XSRoomLogo> logos;
	vector<XSStair> stairs;

	void init(int build_id, XSVectorF build_coord, int floor_id, int level, string name)
	{
		this->build_id = build_id;
		this->floor_id = floor_id;
		this->level = level;
		this->name = name;

		this->map_scale = xs_map_types.map_scale;
		this->build_coord = build_coord;
		//this->build = build;
	}

	void read()
	{
		read_plan();
		read_rooms();
		read_backs();
		aftercalc();
	}
};

struct XSBuild
{
public:
	int id;
	string name;
	map<int, XSFloor> floors;
	XSVectorF coord;

	void init(int bid, string name, XSVectorF coord)
	{
		this->id = bid;
		this->name = name;
		this->coord = coord;
	}

	void read()
	{
		xs_openfile(xs_build_path_to_file_fx(XS_FOLDER_BUILDS, XS_FILE_FLOORS, id, -1));
		//xs_openfile(XS_FILE_FLOORS);
		int fid, level;
		string name;

		vector<XSFloor> v;
		while (xs_file_readline_split(2))
		{
			fid = xs_fileline_read_next_value_i();
			level = xs_fileline_read_next_value_i();
			name = xs_fileline_read_next_value_s();

			XSFloor f;
			f.init(id, coord, fid, level, name);
			v.push_back(f);
		}
		xs_closefile();

		for (int i = 0; i < v.size(); i++)
		{
			XSFloor f = v[i];
			f.read();
			floors.insert(pair<int, XSFloor>(f.level, f));
		}
	}
};

struct XSMap
{
public:
	map<int, XSBuild> builds;

	vector<XSFloor> get_floors_level(int level)
	{
		vector<XSFloor> f;
		XSBuild* b;
		for (map<int,XSBuild>::iterator i = builds.begin(); i !=builds.end(); i++)
		{
			b = &(i->second);
			map<int,XSFloor>::iterator fi = b->floors.find(level);
			if (fi != b->floors.end())
				f.push_back(fi->second);
		}
        return f;
	}

	void read()
	{
		xs_openfile(XS_FILE_BUILDS);
		while (xs_file_readline_split(3))
		{
			int id = xs_fileline_read_next_value_i();
			XSVectorF coord = xs_fileline_read_next_value_vectorf();
			string name = xs_fileline_read_next_value_s();

			XSBuild b;
			b.init(id, name, coord);
			//b.read();

			builds.insert(pair<int, XSBuild>(id, b));
		}
		xs_closefile();
		for (map<int, XSBuild>::iterator i = builds.begin(); i != builds.end(); i++)
			i->second.read();
	}
};

XSMap xs_map;