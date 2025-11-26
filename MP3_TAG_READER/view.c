#include <stdio.h>
#include <string.h>
#include <ctype.h>

void view(FILE *fptr_sample)
{
    for (int i = 0; i < 40; i++)
        printf("-");
    printf("SELECTED VIEW DETAILS");
    for (int i = 0; i < 40; i++)
        printf("-");
    printf("\n\n");

    for (int i = 0; i < 70; i++)
        printf("-");
    printf("\n\t\tMP3 TAG READER AND EDITOR FOR ID3V2\n");
    for (int i = 0; i < 70; i++)
        printf("-");
    printf("\n");

    // SKIP ID3 HEADER
    fseek(fptr_sample, 10, SEEK_SET);

    char frame_id[5];
    unsigned char size_bytes[4];
    unsigned char flags[2];
    int frame_size;
    char buffer[256];

    const char *fields[] = {"TITLE", "ARTIST", "ALBUM", "YEAR", "MUSIC", "COMMENT"};

    for (int field = 0; field < 6; field++)
    {
        if (fread(frame_id, 1, 4, fptr_sample) != 4)
            break;
        frame_id[4] = '\0';

        if (fread(size_bytes, 1, 4, fptr_sample) != 4)
            break;

        if (fread(flags, 1, 2, fptr_sample) != 2)
            break;

        // ID3v2.3 uses 4-byte size as normal int
        frame_size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];

        if (frame_size <= 0 || frame_size > sizeof(buffer) - 1)
            frame_size = sizeof(buffer) - 1;

        if (fread(buffer, 1, frame_size, fptr_sample) != frame_size)
            break;

        // Skip the encoding byte (first byte)
        int text_len = frame_size - 1;
        if (text_len > 0)
        {
            // Copy content safely and null terminate
            memmove(buffer, buffer + 1, text_len);
            buffer[text_len] = '\0';

            // Remove trailing non-printable characters
            for (int i = text_len - 1; i >= 0; i--)
            {
                if (!isprint(buffer[i]))
                    buffer[i] = '\0';
                else
                    break;
            }
        }
        else
        {
            buffer[0] = '\0';
        }

        printf("%s\t\t:%s\n", fields[field], buffer);
    }

    for (int i = 0; i < 70; i++)
        printf("-");
    printf("\n\t\tDETAILS DISPLAYED SUCCESSFULLY\n");
    for (int i = 0; i < 70; i++)
        printf("-");
    printf("\n");
    for (int i = 0; i < strlen("SELECTED VIEW DETAILS") + 80; i++)
        printf("-");
    printf("\n");
}
