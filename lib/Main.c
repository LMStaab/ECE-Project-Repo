#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <string.h>




typedef struct card_s {

	char suit[9]; // name of the suit (clubs, diamonds, hearts, spades)
	int value; 
	// any other attributes of your choice

} card;

typedef struct player_s {


	char name[100];
	int money; // how much money a player has
	int bet;
	int playerNum;
	card hand[10];

} player;

typedef struct deck_s { // use a pointer for this in main? Dynamic memory alloc?

	card cards[52]; //deck of cards
	bool isTaken; //is true when a card is in play (either dealer or player has it);
} deck;

int handSum(player x) {
	int sum = 0;


	return sum;
}


deck makeDeck(){
    deck d;
    char suits[4][9] = {"diamonds", "clubs", "hearts", "spades"};
    int deckIdx = 0;

    for(int s = 0; s < 4; s++){
        for(int i = 1; i <= 13; i++){ //1 = ace, 11, 12, 13 == jack, queen, king
            card c;
            strcpy(c.suit, suits[s]);
            c.value = i;
            d.cards[deckIdx] = c;
        }
    }
    return d;
}


void shuffle(deck x[]) { // shuffles the deck we are playing with

}

void initPlayer(player x) {

}

void printHand(player x) {

}

void resetHand(player x) { // resets player hand


}

int main(void) {

	int numPlayers;




	return 0;


}