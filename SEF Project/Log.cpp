#include <stdio.h>
#include <time.h>
#include <stdarg.h>


static FILE* logFile = NULL;
/*
* FUNCTION: logMessage
* DESCRIPTION: Logs a message with a specific log level and timestamp.
* PARAMETERS:
* const char *level: The level of the log (INFO, WARNING, ERROR).
* const char *message: The message to log.
* RETURNS: None
*/

void logMessage(const char* level, const char* message) {
    if (logFile == NULL) {
        errno_t err = fopen_s(&logFile, "Log.log", "a");
        if (err != 0 || logFile == NULL) {
            perror("Unable to open log file");
            return;
        }
    }

    time_t now = time(NULL);
    struct tm timeinfo;
    char timestamp[64];

    // Correct usage of localtime_s
    if (localtime_s(&timeinfo, &now) != 0) {
        fprintf(stderr, "Failed to get local time\n");
        return;
    }

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
    fprintf(logFile, "[%s] [%s]: %s\n", timestamp, level, message);
    fflush(logFile);
}

/*
* FUNCTION: closeLogger
* DESCRIPTION: Closes the log file if it is open.
* PARAMETERS: None
* RETURNS: None
*/
void closeLogger() {
    if (logFile != NULL) {
        fclose(logFile);
        logFile = NULL;
    }
}