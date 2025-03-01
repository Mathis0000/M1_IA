#ifndef ARMURE_H
#define ARMURE_H

#include "Objet.h"
#include <memory>

class Armure : public Objet, public std::enable_shared_from_this<Armure> {
public:
    int bonusDefensePhysique;
    int bonusDefenseMagique;
    Armure(const std::string& n, int bdp, int bdm);
    void utiliser(Personnage& personnage) override;
};

#endif // ARMURE_H
