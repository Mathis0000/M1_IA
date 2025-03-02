#include "Attaque.h"

/**
 * @brief Construct a new Attaque object.
 * 
 * @param n The name of the attack.
 * @param dm The magical damage of the attack.
 * @param dp The physical damage of the attack.
 */
Attaque::Attaque(const std::string& n, int dm, int dp)
    : nom(n), degatMagique(dm), degatPhysique(dp) {}
