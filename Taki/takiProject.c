//Name: Moran Pol
//ID: 206575409
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_SIZE 21
#define MAX_STATISTICS_ARR_SIZE 14
#define NUM_OF_FIRST_CARDS 4
#define NUM_OF_COLUMNS 9
#define GAME_DIRECTION_WITH_CLOCK 1
#define GAME_DIRECTION_AGAINST_CLOCK -1

//cards
#define MAX_CARDS_TYPE 14
#define MAX_NUMBERS_CARDS 9
#define NO_CARD -1
#define CHANGE_COLOR_CARD 0
#define ONE_CARD 1
#define TWO_CARD 2
#define THREE_CARD 3
#define FOUR_CARD 4
#define FIVE_CARD 5
#define SIX_CARD 6
#define SEVEN_CARD 7
#define EIGHT_CARD 8
#define NINE_CARD 9
#define PLUS_CARD 10
#define STOP_CARD 11
#define CHANGE_DIRECTION_CARD 12
#define TAKI_CARD 13

//color
#define MAX_CARDS_COLOR 4
#define YELLOW_CARD 1
#define RED_CARD 2
#define BLUE_CARD 3
#define GREEN_CARD 4
#define NO_COLOR_CARD 5

struct Card
{
	int color;
	int type;
};
typedef struct Card CARD;

struct Player
{
	char name[MAX_NAME_SIZE];
	CARD* cards;
	int logCardSize;
	int pysCardSize;
};
typedef struct Player PLAYER;

struct Game
{
	PLAYER* players;
	CARD upperCard;
	int numOfPlayers, currentTurn, nextTurn, gameDirection;
};
typedef struct Game GAME;

struct Statistics
{
	int cardValue; //Represents a count of the number of times the card type was chosen as random card.
	int cardType; //Represents the number of the card type.
};
typedef struct Statistics STAT;


CARD* myRealloc(CARD* cardArr, int size, int newsize);
void freeArr(PLAYER players[], int size);
void isNull(void* arr);
void arrayReset(STAT arr[], int size);

CARD chooseRandomCard();
PLAYER insertValuesToPlayer(STAT arr[], int size);
PLAYER* scanPlayers(int* numOfPlayers, STAT arr[], int size);

//print functions
void printRawOfChar(int num, char ch);
void printType(int type);
void printColor(int color);
void printCard(CARD card);
void printWithColors(int color);
void printPlayer(PLAYER player);
void printTurn(PLAYER player, CARD upperCard);
void printStatistics(STAT arr[], int size);

//check if the type of the card is special
bool isPlusCard(CARD card);
bool isStopCard(CARD card);
bool isChangeDirectionCard(CARD card);
bool isTakiCard(CARD card);
bool isChangeColorCard(CARD card);

void deleteCardFromArr(PLAYER* player, int cardIndexToDelete);
void insertCardToArr(PLAYER* player, CARD cardToInsert);
void swap(CARD* card1, CARD* card2);

//check if the choices of the card is correct
int checkCardChoice(PLAYER player, CARD upperCard, int cardChoice);
int checkCardChoiceForTakiCard(PLAYER player, CARD upperCard, int cardChoice);
int checkColor(int colorChoice);
bool isCorrectChoice(CARD upperCard, CARD choiceCard);

//special card functions
void takiCard(GAME* game, STAT arr[], int size);
CARD changeColorCard();
void plusCard(GAME* game, STAT arr[], int size);
int stopCrad(GAME* game, STAT arr[], int size);
int changeDirectionCard(int gameDirection);
void checkSpecialCards(GAME* game, STAT arr[], int size);

//game functions
int ChooseUpperCard(GAME* game, STAT arr[], int size);
int nextPlayerTurn(GAME game);
bool isGameOver(PLAYER players[], int numOfPlayers);
int gameTurn(GAME* game, STAT arr[], int size);
void wholeGame(GAME* game, STAT arr[], int size);

