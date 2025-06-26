
#!/bin/bash

# Script d'installation Maya v5.0
# Installe les commandes globales Maya

RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
BLUE='\033[34m'
PINK='\033[95m'
RESET='\033[0m'

echo -e "${PINK}🌸 === INSTALLATION MAYA v5.0 === 🌸${RESET}"

# Vérifier que le script maya existe
if [ ! -f "maya" ]; then
    echo -e "${RED}❌ Fichier 'maya' non trouvé dans le répertoire courant${RESET}"
    exit 1
fi

# Vérifier que main existe et est compilé
if [ ! -f "main" ]; then
    echo -e "${YELLOW}⚠️  Exécutable 'main' non trouvé. Compilation...${RESET}"
    if [ -f "Makefile" ]; then
        make
    else
        gcc -Wall -Wextra -std=c99 -g -D_GNU_SOURCE -O2 -o main main.c -lm -ldl -lpthread
    fi
    
    if [ ! -f "main" ]; then
        echo -e "${RED}❌ Impossible de compiler Maya${RESET}"
        exit 1
    fi
    echo -e "${GREEN}✅ Compilation réussie${RESET}"
fi

# Créer le répertoire d'installation si nécessaire
INSTALL_DIR="$HOME/.local/bin"
mkdir -p "$INSTALL_DIR"

# Copier le script maya
echo -e "${BLUE}📦 Installation du script maya...${RESET}"
cp maya "$INSTALL_DIR/maya"
chmod +x "$INSTALL_DIR/maya"

# Copier l'exécutable principal
echo -e "${BLUE}📦 Installation de l'interpréteur...${RESET}"
cp main "$INSTALL_DIR/maya-interpreter"
chmod +x "$INSTALL_DIR/maya-interpreter"

# Vérifier si le répertoire est dans le PATH
if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    echo -e "${YELLOW}⚠️  Le répertoire $INSTALL_DIR n'est pas dans votre PATH${RESET}"
    echo -e "${BLUE}💡 Ajoutez cette ligne à votre ~/.bashrc ou ~/.zshrc:${RESET}"
    echo -e "${GREEN}export PATH=\"\$HOME/.local/bin:\$PATH\"${RESET}"
    echo ""
    
    # Ajouter automatiquement au bashrc si possible
    if [ -f "$HOME/.bashrc" ]; then
        if ! grep -q "$INSTALL_DIR" "$HOME/.bashrc"; then
            echo -e "${YELLOW}📝 Ajout automatique au ~/.bashrc...${RESET}"
            echo 'export PATH="$HOME/.local/bin:$PATH"' >> "$HOME/.bashrc"
            echo -e "${GREEN}✅ PATH mis à jour dans ~/.bashrc${RESET}"
            echo -e "${BLUE}💡 Exécutez: source ~/.bashrc ou redémarrez votre terminal${RESET}"
        fi
    fi
fi

echo ""
echo -e "${GREEN}🎉 Installation terminée avec succès!${RESET}"
echo -e "${PINK}🌸 Maya v5.0 est maintenant installé globalement! 🌸${RESET}"
echo ""
echo -e "${BLUE}🚀 Commandes disponibles:${RESET}"
echo -e "  ${GREEN}maya run <fichier.my>${RESET}     - Exécuter un fichier Maya"
echo -e "  ${GREEN}maya interactive${RESET}         - Mode interactif"
echo -e "  ${GREEN}maya create <nom>${RESET}        - Créer un nouveau projet"
echo -e "  ${GREEN}maya version${RESET}             - Afficher la version"
echo -e "  ${GREEN}maya help${RESET}                - Aide complète"
echo ""
echo -e "${YELLOW}💡 Testez avec: maya version${RESET}"
