# On purge la liste des suffixes utilisés pour les règles implicites
.SUFFIXES:

# On ajoute simplement les extensions dont l'on a besoin
.SUFFIXES: .c .o

# Nom de l'exécutable
EXEC=main

# Liste des fichiers sources séparés par des espaces
SOURCES=main.c afficher.c Prim+calcul_poid.c sauvegarde_chargement.c

#Liste des fichiers objets
OBJETS=$(SOURCES:%.c=%.o)

#Compilateur et options de compilation
CCPP=gcc
LFLAGS=  -O0 -Wall -Wextra -std=c99 -g
CFLAGS= -std=c99 -g -O0 -Wall -Wextra


#R?le explicite de construction de l'ex?utable
$(EXEC):$(OBJETS) Makefile
	$(CCPP) -o  $(EXEC) $(OBJETS) $(LFLAGS)
.c.o:
	$(CCPP) $(CFLAGS) -c $< -o $@ $(LFLAGS)

clean:
	rm $(OBJETS)
clear:
	rm $(EXEC)
depend:
	sed -e "/^#DEPENDANCIES/,$$ d" Makefile >dependances
	echo "#DEPENDANCIES" >> dependances
	$(CCPP) -MM $(SOURCES) >> dependances
	cat dependances >Makefile
	rm dependances

#DEPENDANCIES
main.o: main.c afficher.h Prim+calcul_poid.h sauvegarde_chargement.h structures.h
afficher.o: afficher.c afficher.h structures.h
Prim+calcul_poid.o: Prim+calcul_poid.c Prim+calcul_poid.h structures.h
sauvegarde_chargement.o: sauvegarde_chargement.c sauvegarde_chargement.h structures.h Prim+calcul_poid.h

