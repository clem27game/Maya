#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/wait.h>
#include <math.h>

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
    char so_path[MAX_STRING_VALUE];
    void *handle;
    int loaded;
    char function_names[MAX_FUNCTIONS][MAX_VAR_NAME];
    void (*functions[MAX_FUNCTIONS])();
    int function_count;
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

// Structures pour bases de données Maya
#define MAX_DATABASES 50
#define MAX_DB_RECORDS 1000

typedef struct {
    char id[9]; // 8 caractères + \0
    char data[MAX_STRING_VALUE * 5];
    int active;
} MayaDatabase;

MayaDatabase maya_databases[MAX_DATABASES];
int maya_db_count = 0;

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
int compile_c_package(char *source_path, char *output_path);
void call_package_function(char *function_name, char *args);
int find_package_function(char *name);
int detect_package_functions(char *source_path, char function_names[][MAX_VAR_NAME]);
void get_package_info(int func_index, int *package_index, int *local_index);

// Nouvelles fonctionnalités v5.0
void handle_math_median(char *line);
void handle_math_moyenne(char *line);
void handle_math_cube(char *line);
void handle_math_carre(char *line);
void handle_math_racine(char *line);
void handle_math_degres(char *line);
void handle_thales(char *line);
void handle_pytha(char *line);
void handle_rythme_convertir(char *line);
void handle_renvoie_gamme(char *line);
void handle_obtenir_gamme(char *line);
void handle_db(char *line);
void handle_send_db(char *line);
void handle_supp_db(char *line);
void handle_util_db(char *line);
void handle_execute_c(char *line);
void handle_simulation_monrobot(char *line);
void handle_simulation_quantique(char *line);
void handle_simulation_monunivers(char *line);
void handle_simulation_atomes(char *line);
void handle_simulation_timetravel(char *line);
void handle_simulation_fairy(char *line);
void handle_simulation_vampire(char *line);
void handle_simulation_sirene(char *line);
void handle_simulation_monster(char *line);
void handle_exercice_create(char *line);
void handle_exercice_gest_pgi(char *line);
void handle_exercice_gest_treso(char *line);
void handle_exercice_math(char *line);
void handle_exercice_histoire(char *line);
void handle_exercice_mana(char *line);
int validate_db_id(char *id);
int find_database(char *id);
char* extract_c_code(char *maya_line);

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

// Nouvelles fonctions mathématiques v5.0
void handle_math_median(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.math.median - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("📊 CALCUL DE MÉDIANE MAYA 📊\n");

    // Parser les nombres séparés par des virgules
    double numbers[50];
    int count = 0;
    char *token = strtok(start, ",");
    
    while (token != NULL && count < 50) {
        trim(token);
        numbers[count] = evaluate_expression_numeric(token);
        count++;
        token = strtok(NULL, ",");
    }

    if (count == 0) {
        maya_error("Aucun nombre fourni pour le calcul de médiane", 0);
        return;
    }

    // Trier les nombres (bubble sort simple)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                double temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    double median;
    if (count % 2 == 0) {
        median = (numbers[count/2 - 1] + numbers[count/2]) / 2.0;
    } else {
        median = numbers[count/2];
    }

    printf("Nombres triés: ");
    for (int i = 0; i < count; i++) {
        printf("%.2f ", numbers[i]);
    }
    printf("\n");
    printf("Médiane calculée: %.2f\n", median);
}

void handle_math_moyenne(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.math.moyenne - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("📊 CALCUL DE MOYENNE MAYA 📊\n");

    // Parser les nombres séparés par des virgules
    double sum = 0.0;
    int count = 0;
    char *token = strtok(start, ",");
    
    while (token != NULL) {
        trim(token);
        double value = evaluate_expression_numeric(token);
        sum += value;
        count++;
        token = strtok(NULL, ",");
    }

    if (count == 0) {
        maya_error("Aucun nombre fourni pour le calcul de moyenne", 0);
        return;
    }

    double moyenne = sum / count;

    printf("Nombres: %d valeurs\n", count);
    printf("Somme: %.2f\n", sum);
    printf("Moyenne calculée: %.2f / %d = %.2f\n", sum, count, moyenne);
}

void handle_math_cube(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.math.cube - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    int number = evaluate_expression_numeric(start);
    int result = number * number * number;

    printf("🔢 CALCUL AU CUBE MAYA 🔢\n");
    printf("%d³ = %d\n", number, result);
}

void handle_math_carre(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.math.carre - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    int number = evaluate_expression_numeric(start);
    int result = number * number;

    printf("🔢 CALCUL AU CARRÉ MAYA 🔢\n");
    printf("%d² = %d\n", number, result);
}

void handle_math_racine(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.math.racine - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    int number = evaluate_expression_numeric(start);

    if (number < 0) {
        maya_error("Impossible de calculer la racine d'un nombre négatif", 0);
        return;
    }

    double result = sqrt((double)number);

    printf("🔢 CALCUL DE RACINE MAYA 🔢\n");
    printf("√%d ≈ %.2f\n", number, result);
}

void handle_math_degres(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.math.degres - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    int rayon = evaluate_expression_numeric(start);
    double circonference = 2 * 3.14159 * rayon;

    printf("🔄 CALCUL DE DEGRÉS MAYA 🔄\n");
    printf("Cercle de rayon %d:\n", rayon);
    printf("Circonférence: %.2f\n", circonference);
    printf("360° = %.2f unités\n", circonference);
}

void handle_thales(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.thales - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("📐 THÉORÈME DE THALÈS MAYA 📐\n");

    // Parser les arguments: a, b, c
    char *comma1 = strchr(start, ',');
    if (!comma1) {
        maya_error("my.thales nécessite trois arguments: a, b, c", 0);
        return;
    }

    *comma1 = '\0';
    char *a_str = start;
    char *rest = comma1 + 1;

    char *comma2 = strchr(rest, ',');
    if (!comma2) {
        maya_error("my.thales nécessite trois arguments: a, b, c", 0);
        return;
    }

    *comma2 = '\0';
    char *b_str = rest;
    char *c_str = comma2 + 1;

    trim(a_str);
    trim(b_str);
    trim(c_str);

    double a = evaluate_expression_numeric(a_str);
    double b = evaluate_expression_numeric(b_str);
    double c = evaluate_expression_numeric(c_str);

    if (a == 0) {
        maya_error("Division par zéro: a ne peut pas être 0", 0);
        return;
    }

    double result = (b * c) / a;

    printf("Si a/b = c/d, alors d = (b×c)/a\n");
    printf("Avec a=%.2f, b=%.2f, c=%.2f\n", a, b, c);
    printf("d = %.2f\n", result);
    printf("Vérification: %.2f/%.2f = %.2f/%.2f = %.4f\n", a, b, c, result, a/b);
}

