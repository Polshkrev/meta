#include <programme.hpp>

#include <span> // std::span

#include <command.hpp> // command_t, command_t::type_t

#define EXCEPTIONS_IMPLEMENTATION
#include "..\build\Kada\lib\cpp\exceptions.hpp" // polutils::ValueError

namespace
{
    /**
     * @brief Mapping of string literals to their respective command types.
     */
    static const std::unordered_map<std::string, meta::command_t::type_t> __string_commands =
    {
        {"build", meta::command_t::type_t::BUILD},
        {"run", meta::command_t::type_t::RUN},
        {"test", meta::command_t::type_t::TEST},
        {"serve", meta::command_t::type_t::SERVE},
        {"docs", meta::command_t::type_t::DOCS},
        {"dist", meta::command_t::type_t::DIST},
        {"help", meta::command_t::type_t::HELP},
        {"script", meta::command_t::type_t::SCRIPT},
    };

    /**
     * @brief Mapping of command types to their respective string literals.
     */
    static const std::unordered_map<meta::command_t::type_t, std::string> __command_strings =
    {
        {meta::command_t::type_t::BUILD, "build"},
        {meta::command_t::type_t::RUN, "run"},
        {meta::command_t::type_t::TEST, "test"},
        {meta::command_t::type_t::SERVE, "serve"},
        {meta::command_t::type_t::DOCS, "docs"},
        {meta::command_t::type_t::DIST, "dist"},
        {meta::command_t::type_t::HELP, "help"},
        {meta::command_t::type_t::SCRIPT, "script"},
    };
}

namespace meta
{
    /**
     * @brief Obtain a command of a programme based on its given type.
     * @param intent Intent to search for the stored command within the programme.
     * @returns A command based on the given intent.
     * @exception If no commands have been defined for the programme, a `ValueError` is throw.
     * @exception If the given intent does not have a string representation, a `ValueError` is throw.
     * @exception If the given intent can not be reflected, a `ValueError` is throw.
     * @exception If the given intent has not been defined within the programme, a `ValueError` is throw.
     */
    command_t programme_t::command(command_t::type_t intent) const
    {
        if (!commands.has_value())
        {
            throw polutils::ValueError("No commands have been defined for the programme: %s.", project->name->c_str());
        }
        rfl::Ref<std::unordered_map<std::string, std::vector<std::string>>> value = commands.value();
        if (!__command_strings.contains(intent))
        {
            throw polutils::ValueError("The given intent is not mapped to a string representation.");
        }
        else if (!__string_commands.contains(__command_strings.at(intent)))
        {
            throw polutils::ValueError("The given intent is not defined.");
        }
        else if (!value->contains(__command_strings.at(intent)))
        {
            throw polutils::ValueError("'%s' is not defined for the programme: '%s'.", __command_strings.at(intent).c_str(), project->name->c_str());
        }
        return command_t(intent, value->at(__command_strings.at(intent)));
    }

    /**
     * @brief Obtain the available commands stored within the programme.
     * @returns A vector of string representations of the commands stored within the programme.
     * @returns If no commands have been defined for the programme, an empty vector of strings is returned.
     */
    std::vector<std::string> programme_t::available_commands(void) const noexcept
    {
        if (!commands) return std::vector<std::string>();
        std::vector<std::string> result = std::vector<std::string>();
        std::unordered_map<std::string, std::vector<std::string>> mapping = *(*commands);
        result.reserve(mapping.size());
        std::unordered_map<std::string, std::vector<std::string>>::const_iterator iterator;
        for (iterator = mapping.cbegin(); iterator != mapping.cend(); ++iterator)
        {
            result.push_back(iterator->first);
        }
        result.shrink_to_fit();
        return result;
    }

    /**
     * @brief Obtain the intents of command based on the given intents.
     * @param values Non-owning span of intent values passed to the programme.
     * @returns A vector of string representation of each of the given command type intents.
     * @returns If the given span of intent values is empty, an empty vector of strings is returned.
     */
    std::vector<std::string> intents(std::span<command_t::type_t> values)
    {
        if (values.empty()) return std::vector<std::string>();
        std::vector<std::string> result = std::vector<std::string>();
        result.reserve(values.size());
        for (const command_t::type_t &value : values)
        {
            result.push_back(__command_strings.at(value));
        }
        result.shrink_to_fit();
        return result;
    }

    /**
     * @brief Obtain a vector of command types based on the positional arguments passed into the programme.
     * @param argc Count of the runtime arguments passed to the programme.
     * @param argv Vector of runtime arguments passed to the programme.
     * @returns A vector of command types based on the given runtime argument vector and count.
     */
    std::vector<command_t::type_t> intent_from_arguments(int argc, const char **argv)
    {
        std::vector<command_t::type_t> result = std::vector<command_t::type_t>();
        result.reserve(argc);
        for (int index = 0; index < argc; ++index)
        {
            result.push_back(__string_commands.at(std::string(argv[index])));
        }
        result.shrink_to_fit();
        return result;
    }
}