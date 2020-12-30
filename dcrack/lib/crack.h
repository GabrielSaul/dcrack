/* crack.h:
 * 	Functionality for cracking hashes.
 */

#include <stdbool.h>

// Character sets
#define ALPHA_CHARACTERS	"etaoinsrhldcumfpgwybvkxjqzETAOINSRHLDCUMFPGWYBVKXJQZ"
#define NUM_CHARACTERS		"0123456789"
#define SYM_CHARACTERS		" ~!@#$%^&*_-+=`|\\(){}[]:;\"\'<>,.?/"

// Maximum character set size
#define MAX_CHSET_LEN	95

// Dictionary attack function
bool dict_attack(FILE *fptr, const char hash[], char match[]);

// Generated brute-force attack function
bool bf_attack(const int min_len, const int max_len, const char chset[], const char hash[], char match[]);