//statistics functions
void cardsStatistics(CARD randomCard, STAT arr[], int size);
void mergeSort(STAT arr[], int size);
STAT* merge(STAT arr1[], int size1, STAT arr2[], int size2);
void copyArr(STAT dest[], STAT src[], int size);

void main()
{
	GAME game;
	STAT statistics[MAX_STATISTICS_ARR_SIZE];
	srand((int)time(NULL));
	game.currentTurn = 0;
	game.nextTurn = 0;
	game.gameDirection = GAME_DIRECTION_WITH_CLOCK;
	game.upperCard.type = rand() % MAX_NUMBERS_CARDS + 1;
	game.upperCard.color = rand() % MAX_CARDS_COLOR + 1;
	arrayReset(statistics, MAX_STATISTICS_ARR_SIZE);
	cardsStatistics(game.upperCard, statistics, MAX_STATISTICS_ARR_SIZE);

	//game
	printf("************  Welcome to TAKI game !!! ***********\n");
	game.players = scanPlayers(&game.numOfPlayers, statistics, MAX_STATISTICS_ARR_SIZE);
	if (game.numOfPlayers > 0)
	{
		wholeGame(&game, statistics, MAX_STATISTICS_ARR_SIZE);
		freeArr(game.players, game.numOfPlayers);
		free(game.players);
	}
	
	//statistics
	printf("\n************ Game Statistics ************\n");
	mergeSort(statistics, MAX_STATISTICS_ARR_SIZE);
	printStatistics(statistics, MAX_STATISTICS_ARR_SIZE);
}

/*A function that receives an array of cards, its size and a new size.
The function returns a new dynamic array of cards with the new size.*/
CARD* myRealloc(CARD* cardArr, int size, int newsize)
{
	CARD* newCardArr = NULL;
	newCardArr = (CARD*)malloc(newsize * sizeof(CARD));
	int i;
	isNull(newCardArr);
	if (newCardArr)
	{
		for (i = 0; i < size && i < newsize; i++)
			*(newCardArr + i) = cardArr[i];
		free(cardArr);
	}
	return newCardArr;
}

/*A function that receives an array of players and size.
The function releases all the dynamic arrys of cards assigned to each player.*/
void freeArr(PLAYER players[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		free(players[i].cards);
}

/*A function that receives a dynamic array.
The function returns true if memory has been allocated to the array and false if not.*/
void isNull(void* arr)
{
	if (arr == NULL)
	{
		printf("Memory allocation faild !!!");
		exit(1);
	}
}

/*A function that receives an array type STAT and size.
The function initializes the array values.
cardType gets the number that represents the card type, cardValue gets the value 0.*/
void arrayReset(STAT arr[], int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		arr[i].cardType = i;
		arr[i].cardValue = 0;
	}
}

//A function that returns a random card after choosing random color and type.
CARD chooseRandomCard()
{
	CARD card;
	card.type = rand() % MAX_CARDS_TYPE;
	if (card.type != CHANGE_COLOR_CARD)
		card.color = rand() % MAX_CARDS_COLOR + 1;
	else
		card.color = NO_COLOR_CARD;
	return card;
}

/*A function that picks up initial values to a player.
In addition, calls to a function that will insert the selected card into the statistics.*/
PLAYER insertValuesToPlayer(STAT arr[], int size)
{
	int i;
	PLAYER player;
	scanf("%s", player.name);
	player.pysCardSize = NUM_OF_FIRST_CARDS;
	player.logCardSize = NUM_OF_FIRST_CARDS;
	player.cards = (CARD*)malloc(player.pysCardSize * sizeof(CARD));
	isNull(player.cards);
	for (i = 0; i < player.logCardSize; i++)
	{
		player.cards[i] = chooseRandomCard();
		cardsStatistics(player.cards[i], arr, size);
	}
	return player;
}

