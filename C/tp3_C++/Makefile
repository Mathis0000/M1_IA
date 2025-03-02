# Nom de l'exécutable final
TARGET = programme

# Liste des fichiers sources
SOURCES = main.cpp \
          Objet.cpp \
          Potion.cpp \
          Arme.cpp \
          Armure.cpp \
          Attaque.cpp \
          AttaqueObjet.cpp \
          Personnage.cpp \
          Joueur.cpp

# Compilateur et flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -O2 -std=c++17

# Objets générés automatiquement à partir des fichiers sources
OBJECTS = $(SOURCES:.cpp=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Règle générique pour compiler les fichiers .cpp en .o
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJECTS) $(TARGET)

# Pour forcer la re-compilation complète
rebuild: clean all

# Indique que ces règles ne sont pas des fichiers
.PHONY: all clean rebuild
