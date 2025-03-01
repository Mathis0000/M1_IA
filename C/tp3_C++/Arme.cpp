#include "Arme.h"
#include "Personnage.h"
#include <iostream>

Arme::Arme(const std::string& n, int bap, int bam) : Objet(n), bonusAttaquePhysique(bap), bonusAttaqueMagique(bam) {}

void Arme::utiliser(Personnage& personnage) {
    if (personnage.getArmeEquipee() != shared_from_this()) {
        personnage.equiperArme(shared_from_this());
        std::cout << "Vous avez équipé l'arme: " << nom << std::endl;
    } else {
        std::cout << "L'arme " << nom << " est déjà équipée." << std::endl;
    }
}
