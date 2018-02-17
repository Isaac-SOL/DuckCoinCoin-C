OPT=-Wall -std=c99
OPTM=-lm
EXEC=duck_coin_coin
OUTPUTDIR=compiled

#Création de l'exécutable
all: $(EXEC)


$(OUTPUTDIR)/sha256.o: sha256/sha256.c sha256/sha256.h
	$(CC) $(OPT) sha256/sha256.c -c -o $(OUTPUTDIR)/sha256.o

$(OUTPUTDIR)/blockchain.o: blockchain.c blockchain.h sha256/sha256.h transaction.h
	$(CC) $(OPT) blockchain.c -c -o $(OUTPUTDIR)/blockchain.o

$(OUTPUTDIR)/transaction.o: transaction.c transaction.h sha256/sha256.h
	$(CC) $(OPT) transaction.c -c -o $(OUTPUTDIR)/transaction.o

$(OUTPUTDIR)/main.o: main.c blockchain.h sha256/sha256.h transaction.h
	$(CC) $(OPT) main.c -c -o $(OUTPUTDIR)/main.o

$(EXEC): $(OUTPUTDIR)/main.o $(OUTPUTDIR)/blockchain.o $(OUTPUTDIR)/sha256.o $(OUTPUTDIR)/transaction.o
	$(CC) $(OPT) $(OUTPUTDIR)/main.o $(OUTPUTDIR)/blockchain.o $(OUTPUTDIR)/sha256.o $(OUTPUTDIR)/transaction.o -o $(OUTPUTDIR)/$(EXEC) 