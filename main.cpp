
//
//  main.cpp
//  NSTU_MAP_console
//
//  Created by Валерий Карачаков on 24.02.16.
//  Copyright © 2016 Валерий Карачаков. All rights reserved.
//
#ifdef _WIN32 
#include "core_map_gl\xs_main_gl.h"
#else
#include "xseruif_mapi.h"
#endif

#include <iostream>
//#include "xs_main_gl.h"

#define ww 1000
#define wh 650

int main(int argc, const char * argv[])
{
    if (argc != 3) cout<<"Parameters error (count != 2)"<<endl;
    string mapf = string(argv[1]);
    string jsf = string(argv[2]);
    
    #ifdef _WIN32
    try
    {
        //mapi_set_map_folder("D:\\The_Vault_of_the_Traveler\\Projects\\VS2015\\NSTUMAPGL\\map");
        mapi_set_map_folder(mapf);
        mapi_set_slash_type(XS_SLASH_WIN);
        mapi_load_files();
    }
    catch (const std::exception& e)
    {
        cout << e.what();
    }

    try
    {
        //mapi_json_file("D:\\The_Vault_of_the_Traveler\\Projects\\VS2015\\NSTUMAPGL\\map");
        mapi_json_file(jsf);
    }
    catch (const std::exception& e)
    {
        cout << e.what();
    }

    //Json::Value root;
    //root["test"] = 123;
    //string s = root.toStyledString();
    //xs_map_types.map_scale = 1;

    glutInit(&argc, const_cast<char**>(argv));
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(ww, wh);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("NSTUMAP");
    float* color = xsgl_Color3f(xs_map_types.map_background->color);
    glClearColor(color[0], color[1], color[2], 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, ww, 0, wh, -1.0, 1.0);
    //glutDisplayFunc(display);
    xsgl_set_winsow_size_var(ww, wh);
    xsgl_init_vars();

	//xsgl_debug_mode(false);

    glutDisplayFunc(xsgl_draw_test);
    glutMotionFunc(xsgl_motion);
    glutMouseFunc(xsgl_mouse);
    glutPassiveMotionFunc(mouse_pass_moution);
    //glutMouseWheelFunc(xsgl_mouse_wheel);
    glutKeyboardFunc(xsgl_zoom_event);
    glutMainLoop();

    return 0;
    #else
    //mapi_set_map_folder("/Users/Seruif/Documents/Xcode_Projects/NSTU_MAP_console/NSTU_MAP_console/xs_nstu_map/nstu_map");
    mapi_set_map_folder(mapf);
    string err;
    try
    {
        mapi_load_files();
        //mapi_json_file("/Users/Seruif/Documents/MyNSTU-PyServer/static/mynstu/js");
        mapi_json_file(jsf);
    }
    catch (const exception& e)
    {
        err = e.what();
        cout << err << endl;
        return -1;
    }
    cout<<"SUCCESS"<<endl;
    return 0;
    #endif
}
