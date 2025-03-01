#ifndef ARME_H
#define ARME_H

#include "Objet.h"
#include <memory>

class Arme : public Objet, public std::enable_shared_from_this<Arme> {
public:
    int bonusAttaquePhysique;
    int bonusAttaqueMagique;
    Arme(const std::string& n, int bap, int bam);
    void utiliser(Personnage& personnage) override;
};

#endif // ARME_H