void handle_pytha(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.pytha - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("📐 THÉORÈME DE PYTHAGORE MAYA 📐\n");

    // Parser les arguments: a, b
    char *comma = strchr(start, ',');
    if (!comma) {
        maya_error("my.pytha nécessite deux arguments: a et b", 0);
        return;
    }

    *comma = '\0';
    char *a_str = start;
    char *b_str = comma + 1;

    trim(a_str);
    trim(b_str);

    double a = evaluate_expression_numeric(a_str);
    double b = evaluate_expression_numeric(b_str);

    if (a < 0 || b < 0) {
        maya_error("Les côtés d'un triangle ne peuvent pas être négatifs", 0);
        return;
    }

    double c = sqrt(a*a + b*b);

    printf("a² + b² = c²\n");
    printf("Avec a=%.2f et b=%.2f\n", a, b);
    printf("c = √(%.2f² + %.2f²) = √(%.2f + %.2f) = √%.2f = %.2f\n", a, b, a*a, b*b, a*a + b*b, c);
    printf("✅ Hypoténuse calculée: %.2f\n", c);
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
    if (milliseconds > 0) {
        usleep((useconds_t)(milliseconds * 1000)); // convertir en microsecondes
    }
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
    (void)line; // Marquer le paramètre comme volontairement inutilisé

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
    (void)line; // Marquer le paramètre comme volontairement inutilisé

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
    (void)line; // Marquer le paramètre comme volontairement inutilisé

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

void handle_tictac(char *line) {
    (void)line; // Marquer le paramètre comme volontairement inutilisé

    printf("❌⭕ TIC TAC TOE MAYA ⭕❌\n");
    printf("Grille 3x3:\n");

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
    (void)line; // Marquer le paramètre comme volontairement inutilisé

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

// Fonction pour compiler un package C en bibliothèque partagée (ultra-robuste)
int compile_c_package(char *source_path, char *output_path) {
    char compile_cmd[MAX_STRING_VALUE * 2];
    char error_file[MAX_STRING_VALUE];

    // Fichier temporaire pour capturer les erreurs
    snprintf(error_file, sizeof(error_file), "/tmp/maya_compile_errors_%ld.txt", time(NULL) % 10000);

    // Compilation robuste avec tous les flags nécessaires
    snprintf(compile_cmd, sizeof(compile_cmd), 
             "gcc -shared -fPIC -rdynamic -O2 -Wall -Wextra "
             "-o %s %s -lm -ldl -lpthread -std=c99 2>%s", 
             output_path, source_path, error_file);

    printf("🔨 COMPILATION PACKAGE C MAYA v5.0 🔨\n");
    printf("📂 Source: %s\n", source_path);
    printf("📦 Output: %s\n", output_path);
    printf("🔧 Compilation avec optimisations...\n");

    // Vérifier que le fichier source existe et est lisible
    FILE *test = fopen(source_path, "r");
    if (!test) {
        printf("❌ Impossible d'accéder au fichier source!\n");
        return 0;
    }

    // Vérifier le contenu du fichier
    char first_line[256];
    if (fgets(first_line, sizeof(first_line), test)) {
        printf("📄 Première ligne détectée: %.50s%s\n", 
               first_line, strlen(first_line) > 50 ? "..." : "");
    }
    fclose(test);

    // Tentative de compilation
    int result = system(compile_cmd);

    if (result == 0) {
        printf("✅ COMPILATION RÉUSSIE!\n");

        // Vérifier que le fichier de sortie a été créé
        FILE *output_test = fopen(output_path, "r");
        if (output_test) {
            fclose(output_test);
            printf("📦 Bibliothèque partagée créée avec succès!\n");
            printf("🚀 Package C prêt pour Maya!\n");
            printf("💡 Fonctions accessibles via may.package.*\n");

            // Nettoyer le fichier d'erreurs s'il existe
            unlink(error_file);
            return 1;
        } else {
            printf("❌ Fichier de sortie non créé malgré compilation réussie\n");
            return 0;
        }
    } else {
        printf("❌ ERREUR DE COMPILATION!\n");
        printf("🔍 Analyse des erreurs...\n");

        // Lire et afficher les erreurs
        FILE *error_output = fopen(error_file, "r");
        if (error_output) {
            char error_line[256];
            printf("📜 DÉTAILS DES ERREURS:\n");
            printf("════════════════════════════════════════\n");
            while (fgets(error_line, sizeof(error_line), error_output)) {
                printf("   %s", error_line);
            }
            printf("════════════════════════════════════════\n");
            fclose(error_output);
            unlink(error_file);
        }

        printf("💡 CONSEILS POUR CORRIGER:\n");
        printf("   1. Vérifiez que toutes les fonctions commencent par 'may_package_'\n");
        printf("   2. Ajoutez #include <stdio.h> en début de fichier\n");
        printf("   3. Vérifiez la syntaxe C (points-virgules, accolades...)\n");
        printf("   4. Exemple de fonction valide:\n");
        printf("      void may_package_ma_fonction() {\n");
        printf("          printf(\"Hello from package!\\n\");\n");
        printf("      }\n");

        return 0;
    }
}

// Fonction pour détecter automatiquement les fonctions dans un fichier C
int detect_package_functions(char *source_path, char function_names[][MAX_VAR_NAME]) {
    FILE *file = fopen(source_path, "r");
    if (!file) return 0;

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < MAX_FUNCTIONS) {
        // Chercher les définitions de fonctions may_package_*
        if (strstr(line, "void may_package_") || strstr(line, "int may_package_") || 
            strstr(line, "double may_package_") || strstr(line, "char* may_package_")) {

            // Extraire le nom de la fonction
            char *start = strstr(line, "may_package_");
            if (start) {
                char *end = strchr(start, '(');
                if (end) {
                    int len = end - start;
                    if (len < MAX_VAR_NAME) {
                        strncpy(function_names[count], start, len);
                        function_names[count][len] = '\0';
                        count++;
                        printf("🔍 Fonction détectée: %s\n", function_names[count-1]);
                    }
                }
            }
        }
    }

    fclose(file);
    return count;
}

// Fonctions pour rythme et gammes
void handle_rythme_convertir(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.rythme.convertir - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🎵 CONVERTISSEUR RYTHMIQUE MAYA 🎵\n");
    printf("Conversion: %s\n", start);
    printf("4/4 → 3/4: Tempo ajusté de 120 BPM à 90 BPM\n");
    printf("🎼 Conversion rythmique terminée!\n");
}

void handle_renvoie_gamme(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.renvoie.gamme - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🎼 GAMME MUSICALE MAYA 🎼\n");
    printf("Gamme demandée: %s\n", start);

    if (strstr(start, "Do") || strstr(start, "C")) {
        printf("Notes de la gamme de Do majeur:\n");
        printf("Do - Ré - Mi - Fa - Sol - La - Si - Do\n");
        printf("C  - D  - E  - F  - G   - A  - B  - C\n");
    } else {
        printf("Gamme générique: Do - Ré - Mi - Fa - Sol - La - Si\n");
    }
}

void handle_obtenir_gamme(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.obtenir.gamme - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🎼 IDENTIFICATION DE GAMME MAYA 🎼\n");
    printf("Notes analysées: %s\n", start);
    printf("Gamme identifiée: Do majeur (C Major)\n");
    printf("Type: Gamme diatonique majeure\n");
}

// Fonctions pour bases de données
int validate_db_id(char *id) {
    if (strlen(id) != 8) return 0;

    int digits = 0, letters = 0;
    for (int i = 0; i < 8; i++) {
        if (isdigit(id[i])) digits++;
        else if (isalpha(id[i])) letters++;
    }

    return (digits >= 4 && letters >= 2);
}

int find_database(char *id) {
    for (int i = 0; i < maya_db_count; i++) {
        if (strcmp(maya_databases[i].id, id) == 0 && maya_databases[i].active) {
            return i;
        }
    }
    return -1;
}

void handle_db(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.db - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    // Parser les arguments: id, données
    char *comma = strchr(start, ',');
    if (!comma) {
        maya_error("my.db nécessite deux arguments: id et données", 0);
        return;
    }

    *comma = '\0';
    char *db_id = start;
    char *data = comma + 1;

    trim(db_id);
    trim(data);

    // Enlever les guillemets
    if (db_id[0] == '\'' && db_id[strlen(db_id)-1] == '\'') {
        db_id[strlen(db_id)-1] = '\0';
        db_id++;
    }
    if (data[0] == '\'' && data[strlen(data)-1] == '\'') {
        data[strlen(data)-1] = '\0';
        data++;
    }

    if (!validate_db_id(db_id)) {
        maya_error("ID base de données invalide (8 caractères: min 4 chiffres, 2 lettres)", 0);
        return;
    }

    if (maya_db_count < MAX_DATABASES) {
        strcpy(maya_databases[maya_db_count].id, db_id);
        strcpy(maya_databases[maya_db_count].data, data);
        maya_databases[maya_db_count].active = 1;
        maya_db_count++;

        printf("🗄️ BASE DE DONNÉES MAYA CRÉÉE 🗄️\n");
        printf("ID: %s\n", db_id);
        printf("Données stockées: %s\n", data);
        printf("✅ Base de données active!\n");
    } else {
        maya_error("Trop de bases de données créées", 0);
    }
}

void handle_send_db(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.send.db - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    trim(start);
    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }

    int db_index = find_database(start);
    if (db_index == -1) {
        maya_error("Base de données non trouvée", 0);
        return;
    }

    printf("📤 RÉCUPÉRATION BASE DE DONNÉES MAYA 📤\n");
    printf("ID: %s\n", maya_databases[db_index].id);
    printf("Données: %s\n", maya_databases[db_index].data);
    printf("Statut: Active\n");
}

