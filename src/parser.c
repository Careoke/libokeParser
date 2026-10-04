#include "../headers/parser.h"

char *ReadFileStream(FILE *f, int spacing)
{
    char *buffer = NULL;
    int c;
    int size = 0;
    for (;;)
    {
        c = fgetc(f);
        if (c == '\n')
            break;
        if (c == EOF)
        {
            if (size == 0)
                return NULL;
            break;
        }

        buffer = realloc(buffer, size + 2);
        buffer[size] = c;
        size++;
        buffer[size] = '\0';
    }

    if (buffer == NULL)
    {
        buffer = malloc(1);
        buffer[0] = '\0';
    }
    if (spacing)
        for (;;)
        {
            if (*buffer != ' ' && *buffer != '\t')
                break;
            memmove(buffer, buffer + 1, strlen(buffer));
        }
    return buffer;
}

int ReadOKEData(char *dir, Deck *cards, Settings *sets)
{
    FILE *f = fopen(dir, "r");
    if (!f)
        return 1;
    int index = 0;
    char *Line;
    char key[64];
    char value[64];
    char *Signature = ReadFileStream(f, 1);
    if (strcmp(Signature, FILE_SIGNATURE) != 0)
    {
        printf("Invalid file type\n");
        free(Signature);
        return 1;
    }
    else
    {
        free(Signature);
        for (;;)
        {
            Line = ReadFileStream(f, 1);
            if (!Line)
                break;

            if (Line[0] == '!')
            {
                free(Line);
                continue;
            }

            if (strncmp(Line, "[settings]", 10) == 0)
            {
                for (;;)
                {
                    free(Line);
                    Line = ReadFileStream(f, 1);
                    if (!Line)
                        break;
                    if (strcmp(Line, "\\1\\") == 0)
                        break;
                    // key - value
                    if (sscanf(Line, "%63s - %63s", key, value) == 2)
                    {
                        if (strcmp(key, "master_sound") == 0)
                            sets->master_sound = strtof(value, NULL);
                        else if (strcmp(key, "background_sound") == 0)
                            sets->background_sound = strtof(value, NULL);
                        else if (strcmp(key, "background_color") == 0)
                            sets->background_color = strtoul(value, NULL, 16);
                        else if (strcmp(key, "default_config") == 0)
                            if (strcmp(value, "=0=") == 0)
                                sets->default_config = NULL;
                            else
                            {
                                sets->default_config = malloc(strlen(value) + 1);
                                strcpy(sets->default_config, value);
                            }
                    }
                }
                free(Line);
                continue;
            }

            if (strcmp(Line, "(card) /0I") == 0)
            {
                cards->card = realloc(cards->card, (index + 1) * sizeof(MusicTrack));
                free(Line);

                Line = ReadFileStream(f, 1);
                int in;
                if (sscanf(Line, "<%d>", &in) == 1)
                    cards->card[index].index = in;
                free(Line);

                Line = ReadFileStream(f, 1);
                cards->card[index].name = malloc(strlen(Line) + 1);
                strcpy(cards->card[index].name, Line);
                free(Line);

                Line = ReadFileStream(f, 1);
                cards->card[index].length = strtof(Line, NULL);
                free(Line);

                Line = ReadFileStream(f, 1);
                if (strcmp(Line, ZERO) == 0)
                    cards->card[index].score = 0;
                else
                    cards->card[index].score = strtof(Line, NULL);
                free(Line);

                Line = ReadFileStream(f, 1);
                cards->card[index].musicPath = malloc(strlen(Line) + 1);
                strcpy(cards->card[index].musicPath, Line);
                free(Line);

                Line = ReadFileStream(f, 1);
                cards->card[index].lrcPath = malloc(strlen(Line) + 1);
                strcpy(cards->card[index].lrcPath, Line);
                free(Line);

                Line = ReadFileStream(f, 1);
                if (strcmp(Line, ZERO) == 0)
                    cards->card[index].songPath = NULL;
                else
                {
                    cards->card[index].songPath = malloc(strlen(Line) + 1);
                    strcpy(cards->card[index].songPath, Line);
                }
                free(Line);

                Line = ReadFileStream(f, 1); // /0K
                free(Line);

                index++;
                cards->count = index;
                continue;
            }
            free(Line);
        }
    }
    fclose(f);
    return 0;
}
