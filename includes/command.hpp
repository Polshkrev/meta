#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <string> // std::string
#include <vector> // std::vector
#include <optional> // std::optional
#include <string_view> // std::string_view

#include "../build/Kada/process.h" // process_t

namespace meta
{
    /**
     * @brief Representation of a system command.
     */
    struct command_t
    {
        /**
         * @brief Finite list of acceptable command types.
         */
        enum class type_t
        {
            NONE,
            BUILD,
            RUN,
            TEST,
            SERVE,
            DOCS,
            DIST,
            HELP,
            SCRIPT
        };

        /**
         * @brief Construct a command with an empty command-line and a type of `NONE`.
         */
        command_t(void) noexcept;

        /**
         * @brief Construct a command with a given type and full command-line.
         * @param type Initial type of the command.
         * @param arguments Full command-line of the command to run.
         */
        command_t(type_t type, const std::vector<std::string> &arguments) noexcept;

        /**
         * @brief Append a temporary string command-line.
         * @param item Temporary string representation of the command-line to append to the command.
         */
        void append(std::string &&item) noexcept;

        /**
         * @brief Append a non-owning string command-line.
         * @param item String view representation of the command-line to append to the command.
         */
        void append(std::string &item) noexcept;

        /**
         * @brief Extend a given view of items to the command.
         * @param items Representation of the command-line to append to the command.
         */
        void extend(std::vector<std::string> &items);

        /**
         * @brief Obtain a view to a value stored within the command at the given index.
         * @param index Index where the targeted value is located within the command.
         * @returns A view into the string stored within the command at the given index.
         * @returns If the command is empty, or there is no data stored at the index, a `std::nullopt` is returned.
         * @exception If the given index is outside of the bounds of the command, an `OutOfRange` error is thrown.
         */
        std::optional<std::string_view> at(std::size_t index) const noexcept;

        /**
         * @brief Obtain the size of the command.
         * @returns The size of the command.
         */
        std::size_t size(void) const noexcept;

        /**
         * @brief Obtain a string representation of the command.
         * @returns A c-string representation of the entire command.
         * @note At the moment, there is a dangling pointer due to the way a c-string is obtained.
         */
        const char *to_string(void) const noexcept;

        /**
         * @brief Obtain the first element or executable of the command.
         * @returns A view into the first element of the command.
         */
        const std::string &head(void) const noexcept;

        /**
         * @brief Obtain all the arguments of the command without the executable.
         * @returns An owned static-string representation of the arguments to run the command without the executable
         */
        std::string arguments(void) const noexcept;

        /**
         * @brief Run the command asynchronously.
         * @returns The process identifier of the executed command.
         */
        process_t run_async(void) const noexcept;

        /**
         * @brief Run the command synchronously.
         * @returns True if the command was able to be executed, else false.
         */
        bool run(void) const noexcept;
        private:
            /**
             * @brief Type of the command.
             */
            type_t __type;

            /**
             * @brief Command line of the command.
             */
            std::vector<std::string> __items;
    };
}


#endif // COMMAND_HPP_