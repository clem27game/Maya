
# Guide de développement Maya

## Comment ajouter de nouvelles fonctionnalités

### Structure du code

Le code Maya est organisé de manière modulaire :

1. **Déclarations** : Toutes les fonctions sont déclarées en haut du fichier
2. **Fonctions utilitaires** : `trim()`, `maya_error()`, etc.
3. **Gestion des variables** : `get_variable_value()`, `set_variable()`
4. **Handlers** : Une fonction `handle_xxx()` pour chaque type de commande
5. **Interpréteur principal** : `interpret_line()` qui dispatche vers les handlers

### Ajouter une nouvelle fonctionnalité

#### Étape 1 : Ajouter le prototype de fonction
```c
void handle_nouvelle_fonction(char *line);
```

#### Étape 2 : Implémenter la fonction
```c
void handle_nouvelle_fonction(char *line) {
    // Votre code ici
    // Utilisez maya_error() pour les erreurs
    // Utilisez trim() pour nettoyer les chaînes
}
```

#### Étape 3 : Ajouter la condition dans interpret_line()
```c
else if (strstr(line, "my.nouvelle")) {
    handle_nouvelle_fonction(line);
}
```

### Exemples de nouvelles fonctionnalités possibles

#### 1. Fonction my.time (afficher l'heure)
```c
void handle_time(char *line) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    printf("Heure actuelle: %02d:%02d:%02d\n", 
           tm->tm_hour, tm->tm_min, tm->tm_sec);
}
```

#### 2. Fonction my.random (nombre aléatoire)
```c
void handle_random(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Parenthèses manquantes pour my.random", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    int max = evaluate_expression(start);
    printf("Nombre aléatoire: %d\n", rand() % max);
}
```

#### 3. Fonction my.file (écrire dans un fichier)
```c
void handle_file_write(char *line) {
    // Parser le nom du fichier et le contenu
    // Écrire dans le fichier
    // Gérer les erreurs
}
```

### Conventions de nommage Maya

- Toutes les fonctions commencent par `my.`
- Les sous-catégories utilisent le point : `my.math.add`, `my.file.write`
- Les noms sont en français pour rester cohérent avec l'esprit du langage

### Gestion des erreurs

Toujours utiliser `maya_error()` pour signaler les erreurs :
```c
maya_error("Description de l'erreur", numero_ligne);
```

### Tests

Testez vos nouvelles fonctionnalités en :
1. Ajoutant des exemples dans `test.my`
2. Testant en mode interactif
3. Vérifiant la gestion d'erreurs

### Compilation et test

```bash
make clean
make
./main test.my
```
