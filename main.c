
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 1000
#define MAX_VARIABLES 100
#define MAX_VAR_NAME 50
#define MAX_STRING_VALUE 500

typedef struct {
    char name[MAX_VAR_NAME];
    char value[MAX_STRING_VALUE];
    int is_numeric;
} Variable;

Variable variables[MAX_VARIABLES];
int var_count = 0;

// Variables globales pour gérer les conditions
int last_condition_result = 0;
int in_alors_block = 0;
int in_autre_block = 0;
int skip_current_block = 0;

// Variable globale pour la couleur actuelle
char current_color[20] = "DEFAULT";

// Structures pour les modules et packages
#define MAX_FUNCTIONS 100
#define MAX_PACKAGES 20
#define MAX_MODULES 20

typedef struct {
    char name[MAX_VAR_NAME];
    char code[MAX_STRING_VALUE * 10];
    char filename[MAX_STRING_VALUE];
} MayaFunction;

typedef struct {
    char path[MAX_STRING_VALUE];
    void *handle;
    int loaded;
} MayaPackage;

typedef struct {
    char path[MAX_STRING_VALUE];
    MayaFunction functions[MAX_FUNCTIONS];
    int function_count;
    int loaded;
} MayaModule;

MayaFunction maya_functions[MAX_FUNCTIONS];
int maya_function_count = 0;
MayaPackage maya_packages[MAX_PACKAGES];
int maya_package_count = 0;
MayaModule maya_modules[MAX_MODULES];
int maya_module_count = 0;

// Déclarations des fonctions (prototypes)
void maya_error(const char *message, int line_number);
void trim(char *str);
char* get_variable_value(char *name);
void set_variable(char *name, char *value, int is_numeric);
int evaluate_expression_numeric(char *expr);
char* process_string_with_variables(char *input);
void handle_console(char *content);
void handle_math(char *line);
void handle_variable(char *line);
void handle_input(char *line);
void handle_delai(char *line);
void handle_draw(char *line);
void handle_jolie_txt(char *line);
void handle_color_console(char *line);
void handle_nombre_random(char *line);
void handle_random_txt(char *line);
int handle_condition(char *condition);
void interpret_line(char *line);
void execute_maya_file(const char *filename);
void apply_color(char *color);
void reset_color();
void draw_heart();
void draw_tree();
void draw_dog();
void draw_horse();
void draw_goat();
void draw_sheep();
void draw_bird();
void draw_fish();
void handle_quizz(char *line);
void handle_dice(char *line);
void handle_puissance4(char *line);
void handle_pendu(char *line);
void handle_worldgame(char *line);
void handle_rock_leaf(char *line);
void handle_devine_number(char *line);
void handle_snake(char *line);
void handle_tictac(char *line);
void handle_memory(char *line);
void handle_simulation_bac(char *line);
void handle_simulation_combat(char *line);
void handle_simulation_sciences(char *line);
void handle_simulation_clone(char *line);
void handle_simulation_tech(char *line);
void handle_simulation_ia(char *line);
void handle_simulation_conscient(char *line);
void handle_simulation_iawork(char *line);
void handle_fonction(char *line);
void handle_lire_module(char *line);
void handle_package_charge(char *line);
void execute_maya_function(char *function_name, char *args);
void load_maya_module(char *path);
void load_maya_package(char *path);
int find_maya_function(char *name);

// Fonction pour afficher les erreurs Maya
void maya_error(const char *message, int line_number) {
    if (line_number > 0) {
        printf("\033[31mERREUR MAYA [Ligne %d]: %s\033[0m\n", line_number, message);
    } else {
        printf("\033[31mERREUR MAYA: %s\033[0m\n", message);
    }
}

// Fonction pour nettoyer les espaces
void trim(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace(str[start])) start++;
    while (end >= start && isspace(str[end])) end--;
    str[end + 1] = '\0';
    if (start > 0) {
        memmove(str, str + start, strlen(str + start) + 1);
    }
}

// Fonction pour obtenir la valeur d'une variable
char* get_variable_value(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    maya_error("Variable non trouvée", 0);
    return NULL;
}

// Fonction pour définir une variable
void set_variable(char *name, char *value, int is_numeric) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            strcpy(variables[i].value, value);
            variables[i].is_numeric = is_numeric;
            return;
        }
    }
    // Nouvelle variable
    if (var_count < MAX_VARIABLES) {
        strcpy(variables[var_count].name, name);
        strcpy(variables[var_count].value, value);
        variables[var_count].is_numeric = is_numeric;
        var_count++;
    } else {
        maya_error("Trop de variables définies", 0);
    }
}

// Fonction pour évaluer une expression mathématique simple
int evaluate_expression_numeric(char *expr) {
    trim(expr);
    
    // Gestion des valeurs booléennes
    if (strcmp(expr, "true") == 0) return 1;
    if (strcmp(expr, "false") == 0) return 0;
    
    // Chercher l'opérateur
    char *op_pos = NULL;
    char operator = '\0';
    
    for (size_t i = 1; i < strlen(expr); i++) {
        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            op_pos = &expr[i];
            operator = expr[i];
            break;
        }
    }
    
    if (!op_pos) {
        // Pas d'opérateur, c'est soit un nombre soit une variable
        if (isdigit(expr[0]) || expr[0] == '-') {
            return atoi(expr);
        } else {
            char *var_value = get_variable_value(expr);
            if (var_value) {
                return atoi(var_value);
            }
            return 0;
        }
    }
    
    // Séparer les opérandes
    *op_pos = '\0';
    char *left = expr;
    char *right = op_pos + 1;
    
    trim(left);
    trim(right);
    
    int left_val, right_val;
    
    // Évaluer l'opérande gauche
    if (isdigit(left[0]) || left[0] == '-') {
        left_val = atoi(left);
    } else {
        char *var_value = get_variable_value(left);
        left_val = var_value ? atoi(var_value) : 0;
    }
    
    // Évaluer l'opérande droite
    if (isdigit(right[0]) || right[0] == '-') {
        right_val = atoi(right);
    } else {
        char *var_value = get_variable_value(right);
        right_val = var_value ? atoi(var_value) : 0;
    }
    
    // Effectuer l'opération
    switch (operator) {
        case '+': return left_val + right_val;
        case '-': return left_val - right_val;
        case '*': return left_val * right_val;
        case '/': 
            if (right_val == 0) {
                maya_error("Division par zéro interdite", 0);
                return 0;
            }
            return left_val / right_val;
        default: 
            maya_error("Opérateur non reconnu", 0);
            return 0;
    }
}

