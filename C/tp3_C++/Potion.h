#ifndef POTION_H
#define POTION_H

#include "Objet.h"

class Potion : public Objet {
public:
    /**
     * @brief Default constructor for the Potion class.
     * 
     * This constructor initializes a new instance of the Potion class.
     */
    Potion();
    
    /**
     * @brief Utilizes the potion on the given character.
     * 
     * This function applies the effects of the potion to the specified character.
     * 
     * @param personnage The character on which the potion will be used.
     */
    void utiliser(Personnage& personnage) override;
};

#endif // POTION_H
