//Elizabeth Achom
//12-4-2023
//GoFish
#define CRT_NO_Warnings
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define HEART "\u2665"
#define DIAMOND "\u2666"
#define SPADE "\u2660"
#define CLUB "\u2663"
#include <stdbool.h>

//Each card consists of a suit [clubs (♣), spades (♠), hearts (♥), or diamonds (♦)) A face (1(Ace) and 2 – 9
typedef struct card_s {
    char suit;
    int face;
    char suits;
    struct card_s *next;
} card;
card *beginningCard = NULL;
card *endCard = NULL;
card *PCHand = NULL; // Declared globally for both PC and User hands
card *UserHand = NULL; // Declared globally for both PC and User hands
card *middlePile = NULL;
int countingCards(card *hand);
void checkingBooks(card **hand, char *playerName, int *userBooks, int *pcBooks);
char cardSelected, PCcardSelected, giveCardOption;


//Linked list to represet different cards
//Function for card deck
void cardDeck(card **beginningCard, card **endCard){
    card *createdCards;
    //Array for cards of a suit [clubs (♣), spades (♠), hearts (♥), or diamonds (♦)) A face (1(Ace) and 2 – 9
    char suitCards[]={'c', 's', 'h', 'd'};
    int faceCards[]={1, 2, 3, 4, 5, 6, 7, 8, 9};
    int i, j;
    //Creating cards
    //for loop for suitCards
    for(j=0; j<4; j++){
        for(i=0; i<9; i++){
            createdCards = (card *)malloc(sizeof(card));
            createdCards->suit = suitCards[j];
            createdCards->face = faceCards[i];
        if (*beginningCard == NULL) {
            *beginningCard = createdCards;
        }else{
            (*endCard) ->next = createdCards;
        }
        *endCard = createdCards;
        }
    }
}
//Function to shuffle deck
void shuffleGoFishDeck(card **deck) {
    card *currCard = *deck;
    int deckSize = countingCards(*deck);

    // Create an array to hold pointers to each card node
    card **cardsArray = (card **)malloc(deckSize * sizeof(card *));
    if (cardsArray == NULL) {
        printf("Malloc failed");
        return;
    }

    // array with pointers to each card in the deck
    int count = 0;
    while (currCard != NULL) {
        cardsArray[count++] = currCard;
        currCard = currCard->next;
    }

    // Shuffle the array of card pointers
    for (int i = deckSize - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap the card pointers
        card *temp = cardsArray[i];
        cardsArray[i] = cardsArray[j];
        cardsArray[j] = temp;
    }

    // Reconstructing the deck using the shuffled pointers
    // Set the beginning of the deck
    *deck = cardsArray[0]; 

    currCard = *deck;
    for (int i = 1; i < deckSize; i++) {
        currCard->next = cardsArray[i];
        currCard = currCard->next;
    }
    currCard->next = NULL; // indicates the end of the deck
    free(cardsArray); // Free 
}
// Function to shuffle the middle pile using shuffleDeck function
void shuffleMiddlePile(card **middlePile) {
    shuffleGoFishDeck(middlePile);
}
//Debug
void printDeck(card *deck) {
    card *currCard = deck;
    while (currCard != NULL) {
        if (currCard->suit == 'c') {
            printf("%s ", CLUB);
        } else if (currCard->suit == 'h') {
            printf("%s ", HEART);
        } else if (currCard->suit == 'd') {
            printf("%s ", DIAMOND);
        } else {
            printf("%s ", SPADE);
        }

        if (currCard->face == 1) {
            printf("A\n");
        } else {
            printf("%d\n", currCard->face);
        }

        currCard = currCard->next;
    }
}
//Debug purposes
void printHand(card *hand){
    card *currCard = hand;
    while (currCard != NULL) {
        if (currCard->suit == 'c') {
            printf("%s", CLUB);
        } else if (currCard->suit == 'h') {
            printf("%s", HEART);
        } else if (currCard->suit == 'd') {
            printf("%s", DIAMOND);
        } else {
            printf("%s", SPADE);
        }

        if (currCard->face == 1) {
            printf("A ");
        } else {
            printf("%d ", currCard->face);
        }

        currCard = currCard->next;
    }
}

