#ifndef PARSER_H
#define PARSER_H

#define FILE_SIGNATURE "[gameliboke]"
#define ZERO "=0="

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Settings
{
    float master_sound;
    float background_sound;
    unsigned int background_color;
    char *default_config;
} Settings;

typedef struct
{
    char *name;
    float length;
    float score;
    char *musicPath;
    char *lrcPath;
    char *songPath;
} MusicTrack;

typedef struct Deck
{
    MusicTrack *card;
    int count;
} Deck;

typedef enum SettingType
{
    MASTER_SOUND = 0,
    BACKGROUND_SOUND,
    BACKGROUND_COLOR,
    DEFAULT_CONFIG,
} SettingType;

typedef enum Cardtype
{
    TITLE = 0,
    LENGTH,
    SCORE,
    VOCAL_PATH,
    MP3_PATH,
    LRC_PATH,
} Cardtype;

char *ReadFileStream(FILE *f, int spacing);

int ReadOKEData(char *dir, Deck *cards, Settings *sets);
int SetSettingData(char *dir, SettingType type, char *val);
int SetCardData(char *dir, Cardtype type, int cardIndex, char *val);

#endif // !PARSER_H