/** @file scramble.h
 * @brief Currently contains all of the function protoypes for a simple word scrambling game and its gameState struct.
 * 
 * I regret that I need two swap functions but a random scramble and swapping specific letters require two different
 * approaches. Might split the gameState struct up a bit as it is basically the entire program by itself.
 * 
 * @author David McBurney
 * @bug Incomplete, currently nonfunctional
 */

/** @struct gameState.h
 * @brief Contains all of the variables concerning the game as of current.
 * This struct may get split into two, possibly one for the game and correct answer
 * and one for the scrambled word? As of now though it contains the entire program
 * since it absorbed the wordListFile FILE pointer.
 */
typedef struct {
char *guessString; /**< Holds malloced char pointer to the current guess. */
char *correctGuesses; /**< Holds malloced char pointer to what has been guessed correctly so far. */
char *answer; /**< Holds malloced char pointer to the answer itself. */
char *scrambled; /**< A muxed up version of answer, malloced via strdup */
int *hasBeenUnscrambled;
int guessInt; /**< An int conversion of the string the player enters as their guess. */
int currentLetter; /**< The letter the player is currently attempting to guess placement of. */
int answerLength; /**< Int to track the length of the answer which is necessary for any loop that compares to the answer. */
int isGuessCorrect; /**< Int that acts as a bool, may use to set collision array, my toss out. */
int isEnd; /**< Int that acts as bool, allows me to push checking game end conditions outside the core loop condition */
FILE *wordListFile; /**< FILE pointer points to the file that will have words randomly pulled from.*/
}gameState;

/** @brief Cuts off newline characters in strings fed into it, replaces first newline with '\0'
 * Like it says in the brief, takes a string and replaces the first newline character with
 * a terminating character.
 * @param gameState *game, alters game.answer
 * @return Void
 */
void trimEndOfString(gameState *game);

/** @brief Uses time to seed the RNG used to select a random word and randomly scramble it
 * Learned the hard way that if the seed isn't randomized you'll always get the same word
 * scrambled in the exact same way. Not much of a game that way really.
 * Since this calls srand() it should never be called more than once.
 * @param Void, calls srand() using time_t affecting all future calls to rand()
 * @return Void
 */
void seedRandomizer();

/** @brief Seeks through file to find a random word and copies it to game.answer, copies trimEndOfString on game.answer and assigns game.answerLength
 * Opens wordListFile file pointer, finds the files length by seeking the end and then seeks a random point in that length.
 * From that random point it looks for a newline by seeking backwards then fgets a string up to a max length of 256 characters once that newline is found.
 * That fgets is strduped to game.answer which then has trimEndOfString called on it. Once that's done game.answerLength is assigned and the
 * file pointer is closed, ending the funciton.
 * @param gameState *game, assigns game.answer and game.answerLength, opens and closes wordListFile FILE pointer.
 * @reutrn Void
 */
void seekWord(gameState *game);

void characterSwap(char *first, char* second);

void answerShuffle(gameState *game);

void guessShuffle(gameState *game);

void createGameState(gameState *game);

void trimGuess(gameState *game);

void getGuess(gameState *game);

void isUnshuffled(gameState *game);