//Functions to deal cards
//dealing PC cards
void dealPC(card **deck, card **PCHand) {
    card *dealingCards = *deck;
    *deck = (*deck)->next;

    dealingCards->next = *PCHand;
    *PCHand = dealingCards;
}
//dealing user cards
void dealUser(card **deck, card **UserHand){
    card *dealingCards = *deck;
    *deck = (*deck) -> next;
    
    dealingCards->next = *UserHand;
    *UserHand = dealingCards;
}
//Function to count cards in hand
int countingCards(card *hand) {
    int cardCount = 0;
    card *currCard = hand;
    while (currCard != NULL) {
        cardCount++;
        currCard = currCard->next;
    }
    return cardCount;
}
//debug purposes
// Function to print the cards in the middle pile
void printMiddlePile(card *pile) {
    card *currCard = pile;
    printf("Middle Pile: ");
    while (currCard != NULL) {
        if (currCard->suit == 'c') {
            printf("%s", CLUB);
        } else if (currCard->suit == 'h') {
            printf("%s", HEART);
        } else if (currCard->suit == 'd') {
            printf("%s", DIAMOND);
        } else {
            printf("%s", SPADE);
        }

        if (currCard->face == 1) {
            printf("A ");
        } else {
            printf("%d ", currCard->face);
        }

        currCard = currCard->next;
    }
    printf("\n");
}
//Function to remove requested card from hand
int GiveCard(card **from, card **to, char requestedCard, card **middlePile, char *UserName, int *userBooks, int *pcBooks, int isUserTurn) {
    card *previousCard = NULL;
    card *currCard = *from;
    int foundCard = 0;
    int ExtraTurn = 0;
    card *bookHead = NULL; 
    card *matchedCard = NULL;
    while (currCard != NULL) {
        if ((currCard->face == 1 && requestedCard == 'A') || (currCard->face == (requestedCard - '0'))) {
            card *matchedCard = currCard;
            currCard = currCard->next;
            if (previousCard == NULL) {
                *from = matchedCard->next;  // Update the from if it's the first card
            } else {
                previousCard->next = matchedCard->next; // Skip the card
            }
            matchedCard->next = *to; // Add the card to where it's supposed to go
            *to = matchedCard;

            foundCard = 1;
        } else {
            previousCard = currCard;
            currCard = currCard->next;
        }
    }
    if (foundCard) {
    free(currCard); // Free the dealt card's memory
    ExtraTurn = isUserTurn ? 1 : 0;  // Extra turn only if it's the user's turn
    if(isUserTurn==1){
        printf("\n%s, you get another turn!\n", UserName);
    }
    return ExtraTurn;
    ExtraTurn=0;
    }
    card *pickCard = *middlePile;
    ExtraTurn=0;

    if (!foundCard) {
        printf("><(((('> ><(((('> ><(((('> \n");
        printf("~~~ ~~~ Go Fish ~~~ ~~~\n");
        printf("><(((('> ><(((('> ><(((('> \n");
        ExtraTurn=0;

        // Check if the middle pile has cards
        if (*middlePile != NULL) {
            // If there are cards in the middle pile, pick a card from it
            card *pickCard = *middlePile;
            *middlePile = (*middlePile)->next;
            pickCard->next = *to;
            *to = pickCard;

            // Display the card picked from the center pile
            printf("\n%s, you take ", UserName);
            if (pickCard->face == 1) {
                printf("A");
            } else {
                printf("%d", pickCard->face);
            }

            // Add symbols corresponding to the suit
            switch (pickCard->suit) {
                case 'c':
                    printf("%s ", CLUB);
                    break;
                case 'h':
                    printf("%s ", HEART);
                    break;
                case 'd':
                    printf("%s ", DIAMOND);
                    break;
                case 's':
                    printf("%s ", SPADE);
                    break;
                default:
                    break;
            }
            printf("from the middle pile\n");
        }
        return ExtraTurn;
    }
    // Check if the picked card matches the initially requested card
    if ((requestedCard == 'A' && pickCard->face == 1) || (requestedCard != 'A' && pickCard->face == (requestedCard - '0'))) {
        printf("\n%s, you get another turn!\n", UserName);
        ExtraTurn = 1;
        return ExtraTurn;
        
    }
    bool condition=(giveCardOption == 'y') || 
    (PCcardSelected == requestedCard) || 
    (pickCard->face == 1 && requestedCard == 'A') || 
    (pickCard->face == (requestedCard - '0'));
    if (condition) {
        ExtraTurn = 2;
        return ExtraTurn;
    }
    // Check for books in the user's hand after getting a card from the middle pile
    checkingBooks(to, UserName, userBooks, pcBooks);
    return ExtraTurn;
}
//Function for books
void checkingBooks(card **hand, char *playerName, int *userBooks, int *pcBooks) {
    for (int face = 1; face <= 9; face++) {
        int count = 0;
        card *previousCard = NULL;
        card *currCard = *hand;

        // Count the number of cards with the same face
        while (currCard != NULL) {
            if (currCard->face == face) {
                count++;
            }
            currCard = currCard->next;
        }

        if (count == 4) {
        printf("Book of %c is found on %s's hand\n", (face == 1) ? 'A' : ('0' + face), playerName);
        if (strcmp(playerName, "PC") == 0) {
            (*pcBooks)++;  // Increment the PC's book count
        } else {
            (*userBooks)++;  // Increment the user's book count
        }
        currCard = *hand;
        }


        // Remove the cards with the same face
        while (currCard != NULL) {
            if (currCard->face == face) {
                card *temp = currCard;
                if (previousCard == NULL) {
                    *hand = currCard->next;
                } else {
                    previousCard->next = currCard->next;
                }
                currCard = currCard->next;
            } else {
                previousCard = currCard;
                currCard = currCard->next;
            }
        }
    }
}
int countBooks(card *hand) {
    int bookCount = 0;
    int faceCount[10] = {0}; // Array to count occurrences of each face value

    card *currCard = hand;

    while (currCard != NULL) {
        faceCount[currCard->face]++;
        if (faceCount[currCard->face] == 4) {
            bookCount++;
            printf("Book of %d is found on the hand\n", (currCard->face == 1) ? 10 : currCard->face);

            faceCount[currCard->face] = 0; // Reset the count for the current face value
        }
        currCard = currCard->next;
    }

    return bookCount;
}
//Function to print how many books each player has
void printBookCount(int userBooks, int pcBooks, char *UserName) {
    printf("\n");
    printf("%s's book(s): %d\n", UserName, userBooks);
    printf("PC's book(s): %d\n", pcBooks);
}


