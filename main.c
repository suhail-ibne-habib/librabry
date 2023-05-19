#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void password();
void mainMenu();
void addBook();
void viewList();
void searchBook();
void editBook();
void deleteBook();

int wrongPassCount = 1;

void toLowerCase(char *str);

struct Books{
  long int id;
  char name[25];
  char author[16];
  int price;
} book;

FILE *bookFile;

int main(){
    password();

    return 0;
}

void addBook(){
    int exist = 0;
    int isValid = 0;
    char name[25];
    char author[16];
    int price = 0;

    time_t seconds;
    seconds = time(NULL);

    bookFile = fopen("books.txt", "ab+");

    system("cls");
    printf("\n\n\t\t******************Add Book******************\n\n");

    printf( "\t\tEnter Book Name: " );
    fflush(stdin);
    gets(name);
    //scanf("%s", name);

    rewind(bookFile);
    while(fread(&book, sizeof(book), 1, bookFile) == 1){
        if( strcmp( book.name, name ) == 0 ){
            exist = 1;
        }
    }

    if( exist == 0 ){
        strcpy(book.name, name);
        printf( "\t\tEnter author name: " );
        fflush(stdin);
        gets(author);
        strcpy(book.author,author);

        printf( "\t\tEnter price: " );
        scanf("%d", &price);


        if(price != 0 && price > 0 && price != "NAN"){
            isValid = 1;
        }

        if( isValid == 1 ){

            book.price = price;

            book.id = seconds;

            fseek(bookFile,0, SEEK_END);
            size_t written = fwrite(&book, sizeof(book), 1, bookFile);
            fclose(bookFile);

            if(written != 0){
                printf("\n\t\tBook added!\n\t\tEnter any key to return main menu..");
            }else{
                printf("\n\t\tBook couldn't be added!\n\t\tEnter any key to return main menu..");
            }

            fflush(stdin);
            getchar();
            mainMenu();
        }else{
            printf("\n\t\tPrice can't be negative or zero!\n\t\tEnter any key to try again..");
            fflush(stdin);
            getchar();
            mainMenu();
        }
    }else{
        printf("\n\t\tThe book already exists!\n\t\tEnter any key to try again..");
        fflush(stdin);
        getchar();
        mainMenu();
    }


}

void viewList(){
    system("cls");
    printf("\n\n\t\t******************Library Management******************\n\n");
    printf("\n\n\t\tID\t\t\tName\t\t\tAuthor\t\tPrice\n\n");

    int count = 0;

    bookFile = fopen("books.txt", "rb");
    while( fread(&book, sizeof(book), 1, bookFile) == 1 ){
        printf( "\n\t\t%-8d\t\t%-24s%-16s%-16d\n\n", book.id, book.name, book.author, book.price );
        count += sizeof(book);
    }

    fclose(bookFile);
    printf("\n\n\t\tTotal books: %d", count/sizeof(book));
    printf("\n\n\t\t****************************************************");
    printf("\n\n\t\tEnter any key to visit main menu...");
    fflush(stdin);
    getchar();
    mainMenu();
}

void searchBook(){
    system("cls");
    printf("\n\n\t\t*********************** Search Book ***********************\n\n");
    printf("\t\tEnter book name: ");
    char searchQueryStr[25];
    fflush(stdin);
    gets(searchQueryStr);

    toLowerCase(searchQueryStr);

    int count = 0;

    bookFile = fopen("books.txt", "rb");
    printf("\n\n\t\tID\t\t\tName\t\t\tAuthor\t\tPrice\n\n");
    while( fread(&book, sizeof(book), 1, bookFile) == 1 ){
        char str[25];
        strcpy(str, book.name);
        toLowerCase(str);

        if (strstr(str, searchQueryStr) != NULL) {
            printf( "\n\t\t%-8d\t\t%-24s%-16s%-16d\n\n", book.id, book.name, book.author, book.price );

            count += sizeof(book);
        }

    }

    fclose(bookFile);
    printf("\n\n\t\tTotal books found: %d", count/sizeof(book));
    printf("\n\n\t\t****************************************************");
    printf("\n\n\t\tEnter any key to visit main menu...");
    fflush(stdin);
    getchar();
    mainMenu();

}

