/**
 * File: contants.h
 *
 * Description: Contains constants used by various parts of the program.
 * 
 * Date: 
 * 15/3/2019
 * 
 * Authors: 
 *	1. Leonardo Cascianelli (me.leonardocascianelli@gmail.com)
 *	2. Rory Brown (rorybrown660@gmail.com)
 *	3. Ewan Skene (ewancskene@gmail.com)
 * 
 */

#ifndef __CONSTANTS__
#define __CONSTANTS__

// Max lengths for environment variable strings
#define MAX_CMD_LEN 5000
#define MAX_CWD_LEN 5000
#define MAX_LOGIN_LEN 5000
#define MAX_HOSTNAME_LEN 5000

// Environment variable names
#define HOME_ENV "HOME"
#define PATH_ENV "PATH"
#define PS1_ENV "PS1"
#define PWD_ENV "PWD"

// Bashanelli history file
#define HISTORY_FILE ".bnli_history"

// Bashanelli profile
#define BNLI_PROFILE ".bnli_profile"

#endif
