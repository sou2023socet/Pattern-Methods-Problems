/*=========================================================
                PatternSearchEngine.c
        Universal Pattern Search Engine
=========================================================*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXT_LENGTH        1000
#define MAX_PATTERN_LENGTH      100
#define ALPHABET_SIZE           256
#define PRIME                   101

/*=========================================================
                    SEARCH ALGORITHMS
=========================================================*/

typedef enum
{
    ALG_NONE = 0,

    ALG_NAIVE,

    ALG_DFA,

    ALG_KMP,

    ALG_RABIN_KARP,

    ALG_BOYER_MOORE

} SearchAlgorithm;

/*=========================================================
                GLOBAL CONFIGURATION
=========================================================*/

SearchAlgorithm algorithm = ALG_NONE;

/*=========================================================
                FUNCTION PROTOTYPES
=========================================================*/

/*-----------------------------
        Utility
------------------------------*/

void resetEngine(void);

void selectAlgorithm(void);

void readInput(char text[],
               char pattern[]);

void displaySelectedAlgorithm(void);

void displayResult(bool found);

void complexityReport(void);

/*-----------------------------
    Search Controller
------------------------------*/

bool searchPattern(const char text[],
                   const char pattern[]);

/*-----------------------------
        Naive Search
------------------------------*/

bool naiveSearch(const char text[],
                 const char pattern[]);

/*-----------------------------
        DFA Search
------------------------------*/

void buildDFA(const char pattern[],
              int dfa[][ALPHABET_SIZE]);

bool dfaSearch(const char text[],
               const char pattern[]);

/*-----------------------------
        KMP
------------------------------*/

void buildLPS(const char pattern[],
              int lps[]);

bool KMPSearch(const char text[],
               const char pattern[]);

/*-----------------------------
        Rabin Karp
------------------------------*/

bool RabinKarpSearch(const char text[],
                     const char pattern[]);

/*-----------------------------
        Boyer Moore
------------------------------*/

void buildBadCharacterTable(
        const char pattern[],
        int badChar[]);

bool BoyerMooreSearch(
        const char text[],
        const char pattern[]);

/*=========================================================
                RESET ENGINE
=========================================================*/

void resetEngine(void)
{
    algorithm = ALG_NONE;
}

/*=========================================================
            SELECT SEARCH ALGORITHM
=========================================================*/

void selectAlgorithm(void)
{
    int choice;

    printf("\n========================================\n");
    printf("      UNIVERSAL PATTERN SEARCH ENGINE\n");
    printf("========================================\n");

    printf("1. Naive Search\n");
    printf("2. DFA Search\n");
    printf("3. KMP Search\n");
    printf("4. Rabin-Karp Search\n");
    printf("5. Boyer-Moore Search\n");

    printf("----------------------------------------\n");

    printf("Enter Choice : ");

    if(scanf("%d",&choice)!=1)
    {
        printf("Invalid Input.\n");
        return;
    }

    resetEngine();

    switch(choice)
    {
        case 1:

            algorithm = ALG_NAIVE;
            break;

        case 2:

            algorithm = ALG_DFA;
            break;

        case 3:

            algorithm = ALG_KMP;
            break;

        case 4:

            algorithm = ALG_RABIN_KARP;
            break;

        case 5:

            algorithm = ALG_BOYER_MOORE;
            break;

        default:

            printf("Invalid Choice.\n");
            break;
    }
}

/*=========================================================
                READ INPUT
=========================================================*/

void readInput(char text[],
               char pattern[])
{
    getchar();

    printf("\nEnter Text : ");

    fgets(text,
          MAX_TEXT_LENGTH,
          stdin);

    text[strcspn(text,"\n")] = '\0';

    printf("Enter Pattern : ");

    fgets(pattern,
          MAX_PATTERN_LENGTH,
          stdin);

    pattern[strcspn(pattern,"\n")] = '\0';
}

/*=========================================================
        DISPLAY SELECTED ALGORITHM
=========================================================*/

void displaySelectedAlgorithm(void)
{
    printf("\n========================================\n");

    printf("Selected Algorithm : ");

    switch(algorithm)
    {
        case ALG_NAIVE:

            printf("Naive Search\n");
            break;

        case ALG_DFA:

            printf("Finite Automata (DFA)\n");
            break;

        case ALG_KMP:

            printf("Knuth-Morris-Pratt (KMP)\n");
            break;

        case ALG_RABIN_KARP:

            printf("Rabin-Karp\n");
            break;

        case ALG_BOYER_MOORE:

            printf("Boyer-Moore\n");
            break;

        default:

            printf("None\n");
    }

    printf("========================================\n");
}

/*=========================================================
            DISPLAY SEARCH RESULT
=========================================================*/

