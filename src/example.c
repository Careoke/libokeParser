#include "../headers/parser.h"
#include <stdio.h>

#define EXAMPLE "example.liboke"

int main()
{
    Deck cards = {0};
    Settings sett = {0};
    ReadOKEData(EXAMPLE, &cards, &sett);

    printf("card amount: %d\n", cards.count);
    for (int i = 0; i < cards.count; i++)
    {
        printf("Card %d:\n", i);

        printf("Name:       %s\n", cards.card[i].name);
        printf("Length:     %.2f\n", cards.card[i].length);
        printf("Score:      %.2f\n", cards.card[i].score);
        printf("Music Path: %s\n", cards.card[i].musicPath);
        printf("LRC Path:   %s\n", cards.card[i].lrcPath);
        printf("Song Path:  %s\n", cards.card[i].songPath);

        printf("\n");
    }

    printf("Master Sound:      %.2f\n", sett.master_sound);
    printf("Background Sound:  %.2f\n", sett.background_sound);
    printf("Background Color:  #%06X\n", sett.background_color);

    if (sett.default_config == NULL)
        printf("Default Config:    NULL\n");
    else
        printf("Default Config:    %s\n", sett.default_config);

    printf("changing settings values\n");
    SetSettingData(EXAMPLE, SETTING_MASTER_SOUND, "10");
    SetSettingData(EXAMPLE, SETTING_BACKGROUND_SOUND, "10");
    SetSettingData(EXAMPLE, SETTING_BACKGROUND_COLOR, "FFFFFF");
    SetSettingData(EXAMPLE, SETTING_DEFAULT_CONFIG, "./am/a/liboke.liboke");
    ReadOKEData(EXAMPLE, &cards, &sett);

    printf("Master Sound:      %.2f\n", sett.master_sound);

    printf("Master Sound:      %.2f\n", sett.master_sound);
    printf("Background Sound:  %.2f\n", sett.background_sound);
    printf("Background Color:  #%06X\n", sett.background_color);

    if (sett.default_config == NULL)
        printf("Default Config:    NULL\n");
    else
        printf("Default Config:    %s\n", sett.default_config);

    SetCardData(EXAMPLE, CARD_TITLE, 1, "I Think I saw your face today");
    SetCardData(EXAMPLE, CARD_SCORE, 1, "75");
    SetCardData(EXAMPLE, CARD_LRC_PATH, 1, "./lyrics/I Think I saw your face today.lrc");
    SetCardData(EXAMPLE, CARD_MP3_PATH, 1, "./music/I Think I saw your face today vocals.mp3");

    SetCardData(EXAMPLE, CARD_TITLE, 2, "Shapeless you");
    SetCardData(EXAMPLE, CARD_SCORE, 2, "92");
    SetCardData(EXAMPLE, CARD_LRC_PATH, 2, "./idk/this/is/example/iguess.lrc");
    SetCardData(EXAMPLE, CARD_MP3_PATH, 2, "./music/shapeless you.mp3");

    ReadOKEData(EXAMPLE, &cards, &sett);

    printf("card amount: %d\n", cards.count);
    for (int i = 0; i < cards.count; i++)
    {
        printf("Card %d:\n", i);

        printf("Name:       %s\n", cards.card[i].name);
        printf("Length:     %.2f\n", cards.card[i].length);
        printf("Score:      %.2f\n", cards.card[i].score);
        printf("Music Path: %s\n", cards.card[i].musicPath);
        printf("LRC Path:   %s\n", cards.card[i].lrcPath);
        printf("Song Path:  %s\n", cards.card[i].songPath);

        printf("\n");
    }

    MusicTrack testCard = {
        .name = "As It was",
        .length = 165,
        .score = 75,
        .lrcPath = "D:/Musics/My/Dir/",
        .musicPath = "D:/Musics/My/Dir/",
        .songPath = "D:/Musics/My/Dir/"};

    AddNewCard(EXAMPLE, testCard);

    ReadOKEData(EXAMPLE, &cards, &sett);

    printf("card amount: %d\n", cards.count);
    for (int i = 0; i < cards.count; i++)
    {
        printf("Card %d:\n", i);

        printf("Name:       %s\n", cards.card[i].name);
        printf("Length:     %.2f\n", cards.card[i].length);
        printf("Score:      %.2f\n", cards.card[i].score);
        printf("Music Path: %s\n", cards.card[i].musicPath);
        printf("LRC Path:   %s\n", cards.card[i].lrcPath);
        printf("Song Path:  %s\n", cards.card[i].songPath);

        printf("\n");
    }

    return 0;
}