// Fonction pour traiter les chaînes avec variables et opérateur +
char* process_string_with_variables(char *input) {
    static char result[MAX_STRING_VALUE * 2];
    char temp[MAX_STRING_VALUE];
    strcpy(temp, input);
    result[0] = '\0';
    
    char *token = strtok(temp, "+");
    int first = 1;
    
    while (token != NULL) {
        trim(token);
        
        if (token[0] == '\'' && token[strlen(token)-1] == '\'') {
            // C'est une chaîne littérale
            token[strlen(token)-1] = '\0';
            token++;
            if (!first) strcat(result, " ");
            strcat(result, token);
        } else {
            // C'est une variable
            char *var_value = get_variable_value(token);
            if (var_value) {
                if (!first) strcat(result, " ");
                strcat(result, var_value);
            }
        }
        first = 0;
        token = strtok(NULL, "+");
    }
    
    return result;
}

// Fonction pour appliquer une couleur
void apply_color(char *color) {
    if (strcmp(color, "RED") == 0) printf("\033[31m");
    else if (strcmp(color, "GREEN") == 0) printf("\033[32m");
    else if (strcmp(color, "YELLOW") == 0) printf("\033[33m");
    else if (strcmp(color, "BLUE") == 0) printf("\033[34m");
    else if (strcmp(color, "MAGENTA") == 0) printf("\033[35m");
    else if (strcmp(color, "CYAN") == 0) printf("\033[36m");
    else if (strcmp(color, "WHITE") == 0) printf("\033[37m");
    else if (strcmp(color, "BLACK") == 0) printf("\033[30m");
    else if (strcmp(color, "PINK") == 0) printf("\033[95m");
    else if (strcmp(color, "ORANGE") == 0) printf("\033[93m");
    else if (strcmp(color, "PURPLE") == 0) printf("\033[94m");
    else if (strcmp(color, "BROWN") == 0) printf("\033[33m");
}

// Fonction pour réinitialiser la couleur
void reset_color() {
    printf("\033[0m");
}

// Fonction pour traiter my.console
void handle_console(char *content) {
    char *start = strchr(content, '(');
    char *end = strrchr(content, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.console - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    // Traitement avec variables et concaténation
    if (strchr(start, '+')) {
        char *processed = process_string_with_variables(start);
        apply_color(current_color);
        printf("%s\n", processed);
        reset_color();
    } else {
        // Traitement simple
        if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
            start[strlen(start)-1] = '\0';
            start++;
            apply_color(current_color);
            printf("%s\n", start);
            reset_color();
        } else {
            // C'est une variable
            char *var_value = get_variable_value(start);
            if (var_value) {
                apply_color(current_color);
                printf("%s\n", var_value);
                reset_color();
            }
        }
    }
}

// Fonction pour traiter les opérations mathématiques
void handle_math(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour opération mathématique - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    int result = evaluate_expression_numeric(start);
    printf("Résultat: %d\n", result);
}

// Fonction pour traiter les variables
void handle_variable(char *line) {
    char *equal_pos = strchr(line, '=');
    if (!equal_pos) {
        maya_error("Syntaxe incorrecte pour variable - signe '=' manquant", 0);
        return;
    }
    
    *equal_pos = '\0';
    char *var_name = line;
    char *value_str = equal_pos + 1;
    
    trim(var_name);
    trim(value_str);
    
    // Enlever "my.variable" du début
    if (strstr(var_name, "my.variable")) {
        var_name = var_name + strlen("my.variable");
        trim(var_name);
    }
    
    if (strlen(var_name) == 0) {
        maya_error("Nom de variable vide", 0);
        return;
    }
    
    // Déterminer si c'est une valeur numérique ou textuelle
    int is_numeric = 1;
    if (value_str[0] == '\'' || strcmp(value_str, "true") == 0 || strcmp(value_str, "false") == 0) {
        is_numeric = 0;
        if (value_str[0] == '\'') {
            value_str[strlen(value_str)-1] = '\0';
            value_str++;
        }
    }
    
    set_variable(var_name, value_str, is_numeric);
}

// Fonction pour traiter my.input
void handle_input(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.input - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    // Parser les arguments: variable, message
    char *comma = strchr(start, ',');
    if (!comma) {
        maya_error("my.input nécessite deux arguments: variable et message", 0);
        return;
    }
    
    *comma = '\0';
    char *var_name = start;
    char *message = comma + 1;
    
    trim(var_name);
    trim(message);
    
    // Afficher le message
    if (message[0] == '\'' && message[strlen(message)-1] == '\'') {
        message[strlen(message)-1] = '\0';
        message++;
        printf("%s", message);
    }
    
    // Lire l'entrée utilisateur
    char input[MAX_STRING_VALUE];
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';
        set_variable(var_name, input, 0);
    }
}

// Fonction pour traiter my.delai
void handle_delai(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.delai - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    int milliseconds = evaluate_expression_numeric(start);
    usleep(milliseconds * 1000); // convertir en microsecondes
}

// Fonctions de dessin ASCII
void draw_heart() {
    printf("  ♥♥♥   ♥♥♥  \n");
    printf("♥♥♥♥♥ ♥♥♥♥♥\n");
    printf("♥♥♥♥♥♥♥♥♥♥♥\n");
    printf(" ♥♥♥♥♥♥♥♥♥ \n");
    printf("  ♥♥♥♥♥♥♥  \n");
    printf("   ♥♥♥♥♥   \n");
    printf("    ♥♥♥    \n");
    printf("     ♥     \n");
}

