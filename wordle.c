#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define TRIES 5


enum GuessState { EXACT, MISPLACED, WRONG, NOTHING };

struct Word { const char * str; unsigned size; };
struct Guess { char c; enum GuessState state; };

struct Guess DEFAULT_GUESS = { '/', NOTHING };


// Words
char * WORDS[] = 
{ 
    "apple", 
    "cow", 
    "tree", 
    "banana", 
    "car", 
    "house",
    "door",
    "cat",
    "catastrophe",
    "tsunami",
    "hurricane",
    "boomerang"
};


void set_word(struct Word * word, const char * word_str);
void clr();


int main() {

    srand(time(NULL));
    unsigned max_words = sizeof(WORDS) / sizeof(WORDS[0]);

    struct Word word;

    while (1)
    {
        clr();

        set_word(&word, WORDS[rand() % max_words]);
        struct Guess guesses[word.size];
        unsigned remaining_tries = TRIES;
        unsigned completed = 1;

        for (unsigned i = 0; i < word.size; i++) 
            guesses[i] = DEFAULT_GUESS;

        while(1)
        {

            for (unsigned i = 0; i < word.size; i++)
                if (! (completed = word.str[i] == guesses[i].c)) 
                    break;


            if (completed)
            {
                printf("\nYou won!");
                break;
            }
            
            printf("\nRemaining tries: %u\nYour word: ", remaining_tries);

            for (unsigned i = 0; i < word.size; i++) 
            {
                struct Guess guess = guesses[i];
                char c;

                switch (guess.state)
                {
                    case EXACT: c = guess.c; break;
                    default: c = '_';
                }
                    
                printf("%c", c);
            }

            if (remaining_tries <= 0) {
                printf("\n\nYou lost! Your word was: %s", word.str);
                break;
            }

            printf("\n\n\nEnter your guess: ");

            char input[word.size];
            scanf("%s", input);

            unsigned input_size = strlen(input);


            if (input_size == 0) continue;
            
            unsigned alpha = 1;
            for (unsigned j = 0; j < input_size; j++)
                if (! (alpha = input[j] >= 'a' && input[j] <= 'z'))
                    break;
                
            if (!alpha)
            {
                printf("Please only use letters from a-z!\n\n");
                continue;
            }

            if (input_size > word.size)
            {
                printf("Please choose a single letter, or a word less/equals than %u characters.\n\n", word.size);
                continue;
            }

            clr();

            remaining_tries--;

            if (input_size == 1) {
                const char c = input[0];
                unsigned contains = strchr(word.str, c) != NULL ? 1 : 0;


                if (!contains) continue;

                for (unsigned i = 0; i < word.size; i++)
                {
                    if (word.str[i] == c)
                    {
                        struct Guess guess = {c, EXACT};
                        guesses[i] = guess;
                    }
                }

                continue;
            }

            for (unsigned i = 0; i < input_size; i++)
            {
                if (guesses[i].state == EXACT) continue;

                const char c = input[i];
                unsigned contains = strstr(word.str, &c) != NULL ? 1 : 0;

                struct Guess guess;
                enum GuessState state = c == word.str[i] ? EXACT : contains ? MISPLACED : WRONG;

                guess.c = c;
                guess.state = state;
                guesses[i] = guess;
            }
        }

        printf("\n\n");
        char c = 'q';

        printf("Would you like to continue? (y/n)\n");
        while (c != 'y' && c != 'n')
        {
            scanf("%c", &c);
        }

        if (c == 'n') break;
    }

    return 0;
}


void set_word(struct Word * word, const char * word_str)
{
    word->str = word_str;
    word->size = strlen(word_str);
}

// Clears the screen
#define ESC "\x1b"
void clr()
{
    printf(ESC "[2J");
    printf(ESC "[H");
    printf(ESC "[1;1H]");
}
