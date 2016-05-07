#include "xs_vector.h"
#include "xs_fileread.h"
#include <map>

#include "xs_defines.h"

//background
#define XS_BACK_TYPE_PIC 1
#define XS_BACK_TYPE_COLOR 2
#define XS_BACK_RENDER_MOSAIC 1
#define XS_BACK_RENDER_STRETCH 2

struct XSLogoType
{
public:
	int id;
	XSVectorF size;
    float scale;
};

struct XSBackgroundType
{
public:
	int id;
	int type;
	float scale;
	unsigned int color;
    string legend_name;
};

struct XSWallType
{
public:
	int id;
	XSBackgroundType* background_type;
	int background_type_id;
	float width;
	bool strip;
	int strip_factor;
	unsigned short strip_pattern;
};

struct XSDoorType
{
public:
	int id;
	XSBackgroundType* background_type;
	int background_type_id;
	float width;
	float heigh;
};

struct XSMapTypes
{
    void read_defaults()
    {
        xs_openfile(XS_FILE_DEFAULTS);
        map<string,bool> r =
        {
            pair<string,bool>(XS_PRM_PIC_EXTENSTIONS,false),
            pair<string,bool>(XS_PRM_VERSION,false),
            pair<string,bool>(XS_PRM_DOOR,false),
            pair<string,bool>(XS_PRM_WALL,false),
            pair<string,bool>(XS_PRM_SCALE,false),
			pair<string,bool>(XS_PRM_ROOM_WALL,false),
			pair<string,bool>(XS_PRM_BACKGROUND,false),
			pair<string,bool>(XS_PRM_ZOOMSTEP,false),
			pair<string,bool>(XS_PRM_SELECTROOM,false)

		};
        while (xs_file_readline_prm())
        {
            xs_fileline_split();
            if (xs_fileline_prm_is(XS_PRM_PIC_EXTENSTIONS))
            {
                pic_exts = xs_fileline_values;
                r[XS_PRM_PIC_EXTENSTIONS]=true;
            }
            if (xs_fileline_prm_is(XS_PRM_VERSION))
            {
                version = xs_fileline_read_next_value_s();
                r[XS_PRM_VERSION]=true;
            }
            if (xs_fileline_prm_is(XS_PRM_DOOR))
            {
                door_def_size = xs_fileline_read_next_value_vectorf();
                r[XS_PRM_DOOR]=true;
            }
            if (xs_fileline_prm_is(XS_PRM_WALL))
            {
                wall_def_width = xs_fileline_read_next_value_f();
                r[XS_PRM_WALL]=true;
            }
            if (xs_fileline_prm_is(XS_PRM_SCALE))
            {
                map_scale = xs_fileline_read_next_value_f();
                r[XS_PRM_SCALE]=true;
            }
			if (xs_fileline_prm_is(XS_PRM_ROOM_WALL))
			{
				room_wall_id = xs_fileline_read_next_value_i();
				r[XS_PRM_ROOM_WALL] = true;
			}
			if (xs_fileline_prm_is(XS_PRM_ZOOMSTEP))
			{
				zoom_step = xs_fileline_read_next_value_f();
				r[XS_PRM_ZOOMSTEP] = true;
			}
			if (xs_fileline_prm_is(XS_PRM_BACKGROUND))
			{
				map_background_id = xs_fileline_read_next_value_i();
				try { map_background = &(backgrounds[map_background_id]); }
				catch (const exception&) { throw xs_error_backgroind_id(map_background_id); }
				r[XS_PRM_BACKGROUND] = true;
			}
			if (xs_fileline_prm_is(XS_PRM_SELECTROOM))
			{
				selectroom_zoom = xs_fileline_read_next_value_f();
				selectroom_color = xs_fileline_read_next_value_color();
				r[XS_PRM_SELECTROOM] = true;
			}
        }
        xs_closefile();
        xs_fileline_prm="";
        xs_fileline="";
        
        for (map<string,bool>::iterator i = r.begin(); i != r.end(); i++)
            if (!i->second) throw runtime_error(string("Parameter not found: ").append(i->first));
    }
    
