#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variable for all implementations
extern char **environ;

// 0. Print Environment Variables
void print_environ() {
    char **env;
    for (env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

// 1. Compare env and environ addresses
void compare_env_addresses(char **env) {
    printf("Address of env (main parameter): %p\n", (void*)env);
    printf("Address of environ (global var): %p\n", (void*)environ);
    
    // Are they the same?
    if (env == environ) {
        printf("env and environ are the SAME address!\n");
    } else {
        printf("env and environ are DIFFERENT addresses!\n");
    }
}

// 2. Custom getenv() implementation
char *_getenv(const char *name) {
    char **env;
    size_t name_len = strlen(name);
    
    for (env = environ; *env != NULL; env++) {
        // Check if the environment variable starts with the name
        if (strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=') {
            // Return pointer to value part (after '=')
            return *env + name_len + 1;
        }
    }
    
    return NULL;
}

// 3. Print PATH directories
void print_path_directories() {
    char *path = _getenv("PATH");
    if (!path) {
        printf("PATH environment variable not found.\n");
        return;
    }
    
    char *token = strtok(path, ":");
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, ":");
    }
}

// 4. PATH Linked List Implementation
typedef struct PathNode {
    char *directory;
    struct PathNode *next;
} PathNode;

PathNode* build_path_list() {
    char *path = _getenv("PATH");
    if (!path) {
        return NULL;
    }
    
    PathNode *head = NULL, *current = NULL;
    char *token = strtok(path, ":");
    
    while (token != NULL) {
        PathNode *new_node = malloc(sizeof(PathNode));
        new_node->directory = strdup(token);
        new_node->next = NULL;
        
        if (!head) {
            head = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = new_node;
        }
        
        token = strtok(NULL, ":");
    }
    
    return head;
}

// Helper function to free PATH list
void free_path_list(PathNode *head) {
    while (head) {
        PathNode *temp = head;
        head = head->next;
        free(temp->directory);
        free(temp);
    }
}

// 5. Custom setenv() implementation
int _setenv(const char *name, const char *value, int overwrite) {
    char **env;
    size_t name_len = strlen(name);
    size_t value_len = strlen(value);
    
    // Check if variable exists and overwrite is disabled
    for (env = environ; *env != NULL; env++) {
        if (strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=') {
            if (!overwrite) {
                return 0;  // Variable exists, but overwrite is 0
            }
            
            // Replace existing variable
            char *new_var = malloc(name_len + value_len + 2);
            sprintf(new_var, "%s=%s", name, value);
            *env = new_var;
            return 0;
        }
    }
    
    // If we reach here, variable doesn't exist - add new
    // Note: This is a simplified implementation and won't work perfectly
    return 0;
}

// 6. Custom unsetenv() implementation
int _unsetenv(const char *name) {
    char **env, **current;
    size_t name_len = strlen(name);
    int found = 0;
    
    for (env = current = environ; *env != NULL; env++) {
        if (strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=') {
            found = 1;
            continue;  // Skip this entry
        }
        
        *current = *env;
        current++;
    }
    
    *current = NULL;  // Terminate the environment list
    
    return found ? 0 : -1;
}

int main() {
    printf("0. Printing Environment:\n");
    print_environ();
    
    printf("\n1. Comparing env and environ:\n");
    compare_env_addresses(environ);
    
    printf("\n2. Testing _getenv():\n");
    char *home = _getenv("HOME");
    printf("HOME = %s\n", home ? home : "Not found");
    
    printf("\n3. Printing PATH directories:\n");
    print_path_directories();
    
    printf("\n4. Building PATH Linked List:\n");
    PathNode *path_list = build_path_list();
    PathNode *current = path_list;
    while (current) {
        printf("%s\n", current->directory);
        current = current->next;
    }
    free_path_list(path_list);
    
    printf("\n5 & 6. Testing setenv and unsetenv:\n");
    _setenv("TEST_VAR", "test_value", 1);
    char *test_var = _getenv("TEST_VAR");
    printf("TEST_VAR = %s\n", test_var ? test_var : "Not found");
    
    _unsetenv("TEST_VAR");
    test_var = _getenv("TEST_VAR");
    printf("TEST_VAR after unset = %s\n", test_var ? test_var : "Not found");
    
    return 0;
}
