#ifndef POTION_H
#define POTION_H

#include "Objet.h"

class Potion : public Objet {
public:
    Potion();
    void utiliser(Personnage& personnage) override;
};

#endif // POTION_H
