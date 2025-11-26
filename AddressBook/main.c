#include <stdio.h>
#include "contact.h"
#include "file.h"
int main() {
    char choice;
    AddressBook addressBook;
    loadContactsFromFile(&addressBook);

    do {
        char *text="ADDRESS BOOK MENU";
        for(int t=0;t<80;t++)
        {
            printf("-");
        }
        printf("\n%48s\n",text); 
        for(int t=0;t<80;t++)
        {
            printf("-");
        }
        printf("\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts\n");		
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        switch (choice) {
            case '1':
                createContact(&addressBook);
                break;
            case '2':
                searchContact(&addressBook);
                break;
            case '3':
                editContact(&addressBook);
                break;
            case '4':
                deleteContact(&addressBook);
                break;
            case '5': 
                {
                    char ch;
                    do {
                        for(int t=0;t<60;t++)
                        {
                            printf("-");
                        }
                        printf("\n");
                        printf("DISPLAY PRIORITY\n");
                        printf("1. SORTED NAME\n");
                        printf("2. SORTED NUMBER\n");
                        printf("3. SORTED EMAIL\n");
                        printf("0. EXIT TO MAIN MENU\n");
                        printf("Enter your choice: ");
                        scanf(" %c", &ch);
                        int flag=0;
                        switch (ch) 
                        {
                            case '0':
                                break;
                            case '1':
                            case '2':
                            case '3':
                                
                                listContacts(&addressBook, ch);
                                flag=1;
                                break;
                            default:
                                printf("Please enter a valid choice (1-3).\n");
                        }
                        if(ch=='0'||flag==1)
                        {
                            break;
                        }
                    } while (1);
                    break;
                }
            
            case '6':
                saveContactsToFile(&addressBook);
                printf("Saved the contact successfully\n");
                //saveContactsToFile(&addressBook);
                break;
            case '7':
            char *tex="EXIT SUCCESSFUL";
                for(int t=0;t<80;t++)
                {
                    printf("-");
                }
                printf("\n%48s\n",tex); 
                for(int t=0;t<80;t++)
                {
                    printf("-");
                }
                printf("\n");
                        break;
            default:
                
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != '7');
    
       return 0;
}