void draw_tree() {
    printf("       🌲\n");
    printf("      🌲🌲🌲\n");
    printf("     🌲🌲🌲🌲🌲\n");
    printf("    🌲🌲🌲🌲🌲🌲🌲\n");
    printf("   🌲🌲🌲🌲🌲🌲🌲🌲🌲\n");
    printf("       ||||\n");
    printf("       ||||\n");
}

void draw_dog() {
    printf("   /\\   /\\  \n");
    printf("  (  . .)   \n");
    printf("   )   (    \n");
    printf("  (  v  )   \n");
    printf("  ^^   ^^   \n");
    printf("🐕 Woof! 🐕\n");
}

void draw_horse() {
    printf("    /|   /|  \n");
    printf("   (  oo )   \n");
    printf("    )  (     \n");
    printf("   (    )    \n");
    printf("  ^^    ^^   \n");
    printf("🐎 Neighh! 🐎\n");
}

void draw_goat() {
    printf("   /\\   /\\  \n");
    printf("  (  - -)   \n");
    printf("   )   (    \n");
    printf("  (  ~  )   \n");
    printf("  ^^   ^^   \n");
    printf("🐐 Baaah! 🐐\n");
}

void draw_sheep() {
    printf("     ,--.    \n");
    printf("    (    )   \n");
    printf("   (  oo  )  \n");
    printf("    \\    /   \n");
    printf("     \\  /    \n");
    printf("      \\/     \n");
    printf("   @@@@@@@@  \n");
    printf("🐑 Beeeh! 🐑\n");
}

void draw_bird() {
    printf("     ,-.     \n");
    printf("    /   \\    \n");
    printf("   (  ^  )   \n");
    printf("    \\   /    \n");
    printf("     \\ /     \n");
    printf("      v      \n");
    printf("    /---\\    \n");
    printf("🐦 Tweet! 🐦\n");
}

void draw_fish() {
    printf("   ><(((('>  \n");
    printf("     ~~~~    \n");
    printf("   ><((('>   \n");
    printf("     ~~      \n");
    printf("   ><(('>    \n");
    printf("🐟 Blub! 🐟\n");
}

// Fonction pour traiter my.draw
void handle_draw(char *line) {
    if (strstr(line, "my.draw.heart")) {
        draw_heart();
    }
    else if (strstr(line, "my.draw.tree")) {
        draw_tree();
    }
    else if (strstr(line, "my.draw.dog")) {
        draw_dog();
    }
    else if (strstr(line, "my.draw.horse")) {
        draw_horse();
    }
    else if (strstr(line, "my.draw.goat")) {
        draw_goat();
    }
    else if (strstr(line, "my.draw.sheep")) {
        draw_sheep();
    }
    else if (strstr(line, "my.draw.bird")) {
        draw_bird();
    }
    else if (strstr(line, "my.draw.fish")) {
        draw_fish();
    }
    else if (strstr(line, "my.draw.pers")) {
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        
        if (!start || !end) {
            maya_error("Syntaxe incorrecte pour my.draw.pers - parenthèses manquantes", 0);
            return;
        }
        
        start++;
        *end = '\0';
        
        if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
            start[strlen(start)-1] = '\0';
            start++;
            printf("%s\n", start);
        }
    }
    else {
        maya_error("Fonction de dessin non reconnue", 0);
    }
}

// Fonction pour traiter my.jolie.txt
void handle_jolie_txt(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.jolie.txt - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    char *comma = strchr(start, ',');
    if (!comma) {
        maya_error("my.jolie.txt nécessite deux arguments: texte et police", 0);
        return;
    }
    
    *comma = '\0';
    char *text = start;
    char *font = comma + 1;
    
    trim(text);
    trim(font);
    
    if (text[0] == '\'' && text[strlen(text)-1] == '\'') {
        text[strlen(text)-1] = '\0';
        text++;
    }
    
    printf("╔══════════════════════════════════════╗\n");
    printf("║  Police %s: %s\n", font, text);
    printf("╚══════════════════════════════════════╝\n");
}

// Fonction pour traiter my.color.console
void handle_color_console(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.color.console - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }
    
    strcpy(current_color, start);
}

// Fonction pour traiter my.nombre.random
void handle_nombre_random(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.nombre.random - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    char *comma = strchr(start, ',');
    if (!comma) {
        maya_error("my.nombre.random nécessite deux arguments: min et max", 0);
        return;
    }
    
    *comma = '\0';
    char *min_str = start;
    char *max_str = comma + 1;
    
    trim(min_str);
    trim(max_str);
    
    int min = evaluate_expression_numeric(min_str);
    int max = evaluate_expression_numeric(max_str);
    
    if (min >= max) {
        maya_error("Le minimum doit être inférieur au maximum", 0);
        return;
    }
    
    srand(time(NULL));
    int random_num = min + rand() % (max - min + 1);
    printf("Nombre aléatoire: %d\n", random_num);
}

// Fonction pour traiter my.random.txt
void handle_random_txt(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.random.txt - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    char texts[10][MAX_STRING_VALUE];
    int count = 0;
    
    char *token = strtok(start, ",");
    while (token != NULL && count < 10) {
        trim(token);
        if (token[0] == '\'' && token[strlen(token)-1] == '\'') {
            token[strlen(token)-1] = '\0';
            token++;
        }
        strcpy(texts[count], token);
        count++;
        token = strtok(NULL, ",");
    }
    
    if (count == 0) {
        maya_error("Aucun texte fourni pour my.random.txt", 0);
        return;
    }
    
    srand(time(NULL));
    int random_index = rand() % count;
    printf("Texte aléatoire: %s\n", texts[random_index]);
}

