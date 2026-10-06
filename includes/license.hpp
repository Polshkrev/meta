#ifndef LICENSE_HPP_
#define LICENSE_HPP_

#include <string> // std::string

#include <rfl/Ref.hpp> // rfl::Ref

namespace meta
{
    /**
     * @brief Representation of a programme license.
     */
    struct license_t
    {
        enum class type_t
        {
            MIT,
            GNUGPLv3,
            ALv2,
            BSL1,
            BSD2,
            CC0,
            EPL2,
            GAGPLv3,
            GLGPLV2,
            MBSD,
            MPL2,
            UNLICENSE
        };

        /**
         * @brief Type of the license.
         */
        const rfl::Ref<type_t> type;

        /**
         * @brief Path where the license file is located.
         */
        const rfl::Ref<std::string> path;

        /**
         * @brief Read the license file.
         * @returns The contents of the license file as a string.
         * @exception If the license file can not be read, `std::runtime_error` is throw.
         */
        std::string read(void) const;
    };
}

#endif // LICENSE_HPP_