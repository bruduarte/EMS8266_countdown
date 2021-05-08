#ifndef UTIL_H_
#define UTIL_H_

/**
 * @file Util.h
 *
 * "Write a short description of the file here."
 *
 * "Write the detailed description of the file here."
 *
 * $Author:  $
 * $Date:  $
 * $Revision:  $
 *
 * @copyright ThyssenKrupp Presta
 *
 */

#include <stdio.h>
/**** MACROS *************************************************************************************/
#define DEBUG

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#ifdef _WIN32
    #define debug_print(fmt, ...) \
            do { if (DEBUG_TEST) fprintf(stdout, fmt, ##__VA_ARGS__); cout << std::flush;} while (0)
#else
    #define debug_print(fmt, ...) \
            do { if (DEBUG_TEST) Serial.printf(fmt, ##__VA_ARGS__); } while (0)
#endif

/**** END OF MACROS ******************************************************************************/


/**** TYPE DEFINITIONS ***************************************************************************/

/**** END OF TYPE DEFINITIONS ********************************************************************/


/**** GLOBAL FUNCTION PROTOTYPES *****************************************************************/

/**** END OF GLOBAL FUNCTION PROTOTYPES **********************************************************/

#endif /* UTIL_H_ */
