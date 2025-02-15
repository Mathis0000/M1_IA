#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <algorithm>

using namespace std;

// Classe de base pour les objets
class Objet {
public:
    string nom;
    Objet(const string& n) : nom(n) {}
    virtual void utiliser(class Personnage& personnage) = 0;
    virtual ~Objet() = default;
};

// Classe pour les potions
class Potion : public Objet {
public:
    Potion() : Objet("Potion") {}
    void utiliser(Personnage& personnage) override;
};

// Classe pour les armes
class Arme : public Objet {
public:
    int bonusAttaquePhysique;
    int bonusAttaqueMagique;
    Arme(const string& n, int bap, int bam) : Objet(n), bonusAttaquePhysique(bap), bonusAttaqueMagique(bam) {}
    void utiliser(Personnage& personnage) override;
};

// Classe pour les armures
class Armure : public Objet {
public:
    int bonusDefensePhysique;
    int bonusDefenseMagique;
    Armure(const string& n, int bdp, int bdm) : Objet(n), bonusDefensePhysique(bdp), bonusDefenseMagique(bdm) {}
    void utiliser(Personnage& personnage) override;
};

// Classe pour les attaques
class Attaque {
public:
    string nom;
    int degatMagique;
    int degatPhysique;

    Attaque(const string& n, int dm, int dp)
        : nom(n), degatMagique(dm), degatPhysique(dp) {}
};

// Classe de base pour les personnages
class Personnage {
protected:
    string nom;
    int pointsDeVie;
    int valeurAttaquePhysique;
    int valeurAttaqueMagique;
    int defensePhysique;
    int defenseMagique;
    vector<shared_ptr<Objet>> inventaire;
    shared_ptr<Arme> armeEquipee;
    shared_ptr<Armure> armureEquipee;

public:
    Personnage(const string& n, int pv, int vap, int vam, int dp, int dm)
        : nom(n), pointsDeVie(pv), valeurAttaquePhysique(vap), valeurAttaqueMagique(vam),
          defensePhysique(dp), defenseMagique(dm),
          armeEquipee(nullptr), armureEquipee(nullptr) {}

    virtual ~Personnage() = default;

    int getPointsDeVie() const { return pointsDeVie; }
    void setPointsDeVie(int pv) { pointsDeVie = pv; }

    int getValeurAttaquePhysique() const { return valeurAttaquePhysique; }
    void setValeurAttaquePhysique(int vap) { valeurAttaquePhysique = vap; }

    int getValeurAttaqueMagique() const { return valeurAttaqueMagique; }
    void setValeurAttaqueMagique(int vam) { valeurAttaqueMagique = vam; }

    int getDefensePhysique() const { return defensePhysique; }
    void setDefensePhysique(int dp) { defensePhysique = dp; }

    int getDefenseMagique() const { return defenseMagique; }
    void setDefenseMagique(int dm) { defenseMagique = dm; }

    const vector<shared_ptr<Objet>>& getInventaire() const { return inventaire; }
    vector<shared_ptr<Objet>>& getInventaire() { return inventaire; }
    const string& getNom() const { return nom; }

    void attaquer(Personnage& cible, const Attaque& attaque);
    void subirDegats(int degats);
    void ajouterObjet(shared_ptr<Objet> objet);
    void equiperArme(shared_ptr<Arme> arme);
    void equiperArmure(shared_ptr<Armure> armure);

    friend ostream& operator<<(ostream& os, const Personnage& personnage);
};

// Classe pour le joueur
class Joueur : public Personnage {
public:
    Joueur(const string& n, int pv, int vap, int vam, int dp, int dm) : Personnage(n, pv, vap, vam, dp, dm) {}

    void rencontrerEnnemi(Personnage& ennemi, const vector<Attaque>& attaques);
    void rencontrerAllie(Personnage& allie);
    void rencontrerObjet(shared_ptr<Objet> objet);
    void sauvegarder(const string& fichier);
    void charger(const string& fichier);
};

// Implémentation des méthodes

void Potion::utiliser(Personnage& personnage) {
    personnage.setPointsDeVie(min(personnage.getPointsDeVie() + 10, 100)); // Soigne 10 PV
    cout << "Vous avez utilisé une potion et récupéré 10 PV." << endl;
}

void Arme::utiliser(Personnage& personnage) {
    personnage.equiperArme(shared_ptr<Arme>(this));
    cout << "Vous avez équipé l'arme: " << nom << endl;
}

void Armure::utiliser(Personnage& personnage) {
    personnage.equiperArmure(shared_ptr<Armure>(this));
    cout << "Vous avez équipé l'armure: " << nom << endl;
}

