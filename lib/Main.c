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





void shuffle(deck x[]) { // shuffles the deck we are playing with



}

void initPlayer(player *x, int num) {
	printf("Enter player %d Name: ", num);
	scanf("%s", x->name);
	x->money = 100;
	x->bet = 0;
	x->playerNum = num;
}

void printHand(player x) {

}

void resetHand(player x) { // resets player hand


}


int main(void) {

	int numPlayers;
	player crowd[4]; //array for players, max 4
	int i;

	printf("Enter the number of players: ");
	scanf("%d", &numPlayers);

	for (i =0; i < numPlayers; i++){
		initPlayer(&crowd[i], i+1);
	}



	return 0;


}