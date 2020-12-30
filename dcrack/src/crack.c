/* *
 * crack.c:
 * 	Dictionary & brute force attack functionality for cracking DES-based hashes.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <crypt.h>
#include <math.h>
#include <omp.h>
#include "../lib/crack.h"
#include "../lib/input.h"

bool dict_attack(FILE *fptr, const char hash[], char match[])
{
	if (!fptr) {
		fprintf(stderr, "FATAL: Null file pointer passed to dict_attack\n");
		exit(EXIT_FAILURE);
	}

	fseek(fptr, 0L, SEEK_SET);

	const char salt[DES_SALT_LEN + 1] = { [0] = hash[0], [1] = hash[1], [2] = '\0' };
	char buf[MAXBUF];

	while (freadline(fptr, buf, MAXBUF)) {

		if (strlen(buf) > ALLOWED_GEN_LEN_MAX)
			buf[ALLOWED_GEN_LEN_MAX] = '\0';

		if (!strcmp(hash, crypt(buf, salt))) {
			strcpy(match, buf);
			return true;
		}
	}
	
	return false;
}

bool bf_attack(const int min_len, const int max_len, const char chset[], const char hash[], char match[])
{
	const char salt[DES_SALT_LEN + 1] = { [0] = hash[0], [1] = hash[1], [2] = '\0' };
	char gen[max_len + 1];
	int len = min_len;
	
	int index[max_len];
	const int chset_len = (int) strlen(chset);

	for (int i = 0; i < len; ++i)
		gen[i] = chset[0];
	gen[len] = '\0';

	while (len <= max_len) {
	  	
		if (!strcmp(crypt(gen, salt), hash)) {
        		strcpy(match, gen);
			return true;
        	}

        	index[0]++;

        	for (int i = 0; i < len && len <= max_len; i++) {
            		gen[i] = chset[index[i] % chset_len];

            		if (index[i] && !(index[i] %= chset_len)) {
                		if (i == len - 1)
                    			index[len++] = 0;
                		else
                    			index[i + 1]++;
			}
			else
				break;
            	}
		gen[len] = '\0';
       
    	}
	
	return false;
}
