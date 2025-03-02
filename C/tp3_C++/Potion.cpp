#include "Potion.h"
#include "Personnage.h"
#include <iostream>
#include <algorithm>

Potion::Potion() : Objet("Potion") {}

/**
 * @brief Utilizes the potion on the given character.
 *
 * This function restores the character's health points to 100 and removes the potion from the character's inventory.
 *
 * @param personnage The character on whom the potion is used.
 */
void Potion::utiliser(Personnage& personnage) {
    personnage.setPointsDeVie(100);
    std::cout << "Vous avez utilisé une potion et récupéré tous vos PV." << std::endl;

    auto& inventaire = personnage.getInventaire();
    auto it = std::find_if(inventaire.begin(), inventaire.end(),
                      [this](const std::shared_ptr<Objet>& obj) { return obj.get() == this; });
    if (it != inventaire.end()) {
        inventaire.erase(it);
    }
}