//Draw cards from middle pile when deck is empty
void whenHandEmpty(card **hand, card **middlePile, int numCards) {
    for (int i = 0; i < numCards; i++) {
        card *drawCards = *middlePile;
        if (drawCards == NULL) {
            printf("The middle pile is empty. Cannot draw more cards. Game over.\n");
        }
        *middlePile = (*middlePile)->next;

        drawCards->next = *hand;
        *hand = drawCards;
    }
    
}
int main() {
    char UserName[100];
    char continueOption = 'C';
    cardDeck(&beginningCard, &endCard); // Pass pointers to the beginning and end of the deck
    card *GoFishDeck = beginningCard; // Initialize the GoFishDeck
    int i, j, ExtraTurn, pcHasCard, userCardCount;
    card *middlePile = NULL;
    int userBooks = 0;
    int extraTurn;
    int pcBooks = 0;

    srand((unsigned int)time(NULL));// Seed the random number generator 
    //Shuffle deck before dealing
    shuffleGoFishDeck(&GoFishDeck);
    //printDeck(GoFishDeck);    //Debug
    printf("Enter your name: ");
    fgets(UserName, 100, stdin);
    UserName[strcspn(UserName, "\n")] = 0; // Remove the newline
    printf("<°((()))> <°((()))> <°((()))>\n");
    printf("%s, Let's play Go Fish!!\n", UserName);
    printf("<°((()))> <°((()))> <°((()))>\n");
    
    do{
        while(1){
            printf("\nType C/c to Continue: ");
            scanf("%c", &continueOption);
            printf("\n");
            if(continueOption == 'C'|| continueOption == 'c'){
              break;  
            }
            else{
                printf("\nGame ended.");
                return -1;
            }
            
        }
    //Deal 6 cards to User and PC
    for (int i = 0; i < 6; i++) {
        dealUser(&GoFishDeck, &UserHand);
        dealPC(&GoFishDeck, &PCHand);
    }
    // Transfer the remaining cards to the middle pile
    while (GoFishDeck != NULL) {
        card *dealtCard = GoFishDeck;
        GoFishDeck = GoFishDeck->next;

        // Add the dealt card to the middle pile
        dealtCard->next = middlePile;
        middlePile = dealtCard;
    }
    
    userCardCount = countingCards(UserHand);
    printf("%s, your turn. \n", UserName);//Users turn
    printf("%s's Hand has %d cards:\n", UserName, userCardCount);//show user ther cards and how many they have
    printHand(UserHand);
    char selectedAcard;
    extraTurn=1;
 
    do {
    // User's turn to ask for a card
        if(extraTurn==1){
            do {
                // Prompt and get the card input
                printf("\nWhich card (A, 2 - 9) do you want to ask for? ");
                scanf(" %c", &cardSelected);
        
                if (!((cardSelected >= '2' && cardSelected <= '9') || cardSelected == 'A')) {
                    printf("Enter only A, 2 - 9 for the card.\n");
                }
        
                while (getchar() != '\n'); // Clear the input buffer
            } while (!((cardSelected >= '2' && cardSelected <= '9') || cardSelected == 'A'));
            // Check if the middle pile is empty after the user's turn
            if (middlePile == NULL) {
                printf("The middle pile is empty. End of game!\n");
                goto endofGame;
            }
            // Check if the user's hand is empty and draw two cards from the middle pile if needed
            if (userCardCount == 0) {
                printf("%s's hand is empty.", UserName);
                if (middlePile != NULL) {
                    printf("Drawing two cards from the middle pile.\n");
                    if (countingCards(middlePile) >=2 ) {
                        whenHandEmpty(&UserHand, &middlePile, 2);
                        userCardCount = countingCards(UserHand);  // Update user's card count
                    }else if(countingCards(middlePile) ==1){
                        whenHandEmpty(&UserHand, &middlePile, 1);
                        userCardCount = countingCards(UserHand); 
                    }
                }else{
                    printf("\nThe middle pile is also empty. Cannot draw more cards.\n");
                    goto endofGame;//ends game when middle pile empty
                }
            }
            //Calling the GiveCard function to remove requested card or pick from pile
            if (cardSelected == 'A' || (cardSelected >= '2' && cardSelected <= '9')) {
                extraTurn = GiveCard(&PCHand, &UserHand, cardSelected, &middlePile, UserName, &userBooks, &pcBooks, 1);
                checkingBooks(&UserHand, UserName, &userBooks, &pcBooks);
                checkingBooks(&PCHand, "PC", &userBooks, &pcBooks);
                userCardCount = countingCards(UserHand);  // Update user's card count
            }
            
            //Shuffle middle pile
            card *initialMiddlePile = middlePile;
    
            userCardCount = countingCards(UserHand);  // Update user's card count
            // Display user's cards and status
            printf("%s's Hand has %d cards:\n", UserName, userCardCount);
            printHand(UserHand);
        }
        if(extraTurn==0 || extraTurn==2){
            //PC's turn 
            // Check if the PC's hand is empty and draw two cards from the middle pile if needed
            if (countingCards(PCHand) == 0) {
                printf("PC's hand is empty. ");
                if (middlePile != NULL) {
                    printf("Drawing two cards from the middle pile.\n");
                    if (countingCards(middlePile) >=2 ) {
                        whenHandEmpty(&PCHand, &middlePile, 2);
                    }else if(countingCards(middlePile) ==1){
                        whenHandEmpty(&PCHand, &middlePile, 1);;
                    }
                     // Check if the middle pile is empty 
                    else if (middlePile == NULL) {
                        printf("\nThe middle pile is empty. End of game!\n");
                        goto endofGame;
                    }
                } else {
                    printf("The middle pile is also empty. Cannot draw more cards.\n");
                    goto endofGame;
                }
            }
            // Check if the middle pile is empty after the user's turn
            if (middlePile == NULL) {
                printf("\nThe middle pile is empty. End of game!\n");
                goto endofGame;
            }
    
            // Randomly select a card from the computer's hand
            card *currCard = PCHand;
            int count = rand() % countingCards(PCHand);
            for (int i = 0; i < count; i++) {
                currCard = currCard->next;
            }
            PCcardSelected = currCard->face == 1 ? 'A' : ('0' + currCard->face);
            
            
            // Computer will ask based off its deck
            pcHasCard = 0;
            currCard = UserHand;
            while (currCard != NULL) {
                if ((currCard->face == 1 && PCcardSelected == 'A') || (currCard->face == (PCcardSelected - '0'))) {
                    pcHasCard = 1;
                    break;
                }
                currCard = currCard->next;
            }
        
            //Check if middle pile is empty again
            if(middlePile==NULL){
                goto endofGame;
            }
            printf("\nPC: Do you have card(s) with %c\n", PCcardSelected);
            //User answers
            do {
                printf("\nEnter y to give the card(s) with %c or n to say 'Go Fish': ", PCcardSelected);
                scanf(" %c", &giveCardOption);
                // Validate input
                if (giveCardOption != 'y' && giveCardOption != 'n') {
                    printf("Enter 'y' or 'n'.\n");
                }
            
                while (getchar() != '\n'); // Clear the input buffer
            } while (giveCardOption != 'y' && giveCardOption != 'n');
            //Calling the GiveCard function to remove requested card or pick from pile
            if (giveCardOption == 'y') {
                extraTurn = GiveCard(&UserHand, &PCHand, PCcardSelected, &middlePile, "PC", &userBooks, &pcBooks, 0);
                checkingBooks(&UserHand, UserName, &userBooks, &pcBooks);
                checkingBooks(&PCHand, "PC", &userBooks, &pcBooks);
                printf("PC gets another turn!\n");
            } else {
                extraTurn=1;
                printf("<°((()))> <°((()))> <°((()))>\n");
                printf("        Go Fish!!\n");
                printf("<°((()))> <°((()))> <°((()))>\n");
        
                // Check if the middle pile has cards
                if (middlePile != NULL) {
                    // If there are cards in the middle pile, pick a card from it
                    card *pickCard = middlePile;
                    middlePile = middlePile->next;
                    pickCard->next = PCHand;
                    PCHand = pickCard;
            
                    // Display the card picked from the center pile
                    if (pickCard->face == 1) {
                        printf("PC picks from the middle pile\n");
                    } else {
                        printf("PC picks from the middle pile\n");
                    }
                }
            }
            userCardCount = countingCards(UserHand);  // Update user's card count
            // Display user's cards and status
            printf("%s's Hand has %d cards:\n", UserName, userCardCount);
            printHand(UserHand);
            shuffleMiddlePile(&middlePile);
            //Check if middle pile is empty to end game
            if (middlePile == NULL) {
                printf("\nThe middle pile is empty. End of game!\n");
                goto endofGame;
            }
            printf("\nType C/c to Continue: ");
            scanf(" %c", &continueOption);
            
        }
        
    } while (continueOption == 'C' || continueOption == 'c' );

    

    } while (continueOption == 'C' || continueOption == 'c' );
    //Print winner
    endofGame:
    printBookCount(userBooks, pcBooks, UserName);
    if(userBooks > pcBooks){
        printf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
        printf("\n Congratulations %s, you win!!\n", UserName);
        printf("\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    }else if(pcBooks > userBooks){
        printf("\nPC, wins!!\n");
        printf("\nYOU LOSE :(\n");
    }else{
        printf("TIE!!");
    }


    
    // After playing, free the allocated memory
    card *tempm;
    while (beginningCard != NULL) {
        tempm = beginningCard;
        beginningCard = beginningCard->next;
        free(tempm);
    }
    return 0;
}
















