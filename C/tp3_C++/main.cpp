#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

class Personnage; // Forward declaration

// Classe de base pour les objets
class Objet {
public:
    string nom;
    Objet(const string& n) : nom(n) {}
    virtual void utiliser(Personnage& personnage) = 0;
    virtual ~Objet() = default;
};

// Classe pour les potions
class Potion : public Objet {
public:
    Potion() : Objet("Potion") {}
    void utiliser(Personnage& personnage) override;
};

// Classe pour les armes
class Arme : public Objet, public enable_shared_from_this<Arme> {
public:
    int bonusAttaquePhysique;
    int bonusAttaqueMagique;
    Arme(const string& n, int bap, int bam) : Objet(n), bonusAttaquePhysique(bap), bonusAttaqueMagique(bam) {}
    void utiliser(Personnage& personnage) override;
};

// Classe pour les armures
class Armure : public Objet, public enable_shared_from_this<Armure> {
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

// Classe pour encapsuler une attaque comme un objet
class AttaqueObjet : public Objet {
public:
    Attaque attaque;
    AttaqueObjet(const Attaque& a) : Objet(a.nom), attaque(a) {}
    void utiliser(Personnage& /*personnage*/) override {
        // Logique pour utiliser l'attaque
        cout << "Vous utilisez l'attaque: " << attaque.nom << endl;
        // Vous pouvez ajouter ici la logique pour appliquer l'attaque
    }
};

// Classe de base pour les personnages
class Personnage : public enable_shared_from_this<Personnage> {
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

    shared_ptr<Arme> getArmeEquipee() const { return armeEquipee; }
    shared_ptr<Armure> getArmureEquipee() const { return armureEquipee; }

    void attaquer(Personnage& cible, const Attaque& attaque);
    void subirDegats(int degats);
    void ajouterObjet(shared_ptr<Objet> objet);
    void equiperArme(shared_ptr<Arme> arme);
    void desequiperArme();
    void equiperArmure(shared_ptr<Armure> armure);
    void desequiperArmure();

    friend ostream& operator<<(ostream& os, const Personnage& personnage);
};
void Personnage::desequiperArmure() {
    if (armureEquipee) {
        defensePhysique -= armureEquipee->bonusDefensePhysique;
        defenseMagique -= armureEquipee->bonusDefenseMagique;
        armureEquipee = nullptr;
    }
}

void Personnage::desequiperArme() {
    if (armeEquipee) {
        valeurAttaquePhysique -= armeEquipee->bonusAttaquePhysique;
        valeurAttaqueMagique -= armeEquipee->bonusAttaqueMagique;
        armeEquipee = nullptr;
    }
}

void Personnage::equiperArmure(shared_ptr<Armure> armure) {
    if (armureEquipee) {
        defensePhysique -= armureEquipee->bonusDefensePhysique;
        defenseMagique -= armureEquipee->bonusDefenseMagique;
    }
    armureEquipee = armure;
    if (armure) {
        defensePhysique += armure->bonusDefensePhysique;
        defenseMagique += armure->bonusDefenseMagique;
    }
}

void Personnage::equiperArme(shared_ptr<Arme> arme) {
    if (armeEquipee) {
        valeurAttaquePhysique -= armeEquipee->bonusAttaquePhysique;
        valeurAttaqueMagique -= armeEquipee->bonusAttaqueMagique;
    }
    armeEquipee = arme;
    if (arme) {
        valeurAttaquePhysique += arme->bonusAttaquePhysique;
        valeurAttaqueMagique += arme->bonusAttaqueMagique;
    }
}


class Joueur : public Personnage {
public:
    Joueur(const string& n, int pv, int vap, int vam, int dp, int dm) : Personnage(n, pv, vap, vam, dp, dm) {}

