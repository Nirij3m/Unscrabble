//
// Created by nirin on 28/08/2023.
//

#include "scrabble.h"
extern int valueScrabble[MAX_SIZE];

void toLowerString(char* string){
    unsigned int len = strlen(string);
    for(unsigned int i = 0; i < len; i++){
        string[i] = tolower(string[i]);
    }
}
void toUpperString(char* string){
    unsigned int len = strlen(string);
    for(unsigned int i = 0; i < len; i++){
        string[i] = toupper(string[i]);
    }
}


int valueOfWord(char* word){
    unsigned int len = strlen(word);
    int value = 0;
    for(unsigned int i = 0; i < len; i++){
        value += valueScrabble[word[i] - 'a']; //uses the equivalent array letter<=>value at the letter position
    };
    return value;
}
int* wordToArray(char* word){
    int* array = malloc(26*sizeof(int));
    for(int j = 0; j < 26; j++){
        array[j] = 0;
    }
    unsigned int len = strlen(word);
    for(unsigned int i = 0; i < len; i++){
        array[word[i]-'a']++;
    }
    return array;
}
void shuffle(char* array, int length) {
    srand(time(NULL));
    for (int i = length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

char* pickLetters(char* deck){
    char scrabbleAlphabet[93] = "AAAAAAAAABBCCDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSSSTTTTTTUUVWXYZ";
    //shuffle(scrabbleAlphabet, 93);
    srand(time(NULL));

    for(int i = 0; i < DECK_SIZE; i++){
        int r = rand() % 93; //93 to fit in the scrabbleAlphabet
        deck[i] = tolower(scrabbleAlphabet[r]); //pick a random letter in the scrabbleAlphabet and put it in the deck
        //printf("%c", deck[i]);
    }
    return deck;
}
bool wordValid(struct NodeTrie* trie, char* word, char* deck){
    char correctDeck[DECK_SIZE] = "";
    strncpy(correctDeck, deck, DECK_SIZE);
    bool exists = belongs(trie, word);
    if(!exists){
        return false;
    }
    int* wordedArray = wordToArray(correctDeck);
    unsigned int len = strlen(word);

    for(unsigned int i = 0; i < len; i++){
        int index = word[i] - 'a';
        if(wordedArray[index] == 0){ //Your word contains a letter which is not in the deck
            free(wordedArray);
            return false;
        }
        else{ //the letter exists in the deck
            wordedArray[index]--;
        }
    }
    //All the letters choosen belongs to the deck
    free(wordedArray);
    if(exists){
        return true;
    }
    else return false;
}

void printDeck(char deck[DECK_SIZE]){
    for(int i = 0; i < DECK_SIZE; i++){
        printf("%c-", deck[i]);
    }
    printf("\n");
}

void findLongestWordInTrie(struct NodeTrie* trie, int wordedDeck[26], char currentWord[DECK_SIZE], char* longestWord, struct NodeTrie* originalTrie) {
    // Si toutes les lettres du deck sont épuisées, sortir de la recherche
    bool allLettersExhausted = true;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (wordedDeck[i] > 0) {
            allLettersExhausted = false;
            break;
        }
    }
    if (allLettersExhausted) {
        return;
    }
    // Vérifier si le mot actuel est plus long que le plus long mot trouvé jusqu'à présent
    if (strlen(currentWord) > strlen(longestWord) && belongs(originalTrie, currentWord)) {
        strncpy(longestWord, currentWord, DECK_SIZE);
        longestWord[DECK_SIZE] = '\0'; // Assurer la terminaison correcte
    }
    // Explorer toutes les lettres possibles dans le deck
    for (int i = 0; i < MAX_SIZE; i++) {
        if (wordedDeck[i] > 0 && trie->alphabets[i] != NULL) {
            char nextLetter = i + 'a';

            // Utiliser la lettre du wordedDeck
            wordedDeck[i]--;

            // Construire le prochain mot en ajoutant la lettre
            char newWord[DECK_SIZE] ="";
            snprintf(newWord, sizeof(newWord), "%s%c", currentWord, nextLetter);

            // Appel récursif pour explorer le nœud suivant
            findLongestWordInTrie(trie->alphabets[i], wordedDeck, newWord, longestWord, originalTrie);

            // Restaurer le wordedDeck après l'appel récursif
            wordedDeck[i]++;
        }
    }
}
void findHighestValueWordInTrie(struct NodeTrie* trie, int wordedDeck[26], char currentWord[DECK_SIZE], char* highestValueWord, int* highestValue, struct NodeTrie* originalTrie) {
    // Si toutes les lettres du deck sont épuisées, sortir de la recherche
    bool allLettersExhausted = true;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (wordedDeck[i] > 0) {
            allLettersExhausted = false;
            break;
        }
    }
    if (allLettersExhausted) {
        return;
    }

    // Calculer la valeur actuelle du mot en cours de construction
    int currentWordValue = 0;
    for (int i = 0; i < strlen(currentWord); i++) {
        if (trie->alphabets[currentWord[i] - 'a'] != NULL) {
            currentWordValue += trie->alphabets[currentWord[i] - 'a']->value;
        }
    }

    // Vérifier si le mot actuel a une valeur plus élevée que le plus haut mot trouvé jusqu'à présent
    if (currentWordValue > *highestValue && belongs(originalTrie, currentWord)) {
        *highestValue = currentWordValue;
        memset(highestValueWord, 0, strlen(highestValueWord));
        strncpy(highestValueWord, currentWord, DECK_SIZE);
        highestValueWord[DECK_SIZE] = '\0'; // Assurer la terminaison correcte
    }

    // Explorer toutes les lettres possibles dans le deck
    for (int i = 0; i < MAX_SIZE; i++) {
        if (wordedDeck[i] > 0 && trie->alphabets[i] != NULL) {
            char nextLetter = i + 'a';

            // Utiliser la lettre du wordedDeck
            wordedDeck[i]--;

            // Construire le prochain mot en ajoutant la lettre
            char newWord[DECK_SIZE] = "";
            snprintf(newWord, sizeof(newWord), "%s%c", currentWord, nextLetter);

            // Appel récursif pour explorer le nœud suivant
            findHighestValueWordInTrie(trie->alphabets[i], wordedDeck, newWord, highestValueWord, highestValue, originalTrie);

            // Restaurer le wordedDeck après l'appel récursif
            wordedDeck[i]++;

        }
    }
}

















