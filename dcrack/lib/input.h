/* input.h:
 * 	Functionality for processing & analysing input.
 */

#include <stdbool.h>

// Maximum buffer size
#define MAXBUF	4096

// Standard DES hash length values
#define DES_SALT_LEN		2
#define DES_HASH_LEN		13

// Default minimum & maximum length of a generated guess
#define GEN_LEN_MIN		1
#define GEN_LEN_MAX		8

// Minimum & maximum allowed length of a generation
#define ALLOWED_GEN_LEN_MIN	1
#define ALLOWED_GEN_LEN_MAX	8

// Help flags
#define HELP_FLAG		"-h"
#define HELP_FLAG_VERBOSE	"--help"

// Command-line flags
#define DES_HASH_FLAG		"-d"
#define DES_HASH_FLAG_VERBOSE	"--des"
#define LEN_FLAG		"-l"
#define LEN_FLAG_VERBOSE	"--length"
#define WORDLIST_FLAG		"-w"
#define WORDLIST_FLAG_VERBOSE	"--wordlist"
#define HASHLIST_FLAG		"-f"
#define HASHLIST_FLAG_VERBOSE	"--hashfile"
#define ALPHA_FLAG		"-a"
#define ALPHA_FLAG_VERBOSE	"--alpha"
#define NUM_FLAG		"-n"
#define NUM_FLAG_VERBOSE	"--numbers"
#define SYM_FLAG		"-s"
#define SYM_FLAG_VERBOSE	"--symbols"

// Command-line error codes
#define HELP_MENU		101
#define NO_ERROR		0
#define HASH_INVALID		1
#define HASH_NOT_SPECIFIED	2
#define LEN_INVALID		3
#define LEN_NOT_SPECIFIED	4
#define UNKNOWN_FLAG		5
#define UNKNOWN_VALUE		6
#define WORDLIST_NOT_FOUND	7
#define WORDLIST_NOT_SPECIFIED	8
#define HASHLIST_NOT_FOUND	9
#define HASHLIST_NOT_SPECIFIED	10
#define NO_OPTIONS_SELECTED	11

// Number of error messages
#define ERR_MSG_COUNT		11

// Other sentinel messages
#define NO_HASH			"NO_HASH"

// Command-line status
typedef struct status {
	int min_len;
	int max_len;
	char DES_hash[DES_HASH_LEN + 1];
	char DES_salt[DES_SALT_LEN + 1];
	FILE *wordlist;
	FILE *hashlist;
	char wl_filename[MAXBUF];
	char hl_filename[MAXBUF];
	bool alpha;
	bool num;
	bool sym;
	int err_code;
	char tmp[MAXBUF];
} Status;

// Command-line parser for the dcrack program
Status cmdparse(int argc, char *argv[]);

// DES-based hash validator
bool check_DES_hash(char hash[]);

// Line reader for wordlist & hashlist files
int freadline(FILE *fptr, char line[], size_t buf_size);
