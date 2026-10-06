#ifndef PROJECT_HPP_
#define PROJECT_HPP_

#include <string> // std::string
#include <optional> // std::optional

#include <author.hpp> // meta::author_t
#include <command.hpp> // meta::command_t
#include "version.hpp" // meta::version_t

#include <rfl/Ref.hpp> // rfl::Ref

namespace meta
{
    /**
     * @brief The meta details of a language-agnostic serializable project.
     */
    struct project_t
    {
        /**
         * @brief Finite list of project types.
         */
        enum class type_t
        {
            Api,
            Bot,
            Documentation,
            Game,
            Library,
            List,
            Application,
        };

        /**
         * @brief Name of the project.
         */
        const rfl::Ref<std::string> name;

        /**
         * @brief Description of the project.
         */
        const rfl::Ref<std::string> description;

        /**
         * @brief Author of the project.
         */
        const rfl::Ref<author_t> author;

        /**
         * @brief Type of the project.
         */
        const rfl::Ref<type_t> type;

        /**
         * @brief Optional version information of the project.
         */
        std::optional<const rfl::Ref<version_t>> version;
    };
}

#endif // PROJECT_HPP_