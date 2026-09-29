#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>

const char * const cards[52] = {
    "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS",
    "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH",
    "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD",
    "AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC"
};

int handsize = 8;
char *deck[52];
char *playercards[handsize];
char *dealercards[handsize];
unsigned int bankroll = 10000;
unsigned int bet = 0;
unsigned int cardsdealt = 0;
int pc = 0; 
int dc = 0;
int pv = 0;
int dv = 0;


void drawborder() {
    int i;
    /* screen resolution for text is 64x16 - horizontal bars */
    for (i = 0; i < 64; i++) {
        if (i == 0) {
            gotoxy(i,0);
            putchar(184);
            gotoxy(i,14);
            putchar(139);
        } else if (i == 63) {
            gotoxy(i,0);
            putchar(180);
            gotoxy(i,14);
            putchar(135);
        } else {
            gotoxy(i,0);
            putchar(143);
            gotoxy(i,12);
            putchar(143);
            gotoxy(i,14);
            putchar(191);
        }
    }
    /* veritcal bars */
    for (i = 1; i < 14; i++) {
        gotoxy(0,i);
        putchar(191);
        gotoxy(63,i);
        putchar(191);
    }
}

void displaycards(int dud) { /*dud = Dealer second card up or down.  Down = 0, only show 2 cards, Up = 1, show up to 5. */
    int i = 0;
    int j = 0;
    int k = 0;
    
    for (i = 1; i < 10; i++) { /* Clears card area */
        gotoxy(i,1);
        cprintf("                                       ");
    }
    if (dud == 0) {
        gotoxy(1,1);
        cprintf("Dealer: ");
        for (j = 2; j < 6; j++) {
            for (k = 2; k < 6; k++) {
                gotoxy(j,k);
                putchar(191);
            }
        }
        gotoxy(2,2);
        cprintf("%.*s\n", (int)strlen(dealercards[0]) - 1, dealercards[0]);
        gotoxy(2,3);
        switch (dealercards[0][(int)(strlen(dealercards[0])-1)]) {
            case 'C':
                putchar(195);
                break;
            case 'H':
                putchar(193);
                break;
            case 'S':
                putchar(192);
                break;
            case 'D':
                putchar(194);
        }
        for (j = 7; j < 11; j++) {
            for (k = 2; k < 6; k++) {
                gotoxy(j,k);
                putchar(158);
            }
        }
    } else {
        gotoxy(1,1);
        cprintf("Dealer: %d",dv);
        for (i = 0; i < handsize; i++) {
            if (dealercards[i] != NULL) {
                for (j = (i*5)+2; j < (i*5)+6; j++) {
                    for (k = 2; k < 6; k++) {
                        gotoxy(j,k);
                        putchar(191);
                    }
                }
                gotoxy((i*5)+2,2);
                cprintf("%.*s\n", (int)strlen(dealercards[i]) - 1, dealercards[i]);
                gotoxy((i*5)+2,3);
                switch (dealercards[i][(int)(strlen(dealercards[i])-1)]) {
                    case 'C':
                        putchar(195);
                        break;
                    case 'H':
                        putchar(193);
                        break;
                    case 'S':
                        putchar(192);
                        break;
                    case 'D':
                        putchar(194);
                }
            }
        }    
    }
    gotoxy(1,6);
    cprintf("Player: %d",pv);
    for (i = 0; i < handsize; i++) {
        if (playercards[i] != NULL) {
            for (j = (i*5)+2; j < (i*5)+6; j++) {
                for (k = 7; k < 11; k++) {
                    gotoxy(j,k);
                    putchar(191);
                }
            }
            gotoxy((i*5)+2,7);
            cprintf("%.*s\n", (int)strlen(playercards[i]) - 1, playercards[i]);
            gotoxy((i*5)+2,8);
            switch (playercards[i][(int)(strlen(playercards[i])-1)]) {
                case 'C':
                    putchar(195);
                    break;
                case 'H':
                    putchar(193);
                    break;
                case 'S':
                    putchar(192);
                    break;
                case 'D':
                    putchar(194);
            }
        }
    }    
}

/* 
The titlescreen function shows the intro screen and also seeds the RNG timer with the time it takes the user to press a key.
*/

void titlescreen(void) {
    int x;
    /* wait for any key, use the x-position or a tick counter */
    x = 0;
    clrscr();
    drawborder();
    gotoxy(26,4);
    cprintf("Blackjack!");
    gotoxy(23,5);
    cprintf("By Cliff Friedel");
    gotoxy(18,7);
    cprintf("Press any key to continue.");
    while (!kbhit()) x++;
    srand(x);
}

/*  
This next function shuffles the cards using Fisher-Yates algorithm.  It does this by copying the constant array cards into
another array (deck).  Then it goes through each element, switches it and a random position's cards.  By going through them 
all, it effectively shuffles the cards.
*/

void shuffle(void) {
    int i, j, tmp;
    for (i = 51; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = deck[j];
        deck[j] = deck[i];
        deck[i] = tmp;
    }
}

