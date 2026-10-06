#define PROGRAMME_IMPLEMENTATION
#include "Kada/programme.h"

#define FLAG_IMPLEMENTATION
#include "Kada/lib/c/flag.h"

#define FILES_IMPLEMENTATION
#include "Kada/lib/c/fayl/files.h"

#define ENTRY_IMPLEMENTATION
#include "Kada/lib/c/fayl/entry.h"

void add_tags(command_t *command, bool release)
{
    if (!release) return;
    command_append(command, " -O3");
}

void add_warnings(command_t *command)
{
    command_append(command, " -Wall -Wextra");
}

void add_standard(command_t *command)
{
    command_append(command, " -std=c++20 -pedantic");
}

void add_debug(command_t *command)
{
    command_append(command, " -ggdb");
}

void add_linking(command_t *command, const char *include_folder, const char *source_folder)
{
    command_appendf(command, " -I%s -L%s", include_folder, source_folder);
}

void add_reflect(command_t *command, const char *includes_folder)
{
    files_t files = files_init(includes_folder);
    if (!files_fill(&files))
    {
        fprintf(stderr, "IOError: Can not read folder: %s\n", includes_folder);
        files_delete(&files);
        exit(1);
    }
    for (size_t i = 0; i < files.size; ++i)
    {
        const char *file = files.files[i];
        entry_t entry = entry_init(path_from(file));
        string_builder_delete(&entry.content);
        if (entry.type != DIRECTORY_ENTRY_TYPE) continue;
        command_appendf(command, " -I%s", files.files[i]);
    }
    files_delete(&files);
}

void add_source(command_t *command, const char *source_folder)
{
    files_t files = files_init(source_folder);
    if (!files_fill(&files))
    {
        fprintf(stderr, "IOError: Can not read folder: %s\n", source_folder);
        files_delete(&files);
        exit(1);
    }
    for (size_t i = 0; i < files.size; ++i)
    {
        const char *file = files.files[i];
        entry_t entry = entry_init(path_from(file));
        string_builder_delete(&entry.content);
        if (entry.type != FILE_ENTRY_TYPE) continue;
        command_appendf(command, " %s", files.files[i]);
    }
    files_delete(&files);
}

void build_command(command_t *command, const char *compiler, const char *target_folder, const char *target_name, const char *command_folder, const char *includes_folder, const char *source_folder, bool release)
{
    command_append(command, compiler);
    if (!release) add_warnings(command);
    add_standard(command);
    if (!release) add_debug(command);
    add_linking(command, includes_folder, source_folder);
    add_reflect(command, includes_folder);
    add_tags(command, release);
    command_appendf(command, " -o %s/%s.exe %s/%s.cpp", target_folder, target_name, command_folder, target_name);
}

int main(int argc, char **argv)
{
    const char **target_name = flag_string("name", "main", "Set the name of the programme to compile.");
    bool *release = flag_bool("release", false, "Compile the programme with release tags.");
    flag_parse(argc, argv);
    const char *compiler = "g++";
    const char *name = "compile";
    const char *command_folder = "cmd";
    const char *target_folder = "bin";
    const char *includes_folder = "includes";
    const char *source_folder = "source";
    // const char *toml_source = "thirdparty/reflect/rfl/internal";
    logger_t *logger = logger_init(name, LOG_DEBUG);
    logger_add_console(logger);
    command_t compile = command_init();
    build_command(&compile, compiler, target_folder, *target_name, command_folder, includes_folder, source_folder, *release);
    // add_source(&compile, toml_source);
    // add_source(&compile, "thirdparty/toml++");
    add_source(&compile, source_folder);
    // add_source(&compile, "thirdparty/reflect/rfl/json");
    // add_source(&compile, "thirdparty/reflect/rfl/parsing");
    process_t process = command_run_async_logged(&compile, logger);
    if (!process_wait(process))
    {
        size_t checkpoint = buffer_save();
        logger_log(logger, buffer_sprintf("RuntimeError: Can not run command: '%s'.", command_data(&compile)), LOG_CRITICAL);
        if (!process_close(process)) fprintf(stderr,  "ValueError: Can not close the current process.\n");
        logger_delete(&logger);
        command_delete(&compile);
        buffer_rewind(checkpoint);
        return 1;
    }
    if (!process_close(process)) fprintf(stderr,  "ValueError: Can not close the current process.\n");
    logger_delete(&logger);
    command_delete(&compile);
    return 0;
}