
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
    return 0; // Variable non trouvée
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
    }
}

// Fonction pour évaluer une expression mathématique simple
int evaluate_expression(char *expr) {
    trim(expr);
    
    // Chercher l'opérateur
    char *op_pos = NULL;
    char operator = '\0';
    
    for (int i = 1; i < strlen(expr); i++) {
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
        case '/': return right_val != 0 ? left_val / right_val : 0;
        default: return 0;
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
        }
    }
}

// Fonction pour traiter les opérations mathématiques
void handle_math(char *line) {
    if (strstr(line, "my.math.add")) {
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (start && end) {
            start++;
            *end = '\0';
            int result = evaluate_expression(start);
            printf("%d\n", result);
        }
    }
    else if (strstr(line, "my.math.sub")) {
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (start && end) {
            start++;
            *end = '\0';
            int result = evaluate_expression(start);
            printf("%d\n", result);
        }
    }
    else if (strstr(line, "my.math.mult")) {
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (start && end) {
            start++;
            *end = '\0';
            int result = evaluate_expression(start);
            printf("%d\n", result);
        }
    }
    else if (strstr(line, "my.math.div")) {
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (start && end) {
            start++;
            *end = '\0';
            int result = evaluate_expression(start);
            printf("%d\n", result);
        }
    }
}

// Fonction pour traiter les variables
void handle_variable(char *line) {
    char *equal_pos = strchr(line, '=');
    if (equal_pos) {
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
        
        int value = evaluate_expression(value_str);
        set_variable(var_name, value);
    }
}

// Fonction pour traiter les conditions
int handle_condition(char *condition) {
    trim(condition);
    
    // Chercher l'opérateur de comparaison
    char *comp_pos = NULL;
    char comp_op = '\0';
    
    for (int i = 0; i < strlen(condition); i++) {
        if (condition[i] == '<' || condition[i] == '>') {
            comp_pos = &condition[i];
            comp_op = condition[i];
            break;
        }
    }
    
    if (!comp_pos) return 0;
    
    // Séparer les opérandes
    *comp_pos = '\0';
    char *left = condition;
    char *right = comp_pos + 1;
    
    trim(left);
    trim(right);
    
    int left_val = evaluate_expression(left);
    int right_val = evaluate_expression(right);
    
    switch (comp_op) {
        case '<': return left_val < right_val;
        case '>': return left_val > right_val;
        default: return 0;
    }
}

// Fonction principale pour interpréter une ligne
void interpret_line(char *line) {
    trim(line);
    
    if (strlen(line) == 0 || line[0] == '#') return; // Ligne vide ou commentaire
    
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
        // Traitement basique des conditions (simplifié)
        char *start = strchr(line, '(');
        char *end = strrchr(line, ')');
        if (start && end) {
            start++;
            *end = '\0';
            int condition_result = handle_condition(start);
            if (condition_result) {
                printf("Condition vraie\n");
            } else {
                printf("Condition fausse\n");
            }
        }
    }
}

int main(void) {
    printf("=== Interpréteur Maya v1.0 ===\n");
    printf("Langage de programmation créatif Maya\n");
    printf("Entrez votre code Maya (tapez 'exit' pour quitter):\n\n");
    
    char line[MAX_LINE_LENGTH];
    
    // Exemple de démonstration
    printf("Exemple de code Maya:\n");
    printf("my.console('Bonjour Maya!')\n");
    interpret_line("my.console('Bonjour Maya!')");
    
    printf("my.variable test = 15\n");
    interpret_line("my.variable test = 15");
    
    printf("my.math.add(test + 5)\n");
    interpret_line("my.math.add(test + 5)");
    
    printf("my.if(test < 20)\n");
    interpret_line("my.if(test < 20)");
    
    printf("\nMode interactif - Entrez votre code:\n");
    
    while (1) {
        printf("maya> ");
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        
        // Enlever le saut de ligne
        line[strcspn(line, "\n")] = '\0';
        
        if (strcmp(line, "exit") == 0) {
            printf("Au revoir!\n");
            break;
        }
        
        interpret_line(line);
    }
    
    return 0;
}
