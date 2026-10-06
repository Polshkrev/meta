#ifndef AUTHOR_HPP_
#define AUTHOR_HPP_

#include <string> // std::string
#include <optional> // std::optional

#include <rfl/Ref.hpp> // rfl::Ref

namespace meta
{
    /**
     * @brief Representation of an author of a programme.
     */
    struct author_t
    {
        /**
         * @brief Name of the author.
         */
        const rfl::Ref<std::string> name;

        /**
         * @brief Optional email value of the author.
         */
        std::optional<const rfl::Ref<std::string>> email;
    };
}

#endif // AUTHOR_HPP_