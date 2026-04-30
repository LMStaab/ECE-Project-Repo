/*
 * Code Authors: Lucas Staab, Douglas Sheppard
 * Course: ECE 101
 * Assignment: Final project, game of 31
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 4
#define DECK_SIZE 52
#define MAX_NAME 50

// Structs done by Douglas

typedef struct card_struct{
    char suit[MAX_NAME];
    int value;
} Card;

typedef struct deck_struct{
    Card cards[DECK_SIZE];
    int top;
} Deck;

typedef struct player_struct{
    char name[MAX_NAME];
    int balance;
    int currentBet;
    Card hand[10];
    int handCount;
    int total;
    char status[20]; // "stand", "bust", "thirty-one", "fourteen"
} Player;

typedef struct dealer_struct{
    Card hand[10];
    int handCount;
    int total;
} Dealer;

// Utility Functions

// Done by Lucas
void initDeck(Deck *deck) {
    char *suits[] = {"Clubs", "Spades", "Hearts", "Diamonds"};
    char *names[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int vals[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
    
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            sprintf(deck->cards[k].suit, "[%s of %s]", names[j], suits[i]);
            deck->cards[k].value = vals[j];
            k++;
        }
    }
    deck->top = 0;

    // Shuffle
    for (int i = 0; i < DECK_SIZE; i++) {
        int r = i + rand() % (DECK_SIZE - i);
        Card temp = deck->cards[i];
        deck->cards[i] = deck->cards[r];
        deck->cards[r] = temp;
    }
}

// Returns the top card // Done by Lucas
Card drawTop(Deck *deck) {
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
    printf("\n%s's Turn\n", p->name);
    p->balance -= p->currentBet;
    
    while (1) {
        updateHandTotal(p); // Make total up to date
        printf("%s has: ", p->name); 
        for(int i=0; i < p->handCount; i++) {
            printf("%s ", p->hand[i].suit); // Print cards
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
            printf("#31!\n");
            strcpy(p->status, "thirty-one");
            break;
        }

        printf("Would you like to 'hit' or 'stand'? ");
        scanf("%s", choice);

        if (strcmp(choice, "hit") == 0) {
            p->hand[p->handCount++] = drawTop(deck);
            if (p->hand[p->handCount-1].value == 11 || p->total == 11) {
                // Player choses whether ace is worth 1 or 11 points
                int aceVal;
                printf("You drew an Ace! Is it worth 1 or 11? "); 
                scanf("%d", &aceVal); 
                p->hand[p->handCount-1].value = aceVal;
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
    if (p->currentBet > p->balance || p->currentBet <= 0){
        // Sets the bet to max value if too much is entered or too little
        p->currentBet = p->balance;
        printf("Balance is only $%d, bet set to $%d\n", p->balance, p->balance);
    }
}

// Done by Douglas
int dealersTurn(Dealer *dealer, Deck *deck){
    dealer->total = dealer->hand[0].value;
    int dealerBusted = 0;

    // Dealer strategy: continue drawing to get as close to 31 as possible,
    // Stops at 14, we picked 25 as our stopping point
    while (dealer->total < 25) { 
        dealer->hand[dealer->handCount++] = drawTop(deck);
        dealer->total += dealer->hand[dealer->handCount - 1].value; // add new card to the total;

        // If dealer hits 14, he reveals hand and stops drawing
        if (dealer->total == 14) {
            printf("Dealer hit 14 and must stop drawing.\n");
            break;
        }

        // Bets are already taken from player accounts so no need to decrement
        if(dealer->total == 31){
            printf("Dealer hit 31 womp womp :( players lose\n");
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
        printf("MAX PEOPLE IS 4, ONLY MAKING $ PLAYERS");
        playerCount = 4;
    }

    // Creates all the player structs for storing player info
    Player players[MAX_PLAYERS];
    for (int i = 0; i < playerCount; i++) {
        printf("Enter name for Player %d: ", i + 1);
        scanf("%s", players[i].name);
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
    

        // Rule: Dealer reveals all cards except one
        printf("\nDealer reveals: ");
        for(int i = 1; i < dealer.handCount; i++) {
            printf("%s ", dealer.hand[i].suit); //
        }
        printf("[One Card Face Down]\n"); // Dealer shows all but one

        // Player's Turn
        for (int i = 0; i < playerCount; i++) {
            if (players[i].balance > 0) {
                // Do the player turn, will go until win or bust
                playerTurn(&players[i], &deck);
                if(players[i].total >= 14 && dealer.total == 14) { // Loss case by 14
                    printf("%s loses because the dealer got 14!\n", players[i].name); 
                    strcpy(players[i].status, "lost_foteen");
                }
                // If player hits 14 or 31 and dealer doesn't have 14, player wins
                if ((players[i].total == 14 || players[i].total == 31) && dealer.total != 14) { // Win case by 31
                    printf("%s wins immediately with %d!\n", players[i].name, players[i].total);
                    players[i].balance += (players[i].currentBet * 2);
                    strcpy(players[i].status, "won_early");
                }
            }
        }

        // Final Reveal and Comparison
        printf("\nDealer reveals hidden card: %s (Total: %d)\n", dealer.hand[0].suit, dealer.total);

        for (int i = 0; i < playerCount; i++) {
            if (players[i].balance <= 0 || strcmp(players[i].status, "won_early") == 0 || strcmp(players[i].status, "lost_foteen") == 0) continue; // dont need to display since it already happened

            if (strcmp(players[i].status, "bust") == 0) {
                printf("%s busted and loses.\n", players[i].name);
            } else {
                // Rule: Higher hand than dealer wins; equal hands lose
                if (players[i].total > dealer.total) {
                    printf("%s's %d beats Dealer's %d! They win!\n", players[i].name, players[i].total, dealer.total);
                    players[i].balance += (players[i].currentBet * 2);
                } else {
                    printf("%s's %d does not beat Dealer's %d. Dealer wins.\n", players[i].name, players[i].total, dealer.total);
                }
            }
        }

    end_round:
        printf("Round over.\nWould you like to continue? (y/n): ");
        char next;
        while(1){
            scanf(" %c", &next);
            if(next == 'y'){
                for(int i = 0; i < playerCount; i++){
                    // Get a new bet for all the players
                    getBet(&players[i]);
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