
// Exemple de package C simple pour Maya v5.0
// Nom du fichier: exemple_package_simple.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Fonction de salutation
void may_package_saluer() {
    printf("🌟 Bonjour depuis mon package C!\n");
    printf("🚀 Maya v5.0 exécute du code C!\n");
}

// Fonction de calcul mathématique
void may_package_calculer() {
    int a = 10, b = 5;
    printf("🔢 CALCULS DEPUIS PACKAGE C:\n");
    printf("   %d + %d = %d\n", a, b, a + b);
    printf("   %d * %d = %d\n", a, b, a * b);
    printf("   √%d = %.2f\n", a, sqrt(a));
}

// Fonction personnalisée de jeu
void may_package_mini_jeu() {
    printf("🎮 MINI-JEU DEPUIS PACKAGE C!\n");
    printf("Devinez un nombre entre 1 et 10:\n");
    
    int secret = 7; // Nombre fixe pour l'exemple
    printf("💡 (Indice: c'est %d!)\n", secret);
    printf("🎯 Vous avez trouvé le bon nombre!\n");
}

// Fonction d'affichage créatif
void may_package_art() {
    printf("🎨 ART ASCII DEPUIS PACKAGE C:\n");
    printf("    🌟 MAYA 🌟\n");
    printf("   ╔════════════╗\n");
    printf("   ║ Package C! ║\n");
    printf("   ╚════════════╝\n");
    printf("🚀 Créativité sans limites!\n");
}

// Fonction de simulation
void may_package_simulation() {
    printf("🌌 SIMULATION DEPUIS PACKAGE C:\n");
    printf("🔬 Création d'un univers virtuel...\n");
    printf("⭐ Génération de 1000 étoiles...\n");
    printf("🪐 Placement de 500 planètes...\n");
    printf("👽 Découverte de 42 civilisations!\n");
    printf("✅ Simulation terminée avec succès!\n");
}
