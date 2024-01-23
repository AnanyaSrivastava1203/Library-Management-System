#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Book
{
    int book_id;
    char title[100];
    char author[100];
    int quantity;
    int price;
    struct Book *next;
};

struct Reader
{
    int reader_id;
    char reader_name[100];
    struct Reader *next;
};

struct IssueRecord
{
    int book_id;
    int reader_id;
    char title[100];
    char reader_name[100];
    time_t issue_date;
    int quantity;
    struct IssueRecord *next;
};

struct Book *book_head = NULL;
struct Reader *reader_head = NULL;
struct IssueRecord *issue_record_head = NULL;

void addBook(int id, char title[], char author[], int quantity, int price);
void displayBooks();
void issueBook(char title[], int q, int reader_id, char reader_name[]);
void addReader(int id, char name[]);
void displayReaders();
void deleteReader(int id);
void displayIssueRecords();
void deleteBook(int id);
void searchReader(int id);
void submitBook(char title[], int q, int reader_id);
void searchBook(char title[]);

int main()
{
    printf("\nWelcome to E-Library\n");

    char title[100], author[100], name[100];
    int id, quantity, price, reader_id, ch;

    while (1)
    {
        printf("\nE-Library MENU:\n");
        printf("1. Add Books                      ");
        printf("2. Issue Books\n");
        printf("3. Book List                      ");
        printf("4. Add Reader\n");
        printf("5. Display Readers                ");
        printf("6. Delete Reader\n");
        printf("7. Display Issue Records          ");
        printf("8. Delete Book\n");
        printf("9. Search Reader                  ");
        printf("10. Submit a book\n");
        printf("11. Search a book                 ");
        printf("12. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            printf("Enter the book's name: ");
            scanf("%99s", title);
            printf("Enter the book's id: ");
            scanf("%d", &id);
            printf("Enter the book's author: ");
            scanf("%99s", author);
            printf("Enter the number of copies of the book: ");
            scanf("%d", &quantity);
            printf("Enter the cost of each book: ");
            scanf("%d", &price);
            addBook(id, title, author, quantity, price);
            break;
        case 2:
            printf("Enter the book's name: ");
            scanf("%99s", title);
            printf("Enter the number of copies of the book: ");
            scanf("%d", &quantity);
            printf("Enter the reader's name: ");
            scanf("%99s", name);
            printf("Enter the reader's id: ");
            scanf("%d", &reader_id);
            issueBook(title, quantity, reader_id, name);
            break;
        case 3:
            displayBooks();
            break;
        case 4:
            printf("Enter the reader's name: ");
            scanf("%99s", name);
            printf("Enter the reader's id: ");
            scanf("%d", &reader_id);
            addReader(reader_id, name);
            break;
        case 5:
            displayReaders();
            break;
        case 6:
            printf("Enter the reader's id to be deleted: ");
            scanf("%d", &reader_id);
            deleteReader(reader_id);
            break;
        case 7:
            displayIssueRecords();
            break;
        case 8:
            printf("Enter the book's id to be deleted: ");
            scanf("%d", &id);
            deleteBook(id);
            break;
        case 9:
            printf("Enter the reader's id to search: ");
            scanf("%d", &reader_id);
            searchReader(reader_id);
            break;
        case 10:
            printf("Enter the book's name to submit: ");
            scanf("%99s", title);
            printf("Enter the number of copies of the book to submit: ");
            scanf("%d", &quantity);
            printf("Enter the reader's id: ");
            scanf("%d", &reader_id);
            submitBook(title, quantity, reader_id);
            break;
        case 11:
            printf("Enter the book's name to search: ");
            scanf("%99s", title);
            searchBook(title);
            break;
        case 12:
            printf("Exiting the program.\n");
            return 0;
        default:
            printf("Invalid choice\n");
        }
    }

    return 0;
}

