# ++ sfml
#https://www.youtube.com/watch?v=up5agOJNzAQ&list=PLEw78AoQh6uIvhDAw86194imjBIUPytr5&index=138
#https://www.youtube.com/watch?v=1raaJ74U3SI&list=PLEw78AoQh6uIvhDAw86194imjBIUPytr5&index=197
#https://www.youtube.com/watch?v=BgkJRLtu84o&list=PLEw78AoQh6uIvhDAw86194imjBIUPytr5&index=59
#https://www.youtube.com/watch?v=fJPjFFVprBo&list=PLEw78AoQh6uIvhDAw86194imjBIUPytr5&index=130
#https://www.youtube.com/watch?v=YfsVeTXgifk&list=PLlpyBoP2Q5yvOcZ9R_WaRcoo-NeY0JX2K&index=10
#https://www.youtube.com/watch?v=sMz04mBDK-A&list=PLlpyBoP2Q5yvOcZ9R_WaRcoo-NeY0JX2K&index=3
# Nom de l'exécutable final
TARGET = programme

# Liste des fichiers sources (ajoutez vos fichiers ici)
SOURCES = main.cpp

# Liste des dépendances (ajoutez vos fichiers d'entête ici)
HEADERS = interface.hpp 

# Compilateur et flags
CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -g 

# Objets générés automatiquement à partir des fichiers sources
OBJECTS = $(SOURCES:.cpp=.o)

# Règle par défaut
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Règle générique pour compiler les fichiers .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	rm -f $(OBJECTS) $(TARGET)

# Pour forcer la re-compilation complète
rebuild: clean all

# Indique que ces règles ne sont pas des fichiers
.PHONY: all clean rebuild