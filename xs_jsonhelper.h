#include "xs_json.h"
#include "core_map_obj/xs_map.h"

void xs_to_json_value_add_xsvector(XSJSONVALUE* jv, string key, XSVectorF v)
{
	XSJSONVALUE jvv;
	jvv.init_as(XS_JSON_TYPE_DIC);
	jvv.add_value("x", v.X);
	jvv.add_value("y", v.Y);
	jv->add_value(key, jvv);
}

XSJSONVALUE xs_to_json_value(XSBackgroundType v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("color", v.color);
	jv.add_value("id", v.id);
	jv.add_value("scale", v.scale);
	jv.add_value("type", v.type);
    jv.add_value_string("legend_name", v.legend_name);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSDoorType v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("background_type_id", v.background_type_id);
	jv.add_value("heigh", v.heigh);
	jv.add_value("id", v.id);
	jv.add_value("width", v.width);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSLogoType v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("background_type_id", v.scale);
	xs_to_json_value_add_xsvector(&jv, "size", v.size);
	jv.add_value("id", v.id);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSWallType v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("background_type_id", v.background_type_id);
	jv.add_value("id", v.id);
	jv.add_value("strip", v.strip);
	jv.add_value("strip_factor", v.strip_factor);
	jv.add_value("strip_pattern", v.strip_pattern);
	jv.add_value("width", v.width);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSMapTypes v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("map_background_id", v.map_background_id);
	jv.add_value("map_scale", v.map_scale);
	jv.add_value("room_wall_id", v.room_wall_id);
	jv.add_value("selectroom_color", v.selectroom_color);
	jv.add_value("selectroom_zoom", v.selectroom_zoom);
	jv.add_value_string("version", v.version);
	jv.add_value("wall_def_width", v.wall_def_width);
	jv.add_value("zoom_step", v.zoom_step);

	XSJSONVALUE jvav;

	XSJSONVALUE* jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_DIC);
	for (map<int, XSBackgroundType>::iterator i = v.backgrounds.begin(); i != v.backgrounds.end(); i++)
	{
		XSBackgroundType* tp = &(i->second);
		jvav = xs_to_json_value(*tp);
		jva->add_value(to_string(tp->id), jvav);
	}
	jv.add_value("backgrounds", *jva);

	jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_DIC);
	for (map<int, XSDoorType>::iterator i = v.doors.begin(); i != v.doors.end(); i++)
	{
		XSDoorType* tp = &(i->second);
		jvav = xs_to_json_value(*tp);
		jva->add_value(to_string(tp->id), jvav);
	}
	jv.add_value("doors", *jva);

	jva = new XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_DIC);
	for (map<int, XSLogoType>::iterator i = v.logos.begin(); i != v.logos.end(); i++)
	{
		XSLogoType* tp = &(i->second);
		jvav = xs_to_json_value(*tp);
		jva->add_value(to_string(tp->id), jvav);
	}
	jv.add_value("logos", *jva);

	jva = new XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_DIC);
	for (map<int, XSWallType>::iterator i = v.walls.begin(); i != v.walls.end(); i++)
	{
		XSWallType* tp = &(i->second);
		jvav = xs_to_json_value(*tp);
		jva->add_value(to_string(tp->id), jvav);
	}
	jv.add_value("walls", *jva);

	return jv;
}

XSJSONVALUE xs_to_json_value(XSStair v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("wall_type_id", v.wall_type_id);
	jv.add_value_string("orientation", v.orientation);
	jv.add_value("full", v.full);
	xs_to_json_value_add_xsvector(&jv, "coord1", v.coord1);
	xs_to_json_value_add_xsvector(&jv, "coord2", v.coord2);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSWall v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("type_id", v.type_id);
	jv.add_value("width", v.width);
	xs_to_json_value_add_xsvector(&jv, "coord1", v.coord1);
	xs_to_json_value_add_xsvector(&jv, "coord2", v.coord2);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSDoor v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("type_id", v.type_id);
	jv.add_value("width", v.width);
	jv.add_value("heigh", v.heigh);
	jv.add_value("angle", v.angle);
	jv.add_value_string("orientation", v.orientation);
	xs_to_json_value_add_xsvector(&jv, "coord1", v.coord1);
	xs_to_json_value_add_xsvector(&jv, "coord2", v.coord2);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSBackgroundArea v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("type_id", v.type_id);
	jv.add_value("render_type", v.render_type);
	jv.add_value("scale", v.scale);
	jv.add_value("z", v.z);
	xs_to_json_value_add_xsvector(&jv, "coord1", v.coord1);
	xs_to_json_value_add_xsvector(&jv, "coord2", v.coord2);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSRoomLogo v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("type_id", v.type_id);
	jv.add_value("room_id", v.room_id);
	jv.add_value("scale", v.scale);
	xs_to_json_value_add_xsvector(&jv, "real_coord", v.real_coord);
	xs_to_json_value_add_xsvector(&jv, "room_coord", v.room_coord);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSRoom v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("id", v.id);
	jv.add_value_string("info", v.info);
	jv.add_value_string("name", v.name);
    jv.add_value_string("assign", v.assign);
    xs_to_json_value_add_xsvector(&jv, "coord1", v.coord1);
	xs_to_json_value_add_xsvector(&jv, "coord2", v.coord2);
	xs_to_json_value_add_xsvector(&jv, "real_text_coord", v.real_text_coord);
	xs_to_json_value_add_xsvector(&jv, "room_text_coord", v.room_text_coord);
	return jv;
}

