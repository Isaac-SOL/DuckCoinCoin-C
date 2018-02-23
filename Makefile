CC=gcc
OPT=-Wall -std=c99
OPTL=
EXEC=duck_coin_coin
COMPILDIR=compiled
OUTPUTDIR=bin

SRC=$(wildcard *.c)
HDR=$(wildcard *.h)
OBJ=$(patsubst %.c,$(COMPILDIR)/%.o,$(SRC))
SHASRC=$(wildcard sha256/*.c)
SHAOBJ=$(patsubst %.c,$(COMPILDIR)/%.o,$(SHASRC))

#Options conditionnelles pour le débug
ifeq ($(DEBUG),yes)
	OPT += -g
else
	OPT += -O3 -DNDEBUG
endif

.PHONY: clean mrproper doc

#Création de l'exécutable
all: $(OUTPUTDIR)/$(EXEC)

#Création de la documentation
doc:
	doxygen documentation/DCC

#Création des fichiers avant édition de liens
$(COMPILDIR)/%.o: %.c
	$(CC) $(OPT) -c $< -o $@

#Création de l'exécutable
$(OUTPUTDIR)/$(EXEC): $(OBJ) $(SHAOBJ)
	$(CC) $(OPT) -o $@ $^ $(OPTL)

#Nettoyage
clean:
	rm -rf $(COMPILDIR)/*.o

mrproper: clean
	rm -rf $(OUTPUTDIR)/$(EXEC)

#Dépendances spécifiques
$(OUTPUTDIR)/sha256.o: sha256/sha256.h

$(OUTPUTDIR)/blockchain.o: blockchain.h sha256/sha256.h transaction.h

$(OUTPUTDIR)/transaction.o: transaction.h sha256/sha256.h

$(OUTPUTDIR)/main.o: blockchain.h sha256/sha256.h transaction.h
