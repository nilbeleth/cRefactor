#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED


//
#define DEFAULT_LOG_LEVEL refactor::V_Info


// default compile command for to clang to call upon analysis (tool <some macros> <missing stdarg.h library>)
#define DEFAULT_COMPILE_COMMAND "clang-tool -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS"


// file used as compilation database by clang
#define COMPILE_DB_FILE "compile_commands.json"

#endif // DEFINES_H_INCLUDED
