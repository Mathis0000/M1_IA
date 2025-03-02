#ifndef ATTAQUEOBJET_H
#define ATTAQUEOBJET_H

#include "Objet.h"
#include "Attaque.h"

/**
 * @class AttaqueObjet
 * @brief A class representing an attack object that can be used by a character.
 * 
 * This class inherits from the Objet class and contains an Attaque object.
 * It provides functionality to use the attack object on a character.
 */

/**
 * @brief Constructor for AttaqueObjet.
 * 
 * Initializes the AttaqueObjet with a given Attaque object.
 * 
 * @param a The Attaque object to initialize the AttaqueObjet with.
 */

/**
 * @brief Uses the attack object on a given character.
 * 
 * Overrides the utiliser method from the Objet class to apply the attack
 * to the specified character.
 * 
 * @param personnage The character on whom the attack object is used.
 */
class AttaqueObjet : public Objet {
public:
    Attaque attaque;
    AttaqueObjet(const Attaque& a);
    void utiliser(Personnage& personnage) override;
};

#endif // ATTAQUEOBJET_H
