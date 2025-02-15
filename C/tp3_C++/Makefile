# Nom de l'exécutable final
TARGET = programme

# Liste des fichiers sources (ajoutez vos fichiers ici)
SOURCES = main.cpp

# Liste des dépendances (ajoutez vos fichiers d'entête ici)
HEADERS = essai.hpp 

# Compilateur et flags
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -g 

# Objets générés automatiquement à partir des fichiers sources
OBJECTS = $(SOURCES:.cpp=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Règle générique pour compiler les fichiers .cpp en .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJECTS) $(TARGET)

# Pour forcer la re-compilation complète
rebuild: clean all

# Indique que ces règles ne sont pas des fichiers
.PHONY: all clean rebuild