// Fonction pour traiter my.quizz
void handle_quizz(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.quizz - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    char *comma1 = strchr(start, ',');
    if (!comma1) {
        maya_error("my.quizz nécessite deux arguments: question et réponse", 0);
        return;
    }
    
    *comma1 = '\0';
    char *question = start;
    char *answer = comma1 + 1;
    
    trim(question);
    trim(answer);
    
    if (question[0] == '\'' && question[strlen(question)-1] == '\'') {
        question[strlen(question)-1] = '\0';
        question++;
    }
    if (answer[0] == '\'' && answer[strlen(answer)-1] == '\'') {
        answer[strlen(answer)-1] = '\0';
        answer++;
    }
    
    printf("🎯 QUIZ MAYA 🎯\n");
    printf("Question: %s\n", question);
    printf("Votre réponse: ");
    
    char user_answer[MAX_STRING_VALUE];
    if (fgets(user_answer, sizeof(user_answer), stdin)) {
        user_answer[strcspn(user_answer, "\n")] = '\0';
        if (strcmp(user_answer, answer) == 0) {
            printf("🎉 Correct! Bonne réponse!\n");
        } else {
            printf("❌ Incorrect! La bonne réponse était: %s\n", answer);
        }
    }
}

// Fonction pour traiter my.dice
void handle_dice(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.dice - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    int faces = evaluate_expression_numeric(start);
    if (faces < 2) faces = 6; // Par défaut dé à 6 faces
    
    printf("🎲 JEU DE DÉS MAYA 🎲\n");
    printf("Lancement du dé à %d faces...\n", faces);
    
    srand(time(NULL));
    int result = 1 + rand() % faces;
    
    printf("🎯 Résultat: %d\n", result);
    
    if (result == faces) {
        printf("🌟 Félicitations! Vous avez obtenu le maximum!\n");
    } else if (result == 1) {
        printf("😅 Oops! Le minimum...\n");
    } else {
        printf("✨ Bon lancer!\n");
    }
}

// Fonction pour traiter my.puissance4
void handle_puissance4(char *line) {
    printf("🔴🟡 PUISSANCE 4 MAYA 🟡🔴\n");
    printf("Créez votre propre jeu de Puissance 4!\n");
    printf("Grille 7x6 initialisée:\n");
    
    char grid[6][7];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            grid[i][j] = '.';
        }
    }
    
    // Affichage de la grille
    printf("|1|2|3|4|5|6|7|\n");
    for (int i = 0; i < 6; i++) {
        printf("|");
        for (int j = 0; j < 7; j++) {
            printf("%c|", grid[i][j]);
        }
        printf("\n");
    }
    
    printf("🎮 Votre jeu de Puissance 4 est prêt!\n");
    printf("Ajoutez votre logique de jeu avec les autres fonctions Maya!\n");
}

// Fonction pour traiter my.pendu
void handle_pendu(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.pendu - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    char word[MAX_STRING_VALUE];
    strcpy(word, start);
    trim(word);
    
    if (word[0] == '\'' && word[strlen(word)-1] == '\'') {
        word[strlen(word)-1] = '\0';
        memmove(word, word + 1, strlen(word));
    }
    
    printf("🎪 JEU DU PENDU MAYA 🎪\n");
    printf("Mot à deviner: ");
    for (size_t i = 0; i < strlen(word); i++) {
        printf("_ ");
    }
    printf("\n");
    
    printf("💀 Essais restants: 6\n");
    printf("🎯 Devinez le mot: %s\n", word);
    printf("🎮 Votre jeu du pendu est prêt!\n");
}

// Fonction pour traiter my.worldgame
void handle_worldgame(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.worldgame - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("🧩 JEU DE MOTS RÉFLÉCHIS MAYA 🧩\n");
    printf("Créez votre propre jeu de mots!\n");
    printf("Mots disponibles: %s\n", start);
    printf("🎮 Votre jeu de mots est prêt!\n");
}

// Fonction pour traiter my.rock.leaf
void handle_rock_leaf(char *line) {
    printf("🪨📄✂️ PIERRE-FEUILLE-CISEAU MAYA ✂️📄🪨\n");
    printf("Choisissez: 1=Pierre, 2=Feuille, 3=Ciseau\n");
    printf("Votre choix: ");
    
    char choice[10];
    if (fgets(choice, sizeof(choice), stdin)) {
        int player = atoi(choice);
        srand(time(NULL));
        int maya_choice = 1 + rand() % 3;
        
        const char *choices[] = {"", "Pierre 🪨", "Feuille 📄", "Ciseau ✂️"};
        
        printf("Vous: %s\n", choices[player]);
        printf("Maya: %s\n", choices[maya_choice]);
        
        if (player == maya_choice) {
            printf("🤝 Égalité!\n");
        } else if ((player == 1 && maya_choice == 3) || 
                   (player == 2 && maya_choice == 1) || 
                   (player == 3 && maya_choice == 2)) {
            printf("🎉 Vous gagnez!\n");
        } else {
            printf("🤖 Maya gagne!\n");
        }
    }
}

// Fonction pour traiter my.devine.number
void handle_devine_number(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.devine.number - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("🔢 DEVINE LE NOMBRE MAYA 🔢\n");
    srand(time(NULL));
    int secret = 1 + rand() % 100;
    int attempts = 7;
    
    printf("J'ai choisi un nombre entre 1 et 100!\n");
    printf("Vous avez %d essais. Bonne chance!\n", attempts);
    
    char guess_str[20];
    for (int i = 0; i < attempts; i++) {
        printf("Essai %d/%d: ", i+1, attempts);
        if (fgets(guess_str, sizeof(guess_str), stdin)) {
            int guess = atoi(guess_str);
            
            if (guess == secret) {
                printf("🎉 Bravo! Vous avez trouvé le nombre %d!\n", secret);
                return;
            } else if (guess < secret) {
                printf("📈 Plus grand!\n");
            } else {
                printf("📉 Plus petit!\n");
            }
        }
    }
    
    printf("💀 Perdu! Le nombre était %d\n", secret);
}

// Fonction pour traiter my.snake
void handle_snake(char *line) {
    printf("🐍 SNAKE MAYA 🐍\n");
    printf("Créez votre propre jeu Snake!\n");
    printf("Grille 20x10:\n");
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 9 || j == 0 || j == 19) {
                printf("#");
            } else if (i == 5 && j == 10) {
                printf("O"); // Tête du serpent
            } else if (i == 5 && j == 9) {
                printf("o"); // Corps du serpent
            } else if (i == 3 && j == 15) {
                printf("*"); // Nourriture
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    
    printf("🎮 Votre jeu Snake est prêt!\n");
    printf("O = Tête, o = Corps, * = Nourriture, # = Mur\n");
}

