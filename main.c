
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
    printf("🌸 === Interpréteur Maya v2.0 === 🌸\n");
    printf("💖 Nouvelles fonctionnalités: Variables améliorées, Input, Délais, Dessins ASCII, Couleurs, Random! 💖\n");
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
