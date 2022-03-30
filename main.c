#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


#define EXIT 10
#define NAME_MAX_LENGTH 51

typedef struct {
	int day;
	int month;
} Date;

struct REC {
    char name[NAME_MAX_LENGTH];
    int priority;
    Date delivery; 
    struct REC *left;
    struct REC *rigth;
};

typedef struct REC Task;

void copyName(char *inputName, char *name) {
    for (int i = 0; i < NAME_MAX_LENGTH; i++) {
        if (inputName[i] == '\0') {
            name[i] = '\0';
            break;
        }
        name[i] = inputName[i];
    }
}

int namesIsEquals(char *rootName, char *taskName) {
    int i = 0;
    int isEquals = 0;
    while (taskName[i] != '\0') {
        if (rootName[i] == taskName[i]) isEquals = 1;
        else isEquals = 0;
        i++;
    }
    return isEquals;
}


void printInOrder(Task *root) {
    if (root == NULL) {
        return;
    } else {
        printInOrder(root->left);
        printf("Nome: %s\n", root->name);
        printf("Prioridade: %d\n", root->priority);
        printf("Entrega %d/%d\n", root->delivery.day, root->delivery.month);
        printf("---------------------------------\n");
        printInOrder(root->rigth);
    }
}

Task *insertTree(Task *root, Task *task) {
    if (root == NULL) {
        return task;
    } else {
        int cmp = strcmp(root->name, task->name);
        if (cmp >= 0) {
            root->left = insertTree(root->left, task);

        } else {
            root->rigth = insertTree(root->rigth, task);
        }
        return root;
    }
}

Task *removeTree(Task *root, char *name) {
    if (root == NULL) {
        return NULL;
    } else {
        if (namesIsEquals(root->name, name)) {
            if (root->left == NULL && root->rigth == NULL) {
                free(root);
                return NULL;

            } else if (root->left == NULL) {
                Task *aux = root->rigth;
                free(root);
                return aux;

            } else if (root->rigth == NULL) {
                Task *aux = root->left;
                free(root);
                return aux;

            } else {
                Task *aux = root->rigth;
                while (aux->left != NULL)
                    aux = aux->left;
                aux->left = root->left;
                free(root);
                return aux;
            }
        }
        root->left = removeTree(root->left, name);
        root->rigth = removeTree(root->rigth, name);
    }
    return root;
}

Task *updateTree(Task *root, Task *task) {
    if (root == NULL) {
        return NULL;
    } else {
        if (namesIsEquals(root->name, task->name)) {
            root->priority = task->priority;
            root->delivery.day = task->delivery.day;
            root->delivery.month = task->delivery.month;
        }
        updateTree(root->left, task);
        updateTree(root->rigth, task);
    }
    return root;
}

void queryTree(Task *root, char *name) {
    if (root == NULL) {
        return;
    } else {
        if (namesIsEquals(root->name, name)) {
            printf("Nome: %s\n", root->name);
            printf("Prioridade: %d\n", root->priority);
            printf("Entrega %d/%d\n", root->delivery.day, root->delivery.month);
        }
        queryTree(root->left, name);
        queryTree(root->rigth, name);
    }
}


Task *insTask(Task *list) {
    Task *task = malloc(sizeof(Task));
    scanf("%s", task->name);
    scanf("%d", &task->priority);
    scanf("%d", &task->delivery.day);
    scanf("%d", &task->delivery.month);     
    task->left = NULL;
    task->rigth = NULL;    
    return insertTree(list, task);
}

Task *delTask (Task *list) {
    char name[NAME_MAX_LENGTH];
    scanf("\n%s", name);
    return removeTree(list, name);
}

void listTasks (Task *list) {
    printInOrder(list);
}

void queryTask (Task *list) {
    char name[NAME_MAX_LENGTH];
    scanf("%s", name);
    queryTree(list, name);
}

Task *upTask (Task *list) {
    Task *task = malloc(sizeof(Task));
    scanf("%s", task->name);
    scanf("%d", &task->priority);
    scanf("%d", &task->delivery.day);
    scanf("%d", &task->delivery.month);     
    task->left = NULL;
    task->rigth = NULL; 
    return updateTree(list, task);
}

Task *readFile(Task *list) {
    FILE *file;
    file = fopen("tasks.txt", "rt");
    char name[NAME_MAX_LENGTH];
    int day;
    int month;
    int priority;
    while (fscanf(file, "%s %d %d %d\n", name, &priority, &day, &month) != EOF) {
        Task *task = malloc(sizeof(Task));
        copyName(name, task->name);
        task->priority = priority;
        task->delivery.day = day;
        task->delivery.month = month;
        task->left = NULL;
        task->rigth = NULL;
        list = insertTree(list, task);
    }
    fclose(file);
    return list;
}


void writeFile(Task *root, FILE *file) {
    if (root == NULL) {
        return;
    } else {
        writeFile(root->left, file);
        fprintf(file, "%s %d %d %d\n", root->name, root->priority, root->delivery.day, root->delivery.month);
        writeFile(root->rigth, file);
    }
}

void readAndWriteFile(Task *root) {
    FILE *file;
    file = fopen("tasks.txt", "wt");
    writeFile(root, file);
    fclose(file);
}


int menu() {
    int op = 0;
    printf("%d Finaliza", EXIT);
    printf("\n: ");
    scanf("%d", &op);
    return op;
}

int main() {
    int op = 0;
    Task *list = NULL;
    list = readFile(list);
    while (op != EXIT) {
        op = menu();
        switch (op) {
            case 1 : 
                list = insTask(list);
                break;
            case 2 :
                list = delTask(list);
                break;
            case 3 : 
                list = upTask(list);
                break;
            case 4 : 
                queryTask(list);
                break;
            case 5 : 
                listTasks(list);
                break;
        }
    }
    readAndWriteFile(list);
    return 0;
}