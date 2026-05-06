# Text Editor Application Using Queues and Stacks

## 1. Project Overview

This project is a simple menu-driven text editor written in C. The program loads an initial text from a file, lets the user insert and remove strings from the text, supports Undo and Redo operations, and finally saves the updated text into an output file.

The main goal of this project is to apply the concepts of **queues** and **stacks** in a real and useful application. The queue is used to handle the insertion of multiple words in order, and the stack is used to keep track of the user's actions so they can be undone or redone later.

## 2. Course Information

- **Course:** Data Structures and Algorithms
- **Course Code:** COMP2421
- **Project:** Queues
- **Language:** C

## 3. Main Idea

The text is stored inside a normal character array. The user can change the text through a menu by inserting or removing strings.

To make this work in a clean way, the program uses two main data structures:

- A **Queue** to manage the words that need to be inserted in order. When the user enters a string made of several words, the program splits the string into words, places them into a queue, then inserts them one by one into the text.
- Two **Stacks** (an Undo Stack and a Redo Stack) to remember the operations the user did, so the program can undo or redo them later.

## 4. Data Structures Used

The program uses the following data structures:

- **Queue:** Implemented using linked nodes, used when inserting a string of multiple words. Each word is enqueued first, then dequeued and inserted into the text in order (FIFO).
- **Stack (Undo and Redo):** Implemented using linked nodes. Each node in the stack stores:
  - The string related to the operation.
  - The index where the operation happened.
  - The type of the operation (insert or remove).
- **Character array** to store the text being edited.
- **Dynamic memory allocation** using `malloc` and `free` for queue and stack nodes.

## 5. Program Features

The program supports the following operations through its menu:

1. Load the initial text from the input file `originalText.txt`.
2. Print the loaded text.
3. Insert strings into the text. The user can choose where to insert:
   - At the end of the text.
   - At the beginning of the text.
   - In the middle, after a specific word or substring.
4. Remove a string or a word from the text.
5. Perform an **Undo** operation to reverse the last action.
6. Perform a **Redo** operation to reapply the last undone action.
7. Print the contents of both the **Undo Stack** and the **Redo Stack**.
8. Save the updated text to the output file `output.txt`.
9. Exit the program.

The program combines several concepts together: queues, stacks, file handling, insertion, deletion, undo, redo, string processing, and dynamic memory allocation.

## 6. Input File Format

The input file is named `originalText.txt` and must be placed in the same folder as the executable.

The file contains a single line of text that will be loaded into the program. For example:

```
This is the original text used by the editor
```

The program reads this line using `fgets` and stores it inside a character array, then the user can edit it through the menu.

## 7. Output File

When the user selects option **8**, the current text (after all the insertions, deletions, undo, and redo operations) is saved into the output file `output.txt`.

For example, after inserting and removing some words, the saved file may look like:

```
This is the updated text after editing
```

## 8. How Undo and Redo Work

The Undo and Redo operations are based on two stacks: the **Undo Stack** and the **Redo Stack**.

- When the user **inserts** a string, an "insert" operation is pushed into the Undo Stack along with the inserted word and its index.
- When the user **removes** a string, a "remove" operation is pushed into the Undo Stack along with the removed word and its index.
- When the user chooses **Undo**:
  - The top of the Undo Stack is taken.
  - If it was an insert operation, the inserted word is removed from the text.
  - If it was a remove operation, the removed word is inserted again at the same index.
  - The reversed operation is then pushed into the Redo Stack.
- When the user chooses **Redo**:
  - The top of the Redo Stack is taken.
  - The operation is performed again on the text (insert or remove).

This way the user can step backward and forward through the changes they made to the text.

## 9. Example Menu

When the program runs, the following menu is displayed:

```
Welcome to my text editor application
Menu of my application
Please select an operation
1-Load the input file which contains the initial text
2-Print the loaded text
3-Insert strings to the text
4-Remove strings from the text
5-Perform Undo operation
6-Perform Redo operation
7-Print the Undo Stack and the Redo stack
8-Save the updated text to the output file
9-Exit
```

The user types the number of the operation, and the program performs it. The menu keeps appearing until the user chooses option 9 to exit.

## 10. Notes about the Implementation

- The Undo and Redo stacks are created at the start of the program and disposed of when the user exits.
- A **header node** is used at the top of the stack and the front of the queue, which makes the push, pop, enqueue, and dequeue operations easier to write.
- When inserting more than one word at a time, the input string is split using `strtok`, and each word is added to the queue first, then inserted in order. This way the words are inserted in the same order the user wrote them.
- For inserting after a specific word, the program searches for the word using `strcasecmp` so the search is case-insensitive.
- A few global flags (`load`, `flag`, `flag1`, `flag2`) are used to handle special cases, such as inserting a word for the first time at the end of the text or adding a string to the beginning.
- Memory is freed for stack nodes whenever they are popped, and for queue nodes whenever they are dequeued, to avoid memory leaks.
- The `printStackContent` function uses two temporary stacks to print the stack from bottom to top without losing the original order.

