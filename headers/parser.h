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
    SETTING_MASTER_SOUND = 0,
    SETTING_BACKGROUND_SOUND,
    SETTING_BACKGROUND_COLOR,
    SETTING_DEFAULT_CONFIG,
} SettingType;

typedef enum Cardtype
{
    CARD_TITLE = 0,
    CARD_LENGTH,
    CARD_SCORE,
    CARD_VOCAL_PATH,
    CARD_MP3_PATH,
    CARD_LRC_PATH,
} Cardtype;

char *ReadFileStream(FILE *f, int spacing);

void freeCard(MusicTrack card);
void freeDeck(Deck deck);
void freeSettings(Settings sets);
void freeLiboke(Deck deck, Settings sets);
int ReadOKEData(char *dir, Deck *cards, Settings *sets);
int SetSettingData(char *dir, SettingType type, char *val);
int SetCardData(char *dir, Cardtype type, int cardIndex, char *val);
int AddNewCard(char *dir, MusicTrack NewCard);
int RemoveCard(char *dir, int cardIndex);

#endif // !PARSER_H