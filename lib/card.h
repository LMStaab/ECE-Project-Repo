#ifndef CARD_H
#define CARD_H

enum SUIT {CLUBS = 'c', SPADES = 's', HEARTS = 'h', DIAMONDS = 'd'};

typedef struct CARD_struct{
    int value;
    SUIT suit;
}CARD;

CARD makeCard(int val, SUIT s){
    CARD card;
    card.value = val;
    card.suit = s;
    return card;
}

int getVal(CARD card){
    return card.value;
}

char getSuit(CARD card){
    return card.suit;
}

#endif