/* input.c:
 * 	Processing, analysing and validating input.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/input.h"

Status cmdparse(int argc, char *argv[])
{	
	// Initialize default status
	Status status;
	status.min_len = GEN_LEN_MIN;
	status.max_len = GEN_LEN_MAX;
	strcpy(status.DES_hash, NO_HASH);
	status.wordlist = NULL;
	status.hashlist = NULL;
	status.alpha = false;
	status.num = false;
	status.sym = false;
	status.err_code = NO_ERROR;

	// Check for no options
	if (argc <= 1) {
		status.err_code = NO_OPTIONS_SELECTED;
		return status;
	}
	
	// Check for flags
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], HELP_FLAG) || !strcmp(argv[i], HELP_FLAG_VERBOSE)) {
			status.err_code = HELP_MENU;
			return status;
		}
		else if (!strcmp(argv[i], DES_HASH_FLAG) || !strcmp(argv[i], DES_HASH_FLAG_VERBOSE)) {
			if (i + 1 >= argc) {
				status.err_code = HASH_NOT_SPECIFIED;
				return status;
			}
			
			if (!check_DES_hash(argv[i + 1])) {
				status.err_code = HASH_INVALID;
				return status;
			}
			strcpy(status.DES_hash, argv[i + 1]);
			status.DES_salt[0] = status.DES_hash[0];
			status.DES_salt[1] = status.DES_hash[1];
			status.DES_salt[2] = '\0';

			i += 1;
		}
		else if (!strcmp(argv[i], LEN_FLAG) || !strcmp(argv[i], LEN_FLAG_VERBOSE)) {
			if (i + 1 >= argc) {
				status.err_code = LEN_NOT_SPECIFIED;
				return status;
			}

			if (!(status.min_len = atoi(argv[i + 1]))) {
				status.err_code = LEN_INVALID;
				return status;
			}

			status.max_len = status.min_len;
			if (i + 2 < argc && atoi(argv[i + 2])) {
				status.max_len = atoi(argv[i + 2]);
				i += 1;
			}

			if (status.min_len < ALLOWED_GEN_LEN_MIN || status.max_len > ALLOWED_GEN_LEN_MAX || status.min_len > status.max_len) {
				status.err_code = LEN_INVALID;
				return status;
			}

			i += 1;
		}
		else if (!strcmp(argv[i], WORDLIST_FLAG) || !strcmp(argv[i], WORDLIST_FLAG_VERBOSE)) {
			if (i + 1 >= argc) {
				status.err_code = WORDLIST_NOT_SPECIFIED;
				return status;
			}

			if (!(status.wordlist = fopen(argv[i + 1], "r"))) {
				strcpy(status.tmp, argv[i + 1]);
				status.err_code = WORDLIST_NOT_FOUND;
				return status;
			}

			strcpy(status.wl_filename, argv[i + 1]);

			i += 1;
		}
		else if (!strcmp(argv[i], HASHLIST_FLAG) || !strcmp(argv[i], HASHLIST_FLAG_VERBOSE)) {
			if (i + 1 >= argc) {
				status.err_code = HASHLIST_NOT_SPECIFIED;
				return status;
			}

			if (!(status.hashlist = fopen(argv[i + 1], "r"))) {
				strcpy(status.tmp, argv[i + 1]);
				status.err_code = HASHLIST_NOT_FOUND;
				return status;
			}

			strcpy(status.hl_filename, argv[i + 1]);

			i += 1;
		}
		else if (!strcmp(argv[i], ALPHA_FLAG) || !strcmp(argv[i], ALPHA_FLAG_VERBOSE))
			status.alpha = true;
		else if (!strcmp(argv[i], NUM_FLAG) || !strcmp(argv[i], NUM_FLAG_VERBOSE))
			status.num = true;
		else if (!strcmp(argv[i], SYM_FLAG) || !strcmp(argv[i], SYM_FLAG_VERBOSE))
			status.sym = true;
		else if (argv[i][0] == '-') {
			status.err_code = UNKNOWN_FLAG;
			strcpy(status.tmp, argv[i]);
			return status;
		}
		else {
			status.err_code = UNKNOWN_VALUE;
			strcpy(status.tmp, argv[i]);
			return status;
		}
	}

	// Set default character set status, if unspecified
	if (!(status.alpha) && !(status.num) && !(status.sym))
		status.alpha = status.num = status.sym = true;

	// Ensure there is at least an inputted hash or a specified hashlist
	if (!strcmp(status.DES_hash, NO_HASH) && !(status.hashlist)) {
		status.err_code = HASH_NOT_SPECIFIED;
		return status;
	}

	return status;
}

bool check_DES_hash(char hash[]) 
{
	size_t hash_len = strlen(hash);

	if (hash_len != DES_HASH_LEN)
		return false;

	for (int i = 0; i < hash_len; ++i)
		if (!isalpha(hash[i]) && !isdigit(hash[i]) && hash[i] != '.' && hash[i] != '/')
			return false;

	return true;
}

int freadline(FILE *fptr, char line[], size_t buf_size)
{
	if (!fptr) {
		return EOF;
	}
	
	int i, c;
	for (i = 0; (c = fgetc(fptr)) != EOF && c != '\n' && i < (buf_size - 1); ++i)
		line[i] = c;
	line[i] = '\0';

	return i;
}
