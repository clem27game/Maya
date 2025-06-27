# **🧡💖🍓 Langage De programmation Maya v6.0!❤️🐬**

![maya](http://www.image-heberg.fr/files/17507795351188297358.webp)

**Maya v6.0 est maintenant le langage de programmation le plus créatif et révolutionnaire! 🍰🪩**

Maya permet à sa communauté de développeurs d'être ultra-créatif : chatbots personnalisés, boucles avancées, palettes créatives, jeux personnalisés, histoires interactives, inventaires dynamiques et bien plus!

🛑**Maya v6.0 - L'imagination sans limites!🛑**

**🧶 Installation**

### Installation Standard
```bash
git clone https://github.com/clem27game/Maya
cd Maya
make
```

### Installation Globale (Recommandée)
Pour utiliser Maya depuis n'importe où avec `maya run <fichier>`:

```bash
git clone https://github.com/clem27game/Maya
cd Maya
make
chmod +x install_maya.sh
./install_maya.sh
```

### Utilisation
```bash
# Commandes globales (après installation globale)
maya run mon_fichier.my       # Exécuter un fichier Maya
maya interactive              # Mode interactif
maya create mon_projet        # Créer un nouveau projet
maya version                  # Afficher la version
maya help                     # Aide complète

# Commande locale (installation standard)
./main fichier.my
```

---

## **🌟 NOUVELLES FONCTIONNALITÉS v6.0 🌟**

### **🤖 1. CHATBOTS PERSONNALISÉS**

**Création de robots conversationnels:**
- `my.create.robot('NomRobot', 'mot1:réponse1,mot2:réponse2')` - Créer un chatbot
- 
**Exemple:**
```my
my.create.robot('Maya-Assistant', 'bonjour:Salut! Comment allez-vous?,aide:Je suis là pour vous aider!,bye:Au revoir!')
```

### **🔄 2. BOUCLES ET CONTRÔLE DE FLUX**

**Nouvelles structures de boucles:**
- `my.while(condition)` - Boucle tant que condition vraie
- `my.for(variable, début, fin)` - Boucle for classique
- `my.loop(nombre)` - Boucle simple avec répétitions

**Exemple:**
```my
my.for(i, 1, 5)  # Compte de 1 à 5
my.while(x < 10)  # Tant que x < 10
my.loop(3)  # Répète 3 fois
```

### **🎨 3. FONCTIONS DE CRÉATIVITÉ**

**Palettes créatives:**
- `my.palette('dessin', 'couleur')` - Créer des dessins ASCII colorés personnalisés

**Jeux personnalisés:**
- `my.owngame('concept')` - Jouer au jeu de l'histoire de maya
- 
**Exemple:**
```my
my.palette('mouton', 'ROSE')  # Mouton rose
my.palette('oiseau', 'BLEU')  # Oiseau bleu
my.palette('abeille', 'JAUNE')  # Abeille jaune
```

### **💬 4. FONCTIONS AVANCÉES**

**Modification dynamique:**
- `my.modifie.console('message_original', délai, 'message_modifié')` - Changer le texte après un délai

**Menus interactifs:**
- `my.interactive('titre')` - Créer des menus de sélection ASCII

**Système d'inventaire:**
- `my.inventaire('action', 'nom', 'type', 'valeur')` - Gérer un inventaire dynamique

**Exemple:**
```my
my.modifie.console('Chargement...', 2000, 'Chargement terminé!')
my.interactive('Menu Principal')
my.inventaire('ajouter', 'épée', 'objet', 'Épée magique +10')
my.inventaire('utiliser', 'épée')
```

### **📖 5. CRÉATION D'HISTOIRES**

**Récits personnalisés:**
- `my.histoire.recit('votre_histoire')` - Afficher votre histoire en couleur
- `my.histoire.end('fin_personnalisée')` - Terminer avec style

**Exemple:**
```my
my.histoire.recit('Il était une fois, dans un royaume de code...')
my.histoire.end('Et ils codèrent heureux pour toujours!')
```

### **🐾 6. CRÉATIONS DIVERSES**

**Simulations personnalisées:**
- `my.own.pet('histoire_animal')` - Créer votre simulation d'animal
- `my.own.titan('légende_titan')` - Créer votre épopée de titan

**Statistiques aléatoires:**
- `my.nombre.pet()` - Générer des stats d'animaux
- `my.nombre.titan()` - Générer des stats épiques

**Exemple:**
```my
my.own.pet('Mon dragon domestique Maya')
my.nombre.pet()
my.own.titan('Le titan des codes éternels')
my.nombre.titan()
```

---

## **🌟 FONCTIONNALITÉS v5.0 (CONSERVÉES) 🌟**

### **🧮 1. MATHÉMATIQUES AVANCÉES**

**Calculs statistiques:**
- `my.math.median([liste])` - Calcul de médiane
- `my.math.moyenne([liste])` - Calcul de moyenne

**Calculs de puissance:**
- `my.math.cube(nombre)` - Nombre au cube
- `my.math.carre(nombre)` - Nombre au carré
- `my.math.racine(nombre)` - Racine carrée

**Théorèmes:**
- `my.pytha(a, b)` - Théorème de Pythagore
- `my.thales(a, b, c)` - Théorème de Thalès

**Géométrie:**
- `my.math.degres(rayon)` - Calculs de cercle et degrés

**Exemple:**
```my
my.math.cube(3)        # 3³ = 27
my.math.carre(5)       # 5² = 25
my.math.racine(16)     # √16 = 4
my.pytha(3, 4)         # Hypoténuse = 5
```

### **🎵 2. FONCTIONNALITÉS MUSICALES**

- `my.rythme.convertir(signature)` - Conversion rythmique
- `my.renvoie.gamme(gamme)` - Notes d'une gamme
- `my.obtenir.gamme(notes)` - Identifier une gamme

**Exemple:**
```my
my.renvoie.gamme('Do majeur')
my.rythme.convertir('4/4 vers 3/4')
```

### **🗄️ 3. BASES DE DONNÉES MAYA**

**Création:**
- `my.db(id, données)` - Créer une base de données (ID: 8 caractères, min 4 chiffres, 2 lettres)

**Utilisation:**
- `my.send.db(id)` - Récupérer les données
- `my.util.db(id)` - Utiliser la base pour jeux/simulations
- `my.supp.db(id)` - Supprimer/nettoyer

**Exemple:**
```my
my.db('GAME1234', 'Score: 1500, Niveau: 10')
my.send.db('GAME1234')
my.util.db('GAME1234')
```

### **⚡ 4. EXÉCUTION CODE C INTÉGRÉ**

- `my.execute.c(code_c)` - Exécuter du code C directement dans Maya!

**Exemple:**
```my
my.execute.c('printf("Hello from C!"); int x = 5 + 3; printf("Result: %d", x);')
```

### **🌌 5. NOUVELLES SIMULATIONS FANTASTIQUES (Pré-definis pour narration)**

**Simulations scientifiques:**
- `my.simulation.monrobot(nom, messages)` - 
- `my.simulation.quantique(paramètres)` - Univers quantique
- `my.simulation.monunivers(nom, description)` - Votre univers
- `my.simulation.atomes(type)` - Simulations atomiques
- `my.simulation.timetravel(années)` - Voyage temporel (1-15 ans)

**Contes magiques:**
- `my.simulation.fairy(histoire)` - Conte de fée 
- `my.simulation.vampire(récit)` - Histoire de vampire
- `my.simulation.sirene(histoire)` - Récit de sirène
- `my.simulation.monster(créature)` - Histoire de monstre

**Exemple:**
```my
my.simulation.timetravel(5)
my.simulation.fairy('La princesse et le dragon codeur')
my.simulation.monrobot('MayaBot', 'Bonjour!')
```

### **📚 6. EXERCICES ÉDUCATIFS BAC**

**Création d'exercices:**
- `my.exercice.create(matière, sujet)` - Créer vos exercices

**Spécialisés BAC STMG:**
- `my.exercice.gest.pgi()` - PGI et SIC
- `my.exercice.gest.treso()` - Trésorerie, BFR, FRNG
- `my.exercice.mana()` - Management

**BAC Général:**
- `my.exercice.math()` - Mathématiques
- `my.exercice.histoire()` - Histoire-Géographie

**Exemple:**
```my
my.exercice.create('Physique', 'Calcul de vitesse')
my.exercice.gest.pgi()
my.exercice.math()
```

---

## **🌸 FONCTIONNALITÉS EXISTANTES 🌸**

### **🧭 Affichage et Console**
```my
my.console('Hello Maya!')
my.color.console('PINK')
my.delai(1000)
```

### **🔢 Variables et Mathématiques**
```my
my.variable age = 20
my.math.add(5 + 3)
my.math.sub(10 - 4)
```

### **🎨 Dessins ASCII**
```my
my.draw.heart
my.draw.tree
my.draw.dog
my.draw.pers('Mon dessin')
```

### **🎮 Mini-Jeux Complets**
```my
my.quizz('Question?', 'Réponse')
my.dice(6)
my.puissance4()
my.pendu('MAYA')
my.snake()
my.tictac()
my.memory(4)
```

### **🔬 Simulations Réalistes**
```my
my.simulation.bac()
my.simulation.combat('Guerrier vs Mage')
my.simulation.sciences('Expérience chimique')
my.simulation.clone('Chat')
my.simulation.ia('Assistant')
```

### **📦 Système de Packages C (Amélioré!)**
```my
my.package.charge('mon_package.c')
may.package.ma_fonction()
```

---

## **💻 EXEMPLES COMPLETS v6.0**

### **Exemple 1: Chatbot Interactif avec Boucles**
```my
my.console('🤖 Création d\'un assistant personnel')

# Créer un chatbot intelligent
my.create.robot('Assistant-Maya', 'aide:Je peux vous aider avec Maya!,code:Voici un exemple de code,merci:De rien!')

# Utiliser des boucles pour répéter des actions
my.for(i, 1, 3)
my.console('Initialisation système ' + i)

my.loop(2)
my.console('Tests de compatibilité...')
```

### **Exemple 2: Jeu Créatif avec Palette et Inventaire**
```my
my.console('🎮 Création d\'un jeu d\'aventure personnalisé')

# Initialiser l'inventaire
my.inventaire('ajouter', 'potion', 'objet', 'Potion de vie +50')
my.inventaire('ajouter', 'or', 'nombre', '100')

# Créer des éléments visuels colorés
my.palette('dragon', 'ROUGE')
my.palette('trésor', 'JAUNE')

# Lancer le jeu
my.owngame('L\'aventure du développeur Maya')

# Utiliser l'inventaire
my.inventaire('utiliser', 'potion')
```

### **Exemple 3: Histoire Interactive avec Modifications**
```my
my.console('📖 Création d\'une histoire interactive')

# Commencer l'histoire
my.histoire.recit('Un développeur découvrit Maya, le langage magique...')

# Modifier dynamiquement le texte
my.modifie.console('L\'histoire commence...', 1500, 'L\'aventure débute vraiment!')

# Menu interactif pour les choix
my.interactive('Choisissez votre voie')

# Terminer l'histoire
my.histoire.end('Et Maya devint le langage de tous les rêves!')
```

### **Exemple 4: Simulation Complète avec Statistiques**
```my
my.console('🐾 Simulation complète d\'animaux légendaires')

# Créer une simulation personnalisée
my.own.pet('Phoenix Maya, l\'oiseau de feu du code')

# Générer des statistiques
my.nombre.pet()

# Boucle pour multiple créatures
my.for(creature, 1, 3)
my.own.titan('Titan ' + creature + ' des algorithmes')
my.nombre.titan()

# Palette créative pour visualiser
my.palette('phoenix', 'ORANGE')
```

### **Exemple 5: Application Complète Maya v6.0**
```my
my.console('🚀 Application complète Maya v6.0')

# Interface avec menu
my.interactive('Application Maya-Creator')

# Créer un chatbot assistant
my.create.robot('Guide-Maya', 'start:Bienvenue dans Maya!,help:Tapez help pour l\'aide')

# Système d'inventaire pour les ressources
my.inventaire('ajouter', 'créativité', 'stat', '100')
my.inventaire('ajouter', 'inspiration', 'stat', '95')

# Histoire de l'application
my.histoire.recit('Cette application révolutionne la programmation créative...')

# Simulations avancées
my.own.pet('L\'esprit créatif de Maya')
my.own.titan('Le titan de l\'innovation')

# Affichage final avec modification
my.modifie.console('Chargement des fonctionnalités...', 2000, 'Maya v6.0 prêt!')

# Fin épique
my.histoire.end('L\'avenir de la programmation créative commence maintenant!')
```

## **💻 EXEMPLES COMPLETS v5.0 (CONSERVÉS)**

### **Exemple 1: Mathématiques et Sciences**
```my
my.console('🧮 Tests mathématiques avancés')

# Calculs statistiques
my.math.median([1, 3, 5, 7, 9])
my.math.moyenne([2, 4, 6, 8])

# Théorèmes
my.pytha(3, 4)  # Pythagore
my.thales(2, 3, 6)  # Thalès

# Simulation scientifique
my.simulation.atomes('Carbone')
my.simulation.quantique('Multivers')
```

### **Exemple 2: Base de Données pour Jeu**
```my
my.console('🎮 Création système de jeu avec BDD')

# Créer base de données joueur
my.db('PLAY1234', 'Pseudo: Maya, Score: 1500, Niveau: 10')

# Utiliser dans le jeu
my.util.db('PLAY1234')
my.quizz('Quel est votre pseudo?', 'Maya')

# Sauvegarder nouveau score
my.db('PLAY5678', 'Pseudo: Maya, Score: 2000, Niveau: 11')
```

### **Exemple 3: Contes Interactifs**
```my
my.console('🧚‍♀️ Génération de conte interactif')

my.variable choix_conte = 'fée'

my.if(choix_conte == 'fée')
my.alors {
    my.simulation.fairy('La princesse Maya et la programmation magique')
}
my.autre {
    my.simulation.vampire('Le développeur vampire nocturne')
}

my.simulation.timetravel(10)  # Voyage dans le futur!
```

### **Exemple 4: Exercices BAC Interactifs**
```my
my.console('📚 Système de révision BAC')

my.exercice.gest.pgi()
my.input(reponse, 'Votre réponse: ')

my.exercice.math()
my.variable note = 15

my.if(note >= 10)
my.alors {
    my.console('✅ Exercice réussi!')
    my.simulation.bac()  # Simulation de réussite
}
```

### **Exemple 5: Code C Intégré**
```my
my.console('⚡ Utilisation de C dans Maya')

# Exécuter du C directement!
my.execute.c('
    #include <math.h>
    double result = sqrt(25);
    printf("Racine de 25: %.2f\\n", result);

    for(int i = 1; i <= 5; i++) {
        printf("Maya x %d = %d\\n", i, i * 7);
    }
')

my.console('C et Maya fonctionnent ensemble!')
```

---

## **📦 PACKAGES C ULTRA-SIMPLIFIÉS v5.0**

Créer un package C pour Maya est maintenant **VRAIMENT** simple! Plus de problèmes de compilation!

### **🚀 CRÉATION EN 3 ÉTAPES:**

**1. Écrivez votre fichier C:**
```c
// mon_package.c
#include <stdio.h>
#include <math.h>

// IMPORTANT: Les fonctions doivent commencer par "may_package_"
void may_package_saluer() {
    printf("🌟 Bonjour depuis mon package!\n");
}

void may_package_calculer() {
    double result = pow(3, 4);
    printf("🔢 3^4 = %.0f\n", result);
}

void may_package_jeu() {
    printf("🎮 Mini-jeu depuis C!\n");
    printf("🎯 Vous gagnez toujours!\n");
}
```

**2. Chargez dans Maya:**
```my
my.package.charge('mon_package.c')
```

**3. Utilisez vos fonctions:**
```my
may.package.may_package_saluer()
may.package.may_package_calculer()
may.package.may_package_jeu()
```

### **✅ SYSTÈME ULTRA-ROBUSTE:**
- ✅ **Compilation automatique** avec gestion d'erreurs complète
- ✅ **Détection automatique** des fonctions
- ✅ **Messages d'aide** en cas d'erreur
- ✅ **Support complet** des bibliothèques C (math.h, string.h, etc.)
- ✅ **Chargement dynamique** sans redémarrage
- ✅ **Nettoyage automatique** des fichiers temporaires

### **🔧 RÈGLES SIMPLES:**
1. Commencez les fonctions par `may_package_`
2. Incluez `#include <stdio.h>` au début
3. Sauvegardez avec extension `.c`
4. Utilisez `my.package.charge('votre_fichier.c')`

### **💡 EXEMPLE COMPLET:**
```my
# Charger le package
my.package.charge('exemple_package_simple.c')

# Utiliser les fonctions
may.package.may_package_saluer()
may.package.may_package_calculer()

# Intégrer avec Maya
my.variable resultat = 'succès'
my.if(resultat == 'succès')
my.alors {
    may.package.may_package_jeu()
}
```

**🎯 NOUVEAU: Si erreur, Maya vous explique exactement quoi corriger!**

---

## **🎯 UTILISATION COMPLÈTE**

### **Mode Fichier:**
```bash
./Maya/main mon_programme.my
```

### **Mode Interactif:**
```bash
./main
maya> my.console('Hello Maya v5.0!')
maya> my.simulation.quantique('Test')
maya> exit
```

---

## **🌈 MAYA v5.0 - RÉCAPITULATIF COMPLET**

**📊 Statistiques Maya v6.0:**
- ✅ **65+ fonctions natives**
- ✅ **15+ mini-jeux complets + créateur de jeux**
- ✅ **25+ simulations réalistes et personnalisées**
- ✅ **Chatbots personnalisés interactifs**
- ✅ **Boucles et contrôle de flux complet**
- ✅ **Palettes créatives et dessins colorés**
- ✅ **Système d'inventaire dynamique**
- ✅ **Création d'histoires interactives**
- ✅ **Menus interactifs ASCII**
- ✅ **Modification console en temps réel**
- ✅ **Mathématiques niveau BAC+**
- ✅ **Bases de données intégrées**
- ✅ **Code C exécutable dans .my**
- ✅ **Exercices éducatifs BAC**
- ✅ **Système de packages C simplifié**
- ✅ **Variables et conditions avancées**
- ✅ **Interface créative ultime**

---

## **❤️ À propos de Maya**

**Le langage de programmation Maya a été imaginé et créé de A à Z toute seule par nekoclem (de son vrai nom Clémence Baril), née le 12 février 2007 ayant toujours rêvé de créer son propre langage de programmation.**

**Maya v6.0 est maintenant le langage de programmation le plus révolutionnaire et créatif au monde!**

**🧡 Bon codage dans l'univers Maya v6.0! 💖**

🌸 **Maya - Où l'imagination devient réalité!** 🌸