#ifndef PROGRAMME_HPP_
#define PROGRAMME_HPP_

#include <project.hpp> // meta::project_t
#include <license.hpp> // meta::license_t
#include <author.hpp> // meta::author_t

#include <span> // std::span
#include <string> // std::string
#include <vector> // std::vector
#include <optional> // std::optional
#include <unordered_map> // std::unordered_map

#include <rfl/Ref.hpp> // rfl::Ref

namespace meta
{
    /**
     * @brief Meta information about a language-agnostic programme.
     */
    struct programme_t
    {
        /**
         * @brief Information about a project within a programme.
         */
        const rfl::Ref<project_t> project;

        /**
         * @brief Optional license value of the programme.
         */
        std::optional<const rfl::Ref<license_t>> license;

        /**
         * @brief Optional tags of a programme.
         */
        std::optional<const rfl::Ref<std::vector<std::string>>> tags;

        /**
         * @brief Optional contributors to a programme.
         */
        std::optional<const rfl::Ref<std::vector<author_t>>> contributors;

        /**
         * @brief Optional path values of a programme.
         */
        std::optional<const rfl::Ref<std::unordered_map<std::string, std::string>>> paths;

        /**
         * @brief Optional url values of a programme.
         */
        std::optional<const rfl::Ref<std::unordered_map<std::string, std::string>>> urls;

        /**
         * @brief Optional command values of a programme.
         */
        std::optional<const rfl::Ref<std::unordered_map<std::string, std::vector<std::string>>>> commands;

        /**
         * @brief Obtain a command of a programme based on its given type.
         * @param intent Intent to search for the stored command within the programme.
         * @returns A command based on the given intent.
         * @exception If no commands have been defined for the programme, a `ValueError` is throw.
         * @exception If the given intent does not have a string representation, a `ValueError` is throw.
         * @exception If the given intent can not be reflected, a `ValueError` is throw.
         * @exception If the given intent has not been defined within the programme, a `ValueError` is throw.
         */
        command_t command(command_t::type_t intent) const;

        /**
         * @brief Obtain the available commands stored within the programme.
         * @returns A vector of non-owning string representations of the commands stored within the programme.
         * @returns If no commands have been defined for the programme, an empty vector of strings is returned.
         */
        std::vector<std::string> available_commands(void) const noexcept;
    };

    /**
     * @brief Obtain a vector of command types based on the positional arguments passed into the programme.
     * @param argc Count of the runtime arguments passed to the programme.
     * @param argv Vector of runtime arguments passed to the programme.
     * @returns A vector of command types based on the given runtime argument vector and count.
     */
    std::vector<command_t::type_t> intent_from_arguments(int argc, const char **argv);

    /**
     * @brief Obtain the intents of command based on the given intents.
     * @param values Non-owning span of intent values passed to the programme.
     * @returns A vector of string representation of each of the given command type intents.
     * @returns If the given span of intent values is empty, an empty vector of strings is returned.
     */
    std::vector<std::string> intents(std::span<command_t::type_t> values);
}
#endif // PROGRAMME_HPP_