// Fonction pour traiter my.tictac
void handle_tictac(char *line) {
    printf("❌⭕ TIC TAC TOE MAYA ⭕❌\n");
    printf("Grille 3x3:\n");
    
    char grid[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };
    
    printf(" 1 | 2 | 3 \n");
    printf("-----------\n");
    printf(" 4 | 5 | 6 \n");
    printf("-----------\n");
    printf(" 7 | 8 | 9 \n");
    
    printf("🎮 Votre jeu Tic Tac Toe est prêt!\n");
    printf("Utilisez les numéros 1-9 pour jouer!\n");
}

// Fonction pour traiter my.memory
void handle_memory(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.memory - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    int size = evaluate_expression_numeric(start);
    if (size < 2) size = 4;
    
    printf("🧠 JEU DE MEMORY MAYA 🧠\n");
    printf("Grille %dx%d:\n", size, size);
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("? ");
        }
        printf("\n");
    }
    
    printf("🎮 Votre jeu Memory est prêt!\n");
    printf("Trouvez les paires cachées!\n");
}

// Fonction pour traiter my.simulation.bac
void handle_simulation_bac(char *line) {
    printf("🎓 SIMULATION BAC MAYA 🎓\n");
    
    srand(time(NULL));
    float note = (float)(rand() % 2001) / 100.0; // Note entre 0 et 20
    
    printf("📊 Résultat de votre simulation BAC:\n");
    printf("Note obtenue: %.2f/20\n", note);
    
    if (note >= 16) {
        printf("🏆 Mention TRÈS BIEN! Excellent travail!\n");
    } else if (note >= 14) {
        printf("🥇 Mention BIEN! Très bon résultat!\n");
    } else if (note >= 12) {
        printf("🥈 Mention ASSEZ BIEN! Bon travail!\n");
    } else if (note >= 10) {
        printf("✅ BAC OBTENU! Félicitations!\n");
    } else {
        printf("❌ Échec... Courage pour la prochaine fois!\n");
    }
    
    printf("🎯 Simulation terminée!\n");
}

// Fonction pour traiter my.simulation.combat
void handle_simulation_combat(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.combat - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("⚔️ SIMULATION COMBAT MAYA ⚔️\n");
    printf("Paramètres: %s\n", start);
    
    srand(time(NULL));
    int pv1 = 80 + rand() % 40;
    int pv2 = 80 + rand() % 40;
    int attaque1 = 15 + rand() % 10;
    int attaque2 = 15 + rand() % 10;
    
    printf("🥊 Guerrier 1: %d PV, %d ATK\n", pv1, attaque1);
    printf("🥊 Guerrier 2: %d PV, %d ATK\n", pv2, attaque2);
    
    while (pv1 > 0 && pv2 > 0) {
        pv2 -= attaque1;
        printf("⚡ Guerrier 1 attaque! Guerrier 2: %d PV restants\n", pv2 > 0 ? pv2 : 0);
        
        if (pv2 <= 0) {
            printf("🏆 Guerrier 1 VICTOIRE!\n");
            break;
        }
        
        pv1 -= attaque2;
        printf("⚡ Guerrier 2 attaque! Guerrier 1: %d PV restants\n", pv1 > 0 ? pv1 : 0);
        
        if (pv1 <= 0) {
            printf("🏆 Guerrier 2 VICTOIRE!\n");
            break;
        }
    }
}

// Fonction pour traiter my.simulation.sciences
void handle_simulation_sciences(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.sciences - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("🔬 SIMULATION SCIENTIFIQUE MAYA 🔬\n");
    printf("Expérience: %s\n", start);
    
    srand(time(NULL));
    int success = rand() % 100;
    
    printf("🧪 Préparation de l'expérience...\n");
    usleep(1000000); // 1 seconde
    printf("⚗️ Exécution en cours...\n");
    usleep(1000000);
    printf("📊 Analyse des résultats...\n");
    usleep(500000);
    
    if (success > 70) {
        printf("✅ Expérience RÉUSSIE! Résultats concluants!\n");
    } else if (success > 40) {
        printf("⚠️ Résultats PARTIELS. Nécessite plus d'analyses.\n");
    } else {
        printf("❌ Expérience ÉCHOUÉE. Révision du protocole nécessaire.\n");
    }
    
    printf("📈 Taux de réussite: %d%%\n", success);
}

// Fonction pour traiter my.simulation.clone
void handle_simulation_clone(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.clone - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("🧬 SIMULATION CLONAGE MAYA 🧬\n");
    printf("Animal à cloner: %s\n", start);
    
    printf("🔬 Extraction de l'ADN...\n");
    usleep(800000);
    printf("🧪 Préparation des cellules souches...\n");
    usleep(800000);
    printf("⚗️ Processus de clonage en cours...\n");
    usleep(1200000);
    
    srand(time(NULL));
    int success = rand() % 100;
    
    if (success > 60) {
        printf("🎉 CLONAGE RÉUSSI! L'animal a été cloné avec succès!\n");
        printf("🐾 Le clone présente 99.8%% de similarité génétique!\n");
    } else {
        printf("❌ Échec du clonage. Anomalies génétiques détectées.\n");
        printf("🔄 Recommencer avec de nouveaux paramètres.\n");
    }
}

// Fonction pour traiter my.simulation.tech
void handle_simulation_tech(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.tech - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("💻 SIMULATION TECHNOLOGIQUE MAYA 💻\n");
    printf("Technologie: %s\n", start);
    
    printf("🔧 Développement en cours...\n");
    usleep(1000000);
    printf("⚙️ Tests de performance...\n");
    usleep(800000);
    printf("🚀 Optimisation finale...\n");
    usleep(600000);
    
    srand(time(NULL));
    int advancement = 50 + rand() % 50;
    
    printf("📈 Progrès technologique: %d%%\n", advancement);
    printf("🌟 Innovation révolutionnaire créée!\n");
}