void displayResult(bool found)
{
    printf("\n========================================\n");

    if(found)
    {
        printf("Result : ACCEPTED\n");
        printf("Pattern Found.\n");
    }
    else
    {
        printf("Result : REJECTED\n");
        printf("Pattern Not Found.\n");
    }

    printf("========================================\n");
}

/*=========================================================
                    NAIVE SEARCH
=========================================================*/

bool naiveSearch(const char text[],
                 const char pattern[])
{
    int n = strlen(text);
    int m = strlen(pattern);

    if(m == 0)
        return false;

    for(int i = 0; i <= n - m; i++)
    {
        int j;

        for(j = 0; j < m; j++)
        {
            if(text[i + j] != pattern[j])
                break;
        }

        if(j == m)
            return true;
    }

    return false;
}

/*=========================================================
                DFA TRANSITION TABLE
=========================================================*/

void buildDFA(const char pattern[],
              int dfa[][ALPHABET_SIZE])
{
    int m = strlen(pattern);

    /* Initialize all transitions to 0 */
    for(int state = 0; state <= m; state++)
    {
        for(int ch = 0; ch < ALPHABET_SIZE; ch++)
        {
            dfa[state][ch] = 0;
        }
    }

    /* First character transition */
    dfa[0][(unsigned char)pattern[0]] = 1;

    int restartState = 0;

    for(int state = 1; state <= m; state++)
    {
        /* Copy mismatch transitions */
        for(int ch = 0; ch < ALPHABET_SIZE; ch++)
        {
            dfa[state][ch] = dfa[restartState][ch];
        }

        if(state < m)
        {
            /* Match transition */
            dfa[state][(unsigned char)pattern[state]] = state + 1;

            /* Update restart state */
            restartState =
                dfa[restartState]
                   [(unsigned char)pattern[state]];
        }
    }
}

/*=========================================================
                DFA SEARCH
=========================================================*/

bool dfaSearch(const char text[],
               const char pattern[])
{
    int m = strlen(pattern);

    if(m == 0)
        return false;

    int dfa[MAX_PATTERN_LENGTH + 1][ALPHABET_SIZE];

    buildDFA(pattern, dfa);

    int state = 0;

    for(int i = 0; text[i] != '\0'; i++)
    {
        state =
            dfa[state]
               [(unsigned char)text[i]];

        if(state == m)
        {
            return true;
        }
    }

    return false;
}

/*=========================================================
                    BUILD LPS ARRAY
=========================================================*/

