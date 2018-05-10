#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "scramble.h"

void trimEndOfString(gameState *game) {
    int ii = 0;
    while (game->answer[ii] != '\0') {
        if (game->answer[ii] == '\n') {
            game->answer[ii] = '\0';
            break;
        }
        ii++;
    }
}

void seedRandomizer() {
    time_t startTime;
    srand((unsigned) time(&startTime));
}

void seekWord(gameState *game) {
    char buffer[256];
    game->wordListFile = fopen("/usr/share/dict/words", "r");
    fseek(game->wordListFile, 0, SEEK_END);
    int fileLength = ftell(game->wordListFile);
    rewind(game->wordListFile);
    int fileRand = rand() % fileLength -1;
    fseek(game->wordListFile, fileRand, SEEK_SET);
    char *currentChar = fgets(buffer, 2, game->wordListFile);
    while(*currentChar != '\n' || currentChar == NULL){
        fseek(game->wordListFile, -2, SEEK_CUR);
        currentChar = fgets(buffer, 2, game->wordListFile);
    }
    game->answer = strdup(fgets(buffer, 256, game->wordListFile));
    trimEndOfString(game);
    game->answerLength = (int)strlen(game->answer);
    fclose(game->wordListFile);
}

void characterSwap(char *first, char* second) {
    char temp = *first;
    *first = *second;
    *second = temp;
}

void answerShuffle(gameState *game) {
    game->scrambled = strdup(game->answer);
    for(int ii = 0; ii <= (game->answerLength - 1); ii++) {
        int randomAddress = ((rand() % game->answerLength)); //Maybe rework to proper knuth/fisher yates shuffle but scrambles enough for this purpose
        if (game->scrambled[randomAddress] == '\0') {
            ii--;
            continue;
        }
        characterSwap((game->scrambled + ii), (game->scrambled + randomAddress));
    }
}

void guessShuffle(gameState *game) {
    if(game->hasBeenUnscrambled[game->currentLetter] == 1) {
        game->currentLetter++;
        return;
    }
    else if (game->scrambled[game->currentLetter] == game->answer[game->guessInt] && game->hasBeenUnscrambled[game->guessInt] == 0) {
        characterSwap(game->scrambled + game->currentLetter, game->scrambled + game->guessInt);
        game->hasBeenUnscrambled[game->guessInt] = 1;
        if (game->scrambled[game->currentLetter] == game->answer[game->currentLetter]) {
            game->hasBeenUnscrambled[game->currentLetter] = 1;
            game->currentLetter++;
        }
    }
}//Maybe make this a hard mode since it offers less guarantee that you can unscramble the beginning and people kinda read through non obvious words left to right*/

void createGameState(gameState *game) {
    game->guessString = malloc(1024);//healthy buffer size in case someone gets silly with how long their guess is
    game->guessString[0] = '1';//safe default guess
    game->hasBeenUnscrambled = calloc(game->answerLength, sizeof(int));
    game->guessInt = 0;
    game->isEnd = 0;
    game->currentLetter = 0;
}

void getGuess(gameState *game) {
    game->guessString = fgets(game->guessString, 1024, stdin);//gigantobuffer
    if (isdigit(game->guessString[0]) == 0) {//Rejects answers that aren't numbers
        printf("Please enter a number, no letters or symbols\n");
        getGuess(game);
    }
    if (atoi(game->guessString) > game->answerLength) {
        game->guessInt = (game->answerLength - 1);
        printf("Your guess is too high, guess changed to %d.\n", game->answerLength);
    }
    else if (atoi(game->guessString) < 0) {
        game->guessInt = 0;
        printf("Your guess is too low, guess changed to 1.\n");
    }
    else if (atoi(game->guessString) > 0) {
        game->guessInt = (atoi(game->guessString) - 1);
    }
}

void isUnshuffled(gameState *game) {
    for (int letter = 0; letter < game->answerLength; letter++) {
        if ((game->scrambled[letter]) != (game->answer[letter])) {
            return;
        }
    }
    game->isEnd = 1;
}

int main() {
    seedRandomizer();
    gameState game;
    seekWord(&game);
    createGameState(&game);
    printf("%s\n", game.answer);
    answerShuffle(&game);
    while(game.isEnd != 1) {
        printf("Unscramble: %s\n", game.scrambled);
        printf("What position should this letter be in: %c\n", game.scrambled[game.currentLetter]);
        getGuess(&game);
        guessShuffle(&game);
        printf("Currently on letter number: %d\n", game.currentLetter);
        isUnshuffled(&game);
    }
    printf("Congratulations! The answer was: %s.\n", game.answer);
}