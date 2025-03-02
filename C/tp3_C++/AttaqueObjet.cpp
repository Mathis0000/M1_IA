#include "AttaqueObjet.h"
#include "Personnage.h"
#include <iostream>

AttaqueObjet::AttaqueObjet(const Attaque& a) : Objet(a.nom), attaque(a) {}

/**
 * @brief Utilizes the attack object on a given character.
 *
 * This function outputs the name of the attack being used on the specified character.
 *
 * @param personnage The character on whom the attack is being used.
 */
void AttaqueObjet::utiliser(Personnage& personnage) {
    std::cout << "Vous utilisez l'attaque: " << attaque.nom << std::endl;
}