void buildLPS(const char pattern[],
              int lps[])
{
    int m = strlen(pattern);

    int length = 0;
    int i = 1;

    lps[0] = 0;

    while(i < m)
    {
        if(pattern[i] == pattern[length])
        {
            length++;
            lps[i] = length;
            i++;
        }
        else
        {
            if(length != 0)
            {
                length = lps[length - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

/*=========================================================
                    KMP SEARCH
=========================================================*/

bool KMPSearch(const char text[],
               const char pattern[])
{
    int n = strlen(text);
    int m = strlen(pattern);

    if(m == 0)
        return false;

    int lps[MAX_PATTERN_LENGTH];

    buildLPS(pattern, lps);

    int i = 0;
    int j = 0;

    while(i < n)
    {
        if(text[i] == pattern[j])
        {
            i++;
            j++;
        }

        if(j == m)
        {
            return true;
        }
        else if(i < n && text[i] != pattern[j])
        {
            if(j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }

    return false;
}

/*=========================================================
                DISPLAY LPS ARRAY (OPTIONAL)
=========================================================*/

void displayLPS(const char pattern[])
{
    int m = strlen(pattern);

    int lps[MAX_PATTERN_LENGTH];

    buildLPS(pattern, lps);

    printf("\nLPS Array\n");
    printf("-------------------------\n");

    for(int i = 0; i < m; i++)
    {
        printf("%d ", lps[i]);
    }

    printf("\n");
}

/*=========================================================
                RABIN-KARP SEARCH
=========================================================*/

bool RabinKarpSearch(const char text[],
                     const char pattern[])
{
    int n = strlen(text);
    int m = strlen(pattern);

    if(m == 0 || m > n)
        return false;

    int patternHash = 0;
    int textHash = 0;
    int h = 1;

    /* Calculate h = (256^(m-1)) % PRIME */
    for(int i = 0; i < m - 1; i++)
    {
        h = (h * ALPHABET_SIZE) % PRIME;
    }

    /* Compute initial hash values */
    for(int i = 0; i < m; i++)
    {
        patternHash = (ALPHABET_SIZE * patternHash + pattern[i]) % PRIME;
        textHash = (ALPHABET_SIZE * textHash + text[i]) % PRIME;
    }

    for(int i = 0; i <= n - m; i++)
    {
        if(patternHash == textHash)
        {
            int j;

            for(j = 0; j < m; j++)
            {
                if(text[i + j] != pattern[j])
                    break;
            }

            if(j == m)
            {
                return true;
            }
        }

        if(i < n - m)
        {
            textHash =
            (
                ALPHABET_SIZE *
                (textHash - text[i] * h)
                + text[i + m]
            ) % PRIME;

            if(textHash < 0)
            {
                textHash += PRIME;
            }
        }
    }

    return false;
}

/*=========================================================
        BUILD BAD CHARACTER TABLE
=========================================================*/

void buildBadCharacterTable(
        const char pattern[],
        int badChar[])
{
    int m = strlen(pattern);

    for(int i = 0; i < ALPHABET_SIZE; i++)
    {
        badChar[i] = -1;
    }

    for(int i = 0; i < m; i++)
    {
        badChar[(unsigned char)pattern[i]] = i;
    }
}

/*=========================================================
            BOYER-MOORE SEARCH
        (Bad Character Heuristic)
=========================================================*/

bool BoyerMooreSearch(
        const char text[],
        const char pattern[])
{
    int n = strlen(text);
    int m = strlen(pattern);

    if(m == 0 || m > n)
        return false;

    int badChar[ALPHABET_SIZE];

    buildBadCharacterTable(pattern,
                           badChar);

    int shift = 0;

    while(shift <= (n - m))
    {
        int j = m - 1;

        while(j >= 0 &&
              pattern[j] ==
              text[shift + j])
        {
            j--;
        }

        if(j < 0)
        {
            return true;
        }
        else
        {
            int move =
                j -
                badChar[(unsigned char)
                text[shift + j]];

            if(move < 1)
                move = 1;

            shift += move;
        }
    }

    return false;
}

/*=========================================================
                SEARCH CONTROLLER
=========================================================*/

bool searchPattern(const char text[],
                   const char pattern[])
{
    switch(algorithm)
    {
        case ALG_NAIVE:

            return naiveSearch(text, pattern);

        case ALG_DFA:

            return dfaSearch(text, pattern);

        case ALG_KMP:

            return KMPSearch(text, pattern);

        case ALG_RABIN_KARP:

            return RabinKarpSearch(text, pattern);

        case ALG_BOYER_MOORE:

            return BoyerMooreSearch(text, pattern);

        default:

            return false;
    }
}

/*=========================================================
                COMPLEXITY REPORT
=========================================================*/

void complexityReport(void)
{
    printf("\n========================================\n");
    printf("          COMPLEXITY REPORT\n");
    printf("========================================\n");

    switch(algorithm)
    {
        case ALG_NAIVE:

            printf("Algorithm : Naive Search\n");
            printf("Time      : O(n * m)\n");
            printf("Space     : O(1)\n");
            break;

        case ALG_DFA:

            printf("Algorithm : Finite Automata (DFA)\n");
            printf("Build     : O(m * alphabet)\n");
            printf("Search    : O(n)\n");
            printf("Space     : O(m * alphabet)\n");
            break;

        case ALG_KMP:

            printf("Algorithm : Knuth-Morris-Pratt (KMP)\n");
            printf("Build LPS : O(m)\n");
            printf("Search    : O(n)\n");
            printf("Total     : O(n + m)\n");
            printf("Space     : O(m)\n");
            break;

        case ALG_RABIN_KARP:

            printf("Algorithm : Rabin-Karp\n");
            printf("Average   : O(n + m)\n");
            printf("Worst     : O(n * m)\n");
            printf("Space     : O(1)\n");
            break;

        case ALG_BOYER_MOORE:

            printf("Algorithm : Boyer-Moore (Bad Character)\n");
            printf("Best      : O(n / m)\n");
            printf("Average   : O(n)\n");
            printf("Worst     : O(n * m)\n");
            printf("Space     : O(256)\n");
            break;

        default:

            printf("No algorithm selected.\n");
    }

    printf("========================================\n");
}

/*=========================================================
                        MAIN
=========================================================*/

int main()
{
    char text[MAX_TEXT_LENGTH];
    char pattern[MAX_PATTERN_LENGTH];

    /* Step 1: Select Algorithm */
    selectAlgorithm();

    if(algorithm == ALG_NONE)
    {
        printf("Program Terminated.\n");
        return 0;
    }

    /* Step 2: Input */
    readInput(text, pattern);

    /* Step 3: Show selection */
    displaySelectedAlgorithm();

    /* Step 4: Search */
    bool found = searchPattern(text, pattern);

    /* Step 5: Output */
    displayResult(found);

    /* Step 6: Complexity */
    complexityReport();

    return 0;
}