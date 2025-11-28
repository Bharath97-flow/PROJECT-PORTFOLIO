#include <stdio.h>
#include <stdlib.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

#define MAX_CONTACTS 100
int ind=0;
void flushinput()
{
    int ch;
    while((ch=getchar())!='\n'&&ch!=EOF);
}
void my_strcpy(char *s1,char *s2)
{
    int i=0;
    while(s2[i])
    {
        s1[i]=s2[i];
        i++;
    }
    s1[i]='\0';
}
int my_strlen(char *s)
{
    int count=0,i=0;
    while(s[i])
    {
        count++;
        i++;
    }
    return count;
}
int my_strcmp(const char *s1, const char *s2)
{
    char c1, c2;

    while (*s1 && *s2)
    {
        
        c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;

        if (c1 != c2)
        return c1 - c2;

        s1++;
        s2++;
    }

    
    c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
    c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;

    return c1 - c2;
}

void sort_ascending(AddressBook *addressBook,char display)
{
    int i,k;
    if(display=='1')
    {
        for(i=0;i<addressBook->contactCount;i++)
        {
            for(k=i+1;k<addressBook->contactCount;k++)
            {
                    if(my_strcmp(addressBook->contacts[i].name,addressBook->contacts[k].name)>0)
                    {
                        Contact temp;
                        temp=addressBook->contacts[i];
                        addressBook->contacts[i]=addressBook->contacts[k];
                        addressBook->contacts[k]=temp;
                    }
                
            }
        }
    }
    else if(display=='2')
    {
        for(i=0;i<addressBook->contactCount;i++)
        {
            for(k=i+1;k<addressBook->contactCount;k++)
            {
                    if(my_strcmp(addressBook->contacts[i].phone,addressBook->contacts[k].phone)>0)
                    {
                        Contact temp;
                        temp=addressBook->contacts[i];
                        addressBook->contacts[i]=addressBook->contacts[k];
                        addressBook->contacts[k]=temp;
                    }
                
            }
        }
    }
    else
    {
        for(i=0;i<addressBook->contactCount;i++)
        {
            for(k=i+1;k<addressBook->contactCount;k++)
            {
                    if(my_strcmp(addressBook->contacts[i].email,addressBook->contacts[k].email)>0)
                    {
                        Contact temp;
                        temp=addressBook->contacts[i];
                        addressBook->contacts[i]=addressBook->contacts[k];
                        addressBook->contacts[k]=temp;
                    }
                
            }
        }
    }

}
void sort_descending(AddressBook *addressBook,char display)
{
    int i,k;
    if(display=='1')
    {
        for(i=0;i<addressBook->contactCount;i++)
        {
            for(k=i+1;k<addressBook->contactCount;k++)
            {
                    if(my_strcmp(addressBook->contacts[i].name,addressBook->contacts[k].name)<0)
                    {
                        Contact temp;
                        temp=addressBook->contacts[i];
                        addressBook->contacts[i]=addressBook->contacts[k];
                        addressBook->contacts[k]=temp;
                    }
                
            }
        }
    }
    else if(display=='2')
    {
        for(i=0;i<addressBook->contactCount;i++)
        {
            for(k=i+1;k<addressBook->contactCount;k++)
            {
                    if(my_strcmp(addressBook->contacts[i].phone,addressBook->contacts[k].phone)<0)
                    {
                        Contact temp;
                        temp=addressBook->contacts[i];
                        addressBook->contacts[i]=addressBook->contacts[k];
                        addressBook->contacts[k]=temp;
                    }
                
            }
        }
    }
    else
    {
        for(i=0;i<addressBook->contactCount;i++)
        {
            for(k=i+1;k<addressBook->contactCount;k++)
            {
                    if(my_strcmp(addressBook->contacts[i].email,addressBook->contacts[k].email)<0)
                    {
                        Contact temp;
                        temp=addressBook->contacts[i];
                        addressBook->contacts[i]=addressBook->contacts[k];
                        addressBook->contacts[k]=temp;
                    }
                
            }
        }
    }
}
int sort(AddressBook *addressBook,char sortCriteria)
{
    int i,k;
    char j;
    do
    {
        for(int t=0;t<60;t++)
        {
           printf("-");
        }
        printf("\n");
        printf("PLEASE SELECT THE LIST METHOD\n1.Sort ASCENDING\n2.Sort DESCENDING\n0.EXIT TO MAIN MENU\nChoice: ");
        scanf(" %c",&j);
        flushinput();
        for(int t=0;t<60;t++)
        {
           printf("-");
        }
        printf("\n");
        if(j=='0')
        {
            return 0;
        }
        if(sortCriteria=='1'&&j=='1'||sortCriteria=='1'&&j=='2')
        {
           if(j=='1')
           {
                sort_ascending(addressBook,sortCriteria);
                break;
           }
           else 
           {
                sort_descending(addressBook,sortCriteria);
                break;
           }
        }
        else if(sortCriteria=='2'&&j=='1'||sortCriteria=='2'&&j=='2')
        {
            if(j=='1')
            {
                sort_ascending(addressBook,sortCriteria);
                break;
            }
            else
            {
                sort_descending(addressBook,sortCriteria);
                break;
            }
        }    
        else if(sortCriteria=='3'&&j=='1'||sortCriteria=='3'&&j=='2')
        {
            if(j=='1')
            {
                sort_ascending(addressBook,sortCriteria);
                break;
            }
            else
            {
                sort_descending(addressBook,sortCriteria);
                break;
            }
        }  
        else
        {
            printf("ENTER A VALID CHARACTER\n");
        }    
        
    }while(1);
    return 1;
    
}
void listContacts(AddressBook *addressBook, char sortCriteria) 
{
    
    if(sort(addressBook,sortCriteria)==0)
    {
        return;
    }
    char *text="LIST OF CONTACTS";
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
    for(int i=0;i<addressBook->contactCount;i++)
    {
       printf("%-20s %-15s %-30s\n", addressBook->contacts[i].name, addressBook->contacts[i].phone, addressBook->contacts[i].email);
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    //loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}
int is_name_valid(char *n)
{
    for(int i=0;n[i];i++)
    {
        if(n[0]==' '||!(n[i]>='A'&&n[i]<='Z'||n[i]>='a'&&n[i]<='z'||n[i]==' '))
        {
            return 0;
        }
    }
    return 1;
}
int is_num_valid(char *num)
{
    for(int i=0;num[i];i++)
    {
        if((num[i]==' '||!(num[i]>='0'&&num[i]<='9')))
        {
            return 0;
        }
        if(my_strlen(num)!=10)
        {
            return 0;
        }
    }
    return 1;
}
int is_email_valid(char *em)
{
    int count=0;
    if(em[0]==' '||!(em[0]>='A'&&em[0]<='Z'||em[0]>='a'&&em[0]<='z'))
    {
        return 0;
    }
    for(int i=0;em[i];i++)
    {
        if(em[i]=='@')
        {
            count++;
        }
        if(em[i]=='.'&&em[i+1]=='c'&&em[i+2]=='o'&&em[i+3]=='m')
        {
            count++;
        }
    }
    if(count==2)
    {
        return 1;
    }
}
int num_exists(AddressBook *addressBook,char *num)
{
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(my_strcmp(num,addressBook->contacts[i].phone)==0)
        {
            return 0;
        }
    }
    return 1;
}
int email_exists(AddressBook *addressBook,char *em)
{
    for(int i=0;i<addressBook->contactCount;i++)
    {
        if(my_strcmp(em,addressBook->contacts[i].email)==0)
        {
            return 0;
        }
    }
    return 1;
}

void createContact(AddressBook *addressBook)
{
        char *text="CREATE CONTACT";
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
	/* Define the logic to create a Contacts */
    printf("Please enter the name,number and email id respectively\n");
    char dummy_name[50],dummy_email[50],dummy_number[11];
    int flag1,flag2,flag3=0;
    flag1=flag2=flag3;
    flushinput();
    do
    {    
        scanf("%49[^\n]",dummy_name);
        flushinput();
        if(dummy_name[0]=='0')
        {
            return ;
        }
        if(is_name_valid(dummy_name))
        {
            flag1++;
        }
        else
        {
            printf("The name you have entered is invalid\nENTER A VALID NAME or Enter 0 to exit back to the  main menu\n");
        }
    }while(flag1!=1);
    do
    {    
        scanf("%10[^\n]",dummy_number);
        flushinput();
        dummy_number[10]='\0';
        if(dummy_number[0]=='0')
        {
            return ;
        }
        if(!is_num_valid(dummy_number))
        {
            printf("INVALID NUMBER ENTER A VALID ONE or Enter 0 to exit back to the  main menu\n");
        }
        if(num_exists(addressBook,dummy_number)&&is_num_valid(dummy_number))
        {
            flag2++;
        }
        if(!num_exists(addressBook,dummy_number))
        {
            printf("NUMBER EXISTS IN DATA BASE, PLEASE RE-ENTER THE VALID ONE or Enter 0 to exit back to the  main menu\n");
        }
    }while(flag2!=1);
    do
    {    
        scanf("%49[^\n]",dummy_email);
        flushinput();
        if(dummy_email[0]=='0')
        {
            return ;
        }
       if(!is_email_valid(dummy_email))
        {
            printf("INVALID EMAIL ENTER A VALID ONE or Enter 0 to exit back to the  main menu\n");
        }
        if(email_exists(addressBook,dummy_email)&&is_email_valid(dummy_email))
        {
            flag3++;
        }
        if(!email_exists(addressBook,dummy_email))
        {
            printf("EMAIL EXISTS IN DATA BASE, PLEASE RE-ENTER THE VALID ONE or Enter 0 to exit back to the  main menu\n");
        }
    }while(flag3!=1);
    if(flag1&&flag2&&flag3)
    {
        my_strcpy(addressBook->contacts[addressBook->contactCount].name,dummy_name);
        my_strcpy(addressBook->contacts[addressBook->contactCount].phone,dummy_number);
        my_strcpy(addressBook->contacts[addressBook->contactCount].email,dummy_email);
        printf("CONTACT ADDED SUCCESSFULLY!\n");
        addressBook->contactCount++;
    }
    return ;
}


void searchContact(AddressBook *addressBook) 
{
    int flag;
       
    do {
         char *text="SEARCH A CONTACT";
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
        char dummy_name[50], dummy_number[11], dummy_email[50], choice;
        

        printf("1.BY NAME\n2.BY NUMBER\n3.BY EMAIL\n0.EXIT TO MAIN MENU\nEnter choice: ");
        scanf(" %c", &choice);
        flushinput();
        if(choice=='0')
        {
            return ;
        }
        flag = 0;

        if (choice == '1') {
            printf("Enter Name to search (or 0 to cancel): ");
            scanf(" %49[^\n]", dummy_name);
            flushinput();
            if (dummy_name[0] == '0') return ;

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (my_strcmp(dummy_name, addressBook->contacts[i].name) == 0) {
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("%-20s %-15s %-30s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                    flag = 1;
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    return ;
                }
                
            }
            if (!flag) 
            {
                printf("Contact not found!\n");
                return ;
            }
        }
        else if (choice == '2') {
            printf("Enter Number to search (or 0 to cancel): ");
            scanf("%10[^\n]", dummy_number);
            flushinput();
            if (dummy_number[0] == '0') return ;

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (my_strcmp(dummy_number, addressBook->contacts[i].phone) == 0) {
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("%-20s %-15s %-30s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                    flag = 1;
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    return ;
                }
               
            }
            if (!flag) 
            {
                printf("Contact not found!\n");
                return ;
            }
        }
        else if (choice == '3') {
            printf("Enter Email to search (or 0 to cancel): ");
            scanf(" %49[^\n]", dummy_email);
            flushinput();
            if (dummy_email[0] == '0') return ;

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (my_strcmp(dummy_email, addressBook->contacts[i].email) == 0) {
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("%-20s %-15s %-30s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                    flag = 1;
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    return ;
                }
            }
            if (!flag) 
            {
                printf("Contact not found!\n");
                return ;
            }

        }
        else {
            printf("Enter a valid choice (1-3) or 0 to exit.\n");
        }
    } while (!flag);

    
}
int searchcontact_temp(AddressBook *addressBook)
{
    char dummy_name[50], dummy_number[11], dummy_email[50], choice;
    int flag;
   do{
     printf("1.SEARCH BY NAME\n2.SEARCH BY NUMBER\n3.SEARCH BY EMAIL\n0.EXIT TO MAIN MENU\nEnter choice: ");
        scanf(" %c", &choice);
        flushinput();
        if(choice=='0')
        {
            return 0;
        }
        flag = 0;

        if (choice == '1') {
            printf("Enter Name to search (or 0 to cancel): ");
            scanf(" %49[^\n]", dummy_name);
            flushinput();
            if (dummy_name[0] == '0') return 0;

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (my_strcmp(dummy_name, addressBook->contacts[i].name) == 0) {
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("%-20s %-15s %-30s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                    flag = 1;
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    return i;
                }
                
            }
            if (!flag) 
            {
                printf("Contact not found!\n");
                return 0;
            }
        }
        else if (choice == '2') {
            printf("Enter Number to search (or 0 to cancel): ");
            scanf("%10[^\n]", dummy_number);
            flushinput();
            if (dummy_number[0] == '0') return 0;

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (my_strcmp(dummy_number, addressBook->contacts[i].phone) == 0) {
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("%-20s %-15s %-30s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                    flag = 1;
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    return i;
                }
               
            }
            if (!flag) 
            {
                printf("Contact not found!\n");
                return 0;
            }
        }
        else if (choice == '3') {
            printf("Enter Email to search (or 0 to cancel): ");
            scanf(" %49[^\n]", dummy_email);
            flushinput();
            if (dummy_email[0] == '0') return 0;

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (my_strcmp(dummy_email, addressBook->contacts[i].email) == 0) {
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    printf("%-20s %-15s %-30s\n",
                        addressBook->contacts[i].name,
                        addressBook->contacts[i].phone,
                        addressBook->contacts[i].email);
                    flag = 1;
                    for(int t=0;t<60;t++)
                    {
                        printf("-");
                    }
                    printf("\n");
                    return i;
                }
            }
            if (!flag) 
            {
                printf("Contact not found!\n");
                return 0;
            }

        }
        else {
            printf("Enter a valid choice (1-3) or 0 to exit.\n");
        }
    } while (!flag);
    return 0;
}
void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
        char *text="EDIT A CONTACT";
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
    flushinput();
    int index;
    index=searchcontact_temp(addressBook);
    int w;
    do
    {
        printf("Edit the above contact by\n1.Name\n2.Number\n3.E-mail\nOption: ");
        scanf(" %d",&w);
        flushinput();
        char dummy_name[50],dummy_email[50],dummy_number[11];
        int flag1,flag2,flag3=0;
        flag1=flag2=flag3;
        if(w==1)
        {   
            do
            {
                printf("Please enter the name to edit: ");
                scanf("%49[^\n]",dummy_name);
                flushinput();
                if(dummy_name[0]=='0')
                {
                    return ;
                }
                if(is_name_valid(dummy_name))
                {
                    my_strcpy(addressBook->contacts[index].name,dummy_name);
                    flag1++;
                    printf("CONTACT ADDED SUCCESSFULLY!\n");
                    addressBook->contactCount++;
                    return;
                }
                else
                {
                    printf("The name you have entered is invalid\nENTER A VALID NAME or Enter 0 to exit back to the  main menu\n");
                }
            }while(flag1!=1);
        }
        else if(w==2)  
        {  
            do
            {   
                printf("Please enter the number to edit: "); 
                scanf(" %10[^\n]",dummy_number);
                flushinput();
                dummy_number[10]='\0';
                if(dummy_number[0]=='0')
                {
                    return ;
                }
                if(!is_num_valid(dummy_number))
                {
                    printf("INVALID NUMBER ENTER A VALID ONE or Enter 0 to exit back to the  main menu\n");
                }
                if(num_exists(addressBook,dummy_number)&&is_num_valid(dummy_number))
                {
                my_strcpy(addressBook->contacts[index].phone,dummy_number);
                flag2++;
                printf("CONTACT ADDED SUCCESSFULLY!\n");
                    addressBook->contactCount++;
                    return ;
                }
                if(!num_exists(addressBook,dummy_number))
                {
                    printf("NUMBER EXISTS IN DATA BASE, PLEASE RE-ENTER THE VALID ONE or Enter 0 to exit back to the  main menu\n");
                }
            }while(flag2!=1);
        }
        else if(w==3)
        {  
            do
            { 
                printf("Please enter the e-mail to edit: ");   
                scanf(" %49[^\n]",dummy_email);
                flushinput();
                if(dummy_email[0]=='0')
                {
                    return ;
                }
            if(!is_email_valid(dummy_email))
                {
                    printf("INVALID EMAIL ENTER A VALID ONE or Enter 0 to exit back to the  main menu\n");
                }
                if(email_exists(addressBook,dummy_email)&&is_email_valid(dummy_email))
                {
                my_strcpy(addressBook->contacts[index].email,dummy_email);
                flag3++;
                printf("CONTACT ADDED SUCCESSFULLY!\n");
                addressBook->contactCount++;
                return ;
                }
                if(!email_exists(addressBook,dummy_email))
                {
                    printf("EMAIL EXISTS IN DATA BASE, PLEASE RE-ENTER THE VALID ONE or Enter 0 to exit back to the  main menu\n");
                }
            }while(flag3!=1);
        }
        else if(w==0)
        {
            return ;
        }
        else
        {
            printf("Please select valid options from (1-3) or select 0 to go back to main menu\n");
        }
    }while(1);
}
void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
        char *text="DELETE A CONTACT";
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
    int index;
    index=searchcontact_temp(addressBook);
    if(index==0)
    {
        return ;
    }
    int j;
    j=index;
    char opt;
    printf("Are you sure you want to delete the above contact?");
    while(1)
    {
        printf("\npress 1 to confirm or 0 to exit to main menu\n");
        scanf("%c",&opt);
        flushinput();
        if(opt=='0')
        {
            return ;
        }
        else if(opt=='1')
        {
            while(j<addressBook->contactCount-1)
            {
                addressBook->contacts[j]=addressBook->contacts[j+1];
                j++;
            }
            printf("Contact deleted successfully!\n");
            addressBook->contactCount--;
            break;
        }
        else
        {
            printf("Enter a valid option");
        }
    }
}


           
