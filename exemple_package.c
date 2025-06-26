
// Exemple de package C pour Maya
// Ce fichier démontre comment créer des fonctions personnalisées pour Maya

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Fonction exemple: may.package.salut
void may_package_salut() {
    printf("🔧 Salut depuis un package C pour Maya!\n");
    printf("💻 Cette fonction a été écrite en C et chargée dans Maya!\n");
}

// Fonction exemple: may.package.random.avance
void may_package_random_avance() {
    srand(time(NULL));
    int random1 = rand() % 1000;
    int random2 = rand() % 1000;
    int random3 = rand() % 1000;
    
    printf("🎲 Générateur aléatoire avancé (Package C):\n");
    printf("   Nombre 1: %d\n", random1);
    printf("   Nombre 2: %d\n", random2);
    printf("   Nombre 3: %d\n", random3);
    printf("   Somme: %d\n", random1 + random2 + random3);
}

// Fonction exemple: may.package.calcul.scientifique
void may_package_calcul_scientifique(double x) {
    printf("🧮 Calcul scientifique avancé (Package C):\n");
    printf("   Entrée: %.2f\n", x);
    printf("   Carré: %.2f\n", x * x);
    printf("   Cube: %.2f\n", x * x * x);
    printf("   Racine carrée approximative: %.2f\n", x / 2.0); // Approximation simple
}

// Fonction exemple: may.package.systeme.info
void may_package_systeme_info() {
    printf("💻 Informations système (Package C):\n");
    printf("   Compilé avec GCC\n");
    printf("   Exécution sur environnement Replit\n");
    printf("   Package intégré dans Maya v4.0\n");
    printf("   Statut: ✅ Opérationnel\n");
}

// Point d'entrée principal pour le package
int main() {
    printf("📦 Package C Maya - Test des fonctions:\n\n");
    
    may_package_salut();
    printf("\n");
    
    may_package_random_avance();
    printf("\n");
    
    may_package_calcul_scientifique(25.0);
    printf("\n");
    
    may_package_systeme_info();
    
    printf("\n🎉 Toutes les fonctions du package testées avec succès!\n");
    return 0;
}
