#ifndef AUTH_H
#define AUTH_H

#include <string>


using std::string;


string encrypt(string data);
string decrypt(string data);
string HashString(string &pass);


void SaveMasterHash(const string &hash);
bool LoadMasterhash(string &hash);
bool Authenticate();

#endif
