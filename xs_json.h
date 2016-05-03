#include <vector>
#include <map>
#include <string> 
using namespace std;
#define XS_JSON_TYPE_VALUE 1
#define XS_JSON_TYPE_ARRAY 2
#define XS_JSON_TYPE_DIC 3

struct XSJSONVALUE
{
	//string key;
	int type;
	bool val_is_string;

	string val;
	map<string, XSJSONVALUE> dic;
	vector<XSJSONVALUE> arr;
public:
	void null()
	{
		dic.clear();
		arr.clear();
		val = "null";
		type = XS_JSON_TYPE_VALUE;
	}

	void set_value(string v)
	{
		if (type != XS_JSON_TYPE_VALUE)
			throw runtime_error(string("JSON: Require type is value. Value=").append(v)
				.append(" Type=").append(to_string(type)));
		val = v;
	}

	void set_value_string(string v)
	{
		set_value(v);
		val_is_string = true;
	}

	void add_value(XSJSONVALUE v)
	{
		if (type != XS_JSON_TYPE_ARRAY)
			throw runtime_error(string("JSON: Require type is array. Type=").append(to_string(type)));
		arr.push_back(v);
	}

	void add_value(string v)
	{
		XSJSONVALUE jv;
		jv.init_as(XS_JSON_TYPE_VALUE);
		jv = v;
		add_value(jv);
	}

	void add_value_string(string v)
	{
		XSJSONVALUE jv;
		jv.init_as_string_value();
		jv = v;
		add_value(jv);
	}

	void add_value(bool v) { v ? add_value("true") : add_value("false"); }
	void add_value(int v) { add_value(to_string(v)); }
	void add_value(float v) { add_value(to_string(v)); }
	void add_value(unsigned short v) { add_value(to_string(v)); }
	void add_value(unsigned int v) { add_value(to_string(v)); }

	void add_value(string k, XSJSONVALUE v)
	{
		if (type != XS_JSON_TYPE_DIC)
			throw runtime_error(string("JSON: Require type is value. Key=").append(k)
				.append(" Type=").append(to_string(type)));
		dic[k] = v;
	}

	void add_value(string k, string v)
	{
		XSJSONVALUE jv;
		jv.init_as(XS_JSON_TYPE_VALUE);
		jv = v;
		add_value(k, jv);
	}

	void add_value_string(string k, string v)
	{
		XSJSONVALUE jv;
		jv.init_as_string_value();
		jv = v;
		add_value(k, jv);
	}

	void add_value(string k, bool v) { v ? add_value(k, string("true")) : add_value(k, string("false")); }
	void add_value(string k, int v) { add_value(k, to_string(v)); }
	void add_value(string k, float v) { add_value(k, to_string(v)); }
	void add_value(string k, unsigned short v) { add_value(k, to_string((unsigned int)v)); }
	void add_value(string k, unsigned int v) { add_value(k, to_string(v)); }

	void operator =(string v) { set_value(v); }
	void operator =(bool v) { v ? set_value(string("true")) : set_value(string("false")); }
	void operator =(int v) { set_value(to_string(v)); }
	void operator =(float v) { set_value(to_string(v)); }
	void operator =(unsigned short v) { set_value(to_string((unsigned int)v)); }
	void operator =(unsigned int v) { set_value(to_string(v)); }

	XSJSONVALUE& operator [](string& k)
	{
		if (type != XS_JSON_TYPE_DIC)
			throw runtime_error("XSJSON: Type of value is not dic. operator [].");
		if (dic.find(k) == dic.end())
			throw runtime_error(string("XSJSON: Key not found: ").append(k));
		return dic[k];
	}

	void init_as(int type) { this->type = type; val_is_string = false; }
	void init_as_string_value() { type = XS_JSON_TYPE_VALUE; val_is_string = true; }

	string tostring()
	{
		//string s = string("\"").append(key).append("\":");
		string s = "";
		if (type == XS_JSON_TYPE_VALUE)
		{
			if (!val_is_string)
				s = s.append(val);
			else
				s = s.append("\"").append(val).append("\"");
			return s;
		}
		if (type == XS_JSON_TYPE_ARRAY)
		{
			s = s.append("[");
			for (int i = 0; i < arr.size(); i++)
			{
				s = s.append(arr[i].tostring());
				s = s.append(",");
			}
			if (s.length() > 1)
				s = s.substr(0, s.length() - 1);
			s = s.append("]");
		}
		if (type == XS_JSON_TYPE_DIC)
		{
			s = s.append("{");
			for (map<string, XSJSONVALUE>::iterator i = dic.begin(); i != dic.end(); i++)
			{
				s = s.append("\"");
				s = s.append(i->first);
				s = s.append("\"");
				s = s.append(":");
				s = s.append(i->second.tostring());
				s = s.append(",");
			}
			if (s.length() > 1)
				s = s.substr(0, s.length() - 1);
			s.append("}");
		}
		return s;
	}
};