/*A function that receives a pointer of number of players.
The function returns an array of players according to the number of players the user has selected and
absorbs values into the array.*/
PLAYER* scanPlayers(int* numOfPlayers, STAT arr[], int size)
{
	int i, pNumOfPlayers;
	PLAYER* players = NULL;
	printf("Please enter the number of players:\n");
	scanf("%d", &pNumOfPlayers);
	if(pNumOfPlayers > 0)
	{
		players = (PLAYER*)malloc(pNumOfPlayers * sizeof(PLAYER));
		isNull(players);
		if (players)
		{
			for (i = 0; i < pNumOfPlayers; i++)
			{
				printf("Please enter the first name of player #%d:\n", i + 1);
				*(players + i) = insertValuesToPlayer(arr, size);
			}
		}
	}
	*numOfPlayers = pNumOfPlayers;
	return players;
}

/*A function that receives a number and a character.
The function prints a row of characters by the number*/
void printRawOfChar(int num, char ch)
{
	int i;
	for (i = 0; i < num; i++)
		printf("%c", ch);
	printf("\n");
}

/*A function that accepts a number that represents a card type.
The function prints the mark for the card type.*/
void printType(int type)
{
	switch (type)
	{
	case(PLUS_CARD):
		printf("   +   ");
		break;
	case(STOP_CARD):
		printf("  STOP ");
		break;
	case(CHANGE_DIRECTION_CARD):
		printf("  <->  ");
		break;
	case(TAKI_CARD):
		printf("  TAKI ");
		break;
	case(CHANGE_COLOR_CARD):
		printf(" COLOR ");
		break;
	default:
		printf("   %d   ", type); \
			break;
	}
}

/*A function that recives a number that represents a card color.
The function prints the mark for the card color.*/
void printColor(int color)
{
	switch (color)
	{
	case(RED_CARD):
		printf("   R   ");
		break;
	case(BLUE_CARD):
		printf("   B   ");
		break;
	case(GREEN_CARD):
		printf("   G   ");
		break;
	case(YELLOW_CARD):
		printf("   Y   ");
		break;
	default:
		printf("       ");
		break;
	}
}

/*A function that recives  a number that represents a card color.
The function prints with the color according to the card.*/
void printWithColors(int color)
{
	switch (color)
	{
	case(RED_CARD):
		printf("\033[1;31m");
		break;
	case(BLUE_CARD):
		printf("\033[0;34m");
		break;
	case(GREEN_CARD):
		printf("\033[0;32m");
		break;
	case(YELLOW_CARD):
		printf("\033[0;33m");
		break;
	default:
		printf("\033[0m");
		break;
	}
}

/*Function that receives a card.
The function prints the card.*/
void printCard(CARD card)
{
	printWithColors(card.color);
	printRawOfChar(NUM_OF_COLUMNS, '*');
	printf("*       *\n");
	printf("*");
	printType(card.type);
	printf("*\n");
	printf("*");
	printColor(card.color);
	printf("*\n");
	printf("*       *\n");
	printRawOfChar(NUM_OF_COLUMNS, '*');
	printf("\033[0m\n");
}

/*A function that receives a player.
The function prints the player details.*/
void printPlayer(PLAYER player)
{
	int i;
	for (i = 0; i < player.logCardSize; i++)
	{
		printf("Card #%d: \n", i + 1);
		printCard(player.cards[i]);
	}
}

/*A function that receives a player and a main card.
The function prints the details of the player in turn and the main card.*/
void printTurn(PLAYER player, CARD upperCard)
{
	printf("\nUpper card:\n");
	printCard(upperCard);
	printf("%s's turn: \n\n", player.name);
	printPlayer(player);
}

/*A function that receives a STAT array and size.
The function prints the array values in a table.*/
void printStatistics(STAT arr[], int size)
{
	int i;
	printf("Card # | Frequency\n");
	printRawOfChar(18, '_');
	for (i = size - 1; i >= 0; i--)
	{
		printType(arr[i].cardType);
		printf("|    %d\n", arr[i].cardValue);
	}
}

