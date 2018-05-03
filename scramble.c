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
    //printf("%s", game->hangmanAnswer);
}

void characterSwap(char *first, char* second) {
    char temp = *first;
    *first = *second;
    *second = temp;
}

void answerShuffle(gameState *game) {
    char temp[1];
    game->scrambled = strdup(game->answer);
    for(int ii = 0; ii <= (game->answerLength - 1); ii++) {
        int randomAddress = ((rand() % game->answerLength)); //Hasty fix, rework to proper knuth/fisher yates shuffle
        if (game->scrambled[randomAddress] == '\0') {
            ii--;
            continue;
        }
        characterSwap((game->scrambled + ii), (game->scrambled + randomAddress));
        /*temp[0] = game->scrambled[ii];
        game->scrambled[ii] = game->scrambled[randomAddress];
        game->scrambled[randomAddress] = temp[0];
        printf("%d\n", (int)strlen(game->scrambled));*/
    }
}

void guessShuffle(gameState *game) {
    /*if (game->scrambled[game->currentLetter] == game->answer[game->guessInt]) {
        game->currentLetter++;
        char temp = game->scrambled[game->currentLetter];
        game->scrambled[game->currentLetter] = game->answer[game->guessInt];
        for(int ii = game->currentLetter + 1; ii <= game->answerLength; ii++) {
            if (game->scrambled[game->currentLetter] == game->scrambled[ii]) {
                game->scrambled[ii] = temp;
            }
        }
    }*/
    printf("%d %d", game->currentLetter, game->guessInt);
    if (game->scrambled[game->currentLetter] == game->answer[game->guessInt]) {
        characterSwap(game->scrambled + game->currentLetter, game->answer + game->guessInt);
        game->hasBeenUnscrambled[game->currentLetter] = 1;
        for(int ii = 0; ii <= game->answerLength; ii++) {
            if (game->scrambled[game->currentLetter] == game->scrambled[ii] && game->hasBeenUnscrambled[ii] == 0) {
                characterSwap(game->scrambled + ii, game->answer + game->guessInt);
                game->currentLetter++;
                return;
            }
        }
    }
}// Store game->answer[guessInt] as char and swap with first match?
// Swap from answer to scrambled then search scrambled and swap to answer?

void createGameState(gameState *game) {
    game->guessString = malloc(1024);//healthy buffer size in case someone gets silly with how long their guess is
    game->guessString[0] = '1';//safe default guess
    game->hasBeenUnscrambled = calloc(game->answerLength, sizeof(int));
    for (int ii = 0; ii < game->answerLength; ii++)
    game->guessInt = 0;
    game->isEnd = 0;
    game->currentLetter = 0;
    //game->correctGuesses = malloc(sizeof(char) * (game->answerLength + 1));
}

void trimGuess(gameState *game) {
    if ((int)strlen(game->guessString) > (game->answerLength)) {
        game->guessString[game->answerLength] = '\0';
    } 
}

void getGuess(gameState *game) {
    game->guessString = fgets(game->guessString, 1024, stdin);//gigantobuffer that gets hacked off immediately
    if (isdigit(game->guessString[0]) == 0) {//Rejects answers that aren't numbers
        printf("Please enter a number, no letters or symbols\n");
        getGuess(game);
    }
    trimGuess(game);
    game->guessInt = atoi(game->guessString);
    //game->numberOfGuesses++;
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
    trimEndOfString(&game);//Also called in seekWord, remove and test
    createGameState(&game);
    printf("%s\n", game.answer);
    answerShuffle(&game);
    //int numberofplays = 0;
    while(game.isEnd != 1) {
        printf("Unscramble: %s\n", game.scrambled);
        getGuess(&game);
        guessShuffle(&game);
        printf("%d\n", game.currentLetter);
        //printf("%d\n", numberofplays);
        //numberofplays++;
        isUnshuffled(&game);
    }
}