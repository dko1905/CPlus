#ifndef COMMANDS_H
#define COMMANDS_H

#ifndef MAX
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif

#define CMD_IMPORT_STR "import"

#define CMD_MAXLEN MAX(0, sizeof(CMD_IMPORT_STR))

#endif
