#include "auth.h"
#include "helpers.h"

#include <fstream>
#include <iostream>

using namespace std;

string encrypt(string data)
{
    string key = "i_am_iron_man";
    for (size_t i = 0; i < data.length(); i++)
    {
        data[i] ^= key[i % key.length()];
    }
    return data;
}

string decrypt(string data)
{
    return encrypt(data);
}

string HashString(string &pass)
{
    unsigned long long h = 0;
    for (auto &c : pass)
    {
        h = h * 131 + c;
    }
    return to_string(h);
}

void SaveMasterHash(const string &hash)
{
    ofstream file("master_password.dat");
    file << hash;
    file.close();
}

bool LoadMasterhash(string &hash)
{
    ifstream file("master_password.dat");
    if (!file)
    {
        return false;
    }
    getline(file, hash);
    file.close();
    return true;
}

bool Authenticate()
{
    WORD old = save_color();
    string storedHash;
    if (!LoadMasterhash(storedHash))
    {

        setColor(WHITE);
        cout << "Set Master Password: ";
        setColor(old);
        string pass;
        pass = get_string();
        SaveMasterHash(HashString(pass));
        setColor(GREEN);
        cout << "Password Set Successfully!\n";
        setColor(old);
        return true;
    }

    int tries = 3;

    for (int attempt = 0; attempt < tries; attempt++)
    {
        string key;

        setColor(WHITE);
        cout << "Enter Master Password: ";
        setColor(old);
        key = get_hidden_password();
        if (HashString(key) == storedHash)
        {
            setColor(GREEN);
            cout << "Access Granted!\n";
            setColor(old);
            return true;
        }
        else
        {
            setColor(RED);
            cout << "Wrong Password! " << tries - attempt - 1 << " chances left.\n";
            setColor(old);
        }
    }

    setColor(RED);
    cout << "Access Denied. Exiting...\n";
    setColor(old);
    return false;
}