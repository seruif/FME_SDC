#include <stdexcept>
#include <string>

using namespace std;

class xs_error_backgroind_id : runtime_error
{
public:
    xs_error_backgroind_id(int id) : runtime_error(string("Error find background id: ").append(to_string(id))){}
};

class xs_error_wall_id : runtime_error
{
public:
    xs_error_wall_id(int id) : runtime_error(string("Error find wall id: ").append(to_string(id))){}
};

class xs_error_door_id : runtime_error
{
public:
    xs_error_door_id(int id) : runtime_error(string("Error find door id: ").append(to_string(id))){}
};

class xs_error_invalid_key : runtime_error
{
public:
    xs_error_invalid_key(string key) : runtime_error(string("Invalid key: ").append(key)){}
};

class xs_error_invalid_prm : runtime_error
{
public:
    xs_error_invalid_prm(string prm) : runtime_error(string("Invalid parameter: ").append(prm)){}
};