void handle_supp_db(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.supp.db - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    trim(start);
    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }

    int db_index = find_database(start);
    if (db_index == -1) {
        maya_error("Base de données non trouvée", 0);
        return;
    }

    maya_databases[db_index].active = 0;
    strcpy(maya_databases[db_index].data, "");

    printf("🗑️ BASE DE DONNÉES SUPPRIMÉE 🗑️\n");
    printf("ID: %s\n", start);
    printf("✅ Base de données nettoyée!\n");
}

void handle_util_db(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.util.db - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    trim(start);
    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }

    int db_index = find_database(start);
    if (db_index == -1) {
        maya_error("Base de données non trouvée", 0);
        return;
    }

    printf("💾 UTILISATION BASE DE DONNÉES MAYA 💾\n");
    printf("ID: %s\n", maya_databases[db_index].id);
    printf("Données disponibles: %s\n", maya_databases[db_index].data);
    printf("🎮 Base de données prête pour vos jeux et simulations!\n");
}

// Fonction pour charger un package C réel (améliorée)
void load_maya_package(char *path) {
    printf("🔧 CHARGEMENT PACKAGE C MAYA SIMPLIFIÉ 🔧\n");
    printf("📂 Fichier source: %s\n", path);

    // Vérifier si le fichier source existe
    FILE *file = fopen(path, "r");
    if (!file) {
        printf("❌ Fichier source introuvable: %s\n", path);
        printf("💡 Conseil: Vérifiez le nom et l'emplacement du fichier\n");
        return;
    }
    fclose(file);

    if (maya_package_count >= MAX_PACKAGES) {
        printf("❌ Limite de packages atteinte (%d max)\n", MAX_PACKAGES);
        return;
    }

    // Générer le nom de la bibliothèque partagée unique
    char so_path[MAX_STRING_VALUE];
    snprintf(so_path, sizeof(so_path), "./maya_pkg_%d_%ld.so", 
             maya_package_count, time(NULL) % 10000);

    printf("🔨 Compilation en cours...\n");

    // Compiler le package C avec options étendues
    if (!compile_c_package(path, so_path)) {
        printf("❌ COMPILATION ÉCHOUÉE!\n");
        printf("🔍 Solutions possibles:\n");
        printf("   1. Vérifiez la syntaxe C\n");
        printf("   2. Assurez-vous que les fonctions commencent par 'may_package_'\n");
        printf("   3. Incluez les en-têtes nécessaires (#include <stdio.h>)\n");
        return;
    }

    // Charger la bibliothèque partagée
    printf("📦 Chargement de la bibliothèque...\n");
    void *handle = dlopen(so_path, RTLD_NOW | RTLD_GLOBAL);
    if (!handle) {
        printf("❌ Erreur de chargement dynamique:\n");
        printf("   %s\n", dlerror());
        printf("🔧 Le package a été compilé mais ne peut pas être chargé\n");
        return;
    }

    // Initialiser le package
    strcpy(maya_packages[maya_package_count].path, path);
    strcpy(maya_packages[maya_package_count].so_path, so_path);
    maya_packages[maya_package_count].handle = handle;
    maya_packages[maya_package_count].loaded = 1;
    maya_packages[maya_package_count].function_count = 0;

    // Détecter automatiquement les fonctions dans le fichier source
    printf("🔍 Détection automatique des fonctions...\n");
    char detected_functions[MAX_FUNCTIONS][MAX_VAR_NAME];
    int detected_count = detect_package_functions(path, detected_functions);

    if (detected_count == 0) {
        printf("⚠️ Aucune fonction détectée!\n");
        printf("💡 Les fonctions doivent commencer par 'may_package_'\n");
        printf("   Exemple: void may_package_ma_fonction() { ... }\n");
    }

    // Charger les fonctions détectées
    int loaded_count = 0;
    for (int i = 0; i < detected_count; i++) {
        void (*func)() = dlsym(handle, detected_functions[i]);
        if (func) {
            strcpy(maya_packages[maya_package_count].function_names[loaded_count], 
                   detected_functions[i]);
            maya_packages[maya_package_count].functions[loaded_count] = func;
            loaded_count++;
            printf("✅ Fonction chargée: %s\n", detected_functions[i]);
        } else {
            printf("⚠️ Fonction '%s' trouvée mais non chargeable: %s\n", 
                   detected_functions[i], dlerror());
        }
    }

    maya_packages[maya_package_count].function_count = loaded_count;

    printf("🎉 PACKAGE C CHARGÉ AVEC SUCCÈS!\n");
    printf("📊 Statistiques:\n");
    printf("   - Fonctions détectées: %d\n", detected_count);
    printf("   - Fonctions chargées: %d\n", loaded_count);
    printf("💡 Utilisez: may.package.nom_fonction()\n");
    printf("🚀 Package prêt à l'emploi!\n");

    maya_package_count++;
}

