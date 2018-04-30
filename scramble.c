#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct {
FILE *wordListFile; /**< FILE pointer points to the file that will have words randomly pulled from.*/
}wordList;

typedef struct {
char *guessString; /**< Holds malloced char pointer to the current guess. */
char *correctGuesses; /**< Holds malloced char pointer to what has been guessed correctly so far. */
char *answer; /**< Holds malloced char pointer to the answer itself. */
char *scrambled;
int guessInt;
int numberOfGuesses; /**< Int to track the total number of unique guesses which is used to keep hangmanAlreadyGuessed up to date. */
int currentLetter; /**< The letter the player is currently attempting to guess placement of. */
int answerLength; /**< Int to track the length of the answer which is necessary for any loop that compares to the answer. */
int isGuessCorrect; /**< Int that acts as a bool, set in isGuessCorrect to decide whether to increment hangmanStrikes. */
int isEnd; /**< Int that acts as bool, allows me to push checking game end conditions outside the core loop condition */
}gameState;

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

void seekWord(wordList *words, gameState *game) {
    char buffer[256];
    words->wordListFile = fopen("/usr/share/dict/words", "r");
    fseek(words->wordListFile, 0, SEEK_END);
    int fileLength = ftell(words->wordListFile);
    rewind(words->wordListFile);
    int fileRand = rand() % fileLength -1;
    fseek(words->wordListFile, fileRand, SEEK_SET);
    char *currentChar = fgets(buffer, 2, words->wordListFile);
    while(*currentChar != '\n' || currentChar == NULL){
        fseek(words->wordListFile, -2, SEEK_CUR);
        currentChar = fgets(buffer, 2, words->wordListFile);
    }
    game->answer = strdup(fgets(buffer, 256, words->wordListFile));
    trimEndOfString(game);
    game->answerLength = (int)strlen(game->answer);
    fclose(words->wordListFile);
    //printf("%s", game->hangmanAnswer);
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
        temp[0] = game->scrambled[ii];
        game->scrambled[ii] = game->scrambled[randomAddress];
        game->scrambled[randomAddress] = temp[0];
        printf("%d\n", (int)strlen(game->scrambled));
    }
}

void guessShuffle(gameState *game) {
    if (game->scrambled[game->currentLetter] == game->answer[game->guessInt]) {
        game->currentLetter++;
        char temp = game->scrambled[game->currentLetter];
        game->scrambled[game->currentLetter] = game->answer[game->guessInt];
        for(int ii = game->currentLetter + 1; ii <= game->answerLength; ii++) {
            if (game->scrambled[game->currentLetter] == game->scrambled[ii]) {
                game->scrambled[ii] = temp;
            }
        }
    }
}

void createGameState(gameState *game) {
    game->guessString = malloc(1024);//healthy buffer size in case someone gets silly with how long their guess is
    game->guessString[0] = '1';//safe default guess
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
    game->numberOfGuesses++;
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
    wordList words;
    gameState game;
    createGameState(&game);
    seekWord(&words, &game);
    trimEndOfString(&game);
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