/*Function that receives a card.
The function returns true if it is a plus card and false if not*/
bool isPlusCard(CARD card)
{
	bool res = false;
	if (card.type == PLUS_CARD)
		res = true;
	return res;
}

/*Function that receives a card.
The function returns true if it is a stop card and false if not*/
bool isStopCard(CARD card)
{
	bool res = false;
	if (card.type == STOP_CARD)
		res = true;
	return res;
}

/*Function that receives a card.
The function returns true if it is a change direction card and false if not*/
bool isChangeDirectionCard(CARD card)
{
	bool res = false;
	if (card.type == CHANGE_DIRECTION_CARD)
		res = true;
	return res;
}

/*Function that receives a card.
The function returns true if it is a taki card and false if not*/
bool isTakiCard(CARD card)
{
	bool res = false;
	if (card.type == TAKI_CARD)
		res = true;
	return res;
}

/*Function that receives a card.
The function returns true if it is a change color card and false if not*/
bool isChangeColorCard(CARD card)
{
	bool res = false;
	if (card.type == CHANGE_COLOR_CARD)
		res = true;
	return res;
}

/*A function that receives player and an index of a card to delete.
The function deletes the card from the array of cards and updates the number of cards after deleting.*/
void deleteCardFromArr(PLAYER* player, int cardIndexToDelete)
{
	player->cards[cardIndexToDelete - 1].color = NO_CARD;
	player->cards[cardIndexToDelete - 1].type = NO_CARD;
	swap(&player->cards[cardIndexToDelete - 1], &player->cards[player->logCardSize - 1]);
	player->logCardSize--;
}

/*A function that accepts a player and a card.
The function inserts the card into the array, if there is no space in the array it doubles it and then inserts the card.*/
void insertCardToArr(PLAYER* player, CARD cardToInsert)
{
	if (player->logCardSize < player->pysCardSize)
	{
		player->cards[player->logCardSize] = cardToInsert;
		player->logCardSize++;
	}
	else
	{
		player->cards = myRealloc(player->cards, player->logCardSize, player->logCardSize * 2);
		player->pysCardSize = player->logCardSize * 2;
		player->cards[player->logCardSize] = cardToInsert;
		player->logCardSize++;
	}
}

/*A function that receives 2 cards and switches between them.*/
void swap(CARD* card1, CARD* card2)
{
	CARD temp;
	temp = *card1;
	*card1 = *card2;
	*card2 = temp;
}

/*Function that accepts a player, main card, selected card number.
The function checks whether the value of the selected card is correct,
if it does not, request a new value until a correct value is sacned.
The function returns the correct value.*/
int checkCardChoice(PLAYER player, CARD upperCard, int cardChoice)
{
	bool stop = false;
	while (!stop)
	{
		if (cardChoice == 0)
			stop = true;
		else if (cardChoice > 0 && cardChoice <= player.logCardSize)
		{
			if (isCorrectChoice(upperCard, player.cards[cardChoice - 1]))
				stop = true;
			else
			{
				printf("Invalid choice! Try again.\n");
				printf("Please enter 0 if you want to take a card from the deck\n");
				printf("or 1-%d if you want to put one of your cards in the middle:\n", player.logCardSize);
				scanf("%d", &cardChoice);
			}
		}
		else
		{
			printf("Invalid choice! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", player.logCardSize);
			scanf("%d", &cardChoice);
		}
	}
	return cardChoice;
}

