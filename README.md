# NOTE MAKER APPLICATION

#OVERVIEW
Note Taking Back-end Application using Data Structures in C language. 
It is an implementation of a simple text-based file and folder management system. 
Users can create folders, add, open, view, and delete notes (text files) within those folders. 
The program also supports navigation between folders.

#CONTENTS
Data Structures used in the following project are:
1) Pointers
2) Arrays
3) Linked List

#CODE-BREAKDOWN
1) struct FileNode and struct FolderNode: These structures represent individual files and folders, respectively. FileNode contains the filename and a pointer to the next file in the list. FolderNode contains the    folder name, pointers to child folders and files, a reference to the parent folder, and a pointer to the next folder in the list.
2) createFileNode and createFolderNode: These functions allocate memory for new file and folder nodes, respectively, and initialize their fields.
3) addFileToList and addFolderToList: These functions add a file or folder node to the linked list of files or folders.
4) deleteFile: This function deletes a file from both the linked list and the file system by using the remove function.
5) openFileInNotepad: This function opens a file in the Notepad text editor. It uses the system function to execute a command to open the file.
6) printFileList: This function prints the list of files in the current folder.
7) freeFileList: This function frees the memory allocated for the linked list of files.
8) saveToFile and loadFromFile: These functions are used to save the list of files to a text file and load them from a file, respectively.
9) freeFolderTree: This function frees the memory allocated for the entire folder tree, including all child folders and their contents.
10) navigateToChildFolder and navigateToParentFolder: These functions are used for folder navigation within the tree.

 In the main function, the program provides a menu for various actions, such as adding, opening, viewing, and deleting notes, creating new folders, navigating to child or parent folders, and deleting folders. The program continuously loops to receive user input until the user chooses to exit.
 Overall, this code serves as a basic text-based file and folder management system, allowing users to organize notes and folders in a hierarchical structure. It demonstrates the use of linked lists and file I/O operations in C.

#FUTURE DEVELOPMENTS
1) Front end UI.
2) Better interaction with system.
3) Faster execution with better data structure implementation.


  THANK YOU
  
