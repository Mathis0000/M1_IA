#include "Arme.h"
#include "Personnage.h"
#include <iostream>

Arme::Arme(const std::string& n, int bap, int bam) : Objet(n), bonusAttaquePhysique(bap), bonusAttaqueMagique(bam) {}

/**
 * @brief Utilizes the weapon by equipping it to the given character if it is not already equipped.
 *
 * This function checks if the weapon is already equipped by the character. If not, it equips the weapon
 * and prints a message indicating that the weapon has been equipped. If the weapon is already equipped,
 * it prints a message indicating that the weapon is already equipped.
 *
 * @param personnage The character to equip the weapon to.
 */
void Arme::utiliser(Personnage& personnage) {
    if (personnage.getArmeEquipee() != shared_from_this()) {
        personnage.equiperArme(shared_from_this());
        std::cout << "Vous avez équipé l'arme: " << nom << std::endl;
    } else {
        std::cout << "L'arme " << nom << " est déjà équipée." << std::endl;
    }
}
