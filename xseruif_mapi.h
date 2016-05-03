//  #####           #####      #################      ###############        ###############      #####         #####      ###############         ###############                 ######
//   #####         #####     #################      ###################    ###################    #####         #####    ###################     ###################                  ######
//    #####       #####      #####                  #####                  #####         #####    #####         #####           #####            #####                                   ######
//     #####     #####       #####                  #####                  #####         #####    #####         #####           #####            #####                                      ######
//      #####   #####        #####                  #####                  #####         #####    #####         #####           #####            #####
//       ##### #####         #####                  #####                  #####         #####    #####         #####           #####            #####                             ######   ######
//        #########          #################      ###################    ###################    #####         #####           #####            ###################            ######   ######
//        #########            #################    ###################    ###################    #####         #####           #####            #################           ######   ######
//       ##### #####                       #####    #####                  #####   ######         #####         #####           #####            #####                    ######   ######
//      #####   #####                      #####    #####                  #####    ######        #####         #####           #####            #####
//     #####     #####                     #####    #####                  #####     ######       #####         #####           #####            #####                    ######
//    #####       #####                    #####    #####                  #####      ######      #####         #####           #####            #####                       ######
//   #####         #####     ###################    ###################    #####       ######     ###################    ###################     #####                          ######
//  #####           #####      ###############        ###############      #####        ######      ###############        ###############       #####                             ######

//#include "core_map_obj/xs_map.h"
//#include "xsjsonhelper.h"
//#include "core_map_gl/xs_main_gl.h"
#include "xsmap_connecter.h"

void mapi_set_slash_type(bool slash_type) { xs_enable_replace_windows = slash_type; }

void mapi_set_map_folder(string f)
{
	char c = f.at(f.length() - 1);
	if (c == '/' || c == '\\')
		f = f.substr(0, f.length() - 1);
	xs_map_folder = f;
	mapi_set_slash_type(XS_SLASH_UNIX);
}

//void mapi_file_error_string(const exception& e) { xs_fileline_err_string(e.what()); }

void mapi_load_files()
{
	try
	{
		//xs_openfile("/Users/seruif/Documents/Xcode_Projects/NSTU_MAP_console/map/builds/7/floors.txt");
		//xs_closefile();
		xs_map_types.read();
		xs_map.read();
	}
	catch (const exception& e)
	{
		xs_closefile();
		throw runtime_error(xs_fileline_err_string(e.what()));
	}
}

//string mapi_json_map_types()
//{
//	XSJSONVALUE v = xs_to_json_value(xs_map_types);
//	string s = v.tostring();
//	return s;
//}
//
//string mapi_json_map()
//{
//	XSJSONVALUE v = xs_to_json_value(xs_map);
//	string s = v.tostring();
//	return s;
//}

void mapi_json_file(string folder)
{
	string json_all;

	json_all = xs_all_to_json_value().tostring();

	char c = folder.at(folder.length() - 1);
	if (c == '/' || c == '\\')
		folder = folder.substr(0, folder.length() - 1);

	ofstream fout;

	string fname = folder;
	if (xs_enable_replace_windows) fname = fname.append("\\");
	else fname = fname.append("/");
	fname = fname.append("nstu_map.js");

	fout.open(fname, fstream::out);
	if (fout.fail())
	{
		fout.close();
		string err = "Error open file: ";
		char* erch = new char[64];
#ifdef _WIN32
        strerror_s(erch, 64, errno);
#else
        strerror_r(errno,erch,64);
#endif	
        err = err.append(erch);
		err = err.append(". File=").append(fname);
		//err.append(f);
		throw runtime_error(err);
	}
	json_all = string("function get_map_data(){var data = ").append(json_all).append(";return data;}");
	fout << json_all;
	fout.flush();
	fout.close();
}