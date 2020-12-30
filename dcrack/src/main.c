/**
 * dcrack, by Gabriel Doyle-Finch
 * 	   V1.0	30.12.2020
 **/

// TODO: GIve user option to save generations for later use as wordlists
// 	 Parallelize dict_attack and bf_attack functions

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <crypt.h>
#include "../lib/input.h"
#include "../lib/crack.h"

// Error message array
static const char err_msg[ERR_MSG_COUNT][MAXBUF] = { "Error: Invalid DES hash.",
					             "Error: DES hash not specified.\n       Use [-d] to specify a DES hash, or [-f] to specify a hashlist.",
					      	     "Error: Invalid length value(s).",
					             "Error: Length values not specified.",
					             "Error: Unknown flag:",
						     "Error: Unknown value:",
					             "Error: Wordlist file could not be opened:",
					             "Error: Wordlist file not specified.",
					             "Error: Hashlist file could not be opened:",
					             "Error: Hashlist file not specified." };

// Print a basic underline of specified length
void underline(char ch, int len);

int main(int argc, char *argv[])
{
	// Parse command-line input
	Status status = cmdparse(argc, argv);

	// Print help menu, if requested
	if (status.err_code == HELP_MENU || status.err_code == NO_OPTIONS_SELECTED) {
		printf("Usage: %s [%s] <DES hash> [%s] <minimum length> <maximum length> [%s] <wordlist file> [%s] <hashlist file> [%s][%s][%s]\n\n",
				argv[0], DES_HASH_FLAG, LEN_FLAG, WORDLIST_FLAG, HASHLIST_FLAG, ALPHA_FLAG, NUM_FLAG, SYM_FLAG);
		printf("\t%s, %s\t\tSpecify a DES hash to crack.\n", DES_HASH_FLAG, DES_HASH_FLAG_VERBOSE);
		printf("\t%s, %s\t\tSpecify a minimum & maximum length or a set length for generated guesses.\n", LEN_FLAG, LEN_FLAG_VERBOSE);
		printf("\t\t\t\tDefault minimum: %d\tDefault maximum: %d\n\n", GEN_LEN_MIN, GEN_LEN_MAX);
		printf("\t%s, %s\t\tSpecify a wordlist file instead of generated guesses.\n", WORDLIST_FLAG, WORDLIST_FLAG_VERBOSE);
		printf("\t%s, %s\t\tSpecify a list of DES hashes, in file format, to crack.\n\n", HASHLIST_FLAG, HASHLIST_FLAG_VERBOSE);
		printf("\t%s, %s\t\tUse alphabetical characters in the generated character set.\n", ALPHA_FLAG, ALPHA_FLAG_VERBOSE);
		printf("\t%s, %s\t\tUse numerical characters in the generated character set.\n", NUM_FLAG, NUM_FLAG_VERBOSE);
		printf("\t%s, %s\t\tUse symbols in the generated character set.\n", SYM_FLAG, SYM_FLAG_VERBOSE);
		exit(EXIT_SUCCESS);
	}
	else if (status.err_code == UNKNOWN_FLAG || status.err_code == UNKNOWN_VALUE || status.err_code == WORDLIST_NOT_FOUND || status.err_code == HASHLIST_NOT_FOUND) {
		printf("%s %s\n", err_msg[status.err_code - 1], status.tmp);
		exit(status.err_code);
	}
	else if (status.err_code != NO_ERROR) {
		printf("%s\n", err_msg[status.err_code - 1]);
		exit(status.err_code);
	}

	// Character set for brute-force attack
	char chset[MAX_CHSET_LEN] = { [0] = '\0' };
	
	// Buffer
	char buf[MAXBUF];

	// Match array
	char match[MAXBUF];

	// Result status
	bool result;

	// Print system status
	int underline_len;
	if (status.wordlist) {
		printf("DICTIONARY ATTACK\n");
		underline_len = printf("Wordlist:    %s\n", status.wl_filename);
		underline('-', underline_len);
	}
	else {
		// Build character set
		if (status.alpha)
			strcat(chset, ALPHA_CHARACTERS);
		if (status.num)
			strcat(chset, NUM_CHARACTERS);
		if (status.sym)
			strcat(chset, SYM_CHARACTERS);

		printf("BRUTE-FORCE ATTACK\nMimimum length:\t%d\nMaximum length:\t%d\n", status.min_len, status.max_len);
		underline_len = printf("Character set: \"%s\"\n", chset);
		underline('-', underline_len);
	}	
	putchar('\n');

	// Attempt to crack a hashlist file
	int hash_no = 1;
	if (status.hashlist) {
		
		printf("Reading file:    %s\n\n", status.hl_filename);
		
		while (freadline(status.hashlist, buf, MAXBUF)) {
				
			// Check for invalid hashes
			if (!check_DES_hash(buf)) {
				printf("%d:\t%s => Invalid DES hash.\n", hash_no, buf);
				continue;
			}
			else if (status.wordlist)
				result = dict_attack(status.wordlist, buf, match);
			else
				result = bf_attack(status.min_len, status.max_len, chset, buf, match);

							
			if (!result)
				printf("%d:\t%s => No match found.\n", hash_no, buf);
			else
				printf("%d:\t%s => \"%s\"\n", hash_no, buf, match);

			++hash_no;
		}
		putchar('\n');
		underline('-', underline_len);
		putchar('\n');
	}
	
	// Attempt to crack an inputted hash
	if (strcmp(status.DES_hash, NO_HASH)) {

		printf("Cracking inputted hash: %s\n\n", status.DES_hash);
		
		if (status.wordlist)
			result = dict_attack(status.wordlist, status.DES_hash, match);
		else
			result = bf_attack(status.min_len, status.max_len, chset, status.DES_hash, match);

		if (!result)
			printf("%s => No match found.\n", status.DES_hash);
		else
			printf("%s => \"%s\"\n", status.DES_hash, match);

		putchar('\n');
		underline('-', underline_len);
		putchar('\n');
	}

	// Print final notes
	printf("NOTE: DES-based hashes only account for a maximum of 8 characters.\n");

	// Close wordlist file, if open
	if (status.wordlist)
		fclose(status.wordlist);

	// Close hashlist file, if open
	if (status.hashlist)
		fclose(status.hashlist);

	exit(EXIT_SUCCESS);
}

void underline(char ch, int len)
{
	for (int i = 0; i < len; ++i)
		putchar(ch);
	putchar('\n');
}
