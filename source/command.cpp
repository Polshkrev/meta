#define PROCESS_IMPLEMENTATION
#include <command.hpp>

#include <stdexcept> // invalid_argument

/**
 * @brief Flatten a given vector of strings into a single string.
 * @param elements Vector of strings of which to flatten.
 * @param delimeter Delimeter to seperate each joined string.
 * @param string Out parameter of the modified string memory.
 * @returns A reference to a flattened string of each string in the given vector seperated by the given delimeter.
 */
static std::string &__flatten(const std::vector<std::string> &elements, char delimeter, std::string &string)
{
    for (std::vector<std::string>::const_iterator iterator = elements.cbegin(); iterator != elements.cend(); ++iterator)
    {
        string += (*iterator);
        if (iterator + 1 != elements.end())
        {
            string += delimeter;
        }
    }
    return string;
}

/**
 * @brief Flatten a given vector of strings into a single string.
 * @param elements Vector of strings of which to flatten.
 * @param delimeter Delimeter to seperate each joined string.
 * @returns A static-owned string of each string in the given vector seperated by the given delimeter.
 */
static std::string _flatten(const std::vector<std::string> &elements, char delimeter)
{
    std::string string = std::string();
    return __flatten(elements, delimeter, string);
}

/**
 * @brief Obtain a checked handle from a DWORD.
 * @param handle Handle to check.
 * @param command Command to assign.
 * @returns A checked handle.
 * @exception If the given handle is invalid, an `InvalidArgument` error is thrown.
 */
static HANDLE _check_handle(DWORD handle)
{
    HANDLE result = GetStdHandle(handle);
    if (INVALID_HANDLE_VALUE == result || nullptr == result) throw std::invalid_argument("The handle to the process is invalid.");
    return result;
}

/**
 * @brief Assing the start info to the given info parameter.
 * @param info Info to which to assign.
 */
static void __assign_start_info(STARTUPINFO &info)
{
    info.cb = sizeof(STARTUPINFO);
    info.hStdOutput = _check_handle(STD_OUTPUT_HANDLE);
    info.hStdInput = _check_handle(STD_INPUT_HANDLE);
    info.hStdError = _check_handle(STD_ERROR_HANDLE);
    info.dwFlags |= STARTF_USESTDHANDLES;
}

/**
 * @brief Run a given command asynchronusly.
 * @param command Command to run.
 * @returns The process identifier of the command.
 * @returns If the command can not be run, an `INVALID_PROCESS` is returned.
 */
process_t __command_run_async(const meta::command_t &command)
{
    if (command.size() == 0) return INVALID_PROCESS;
#ifdef _WIN32
    STARTUPINFO start_info;
    ZeroMemory(&start_info, sizeof(start_info));
    __assign_start_info(start_info);
    PROCESS_INFORMATION process_info;
    ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));
    BOOL success = CreateProcess(nullptr, const_cast<char *>(command.to_string()), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &start_info, &process_info);
    if (!success) return INVALID_PROCESS;
    else if (!process_close(process_info.hThread)) return INVALID_PROCESS;
    return process_info.hProcess;
#else
#error "NotImplementedError: The linux implementation of 'command_run_async' has not been implemented yet."
#endif // _WIN32
}

#include <sstream> // std::istringstream

namespace meta
{
    /**
     * @brief Construct a command with an empty command-line and a type of `NONE`.
     */
    command_t::command_t(void) noexcept : __type(command_t::type_t::NONE), __items(std::vector<std::string>()) {}

    /**
     * @brief Construct a command with a given type and full command-line.
     * @param type Initial type of the command.
     * @param arguments Full command-line of the command to run.
     */
    command_t::command_t(type_t type, const std::vector<std::string> &arguments) noexcept : __type(type), __items(arguments) {}

    /**
     * @brief Append a temporary string command-line.
     * @param item Temporary string representation of the command-line to append to the command.
     */
    void command_t::append(std::string &&item) noexcept
    {
        std::istringstream buffer(item);
        while (buffer >> item)
        {
           __items.push_back(std::move(item));
        }
    }

    /**
     * @brief Append a non-owning string command-line.
     * @param item String view representation of the command-line to append to the command.
     */
    void command_t::append(std::string &item) noexcept
    {
        append(std::move(item));
    }

    /**
     * @brief Extend a given view of items to the command.
     * @param items Representation of the command-line to append to the command.
     */
    void command_t::extend(std::vector<std::string> &items)
    {
        for (std::vector<std::string>::iterator iterator = items.begin(); iterator != items.end(); ++iterator)
        {
            append(*iterator);
        }
    }

    /**
     * @brief Obtain a view to a value stored within the command at the given index.
     * @param index Index where the targeted value is located within the command.
     * @returns A view into the string stored within the command at the given index.
     * @returns If the command is empty, or there is no data stored at the index, a `std::nullopt` is returned.
     * @exception If the given index is outside of the bounds of the command, an `OutOfRange` error is thrown.
     */
    std::optional<std::string_view> command_t::at(std::size_t index) const noexcept
    {
        if (size() == 0) return std::nullopt;
        return __items.at(index);
    }

    /**
     * @brief Obtain the size of the command.
     * @returns The size of the command.
     */
    std::size_t command_t::size(void) const noexcept
    {
        return __items.size();
    }

    /**
     * @brief Obtain a string representation of the command.
     * @returns A c-string representation of the entire command.
     * @note At the moment, there is a dangling pointer due to the way a c-string is obtained.
     */
    const char *command_t::to_string(void) const noexcept
    {
        return _flatten(__items, ' ').c_str();
    }

    /**
     * @brief Obtain the first element or executable of the command.
     * @returns A view into the first element of the command.
     */
    const std::string &command_t::head(void) const noexcept
    {
        return __items.at(0);
    }

    /**
     * @brief Obtain all the arguments of the command without the executable.
     * @returns An owned static-string representation of the arguments to run the command without the executable
     */
    std::string command_t::arguments(void) const noexcept
    {
        if (__items.size() <= 1) return std::string();
        return _flatten(std::vector<std::string>(__items.begin() + 1, __items.end()), ' ');
    }

    /**
     * @brief Run the command asynchronously.
     * @returns The process identifier of the executed command.
     */
    process_t command_t::run_async(void) const noexcept
    {
        return __command_run_async(*this);
    }

    /**
     * @brief Run the command synchronously.
     * @returns True if the command was able to be executed, else false.
     */
    bool command_t::run(void) const noexcept
    {
        process_t process = run_async();
        if (!process_wait(process)) return false;
        return process_close(process);
    }
}