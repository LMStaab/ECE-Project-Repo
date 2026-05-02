/*
 * Code Authors: Lucas Staab, Douglas Sheppard
 * Course: ECE 101
 * Assignment: Final project, game of 31
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_PLAYERS 4
#define DECK_SIZE 52
#define MAX_NAME 50

// Structs done by Douglas

typedef struct cards{
    char suit[9];
    int value;
    // added attributes
    char name[MAX_NAME];
} card;

typedef struct deck_struct{
    card cards[DECK_SIZE];
    int top;
} Deck;

typedef struct player_struct{
    char name[MAX_NAME];
    int balance;
    int currentBet;
    card hand[10];
    int handCount;
    int total;
    char status[20]; // "stand", "bust", "thirty-one", "fourteen"
} Player;

typedef struct dealer_struct{
    card hand[10];
    int handCount;
    int total;
} Dealer;

// Utility Functions

// Done by Douglas
void shuffle(Deck* deck){ // shuffles the deck via pointer
    for (int i = 0; i < DECK_SIZE; i++) {
        int r = i + rand() % (DECK_SIZE - i);
        card temp = deck->cards[i];
        deck->cards[i] = deck->cards[r];
        deck->cards[r] = temp;
    }
}

// Done by Lucas
void initDeck(Deck *deck) {
    char *suits[] = {"Clubs", "Spades", "Hearts", "Diamonds"};
    char *names[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int vals[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            strcpy(deck->cards[k].suit,  suits[i]);
            strcpy(deck->cards[k].name, names[j]);
            deck->cards[k].value = vals[j];
            k++;
        }
    }
    deck->top = 0;

    shuffle (deck);
    
}



// Returns the top card // Done by Lucas
card drawTop(Deck *deck) {
    if (deck->top >= DECK_SIZE) {
        initDeck(deck); // Reshuffles and resets deck->top to 0
    }
    return deck->cards[deck->top++];
}

// Makes sure the total is fully up to date  // Done by Lucas
void updateHandTotal(Player *p) {
    p->total = 0;
    for (int i = 0; i < p->handCount; i++) {
        p->total += p->hand[i].value;
    }
}

// Game Logic

// Done by Douglas
void playerTurn(Player *p, Deck *deck) {
    char choice[10];
    int acesDrawn = 0; // Track total aces drawn this round
    
    // Check initial starting card for an Ace
    if (p->handCount > 0 && p->hand[0].value == 11) {
        acesDrawn++;
        int aceVal;
        printf("\n%s, your starting card is an Ace! Is it worth 1 or 11? ", p->name); 
        scanf("%d", &aceVal); 
        p->hand[0].value = aceVal;
    }

    printf("\n%s's Turn\n", p->name);
    
    while (1) {
        updateHandTotal(p); // Make total up to date
        printf("%s has: ", p->name); 
        for(int i=0; i < p->handCount; i++) {
            printf("[%s of %s] ",p->hand[i].name, p->hand[i].suit); // Print cards
        }
        printf("\nTotal: %d\n", p->total);

        // These conditions make drawing unnessesary and break loop
        if(p->total == 14){ 
            printf("14!\n");
            strcpy(p->status, "fourteen");
            break;
        } else if (p->total > 31) {
            printf("You busted!\n");
            strcpy(p->status, "bust");
            break;
        } else if (p->total == 31) {
            printf("31!\n");
            strcpy(p->status, "thirty-one");
            break;
        }

        
        printf("Would you like to 'hit' or 'stand'? ");
        scanf("%9s", choice);
        while (strcmp(choice, "hit") != 0 && strcmp(choice, "stand") != 0){
            printf("Invalid Choice, try again.\n");
            printf("Would you like to 'hit' or 'stand'? ");
            scanf("%9s", choice);
        }

        if (strcmp(choice, "hit") == 0) {
            p->hand[p->handCount++] = drawTop(deck);
            
            // If value is an ace
            if (strcmp(p->hand[p->handCount-1].name, "Ace") == 0) {
                acesDrawn++;
                // If a player draws two aces, one counts as 1 and one counts as 11.
                // A third ace can be assigned a 1 or an 11, with the next ace being fixed to the
                // opposite value.
                if (acesDrawn == 1 || acesDrawn == 3) {
                    int aceVal;
                    printf("You drew an Ace! Is it worth 1 or 11? "); 
                    scanf("%d", &aceVal); 
                    p->hand[p->handCount-1].value = aceVal;
                } else {
                    // 2nd or 4th Ace must be the opposite of the previous Ace
                    int lastAceVal = 11; // Default
                    for (int i = p->handCount - 2; i >= 0; i--) {
                        if (strcmp(p->hand[i].name, "Ace") == 0) {
                            lastAceVal = p->hand[i].value;
                            break;
                        }
                    }
                    int newAceVal = 11;
                    if (lastAceVal == 11){ // opposite of the prervious
                        newAceVal = 1;
                    }
                    printf("You drew an Ace! By the rules, it is automatically worth the opposite: %d.\n", newAceVal);
                    p->hand[p->handCount-1].value = newAceVal;
                }
            }
        } else {
            strcpy(p->status, "stand");
            break;
        }
    }
}

// Done by Lucas
void getBet(Player *p){
    printf("Enter bet for Player %s (current balance: $%d): ", p->name, p->balance);
    scanf(" %d", &p->currentBet);
    if (p->currentBet > p->balance || p-> currentBet <= 0){
        // Sets the bet to max value if too much is entered or too little
        p->currentBet = p->balance;
        printf("Balance is only $%d, bet set to $%d\n", p->balance, p->balance);
    }
}

// Done by Douglas1
int dealersTurn(Dealer *dealer, Deck *deck){
    dealer->total = dealer->hand[0].value;
    int dealerBusted = 0;
    int dealerAces = 0;
    if(dealer->hand[0].value == 11){
        dealerAces = 1;
    }

    // Dealer strategy: continue drawing to get as close to 31 as possible,
    // Stops at 14, we picked 25 as our stopping point
    while (dealer->total < 25) { 
        dealer->hand[dealer->handCount++] = drawTop(deck);
        int drawnValue = dealer->hand[dealer->handCount - 1].value;
        dealer->total += drawnValue; // add new card to the total;

        if(drawnValue == 11){
            dealerAces++;
        }

        // If dealer is over 31 because of aces, makes them into one instead of 11 
        while (dealer->total > 31 && dealerAces > 0){
            dealer->total -= 10;
            dealerAces--;
        }

        // If dealer hits 14, he reveals hand and stops drawing
        if (dealer->total == 14) {
            printf("Dealer hit 14 and must stop drawing.\n");
            break;
        }

        // Bets are already taken from player accounts so no need to decrement
        if(dealer->total == 31){
            printf("Dealer hit 31, players lose\n");
            break;
        }

        // Bigger than 31 is a big fat bust so we return a 1 so that we skip the player turns since they all win
        if (dealer->total > 31) {
            dealerBusted = 1;
            break;
        }
        
    }
    return dealerBusted;
}

// Contributed to by both authors equally
int main() {
    // Seeds the random so the deck is not the same
    srand(time(NULL));

    int playerCount;
    printf("How many players? (Max 4): ");
    scanf("%d", &playerCount);
    if(playerCount > 4){ // If more than 4, sets the player number to 4 
        printf("MAX PEOPLE IS 4, ONLY MAKING 4 PLAYERS");
        playerCount = 4;
    }

    // Creates all the player structs for storing player info
    Player players[MAX_PLAYERS];
    for (int i = 0; i < playerCount; i++) {
        printf("Enter name for Player %d: ", i + 1);
        scanf("%49s", players[i].name);
        players[i].balance = 100; // Starting balance is 100
        getBet(&players[i]);
        players[i].handCount = 0; // 0 cards
    }

    int running = 1;
    while (running) {
        // Initialize deck
        Deck deck; 
        initDeck(&deck);
        Dealer dealer;
        // Dealer deals one card face down to himself
        dealer.handCount = 0;
        dealer.total = 0;
        dealer.hand[dealer.handCount++] = drawTop(&deck);
        
        // One card facing up to each player
        for (int i = 0; i < playerCount; i++) {
            if (players[i].balance <= 0) continue; // out of money so that player is skipped
            players[i].handCount = 0; 
            players[i].hand[players[i].handCount++] = drawTop(&deck); // increases handcount by 1 and draws top card
            strcpy(players[i].status, ""); // Reset status for new round
        }

        // dealers turn before the players
        // sets dealer busted to 1 if he did
        int dealerBusted = dealersTurn(&dealer, &deck);

        // Dealer strategy: continue drawing to get as close to 31 as possible,
        // Stops at 14, we picked 25 as our stopping point
        if (dealerBusted) {
            printf("Dealer busted with %d! All active players win double.\n", dealer.total);
            for (int i = 0; i < playerCount; i++) {
                if (players[i].balance > 0) {
                    players[i].balance += players[i].currentBet; // Pay out the bet
                } else {
                    players[i].balance += 1; // broke people get one dollar per dealer loss
                }
            }
        }

        if (dealerBusted || dealer.total == 31) goto end_round; // Skip player turns if dealer bustsor wins

        printf("\nDealer reveals: ");
        if (dealer.total == 14) {
            // If dealer hits 14, reveal EVERYTHING
            for(int i = 0; i < dealer.handCount; i++) {
                printf("[%s of %s] ", dealer.hand[i].name, dealer.hand[i].suit); 
            } 
            printf("\n");
        } else {
            // Otherwise, hide the first card
            for(int i = 1; i < dealer.handCount; i++) {
                printf("[%s of %s] ", dealer.hand[i].name, dealer.hand[i].suit); 
            } 
            printf("[One Card Face Down]\n");
        }

        // Player's Turn
        for (int i = 0; i < playerCount; i++){
            if (players[i].balance > 0) {
                // Do the player turn, will go until win or bust
                playerTurn(&players[i], &deck);
                // If player hits 14 or 31 and dealer doesn't have 14, player wins
                if ((players[i].total == 14 || players[i].total == 31) && dealer.total != 14) { // Win case by 31
                    printf("%s wins immediately with %d!\n", players[i].name, players[i].total);
                    players[i].balance += (players[i].currentBet);
                    strcpy(players[i].status, "won_early");
                }
            }
        }

        // Final Reveal and Comparison
        printf("\nDealer reveals hidden card: [%s of %s] (Total: %d)\n", dealer.hand[0].name, dealer.hand[0].suit, dealer.total);

        for (int i = 0; i < playerCount; i++) {
            if (players[i].balance <= 0 || strcmp(players[i].status, "won_early") == 0 ) continue; // dont need to display since it already happened

            if (strcmp(players[i].status, "bust") == 0) {
                printf("%s busted and loses.\n", players[i].name);
                players[i].balance -= (players[i].currentBet);
            } else {
                // Rule: Higher hand than dealer wins; equal hands lose
                if (players[i].total > dealer.total) {
                    printf("%s's %d beats Dealer's %d! They win!\n", players[i].name, players[i].total, dealer.total);
                    players[i].balance += (players[i].currentBet);
                } else {
                    printf("%s's %d does not beat Dealer's %d. Dealer wins.\n", players[i].name, players[i].total, dealer.total);
                    players[i].balance -= players[i].currentBet;
                }
            }
        }

    end_round:
        if(dealer.total == 31){
            printf("Dealer got to 31, this is over.\n");
        }
        else{
            printf("Round over.\n");
        }
        printf("Would you like to continue? (y/n): ");
        char next;
        while(1){
            scanf(" %c", &next);
            if(next == 'y'){
                for(int i = 0; i < playerCount; i++){
                    // Only ask players who actually have money left
                    if (players[i].balance > 0) {
                        getBet(&players[i]);
                    } else {
                        printf("Player %s is broke and out of the game!\n", players[i].name);
                    }
                }
                break;
            } else if (next == 'n') {
                // 'n' means end game and ends whole program
                return 0;
            } else {
                // Will go until only 'y' or 'n' is entered
                printf("Invaild Input");
            }
        }

    }

    return 0;
}