    void rencontrerEnnemi(Personnage& ennemi, const vector<Attaque>& attaques ,string nomAleatoire);
    void rencontrerAllie(Personnage& allie);
    void rencontrerObjet(shared_ptr<Objet> objet);
    void sauvegarder(const string& fichier);
    void charger(const string& fichier);

private:
    vector<Attaque> getAttaquesDisponibles() const;
};

// Implémentation des méthodes de sauvegarde et de chargement
void Joueur::sauvegarder(const string& fichier) {
    ofstream out(fichier);
    if (!out) {
        cerr << "Erreur : Impossible de sauvegarder le joueur." << endl;
        return;
    }
    out << nom << " " << pointsDeVie << " " << valeurAttaquePhysique << " " << valeurAttaqueMagique << " "
        << defensePhysique << " " << defenseMagique << endl;
    for (const auto& objet : inventaire) {
        out << objet->nom << endl;
    }
}

void Joueur::charger(const string& fichier) {
    ifstream in(fichier);
    if (!in) {
        cerr << "Erreur : Impossible de charger le joueur." << endl;
        return;
    }
    in >> nom >> pointsDeVie >> valeurAttaquePhysique >> valeurAttaqueMagique >> defensePhysique >> defenseMagique;
    inventaire.clear();
    string nomObjet;
    while (in >> nomObjet) {
        if (nomObjet == "Potion") {
            inventaire.push_back(make_shared<Potion>());
        } else {
            cerr << "Erreur : Objet inconnu lors du chargement." << endl;
        }
    }
}

// Implémentation des méthodes

void Potion::utiliser(Personnage& personnage) {
    personnage.setPointsDeVie(100); // Remet la vie à 100 PV
    cout << "Vous avez utilisé une potion et récupéré tous vos PV." << endl;

    // Supprimer seulement la première potion de l'inventaire
    auto& inventaire = personnage.getInventaire();
    auto it = find_if(inventaire.begin(), inventaire.end(),
                      [this](const shared_ptr<Objet>& obj) { return obj.get() == this; });
    if (it != inventaire.end()) {
        inventaire.erase(it);
    }
}

void Arme::utiliser(Personnage& personnage) {
    if (personnage.getArmeEquipee() != shared_from_this()) {
        personnage.equiperArme(shared_from_this());
        cout << "Vous avez équipé l'arme: " << nom << endl;
    } else {
        cout << "L'arme " << nom << " est déjà équipée." << endl;
    }
}

void Armure::utiliser(Personnage& personnage) {
    personnage.equiperArmure(shared_from_this());
    cout << "Vous avez équipé l'armure: " << nom << endl;
}

void Personnage::attaquer(Personnage& cible, const Attaque& attaque) {
    cout << "----------------------------------------" << endl;
    cout << nom << " attaque avec l'attaque: " << attaque.nom << endl;

    int degatsPhysiques = valeurAttaquePhysique + attaque.degatPhysique - cible.defensePhysique;
    int degatsMagiques = valeurAttaqueMagique + attaque.degatMagique - cible.defenseMagique;

    cout << "Dégâts Physiques: " << degatsPhysiques << " (Attaque Physique: " << valeurAttaquePhysique << ", Dégât du sort Physique: " << attaque.degatPhysique << ", Défense Physique: " << cible.defensePhysique << ")" << endl;
    cout << "Dégâts Magiques: " << degatsMagiques << " (Attaque Magique: " << valeurAttaqueMagique << ", Dégât du sort Magique: " << attaque.degatMagique << ", Défense Magique: " << cible.defenseMagique << ")" << endl;

    int degatsTotaux = max(max(degatsPhysiques, 0), max(degatsMagiques, 0));
    cout << "Dégâts les plus efficaces : " << degatsTotaux << endl;

    cout << "Cible avant attaque: " << endl;
    cout << cible.nom << " - Points de vie: " << cible.getPointsDeVie() << endl;

    cible.subirDegats(degatsTotaux);

    cout << "Cible après attaque: " << endl;
    cout << cible.nom << " - Points de vie: " << cible.getPointsDeVie() << endl;
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


ostream& operator<<(ostream& os, const Personnage& personnage) {
    os << "Nom: " << personnage.nom << endl;
    os << "Points de vie: " << personnage.pointsDeVie << endl;
    os << "Attaque Physique: " << personnage.valeurAttaquePhysique << endl;
    os << "Attaque Magique: " << personnage.valeurAttaqueMagique << endl;
    os << "Défense Physique: " << personnage.defensePhysique << endl;
    os << "Défense Magique: " << personnage.defenseMagique << endl;
    return os;
}

void Joueur::rencontrerEnnemi(Personnage& ennemi, const vector<Attaque>& attaques, string nomAleatoire) {
    bool tourJoueur = rand() % 2 == 0;
    while (getPointsDeVie() > 0 && ennemi.getPointsDeVie() > 0) {
        if (tourJoueur) {
            cout << "----------------------------------------" << endl;
            cout << "C'est votre tour!" << endl;
            vector<Attaque> attaquesDisponibles = getAttaquesDisponibles();
            if (attaquesDisponibles.empty()) {
                cout << "Vous n'avez aucune attaque disponible!" << endl;
                break;
            }
            cout << "Choisissez une attaque :" << endl;
            for (size_t i = 0; i < attaquesDisponibles.size(); ++i) {
                cout << i + 1 << ". " << attaquesDisponibles[i].nom  <<", Attaque Magique : "<<attaquesDisponibles[i].degatMagique<<", Attaque Physique : "<<attaquesDisponibles[i].degatPhysique << endl;
            }
            int choix;
            cin >> choix;
            if (choix > 0 && static_cast<size_t>(choix) <= attaquesDisponibles.size()) {
                attaquer(ennemi, attaquesDisponibles[choix - 1]);
            } else {
                cout << "Choix invalide! Tour perdu." << endl;
            }
        } else {
            cout << "----------------------------------------" << endl;
            cout << "C'est le tour du " <<nomAleatoire<< " !" << endl;
            if (!attaques.empty()) {
                int attaqueAleatoire = rand() % attaques.size();
                ennemi.attaquer(*this, attaques[attaqueAleatoire]);
            }
        }
        tourJoueur = !tourJoueur;

        // Vérifier si le joueur a été vaincu
        if (getPointsDeVie() <= 0) {
            cout << "Vous avez été vaincu!" << endl;
            auto& inventaire = getInventaire();
            auto it = find_if(inventaire.begin(), inventaire.end(), [](const shared_ptr<Objet>& obj) {
                return dynamic_pointer_cast<Potion>(obj) != nullptr;
            });
            if (it != inventaire.end()) {
                (*it)->utiliser(*this);
            }
        }

        // Vérifier si l'ennemi a été vaincu
        if (ennemi.getPointsDeVie() <= 0) {
            cout << "Vous avez vaincu le " <<nomAleatoire<< " !"  << endl;
            break;
        }
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

vector<Attaque> Joueur::getAttaquesDisponibles() const {
    vector<Attaque> attaquesDisponibles;
    for (const auto& objet : inventaire) {
        if (auto attaqueObjet = dynamic_pointer_cast<AttaqueObjet>(objet)) {
            attaquesDisponibles.push_back(attaqueObjet->attaque);
        }
    }
    return attaquesDisponibles;
}

vector<Attaque> chargerAttaques(const string& fichier) {
    vector<Attaque> attaques;
    ifstream in(fichier);
    if (!in) {
        cerr << "Erreur : Impossible de charger les attaques." << endl;
        return attaques;
    }
    string ligne;
    while (getline(in, ligne)) {
        istringstream iss(ligne);
        string nom;
        int degatMagique, degatPhysique;
        // Utiliser '/' comme délimiteur pour séparer les valeurs
        getline(iss, nom, '/');
        iss >> degatMagique;
        iss.ignore(); // Ignorer le '/'
        iss >> degatPhysique;
        if (!nom.empty() && iss) {
            attaques.emplace_back(nom, degatMagique, degatPhysique);
        } else {
            cerr << "Erreur : Format incorrect dans le fichier des attaques." << endl;
        }
    }
    return attaques;
}


vector<shared_ptr<Arme>> chargerArmes(const string& fichier) {
    vector<shared_ptr<Arme>> armes;
    ifstream in(fichier);
    if (!in) {
        cerr << "Erreur : Impossible de charger les armes." << endl;
        return armes;
    }
    string ligne;
    while (getline(in, ligne)) {
        istringstream iss(ligne);
        string nom;
        int bonusAttaquePhysique, bonusAttaqueMagique;
        // Utiliser '/' comme délimiteur pour séparer les valeurs
        getline(iss, nom, '/');
        iss >> bonusAttaquePhysique;
        iss.ignore(); // Ignorer le '/'
        iss >> bonusAttaqueMagique;
        if (!nom.empty() && iss) {
            armes.push_back(make_shared<Arme>(nom, bonusAttaquePhysique, bonusAttaqueMagique));
        } else {
            cerr << "Erreur : Format incorrect dans le fichier des armes." << endl;
        }
    }
    return armes;
}


vector<shared_ptr<Armure>> chargerArmures(const string& fichier) {
    vector<shared_ptr<Armure>> armures;
    ifstream in(fichier);
    if (!in) {
        cerr << "Erreur : Impossible de charger les armures." << endl;
        return armures;
    }
    string ligne;
    while (getline(in, ligne)) {
        istringstream iss(ligne);
        string nom;
        int bonusDefensePhysique, bonusDefenseMagique;
        // Utiliser '/' comme délimiteur pour séparer les valeurs
        getline(iss, nom, '/');
        iss >> bonusDefensePhysique;
        iss.ignore(); // Ignorer le '/'
        iss >> bonusDefenseMagique;
        if (!nom.empty() && iss) {
            armures.push_back(make_shared<Armure>(nom, bonusDefensePhysique, bonusDefenseMagique));
        } else {
            cerr << "Erreur : Format incorrect dans le fichier des armures." << endl;
        }
    }
    return armures;
}

string nomAleatoire() {
    ifstream in("ennemis.txt");
    if (!in) {
        cerr << "Erreur : Impossible de charger les noms des ennemis." << endl;
        return "";
    }

    vector<string> noms;
    string nom;
    while (getline(in, nom)) {
        noms.push_back(nom);
    }

    int chance = rand() % noms.size();
    return noms[chance];
}



int interface() {
    sf::VideoMode videoMode(1920, 1080);
    sf::RenderWindow window(videoMode, "Introduction");
    window.setPosition(sf::Vector2i(0, 0)); // Assure l'affichage sur l'écran principal

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("image/village.jpg")) {
        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
        return 1;
    }
    window.setVerticalSyncEnabled(true);

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Adjust the sprite size to match the window size
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(
        float(windowSize.x) / textureSize.x,
        float(windowSize.y) / textureSize.y
    );

    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police Arial.ttf !" << std::endl;
        return 1;
    }

    // Définition du texte en UTF-8
    unsigned int characterSize = videoMode.width / 80; // Adjust character size based on screen width
    std::string texteUtf8 = u8"Dans un monde ravagé par la guerre et la corruption, "
                            "les anciennes légendes parlent d’un artefact perdu, "
                            "la Pierre de Lumen, capable de restaurer l’équilibre entre "
                            "la lumière et les ténèbres."
                            "Cet artefact est caché dans le Donjon du Néant, une forteresse maudite "
                            "où résident des créatures cauchemardesques et leur maître impitoyable : "
                            "Malakar, l’Ombre Éternelle."
                            "Vous incarnez un aventurier solitaire, chargé d’infiltrer ce donjon maudit "
                            "pour récupérer la pierre avant que les ténèbres ne consument définitivement le royaume.";

    // Set the maximum width for the text to one-third of the screen width
    float maxTextWidth = videoMode.width / 3.0f;

    sf::Text text;
    text.setLineSpacing(1.5f); // Optional: Adjust line spacing for better readability

    // Wrap the text to fit within the specified width
    std::string wrappedText;
    std::istringstream words(texteUtf8);
    std::string word;
    float currentLineWidth = 0.0f;
    while (words >> word) {
        sf::Text tempText(word, font, characterSize);
        float wordWidth = tempText.getLocalBounds().width;
        if (currentLineWidth + wordWidth > maxTextWidth) {
            wrappedText += "\n";
            currentLineWidth = 0.0f;
        }
        wrappedText += word + " ";
        currentLineWidth += wordWidth + tempText.getLetterSpacing();
    }

    texteUtf8 = wrappedText;

    // Conversion UTF-8 vers sf::String
    sf::String texteSfml = sf::String::fromUtf8(texteUtf8.begin(), texteUtf8.end());

    text.setFont(font);
    text.setString(texteSfml); // Utilisation du texte converti
    text.setCharacterSize(characterSize); // Utilisation de la taille de caractère adaptée
    text.setFillColor(sf::Color::White);

    // Ajustement de la position du texte pour qu'il soit centré
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(videoMode.width / 2.0f, videoMode.height / 2.0f));

    // Create the "barbare" button
    sf::RectangleShape barbareButton(sf::Vector2f(200, 50));
    barbareButton.setFillColor(sf::Color::Red);
    barbareButton.setPosition(videoMode.width / 2.0f - 100, text.getPosition().y + textRect.height / 2.0f + 20);

    sf::Text barbareButtonText;
    barbareButtonText.setFont(font);
    barbareButtonText.setString("Barbare");
    barbareButtonText.setCharacterSize(30);
    barbareButtonText.setFillColor(sf::Color::White);
    sf::FloatRect barbareButtonTextRect = barbareButtonText.getLocalBounds();
    barbareButtonText.setOrigin(barbareButtonTextRect.left + barbareButtonTextRect.width / 2.0f, barbareButtonTextRect.top + barbareButtonTextRect.height / 2.0f);
    barbareButtonText.setPosition(barbareButton.getPosition().x + barbareButton.getSize().x / 2.0f, barbareButton.getPosition().y + barbareButton.getSize().y / 2.0f);

    // Create the "Magicien" button
    sf::RectangleShape magicienButton(sf::Vector2f(200, 50));
    magicienButton.setFillColor(sf::Color::Red);
    magicienButton.setPosition(videoMode.width / 2.0f - 100, barbareButton.getPosition().y + barbareButton.getSize().y + 20);

    sf::Text magicienButtonText;
    magicienButtonText.setFont(font);
    magicienButtonText.setString("Magicien");
    magicienButtonText.setCharacterSize(30);
    magicienButtonText.setFillColor(sf::Color::White);
    sf::FloatRect magicienButtonTextRect = magicienButtonText.getLocalBounds();
    magicienButtonText.setOrigin(magicienButtonTextRect.left + magicienButtonTextRect.width / 2.0f, magicienButtonTextRect.top + magicienButtonTextRect.height / 2.0f);
    magicienButtonText.setPosition(magicienButton.getPosition().x + magicienButton.getSize().x / 2.0f, magicienButton.getPosition().y + magicienButton.getSize().y / 2.0f);

    // Create the "Quit" button
    sf::RectangleShape quitButton(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(videoMode.width / 2.0f - 100, magicienButton.getPosition().y + magicienButton.getSize().y + 20);

    sf::Text quitButtonText;
    quitButtonText.setFont(font);
    quitButtonText.setString("Quitter");
    quitButtonText.setCharacterSize(30);
    quitButtonText.setFillColor(sf::Color::White);
    sf::FloatRect quitButtonTextRect = quitButtonText.getLocalBounds();
    quitButtonText.setOrigin(quitButtonTextRect.left + quitButtonTextRect.width / 2.0f, quitButtonTextRect.top + quitButtonTextRect.height / 2.0f);
    quitButtonText.setPosition(quitButton.getPosition().x + quitButton.getSize().x / 2.0f, quitButton.getPosition().y + quitButton.getSize().y / 2.0f);
    
    sf::RectangleShape quitButtonn(sf::Vector2f(200, 50));
    quitButtonn.setFillColor(sf::Color::Red);
    quitButtonn.setPosition(videoMode.width - 220, videoMode.height - 70); // Position en bas à droite

    sf::Text quitButtonnText;
    quitButtonnText.setFont(font);
    quitButtonnText.setString("Quitter");
    quitButtonnText.setCharacterSize(30);
    quitButtonnText.setFillColor(sf::Color::White);
    sf::FloatRect quitButtonnTextRect = quitButtonnText.getLocalBounds();
    quitButtonnText.setOrigin(quitButtonnTextRect.left + quitButtonnTextRect.width / 2.0f, quitButtonnTextRect.top + quitButtonnTextRect.height / 2.0f);
    quitButtonnText.setPosition(quitButtonn.getPosition().x + quitButtonn.getSize().x / 2.0f, quitButtonn.getPosition().y + quitButtonn.getSize().y / 2.0f);

    
    
    srand(time(nullptr));
    vector<Attaque> attaques = chargerAttaques("attaques.txt");
    vector<shared_ptr<Arme>> armes = chargerArmes("armes.txt");
    vector<shared_ptr<Armure>> armures = chargerArmures("armures.txt");
    Joueur joueur("Joueur", 100, 10, 10, 5, 5);

    // Ajouter une attaque aléatoire à l'inventaire du joueur
    if (!attaques.empty()) {
        int numero_attaque = rand() % attaques.size();
        auto attaque_heros = make_shared<AttaqueObjet>(attaques[numero_attaque]);
        joueur.ajouterObjet(attaque_heros);
    }

    shared_ptr<Arme> arme;
    shared_ptr<Armure> armure;
    
    bool showbarbaremagicien = true;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (barbareButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        // barbare button logic
                        std::cout << "barbare button clicked!" << std::endl;
                        joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);
                        arme = make_shared<Arme>("Épée", 5, 0);
                        armure = make_shared<Armure>("Armure de l'apprenti soldat", 5, 0);

                        // Load new background image
                        if (!backgroundTexture.loadFromFile("image/grotte.jpg")) {
                            std::cerr << "Erreur : Impossible de charger l'image grotte.jpg !" << std::endl;
                        }
                        backgroundSprite.setTexture(backgroundTexture);
                        
                        showbarbaremagicien = false;
                        sf::RectangleShape quitButton(sf::Vector2f(200, 50));
                        quitButton.setFillColor(sf::Color::Red);
                        quitButton.setPosition(videoMode.width - 220, videoMode.height - 70); // Position en bas à droite

                        
                    }
                    if (magicienButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        // Magicien button logic
                        std::cout << "Magicien button clicked!" << std::endl;
                        joueur.setValeurAttaqueMagique(joueur.getValeurAttaqueMagique() + 15);
                        arme = make_shared<Arme>("Bâton magique", 0, 5);
                        armure = make_shared<Armure>("Armure de l'apprenti magicien", 0, 5);

                        // Load new background image
                        if (!backgroundTexture.loadFromFile("image/grotte.jpg")) {
                            std::cerr << "Erreur : Impossible de charger l'image grotte.jpg !" << std::endl;
                        }
                        backgroundSprite.setTexture(backgroundTexture);

                        showbarbaremagicien = false;
                        sf::RectangleShape quitButton(sf::Vector2f(200, 50));
                        quitButton.setFillColor(sf::Color::Red);
                        quitButton.setPosition(videoMode.width - 220, videoMode.height - 70); // Position en bas à droite

                        
                    }
                    
                }
                if (quitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) || quitButtonn.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                    window.close();
                }
            }
        }

        window.clear();
        window.draw(backgroundSprite); // Draw the background image
        if (showbarbaremagicien) {
            window.draw(text); // Draw the text on top of the background
            window.draw(barbareButton); // Draw the barbare button
            window.draw(barbareButtonText); // Draw the barbare button text
            window.draw(magicienButton); // Draw the Magicien button
            window.draw(magicienButtonText); // Draw the Magicien button text
            window.draw(quitButton); // Draw the quit button
            window.draw(quitButtonText); // Draw the quit button text
        }
        if (!showbarbaremagicien) {
            window.draw(quitButtonn); // Draw the quit button
            window.draw(quitButtonnText); // Draw the quit button text
        }

        window.display();
    }

    return 0;
}























