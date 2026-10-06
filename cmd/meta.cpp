#include <iostream> // std::cout

#include <meta.hpp> // meta::programme_t, meta::load_file, meta::run 

#define FLAG_IMPLEMENTATION
#include "../build/Kada/lib/c/flag.h" // flag_bool, flag_parse, flag_arguments_count, flag_arguments

/**
 * @brief Handle the runtime flags passed into the programme.
 * @param programme Programme from which the license and version will be accessed.
 * @param license Sentinal flag to display the given programme's license.
 * @param version Sentinal flag to display the given programme's version.
 * @returns True if the function completes successfully, else false.
 */
bool handle_flags(const meta::programme_t &programme, bool license, bool version)
{
    if (!license && !version) return true;
    else if (license)
    {
        std::cout << (*programme.license)->read();
        return false;
    }
    else if (version)
    {
        std::cout << (*programme.project->version)->to_string() << "\n";
        return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    const std::string &filename = "meta.toml";
    meta::programme_t programme = meta::load_file(filename);
    bool *async = flag_bool("async", false, "Run the programme as async.");
    bool *license = flag_bool("l", false, "Read the license file.");
    bool *version = flag_bool("v", false, "Display the version of the programme.");
    flag_parse(argc, argv);
    if (!handle_flags(programme, *license, *version)) return EXIT_SUCCESS;
    return meta::run(programme, *async, flag_arguments_count(), flag_arguments());
}