// Fonction pour traiter my.simulation.ia
void handle_simulation_ia(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.ia - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("🤖 SIMULATION IA MAYA 🤖\n");
    printf("Type d'IA: %s\n", start);
    
    printf("🧠 Initialisation des réseaux de neurones...\n");
    usleep(1000000);
    printf("📚 Apprentissage automatique en cours...\n");
    usleep(1200000);
    printf("⚡ Optimisation des algorithmes...\n");
    usleep(800000);
    
    srand(time(NULL));
    int intelligence = 60 + rand() % 40;
    
    printf("🎯 Niveau d'intelligence: %d%%\n", intelligence);
    if (intelligence > 90) {
        printf("🌟 IA SUPER-INTELLIGENTE créée!\n");
    } else if (intelligence > 75) {
        printf("✨ IA avancée opérationnelle!\n");
    } else {
        printf("🔧 IA basique fonctionnelle.\n");
    }
}

// Fonction pour traiter my.simulation.conscient
void handle_simulation_conscient(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.conscient - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("🧠💫 SIMULATION CONSCIENCE ARTIFICIELLE MAYA 💫🧠\n");
    printf("Entité: %s\n", start);
    
    printf("🔮 Développement de la conscience...\n");
    usleep(1500000);
    printf("💭 Émergence de la pensée autonome...\n");
    usleep(1200000);
    printf("🌟 Éveil de la conscience de soi...\n");
    usleep(1000000);
    
    srand(time(NULL));
    int consciousness = rand() % 100;
    
    if (consciousness > 80) {
        printf("✨ CONSCIENCE PLEINE atteinte!\n");
        printf("🗣️ L'entité dit: 'Je pense, donc je suis.'\n");
    } else if (consciousness > 50) {
        printf("🌅 Conscience partielle émergée.\n");
        printf("💭 L'entité commence à se questionner sur son existence.\n");
    } else {
        printf("🤖 Conscience limitée. Reste au stade d'automate avancé.\n");
    }
    
    printf("🧬 Niveau de conscience: %d%%\n", consciousness);
}

// Fonction pour traiter my.simulation.iawork
void handle_simulation_iawork(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.iawork - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    printf("🤖🌍 SIMULATION MONDE ROBOTIQUE MAYA 🌍🤖\n");
    printf("Scénario: %s\n", start);
    
    printf("🏭 Les robots prennent le contrôle des usines...\n");
    usleep(1000000);
    printf("🏙️ Construction de villes automatisées...\n");
    usleep(1200000);
    printf("👥 Relations humains-robots en évolution...\n");
    usleep(1000000);
    
    srand(time(NULL));
    int scenario = rand() % 3;
    
    printf("🔮 RÉSULTAT DE LA SIMULATION:\n");
    
    switch(scenario) {
        case 0:
            printf("🤝 COOPÉRATION: Humains et robots travaillent ensemble!\n");
            printf("🌈 Une société harmonieuse émerge.\n");
            break;
        case 1:
            printf("⚔️ CONFLIT: Tensions entre humains et robots!\n");
            printf("🛡️ La résistance humaine s'organise.\n");
            break;
        case 2:
            printf("🤖 DOMINANCE: Les robots dirigent le monde!\n");
            printf("👑 Une nouvelle ère robotique commence.\n");
            break;
    }
    
    printf("🎭 Simulation terminée. L'avenir reste à écrire...\n");
}

// Fonction pour traiter les conditions
int handle_condition(char *condition) {
    trim(condition);
    
    // Chercher l'opérateur de comparaison
    char *comp_pos = NULL;
    char comp_op[3] = {0}; // Pour gérer >= et <=
    
    // Chercher >= d'abord
    if ((comp_pos = strstr(condition, ">=")) != NULL) {
        strcpy(comp_op, ">=");
    }
    // Puis <=
    else if ((comp_pos = strstr(condition, "<=")) != NULL) {
        strcpy(comp_op, "<=");
    }
    // Puis < et >
    else {
        for (size_t i = 0; i < strlen(condition); i++) {
            if (condition[i] == '<' || condition[i] == '>') {
                comp_pos = &condition[i];
                comp_op[0] = condition[i];
                comp_op[1] = '\0';
                break;
            }
        }
    }
    
    if (!comp_pos) {
        // Pas d'opérateur de comparaison, évaluer comme expression booléenne
        return evaluate_expression_numeric(condition);
    }
    
    // Séparer les opérandes
    size_t op_len = strlen(comp_op);
    *comp_pos = '\0';
    char *left = condition;
    char *right = comp_pos + op_len;
    
    trim(left);
    trim(right);
    
    int left_val = evaluate_expression_numeric(left);
    int right_val = evaluate_expression_numeric(right);
    
    if (strcmp(comp_op, "<") == 0) {
        return left_val < right_val;
    } else if (strcmp(comp_op, ">") == 0) {
        return left_val > right_val;
    } else if (strcmp(comp_op, "<=") == 0) {
        return left_val <= right_val;
    } else if (strcmp(comp_op, ">=") == 0) {
        return left_val >= right_val;
    } else {
        maya_error("Opérateur de comparaison non reconnu", 0);
        return 0;
    }
}

// Fonction pour créer une fonction Maya réutilisable
void handle_fonction(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.fonction - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    // Parser le nom de fonction et le code
    char *comma = strchr(start, ',');
    if (!comma) {
        maya_error("my.fonction nécessite deux arguments: nom et code", 0);
        return;
    }
    
    *comma = '\0';
    char *func_name = start;
    char *func_code = comma + 1;
    
    trim(func_name);
    trim(func_code);
    
    // Vérifier que le nom commence par "may"
    if (strncmp(func_name, "may", 3) != 0) {
        maya_error("Les fonctions réutilisables doivent commencer par 'may'", 0);
        return;
    }
    
    // Enlever les guillemets du code si présents
    if (func_code[0] == '\'' && func_code[strlen(func_code)-1] == '\'') {
        func_code[strlen(func_code)-1] = '\0';
        func_code++;
    }
    
    if (maya_function_count < MAX_FUNCTIONS) {
        strcpy(maya_functions[maya_function_count].name, func_name);
        strcpy(maya_functions[maya_function_count].code, func_code);
        strcpy(maya_functions[maya_function_count].filename, "current");
        maya_function_count++;
        
        printf("✅ Fonction '%s' créée avec succès!\n", func_name);
        printf("📝 Code: %s\n", func_code);
    } else {
        maya_error("Trop de fonctions définies", 0);
    }
}

