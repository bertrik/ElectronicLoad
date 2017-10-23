
/** Initializes the logging system. */
void LoggingInit(void);

/**
 * Turns logging on.
 * @param[in] intervalMs the logging interval
*/
void LoggingOn(int intervalMs);

/** Turns logging off. */
void LoggingOff(void);

/** Logs a line if the logging interval has been reached. */
void LoggingUpdate(unsigned long micros, float cur_set, float current, float voltage, float power, float charge, float energy);