/*Function that accepts a player, main card, selected card number.
The function checks whether the value of the selected card is correct for special taki card,
if it does not, request a new value until a correct value is sacned.
The function returns the correct value.*/
int checkCardChoiceForTakiCard(PLAYER player, CARD upperCard, int cardChoice)
{
	bool stop = false;
	while (!stop)
	{
		if (cardChoice == 0)
			stop = true;
		else if (cardChoice > 0 && cardChoice <= player.logCardSize)
		{
			if (upperCard.color == player.cards[cardChoice - 1].color || isChangeColorCard(player.cards[cardChoice - 1]))
				stop = true;
			else
			{
				printf("Invalid choice! Try again.\n");
				printf("Please enter 0 if you want to take a card from the deck\n");
				printf("or 1-%d if you want to put one of your cards in the middle:\n", player.logCardSize);
				scanf("%d", &cardChoice);
			}
		}
		else
		{
			printf("Invalid choice! Try again.\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", player.logCardSize);
			scanf("%d", &cardChoice);
		}
	}
	return cardChoice;
}

/*A function that receives a number that represents a color.
The function checks if the entered value is correct and if not request a new one until it is correct.
The function returns a correct color value.*/
int checkColor(int colorChoice)
{
	while (colorChoice < YELLOW_CARD || colorChoice > GREEN_CARD)
	{
		printf("Please enter your color choice:\n");
		printf("1 - Yellow\n");
		printf("2 - Red\n");
		printf("3 - Blue\n");
		printf("4 - Green\n");
		scanf("%d", &colorChoice);
	}
	return colorChoice;
}

/*A function that receives a main card and a selected card.
The function returns true if the selected card conforms to the rules of the game and false if not.*/
bool isCorrectChoice(CARD upperCard, CARD choiceCard)
{
	bool res = true;
	if (upperCard.color != choiceCard.color && upperCard.type != choiceCard.type && !isChangeColorCard(choiceCard))
		res = false;
	return res;
}

/*A function that receives a game, an array of STAT type and size.
The function receives cards until the user enters 0.
The function then checks whether the last card inserted is a special card and if so activates the card.*/
void takiCard(GAME* game, STAT arr[], int size)
{
	int cardChoice, numOfChoices = 0;
	printTurn(game->players[game->currentTurn], game->upperCard);
	printf("Please enter 0 if you want to finish your turn\n");
	printf("or 1-%d if you want to put one of your cards in the middle:\n", game->players[game->currentTurn].logCardSize);
	scanf("%d", &cardChoice);
	cardChoice = checkCardChoiceForTakiCard(game->players[game->currentTurn], game->upperCard, cardChoice);
	while (cardChoice != 0 && game->players[game->currentTurn].logCardSize != 0)
	{
		game->upperCard = game->players[game->currentTurn].cards[cardChoice - 1];
		deleteCardFromArr(&game->players[game->currentTurn], cardChoice);
		if (game->players[game->currentTurn].logCardSize != 0)
		{
			printTurn(game->players[game->currentTurn], game->upperCard);
			printf("Please enter 0 if you want to finish your turn\n");
			printf("or 1-%d if you want to put one of your cards in the middle:\n", game->players[game->currentTurn].logCardSize);
			scanf("%d", &cardChoice);
			cardChoice = checkCardChoiceForTakiCard(game->players[game->currentTurn], game->upperCard, cardChoice);
		}
	}
	if (numOfChoices > 0)
		checkSpecialCards(game, arr, size);
}

/*A function that asks the user to enter the color he wants and then returns the main card updated with the color.*/
CARD changeColorCard()
{
	CARD upperCard;
	int colorChoice;
	upperCard.type = CHANGE_COLOR_CARD;
	printf("Please enter your color choice:\n");
	printf("1 - Yellow\n");
	printf("2 - Red\n");
	printf("3 - Blue\n");
	printf("4 - Green\n");
	scanf("%d", &colorChoice);
	colorChoice = checkColor(colorChoice);
	upperCard.color = colorChoice;
	return upperCard;
}