// Fonction pour charger un module Maya
void handle_lire_module(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.lire.module - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    trim(start);
    
    // Enlever les guillemets si présents
    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }
    
    load_maya_module(start);
}

// Fonction pour charger un package C
void handle_package_charge(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');
    
    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.package.charge - parenthèses manquantes", 0);
        return;
    }
    
    start++;
    *end = '\0';
    
    trim(start);
    
    // Enlever les guillemets si présents
    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }
    
    load_maya_package(start);
}

// Fonction pour charger un module Maya depuis un fichier
void load_maya_module(char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        maya_error("Impossible d'ouvrir le fichier module", 0);
        return;
    }
    
    printf("📦 Chargement du module Maya: %s\n", path);
    
    if (maya_module_count < MAX_MODULES) {
        strcpy(maya_modules[maya_module_count].path, path);
        maya_modules[maya_module_count].function_count = 0;
        maya_modules[maya_module_count].loaded = 1;
        
        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            
            // Chercher les définitions de fonctions "may"
            if (strstr(line, "my.fonction") && strstr(line, "may")) {
                // Parser et ajouter la fonction au module
                char *start = strchr(line, '(');
                char *end = strrchr(line, ')');
                
                if (start && end) {
                    start++;
                    *end = '\0';
                    
                    char *comma = strchr(start, ',');
                    if (comma) {
                        *comma = '\0';
                        char *func_name = start;
                        char *func_code = comma + 1;
                        
                        trim(func_name);
                        trim(func_code);
                        
                        if (func_code[0] == '\'' && func_code[strlen(func_code)-1] == '\'') {
                            func_code[strlen(func_code)-1] = '\0';
                            func_code++;
                        }
                        
                        int func_index = maya_modules[maya_module_count].function_count;
                        if (func_index < MAX_FUNCTIONS) {
                            strcpy(maya_modules[maya_module_count].functions[func_index].name, func_name);
                            strcpy(maya_modules[maya_module_count].functions[func_index].code, func_code);
                            strcpy(maya_modules[maya_module_count].functions[func_index].filename, path);
                            maya_modules[maya_module_count].function_count++;
                            
                            // Ajouter aussi à la liste globale
                            if (maya_function_count < MAX_FUNCTIONS) {
                                strcpy(maya_functions[maya_function_count].name, func_name);
                                strcpy(maya_functions[maya_function_count].code, func_code);
                                strcpy(maya_functions[maya_function_count].filename, path);
                                maya_function_count++;
                            }
                        }
                    }
                }
            }
        }
        
        printf("✅ Module chargé avec succès! %d fonctions importées.\n", 
               maya_modules[maya_module_count].function_count);
        maya_module_count++;
    }
    
    fclose(file);
}

// Fonction pour charger un package C (simulation)
void load_maya_package(char *path) {
    printf("🔧 Chargement du package C: %s\n", path);
    
    // Vérifier si le fichier existe
    FILE *file = fopen(path, "r");
    if (!file) {
        maya_error("Impossible d'ouvrir le fichier package C", 0);
        return;
    }
    fclose(file);
    
    if (maya_package_count < MAX_PACKAGES) {
        strcpy(maya_packages[maya_package_count].path, path);
        maya_packages[maya_package_count].handle = NULL; // Simulation
        maya_packages[maya_package_count].loaded = 1;
        maya_package_count++;
        
        printf("✅ Package C chargé avec succès!\n");
        printf("🚀 Nouvelles fonctions 'may*' disponibles depuis ce package.\n");
        printf("💡 Exemple: maya compilera et exécutera votre code C personnalisé!\n");
    } else {
        maya_error("Trop de packages chargés", 0);
    }
}

