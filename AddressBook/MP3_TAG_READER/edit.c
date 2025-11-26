#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"

void find_frame(FILE *fp, const char *target, const char *new_value)
{
    fseek(fp, 0, SEEK_SET); // <-- FIX: Ensure file pointer starts from beginning

    char id3_header[10];
    fread(id3_header, 1, 10, fp); // Read ID3 header first

    while (1)
    {
        char frame_header[10];
        if (fread(frame_header, 1, 10, fp) != 10)
            break; // EOF or corrupted frame

        if (frame_header[0] == 0) // Padding reached
            break;

        char frame_id[5];
        memcpy(frame_id, frame_header, 4);
        frame_id[4] = '\0';

        unsigned int frame_size = (frame_header[4] << 24) |
                                  (frame_header[5] << 16) |
                                  (frame_header[6] << 8) |
                                  (frame_header[7]);

        long frame_start = ftell(fp);

        // Compare current frame ID with target
        if (strcmp(frame_id, target) == 0)
        {
            unsigned char encoding_byte;
            fread(&encoding_byte, 1, 1, fp);
            long data_start = ftell(fp);

            int data_len = frame_size - 1; // 1 byte encoding info
            int new_len = strlen(new_value);
            if (new_len > data_len)
                new_len = data_len;

            // <-- FIX: Clear old frame data before writing new value
            fseek(fp, data_start, SEEK_SET);
            for (int i = 0; i < data_len; i++)
                fputc('\0', fp);

            // <-- FIX: Write new value
            fseek(fp, data_start, SEEK_SET);
            fwrite(new_value, 1, new_len, fp);
            fflush(fp); // <-- FIX: Ensure changes are written to disk

            printf("\n");
            if(!strcmp(frame_id,"TIT2"))
            {
                printf("TITLE\t:%s\n",new_value);
                for(int i=0;i<20;i++) printf("-");
                printf("TITLE CHANGED SUCCESSFULLY");
                for(int i=0;i<20;i++) printf("-");
            }
            else if(!strcmp(frame_id,"TPE1"))
            {
                printf("ARTIST\t:%s\n",new_value);
                for(int i=0;i<20;i++) printf("-");
                printf("ARTIST CHANGED SUCCESSFULLY");
                for(int i=0;i<20;i++) printf("-");
            }
            else if(!strcmp(frame_id,"TALB"))
            {
                printf("ALBUM\t:%s\n",new_value);
                for(int i=0;i<20;i++) printf("-");
                printf("ALBUM CHANGED SUCCESSFULLY");
                for(int i=0;i<20;i++) printf("-");
            }
            else if(!strcmp(frame_id,"TYER"))
            {
                printf("YEAR\t:%s\n",new_value);
                for(int i=0;i<20;i++) printf("-");
                printf("YEAR CHANGED SUCCESSFULLY");
                for(int i=0;i<20;i++) printf("-");
            }
            else if(!strcmp(frame_id,"TCON"))
            {
                printf("MUSIC\t:%s\n",new_value);
                for(int i=0;i<20;i++) printf("-");
                printf("MUSIC CHANGED SUCCESSFULLY");
                for(int i=0;i<20;i++) printf("-");
            }
            else if(!strcmp(frame_id,"COMM"))
            {
                printf("COMMENT\t:%s\n",new_value);
                for(int i=0;i<20;i++) printf("-");
                printf("COMMENT CHANGED SUCCESSFULLY");
                for(int i=0;i<20;i++) printf("-");
            }
            return;
        }
        else
        {
            fseek(fp, frame_size, SEEK_CUR); // Skip non-target frame
        }
    }

    printf("Tag not found: %s\n", target);
}


void edit(char *arg, char *arg2, FILE *fptr_sample)
{
    for(int i=0;i<40;i++)
    {
        printf("-");
    }
    printf("SELECTED EDIT DETAILS");
    for(int i=0;i<40;i++)
    {
        printf("-");
    }
    printf("\n\n");
    if (!strcmp(arg, "-t"))
    {
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("CHANGE THE TITLE");
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("\n");
        find_frame(fptr_sample, "TIT2", arg2); // Title
    }
    else if (!strcmp(arg, "-a"))
    {
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("CHANGE THE ARTIST");
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("\n");
        find_frame(fptr_sample, "TPE1", arg2); // Artist
    }
    else if (!strcmp(arg, "-A"))
    {
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("CHANGE THE ALBUM");
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("\n");
        find_frame(fptr_sample, "TALB", arg2); // Album
    }
    else if (!strcmp(arg, "-y"))
    {
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("CHANGE THE YEAR");
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("\n");
        find_frame(fptr_sample, "TYER", arg2); // Year
    }
    else if (!strcmp(arg, "-m"))
    {
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("CHANGE THE GENRE");
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("\n");
        find_frame(fptr_sample, "TCON", arg2); // Genre
    }
    else if (!strcmp(arg, "-c"))
    {
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("CHANGE THE COMMENT");
        for(int i=0;i<20;i++)
        {
            printf("-");
        }
        printf("\n");
        find_frame(fptr_sample, "COMM", arg2); // Comment
    }
    else
        printf("INVALID ARGUMENTS\n");
}
