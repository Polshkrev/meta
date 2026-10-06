#include <meta.hpp>

#include <algorithm> // std::find

#include <rfl/toml.hpp> // rfl::toml::load

namespace
{
    /**
     * @brief Determine if any of the given needle values are contained within the haystack.
     * @returns True if any of the given needle values are contained within the haystack, else false.
     */
    bool __contains_intent(std::span<std::string> needle, std::span<std::string> haystack)
    {
        for (std::size_t i = 0; i < needle.size(); ++i)
        {
            if (std::find(haystack.begin(), haystack.end(), needle[i]) == haystack.end()) return false;
        }
        return true;
    }

    /**
     * @brief Flatten a given span of intents to a given result parameter.
     * @param programme View into a programme with commands to run.
     * @param intents Span of intents to flatten.
     * @param result Out parameter vector to populate with the programme commands' process handles.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's intent has not been defined within the programme, a `ValueError` is thrown.
     */
    void __gather_intents_async(const meta::programme_t &programme, std::span<meta::command_t::type_t> intents, std::vector<process_t> &result)
    {
        for (meta::command_t::type_t intent : intents)
        {
            meta::command_t command = programme.command(intent);
            process_t process = command.run_async();
            result.push_back(process);
        }
    }

    /**
     * @brief Flatten a given span of intents.
     * @param programme View into a programme with commands to run.
     * @param intents Span of intents to flatten.
     * @returns True if any of the programme's commands complete successfully, else false.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's command intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's command intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's command intent has not been defined within the programme, a `ValueError` is thrown.
     */
    bool __gather_intents(const meta::programme_t &programme, std::span<meta::command_t::type_t> intents)
    {
        for (meta::command_t::type_t intent : intents)
        {
            meta::command_t command = programme.command(intent);
            process_t process = command.run_async();
            if (!process_wait(process)) return false;
        }
        return true;
    }

    /**
     * @brief Flatten the given programme's `script` command line into a result vector.
     * @param programme View into a programme with commands to run.
     * @param result Out parameter vector to populate with the programme commands' process handles.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's command intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's command intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's command intent has not been defined within the programme, a `ValueError` is thrown.
     */
    void __handle_script_async(const meta::programme_t &programme, std::vector<process_t> &result)
    {
        
        meta::command_t command = programme.command(meta::command_t::type_t::SCRIPT);
        process_t process = command.run_async();
        result.push_back(process);
    }

    /**
     * @brief Flatten the given programme's `script` command line.
     * @param programme View into a programme with commands to run.
     * @returns True if any of the programme's commands complete successfully, else false.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's command intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's command intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's command intent has not been defined within the programme, a `ValueError` is thrown.
     */
    bool __handle_script(const meta::programme_t &programme)
    {
        meta::command_t command = programme.command(meta::command_t::type_t::SCRIPT);
        return process_wait(command.run_async());
    }

    /**
     * @brief Validate the given intent vector against the given programme's command intents.
     * @param programme Programme to which against the given intents vector.
     * @param intents Vector of intents to check against the programme's intents.
     * @exception If the given programme's commands are empty, a `std::runtime_error` is thrown.
     * @exception If an invalid intent is within the given intents vector, a `std::runtime_error` is thrown.
     */
    void __validate_intents(const meta::programme_t &programme, std::span<meta::command_t::type_t> intents)
    {
        std::vector<std::string> commands = programme.available_commands();

        const std::string &name = *programme.project->name;

        if (intents.empty()) return;
        else if (commands.empty())
        {
            throw std::runtime_error("No commands have been defined for '" + name + "'.");
        }

        std::vector<std::string> intent_values = meta::intents(intents);

        if (__contains_intent(intent_values, commands)) return;

        throw std::runtime_error("An unknown intent has been passed to '" + name + "'.\nAvailable Commands: " + meta::available_commands(commands));
    }

    /**
     * @brief Run the given vector of intents asynchronously.
     * @param programme Programme whose commands run asynchronously.
     * @param intents Runtime intents passed into the programme.
     * @returns A vector of process handles from the ran commands.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's command intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's command intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's command intent has not been defined within the programme, a `ValueError` is thrown.
     */
    std::vector<process_t> __run_intents_async(const meta::programme_t &programme, std::span<meta::command_t::type_t> intents)
    {
        std::vector<process_t> results = std::vector<process_t>();
        __validate_intents(programme, intents);
        if (intents.empty())
        {
            __handle_script_async(programme, results);
            results.shrink_to_fit();
            return results;
        }
        results.reserve(intents.size());
        __gather_intents_async(programme, intents, results);
        results.shrink_to_fit();
        return results;
    }

