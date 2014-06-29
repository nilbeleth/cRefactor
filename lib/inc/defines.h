#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED


//
#define DEFAULT_LOG_LEVEL refactor::V_Error


// default compile command for to clang to call upon analysis (tool <some macros> <missing stdarg.h library>)
#define DEFAULT_COMPILE_COMMAND "clang-tool -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS"

// all files that should be changes are backed up
#define BACKUP_CHANGED_FILES false

// file used as compilation database by clang
#define COMPILE_DB_FILE "compile_commands.json"

// enforce overwriting compilation database
#define COMPILE_DB_OVERWRITE true

#endif // DEFINES_H_INCLUDED
