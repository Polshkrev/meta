#define PROGRAMME_IMPLEMENTATION
#include "Kada/programme.h"

#define FLAG_IMPLEMENTATION
#include "Kada/lib/c/flag.h"

void add_linker_flags(command_t *command)
{
    command_append(command, " -ldflags=\"-s -w\"");
}

void add_tags(command_t *command, bool release)
{
    if (!release) return;
    command_append(command, " -tags release");
}

void build_command(command_t *command, const char *compiler, const char *target_folder, const char *target_name, const char *command_folder, bool release)
{
    command_append(command, compiler);
    command_append(command, " build");
    add_linker_flags(command);
    add_tags(command, release);
    command_appendf(command, " -o %s/%s.exe %s/%s.go", target_folder, target_name, command_folder, target_name);
}

int main(int argc, char **argv)
{
    char **target_name = flag_string("name", "main", "Set the name of the programme to compile.");
    bool *release = flag_bool("release", false, "Compile the programme with release tags.");
    flag_parse(argc, argv);
    const char *compiler = "go";
    const char *name = "compile";
    const char *command_folder = "cmd";
    const char *target_folder = "bin";
    logger_t *logger = logger_init(name, LOG_DEBUG);
    logger_add_console(logger);
    command_t compile = command_init();
    build_command(&compile, compiler, target_folder, *target_name, command_folder, *release);
    programme_t programme = programme_init_with_logger(logger);
    programme_append(&programme, &compile);
    if (!programme_run(&programme))
    {
        size_t checkpoint = buffer_save();
        logger_log(logger, buffer_sprintf("RuntimeError: Can not run command: '%s'.", command_data(&compile)), LOG_CRITICAL);
        logger_delete(logger);
        command_delete(&compile);
        buffer_rewind(checkpoint);
        return 1;
    }
    logger_delete(logger);
    command_delete(&compile);
    return 0;
}