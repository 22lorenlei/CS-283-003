#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here

int getLength(char *);

int getLength(char *word) {
    int charCount = 0;
    while (*word != '\0') {
        charCount++;
        word++;
    }
    return charCount;
}


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int characterCount = 0;
    int whitespaceCount = 0;
    while (*user_str != '\0') {
        if (characterCount >= len) {
            return -1;
        }
        // If it is a space or tab
        if (*user_str == ' ' || *user_str == '\t') {
            // Then incrmement space
            whitespaceCount++;

            // Don't increment if it is a streak, don't copy whitespace if 
            // it is at the beginning or the end
            if (whitespaceCount == 1 && characterCount != 0) {
                *buff = ' ';
                buff++;
                characterCount++;
            }
        } else {
            // Reset if it is a nonspace
            whitespaceCount = 0;
            *buff = *user_str;
            buff++;
            characterCount++;
        }
        user_str++;
    }

    // The case where we end on a space, fix it with a dot
    buff--;
    if (*buff == ' ' || *buff == '\t') {
        *buff = '.';
    }
    buff++;

    // Fill the rest with dots (the -1 is for the \0)
    int numberOfDots = 50 - characterCount; 
    for (int i = 0; i < numberOfDots; i++) {
        *buff = '.';
        buff++;
    }
    *buff = '\0';
    return characterCount; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]");
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT

    // Honestly, I don't thin I need this len
    if (len > 50) {
        return -2;
    }

    int wordCount = 0;
    int isLetter = 0;
    for (int i = 0; i < str_len; i++) {
        if (*buff == ' ' || *buff == '\t') {
            if (isLetter == 1) {
                wordCount++;
            }
            isLetter = 0;
        } else {
            isLetter = 1;
        }
        buff++;
    }
    // If it is just 1 letter or at the end
    if (isLetter == 1) {
        wordCount++;
    }
    return wordCount;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // Because when you evaluate an or statement, it doens't have to evaluate every term for it to reach the result. AT least one true 
    // means it is true.
    // It evaluates left to right, and the moment it encounters a true, it short circuits and returns true and does
    // not need to evaluate any others after that. If argv[1] doesn't exist, then that means that the arg count is less
    // than 2, so that is true, and we don't have to do argv[1] cause it never even goes there.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    // This is another error checking statement. If you add an extra flag or 
    // if there are extra tokens, throw an error. 
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *) malloc (BUFFER_SZ * sizeof(char));

    // This would just be to reset the pointer back to the original memory location
    char *beginningBuff = buff;

    // If there is no memory
    if (buff == NULL) {
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    buff = beginningBuff;

    switch (opt){
        case 'c':
            if (argc != 3) {
                printf("Wrong number of arguments. You have %d instead of 3. Error -2", argc);
                return -2;
            }
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            if (argc != 3) {
                printf("Wrong number of arguments. You have %d instead of 3. Error -2", argc);
                exit(-2);
            }
            char *start = buff;
            char *end = buff + user_str_len - 1;
            char temp;

            // Begin is end, both walk towards each other until begin
            // is right of end
            while (start < end) {
                temp = *start;
                *start = *end;
                *end = temp;
                start++;
                end--;
            }
            break;
        case 'w':
            if (argc != 3) {
                printf("Wrong number of arguments. You have %d instead of 3. Error -2", argc);
                exit(-2);
            }
            buff = beginningBuff;
            int wordCount = 1;
            int charCount = 0;

            // We don't care about the trailing ...
            int totalCharacterCount = user_str_len;

            printf("Word Print\n");
            printf("----------\n");

            while (totalCharacterCount > 0) {
                // Skip any leading whitespaces
                while (*buff == ' ' || *buff == '\t') {
                    buff++;
                    totalCharacterCount--;
                }

                // If it is an empty string
                if (*buff == '\0') {
                    break;
                }

                // Keep a starting pointer per new word. Also, syncs up "position" with wordStart and buff
                char *wordStart = buff;

                // Go through each character in the word (a word starts if it is a nonwhitespace)
                while (*buff != ' ' && *buff != '\t' && totalCharacterCount > 0) {
                    charCount++;
                    buff++;
                    totalCharacterCount--;
                }

                // If there is at least one char, then there must be a word
                if (charCount > 0) {
                    printf("%d. ", wordCount); 
                    // Start of word
                    for (int i = 0; i < charCount; i++) {
                        printf("%c", *wordStart);
                        wordStart++;
                    }
                    printf("(%d)", charCount);
                    // Word finished
                    wordCount++;
                }

                // Any trailing whitespace will be essentially ignored
                while (*buff == ' ' || *buff == '\t') {
                    buff++;
                    totalCharacterCount--;
                }
                printf("\n");
                charCount = 0;
            }
            printf("\n");
            printf("Number of words returned: %d\n", (wordCount - 1));
            break;
        case 'x':
            // Implemented 
            if (argc != 5) {
                printf("Wrong number of arguments. You have %d instead of 5. Error -2", argc);
                return -2;
            } 
            buff = beginningBuff;

            // length of search target
            // Will just search and replace one occurrence only. 

            // Arguments into arrays
            char *searchString = argv[3];
            char *replaceString = argv[4];

            char *tempSearchString = searchString;
            char *tempReplaceString = replaceString;

            int searchLength = getLength(tempSearchString);
            int replaceLength = getLength(tempReplaceString);

            totalCharacterCount = user_str_len;

            int match = 0;
            while (totalCharacterCount > 0 && match != 1) {

                // Search length
                int newSearchLength = searchLength;

                // Used at the end. Brings the pointer back to next char
                int reset = newSearchLength - 1;

                // Set the pointer to the beginning of the search string
                char *searchStart = searchString;

                char *buffCopy = buff;

                // Essentially, it checks the replace word to the current letters
                // If all the characters match, then it is a match
                int charMatch = 0;
                while (newSearchLength > 0) {
                    if (*buff == *searchStart) {
                        charMatch++;
                    }
                    buff++;
                    searchStart++;
                    newSearchLength--;
                }

                // If a match
                if (charMatch == searchLength) {

                    // The case where the repalce word is too big

                    int overflowProblem = 0;
                    if ((replaceLength - searchLength + user_str_len) > BUFFER_SZ) {
                        overflowProblem = replaceLength - searchLength;
                    }

                    match = 1;

                    // Difference between the replace word and the search word
                    int offSet = replaceLength - searchLength;

                    // If the replace word is larger than the search word
                    // Essentially shift what is left after the replace word
                    // by the length of the replace words to the right if it is in the middle or beginning
                    // but don't shift when it is at the end

                    int charactersToShift = totalCharacterCount - searchLength;
                    if (offSet > 0) {
                        // If it is at the end of the string, then just replace the dots with the leftover characters
                        // No need to shift
                        buff--;
                        for (int i = 0; i < charactersToShift - overflowProblem; i++) {
                            
                            // Go to the end of string
                            for (int j = 0; j < charactersToShift - overflowProblem - i; j++) {
                                buff++;
                            }

                            // So we want to get the last character shifted, then the second to last, etc
                            for (int j = 0; j < i; j++) {
                                buff--;
                            }

                            char characterToShift = *buff;

                            // Go to the offset
                            for (int j = 0; j < offSet; j++) {
                                buff++;
                            }

                            *buff = characterToShift;
                            
                            // Undo the offset
                            for (int j = 0; j < offSet; j++) {
                                buff--;
                            }

                            // Undo the second for loop
                            for (int j = 0; j < i; j++) {
                                buff++;
                            }
                            // Go to the end of string
                            for (int j = 0; j < charactersToShift - overflowProblem - i; j++) {
                                buff--;
                            }
                            buff++;
                        }
                        buff = buffCopy;
                        // The replacement starts here
                        for (int i = 0; i < replaceLength; i++) {
                            *buff = *replaceString;
                            buff++;
                            replaceString++;
                        }
                    } 
                    // If the replace word is shorter than the search word
                    // The we have to shift everything to the left by that length
                    else if (offSet < 0) {   
                        // offSet is negative here, so we just change it 
                        offSet *= -1;
                        for (int i = 0; i < charactersToShift; i++) {

                            // Get the characters we need to shift from left to right
                            for (int j = 0; j < i; j++) {
                                printf("%c\n", *buff);
                                buff++;
                            }

                            char characterToShift = *buff;

                            // Go to the offset
                            for (int j = 0; j < offSet; j++) {
                                printf("%c\n", *buff);
                                buff--;
                            }

                            *buff = characterToShift;

                            // Undo offset
                            for (int j = 0; j < offSet; j++) {
                                printf("%c\n", *buff);
                                buff++;
                            }

                            // Undo first for loop
                            for (int j = 0; j < i; j++) {
                                printf("%c\n", *buff);
                                buff--;
                            }
                        }
                        buff = buffCopy;
                        // Replace the string
                        for (int i = 0; i < replaceLength; i++) {
                            *buff = *replaceString;
                            replaceString++;
                            buff++;
                        }
                        // Fill in empty gaps with periods
                        for (int i = 0; i < offSet; i++) {
                            *buff = '.';
                            buff++;
                        }
                        offSet *= -1;
                    }

                    else {
                        // Regular replace with equal lengths
                        for (int i = 0; i < searchLength; i++) {
                            buff--;
                        }
                        for (int i = 0; i < replaceLength; i++) {
                            *buff = *replaceString;
                            replaceString++;
                            buff++;
                        }
                    }
                }

                // Only reset for the next search if we don't have a match
                if (match != 1) {
                    for (int i = 0; i < reset; i++) {
                    buff--;
                    searchStart--;
                    }
                    totalCharacterCount--;
                    match = 0;
                }
            }
            if (match == 0) {
                // No match
                return -2;
            }
            break;
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    buff = beginningBuff;
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
//          What comes to mind here is to prevent segmentation fault problems. 
//          By passing in the pointer to the beginning of the memory that we allocated
//          and also how the length so that we know how much we can move the pointer
//          until we went over our allocated memory. Also, this could be helpful if 
//          it is a team environment. While we may know that our BUFFER_SZ is 50, the
//          other guy might not, so by passing in those arguments, we can ensure the 
//          other people are on the same page and not cause memory problems.
//          