void addBook(int id, char title[], char author[], int quantity, int price)
{
    struct Book *newBook = (struct Book *)malloc(sizeof(struct Book));
    if (newBook == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strcpy(newBook->author, author);
    newBook->book_id = id;
    strcpy(newBook->title, title);
    newBook->quantity = quantity;
    newBook->price = price;
    newBook->next = book_head;
    book_head = newBook;

    printf("Book added successfully\n");
}

void displayBooks()
{
    struct Book *current = book_head;

    if (current == NULL)
    {
        printf("Library is empty\n");
    }
    else
    {
        printf("Library:\n");
        printf("%-10s %-30s %-20s %-10s %-10s\n", "Book ID", "Title", "Author", "Quantity", "Price");
        printf("==========================================================\n");

        while (current != NULL)
        {
            printf("%-10d %-30s %-20s %-10d %-10d\n", current->book_id, current->title, current->author, current->quantity, current->price);
            current = current->next;
        }
    }

    printf("==================================\n");
}

void issueBook(char title[], int q, int reader_id, char reader_name[])
{
    if (!isReaderExists(reader_id))
    {
        printf("Sorry! No such reader exists. First, get the reader added.\n");
        printf("==================================\n");
        return;
    }

    struct Book *currentBook = book_head;
    struct Book *prevBook = NULL;

    while (currentBook != NULL)
    {
        if (strcmp(currentBook->title, title) == 0)
        {
            if (currentBook->quantity >= q)
            {
                // Update book quantity
                currentBook->quantity -= q;

                // Add issue record
                struct IssueRecord *newRecord = (struct IssueRecord *)malloc(sizeof(struct IssueRecord));
                if (newRecord == NULL)
                {
                    printf("Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }

                newRecord->book_id = currentBook->book_id;
                strcpy(newRecord->title, currentBook->title);
                newRecord->reader_id = reader_id;
                strcpy(newRecord->reader_name, reader_name);
                newRecord->quantity = q;
                newRecord->issue_date = time(NULL);
                newRecord->next = issue_record_head;
                issue_record_head = newRecord;

                printf("Book issued successfully\n");
                return;
            }
            else
            {
                printf("Insufficient books in the library\n");
                printf("==================================\n");
                return;
            }
        }

        prevBook = currentBook;
        currentBook = currentBook->next;
    }

    printf("This book isn't available in the library\n");
    printf("==================================\n");
}

int isReaderExists(int reader_id)
{
    struct Reader *currentReader = reader_head;

    while (currentReader != NULL)
    {
        if (currentReader->reader_id == reader_id)
        {
            return 1; // Reader exists
        }

        currentReader = currentReader->next;
    }

    return 0; // Reader doesn't exist
}

void addReader(int id, char name[])
{
    struct Reader *newReader = (struct Reader *)malloc(sizeof(struct Reader));
    if (newReader == NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newReader->reader_id = id;
    strcpy(newReader->reader_name, name);
    newReader->next = reader_head;
    reader_head = newReader;

    printf("Reader added successfully\n");
}

void displayReaders()
{
    struct Reader *current = reader_head;

    if (current == NULL)
    {
        printf("No readers in the library\n");
    }
    else
    {
        printf("Readers:\n");
        printf("%-10s %-30s\n", "Reader ID", "Reader Name");
        printf("==================================\n");

        while (current != NULL)
        {
            printf("%-10d %-30s\n", current->reader_id, current->reader_name);
            current = current->next;
        }
    }

    printf("==================================\n");
}

void deleteReader(int id)
{
    struct Reader *current = reader_head;
    struct Reader *prev = NULL;

    while (current != NULL)
    {
        if (current->reader_id == id)
        {
            if (prev == NULL)
            {
                reader_head = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            free(current);
            printf("Reader with id %d deleted successfully\n", id);
            return;
        }

        prev = current;
        current = current->next;
    }

    printf("Reader with id %d not found\n", id);
    printf("==================================\n");
}

void displayIssueRecords()
{
    struct IssueRecord *current = issue_record_head;

    if (current == NULL)
    {
        printf("No issue records\n");
    }
    else
    {
        printf("Issue Records:\n");
        printf("%-10s %-30s %-10s %-30s %-10s %-20s\n", "Book ID", "Book Title", "Reader ID", "Reader Name", "Quantity", "Issue Date");
        printf("===========================================================================================\n");

        while (current != NULL)
        {
            printf("%-10d %-30s %-10d %-30s %-10d %-20s", current->book_id, current->title, current->reader_id, current->reader_name, current->quantity, asctime(localtime(&current->issue_date)));
            current = current->next;
        }
    }

    printf("==================================\n");
}

void deleteBook(int id)
{
    struct Book *current = book_head;
    struct Book *prev = NULL;

    while (current != NULL)
    {
        if (current->book_id == id)
        {
            if (prev == NULL)
            {
                book_head = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            free(current);
            printf("Book with id %d deleted successfully\n", id);
            return;
        }

        prev = current;
        current = current->next;
    }

    printf("Book with id %d not found\n", id);
    printf("==================================\n");
}

void searchReader(int id)
{
    struct Reader *currentReader = reader_head;

    while (currentReader != NULL)
    {
        if (currentReader->reader_id == id)
        {
            printf("Reader Details:\n");
            printf("Reader ID: %d\n", currentReader->reader_id);
            printf("Reader Name: %s\n", currentReader->reader_name);

            // Display books issued by the reader
            struct IssueRecord *currentRecord = issue_record_head;
            printf("Books Issued by the Reader:\n");

            if (currentRecord == NULL)
            {
                printf("No books issued\n");
            }
            else
            {
                printf("%-10s %-30s %-10s %-30s %-10s %-20s\n", "Book ID", "Book Title", "Reader ID", "Reader Name", "Quantity", "Issue Date");
                printf("===========================================================================================\n");
                while (currentRecord != NULL)
                {
                    if (currentRecord->reader_id == id)
                    {
                        printf("%-10d %-30s %-10d %-30s %-10d ", currentRecord->book_id, currentRecord->title, currentRecord->reader_id, currentRecord->reader_name, currentRecord->quantity);

                        // Display "nil" if issue date is not available
                        if (currentRecord->issue_date != 0)
                        {
                            printf("%-20s", asctime(localtime(&currentRecord->issue_date)));
                        }
                        else
                        {
                            printf("%-20s", "nil");
                        }

                        printf("\n");
                    }
                    currentRecord = currentRecord->next;
                }
                printf("==================================\n");
            }

            return;
        }

        currentReader = currentReader->next;
    }

    printf("Reader with id %d not found\n", id);
    printf("==================================\n");
}

void submitBook(char title[], int q, int reader_id)
{
    struct IssueRecord *currentRecord = issue_record_head;
    struct IssueRecord *prevRecord = NULL;

    while (currentRecord != NULL)
    {
        if (strcmp(currentRecord->title, title) == 0 && currentRecord->reader_id == reader_id)
        {
            // Update book quantity
            struct Book *currentBook = book_head;
            while (currentBook != NULL)
            {
                if (strcmp(currentBook->title, title) == 0)
                {
                    currentBook->quantity += q;
                    break;
                }
                currentBook = currentBook->next;
            }

            // Delete the issue record
            if (prevRecord == NULL)
            {
                issue_record_head = currentRecord->next;
            }
            else
            {
                prevRecord->next = currentRecord->next;
            }

            free(currentRecord);
            printf("Book submitted successfully\n");
            return;
        }

        prevRecord = currentRecord;
        currentRecord = currentRecord->next;
    }

    printf("No matching records found for submission\n");
    printf("==================================\n");
}

void searchBook(char title[])
{
    struct Book *current = book_head;

    while (current != NULL)
    {
        if (strcmp(current->title, title) == 0)
        {
            printf("Book Details:\n");
            printf("Book ID: %d\n", current->book_id);
            printf("Title: %s\n", current->title);
            printf("Author: %s\n", current->author);
            printf("Quantity: %d\n", current->quantity);
            printf("Price: %d\n", current->price);
            return;
        }

        current = current->next;
    }

    printf("Book with title '%s' not found\n", title);
    printf("==================================\n");
}