    void read_doors()
    {
        xs_openfile(XS_FILE_DOORS);
        while (xs_file_readline_split())
        {
            XSDoorType dt;
            dt.id = xs_fileline_read_next_value_i();
            dt.background_type_id = xs_fileline_read_next_value_i();
            dt.width = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, door_def_size.X);
            dt.heigh = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, door_def_size.Y);
            
            try
            {
                dt.background_type = &(backgrounds.at(dt.background_type_id));
            }
            catch(const exception&)
            {
                throw xs_error_backgroind_id(dt.background_type_id);
            }
            
            doors.insert(pair<int,XSDoorType>(dt.id,dt));
        }
        xs_closefile();
    }
    
	void read_walls()
	{
		xs_openfile(XS_FILE_WALLS);
		while (xs_file_readline_split())
		{
			XSWallType wt;
			wt.id = xs_fileline_read_next_value_i();
			wt.background_type_id = xs_fileline_read_next_value_i();
			wt.width = xs_fileline_read_next_value_f(XS_PRM_DEFAULT, wall_def_width);

			try { wt.background_type = &(backgrounds.at(wt.background_type_id)); }
			catch (const exception&) { throw xs_error_backgroind_id(wt.background_type_id); }

			wt.strip = false;
			wt.strip_factor = -1;
			wt.strip_pattern = 0;
			if (xs_fileline_next_value_exist())
			{
				if (xs_fileline_read_next_value_s().compare(XS_PRM_STRIP) == 0)
				{
					wt.strip = true;
					wt.strip_factor = xs_fileline_read_next_value_f();
					wt.strip_pattern = xs_fileline_read_next_value_us();
				}
			}

			walls.insert(pair<int, XSWallType>(wt.id, wt));
		}
		xs_closefile();
	}
    
    void read_backgrounds()
    {
        xs_openfile(XS_FILE_BACKGROUNDS);
        while (xs_file_readline_split(3))
        {
            XSBackgroundType bt;
            bt.id = xs_fileline_read_next_value_i();
			bt.scale = 1;
			bt.color = 0;
            string p = xs_fileline_read_next_value_s();
            if (p.compare(XS_PRM_COLOR) == 0)
            {
                bt.type = XS_BACK_TYPE_COLOR;
				int a = xs_fileline_read_next_value_color();

                bt.color = a;
            }
            else if (p.compare(XS_PRM_PICTURE) == 0)
            {
                bt.type = XS_BACK_TYPE_COLOR;
                if (xs_fileline_next_value_exist())
                    bt.scale = xs_fileline_read_next_value_f();
            }
            else throw xs_error_invalid_key(p);
            bt.legend_name="[NO LEGEND NAME]";
            if (xs_fileline_next_value_exist())
                bt.legend_name = xs_fileline_read_next_value_s();
                
            backgrounds.insert(pair<int,XSBackgroundType>(bt.id,bt));
        }
        xs_closefile();
    }
    
    void read_logos()
    {
        xs_openfile(XS_FILE_LOGOS);
        while (xs_file_readline_split())
        {
            XSLogoType lt;
            lt.id = xs_fileline_read_next_value_i();
            lt.size = xs_fileline_read_next_value_vectorf();
            lt.scale = 1;
            if (xs_fileline_next_value_exist())
                lt.scale = xs_fileline_read_next_value_f();
            logos.insert(pair<int,XSLogoType>(lt.id,lt));
        }
        xs_closefile();
    }
public:
	//defaults
	string version;
	vector<string> pic_exts;
	XSVectorF door_def_size;
	float wall_def_width;
	float map_scale;
    int room_wall_id;
    XSWallType* room_wall;
	int map_background_id;
	XSBackgroundType* map_background;
	float zoom_step;
	float selectroom_zoom;
	int selectroom_color;

	//types
	map<int, XSBackgroundType> backgrounds;
	map<int, XSWallType> walls;
	map<int, XSLogoType> logos;
    map<int, XSDoorType> doors;
    
	void read()
	{
		pic_exts.clear();
		backgrounds.clear();
		walls.clear();
		logos.clear();
		doors.clear();
		room_wall = NULL;

        read_backgrounds();
		read_defaults();
		read_doors();
        read_walls();
        read_logos();
        
        try
        {
            room_wall = &(walls.at(room_wall_id));
        }
        catch(const exception& e)
        {
            throw runtime_error(string("Error find room_wall id: ").append(to_string((room_wall_id))));
        }
    }
};