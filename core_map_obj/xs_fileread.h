#pragma once

#include "xs_exceptions.h"

#include <vector>
//#include <algorithm>
//#include <cctype>
//#include <stdio.h>
#include <fstream>

#include "xs_processing_string.h"
using namespace std;
#define XS_SLASH_UNIX false
#define XS_SLASH_WIN true
bool xs_enable_replace_windows;

// FILE VARS
ifstream xs_file;
string xs_filename;
string xs_map_folder;
string xs_fileline;
string xs_fileline_prm;
vector<string> xs_fileline_values;
int xs_next_read_index;//i am not want use queue here

string xs_build_path_to_file_fx(string folder, string file_name, int build, int floor)
{
    return floor>=0 ?
    folder.append(to_string(build)).append("/").append(to_string(floor)).append(file_name):
    folder.append(to_string(build)).append(file_name);
}

//string xs_build_path_to_build_file(string path, int build)
//{
//    string fmt = path;
//    char b;
//    unsigned required = std::snprintf(&b, 0, fmt.c_str(), build) + 1;
//    
//    char bytes[required];
//    std::snprintf(bytes, required, fmt.c_str(), build);
//    
//    string res = string(bytes);
//    return res;
//}
//
//string xs_build_path_to_floor_file(string path, int build, int floor)
//{
//    string fmt = path;
//    char b;
//    unsigned required = std::snprintf(&b, 0, fmt.c_str(), build, floor) + 1;
//    
//    char bytes[required];
//    std::snprintf(bytes, required, fmt.c_str(), build, floor);
//    
//    string res = string(bytes);
//    return res;
//}


string xs_fileline_err_string(string message)
{
	return message.append(" | File: ").append(xs_filename).append(" Line: ").append(xs_fileline_prm.append(" ").append(xs_fileline));
}

string xs_fileline_err_string(const char* message)
{
    return xs_fileline_err_string(string(message));
}

void xs_closefile()
{
    //xs_fileline_prm = "";
    //xs_fileline = "";
    //xs_filename = "";
    xs_file.close();
}

void xs_openfile(string fn)
{
    xs_fileline_prm = "";
    xs_fileline = "";
	string f = xs_map_folder;
    //fn = "/builds/7/floors.txt";
    f.append(fn);
    xs_filename = f;
	if (xs_enable_replace_windows)
		xs_filename = xs_replace_char(xs_filename, '/', '\\');
    //xs_filename = "/Users/seruif/Documents/Xcode_Projects/NSTU_MAP_console/map/builds/7/floors.txt";
    xs_file.open(xs_filename, ifstream::in);
	if (xs_file.fail())
	{
        xs_closefile();
		string err = "Error open file: ";
		char* erch = new char[64];
#ifdef _WIN32
		strerror_s(erch, 64, errno);
#elif __APPLE__
        strerror_r(errno,erch,64);
#else
        errno = string("[NOT ASSIGN COMPILER] Error code=").append(to_string(errno));
#endif
        err.append(erch);
        //err.append(f);
        throw runtime_error(err);
	}
}

string xs_file_readline()
{
	string s;
	if (!std::getline(xs_file, s))
        return "\0";
	if (s.length() == 0 || s.at(0) == '#' || s.compare("\r") == 0)
		return xs_file_readline();

	bool fspace = true;
	for (unsigned int i = 0; i < s.length() - 1; i++)
	{
		if (s.at(i) != ' ')
		{
			fspace = false;
			break;
		}
	}
	if (fspace) return xs_file_readline();
    
    //FIX EBANYI - REMOVE \r IN LINEEND!
    if (s[s.length()-1] == '\r')
        s = s.substr(0,s.length()-1);
    
	return s;
}

#pragma region READLINE
void xs_fileline_split(int n)
{
    xs_fileline_values = xs_split(xs_fileline, n);
    xs_next_read_index = 0;
}

void xs_fileline_split()
{
    xs_fileline_values = xs_split(xs_fileline);
    xs_next_read_index = 0;
}

bool xs_file_readline_split()
{
	xs_fileline = xs_file_readline();
	if (xs_fileline.empty()) return false;
    xs_fileline_prm = "";
    xs_fileline_split();
	return true;
}

bool xs_file_readline_split(int n)
{
    xs_fileline = xs_file_readline();
    if (xs_fileline.empty()) return false;
    xs_fileline_prm = "";
    xs_fileline_split(n);
    return true;
}

bool xs_file_readline_prm(int n)
{
    xs_fileline = xs_file_readline();
    if (xs_fileline.empty()) return false;
    vector<string> v = xs_split(xs_fileline, 1);
    xs_fileline_prm = v.at(0);
	if (v.size() > 1)
	{
		xs_fileline = v.at(1);
		if (--n > 0)
			xs_fileline_split(n);
		else
			xs_fileline_split();
	}
	else
		xs_fileline_values.clear();
    return true;
}

bool xs_file_readline_prm()
{
    return xs_file_readline_prm(0);
}

