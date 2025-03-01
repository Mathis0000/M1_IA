#include "Armure.h"
#include "Personnage.h"
#include <iostream>

Armure::Armure(const std::string& n, int bdp, int bdm) : Objet(n), bonusDefensePhysique(bdp), bonusDefenseMagique(bdm) {}

void Armure::utiliser(Personnage& personnage) {
    personnage.equiperArmure(shared_from_this());
    std::cout << "Vous avez équipé l'armure: " << nom << std::endl;
}
