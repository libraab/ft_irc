#pragma once
#include <sys/resource.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>
#include <utility>
#include <stdio.h>
#include <errno.h>
#include <cstring>
#include <netdb.h>
#include <string>
#include <cctype>
#include <vector>
#include <map>

using namespace std;

//--------------------------------------------//
//              C O L O U R S                 //
//--------------------------------------------//
        #define black   "[1;30m"
        #define red     "[1;31m"
        #define green   "[1;32m"
        #define yellow  "[1;33m"
        #define blue    "[1;34m"
        #define magenta "[1;35m"
        #define cyan    "[1;36m"
        #define white   "[1;37m"

        #define bg_black        "[40m"
        #define bg_red          "[41m"
        #define bg_green        "[42m"
        #define bg_yellow       "[43m"
        #define bg_blue         "[44m"
        #define bg_magenta      "[45m"
        #define bg_cyan         "[46m"
        #define bg_white        "[47m"

        #define reset   "[0m"
//--------------------------------------------//
