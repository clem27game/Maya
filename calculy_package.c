
#include <stdio.h>
#include <math.h>

// Fonction pour calculer le cube d'un nombre
void may_package_cuber() {
    double x = 3.0; // Valeur par défaut pour le test
    printf("🔢 Le cube de %.2f est: %.2f\n", x, pow(x, 3));
}

// Fonction pour calculer le carré d'un nombre
void may_package_carre() {
    double x = 4.0; // Valeur par défaut pour le test
    printf("🔢 Le carré de %.2f est: %.2f\n", x, pow(x, 2));
}

// Fonction pour calculer la médiane de deux nombres
void may_package_median() {
    double a = 5.0, b = 7.0; // Valeurs par défaut
    double result = (a + b) / 2.0;
    printf("📊 Médiane entre %.2f et %.2f: %.2f\n", a, b, result);
}

// Fonction pour calculer la moyenne de trois nombres
void may_package_moyenne() {
    double a = 4.0, b = 5.0, c = 6.0; // Valeurs par défaut
    double result = (a + b + c) / 3.0;
    printf("📊 Moyenne entre %.2f, %.2f et %.2f: %.2f\n", a, b, c, result);
}

// Fonction pour appliquer le théorème de Pythagore
void may_package_pythagore() {
    double a = 3.0, b = 4.0; // Valeurs par défaut
    double result = sqrt(pow(a, 2) + pow(b, 2));
    printf("📐 Hypoténuse pour les côtés %.2f et %.2f: %.2f\n", a, b, result);
}

// Fonction pour appliquer le théorème de Thalès
void may_package_thales() {
    double a = 2.0, b = 3.0, c = 6.0; // Valeurs par défaut
    double result = (b * c) / a;
    printf("📐 Thalès avec a=%.2f, b=%.2f, c=%.2f: %.2f\n", a, b, c, result);
}

// Point d'entrée principal pour exécuter des tests
int main() {
    printf("📦 Package Calculy - Test des fonctions:\n\n");
    
    may_package_cuber();
    may_package_carre();
    may_package_median();
    may_package_moyenne();
    may_package_pythagore();
    may_package_thales();
    
    return 0;
}
