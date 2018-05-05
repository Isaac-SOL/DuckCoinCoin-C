CC=gcc
OPT=-Wall -std=c99
OPTL=-lm
EXEC=duck_coin_coin
COMPILDIR=compiled
OUTPUTDIR=bin

SRC=$(wildcard *.c)
HDR=$(wildcard *.h)
OBJ=$(patsubst %.c,$(COMPILDIR)/%.o,$(SRC))
SHASRC=$(wildcard sha256/*.c)
SHAOBJ=$(patsubst %.c,$(COMPILDIR)/%.o,$(SHASRC))
JSONSRC=$(wildcard json/*.c)
JSONOBJ=$(patsubst %.c,$(COMPILDIR)/%.o,$(JSONSRC))

#Options conditionnelles pour le débug
ifeq ($(DEBUG),yes)
	OPT += -g
start: startDebug all
else
	OPT += -O3 -DNDEBUG
start: startRelease all
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
all: $(OUTPUTDIR)/$(EXEC)

#Création de la documentation
doc:
	doxygen documentation/DCC

#Création des fichiers avant édition de liens
$(COMPILDIR)/%.o: %.c
	$(CC) $(OPT) -c $< -o $@

#Création de l'exécutable
$(OUTPUTDIR)/$(EXEC): $(OBJ) $(SHAOBJ) $(JSONOBJ)
	$(CC) $(OPT) -o $@ $^ $(OPTL)

#Nettoyage
clean:
	rm -rf $(COMPILDIR)/*.o

mrproper: clean
	rm -rf $(OUTPUTDIR)/$(EXEC)

#Dépendances spécifiques
$(COMPILDIR)/randomGen.o: sha256/sha256_utils.h blockchain.h deque.h transaction.h util.h 

$(COMPILDIR)/sha256.o: sha256/sha256.h

$(COMPILDIR)/blockchain.o: util.h blockchain.h sha256/sha256.h transaction.h json/json.h

$(COMPILDIR)/transaction.o: util.h transaction.h sha256/sha256.h

$(COMPILDIR)/main.o: util.h blockchain.h sha256/sha256.h transaction.h randomGen.h json/json.h
