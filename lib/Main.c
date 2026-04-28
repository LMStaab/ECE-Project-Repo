#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

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
	card hand[14];

} player;

typedef struct deck_s { // use a pointer for this in main? Dynamic memory alloc?

	card cards[52]; //deck of cards
	bool isTaken; //is true when a card is in play (either dealer or player has it);
    int currIdx;
} deck;

int handSum(player x) {
	int sum = 0;


	return sum;
}


deck makeDeck(){
    deck d;
    char suits[4][9] = {"diamonds", "clubs", "hearts", "spades"};
    int deckIdx = 0;
    d.currIdx = 0;

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


void shuffle(deck d) { // shuffles the deck we are playing with
    for(int i = 0; i < 12345; i++){
        srand(time(NULL));
        int x = (rand() % (52));
        int y = (rand() % (52));
        card temp1 = d.cards[x];
        card temp2 = d.cards[y];
        d.cards[x] = temp2;
        d.cards[y] = temp1;
    }
    d.currIdx = 0;

}

card drawTop(deck d){
    // need to fix case for empty deck
    if(d.currIdx == 52){
        shuffleDeck(d);
    }
    card c = d.cards[d.currIdx];
    d.currIdx++;
    return  c;
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