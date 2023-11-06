#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a file node
struct FileNode
{
    char filename[256];
    struct FileNode *next;
};

// Structure to represent a folder node
struct FolderNode
{
    char foldername[256];
    struct FileNode *fileList;
    struct FolderNode *childFolders;
    struct FolderNode *parentFolder; // Reference to the parent folder
    struct FolderNode *next;
};

// Function to create a new file node
struct FileNode *createFileNode(const char *filename)
{
    struct FileNode *newNode = (struct FileNode *)malloc(sizeof(struct FileNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(newNode->filename, filename, sizeof(newNode->filename));
    newNode->next = NULL;
    return newNode;
}

// Function to add a file to the linked list
void addFileToList(struct FileNode **head, const char *filename)
{
    struct FileNode *newNode = createFileNode(filename);

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct FileNode *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to create a new folder node
struct FolderNode *createFolderNode(const char *foldername, struct FolderNode *parentFolder)
{
    struct FolderNode *newNode = (struct FolderNode *)malloc(sizeof(struct FolderNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strncpy(newNode->foldername, foldername, sizeof(newNode->foldername));
    newNode->fileList = NULL;
    newNode->childFolders = NULL;
    newNode->parentFolder = parentFolder;
    newNode->next = NULL;
    return newNode;
}

// Function to add a folder to the linked list of folders
void addFolderToList(struct FolderNode **head, const char *foldername, struct FolderNode *parentFolder)
{
    struct FolderNode *newNode = createFolderNode(foldername, parentFolder);

    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct FolderNode *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to delete a file from the linked list and the file system
void deleteFile(struct FileNode **head, const char *filename)
{
    // Implementation remains the same
    struct FileNode *current = *head;
    struct FileNode *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->filename, filename) == 0)
        {
            if (previous == NULL)
            {
                *head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            remove(filename); // Delete the file from the file system
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Function to open a file in Notepad
void openFileInNotepad(const char *filename)
{
    // Implementation remains the same
    char command[256];
    snprintf(command, sizeof(command), "notepad %s", filename);
    system(command);
}

// Function to print the list of files
void printFileList(struct FileNode *head)
{
    // Implementation remains the same
    struct FileNode *current = head;
    int index = 1;
    while (current != NULL)
    {
        printf("%d. %s\n", index, current->filename);
        current = current->next;
        index++;
    }
}

// Function to free the memory allocated for the linked list
void freeFileList(struct FileNode *head)
{
    // Implementation remains the same
    while (head != NULL)
    {
        struct FileNode *temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to save the linked list to a file
void saveToFile(struct FileNode *head)
{
    // Implementation remains the same
    FILE *file = fopen("filelist.txt", "a");
    if (file == NULL)
    {
        perror("File opening failed");
        return;
    }

    struct FileNode *current = head;
    while (current != NULL)
    {
        fprintf(file, "%s\n", current->filename);
        current = current->next;
    }

    fclose(file);
}

// Function to load the linked list from a file
void loadFromFile(struct FileNode **head)
{
    // Implementation remains the same
    FILE *file = fopen("filelist.txt", "r");
    if (file == NULL)
    {
        perror("File opening failed");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';
        addFileToList(head, buffer);
    }

    fclose(file);
}

// Function to free the memory allocated for the folder tree
void freeFolderTree(struct FolderNode *root)
{
    while (root != NULL)
    {
        struct FolderNode *currentFolder = root;
        root = root->next;
        freeFileList(currentFolder->fileList);
        freeFolderTree(currentFolder->childFolders);
        free(currentFolder);
    }
}

// Function to navigate to a child folder
struct FolderNode *navigateToChildFolder(struct FolderNode *currentFolder, const char *foldername)
{
    struct FolderNode *newFolder = currentFolder->childFolders;
    while (newFolder != NULL)
    {
        if (strcmp(newFolder->foldername, foldername) == 0)
        {
            return newFolder;
        }
        newFolder = newFolder->next;
    }
    return NULL; // Folder not found
}

// Function to navigate to the parent folder
struct FolderNode *navigateToParentFolder(struct FolderNode *currentFolder)
{
    return currentFolder->parentFolder;
}
// Function to delete a folder and its contents
void deleteFolder(struct FolderNode **head, const char *foldername)
{
    struct FolderNode *current = *head;
    struct FolderNode *previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->foldername, foldername) == 0)
        {
            if (previous == NULL)
            {
                *head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            freeFileList(current->fileList);       // Delete files in the folder
            freeFolderTree(current->childFolders); // Delete child folders
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

int main()
{
    struct FolderNode *rootFolder = createFolderNode("Root", NULL);
    struct FolderNode *currentFolder = rootFolder;

    int choice;
    char filename[256];
    char foldername[256];

    while (1)
    {
        printf("\n");
        printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf("__________________________________________________________________\n");
        printf("| NOTE MAKER APPLICATION                                         |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| Current Topic Folder: %s                                     |\n", currentFolder->foldername);
        printf("|----------------------------------------------------------------|\n");
        printf("| NOTES MANAGER                                                  |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 1. Add a Note to the current Topic Folder                      |\n");
        printf("| 2. Open a Note                                                 |\n");
        printf("| 3. View all the Notes in the current Topic Folder              |\n");
        printf("| 4. Delete a Note from the current Topic Folder                 |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| TOPIC FOLDER MANAGER                                           |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 5. Create a new Topic Folder in the current Topic Folder       |\n");
        printf("| 6. Navigate to a child Topic Folder                            |\n");
        printf("| 7. Navigate to the parent Topic Folder                         |\n");
        printf("| 8. Delete the Topic Folder of your choice                      |\n");
        printf("| 9. Exit                                                        |\n");
        printf("__________________________________________________________________\n");
        printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf(" Choose an option: ");
        scanf("%d", &choice);

        

        switch (choice)
        {
        case 1:
            printf("\n");

            printf(" Enter the Note Title (e.g., mynote): ");
            scanf("%s", filename);
            addFileToList(&(currentFolder->fileList), filename);
            break;
        case 2:
            if (currentFolder->fileList != NULL)
            {
                printFileList(currentFolder->fileList);
                printf("\n");

                printf(" Enter the index of the Note to open: ");
                int fileIndex;
                scanf("%d", &fileIndex);

                struct FileNode *current = currentFolder->fileList;
                int currentIndex = 1;
                while (current != NULL && currentIndex < fileIndex)
                {
                    current = current->next;
                    currentIndex++;
                }

                if (current != NULL)
                {
                    openFileInNotepad(current->filename);
                }
                else
                {
                    printf("\n");

                    printf(" Invalid index.\n");
                }
            }
            else
            {
                printf("\n");

                printf(" The Topic Flder is empty.\n");
            }
            break;
        case 3:
            if (currentFolder->fileList != NULL)
            {
                printf("\n");

                printf(" List of Notes in %s:\n", currentFolder->foldername);
                printFileList(currentFolder->fileList);
            }
            else
            {
                printf("\n");

                printf(" The Topic Folder is empty.\n");
            }
            break;
        case 4:
            if (currentFolder->fileList != NULL)
            {
                printFileList(currentFolder->fileList);
                printf("\n");

                printf(" Enter the index of the Note to delete: ");
                int fileIndex;
                scanf("%d", &fileIndex);

                struct FileNode *current = currentFolder->fileList;
                int currentIndex = 1;
                while (current != NULL && currentIndex < fileIndex)
                {
                    current = current->next;
                    currentIndex++;
                }

                if (current != NULL)
                {
                    deleteFile(&(currentFolder->fileList), current->filename);
                    printf("\n");

                    printf(" Note deleted.\n");
                }
                else
                {
                    printf("\n");
                    printf(" Invalid index.\n");
                }
            }
            else
            {
                printf("\n");

                printf(" The Topic Folder is empty.\n");
            }
            break;
        case 5:
            printf("\n");

            printf(" Enter the name of the new Topic Folder: ");
            scanf("%s", foldername);
            addFolderToList(&(currentFolder->childFolders), foldername, currentFolder);
            break;
        case 6:
            printf("\n");

            printf(" Enter the name of the Topic Folder to navigate to: ");
            scanf("%s", foldername);
            struct FolderNode *childFolder = navigateToChildFolder(currentFolder, foldername);
            if (childFolder != NULL)
            {
                currentFolder = childFolder;
            }
            else
            {
                printf("\n");

                printf(" Topic Folder not found.\n");
            }
            break;
        case 7:;
            struct FolderNode *parentFolder = navigateToParentFolder(currentFolder);
            if (parentFolder != NULL)
            {
                currentFolder = parentFolder;
            }
            else
            {
                printf("\n");

                printf(" You are already in the Default Workspace.\n");
            }
            break;
        case 8:
            printf("\n");

            printf(" Enter the name of the Topic Folder to delete: ");
            scanf("%s", foldername);

            if (strcmp(foldername, currentFolder->foldername) == 0)
            {
                printf(" You cannot delete the current Topic Folder.\n");
            }
            else
            {
                deleteFolder(&(currentFolder->childFolders), foldername);
                printf(" Topic Folder deleted.\n");
            }
            break;

        case 9:
            // Free memory used by the tree and exit
            freeFolderTree(rootFolder);
            exit(0);
        default:
            printf("\n");

            printf(" Invalid Menu Choice.\n");
        }
    }

    return 0;
}