/*A function that receives a game, an array type STAT and size.
The function activates the plus card*/
void plusCard(GAME* game, STAT arr[], int size)
{
	int cardChoose;
	CARD randomCard;
	if (game->players[game->currentTurn].logCardSize == 0)
	{
		randomCard = chooseRandomCard();
		insertCardToArr(&game->players[game->currentTurn], randomCard);
		cardsStatistics(randomCard, arr, size);
	}
	else 
	{
		printTurn(game->players[game->currentTurn], game->upperCard);
		cardChoose = ChooseUpperCard(game, arr, size);
		if (cardChoose != 0)
			checkSpecialCards(game, arr, size);
	}
}

/*A function that receives a game, an array type STAT and size.
The function returns who is the next player to play after stop card according to the direction of the game.*/
int stopCrad(GAME* game, STAT arr[], int size)
{
	CARD randomCard;
	if ((game->numOfPlayers == 1 || game->numOfPlayers == 2) && game->players[game->currentTurn].logCardSize == 0)
	{
		randomCard = chooseRandomCard();
		insertCardToArr(&game->players[game->currentTurn], randomCard);
		cardsStatistics(randomCard, arr, size);
	}
	else if (game->gameDirection == GAME_DIRECTION_WITH_CLOCK)
		game->nextTurn = (game->currentTurn + 2) % game->numOfPlayers;
	else if (game->gameDirection == GAME_DIRECTION_AGAINST_CLOCK && game->numOfPlayers % 2 == 1)
		game->nextTurn = (game->numOfPlayers + game->gameDirection + game->currentTurn + 2) % game->numOfPlayers;
	else if (game->gameDirection == GAME_DIRECTION_AGAINST_CLOCK && game->numOfPlayers % 2 == 0)
		game->nextTurn = (game->numOfPlayers + game->currentTurn + 2) % game->numOfPlayers;
	return game->nextTurn;
}

/*A function that receives the direction of a game.
The function returns the new direction after the change direction card.*/
int changeDirectionCard(int gameDirection)
{
	if (gameDirection == GAME_DIRECTION_WITH_CLOCK)
		gameDirection = GAME_DIRECTION_AGAINST_CLOCK;
	else
		gameDirection = GAME_DIRECTION_WITH_CLOCK;
	return gameDirection;
}

/*A function that receives a game, an array type STAT and size.
The function checks if the selected main card is a special card and if so activates the features relevant to it.*/
void checkSpecialCards(GAME* game, STAT arr[], int size)
{
	if (isTakiCard(game->upperCard))
		takiCard(game, arr, size);
	else if (isChangeColorCard(game->upperCard))
		game->upperCard = changeColorCard();
	else if (isPlusCard(game->upperCard))
		plusCard(game, arr, size);
	else if (isStopCard(game->upperCard))
		game->nextTurn = stopCrad(game, arr, size);
	else if (isChangeDirectionCard(game->upperCard))
		game->gameDirection = changeDirectionCard(game->gameDirection);
}

/*A function that receives a game, an array type STAT and size.
The function asks the user to select a card and checks its correctness,
if not correct will ask for a new card until it is correct.
The function returns the selected card.*/
int ChooseUpperCard(GAME* game, STAT arr[], int size)
{
	int cardChoose;
	CARD randomCard, tempUpperCard = game->upperCard;
	printf("Please enter 0 if you want to take a card from the deck\n");
	printf("or 1-%d if you want to put one of your cards in the middle:\n", game->players[game->currentTurn].logCardSize);
	scanf("%d", &cardChoose);
	cardChoose = checkCardChoice(game->players[game->currentTurn], tempUpperCard, cardChoose);
	if (cardChoose == 0)
	{
		randomCard = chooseRandomCard();
		insertCardToArr(&game->players[game->currentTurn], randomCard);
		cardsStatistics(randomCard, arr, size);
	}
	else
	{
		tempUpperCard = game->players[game->currentTurn].cards[cardChoose - 1];
		deleteCardFromArr(&game->players[game->currentTurn], cardChoose);
	}
	game->upperCard = tempUpperCard;
	return cardChoose;
}

