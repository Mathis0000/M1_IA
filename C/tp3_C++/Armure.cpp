#include "Armure.h"
#include "Personnage.h"
#include <iostream>

Armure::Armure(const std::string& n, int bdp, int bdm) : Objet(n), bonusDefensePhysique(bdp), bonusDefenseMagique(bdm) {}

/**
 * @brief Utilizes the armor by equipping it to the given character.
 * 
 * This function equips the armor to the specified character and outputs a message
 * indicating that the armor has been equipped.
 * 
 * @param personnage The character to equip the armor to.
 */
void Armure::utiliser(Personnage& personnage) {
    personnage.equiperArmure(shared_from_this());
    std::cout << "Vous avez équipé l'armure: " << nom << std::endl;
}
