#include <string>
using namespace std;

//WARNING!! N PARAMETER IN SPLIT FUNC BEGIN FROM ZERO!!

string xs_replace_char(string str, char x, char y)
{
	size_t index = 0;
	while (true) {
		index = str.find(x, index);
		if (index == std::string::npos) break;
		str.replace(index, 1, &y);
		index ++;
	}
	return str;
}

string xs_trim(const string& str)
{
    bool l=true,r=true;
    string s = str;
    while (s.length()>1 && (l || r))
    {
        l=false; r=false;
        if (s[0]==' ')
        {
            s=s.substr(1);
            l=true;
        }
        if (s[s.length()-1]==' ')
        {
            s=s.substr(0,s.length()-1);
            r=true;
        }
    }
    if (s==" ") return "\0";
    else return s;
}

vector<string> xs_split(const string& str, int n, char sep)
{
	unsigned long p;//int
	vector<string> ret;
	int nn = 0;
	//n--;
    string sub;
    string s = xs_trim(str);
	while (true)
	{
		p = s.find_first_of(sep);
		if (p == -1 || n == nn)
		{
            s = xs_trim(s);
            if (!s.empty()) ret.push_back(s);
			break;
		}
        sub =s.substr(0, p);
        if (sub=="")
        {
            s = s.substr(p+1);
            continue;
        }
		ret.push_back(sub);
		s = s.substr(p + 1, s.length() - p - 1);
		nn++;
	}
	return ret;
}

vector<string> xs_split(const string& s, int n)
{
    return xs_split(s, n, ' ');
}

vector<string> xs_split(const string& s)
{
	return xs_split(s, INT32_MAX, ' ');
}

vector<string> xs_split(const string& s, char sep)
{
    return xs_split(s, INT32_MAX, sep);
}
//next functions is stolen
//inline bool __xs_space(char c){return isspace(c);}
//
//inline bool __xs_notspace(char c){return !isspace(c);}
//
//vector<string> xs_split(const string& s)
//{
//	typedef std::string::const_iterator iter;
//	std::vector<std::string> ret;
//	iter i = s.begin();
//	while (i != s.end())
//	{
//		i = std::find_if(i, s.end(), __xs_notspace); // find the beginning of a word
//		iter j = std::find_if(i, s.end(), __xs_space); // find the end of the same word
//		if (i != s.end())
//		{
//			ret.push_back(std::string(i, j)); //insert the word into vector
//			i = j; // repeat 1,2,3 on the rest of the line.
//		}
//	}
//	return ret;
//}