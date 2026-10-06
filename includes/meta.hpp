#ifndef META_HPP_
#define META_HPP_

#include <cstdint> // std::uint8_t

#include <string> // std::string
#include <span> // std::span
#include <functional> // std::function

#include <programme.hpp> // programme_t

namespace meta
{
    /**
     * @brief Standardization of a function that can run a given programme with given runtime argument count and arguments.
     */
    using runner_t = std::function<bool(const meta::programme_t &programme, int argc, const char **argv)>;

    /**
     * @brief Standardization of an exit code for a command.
     */
    using exit_code_t = std::uint8_t;

    /**
     * @brief Reflect a programme from a given filename.
     * @param filename Name of the file where the meta file is located.
     * @returns A programme loaded from the given filename.
     * @exception If the given filename can not be loaded, a `std::runtime_error` is thrown.
     */
    programme_t load_file(const std::string &filename);

    /**
     * @brief Flatten a given vector of string commands.
     * @param commands Vector of string representations of the commands passed to the programme.
     * @returns A flattened string representation of the given string commands.
     */
    std::string available_commands(std::vector<std::string> commands);

    /**
     * @brief Run a given programme.
     * @param programme Programme to run.
     * @param async Sentinal value to run the programme asynchronously.
     * @param argc Count of the runtime arguments passed into the programme.
     * @param argv Vector of runtime arguments passed to the programme.
     * @returns The exit code of all the commands ran by the programme.
     */
    exit_code_t run(const programme_t &programme, bool async, int argc, const char **argv);
}

#endif // META_HPP_