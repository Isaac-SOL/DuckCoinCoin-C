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
start: startDebug
else
	OPT += -O3 -DNDEBUG
start: startRelease
endif

.PHONY: clean mrproper doc start

#informations de lancement
startDebug:
	@echo Makefile DuckCoinCoin, par Pierre Cuquel
	@echo Compilation en mode Debug

startRelease:
	@echo Makefile DuckCoinCoin, par Pierre Cuquel
	@echo Compilation en mode Release

#Création de l'exécutable
all: start $(OUTPUTDIR)/$(EXEC)

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
$(COMPILDIR)/sha256.o: sha256/sha256.h

$(COMPILDIR)/blockchain.o: util.h blockchain.h sha256/sha256.h transaction.h

$(COMPILDIR)/transaction.o: util.h transaction.h sha256/sha256.h

$(COMPILDIR)/main.o: util.h blockchain.h sha256/sha256.h transaction.h
