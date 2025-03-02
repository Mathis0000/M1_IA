#ifndef OBJET_H
#define OBJET_H

#include <string>
#include <memory>

class Personnage;

/**
 * @class Objet
 * @brief A base class representing an object with a name that can be used by a character.
 *
 * The Objet class serves as an abstract base class for objects that have a name and can be used by a character.
 * It provides a pure virtual function `utiliser` that must be overridden by derived classes to define the specific
 * behavior when the object is used.
 */
class Objet {
public:
    std::string nom;
    Objet(const std::string& n);
    virtual void utiliser(Personnage& personnage) = 0;
    virtual ~Objet() = default;
};

#endif // OBJET_H
