/* Example of the use of C enumerations (enums)
 * Creates a deck of cards, shuffles it, then prints out the
 * shuffled deck.
 *
 * Richard Lobb, August 2014.
 */

#include <stdio.h>
#include <stdlib.h>

/* Declare the prototype for drand48, which is a non-ANSI library
 * function.
 */
double drand48(void);

#define NUM_SUITS 4
#define NUM_IN_SUIT 13
#define NUM_IN_DECK 52
#define NUM_SWAPS 10000

typedef enum {SPADES, HEARTS, DIAMONDS, CLUBS} Suit;
typedef enum {ACE=1, JACK=11, QUEEN, KING} PipCount;

char* suits[] = {"spades", "hearts", "diamonds", "clubs"};
char* pips[]  = {"UNDEFINED", "ace", "2", "3", "4", "5", "6", "7", "8",
                 "9", "10", "jack", "queen", "king"};

typedef struct {
    PipCount pip;
    Suit suit;
} Card;


/* Shuffle a deck of cards by making a large number of swaps
 * of two randomly selected cards in the deck */
void shuffle(Card* deck)
{
    int swap = 0;
    int i = 0, j = 0;
    Card temp = {ACE, SPADES};
    for (swap = 0; swap < NUM_SWAPS; swap += 1) {
        i = drand48() * NUM_IN_DECK;   // Random in [0,51]
        j = drand48() * NUM_IN_DECK;
        temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// main program: create a deck, shuffle it and display the result
int main() {
    Card deck[NUM_IN_DECK];
    int cardNum = 0;
    
    for (Suit suit = SPADES; suit <= CLUBS; suit += 1) {
        for (PipCount pip = ACE; pip <= KING; pip += 1) {
            deck[cardNum].suit = suit;
            deck[cardNum].pip = pip;
            cardNum += 1;
        }
    }

    //shuffle(deck);
    printf("Shuffled deck:\n");
    for (int iCard = 0; iCard < NUM_IN_DECK; iCard += 1) {
        printf("%s of %s\n", pips[deck[iCard].pip],
                             suits[deck[iCard].suit]);
    }
    return EXIT_SUCCESS;
}
