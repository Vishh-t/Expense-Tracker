#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <windows.h>


using std::string;


extern const WORD CYAN;
extern const WORD RED;
extern const WORD GREEN;
extern const WORD WHITE;
extern const WORD YELLOW; 

void setColor(int color);
WORD save_color();


void do_ignore();
bool get_int(int &x);
string get_string();
string get_hidden_password();


string convert_lower(string s);
bool verify_input_key(string key, string input);


bool isleapyear(int year);
bool DateValidator(string date);

#endif
