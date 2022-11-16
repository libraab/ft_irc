#include "../ft_irc.hpp"

void error(string str)
{
	cout << str << endl;
	exit(1);
}
void failed_function(string function_name)
{
	cerr << "Error: " + function_name + " failed! ⛔️" << endl;
	exit(1);
}

vector<string> split(string str, char del)
{
    vector <string> tab;
    while (1) {
        size_t occ = str.find(del);
        if (occ != string::npos) {
            string tmp = str.substr(0, occ);
            if (tmp.size() >= 1 && tmp[0] != del)
                tab.push_back(tmp);
            str = str.substr(occ + 1);
        } else if (!str.empty()) {
            tab.push_back(str);
            return (tab);
        } else
            return (tab);
    }
    return (tab);
}

ostream & operator<<(ostream & o, vector<string> const & v) {
    for (vector<string>::const_iterator it = v.begin(); it != v.end(); it++)
        o << "{" << *it << "}" << endl;
    return (o);
}

void	print_vector(vector<string> vec) {
	cout << vec << endl;
}

void    remove_r(string & str) {
	size_t r = 1;
	while (r != string::npos) {
		r = str.find('\r');
		if (r != string::npos)
			str.erase(r, 1);
	}
}

void    ft_send(int fd, string const & msg)
{
	if (fd == -1 ||  msg == ":localhost PONG :localhost")
        return ;
	if (send(fd, msg.c_str(), msg.length(), 0) == -1)
		cerr << "Send failed ❌" << endl;
    cout << "-------------------" << endl;
    cout << black bg_green << "| SEND TO CLIENT " << fd <<"|" << reset << endl<< msg.substr(0, msg.length()-1) << endl;
    cout << "-------------------" << endl;
}