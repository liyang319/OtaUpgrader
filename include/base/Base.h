#pragma once

#include <iostream>
#include <cstdio>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <string.h>

using namespace std;

// #define COUT (cout << "[" << Base::currentTime() << ": " << Base::fileName(new string(__FILE__)) << ": " << __func__ << ": " << __LINE__ << "]->: ")
// #define CERR (cerr << "[" << Base::currentTime() << ": " << Base::fileName(new string(__FILE__)) << ": " << __func__ << ": " << __LINE__ << "]->: ")
// #define ENABLE_LOGFILE

#ifndef ENABLE_LOGFILE
#define COUT (cout << "[" << Base::currentTime() << ": " << Base::fileName(new string(__FILE__)) << ": " << __func__ << ": " << __LINE__ << "]->: ")
#define CERR (cerr << "[" << Base::currentTime() << ": " << Base::fileName(new string(__FILE__)) << ": " << __func__ << ": " << __LINE__ << "]->: ")
#else
#define COUT (ofstream("update_" + Base::currentDay() + ".log", ios::app) << "[" << Base::currentTime() << ": " << Base::fileName(new string(__FILE__)) << ": " << __func__ << ": " << __LINE__ << "]->: ")
#define CERR (ofstream("update_" + Base::currentDay() + ".log", ios::app) << "[" << Base::currentTime() << ": " << Base::fileName(new string(__FILE__)) << ": " << __func__ << ": " << __LINE__ << "]->: ")
#endif

class Base
{
private:
    /* data */
public:
    Base(/* args */);
    ~Base();

    static string currentTime();
    static string currentDay();
    static string fileName(string *path);
};
