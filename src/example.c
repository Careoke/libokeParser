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

        printf("Index:       %d\n", cards.card[i].index);
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
    return 0;
}