void editBook(){
    system("cls");
    printf("\n\n\t\t*********************** Edit Book ***********************\n\n");
    printf("\t\tEnter book id: ");

    long int bookId = 0;
    scanf("%ld", &bookId);

    bookFile = fopen("books.txt", "rb+");

    while( fread(&book, sizeof(book), 1, bookFile) == 1 ){
        if(bookId == book.id){
            printf("\n\n\t\tEnter zero(0) to pass...\n\n");

            char name[25];
            char author[16];
            int price = 0;

            printf("\n\t\tEnter name: ");
            fflush(stdin);
            gets(name);

            printf("\n\t\tEnter author: ");
            fflush(stdin);
            gets(author);

            printf("\n\t\tEnter price: ");
            scanf("%d", &price);

            if (strcmp(name, "0") == 0) {
                strcpy(name, book.name);
            }

            if (strcmp(author, "0") == 0) {
                strcpy(author, book.author);
            }

            if(price == 0){
                price = book.price;
            }

            strcpy(book.name, name);
            strcpy(book.author, author);
            book.price = price;

            fseek( bookFile, ftell(bookFile)- sizeof(book), 0 );

            fwrite(&book, sizeof(book), 1, bookFile);
            fclose(bookFile);

        }
    }

    printf("\n\n\t\tBook edited success!");
    printf("\n\n\t\t****************************************************");
    printf("\n\n\t\tEnter any key to visit main menu...");
    fflush(stdin);
    getchar();
    mainMenu();
}

void deleteBook(){
    int found = 0;
    system("cls");
    printf("\n\n\t\t*********************** Delete book ***********************\n\n");
    printf("\t\tEnter book id: ");

    long int bookId = 0;
    scanf("%ld", &bookId);

    bookFile = fopen("books.txt", "rb+");

    FILE *temp;
    temp = fopen("temp.txt", "ab");

    while(fread(&book, sizeof(book), 1, bookFile) == 1){
        if( bookId == book.id ){
            found = 1;
        }else{
            fseek(temp, ftell(temp)-sizeof(book), 0);
            fwrite(&book, sizeof(book), 1, temp);
        }
    }

    fclose(bookFile);
    fclose(temp);
    remove("books.txt");
    rename("temp.txt", "books.txt");

    (found == 1) ? printf("\t\tRecord deleted.") : printf("\t\tRecord not found!");
    printf("\n\n\t\t****************************************************");
    printf("\n\n\t\tEnter any key to visit main menu...");
    fflush(stdin);
    getchar();
    mainMenu();


}

void mainMenu(){
    system("cls");
    printf("\n\n\t\t*********************** main menu ***********************\n\n");
    printf("\n\n\t\t1. add book\n");
    printf("\t\t2. view book list\n");
    printf("\t\t3. search book\n");
    printf("\t\t4. edit book\n");
    printf("\t\t5. delete book\n");
    printf("\t\t0. exit\n");
    printf("\n\t\t*********************************************************\n\n");
    printf("\t\tEnter your choice: ");

    int choise = 0;
    scanf("%d", &choise);

    switch(choise){
        case 1:
          addBook();
          break;

        case 2:
          viewList();
          break;
        case 3:
            searchBook();
            break;
        case 4:
            editBook();
            break;
        case 5:
            deleteBook();
            break;
        case 0:
            exit(0);
            break;
        default:
            printf("\t\tWrong option! Try again.");
            fflush(stdin);
            getchar();
            mainMenu();
    };



}

void password(){
    system("cls");

    printf("\n\n\t\t******************Library Management******************\n\n");

    char adminPass[10] = "eu_lib";
    char userPass[10];

    printf("\n\n\t\tEnter Password: ");
    scanf("%s", userPass);

    if(strcmp(adminPass, userPass)== 0){
        printf("\n\n\t\tPassword Matched!\n\t\tEnter any key to continue..");
        mainMenu();
    }else{
        if(wrongPassCount == 3){
            exit(0);
        }
        printf("\n\n\t\tWrong Password %d times out of 3!\n\t\tTry again..", wrongPassCount);
        wrongPassCount++;
        fflush(stdin);
        getchar();
        password();
    }

}

void toLowerCase(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }
}

