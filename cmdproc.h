// maximum number of arguments (including command itself)
#define CMD_MAX_ARGS 5

// default result codes
#define CMD_OK      0
#define CMD_NO_CMD  -0x7F00
#define CMD_UNKNOWN -0x7F01

typedef int (cmd_fn)(int argc, char *argv[]);

// command table entry
typedef struct {
    const char *name;
    cmd_fn *cmd;
    const char *help;
} cmd_t;

/**
 * parses the given line into arguments
 * matches it with a command in the command table and
 * executes the command
 */
int cmd_process(const cmd_t *commands, char *line);