int main() {
    int choixInterface;
    cout << "Choisissez votre interface :" << endl;
    cout << "1. Interface Graphique" << endl;
    cout << "2. Interface Console" << endl;
    cin >> choixInterface;
    if (choixInterface == 1) {
        interface();
    } else {
        cout<<endl;
        cout << "Dans un monde ravagé par la guerre et la corruption, les anciennes légendes parlent d’un artefact perdu, la Pierre de Lumen, capable de restaurer l’équilibre entre la lumière et les ténèbres. "<<endl;
        cout<< "Cet artefact est caché dans le Donjon du Néant, une forteresse maudite où résident des créatures cauchemardesques et leur maître impitoyable : Malakar, l’Ombre Éternelle."<<endl;
        cout<< "Vous incarnez un aventurier solitaire, chargé d’infiltrer ce donjon maudit pour récupérer la pierre avant que les ténèbres ne consument définitivement le royaume." << endl;
        cout<<endl;
        srand(time(nullptr));
        vector<Attaque> attaques = chargerAttaques("attaques.txt");
        vector<shared_ptr<Arme>> armes = chargerArmes("armes.txt");
        vector<shared_ptr<Armure>> armures = chargerArmures("armures.txt");

        int choixClasse;
        cout << "Choisissez votre classe :" << endl;
        cout << "1. Barbare (dégâts physiques accrus)" << endl;
        cout << "2. Magicien (dégâts magiques accrus)" << endl;
        cin >> choixClasse;

        Joueur joueur("Joueur", 100, 10, 10, 5, 5);

        // Ajouter une attaque aléatoire à l'inventaire du joueur
        if (!attaques.empty()) {
            int numero_attaque = rand() % attaques.size();
            auto attaque_heros = make_shared<AttaqueObjet>(attaques[numero_attaque]);
            joueur.ajouterObjet(attaque_heros);
        }

        shared_ptr<Arme> arme;
        shared_ptr<Armure> armure;
        if (choixClasse == 1) {
            // Barbare : bonus de dégâts physiques
            joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);
            arme = make_shared<Arme>("Épée", 5, 0);
            armure = make_shared<Armure>("Armure de l'apprenti soldat", 5, 0);
        } else if (choixClasse == 2) {
            // Magicien : bonus de dégâts magiques
            joueur.setValeurAttaqueMagique(joueur.getValeurAttaqueMagique() + 15);
            arme = make_shared<Arme>("Bâton magique", 0, 5);
            armure = make_shared<Armure>("Armure de l'apprenti magicien", 0, 5);
        } else {
            cout << "Choix invalide! Barbare choisi par défaut." << endl;
            joueur.setValeurAttaquePhysique(joueur.getValeurAttaquePhysique() + 15);
            arme = make_shared<Arme>("Épée", 5, 0);
            armure = make_shared<Armure>("Armure de l'apprenti soldat", 5, 0);
        }

        // Équiper l'arme et l'armure
        joueur.equiperArmure(armure);
        joueur.ajouterObjet(armure);
        joueur.equiperArme(arme);
        joueur.ajouterObjet(arme);

        auto potion = make_shared<Potion>();
        joueur.ajouterObjet(potion);

        int nombre_de_tours = 0;

        while (nombre_de_tours < 10) {
            // Clear the terminal
            string nomAleatoiree = nomAleatoire();
            cout << "Vous avez rencontré un " << nomAleatoiree << " sur votre chemin!" << endl;
            // Display the current turn
            cout << "Nombre de monstre rencontré: " << nombre_de_tours + 1 << endl;
            cout << "Vie restante du joueur :"<< joueur.getPointsDeVie() << endl;
            cout << "Inventaire du joueur:" << endl;
            for (const auto& objet : joueur.getInventaire()) {
                cout << "- " << objet->nom;
                if (dynamic_pointer_cast<Arme>(objet)) {
                cout << " (Arme)";
                } else if (dynamic_pointer_cast<Armure>(objet)) {
                cout << " (Armure)";
                } else if (dynamic_pointer_cast<AttaqueObjet>(objet)) {
                cout << " (Attaque)";
                } else if (dynamic_pointer_cast<Potion>(objet)) {
                cout << " (Potion)";
                }
                cout << endl;
            }
            Personnage ennemi("Ennemi", 50, 8, 8, 3, 3);

            joueur.rencontrerEnnemi(ennemi, attaques,nomAleatoiree);
            if (joueur.getPointsDeVie() == 0) {
                break;
            }
            cout<<endl;
            int chance = rand() % 4;
            if (chance == 0) {
                // Ajouter une nouvelle attaque au hasard suite au combat contre le monstre
                if (!attaques.empty()) {
                    int numero_attaque = rand() % attaques.size();
                    auto nouvelle_attaque = make_shared<AttaqueObjet>(attaques[numero_attaque]);
                    cout << "Vous avez de la chance aventurier, le monstre avait sur lui une nouvelle attaque!" << endl;
                    cout << "Vous avez obtenu : " << nouvelle_attaque->nom << endl;
                    joueur.ajouterObjet(nouvelle_attaque);
                }
            } //vous looter l'armure que cachait le monstre
            else if (chance == 1) {
                if (!armures.empty()) {
                    auto armureAleatoire = armures[rand() % armures.size()];
                    cout << "Vous avez trouvé une superbe armure qui appartenait à cette horible créature : " << armureAleatoire->nom << ", Bonus Défense Physique : " << armureAleatoire->bonusDefensePhysique<< ", Bonus Défense Magique : " << armureAleatoire->bonusDefenseMagique<< endl;
                    cout << "Stats de l'armure : " << endl;
                    if (joueur.getArmureEquipee()) {
                        cout << "Votre armure actuelle : " << joueur.getArmureEquipee()->nom << ", Bonus défense Physique : " << joueur.getArmureEquipee()->bonusDefensePhysique << ", Bonus défense Magique : " << joueur.getArmureEquipee()->bonusDefenseMagique<< endl;
                    } else {
                        cout << "Vous n'avez pas d'armure équipée actuellement." << endl;
                    }
                    cout << "Voulez-vous équiper la nouvelle armure ? (1: Oui, 0: Non)" << endl;
                    int choix;
                    cin >> choix;
                    if (choix == 1) {
                        if (joueur.getArmureEquipee()) {
                            auto& inventaire = joueur.getInventaire();
                            inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                    [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmureEquipee(); }),
                                            inventaire.end());
                            joueur.desequiperArmure();
                        }
                        joueur.ajouterObjet(armureAleatoire);
                        joueur.equiperArmure(armureAleatoire);
                        cout << "Vous avez équipé l'armure: " << armureAleatoire->nom << endl;
                    } else {
                        cout << "Vous avez décidé de ne pas équiper la nouvelle armure." << endl;
                    }
                }
            } //vous looter l'arme que cachait le monstre
            else if (chance == 2) {
                if (!armes.empty()) {
                    auto armeAleatoire = armes[rand() % armes.size()];
                    cout << "Cette horible créature cachait une incroyable arme : " << armeAleatoire->nom << ", Bonus Attaque Physique : " << armeAleatoire->bonusAttaquePhysique << ", Bonus Attaque Magique : " << armeAleatoire->bonusAttaqueMagique << endl;
                    if (joueur.getArmeEquipee()) {
                        cout << "Votre arme actuelle : " << joueur.getArmeEquipee()->nom << ", Bonus Attaque Physique : " << joueur.getArmeEquipee()->bonusAttaquePhysique << ", Bonus Attaque Magique : " << joueur.getArmeEquipee()->bonusAttaqueMagique << endl;
                    } else {
                        cout << "Vous n'avez pas d'arme équipée actuellement." << endl;
                    }
                    cout << "Voulez-vous équiper la nouvelle arme ? (1: Oui, 0: Non)" << endl;
                    int choix;
                    cin >> choix;
                    if (choix == 1) {
                        if (joueur.getArmeEquipee()) {
                            auto& inventaire = joueur.getInventaire();
                            inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                    [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmeEquipee(); }),
                                            inventaire.end());
                            joueur.desequiperArme();
                        }
                        joueur.ajouterObjet(armeAleatoire);
                        joueur.equiperArme(armeAleatoire);
                        cout << "Vous avez équipé l'arme: " << armeAleatoire->nom << endl;
                    } else {
                        cout << "Vous avez décidé de ne pas équiper la nouvelle arme." << endl;
                    }
                }
            }//trouve qqch sur le chemin
            else if (chance == 3) {
                int chance_chemin = rand() % 3;
                if (chance_chemin == 0) {
                    if (!armures.empty()) {
                        auto armureAleatoire = armures[rand() % armures.size()];
                        cout << "Vous avez trouvé une vielle armure dans une grotte : " << armureAleatoire->nom << ", Bonus Défense Physique : " << armureAleatoire->bonusDefensePhysique<< ", Bonus Défense Magique : " << armureAleatoire->bonusDefenseMagique<< endl;
                        cout << "Stats de l'armure : " << endl;
                        if (joueur.getArmureEquipee()) {
                            cout << "Votre armure actuelle : " << joueur.getArmureEquipee()->nom << ", Bonus défense Physique : " << joueur.getArmureEquipee()->bonusDefensePhysique << ", Bonus défense Magique : " << joueur.getArmureEquipee()->bonusDefenseMagique<< endl;
                        } else {
                            cout << "Vous n'avez pas d'armure équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle armure ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmureEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmureEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArmure();
                            }
                            joueur.ajouterObjet(armureAleatoire);
                            joueur.equiperArmure(armureAleatoire);
                            cout << "Vous avez équipé l'armure: " << armureAleatoire->nom << endl;
                        } else {
                            cout << "Vous avez décidé de ne pas équiper la nouvelle armure." << endl;
                        }
                    }
                }
                else if (chance_chemin == 1) {
                    if (!armes.empty()) {
                        auto armeAleatoire = armes[rand() % armes.size()];
                        cout << "Vous trébuché sur quelque chose durant votre voyage... : " << armeAleatoire->nom << ", Bonus Attaque Physique : " << armeAleatoire->bonusAttaquePhysique << ", Bonus Attaque Magique : " << armeAleatoire->bonusAttaqueMagique << endl;
                        if (joueur.getArmeEquipee()) {
                            cout << "Votre arme actuelle : " << joueur.getArmeEquipee()->nom << ", Bonus Attaque Physique : " << joueur.getArmeEquipee()->bonusAttaquePhysique << ", Bonus Attaque Magique : " << joueur.getArmeEquipee()->bonusAttaqueMagique << endl;
                        } else {
                            cout << "Vous n'avez pas d'arme équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle arme ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmeEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmeEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArme();
                            }
                            joueur.ajouterObjet(armeAleatoire);
                            joueur.equiperArme(armeAleatoire);
                            cout << "Vous avez équipé l'arme: " << armeAleatoire->nom << endl;
                        } else {
                            cout << "Vous avez décidé de ne pas équiper la nouvelle arme." << endl;
                        }
                    }  
                }
                else {
                    cout << "Sur le chemin, vous vous rappelez les conseils de votre vieux maître et préparez une potion avec des herbes médicinales. " << endl;
                    joueur.ajouterObjet(potion);            
                }
            }
            else {
                // Le joueur rencontre un ami qui lui donne un objet au choix
                cout << "Vous rencontrez un ami sur le chemin. Il vous propose de choisir un objet :" << endl;
                cout << "1. Une arme" << endl;
                cout << "2. Une armure" << endl;
                cout << "3. Une potion" << endl;
                int choix;
                cin >> choix;

                shared_ptr<Objet> nouvelObjet;
                if (choix == 1) {
                    if (!armes.empty()) {
                        auto armeAleatoire = armes[rand() % armes.size()];
                        cout << "Votre meilleur amis à cette arme : " << armeAleatoire->nom << ", Bonus Attaque Physique : " << armeAleatoire->bonusAttaquePhysique << ", Bonus Attaque Magique : " << armeAleatoire->bonusAttaqueMagique << endl;
                        if (joueur.getArmeEquipee()) {
                            cout << "Votre arme actuelle : " << joueur.getArmeEquipee()->nom << ", Bonus Attaque Physique : " << joueur.getArmeEquipee()->bonusAttaquePhysique << ", Bonus Attaque Magique : " << joueur.getArmeEquipee()->bonusAttaqueMagique << endl;
                        } else {
                            cout << "Vous n'avez pas d'arme équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle arme ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmeEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmeEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArme();
                            }
                            joueur.ajouterObjet(armeAleatoire);
                            joueur.equiperArme(armeAleatoire);
                            cout << "Vous avez équipé l'arme: " << armeAleatoire->nom << endl;
                        } else {
                            cout << "Vous avez décidé de ne pas équiper la nouvelle arme." << endl;
                        }
                    }
                } else if (choix == 2) {
                    // Créer une nouvelle armure au hasard depuis armures.txt
                    if (!armures.empty()) {
                        auto armureAleatoire = armures[rand() % armures.size()];
                        cout << "Votre fabuleux amis à cette armure : " << armureAleatoire->nom << ", Bonus Défense Physique : " << armureAleatoire->bonusDefensePhysique<< ", Bonus Défense Magique : " << armureAleatoire->bonusDefenseMagique<< endl;
                        if (joueur.getArmureEquipee()) {
                            cout << "Votre armure actuelle : " << joueur.getArmureEquipee()->nom << ", Bonus défense Physique : " << joueur.getArmureEquipee()->bonusDefensePhysique << ", Bonus défense Magique : " << joueur.getArmureEquipee()->bonusDefenseMagique<< endl;
                        } else {
                            cout << "Vous n'avez pas d'armure équipée actuellement." << endl;
                        }
                        cout << "Voulez-vous équiper la nouvelle armure ? (1: Oui, 0: Non)" << endl;
                        int choix;
                        cin >> choix;
                        if (choix == 1) {
                            if (joueur.getArmureEquipee()) {
                                auto& inventaire = joueur.getInventaire();
                                inventaire.erase(remove_if(inventaire.begin(), inventaire.end(),
                                                        [&joueur](const shared_ptr<Objet>& obj) { return obj == joueur.getArmureEquipee(); }),
                                                inventaire.end());
                                joueur.desequiperArmure();
                            }
                            joueur.ajouterObjet(armureAleatoire);
                            joueur.equiperArmure(armureAleatoire);
                            cout << "Vous avez équipé l'armure: " << armureAleatoire->nom << endl;
                        }
                } else if (choix == 3) {
                    cout << "Votre ami vous donne une potion." << endl;
                    joueur.ajouterObjet(potion);
                } else {
                    cout << "Choix invalide! Vous ne recevez aucun objet." << endl;
                }

                if (nouvelObjet) {
                    joueur.ajouterObjet(nouvelObjet);
                    cout << "Votre ami vous a donné: " << nouvelObjet->nom << endl;
                }
            }
        }
            this_thread::sleep_for(chrono::seconds(5));
            std::cout << std::string(100, '\n'); // Affiche 100 sauts de ligne
            nombre_de_tours++;
        }

        cout<<"------------------------------------------------------------"<<endl;
        cout<<"Aventurier, votre combat touche à sa fin. Soudain, l'air devient pesant et, derrière vous, apparaît le boss du donjon : Malakar, l'Ombre Éternelle."<<endl;
        cout<<"------------------------------------------------------------"<<endl;
        cout << "Vie restante du joueur :"<< joueur.getPointsDeVie() << endl;
            cout << "Inventaire du joueur:" << endl;
            for (const auto& objet : joueur.getInventaire()) {
                cout << "- " << objet->nom;
                if (dynamic_pointer_cast<Arme>(objet)) {
                cout << " (Arme)";
                } else if (dynamic_pointer_cast<Armure>(objet)) {
                cout << " (Armure)";
                } else if (dynamic_pointer_cast<AttaqueObjet>(objet)) {
                cout << " (Attaque)";
                } else if (dynamic_pointer_cast<Potion>(objet)) {
                cout << " (Potion)";
                }
                cout << endl;
            }
        Personnage ennemi("Ennemi", 100, 15, 15, 5, 5);
        joueur.rencontrerEnnemi(ennemi, attaques,"Malakar, l'Ombre Éternelle");
        auto inventaire = joueur.getInventaire();
        auto it = find_if(inventaire.begin(), inventaire.end(), [](const shared_ptr<Objet>& obj) {
            return dynamic_pointer_cast<Potion>(obj) != nullptr;
        });

        // Affiche le résultat final
        if (joueur.getPointsDeVie() <= 0) {
            cout << "Le sol tremble sous vos pieds. Malakar, l’Ombre Éternelle, bien que blessé, se redresse lentement, un sourire sinistre aux lèvres."<<endl;
            cout<<"Son regard brûlant de malice transperce votre âme. Vous tentez une dernière attaque, mais votre corps ne répond plus."<<endl;
            cout <<"L’air est devenu trop lourd, l’obscurité trop oppressante."<<endl;

            cout << 'Un rire glacial résonne dans la salle du trône. "Tu as été un adversaire valeureux, aventurier… mais ici, c’est MOI qui dicte la fin de cette histoire."'<<endl;


            cout << "Des ombres s’élèvent du sol, s’enroulant autour de vous comme des chaînes vivantes. Vous luttez, mais la force vous abandonne."<<endl;
            cout <<"Peu à peu, la lumière quitte vos yeux, votre essence absorbée par l’abîme sans fin de Malakar."<<endl;

            cout <<"Le monde extérieur ne sait rien de ce qui vient de se produire. Les villageois attendent, espérant voir leur héros revenir. Mais il ne reviendra jamais."<<endl;

            cout <<"La nuit s’étend, plus sombre et plus froide que jamais. Dans les jours qui suivent, les ténèbres s’abattent sur le royaume. "<<endl;

            cout<<"Au sommet de son trône d’ombres, Malakar règne désormais sans opposition."<<endl;

            cout<<"L’ombre a triomphé." << endl;
        } else{
                cout <<"Le silence s’installe dans la salle du trône dévastée. Le corps de Malakar, l’Ombre Éternelle, s’effondre dans un tourbillon de ténèbres, ne laissant derrière lui qu’un écho lointain de son rire maudit."<<endl;
                cout<<"Vous tenez enfin entre vos mains la Pierre de Lumen, son éclat vacillant dans l’obscurité pesante du donjon."<<endl;
                if (joueur.getPointsDeVie() > 0 && it != inventaire.end()) { //fin malveillante
                    cout << "Mais alors que vous contemplez la Pierre de Lumen, une voix résonne dans votre esprit… une promesse de pouvoir, de domination."<<endl;
                    cout<<"Pourquoi restaurer un monde faible, alors que vous pourriez le modeler à votre image ?"<<endl;

                    cout <<"Dans un murmure, vous absorbez l’essence de la pierre. Une onde d’énergie obscure vous traverse, et vos yeux brillent d’un éclat surnaturel."<<endl;
                    cout<<"Vous quittez le donjon, mais ce n’est pas la lumière que vous ramenez… c’est une nouvelle ère d’ombres qui commence."<<endl;
                    cout<<"Désormais, c’est vous qui régnez sur la nuit..." << endl;
                }
                else{
                    cout<<"Alors que vous vous apprêtez à quitter le donjon, vous sentez la noirceur de Malakar toujours présente…"<<endl;
                    cout<<"Vous comprenez alors la vérité : la pierre ne peut être contenue, elle doit être détruite."<<endl;
                    cout<<"Dans un dernier acte de courage, vous brisez la Pierre de Lumen entre vos mains. Une explosion de lumière consume tout, purifiant le mal… mais aussi vous-même."<<endl;
                    cout<<"Lorsque les villageois s’approchent du donjon, ils ne trouvent que des ruines baignées d’un éclat lunaire. Votre nom ne sera peut-être jamais chanté, mais le monde, lui, est sauvé."<<endl;
                }
        }


    }
    return 0;
}
