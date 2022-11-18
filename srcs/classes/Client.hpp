#pragma once
#include "../../libraries.hpp"

class Client
{
	public:
		Client();
		~Client();

		void set_fd(int const & fd);
		void set_pass(string const & pass);
		void set_nick(string const & nick);
		void set_user(string const & user);
		void set_mode(string const & mode);

		int const &	get_fd() const;
		string const &	get_pass() const;
		string const &	get_nick() const;
		string const &	get_user() const;
		string const &	get_mode() const;
		
		bool is_registered(string const & pass);
		bool password_is_set(string const & pass);
		bool nickname_is_set();
		bool username_is_set();

	private:

		int _fd;
		string _pass;
		string _nick;
		string _user;
		string _mode;
};