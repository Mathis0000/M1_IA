#ifndef OBJET_H
#define OBJET_H

#include <string>
#include <memory>

class Personnage;

class Objet {
public:
    std::string nom;
    Objet(const std::string& n);
    virtual void utiliser(Personnage& personnage) = 0;
    virtual ~Objet() = default;
};

#endif // OBJET_H