void clearinput(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

void getbet(void) {
    int i;

    while (bet <= 0) {
        /* loop clears out line if invalid bet showing */
        for (i = 2; i < 63; i++){
            gotoxy(i,13);
            putchar(32);
        }
        gotoxy(2,13);
        cprintf("Bankroll: $%6d", bankroll);
        gotoxy(22,13);
        cprintf("Bet: $");
        scanf("%d",&bet);
        clearinput();
        if ((bet <= 0) || (bet > bankroll)) {
            gotoxy(40,13);
            cprintf("Invalid bet.");
            for (i=0 ; i<10000 ; i++); /* delay loop */
            bet = 0;
        }
    }
    bankroll -= bet;
    gotoxy(2,13);
    cprintf("Bankroll: $%6d", bankroll);
}

void initialcards(void) {

    for (cardsdealt = 0; cardsdealt < 4; cardsdealt++) {
        if (cardsdealt % 2 == 0) {
            playercards[pc] = (char *)deck[cardsdealt];
            pc++;
        } else {
            dealercards[dc] = (char *)deck[cardsdealt];
            dc++;
        }
    }    
}

int computevalue (char *ctc[]) {
    int i = 0; 
    int cv = 0;
    char cardnum; 

    for (i = 0; i < handsize; i++) {
        if (ctc[i] != NULL) {
            cardnum = ctc[i][0];
            switch (cardnum) {
                case '1':
                case 'J':
                case 'Q':
                case 'K':
                    cv += 10;
                    break;
                case '2':
                    cv += 2;
                    break;
                case '3':
                    cv += 3;
                    break;
                case '4':
                    cv += 4;
                    break;
                case '5':
                    cv += 5;
                    break;
                case '6':
                    cv += 6;
                    break;
                case '7':
                    cv += 7;
                    break;
                case '8':
                    cv += 8;
                    break;
                case '9':
                    cv += 9;
                    break;
                case 'A':
                    if (cv < 21) {
                        cv += 11;
                        if (cv > 21) {
                            cv -= 10;
                        }
                    }
            }
        }
    }
    return cv;
}

void playerhit (void) {
    playercards[pc] = (char *)deck[cardsdealt];
    pc++;
    cardsdealt++;
}

void dealerhit (void) {
    dealercards[dc] = (char *)deck[cardsdealt];
    dc++;
    cardsdealt++;
}

int playerturn (void) {
    char choice = NULL;
    int i;

    if (pv == 21) { /* Player hit Blackjack. Pay back his bet plus his bet * 1.5 (3:2 blackjack). Return 1 to show we won. */
        gotoxy(35,13);
        cprintf("Blackjack! You win $%d",(int)(bet*1.5));
        bankroll += bet+(bet*1.5);
        return 1;
    }

    if (dv == 21) { /* Dealer hit blackjack.  Return 0 */
        return 0;
    }

    while (1) {
        gotoxy(1,11);
        cprintf("(H)it or (S)tand: ");
        scanf("%c",&choice);
        clearinput();
        choice = toupper(choice);
        if (choice == 'H') {
            playerhit();
            pv = computevalue(playercards);
            displaycards(0);
            if (pv >= 21) {
                return 0;
            } 
        } else {
        return 0;
        }
    }
}

int dealerturn (void) {
    int i;

    displaycards(1);
    while (1) {
        if (dv < 17) {
            dealerhit();
            dv = computevalue(dealercards);
            displaycards(1);
        }
        if (dv > 21) {
            return 1;
        }
        if ((dv >= 17) && (dv <= 21))
        {
            return 0;
        }
        for (i=0 ; i<10000 ; i++); /* Delay loop */
    }
}


int main(void) {
    int i;
    int wonbj = 0;
    char exit = NULL;

    titlescreen();

    /* Initialize the deck */

    for (i = 0; i < 52; i++) {
        deck[i] = (char *)cards[i];
    }

    while (exit != 'N') {
        wonbj = 0;
        pv = 0; 
        dv = 0;
        pc = 0;
        dc = 0;
        bet = 0;
        cardsdealt = 0;

        for (i = 0; i < 5; i++) {
            playercards[i] = NULL;
            dealercards[i] = NULL;
        }

        clrscr();
        drawborder();
        shuffle();
        getbet();
        initialcards();
        pv = computevalue(playercards);
        dv = computevalue(dealercards);
        displaycards(0);
        wonbj = playerturn();
        if ((wonbj == 0) && (pv <= 21)) {
            dealerturn();
        }
        if (wonbj == 0) {
           if (((pv <= 21) && (pv > dv)) || (dv > 21)) {
                gotoxy(35,13);
                cprintf("You win $%d",bet);
                bankroll += (bet*2);
            }
            else if (pv == dv) {
                gotoxy(35,13);
                cprintf("Push. Bet returned.");
                bankroll += bet;
            }
            else
            {
                gotoxy(40,13);
                cprintf("Dealer wins."); 
            }
        }
        gotoxy(1,11);
        cprintf("                              ");
        gotoxy(1,11);
        cprintf("Play again (Y/N)? ");
        scanf("%c",&exit);
        clearinput();
        if (exit == 'N') {
            return 0;
        }
        if (bankroll <= 0) {
            gotoxy(1,11);
            cprintf("You're broke! Leaving the casino!");
            for (i=0 ; i<10000 ; i++); /* delay loop */
            return 0;
        }
    }
    return 0;
}