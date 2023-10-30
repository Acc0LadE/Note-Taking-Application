#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a file node
struct FileNode {
    char filename[256];
    struct FileNode* next;
};

// Function to create a new file node
struct FileNode* createFileNode(const char* filename) {
    struct FileNode* newNode = (struct FileNode*)malloc(sizeof(struct FileNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(newNode->filename, filename, sizeof(newNode->filename));
    newNode->next = NULL;
    return newNode;
}

// Function to add a file to the linked list
void addFileToList(struct FileNode** head, const char* filename) {
    struct FileNode* newNode = createFileNode(filename);

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct FileNode* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to open a file in Notepad
void openFileInNotepad(const char* filename) {
    char command[256];
    snprintf(command, sizeof(command), "notepad %s", filename);
    system(command);
}

// Function to print the list of files
void printFileList(struct FileNode* head) {
    struct FileNode* current = head;
    int index = 1;
    while (current != NULL) {
        printf("%d. %s\n", index, current->filename);
        current = current->next;
        index++;
    }
}

// Function to free the memory allocated for the linked list
void freeFileList(struct FileNode* head) {
    while (head != NULL) {
        struct FileNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to save the linked list to a file
void saveToFile(struct FileNode* head) {
    FILE* file = fopen("filelist.txt", "w");
    if (file == NULL) {
        perror("File opening failed");
        return;
    }

    struct FileNode* current = head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->filename);
        current = current->next;
    }

    fclose(file);
}

// Function to load the linked list from a file
void loadFromFile(struct FileNode** head) {
    FILE* file = fopen("filelist.txt", "r");
    if (file == NULL) {
        perror("File opening failed");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';
        addFileToList(head, buffer);
    }

    fclose(file);
}

int main() {
    struct FileNode* fileList = NULL;
    loadFromFile(&fileList);

    int choice;
    char filename[256];
    while (1) {
        printf("Choose an option:\n");
        printf("1. Add a text file to the list\n");
        printf("2. Open a text file in Notepad\n");
        printf("3. Print the list of files\n");
        printf("4. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the filename (e.g., myfile.txt): ");
                scanf("%s", filename);
                addFileToList(&fileList, filename);
                saveToFile(fileList); // Save the updated list
                break;
            case 2:
                if (fileList != NULL) {
                    printFileList(fileList);
                    printf("Enter the index of the file to open: ");
                    int fileIndex;
                    scanf("%d", &fileIndex);

                    struct FileNode* current = fileList;
                    int currentIndex = 1;
                    while (current != NULL && currentIndex < fileIndex) {
                        current = current->next;
                        currentIndex++;
                    }

                    if (current != NULL) {
                        openFileInNotepad(current->filename);
                    } else {
                        printf("Invalid index.\n");
                    }
                } else {
                    printf("The list is empty.\n");
                }
                break;
            case 3:
                if (fileList != NULL) {
                    printf("List of files:\n");
                    printFileList(fileList);
                } else {
                    printf("The list is empty.\n");
                }
                break;
            case 4:
                // Free the memory used by the list and exit
                freeFileList(fileList);
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
