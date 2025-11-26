#include<stdio.h>
#include<string.h>
#include"main.h"
int validatemp3(char argv[])
{
  if(strstr(argv,".mp3")!=NULL)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
 
void main(int argc,char *argv[])
{
    if(argv[1]==NULL)
    {
        printf("INVALID ARGUEMENTS.\n");
    }
    else if(!strcmp(argv[1],"--help"))
    {
        for(int i=0;i<30;i++)
        {
            printf("-");
        }
        printf("HELP MENU");
        for(int i=0;i<30;i++)
        {
            printf("-");
        }
        printf("\n");
        printf("1 -v --> view mp3 file contents.\n");
        printf("2 -e --> edit mp3 file.\n");
        printf("\t2.1 -t --> edit song title.\n");
        printf("\t2.2 -a --> edit artist name.\n");
        printf("\t2.3 -A --> edit song title.\n");
        printf("\t2.4 -y --> edit song title.\n");
        printf("\t2.5 -m --> edit song title.\n");
        printf("\t2.6 -c --> edit song title.\n");
        for(int i=0;i<70;i++)
        {
            printf("-");
        }
    }
    else if(!strcmp(argv[1], "-e"))
    {
        // Find the .mp3 argument
        int mp3_index = -1;
        for (int i = 2; i < argc; i++)
        {
            if (validatemp3(argv[i]))
            {
                mp3_index = i;
                break;
            }
        }

        if (mp3_index == -1)
        {
            printf("INVALID MP3 FILE.\n");
            return;
        }

        printf("VALID MP3 FILE\n");

        // Combine all words between argv[3] and mp3_index-1
        char new_value[256] = "";
        for (int i = 3; i < mp3_index; i++)
        {
            strcat(new_value, argv[i]);
            if (i < mp3_index - 1)
                strcat(new_value, " "); // Add spaces between words
        }

        FILE *fptr_sample = fopen(argv[mp3_index], "r+b");
        if (!fptr_sample)
        {
            perror("Error opening file");
            return;
        }

        edit(argv[2], new_value, fptr_sample);
        fclose(fptr_sample);
    }
    else if(!strcmp(argv[1],"-v"))
    { 
        if(validatemp3(argv[2]))
        {
            printf("VIEW MODE SELECTED.\n");
            FILE *fptr_sample=fopen("sample.mp3","r+");
            char str[4];
            fread(str,1,3,fptr_sample);//ID3 VALIDATION
            if(strcmp(str,"ID3")==0)
            {
                printf("ID3 VALID\n");
                char str2[4];
                fseek(fptr_sample,3,SEEK_SET);
                fread(str2,1,2,fptr_sample);
                if(str2[0]==3&&str2[1]==0)
                {
                    printf("VERSION VALID\n");
                    view(fptr_sample);
                }
                else
                {
                    printf("VERSION INVALID\n");
                }
            }
            
        }
    }
}