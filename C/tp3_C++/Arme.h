#ifndef ARME_H
#define ARME_H

#include "Objet.h"
#include <memory>

/**
 * @class Arme
 * @brief A class representing a weapon that can be used by a character.
 * 
 * The Arme class inherits from Objet and std::enable_shared_from_this<Arme>.
 * It provides additional attributes for physical and magical attack bonuses.
 */
 
/**
 * @brief Constructs a new Arme object.
 * 
 * @param n The name of the weapon.
 * @param bap The bonus physical attack provided by the weapon.
 * @param bam The bonus magical attack provided by the weapon.
 */
 
/**
 * @brief Uses the weapon on a given character.
 * 
 * @param personnage The character on which the weapon is used.
 */
class Arme : public Objet, public std::enable_shared_from_this<Arme> {
public:
    int bonusAttaquePhysique;
    int bonusAttaqueMagique;
    Arme(const std::string& n, int bap, int bam);
    void utiliser(Personnage& personnage) override;
};

#endif // ARME_H
