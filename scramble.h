/** @file scramble.h
 * @brief Currently contains all of the function protoypes for a simple word scrambling game and its gameState struct.
 * @author David McBurney
 * @bug None documented
 */

/** @struct gameState.h
 * @brief Contains all of the variables concerning the game as of current.
 */
typedef struct {
char *guessString; /**< Holds malloced char pointer to the current guess. */
char *answer; /**< Holds malloced char pointer to the answer itself. */
char *scrambled; /**< A muxed up version of answer, malloced via strdup */
int *hasBeenUnscrambled; /**< Array of ints acting as bools same length as answer, used to mark an array address as having already been correctly unscrambled */
int guessInt; /**< An int conversion of the string the player enters as their guess. */
int currentLetter; /**< The letter the player is currently attempting to guess placement of. */
int answerLength; /**< Int to track the length of the answer which is necessary for any loop that compares to the answer. */
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

/** @brief Seeks through file to find a random word and copies it to game.answer, calls trimEndOfString on game.answer and assigns game.answerLength
 * Opens wordListFile file pointer, finds the files length by seeking the end and then seeks a random point in that length.
 * From that random point it looks for a newline by seeking backwards then fgets a string up to a max length of 256 characters once that newline is found.
 * That fgets is strduped to game.answer which then has trimEndOfString called on it. Once that's done game.answerLength is assigned and the
 * file pointer is closed, and the function ends.
 * @param gameState *game, assigns game.answer and game.answerLength, opens and closes wordListFile FILE pointer.
 * @return Void
 */
void seekWord(gameState *game);

/** @brief Takes two char pointers to individual letters and swaps them.
 * As traditional as character swap functions get. Copies the letter at the first address to char temp,
 * it then copies the letter at the second address to the first address and copies the letter in temp tothe second address.
 * @param char *first, a character whose address you want swapped. In this program it always takes in one provided by game->scrambled.
 * @param char *second, a character whose address you want swapped. In this program it always takes in one provided by game->scrambled.
 * @return Void
 */
void characterSwap(char *first, char* second);

/** @brief Randomly mixes up letters in a word until it has moved all of them around at least once.
 * Starts by strduping game->answer to game->scrambled, it then loops through scrambled and selects
 * "random" addresses to call characterSwap on in combination with where it is in the for loop.
 * If the "random" address it generates is the address of the terminating character it decrements and continues the loop
 * Repeat until it has "randomly" swapped eerything other than the terminating character.
 * @param gameState *game, it duplicates what's in game.answer to assign game.scrambled then mixes up what's in game.scrambled.
 * @return Void
 */
void answerShuffle(gameState *game);

/** @brief Swaps letters if the player guesses the correct position of game.shuffled[currentLetter]
 * Immediately checks if game->hasBeenUnshuffled[currentLetter] is true. If it is it increments currentLetter and returns.
 * If this isn't the case but game->scrambled[currentLetter] is the same letter as game->answer[guessInt] character swap is
 * called on game->scrambled + currentLetter and game->scrambled + guessInt. game->hasBeenUnscrambled[guessInt] is set to 1
 * and then a check is done on whether the new letter at scrambled[currentLetter] is correct. If this is the case the 
 * game sets hasBeenUnscrambled[currentLetter] to 1 and increments currentLetter
 * @param gameState *game, swaps characters around in game.scrambled and checks against game.answer
 * @return Void
 */
void guessShuffle(gameState *game);

/** @brief Assigns default values in the gameState struct and memory to the various arrays in it.
 * First memory is assigned for guessString and a default value of '1' is assigned to guessString.
 * hasBeenUnscrambled is then calloced to make sure it has 0 for its values to start with.
 * A default value of 0 is then given to guessInt, isEnd and currentLetter.
 * @param gameState *game, assigns values, memory or both to guessString, hasBeenUnscrambled, guessInt, isEnd and currentLetter
 * @return Void
 */
void createGameState(gameState *game);

/** @brief takes a player guess checks if that guess isdigit and then convers it to an int if that's the case.
 * First fgets is used to get a guess from the player which is assigned to game.guessString. isDigit is called on
 * the first character in guessString to make sure there at least is a number in it. atoi is then called on guessString
 * if the number is too small or too large it is changed to 1 or game.answerLength respectively and assigned to guessInt. 
 * Otherwise the return value of atoi is assigned to guessInt.
 * @param gameState *game, assigns guessString and guessInt.
 * @return Void
 */
void getGuess(gameState *game);

/** @brief Loops through game.answer and game.answer to check if they are equal
 * Pretty much what the brief says. Loops through comparing letters between game.scrambled and game.answer.
 * If any letter fails to match the function returns, if they all match, isEnd is set to 1, in turn terminating he main loop.
 * @param gameState *game, loops though answer andscrambled, sets isEnd.
 * @return Void
 */
void isUnshuffled(gameState *game);