bool xs_fileline_prm_is(string prm)
{
    return xs_fileline_prm.compare(prm)==0;
}
#pragma endregion

//read next value
#pragma region READ NEXT VALUE
bool xs_fileline_next_value_exist()
{
	if (xs_next_read_index < xs_fileline_values.size()) return true;
	return false;
}

int xs_fileline_value_to_int(string s)
{
	try
	{
		return stoi(s);
	}
	catch (const std::exception&)
	{
        xs_closefile();
		throw runtime_error(string("Error convert to int. Value: ").append(s).c_str());
	}
}

float xs_fileline_value_to_float(string s)
{
	try
	{
		return stof(s);
	}
	catch (const std::exception&)
	{
        xs_closefile();
		throw runtime_error(string("Error convert to float. Value: ").append(s).c_str());
	}
}

unsigned short xs_fileline_value_to_us(string s)
{
	try
	{
		return stoul(s);
	}
	catch (const std::exception&)
	{
		xs_closefile();
		throw runtime_error(string("Error convert to unsigned short. Value: ").append(s).c_str());
	}
}

string xs_fileline_read_next_value_s()
{
	try
	{
        return xs_fileline_values.at(xs_next_read_index++);
	}
	catch (const std::exception&)
	{
        xs_closefile();
		throw runtime_error(string("Out of range. Unexpected end of values. ").c_str());
	}
}

int xs_fileline_read_next_value_i()
{
	return xs_fileline_value_to_int(xs_fileline_read_next_value_s());
}

float xs_fileline_read_next_value_f()
{
	return xs_fileline_value_to_float(xs_fileline_read_next_value_s());
}

unsigned short xs_fileline_read_next_value_us()
{
	return xs_fileline_value_to_us(xs_fileline_read_next_value_s());
}

string xs_fileline_read_next_value_s(string def_key, string def)
{
	string s = xs_fileline_read_next_value_s();
	if (s.compare(def_key)) return def;
	return s;
}

int  xs_fileline_read_next_value_i(string def_key, int def)
{
	string s = xs_fileline_read_next_value_s();
	if (s.compare(def_key) == 0) return def;
	return xs_fileline_value_to_int(s);
}

float xs_fileline_read_next_value_f(string def_key, float def)
{
	string s = xs_fileline_read_next_value_s();
	if (s.compare(def_key) == 0) return def;
	return xs_fileline_value_to_float(s);
}

unsigned short xs_fileline_read_next_value_us(string def_key, unsigned short def)
{
	string s = xs_fileline_read_next_value_s();
	if (s.compare(def_key) == 0) return def;
	return xs_fileline_value_to_us(s);
}

XSVectorF xs_fileline_read_next_value_vectorf(string def_key, float def_x, float def_y)
{
    XSVectorF f;
    f.X = def_x;
    f.Y = def_y;
    
    string s = xs_fileline_read_next_value_s();
    if (s.compare(def_key) != 0)
        f.X = xs_fileline_value_to_float(s);
    
    s = xs_fileline_read_next_value_s();
    if (s.compare(def_key) != 0)
        f.Y = xs_fileline_value_to_float(s);
    
    return f;
}

XSVectorF xs_fileline_read_next_value_vectorf()
{
    return xs_fileline_read_next_value_vectorf("\0", 0, 0);
}

XSVectorF xs_fileline_read_next_value_vectorf(XSVectorF offset)
{
	XSVectorF v = xs_fileline_read_next_value_vectorf();
	v += offset;
	return v;
}

XSVectorF xs_fileline_read_next_value_vectorf(string def_key, float def_x, float def_y, XSVectorF offset)
{
	XSVectorF v = xs_fileline_read_next_value_vectorf(def_key, def_x, def_y);
	v += offset;
	return v;
}

int xs_fileline_read_next_value_color()
{
	string as = xs_fileline_read_next_value_s();
    vector<string> avs = xs_split(as,':');
    
    if (avs.size()!=1 && avs.size()!=3)
        throw runtime_error("error color format");
    
    vector<int> avi;
    for (int i = 0; i<avs.size(); avi.push_back(stoi(avs[i++])));
    
    int a = avi[0];
    int r,g,b;
	if (avi.size() == 3)
	{
		r = a;
        g = avi[1];
        b = avi[2];
	}
    else
    {
        b = a % 256;
        a = a / 256;
        g = a % 256;
        r = a / 256;
    }
    a = r /2 * 128*128 + g/2 * 128 + b/2;
	return a;
}
//XSVectorI xs_fileline_read_next_value_vectori(string def_key, int def_x, int def_y)
//{
//    return xs_vectorf_to_i(xs_fileline_read_next_value_vectorf(def_key, def_x, def_y));
//}
//
//XSVectorI xs_fileline_read_next_value_vectori()
//{
//    return xs_vectorf_to_i(xs_fileline_read_next_value_vectorf("\0", 0, 0));
//}


#pragma endregion