    /**
     * @brief Run the given vector of intents.
     * @param programme Programme whose commands run asynchronously.
     * @param intents Runtime intents passed into the programme.
     * @returns True if any of the programme's commands complete successfully, else false.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's command intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's command intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's command intent has not been defined within the programme, a `ValueError` is thrown.
     */
    bool __run_intents(const meta::programme_t &programme, std::span<meta::command_t::type_t> intents)
    {
        __validate_intents(programme, intents);
        if (intents.empty())
        {
            return __handle_script(programme);
        }
        return __gather_intents(programme, intents);
    }

    /**
     * @brief Run the programme's commands asynchronously.
     * @param programme Programme whose commands run asynchronously.
     * @param argc Count of the runtime arguments passed into the programme.
     * @param argv Vector of runtime arguments passed to the programme.
     * @returns True if any of the programme's commands complete successfully, else false.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's command intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's command intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's command intent has not been defined within the programme, a `ValueError` is thrown.
     */
    bool _run_async(const meta::programme_t &programme, int argc, const char **argv)
    {
        std::vector<meta::command_t::type_t> intents = meta::intent_from_arguments(argc, argv);
        try
        {
            std::vector<process_t> results = __run_intents_async(programme, intents);
            for (process_t result : results)
            {
                if (!process_wait(result)) return false;
            }
        }
        catch (const std::runtime_error &error)
        {
            std::fprintf(stderr, "RuntimeError: %s\n", error.what());
            return false;
        }
        return true;
    }

    /**
     * @brief Run the programme's commands.
     * @param programme Programme whose commands run synchronously.
     * @param argc Count of the runtime arguments passed into the programme.
     * @param argv Vector of runtime arguments passed to the programme.
     * @returns True if any of the programme's commands complete successfully, else false.
     * @exception If no commands have been defined for the programme, a `ValueError` is thrown.
     * @exception If the given programme's command intent does not have a string representation, a `ValueError` is thrown.
     * @exception If the given programme's command intent can not be reflected, a `ValueError` is thrown.
     * @exception If the given programme's command intent has not been defined within the programme, a `ValueError` is thrown.
     */
    bool _run(const meta::programme_t &programme, int argc, const char **argv)
    {
        std::vector<meta::command_t::type_t> intents = meta::intent_from_arguments(argc, argv);
        try
        {
            return __run_intents(programme, intents);
        }
        catch (const std::runtime_error &error)
        {
            std::fprintf(stderr, "RuntimeError: %s\n", error.what());
            return false;
        }
        return true;
    }

    /**
     * @brief Dispatch mapping to programmatically run a mapped `runner_t` function synchronously or asynchronously.
     */
    static const std::unordered_map<bool, meta::runner_t> __runner_mapping =
    {
        {true, _run_async},
        {false, _run}
    };
}

namespace meta
{
    /**
     * @brief Reflect a programme from a given filename.
     * @param filename Name of the file where the meta file is located.
     * @returns A programme loaded from the given filename.
     * @exception If the given filename can not be loaded, a `std::runtime_error` is thrown.
     */
    programme_t load_file(const std::string &filename)
    {
        rfl::Result<programme_t> file = rfl::toml::load<programme_t>(filename);
        try
        {
            return file.value();
        }
        catch(const std::exception &except)
        {
            throw std::runtime_error(except.what());
        }
    }

    /**
     * @brief Flatten a given vector of string commands.
     * @param commands Vector of string representations of the commands passed to the programme.
     * @returns A flattened string representation of the given string commands.
     */
    std::string available_commands(std::vector<std::string> commands)
    {
        if (commands.empty()) return "[]";
        std::stringstream buffer = std::stringstream();
        buffer << "[";
        for (std::size_t index = 0; index < commands.size(); ++index)
        {
            buffer << commands[index];
            if (index != commands.size() - 1)
            {
                buffer << ", ";
            }
        }
        buffer << "]";
        return buffer.str();
    }

    /**
     * @brief Run a given programme.
     * @param programme Programme to run.
     * @param async Sentinal value to run the programme asynchronously.
     * @param argc Count of the runtime arguments passed into the programme.
     * @param argv Vector of runtime arguments passed to the programme.
     * @returns The exit code of all the commands ran by the programme.
     */
    exit_code_t run(const meta::programme_t &programme, bool async, int argc, const char **argv)
    {
        if (!__runner_mapping.at(async)(programme, argc, argv)) return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }
}