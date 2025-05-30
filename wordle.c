#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int getFileLength(FILE *fileName) {
    // open file
    FILE *wFile = fileName;
    if (wFile == NULL) {
        printf("Failed to open file");
        return 1;
    }
    
    // count lines in file
    int fileLength = 0;
    char buffer[64];
    while (fgets(buffer, sizeof(buffer), wFile)){
        fileLength++;
    }

    return fileLength;

}
// user guess
char* getUserGuess() {
    char *userInput = malloc(6);
    if (!userInput) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    printf("_____\r");
    scanf("%5s", userInput);
    printf("\x1b[1F");
    return userInput;
}

int printProgress(const char *guess, const char *answer, int rounds) {
    int score = 0;
    printf("\r");
    for (int a = 0; a < 5; a++){
        // green
        if (guess[a] == answer[a]){
            printf("\033[1;32m%c\033[0m", answer[a]);
            score++;
        }
        // yellow
        else if (strchr(answer, guess[a])){
            printf("\033[1;33m%c\033[0m", guess[a]);
        }
        // gray
        else {
            printf("\033[37m%c\033[0m", guess[a]);
            // printf("_");
        }
    }
    printf("\n");
    if (score == 5){
        if (rounds < 1) {
            printf("You guessed the wordle! It took %d try!\n", rounds+1);
        }
        else {
            printf("You guessed the wordle! It took %d tries.\n", rounds+1);
        }
    }

    return score;
}

char* getWord(FILE *fileName, int fileLength) {
    FILE *wordsFile = fileName;
    // get random word
    srand(time(NULL));
	int randint = rand() % fileLength;
    int curind = 0;
    char *line = malloc(8);

    rewind(wordsFile);
    while (fgets(line, 8, wordsFile)){
        if (curind == randint){
            line[strcspn(line, "\n")] = '\0';
            break;
        }
        curind++;
        
    }

    return line;
}

int play(const char *answer, int rounds) {

   // user guess
   char *userGuess = getUserGuess();

    // print progress
    int score = printProgress(userGuess, answer, rounds);
    free(userGuess);

    return score;
}

int main() {

    // open word txt file and get length
    // MODIFY PATH WITH SPECIFIC DIRECTORY: char* path = "[enter directory here]"
    char* path = "wordsFile.txt";
    FILE *wordsFile = fopen(path, "r");
    int fileLength = getFileLength(wordsFile);

    char *answer = getWord(wordsFile, fileLength);

    printf(" 🅆 🄾 🅁 🄳 🄻 🄴\n\n");\

    // 5 rounds max, unless user wins before that
    int rounds = 0;
    int score = 0;
    while (rounds < 5) {
        if (score == 5) {
            break;
        }
        score = play(answer, rounds);
        rounds++;
    }

    if (rounds == 5 && score < 5) {
        // printf("%s", answer);
        printf("The word was \033[1;30m%s\033[0m. You didn't guess it this time. Try again!\n", answer);
    }
    free(answer);

	// close
	fclose(wordsFile);		
	return 0;
}