void Personnage::attaquer(Personnage& cible, const Attaque& attaque) {
    cout << "----------------------------------------" << endl;
    cout << nom << " attaque avec l'attaque: " << attaque.nom << endl;
    cout << "Valeur Attaque Physique: " << valeurAttaquePhysique << endl;
    cout << "Valeur Attaque Magique: " << valeurAttaqueMagique << endl;
    cout << "Défense Physique: " << defensePhysique << endl;
    cout << "Défense Magique: " << defenseMagique << endl;

    int degatsPhysiques = valeurAttaquePhysique + attaque.degatPhysique - cible.defensePhysique;
    int degatsMagiques = valeurAttaqueMagique + attaque.degatMagique - cible.defenseMagique;

    cout << "Degats Physiques: " << degatsPhysiques << " (Attaque Physique: " << valeurAttaquePhysique << ", Degat Physique: " << attaque.degatPhysique << ", Defense Physique: " << cible.defensePhysique << ")" << endl;
    cout << "Degats Magiques: " << degatsMagiques << " (Attaque Magique: " << valeurAttaqueMagique << ", Degat Magique: " << attaque.degatMagique << ", Defense Magique: " << cible.defenseMagique << ")" << endl;

    int degatsTotaux = max(max(degatsPhysiques, 0), max(degatsMagiques, 0));

    cout << "Calcul des dégâts:" << endl;
    cout << "Dégâts Physiques: " << max(degatsPhysiques, 0) << endl;
    cout << "Dégâts Magiques: " << max(degatsMagiques, 0) << endl;
    cout << "Dégâts Totaux: " << degatsTotaux << endl;

    cout << "Cible avant attaque: " << endl;
    cout << cible.nom << " - Points de vie: " << cible.getPointsDeVie() << endl;
    cout << "Défense Physique: " << cible.getDefensePhysique() << endl;
    cout << "Défense Magique: " << cible.getDefenseMagique() << endl;

    cible.subirDegats(degatsTotaux);

    cout << "Cible après attaque: " << endl;
    cout << cible.nom << " - Points de vie: " << cible.getPointsDeVie() << endl;
    cout << "----------------------------------------" << endl;
}
void Personnage::subirDegats(int degats) {
    pointsDeVie -= degats;
    if (pointsDeVie <= 0) {
        pointsDeVie = 0;
        cout << nom << " a été vaincu!" << endl;
    } else {
        cout << nom << " a subi " << degats << " dégâts!" << endl;
    }
}

void Personnage::ajouterObjet(shared_ptr<Objet> objet) {
    if (inventaire.size() < 10) {
        inventaire.push_back(objet);
        cout << "Vous avez ajouté " << objet->nom << " à votre inventaire." << endl;
    } else {
        cout << "Votre inventaire est plein!" << endl;
    }
}

void Personnage::equiperArme(shared_ptr<Arme> arme) {
    armeEquipee = arme;
    valeurAttaquePhysique += arme->bonusAttaquePhysique;
    valeurAttaqueMagique += arme->bonusAttaqueMagique;
}

void Personnage::equiperArmure(shared_ptr<Armure> armure) {
    armureEquipee = armure;
    defensePhysique += armure->bonusDefensePhysique;
    defenseMagique += armure->bonusDefenseMagique;
}

ostream& operator<<(ostream& os, const Personnage& personnage) {
    os << "Nom: " << personnage.nom << endl;
    os << "Points de vie: " << personnage.pointsDeVie << endl;
    os << "Attaque Physique: " << personnage.valeurAttaquePhysique << endl;
    os << "Attaque Magique: " << personnage.valeurAttaqueMagique << endl;
    os << "Défense Physique: " << personnage.defensePhysique << endl;
    os << "Défense Magique: " << personnage.defenseMagique << endl;
    return os;
}