// Fonction pour trouver une fonction dans les packages
int find_package_function(char *name) {
    for (int i = 0; i < maya_package_count; i++) {
        if (!maya_packages[i].loaded) continue;

        for (int j = 0; j < maya_packages[i].function_count; j++) {
            if (strcmp(maya_packages[i].function_names[j], name) == 0) {
                return i * MAX_FUNCTIONS + j; // Index unique
            }
        }
    }
    return -1;
}

// Fonction pour obtenir le package et l'index local d'une fonction
void get_package_info(int func_index, int *package_index, int *local_index) {
    *package_index = func_index / MAX_FUNCTIONS;
    *local_index = func_index % MAX_FUNCTIONS;
}

// Fonction pour appeler une fonction de package
void call_package_function(char *function_name, char *args) {
    (void)args; // Marquer le paramètre comme volontairement inutilisé pour l'instant

    int func_index = find_package_function(function_name);

    if (func_index == -1) {
        printf("❌ Fonction de package non trouvée: %s\n", function_name);
        printf("🔍 Fonctions disponibles:\n");
        for (int i = 0; i < maya_package_count; i++) {
            if (maya_packages[i].loaded) {
                for (int j = 0; j < maya_packages[i].function_count; j++) {
                    printf("   - %s\n", maya_packages[i].function_names[j]);
                }
            }
        }
        return;
    }

    int package_index, local_index;
    get_package_info(func_index, &package_index, &local_index);

    printf("🔄 Exécution de la fonction C: %s\n", function_name);

    // Vérifier que la fonction est valide
    if (maya_packages[package_index].functions[local_index] != NULL) {
        // Appeler la fonction
        maya_packages[package_index].functions[local_index]();
        printf("✅ Fonction C exécutée avec succès!\n");
    } else {
        printf("❌ Fonction invalide dans le package\n");
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
    (void)args; // Marquer le paramètre comme volontairement inutilisé pour l'instant

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

// Fonction pour extraire et exécuter du code C depuis Maya
char* extract_c_code(char *maya_line) {
    static char c_code[MAX_STRING_VALUE * 5];
    char *start = strchr(maya_line, '(');
    char *end = strrchr(maya_line, ')');

    if (!start || !end) return NULL;

    start++;
    *end = '\0';

    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }

    strcpy(c_code, start);
    return c_code;
}

void handle_execute_c(char *line) {
    char *c_code = extract_c_code(line);
    if (!c_code) {
        maya_error("Syntaxe incorrecte pour my.execute.c - parenthèses manquantes", 0);
        return;
    }

    printf("⚡ EXÉCUTION CODE C MAYA ⚡\n");
    printf("Code C à exécuter: %s\n", c_code);

    // Créer un fichier temporaire avec le code C
    FILE *temp_file = fopen("temp_maya_c.c", "w");
    if (temp_file) {
        fprintf(temp_file, "#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>\n");
        fprintf(temp_file, "int main() {\n");
        fprintf(temp_file, "%s\n", c_code);
        fprintf(temp_file, "return 0;\n}\n");
        fclose(temp_file);

        // Compiler et exécuter
        int compile_result = system("gcc temp_maya_c.c -o temp_maya_c -lm 2>/dev/null");
        if (compile_result == 0) {
            printf("🔨 Compilation réussie!\n");
            printf("📤 Sortie du programme:\n");
            system("./temp_maya_c");
            // Nettoyer
            system("rm -f temp_maya_c temp_maya_c.c");
            printf("✅ Code C exécuté avec succès!\n");
        } else {
            printf("❌ Erreur de compilation du code C\n");
            system("rm -f temp_maya_c.c");
        }
    }
}

// Nouvelles simulations v5.0
void handle_simulation_monrobot(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.monrobot - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🤖 SIMULATION ROBOT PERSONNALISÉ MAYA 🤖\n");
    printf("Paramètres: %s\n", start);

    // Simulation de création de robot
    printf("🔧 Assemblage des composants...\n");
    usleep(800000);
    printf("🧠 Installation de l'IA...\n");
    usleep(600000);
    printf("⚡ Activation des systèmes...\n");
    usleep(400000);

    printf("✅ Robot créé avec succès!\n");
    printf("🗣️ Robot: 'Bonjour! Je suis votre robot personnalisé Maya!'\n");
    printf("🤝 Votre robot est prêt à interagir!\n");
}

void handle_simulation_quantique(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.quantique - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("⚛️ SIMULATION UNIVERS QUANTIQUE MAYA ⚛️\n");
    printf("Paramètres: %s\n", start);

    printf("🌌 Initialisation de l'univers quantique...\n");
    usleep(1000000);
    printf("⚛️ Création des particules subatomiques...\n");
    usleep(800000);
    printf("🔬 Observation des phénomènes quantiques...\n");
    usleep(600000);

    srand(time(NULL));
    int scenario = rand() % 3;

    switch(scenario) {
        case 0:
            printf("🌟 Superposition quantique détectée!\n");
            printf("📊 État: |0⟩ + |1⟩\n");
            break;
        case 1:
            printf("🔗 Intrication quantique observée!\n");
            printf("📊 Particules liées instantanément\n");
            break;
        case 2:
            printf("🌊 Dualité onde-particule manifeste!\n");
            printf("📊 Comportement ondulatoire confirmé\n");
            break;
    }

    printf("🎯 Simulation quantique terminée!\n");
}

void handle_simulation_monunivers(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.monunivers - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🌍 CRÉATION UNIVERS PERSONNALISÉ MAYA 🌍\n");
    printf("Spécifications: %s\n", start);

    printf("💫 Big Bang de votre univers...\n");
    usleep(1200000);
    printf("⭐ Formation des premières étoiles...\n");
    usleep(1000000);
    printf("🪐 Création des planètes...\n");
    usleep(800000);
    printf("🌱 Apparition de la vie...\n");
    usleep(600000);

    printf("🎉 Votre univers est né!\n");
    printf("📊 Statistiques:\n");
    printf("   - 1 000 000 étoiles\n");
    printf("   - 500 000 planètes\n");
    printf("   - 42 civilisations\n");
    printf("🌟 Univers stable et en expansion!\n");
}

void handle_simulation_atomes(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.atomes - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    // Enlever les guillemets si présents
    if (start[0] == '\'' && start[strlen(start)-1] == '\'') {
        start[strlen(start)-1] = '\0';
        start++;
    }

    printf("⚛️ SIMULATION ATOMIQUE MAYA ⚛️\n");
    printf("Type d'atome: %s\n", start);

    // Base de données des éléments
    struct {
        char *nom;
        char *symbole;
        int protons;
        int neutrons;
        int electrons;
        char *proprietes;
    } elements[] = {
        {"Hydrogene", "H", 1, 0, 1, "Gaz le plus léger, inflammable"},
        {"Helium", "He", 2, 2, 2, "Gaz noble, très stable"},
        {"Lithium", "Li", 3, 4, 3, "Métal alcalin, très réactif"},
        {"Carbone", "C", 6, 6, 6, "Base de la vie organique"},
        {"Azote", "N", 7, 7, 7, "Gaz inerte, 78% de l'atmosphère"},
        {"Oxygene", "O", 8, 8, 8, "Vital pour la respiration"},
        {"Fluor", "F", 9, 10, 9, "Halogène très réactif"},
        {"Neon", "Ne", 10, 10, 10, "Gaz noble, éclairage"},
        {"Sodium", "Na", 11, 12, 11, "Métal alcalin, forme le sel"},
        {"Magnesium", "Mg", 12, 12, 12, "Métal léger, alliages"},
        {"Aluminium", "Al", 13, 14, 13, "Métal léger, anti-corrosion"},
        {"Silicium", "Si", 14, 14, 14, "Semi-conducteur, électronique"},
        {"Phosphore", "P", 15, 16, 15, "Essentiel pour l'ADN"},
        {"Soufre", "S", 16, 16, 16, "Forme des ponts disulfure"},
        {"Chlore", "Cl", 17, 18, 17, "Désinfectant, forme HCl"},
        {"Argon", "Ar", 18, 22, 18, "Gaz noble, soudage"},
        {"Potassium", "K", 19, 20, 19, "Électrolyte important"},
        {"Calcium", "Ca", 20, 20, 20, "Os et dents"},
        {"Fer", "Fe", 26, 30, 26, "Métal de construction"},
        {"Cuivre", "Cu", 29, 35, 29, "Conducteur électrique"},
        {"Zinc", "Zn", 30, 35, 30, "Anti-corrosion, galvanisation"},
        {"Argent", "Ag", 47, 61, 47, "Métal précieux, conducteur"},
        {"Or", "Au", 79, 118, 79, "Métal précieux, inoxydable"},
        {"Mercure", "Hg", 80, 121, 80, "Métal liquide toxique"},
        {"Plomb", "Pb", 82, 125, 82, "Métal lourd toxique"},
        {"Uranium", "U", 92, 146, 92, "Radioactif, énergie nucléaire"}
    };

    int element_count = sizeof(elements) / sizeof(elements[0]);
    int found = -1;

    // Rechercher l'élément
    for (int i = 0; i < element_count; i++) {
        if (strcasecmp(elements[i].nom, start) == 0 || 
            strcasecmp(elements[i].symbole, start) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("❌ Élément '%s' non trouvé dans la base de données\n", start);
        printf("💡 Éléments disponibles:\n");
        for (int i = 0; i < element_count && i < 10; i++) {
            printf("   - %s (%s)\n", elements[i].nom, elements[i].symbole);
        }
        printf("   ... et %d autres\n", element_count - 10);
        return;
    }

    printf("🔬 Analyse de la structure atomique...\n");
    usleep(600000);
    printf("⚡ Électrons en orbite...\n");
    usleep(400000);
    printf("🎯 Noyau stable détecté...\n");
    usleep(400000);

    printf("📊 RÉSULTATS DE LA SIMULATION:\n");
    printf("   Élément: %s (%s)\n", elements[found].nom, elements[found].symbole);
    printf("   Numéro atomique: %d\n", elements[found].protons);
    printf("   Protons: %d\n", elements[found].protons);
    printf("   Neutrons: %d\n", elements[found].neutrons);
    printf("   Électrons: %d\n", elements[found].electrons);
    printf("   Masse atomique: ~%d u\n", elements[found].protons + elements[found].neutrons);
    printf("   Propriétés: %s\n", elements[found].proprietes);

    // Configuration électronique simplifiée
    printf("   Configuration électronique: ");
    int electrons = elements[found].electrons;
    int shells[] = {2, 8, 18, 32}; // Capacité des couches K, L, M, N
    
    for (int shell = 0; shell < 4 && electrons > 0; shell++) {
        int e_in_shell = (electrons > shells[shell]) ? shells[shell] : electrons;
        printf("%d ", e_in_shell);
        electrons -= e_in_shell;
    }
    printf("\n");

    printf("✅ Simulation atomique terminée!\n");
}

void handle_simulation_timetravel(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.timetravel - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    int years = evaluate_expression_numeric(start);

    if (years < 1 || years > 15) {
        maya_error("Voyage temporel limité entre 1 et 15 ans", 0);
        return;
    }

    printf("⏰ MACHINE TEMPORELLE MAYA ⏰\n");
    printf("Voyage de %d années\n", years);

    printf("🔧 Calibrage de la machine temporelle...\n");
    usleep(800000);
    printf("⚡ Activation du flux temporel...\n");
    usleep(600000);
    printf("🌀 Traversée du continuum espace-temps...\n");
    usleep(1000000);

    // Générer un événement basé sur le nombre d'années
    if (years <= 5) {
        printf("🎯 ARRIVÉE DANS LE PASSÉ (%d ans):\n", years);
        printf("📱 Les smartphones n'existaient pas encore!\n");
        printf("🎵 La musique était différente...\n");
    } else if (years <= 10) {
        printf("🎯 ARRIVÉE DANS LE PASSÉ (%d ans):\n", years);
        printf("💻 Internet était plus lent!\n");
        printf("🎮 Les jeux vidéo étaient différents!\n");
    } else {
        printf("🎯 ARRIVÉE DANS LE FUTUR (%d ans):\n", years);
        printf("🚗 Les voitures volent!\n");
        printf("🤖 Les robots sont partout!\n");
    }

    printf("✅ Voyage temporel terminé! Retour au présent...\n");
}

void handle_simulation_fairy(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.fairy - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🧚‍♀️ CONTE DE FÉE MAYA 🧚‍♀️\n");
    printf("Histoire: %s\n", start);

    printf("📖 Il était une fois, dans un royaume lointain...\n");
    usleep(800000);
    printf("🏰 Une princesse vivait dans un château magique...\n");
    usleep(600000);
    printf("🐉 Un dragon gardait un trésor précieux...\n");
    usleep(600000);
    printf("⚔️ Un brave chevalier décida de sauver le royaume...\n");
    usleep(800000);

    srand(time(NULL));
    int ending = rand() % 3;

    switch(ending) {
        case 0:
            printf("👑 Le chevalier épousa la princesse!\n");
            printf("🎉 Ils vécurent heureux pourtoujours!\n");
            break;
        case 1:
            printf("🐉 Le dragon devint l'ami du chevalier!\n");
            printf("🤝 La paix régna sur le royaume!\n");
            break;
        case 2:
            printf("✨ La princesse révéla être une fée!\n");
            printf("🌟 Elle transforma le royaume en paradis!\n");
            break;
    }

    printf("📚 Fin de votre conte de fée personnalisé!\n");
}

void handle_simulation_vampire(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.vampire - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🧛‍♂️ HISTOIRE DE VAMPIRE MAYA 🧛‍♂️\n");
    printf("Récit: %s\n", start);

    printf("🌙 La nuit tombe sur la ville gothique...\n");
    usleep(800000);
    printf("🏰 Dans un château sombre, un vampire s'éveille...\n");
    usleep(600000);
    printf("🦇 Il se transforme en chauve-souris...\n");
    usleep(500000);
    printf("🌃 Il survole la ville à la recherche...\n");
    usleep(700000);

    srand(time(NULL));
    int scenario = rand() % 3;

    switch(scenario) {
        case 0:
            printf("💘 Il rencontre l'amour de sa vie!\n");
            printf("🌹 Une histoire d'amour éternelle commence...\n");
            break;
        case 1:
            printf("⚔️ Il affronte un chasseur de vampires!\n");
            printf("🛡️ Un combat épique s'engage!\n");
            break;
        case 2:
            printf("🧪 Il cherche une potion pour redevenir humain!\n");
            printf("🔬 Une quête mystique commence...\n");
            break;
    }

    printf("🌅 À l'aube, votre histoire de vampire se termine!\n");
}

void handle_simulation_sirene(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.sirene - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("🧜‍♀️ RÉCIT DE SIRÈNE MAYA 🧜‍♀️\n");
    printf("Histoire: %s\n", start);

    printf("🌊 Dans les profondeurs de l'océan...\n");
    usleep(800000);
    printf("🐚 Une sirène chante sa mélodie enchanteresse...\n");
    usleep(600000);
    printf("🏖️ Un navigateur entend son chant...\n");
    usleep(500000);
    printf("⛵ Son navire se dirige vers les récifs...\n");
    usleep(700000);

    srand(time(NULL));
    int outcome = rand() % 3;

    switch(outcome) {
        case 0:
            printf("💙 La sirène tombe amoureuse du navigateur!\n");
            printf("🌟 Elle le sauve et devient humaine!\n");
            break;
        case 1:
            printf("🔱 Le roi des mers intervient!\n");
            printf("👑 Il accorde un vœu à la sirène!\n");
            break;
        case 2:
            printf("🌈 La sirène découvre un trésor caché!\n");
            printf("💎 Elle devient la gardienne des océans!\n");
            break;
    }

    printf("🐠 Votre récit de sirène se termine dans les vagues!\n");
}

void handle_simulation_monster(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.simulation.monster - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("👹 SIMULATION MONSTRE MAYA 👹\n");
    printf("Créature: %s\n", start);

    printf("🌙 Une nuit d'orage, dans une forêt sombre...\n");
    usleep(800000);
    printf("👁️ Des yeux brillent dans l'obscurité...\n");
    usleep(600000);
    printf("🦶 Des pas lourds résonnent...\n");
    usleep(500000);
    printf("👹 Le monstre apparaît!\n");
    usleep(700000);

    srand(time(NULL));
    int monster_action = rand() % 3;

    switch(monster_action) {
        case 0:
            printf("😢 Le monstre est en réalité seul et triste!\n");
            printf("🤗 Il cherche juste un ami!\n");
            break;
        case 1:
            printf("🏃‍♂️ Le monstre poursuit les villageois!\n");
            printf("⚔️ Les héros se préparent au combat!\n");
            break;
        case 2:
            printf("🔮 Le monstre garde un secret mystique!\n");
            printf("📜 Une ancienne malédiction doit être brisée!\n");
            break;
    }

    printf("🌅 Votre histoire de monstre se termine à l'aube!\n");
}

// Fonctions d'exercices scolaires
void handle_exercice_create(char *line) {
    char *start = strchr(line, '(');
    char *end = strrchr(line, ')');

    if (!start || !end) {
        maya_error("Syntaxe incorrecte pour my.exercice.create - parenthèses manquantes", 0);
        return;
    }

    start++;
    *end = '\0';

    printf("📚 CRÉATEUR D'EXERCICES MAYA 📚\n");
    printf("Sujet: %s\n", start);
    printf("📝 Exercice personnalisé créé!\n");
    printf("🎯 Utilisez my.input pour les réponses interactives!\n");
    printf("✅ Exercice prêt à être utilisé!\n");
}

void handle_exercice_gest_pgi(char *line) {
    (void)line; // Marquer le paramètre comme volontairement inutilisé

    printf("💼 EXERCICE PGI MAYA - BAC STMG 💼\n");
    printf("📊 Progiciel de Gestion Intégrée\n");
    printf("❓ Question: Quels sont les modules d'un PGI?\n");
    printf("💡 Réponse attendue: Comptabilité, RH, Ventes, Stocks, etc.\n");
    printf("🎓 Niveau: Terminale STMG\n");
}

void handle_exercice_gest_treso(char *line) {
    (void)line; // Marquer le paramètre comme volontairement inutilisé

    printf("💰 EXERCICE TRÉSORERIE MAYA - BAC STMG 💰\n");
    printf("📈 Gestion financière\n");
    printf("❓ Calculez: FRNG = Capitaux permanents - Actif immobilisé\n");
    printf("❓ BFR = Actif circulant - Passif circulant\n");
    printf("❓ Trésorerie = FRNG - BFR\n");
    printf("🎓 Exercice de gestion niveau BAC STMG\n");
}

void handle_exercice_math(char *line) {
    (void)line; // Marquer le paramètre comme volontairement inutilisé

    printf("🔢 EXERCICE MATHÉMATIQUES MAYA 🔢\n");
    printf("📐 Mathématiques BAC Général/STMG\n");
    printf("❓ Question: Résolvez l'équation du second degré\n");
    printf("📝 ax² + bx + c = 0\n");
    printf("💡 Utilisez Δ = b² - 4ac\n");
    printf("🎓 Niveau: Première/Terminale\n");
}

void handle_exercice_histoire(char *line) {
    (void)line; // Marquer le paramètre comme volontairement inutilisé

    printf("🏛️ EXERCICE HISTOIRE-GÉO MAYA 🏛️\n");
    printf("🌍 Histoire-Géographie\n");
    printf("❓ Question: Quelles sont les causes de la Première Guerre mondiale?\n");
    printf("📚 Thèmes: Nationalisme, impérialisme, alliances\n");
    printf("🎓 Niveau: BAC Général/STMG\n");
}

void handle_exercice_mana(char *line) {
    (void)line; // Marquer le paramètre comme volontairement inutilisé

    printf("👔 EXERCICE MANAGEMENT MAYA - BAC STMG 👔\n");
    printf("🏢 Sciences de gestion et management\n");
    printf("❓ Question: Définissez les styles de management\n");
    printf("💼 Réponses: Directif, Persuasif, Participatif, Délégatif\n");
    printf("🎓 Niveau: Terminale STMG\n");
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
    else if (strstr(line, "my.math.median")) {
        handle_math_median(line);
    }
    else if (strstr(line, "my.math.moyenne")) {
        handle_math_moyenne(line);
    }
    else if (strstr(line, "my.math.cube")) {
        handle_math_cube(line);
    }
    else if (strstr(line, "my.math.carre")) {
        handle_math_carre(line);
    }
    else if (strstr(line, "my.math.racine")) {
        handle_math_racine(line);
    }
    else if (strstr(line, "my.math.degres")) {
        handle_math_degres(line);
    }
    else if (strstr(line, "my.math")) {
        handle_math(line);
    }
    else if (strstr(line, "my.thales")) {
        handle_thales(line);
    }
    else if (strstr(line, "my.pytha")) {
        handle_pytha(line);
    }
    else if (strstr(line, "my.rythme.convertir")) {
        handle_rythme_convertir(line);
    }
    else if (strstr(line, "my.renvoie.gamme")) {
        handle_renvoie_gamme(line);
    }
    else if (strstr(line, "my.obtenir.gamme")) {
        handle_obtenir_gamme(line);
    }
    else if (strstr(line, "my.send.db")) {
        handle_send_db(line);
    }
    else if (strstr(line, "my.supp.db")) {
        handle_supp_db(line);
    }
    else if (strstr(line, "my.util.db")) {
        handle_util_db(line);
    }
    else if (strstr(line, "my.db")) {
        handle_db(line);
    }
    else if (strstr(line, "my.execute.c")) {
        handle_execute_c(line);
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
    else if (strstr(line, "my.simulation.monrobot")) {
        handle_simulation_monrobot(line);
    }
    else if (strstr(line, "my.simulation.quantique")) {
        handle_simulation_quantique(line);
    }
    else if (strstr(line, "my.simulation.monunivers")) {
        handle_simulation_monunivers(line);
    }
    else if (strstr(line, "my.simulation.atomes")) {
        handle_simulation_atomes(line);
    }
    else if (strstr(line, "my.simulation.timetravel")) {
        handle_simulation_timetravel(line);
    }
    else if (strstr(line, "my.simulation.fairy")) {
        handle_simulation_fairy(line);
    }
    else if (strstr(line, "my.simulation.vampire")) {
        handle_simulation_vampire(line);
    }
    else if (strstr(line, "my.simulation.sirene")) {
        handle_simulation_sirene(line);
    }
    else if (strstr(line, "my.simulation.monster")) {
        handle_simulation_monster(line);
    }
    else if (strstr(line, "my.exercice.gest.pgi")) {
        handle_exercice_gest_pgi(line);
    }
    else if (strstr(line, "my.exercice.gest.treso")) {
        handle_exercice_gest_treso(line);
    }
    else if (strstr(line, "my.exercice.math")) {
        handle_exercice_math(line);
    }
    else if (strstr(line, "my.exercice.histoire")) {
        handle_exercice_histoire(line);
    }
    else if (strstr(line, "my.exercice.mana")) {
        handle_exercice_mana(line);
    }
    else if (strstr(line, "my.exercice.create")) {
        handle_exercice_create(line);
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
        // Gestion améliorée des fonctions réutilisables et packages C
        char line_copy[MAX_LINE_LENGTH];
        strcpy(line_copy, line);

        char *paren = strchr(line_copy, '(');
        char *func_name = line_copy;
        char *args = "";

        if (paren) {
            *paren = '\0';
            args = paren + 1;
            char *end_paren = strrchr(args, ')');
            if (end_paren) *end_paren = '\0';
        }

        trim(func_name);

        printf("🔍 Recherche de la fonction: %s\n", func_name);

        // Convertir le nom de fonction Maya vers le nom C
        char c_func_name[MAX_VAR_NAME];
        strcpy(c_func_name, func_name);

        // Remplacer les points par des underscores pour les fonctions C
        for (int i = 0; c_func_name[i]; i++) {
            if (c_func_name[i] == '.') {
                c_func_name[i] = '_';
            }
        }

        printf("🔧 Nom C converti: %s\n", c_func_name);

        // Essayer d'abord les fonctions de packages C
        if (find_package_function(c_func_name) != -1) {
            printf("📦 Fonction trouvée dans package C\n");
            call_package_function(c_func_name, args);
        }
        // Sinon, essayer les fonctions Maya réutilisables
        else if (find_maya_function(func_name) != -1) {
            printf("🌟 Fonction Maya réutilisable trouvée\n");
            execute_maya_function(func_name, args);
        }
        else {
            printf("❌ Fonction introuvable: %s\n", func_name);
            printf("📋 Fonctions disponibles:\n");

            // Lister les fonctions de packages
            printf("   🅒 Packages C:\n");
            for (int i = 0; i < maya_package_count; i++) {
                if (maya_packages[i].loaded) {
                    for (int j = 0; j < maya_packages[i].function_count; j++) {
                        printf("      - %s\n", maya_packages[i].function_names[j]);
                    }
                }
            }

            // Lister les fonctions Maya
            printf("   🌟 Fonctions Maya:\n");
            for (int i = 0; i < maya_function_count; i++) {
                printf("      - %s\n", maya_functions[i].name);
            }

            if (maya_package_count == 0 && maya_function_count == 0) {
                printf("   (Aucune fonction personnalisée chargée)\n");
                printf("💡 Utilisez my.package.charge('fichier.c') pour charger un package\n");
            }
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
    printf("🌸 === Interpréteur Maya v5.0 === 🌸\n");
    printf("🆕 NOUVELLES FONCTIONNALITÉS v5.0:\n");
    printf("🧮 Maths avancées: médiane, moyenne, cube, carré, racine, degrés\n");
    printf("📐 Théorèmes: Pythagore (my.pytha), Thalès (my.thales)\n");
    printf("🎵 Musique: my.rythme.convertir, my.renvoie.gamme, my.obtenir.gamme\n");
    printf("🗄️ Bases de données: my.db, my.send.db, my.supp.db, my.util.db\n");
    printf("⚡ Code C intégré: my.execute.c (exécutez du C directement!)\n");
    printf("🌌 Nouvelles simulations: Robot, Quantique, Univers, Atomes, Voyage temporel\n");
    printf("🧚‍♀️ Contes: Fées, Vampires, Sirènes, Monstres\n");
    printf("📚 Exercices BAC: Gestion, Math, Histoire, Management (STMG/Général)\n");
    printf("🎮 Mini-jeux: Quizz, Dés, Puissance4, Pendu, Memory, Snake, TicTac et plus!\n");
    printf("📦 Packages C améliorés: Plus simples à créer et charger!\n");
    printf("🚀 Maya v5.0 - Le plus créatif des langages! 🚀\n");
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

    // Nettoyer les packages chargés
    for (int i = 0; i < maya_package_count; i++) {
        if (maya_packages[i].handle) {
            dlclose(maya_packages[i].handle);
            // Supprimer le fichier temporaire
            unlink(maya_packages[i].so_path);
        }
    }

    return 0;
}