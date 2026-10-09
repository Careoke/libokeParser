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
    {
        perror("fopen");
        return 1;
    }
    int index = 0;
    char *Line;
    char *key = {0};
    char *value = {0};
    char *separator = {0};
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
                    if (Line[0] == '!')
                        continue;
                    if (strcmp(Line, "\\1\\") == 0)
                        break;

                    // key - value
                    separator = strstr(Line, " - ");
                    if (separator)
                    {
                        *separator = '\0';
                        key = Line;
                        value = separator + 3; // now that i think the value doeses nothing -,-
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

int SetSettingData(char *dir, SettingType type, char *val)
{
    FILE *f = fopen(dir, "r");
    FILE *f2 = fopen("tmp.liboke", "w");

    char *key = {0};
    char *value = {0};
    char *Line = {0};
    char *door = {0};
    char *target = {0};
    char *separator = {0};
    switch (type)
    {
    case SETTING_MASTER_SOUND:
        target = malloc(strlen("master_sound") + 1);
        strcpy(target, "master_sound");
        break;
    case SETTING_BACKGROUND_SOUND:
        target = malloc(strlen("background_sound") + 1);
        strcpy(target, "background_sound");
        break;
    case SETTING_BACKGROUND_COLOR:
        target = malloc(strlen("background_color") + 1);
        strcpy(target, "background_color");
        break;
    case SETTING_DEFAULT_CONFIG:
        target = malloc(strlen("default_config") + 1);
        strcpy(target, "default_config");
        break;
    }

    for (;;)
    {
        Line = ReadFileStream(f, 0);

        if (!Line)
            break;

        if (Line[0] == '!')
        {
            fprintf(f2, "%s\n", Line);
            free(Line);
            continue;
        }

        if (strncmp(Line, "[settings]", 10) == 0)
        {
            fprintf(f2, "%s\n", Line);

            for (;;)
            {
                free(Line);
                Line = ReadFileStream(f, 0);

                if (!Line)
                    break;
                if (strcmp(Line, "\\1\\") == 0)
                {
                    fprintf(f2, "%s\n", Line);
                    break;
                }

                if (Line[0] == '!')
                {
                    fprintf(f2, "%s\n", Line);
                    continue;
                }

                char *tmpLine = malloc(strlen(Line) + 1);
                strcpy(tmpLine, Line);
                for (;;)
                {
                    if (*tmpLine != ' ' && *tmpLine != '\t')
                        break;
                    memmove(tmpLine, tmpLine + 1, strlen(tmpLine));
                }

                separator = strstr(tmpLine, " - ");
                if (separator)
                {
                    *separator = '\0';
                    key = tmpLine;
                    value = separator + 3;

                    if (strcmp(key, target) == 0)
                    {
                        switch (type)
                        {
                        case SETTING_MASTER_SOUND:
                        {
                            float sound = strtof(val, NULL);
                            door = malloc(snprintf(NULL, 0, "        master_sound - %.2f\n", sound) + 1);
                            sprintf(door, "        master_sound - %.2f\n", sound);
                            break;
                        }
                        case SETTING_BACKGROUND_SOUND:
                        {
                            float sound = strtof(val, NULL);
                            door = malloc(snprintf(NULL, 0, "        background_sound - %.2f\n", sound) + 1);
                            sprintf(door, "        background_sound - %.2f\n", sound);
                            break;
                        }
                        case SETTING_BACKGROUND_COLOR:
                        {
                            unsigned int color = strtoul(val, NULL, 16);
                            door = malloc(snprintf(NULL, 0, "        background_color - %x\n", color) + 1);
                            sprintf(door, "        background_color - %x\n", color);
                            break;
                        }
                        case SETTING_DEFAULT_CONFIG:
                        {
                            door = malloc(snprintf(NULL, 0, "        default_config - %s\n", val) + 1);
                            sprintf(door, "        default_config - %s\n", val);
                            break;
                        }
                        }
                        fprintf(f2, "%s", door);
                        free(door);
                    }
                    else
                        fprintf(f2, "%s\n", Line);
                    free(tmpLine);
                }
                else
                {
                    fprintf(f2, "%s\n", Line);
                }
            }
        }
        else
        {
            fprintf(f2, "%s\n", Line);
        }

        free(Line);
    }

    free(target);
    fclose(f);
    fclose(f2);
    if (remove(dir) != 0)
        perror("remove");

    if (rename("tmp.liboke", dir) != 0)
        perror("rename");
    return 0;
}

int SetCardData(char *dir, Cardtype type, int cardIndex, char *val)
{
    FILE *f = fopen(dir, "r");
    FILE *f2 = fopen("tmp.liboke", "w");

    char *Line = {0};
    int index = 0;
    int call = 0;
    switch (type)
    {
    case CARD_TITLE:
        call = 0;
        break;

    case CARD_LENGTH:
        call = 1;
        break;

    case CARD_SCORE:
        call = 2;
        break;

    case CARD_VOCAL_PATH:
        call = 3;
        break;

    case CARD_LRC_PATH:
        call = 4;
        break;

    case CARD_MP3_PATH:
        call = 5;
        break;
    }

    for (;;)
    {
        Line = ReadFileStream(f, 0);

        if (!Line)
            break;

        if (Line[0] == '!')
        {
            fprintf(f2, "%s\n", Line);
            free(Line);
            continue;
        }

        if (strncmp(Line, "[cards]", 7) == 0)
        {
            fprintf(f2, "%s\n", Line);
            free(Line);

            for (;;)
            {
                Line = ReadFileStream(f, 0);

                if (!Line)
                    break;
                if (strcmp(Line, "\\1\\") == 0)
                {
                    fprintf(f2, "%s\n", Line);
                    free(Line);
                    break;
                }
                if (Line[0] == '!')
                {
                    fprintf(f2, "%s\n", Line);
                    free(Line);
                    continue;
                }

                char *tmpLine = malloc(strlen(Line) + 1);
                strcpy(tmpLine, Line);
                for (;;)
                {
                    if (*tmpLine != ' ' && *tmpLine != '\t')
                        break;

                    memmove(tmpLine, tmpLine + 1, strlen(tmpLine));
                }

                if (strncmp(tmpLine, "(card)", 6) == 0)
                {
                    index++;
                    if (index == cardIndex)
                    {
                        fprintf(f2, "%s\n", Line);
                        free(Line);
                        for (int i = 0; i < call; i++)
                        {
                            Line = ReadFileStream(f, 0);

                            if (!Line)
                                break;

                            fprintf(f2, "%s\n", Line);
                            free(Line);
                        }
                        Line = ReadFileStream(f, 0);
                        free(Line);
                        fprintf(f2, "            %s\n", val);
                    }
                    else
                    {
                        fprintf(f2, "%s\n", Line);
                        free(Line);
                    }
                }
                else
                {
                    fprintf(f2, "%s\n", Line);
                    free(Line);
                }
                free(tmpLine);
            }
            continue;
        }

        fprintf(f2, "%s\n", Line);
        free(Line);
    }

    fclose(f);
    fclose(f2);

    if (remove(dir) != 0)
        perror("remove");

    if (rename("tmp.liboke", dir) != 0)
        perror("rename");

    return 0;
}

int AddNewCard(char *dir, MusicTrack NewCard)
{
    FILE *f = fopen(dir, "r");
    FILE *f2 = fopen("tmp.liboke", "w");
    char *Line = {0};
    int index = 0;

    for (;;)
    {
        Line = ReadFileStream(f, 0);

        if (!Line)
            break;

        if (Line[0] == '!')
        {
            fprintf(f2, "%s\n", Line);
            free(Line);
            continue;
        }

        if (strncmp(Line, "[cards]", 7) == 0)
        {
            fprintf(f2, "%s\n", Line);
            free(Line);

            for (;;)
            {
                Line = ReadFileStream(f, 0);

                if (!Line)
                    break;
                if (Line[0] == '!')
                {
                    fprintf(f2, "%s\n", Line);
                    free(Line);
                    continue;
                }

                if (strcmp(Line, "\\1\\") == 0)
                {
                    fprintf(f2, "        (card) /0I\n");
                    fprintf(f2, "            %s\n", NewCard.name);
                    fprintf(f2, "            %.2f\n", NewCard.length);
                    if (strcmp(Line, ZERO) == 0)
                        fprintf(f2, "            %s\n", ZERO);
                    else
                        fprintf(f2, "            %.2f\n", NewCard.score);
                    fprintf(f2, "            %s\n", NewCard.musicPath);
                    fprintf(f2, "            %s\n", NewCard.lrcPath);
                    if (strcmp(Line, ZERO) == 0)
                        fprintf(f2, "            %s\n", ZERO);
                    else
                        fprintf(f2, "            %s\n", NewCard.songPath);
                    fprintf(f2, "            /0K\n");

                    fprintf(f2, "%s\n", Line);
                    free(Line);
                    break;
                }
                else
                {
                    fprintf(f2, "%s\n", Line);
                    free(Line);
                }
            }
            continue;
        }

        fprintf(f2, "%s\n", Line);
        free(Line);
    }

    fclose(f);
    fclose(f2);

    if (remove(dir) != 0)
        perror("remove");

    if (rename("tmp.liboke", dir) != 0)
        perror("rename");

    return 0;
}