void Joueur::rencontrerEnnemi(Personnage& ennemi, const vector<Attaque>& attaques) {
    bool tourJoueur = rand() % 2 == 0;
    while (getPointsDeVie() > 0 && ennemi.getPointsDeVie() > 0) {
        if (tourJoueur) {
            cout << "----------------------------------------" << endl;
            cout << "C'est votre tour!" << endl;
            cout << "Choisissez une attaque :" << endl;
            for (size_t i = 0; i < attaques.size(); ++i) {
                cout << i + 1 << ". " << attaques[i].nom << endl;
            }
            int choix;
            cin >> choix;
            if (choix > 0 && static_cast<size_t>(choix) <= attaques.size()) {
                attaquer(ennemi, attaques[choix - 1]);
            } else {
                cout << "Choix invalide! Tour perdu." << endl;
            }
        } else {
            cout << "----------------------------------------" << endl;
            cout << "C'est le tour de l'ennemi!" << endl;
            ennemi.attaquer(*this, attaques[rand() % attaques.size()]);
        }
        tourJoueur = !tourJoueur;
    }
    if (getPointsDeVie() > 0) {
        cout << "Vous avez vaincu l'ennemi!" << endl;
        // Récupérer les objets de l'ennemi
        for (auto& objet : ennemi.getInventaire()) {
            if (inventaire.size() < 10) {
                inventaire.push_back(objet);
                cout << "Vous avez récupéré: " << objet->nom << endl;
            }
        }
    } else {
        cout << "Vous avez été vaincu par l'ennemi!" << endl;
    }
}

void Joueur::rencontrerAllie(Personnage& allie) {
    if (!allie.getInventaire().empty()) {
        auto objet = allie.getInventaire().back();
        allie.getInventaire().pop_back();
        ajouterObjet(objet);
    }
}

void Joueur::rencontrerObjet(shared_ptr<Objet> objet) {
    ajouterObjet(objet);
}

void Joueur::sauvegarder(const string& fichier) {
    ofstream out(fichier, ios::binary);
    out.write(reinterpret_cast<const char*>(&pointsDeVie), sizeof(pointsDeVie));
    out.write(reinterpret_cast<const char*>(&valeurAttaquePhysique), sizeof(valeurAttaquePhysique));
    out.write(reinterpret_cast<const char*>(&valeurAttaqueMagique), sizeof(valeurAttaqueMagique));
    out.write(reinterpret_cast<const char*>(&defensePhysique), sizeof(defensePhysique));
    out.write(reinterpret_cast<const char*>(&defenseMagique), sizeof(defenseMagique));
    // Sauvegarder les autres attributs et objets de l'inventaire
}

void Joueur::charger(const string& fichier) {
    ifstream in(fichier, ios::binary);
    in.read(reinterpret_cast<char*>(&pointsDeVie), sizeof(pointsDeVie));
    in.read(reinterpret_cast<char*>(&valeurAttaquePhysique), sizeof(valeurAttaquePhysique));
    in.read(reinterpret_cast<char*>(&valeurAttaqueMagique), sizeof(valeurAttaqueMagique));
    in.read(reinterpret_cast<char*>(&defensePhysique), sizeof(defensePhysique));
    in.read(reinterpret_cast<char*>(&defenseMagique), sizeof(defenseMagique));
    // Charger les autres attributs et objets de l'inventaire
}

vector<Attaque> chargerAttaques(const string& fichier) {
    vector<Attaque> attaques;
    ifstream in(fichier);
    string ligne;
    while (getline(in, ligne)) {
        istringstream iss(ligne);
        string nom;
        int degatMagique, degatPhysique;
        if (iss >> nom >> degatMagique >> degatPhysique) {
            attaques.emplace_back(nom, degatMagique, degatPhysique);
        }
    }
    return attaques;
}

int main() {
    srand(time(nullptr));

    int choixClasse;
    cout << "Choisissez votre classe :" << endl;
    cout << "1. Barbare (dégâts physiques accrus)" << endl;
    cout << "2. Magicien (dégâts magiques accrus)" << endl;
    cin >> choixClasse;

    Joueur joueur("Joueur", 100, 10, 10, 5, 5);
    if (choixClasse == 1) {
        // Barbare : bonus de dégâts physiques
        joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);    
        auto arme = make_shared<Arme>("Épée", 5, 0);

    } else if (choixClasse == 2) {
        // Magicien : bonus de dégâts magiques
        joueur.setValeurAttaqueMagique(joueur.getValeurAttaqueMagique() + 15);
        auto arme = make_shared<Arme>("Baton magique", 0, 5);
    } else {
        cout << "Choix invalide! Barbare choisi par défaut." << endl;
        joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);
    }

    vector<Attaque> attaques = chargerAttaques("attaques.txt");

    Personnage ennemi("Ennemi", 50, 8, 8, 3, 3);
    Personnage allie("Allié", 30, 5, 5, 2, 2);

    auto potion = make_shared<Potion>();
    auto bouclier = make_shared<Armure>("Bouclier", 3, 2);

    joueur.rencontrerObjet(potion);
    joueur.rencontrerObjet(arme);
    joueur.rencontrerObjet(bouclier);

    joueur.rencontrerEnnemi(ennemi, attaques);
    joueur.rencontrerAllie(allie);

    cout << joueur << endl;

    return 0;
}