/*The function gets a game.
The function returns who the next player to play.*/
int nextPlayerTurn(GAME game)
{
	if (game.gameDirection == GAME_DIRECTION_WITH_CLOCK)
		game.nextTurn = (game.currentTurn + game.gameDirection) % game.numOfPlayers;
	else
		game.nextTurn = (game.numOfPlayers + game.currentTurn + game.gameDirection) % game.numOfPlayers;
	return game.nextTurn;
}

/*The function gets an array of players and size.
The function returns true if one of the players has finished his cards and is false or not*/
bool isGameOver(PLAYER players[], int numOfPlayers)
{
	int i;
	int res = false;
	for (i = 0; i < numOfPlayers && res == false; i++)
	{
		if (players[i].logCardSize == 0)
		{
			res = true;
			printf("The winner is... %s! Congratulations!\n", players[i].name);
		}
	}
	return res;
}

/*A function that receives a game, the array type STAT and size.
The function activates a player's turn and returns the selected card to the main card.*/
int gameTurn(GAME* game, STAT arr[], int size)
{
	int cardChoice;
	printTurn(game->players[game->currentTurn], game->upperCard);
	cardChoice = ChooseUpperCard(game, arr, size);
	if (cardChoice != 0)
		checkSpecialCards(game, arr, size);
	return cardChoice;
}

/*A function that receives a game, the array type STAT and size.
The function runs the entire game.
The game is over when one of the players running out of cards.*/
void wholeGame(GAME* game, STAT arr[], int size)
{
	bool stop = false;
	int cardChoice;
	while (!stop)
	{
		cardChoice = gameTurn(game, arr, size);
		stop = isGameOver(game->players, game->numOfPlayers);
		if (!stop)
			if ((isStopCard(game->upperCard) && cardChoice == 0) || !isStopCard(game->upperCard))
				game->nextTurn = nextPlayerTurn(*game);
		game->currentTurn = game->nextTurn;
	}
}

/*The function receives a card, an array type STAT and size.
The function updates the appropriate value in the statistics array according to the card.*/
void cardsStatistics(CARD randomCard, STAT arr[], int size)
{
	int i;
	bool stop = false;
	for (i = 0; i < size && !stop; i++)
	{
		if (arr[i].cardType == randomCard.type)
		{
			arr[i].cardValue++;
			stop = true;
		}
	}
}

/*The function receive an array type STAT and size.
A function sorts the array values from the smallest to the largest.*/
void mergeSort(STAT arr[], int size)
{
	STAT* tempArr = NULL;
	if (size <= 1)
		return;
	mergeSort(arr, size / 2);
	mergeSort(arr + size / 2, size - size / 2);
	tempArr = merge(arr, size / 2, arr + size / 2, size - size / 2);
	isNull(tempArr);
	if (tempArr)
	{
		copyArr(arr, tempArr, size);
		free(tempArr);
	}
}

/*A function that receive two arrays and their sizes.
The function returns one unified and sorted array of arrays.*/
STAT* merge(STAT arr1[], int size1, STAT arr2[], int size2)
{
	int i1 = 0, i2 = 0, resI = 0;
	STAT* res = (STAT*)malloc((size1 + size2) * sizeof(STAT));
	isNull(res);
	if (res)
	{
		while (i1 < size1 && i2 < size2)
		{
			if (arr1[i1].cardValue <= arr2[i2].cardValue)
			{
				*(res + resI) = arr1[i1];
				i1++;
			}
			else
			{
				*(res + resI) = arr2[i2];
				i2++;
			}
			resI++;
		}
		while (i1 < size1 && resI <= size1 + size2)
		{
			*(res + resI) = arr1[i1];
			resI++;
			i1++;
		}
		while (i2 < size2 && resI <= size1 + size2)
		{
			*(res + resI) = arr2[i2];
			resI++;
			i2++;
		}
	}
	return res;
}

/*A function that receives 2 arrays and a size.
The function copies the desired array to the second array.*/
void copyArr(STAT dest[], STAT src[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		dest[i] = src[i];
}

