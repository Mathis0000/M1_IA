#ifndef ATTAQUE_H
#define ATTAQUE_H

#include <string>

class Attaque {
public:
    std::string nom;
    int degatMagique;
    int degatPhysique;

    Attaque(const std::string& n, int dm, int dp);
};

#endif // ATTAQUE_H