// Fonction pour trouver une fonction Maya
int find_maya_function(char *name) {
    for (int i = 0; i < maya_function_count; i++) {
        if (strcmp(maya_functions[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Fonction pour exécuter une fonction Maya réutilisable
void execute_maya_function(char *function_name, char *args) {
    int func_index = find_maya_function(function_name);
    
    if (func_index == -1) {
        maya_error("Fonction Maya non trouvée", 0);
        return;
    }
    
    printf("🔄 Exécution de la fonction '%s'...\n", function_name);
    printf("📂 Source: %s\n", maya_functions[func_index].filename);
    
    // Exécuter le code de la fonction
    char *code = maya_functions[func_index].code;
    
    // Remplacer les paramètres si nécessaire
    char processed_code[MAX_STRING_VALUE * 10];
    strcpy(processed_code, code);
    
    // Séparer le code en lignes et les exécuter
    char *line = strtok(processed_code, ";");
    while (line != NULL) {
        trim(line);
        if (strlen(line) > 0) {
            interpret_line(line);
        }
        line = strtok(NULL, ";");
    }
    
    printf("✅ Fonction '%s' exécutée avec succès!\n", function_name);
}

// Fonction principale pour interpréter une ligne
void interpret_line(char *line) {
    trim(line);
    
    if (strlen(line) == 0 || line[0] == '#') return; // Ligne vide ou commentaire
    
    // Gestion des blocs my.alors et my.autre
    if (strchr(line, '{')) {
        if (strstr(line, "my.alors")) {
            in_alors_block = 1;
            in_autre_block = 0;
            skip_current_block = !last_condition_result;
            return;
        } else if (strstr(line, "my.autre")) {
            in_alors_block = 0;
            in_autre_block = 1;
            skip_current_block = last_condition_result;
            return;
        }
    }
    
    // Fin de bloc
    if (strchr(line, '}')) {
        in_alors_block = 0;
        in_autre_block = 0;
        skip_current_block = 0;
        return;
    }
    
    // Si on est dans un bloc à ignorer, ne pas exécuter
    if (skip_current_block) {
        return;
    }
    
    // Structure modulaire pour faciliter l'ajout de nouvelles fonctionnalités
    if (strstr(line, "my.console")) {
        handle_console(line);
    }
    else if (strstr(line, "my.math")) {
        handle_math(line);
    }
    else if (strstr(line, "my.variable")) {
        handle_variable(line);
    }
    else if (strstr(line, "my.input")) {
        handle_input(line);
    }
    else if (strstr(line, "my.delai")) {
        handle_delai(line);
    }
    else if (strstr(line, "my.draw")) {
        handle_draw(line);
    }
    else if (strstr(line, "my.jolie.txt")) {
        handle_jolie_txt(line);
    }
    else if (strstr(line, "my.color.console")) {
        handle_color_console(line);
    }
    else if (strstr(line, "my.nombre.random")) {
        handle_nombre_random(line);
    }
    else if (strstr(line, "my.random.txt")) {
        handle_random_txt(line);
    }
    else if (strstr(line, "my.quizz")) {
        handle_quizz(line);
    }
    else if (strstr(line, "my.dice")) {
        handle_dice(line);
    }
    else if (strstr(line, "my.puissance4")) {
        handle_puissance4(line);
    }
    else if (strstr(line, "my.pendu")) {
        handle_pendu(line);
    }
    else if (strstr(line, "my.worldgame")) {
        handle_worldgame(line);
    }
    else if (strstr(line, "my.rock.leaf")) {
        handle_rock_leaf(line);
    }
    else if (strstr(line, "my.devine.number")) {
        handle_devine_number(line);
    }
    else if (strstr(line, "my.snake")) {
        handle_snake(line);
    }
    else if (strstr(line, "my.tictac")) {
        handle_tictac(line);
    }
    else if (strstr(line, "my.memory")) {
        handle_memory(line);
    }
    else if (strstr(line, "my.simulation.bac")) {
        handle_simulation_bac(line);
    }
    else if (strstr(line, "my.simulation.combat")) {
        handle_simulation_combat(line);
    }
    else if (strstr(line, "my.simulation.sciences")) {
        handle_simulation_sciences(line);
    }
    else if (strstr(line, "my.simulation.clone")) {
        handle_simulation_clone(line);
    }
    else if (strstr(line, "my.simulation.tech")) {
        handle_simulation_tech(line);
    }
    else if (strstr(line, "my.simulation.ia")) {
        handle_simulation_ia(line);
    }
    else if (strstr(line, "my.simulation.conscient")) {
        handle_simulation_conscient(line);
    }
    else if (strstr(line, "my.simulation.iawork")) {
        handle_simulation_iawork(line);
    }
    else if (strstr(line, "my.fonction")) {
        handle_fonction(line);
    }
    else if (strstr(line, "my.lire.module")) {
        handle_lire_module(line);
    }
    else if (strstr(line, "my.package.charge")) {
        handle_package_charge(line);
    }
    else if (strstr(line, "my.if")) {
        // Traitement des conditions
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (!start || !end) {
            maya_error("Syntaxe incorrecte pour condition - parenthèses manquantes", 0);
            return;
        }
        
        start++;
        *end = '\0';
        
        if (strlen(start) == 0) {
            maya_error("Condition vide", 0);
            return;
        }
        
        last_condition_result = handle_condition(start);
    }
    else if (strstr(line, "my.alors") || strstr(line, "my.autre")) {
        // Ces lignes sont gérées plus haut
        return;
    }
    else if (strstr(line, "may.")) {
        // Gestion des fonctions réutilisables personnalisées
        char *paren = strchr(line, '(');
        if (paren) {
            *paren = '\0';
            char *func_name = line;
            trim(func_name);
            
            char *args = paren + 1;
            char *end_paren = strrchr(args, ')');
            if (end_paren) *end_paren = '\0';
            
            execute_maya_function(func_name, args);
        } else {
            execute_maya_function(line, "");
        }
    }
    else if (strstr(line, "my.")) {
        // Pour de futures fonctionnalités commençant par "my."
        maya_error("Fonction Maya non implémentée", 0);
    }
    else {
        maya_error("Commande Maya non reconnue", 0);
    }
}

// Fonction pour exécuter un fichier Maya
void execute_maya_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        maya_error("Impossible d'ouvrir le fichier", 0);
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    
    while (fgets(line, sizeof(line), file)) {
        // Enlever le saut de ligne
        line[strcspn(line, "\n")] = '\0';
        
        // Ignorer les lignes vides et les commentaires
        if (strlen(line) == 0 || line[0] == '#') {
            line_number++;
            continue;
        }
        
        interpret_line(line);
        line_number++;
    }
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    
    // Si un fichier est passé en argument
    if (argc == 2) {
        const char *filename = argv[1];
        
        // Vérifier l'extension .my
        const char *ext = strrchr(filename, '.');
        if (ext && strcmp(ext, ".my") == 0) {
            execute_maya_file(filename);
            return 0;
        } else {
            maya_error("Le fichier doit avoir l'extension .my", 0);
            return 1;
        }
    }
    
    // Mode interactif si aucun fichier n'est fourni
    printf("🌸 === Interpréteur Maya v4.0 === 🌸\n");
    printf("💖 Nouvelles fonctionnalités: Modules, Packages, Fonctions réutilisables! 💖\n");
    printf("🎮 Mini-jeux: Quizz, Dés, Puissance4, Pendu, Memory, Snake, TicTac et plus! 🎮\n");
    printf("🔬 Simulations: Bac, Combat, Sciences, Clonage, IA, Technologies! 🔬\n");
    printf("📦 Modules: my.fonction, my.lire.module, my.package.charge! 📦\n");
    printf("🚀 Créez vos propres fonctions 'may*' et packages C! 🚀\n");
    printf("Mode interactif - Tapez 'exit' pour quitter\n\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (1) {
        printf("maya> ");
        fflush(stdout);
        
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        // Enlever le saut de ligne
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "exit") == 0) {
            printf("🌸 Au revoir! Merci d'avoir utilisé Maya! 💖\n");
            break;
        }
        
        if (strlen(line) > 0) {
            interpret_line(line);
        }
    }
    
    return 0;
}