XSJSONVALUE xs_to_json_value(XSFloor v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("build_id", v.build_id);
	jv.add_value("floor_id", v.floor_id);
	jv.add_value("level", v.level);
	jv.add_value_string("name", v.name);
	xs_to_json_value_add_xsvector(&jv, "floor_size", v.floor_size);

	XSJSONVALUE jvav;

	XSJSONVALUE* jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_ARRAY);
	for (vector<XSDoor>::iterator i = v.doors.begin(); i != v.doors.end(); i++)
	{
		XSDoor* tp = &(i.operator*());
		jvav = xs_to_json_value(*tp);
		jva->add_value(jvav);
	}
	jv.add_value("doors", *jva);

	jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_ARRAY);
	for (vector<XSRoomLogo>::iterator i = v.logos.begin(); i != v.logos.end(); i++)
	{
		XSRoomLogo* tp = &(i.operator*());
		jvav = xs_to_json_value(*tp);
		jva->add_value(jvav);
	}
	jv.add_value("logos", *jva);

	jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_ARRAY);
	for (vector<XSStair>::iterator i = v.stairs.begin(); i != v.stairs.end(); i++)
	{
		XSStair* tp = &(i.operator*());
		jvav = xs_to_json_value(*tp);
		jva->add_value(jvav);
	}
	jv.add_value("stairs", *jva);

	jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_ARRAY);
	for (vector<XSWall>::iterator i = v.walls.begin(); i != v.walls.end(); i++)
	{
		XSWall* tp = &(i.operator*());
		jvav = xs_to_json_value(*tp);
		jva->add_value(jvav);
	}
	jv.add_value("walls", *jva);

	jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_ARRAY);
	for (vector<XSBackgroundArea>::iterator i = v.backgrounds.begin(); i != v.backgrounds.end(); i++)
	{
		XSBackgroundArea* tp = &(i.operator*());
		jvav = xs_to_json_value(*tp);
		jva->add_value(jvav);
	}
	jv.add_value("backgrounds", *jva);

	jva = new  XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_ARRAY);
	for (int i = 0; i < v.Zs.size();i++)
	{
		jva->add_value(v.Zs[i]);
	}
	jv.add_value("Zs", *jva);

	jva = new XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_DIC);
	for (map<int, XSRoom>::iterator i = v.rooms.begin(); i != v.rooms.end(); i++)
	{
		XSRoom* tp = &(i->second);
		jvav = xs_to_json_value(*tp);
		jva->add_value(to_string(tp->id), jvav);
	}
	jv.add_value("rooms", *jva);

	return jv;
}

XSJSONVALUE xs_to_json_value(XSBuild v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);
	jv.add_value("id", v.id);
	jv.add_value_string("name", v.name);
	xs_to_json_value_add_xsvector(&jv, "coord", v.coord);
    xs_to_json_value_add_xsvector(&jv, "build_size", v.build_size);

	XSJSONVALUE jvav;

	XSJSONVALUE* jva = new XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_DIC);
	for (map<int, XSFloor>::iterator i = v.floors.begin(); i != v.floors.end(); i++)
	{
		XSFloor* tp = &(i->second);
		jvav = xs_to_json_value(*tp);
		jva->add_value(to_string(tp->floor_id), jvav);
	}
	jv.add_value("floors", *jva);

	return jv;
}

XSJSONVALUE xs_to_json_value(XSMap v)
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);

	XSJSONVALUE jvav;

	//XSJSONVALUE* jvaids = new XSJSONVALUE();
	//jvaids->init_as(XS_JSON_TYPE_ARRAY);

	XSJSONVALUE* jva = new XSJSONVALUE();
	jva->init_as(XS_JSON_TYPE_DIC);
	for (map<int, XSBuild>::iterator i = v.builds.begin(); i != v.builds.end(); i++)
	{
		XSBuild* tp = &(i->second);
		jvav = xs_to_json_value(*tp);
		jva->add_value(to_string(tp->id), jvav);

		//jvaids->add_value(tp->id);
	}
	jv.add_value("builds", *jva);
	//jv.add_value("bids", *jvaids);

	return jv;
}

XSJSONVALUE xs_all_to_json_value()
{
	XSJSONVALUE jv;
	jv.init_as(XS_JSON_TYPE_DIC);

	jv.add_value("map", xs_to_json_value(xs_map));
	jv.add_value("map_types", xs_to_json_value(xs_map_types));

	return jv;
}