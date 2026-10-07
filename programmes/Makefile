# --- COMPILATEUR ET OPTIONS ---
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# LDFLAGS spécifiques
LDFLAGS_ALGO = 
LDFLAGS_ROBOT = -lwiringPi -lpthread -lm
LDFLAGS_TESTS = -lcunit

# --- DOSSIERS ---
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DOC_DIR = doc
TEST_SRC_DIR = src/tests
TEST_BIN_DIR = tests

# --- FICHIERS SOURCES ---
SRCS_ALGO = $(SRC_DIR)/main.c \
            $(SRC_DIR)/Ville.c \
            $(SRC_DIR)/Chemin.c \
            $(SRC_DIR)/pilote.c \
            $(SRC_DIR)/direction.c

SRCS_ROBOT = $(SRC_DIR)/parcours.c \
             $(SRC_DIR)/thread.c \
             $(SRC_DIR)/tcs.c \
             $(SRC_DIR)/lcd.c

# --- OBJETS ---
OBJS_ALGO = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_ALGO))
OBJS_ROBOT = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_ROBOT))

# --- EXECUTABLES ---
EXEC_ALGO = $(BIN_DIR)/cerveau
EXEC_ROBOT = $(BIN_DIR)/robot
TEST_EXEC = $(TEST_BIN_DIR)/runner_test

# --- TESTS CONFIG ---
# Objets de l'algo SAUF main.o
OBJS_TEST = $(filter-out $(OBJ_DIR)/main.o, $(OBJS_ALGO))
# Fichiers sources des tests
TEST_SRCS = $(wildcard $(TEST_SRC_DIR)/*.c)
# Objets des tests
TEST_OBJS = $(patsubst $(TEST_SRC_DIR)/%.c, $(OBJ_DIR)/tests/%.o, $(TEST_SRCS))



# --- REGLES PRINCIPALES (Placées au début) ---


.PHONY: all prg tests doc clean directories

all: prg doc $(TEST_EXEC)

prg: directories $(EXEC_ALGO) $(EXEC_ROBOT)

directories:
	@mkdir -p $(BIN_DIR) $(TEST_BIN_DIR)



# --- REGLES DE COMPILATION ---


# --- EXECUTABLES PRINCIPAUX ---
$(EXEC_ALGO): $(OBJS_ALGO)
	@echo ">>> Édition des liens (Algo) : $@"
	$(CC) $(OBJS_ALGO) -o $@ $(LDFLAGS_ALGO)

$(EXEC_ROBOT): $(OBJS_ROBOT)
	@echo ">>> Édition des liens (Robot) : $@"
	$(CC) $(OBJS_ROBOT) -o $@ $(LDFLAGS_ROBOT)

# --- OBJETS GENERIQUES (.c -> .o) ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo ">>> Compilation : $<"
	$(CC) $(CFLAGS) -c $< -o $@



# --- GESTION DES TESTS ---


# Cette règle compile les tests (ne les exécute pas)
tests: $(TEST_EXEC)
	@echo ">>> Tests compilés : $(TEST_EXEC)"

# Cette règle construit l'exécutable de test
$(TEST_EXEC): $(OBJS_TEST) $(TEST_OBJS)
	@mkdir -p $(TEST_BIN_DIR)
	@echo ">>> Édition des liens des tests..."
	$(CC) $(OBJS_TEST) $(TEST_OBJS) -o $@ $(LDFLAGS_TESTS)

# Compilation des objets de tests
$(OBJ_DIR)/tests/%.o: $(TEST_SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/tests
	@echo ">>> Compilation fichier test : $<"
	$(CC) $(CFLAGS) -c $< -o $@


# --- OUTILS (Doc & Clean) ---


doc:
	@mkdir -p $(DOC_DIR)
	@if [ -f Doxyfile ]; then \
		doxygen Doxyfile; \
		echo ">>> Documentation générée."; \
	else \
		echo "!!! Erreur: Manque Doxyfile !!!"; \
	fi

clean:
	@echo ">>> Suppression des fichiers compilés..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(DOC_DIR)/html $(DOC_DIR)/latex $(TEST_BIN_DIR)
