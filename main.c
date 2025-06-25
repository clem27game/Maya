
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define MAX_VARIABLES 100
#define MAX_VAR_NAME 50

typedef struct {
    char name[MAX_VAR_NAME];
    int value;
} Variable;

Variable variables[MAX_VARIABLES];
int var_count = 0;

// Variables globales pour gérer les conditions
int last_condition_result = 0;
int in_alors_block = 0;
int in_autre_block = 0;
int skip_current_block = 0;

// Déclarations des fonctions (prototypes)
void maya_error(const char *message, int line_number);
void trim(char *str);
int get_variable_value(char *name);
void set_variable(char *name, int value);
int evaluate_expression(char *expr);
void handle_console(char *content);
void handle_math(char *line);
void handle_variable(char *line);
int handle_condition(char *condition);
void interpret_line(char *line);
void execute_maya_file(const char *filename);

// Fonction pour afficher les erreurs Maya
void maya_error(const char *message, int line_number) {
    if (line_number > 0) {
        printf("ERREUR MAYA [Ligne %d]: %s\n", line_number, message);
    } else {
        printf("ERREUR MAYA: %s\n", message);
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
int get_variable_value(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    maya_error("Variable non trouvée", 0);
    return 0;
}

// Fonction pour définir une variable
void set_variable(char *name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }
    // Nouvelle variable
    if (var_count < MAX_VARIABLES) {
        strcpy(variables[var_count].name, name);
        variables[var_count].value = value;
        var_count++;
    } else {
        maya_error("Trop de variables définies", 0);
    }
}

// Fonction pour évaluer une expression mathématique simple
int evaluate_expression(char *expr) {
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
            return get_variable_value(expr);
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
        left_val = get_variable_value(left);
    }
    
    // Évaluer l'opérande droite
    if (isdigit(right[0]) || right[0] == '-') {
        right_val = atoi(right);
    } else {
        right_val = get_variable_value(right);
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

// Fonction pour traiter my.console
void handle_console(char *content) {
    // Extraire le contenu entre quotes
    char *start = strchr(content, '\'');
    if (start) {
        start++;
        char *end = strrchr(content, '\'');
        if (end && end != start) {
            *end = '\0';
            printf("%s\n", start);
        } else {
            maya_error("Syntaxe incorrecte pour my.console - guillemets manquants", 0);
        }
    } else {
        maya_error("Syntaxe incorrecte pour my.console - guillemets manquants", 0);
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
    
    if (strstr(line, "my.math.add")) {
        int result = evaluate_expression(start);
        printf("Résultat: %d\n", result);
    }
    else if (strstr(line, "my.math.sub")) {
        int result = evaluate_expression(start);
        printf("Résultat: %d\n", result);
    }
    else if (strstr(line, "my.math.mult")) {
        int result = evaluate_expression(start);
        printf("Résultat: %d\n", result);
    }
    else if (strstr(line, "my.math.div")) {
        int result = evaluate_expression(start);
        printf("Résultat: %d\n", result);
    }
    else {
        maya_error("Opération mathématique non reconnue", 0);
    }
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
    
    if (strlen(value_str) == 0) {
        maya_error("Valeur de variable vide", 0);
        return;
    }
    
    int value = evaluate_expression(value_str);
    set_variable(var_name, value);
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
        return evaluate_expression(condition);
    }
    
    // Séparer les opérandes
    size_t op_len = strlen(comp_op);
    *comp_pos = '\0';
    char *left = condition;
    char *right = comp_pos + op_len;
    
    trim(left);
    trim(right);
    
    int left_val = evaluate_expression(left);
    int right_val = evaluate_expression(right);
    
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
    printf("=== Interpréteur Maya v1.0 ===\n");
    printf("Mode interactif - Tapez 'exit' pour quitter\n\n");
    
    char line[MAX_LINE_LENGTH];
    
    while (1) {
        printf("maya> ");
        fflush(stdout);
        
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        // Enlever le saut de ligne
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "exit") == 0) {
            printf("Au revoir!\n");
            break;
        }
        
        if (strlen(line) > 0) {
            interpret_line(line);
        }
    }
    
    return 0;
}
