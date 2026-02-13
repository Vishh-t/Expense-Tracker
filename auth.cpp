#include "auth.h"
#include "helpers.h"

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

static const string BASE64_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

string base64_encode(const string &in)
{
    string out;
    int val = 0, valb = -6;
    for (unsigned char c : in)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            out.push_back(BASE64_CHARS[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back(BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4)
        out.push_back('=');
    return out;
}

string base64_decode(const string &in)
{
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[BASE64_CHARS[i]] = i;

    string out;
    int val = 0, valb = -8;
    for (unsigned char c : in)
    {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

string encrypt(string data)
{
    string key = "Xp3nseTr@cker_2026";
    for (size_t i = 0; i < data.length(); i++)
    {
        data[i] ^= key[i % key.length()];
    }
    return base64_encode(data);
}

string decrypt(string data)
{
    data = base64_decode(data);

    string key = "Xp3nseTr@cker_2026";

    for (size_t i = 0; i < data.length(); i++)
    {
        data[i] ^= key[i % key.length()];
    }

    return data;
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