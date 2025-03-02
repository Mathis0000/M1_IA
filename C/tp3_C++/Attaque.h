#ifndef ATTAQUE_H
#define ATTAQUE_H

#include <string>

/**
 * @class Attaque
 * @brief Represents an attack with magical and physical damage.
 *
 * The Attaque class encapsulates the properties of an attack, including its name,
 * magical damage, and physical damage.
 */

/**
 * @brief Constructs an Attaque object with the specified name, magical damage, and physical damage.
 * 
 * @param n The name of the attack.
 * @param dm The magical damage of the attack.
 * @param dp The physical damage of the attack.
 */
class Attaque {
public:
    std::string nom;
    int degatMagique;
    int degatPhysique;

    Attaque(const std::string& n, int dm, int dp);
};

#endif // ATTAQUE_H
