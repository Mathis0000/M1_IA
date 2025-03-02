#ifndef ARMURE_H
#define ARMURE_H

#include "Objet.h"
#include <memory>

/**
 * @class Armure
 * @brief Represents an armor object that provides physical and magical defense bonuses.
 * 
 * The Armure class inherits from Objet and std::enable_shared_from_this<Armure>.
 * It provides additional defense capabilities to a character.
 * 
 * @var int bonusDefensePhysique
 * Member variable that stores the physical defense bonus provided by the armor.
 * 
 * @var int bonusDefenseMagique
 * Member variable that stores the magical defense bonus provided by the armor.
 * 
 * @fn Armure(const std::string& n, int bdp, int bdm)
 * Constructor that initializes the armor with a name, physical defense bonus, and magical defense bonus.
 * 
 * @param n The name of the armor.
 * @param bdp The physical defense bonus provided by the armor.
 * @param bdm The magical defense bonus provided by the armor.
 * 
 * @fn void utiliser(Personnage& personnage) override
 * Method to apply the armor's defense bonuses to a character.
 * 
 * @param personnage The character to which the armor's bonuses will be applied.
 */
class Armure : public Objet, public std::enable_shared_from_this<Armure> {
public:
    int bonusDefensePhysique;
    int bonusDefenseMagique;
    Armure(const std::string& n, int bdp, int bdm);
    void utiliser(Personnage& personnage) override;
};

#endif // ARMURE_H
