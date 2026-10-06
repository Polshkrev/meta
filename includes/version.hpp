#ifndef VERSION_HPP
#define VERSION_HPP

#include <string> // std::string
#include <istream> // std::istream
#include <ostream> // std::ostream
#include <optional> // std::optional
#include <cstdint> // std::uint8_t, std::uint16_t, UINT8_MAX

#include <rfl/Ref.hpp> // rfl::Ref

namespace polutils
{
    /**
     * @brief Representation of a semantic-versioning object.
     */
    struct version_t
    {
        /**
         * @brief Release the version object.
         * @exception If the major value has reached the maximum `uint8_t`, a `ValueError` is thrown.
         */
        void release(void);

        /**
         * @brief Update the version object.
         * @exception If the minor value has reached the maximum `uint8_t`, a `ValueError` is thrown.
         */
        void update(void);

        /**
         * @brief Patch the version object.
         * @exception If the patch value has reached the maximum `uint8_t`, a `ValueError` is thrown.
         */
        void fix(void);

        /**
         * @brief Publish a new version. Sets the given version's major release number to one.
         * @exception If the given version object is evaluated to already be published, a `ValueError` is thrown.
         */
        void publish(void);

        /**
         * @brief Compare the major release of a given version object.
         * @param major Major release number to compare.
         * @returns False if the given version object's major version is not greater than or equal to the given major parametre, else true.
         */
        bool compare_major(std::uint8_t major) const noexcept;

        /**
         * @brief Compare the minor release of a given version object.
         * @param minor Minor release number to compare.
         * @returns False if the given version object's minor version is not greater than or equal to the given minor parametre, else true.
         */
        bool compare_minor(std::uint8_t minor) const noexcept;

        /**
         * @brief Compare the patch release of a given version object.
         * @param patch Patch release number to compare.
         * @returns False if the given version object's patch version is not greater than or equal to the given patch parametre, else true.
         */
        bool compare_patch(std::uint8_t patch) const noexcept;

        /**
         * @brief Compare one given version object to another.
         * @param version Version object to compare.
         * @returns False if all of one given version object's properties are not greater than or equal to each other, else true.
         */
        bool compare(const version_t &version) const noexcept;

        /**
         * @brief Determine if the version is public.
         * @returns True if the version's major value is greater than or equal to one, else false.
         */
        bool is_public(void) const noexcept;

        /**
         * @brief Obtain a string representation of the version object.
         * @returns A string representing a semantic version.
         */
        const char *to_string(void) const noexcept;

        protected:
            /**
             * @brief Obtain a string representation of the version object.
             * @returns A string representing a semantic version.
             */
            std::string _string(void) const noexcept;

        public:
            /**
             * @brief Major value of the version.
             */
            std::optional<std::uint8_t> major;

            /**
             * @brief Minor value of the version.
             */
            std::optional<std::uint8_t> minor;

            /**
             * @brief Patch value of the version.
             */
            std::optional<std::uint8_t> patch;

            /**
             * @brief Name value of the version.
             */
            std::optional<const rfl::Ref<std::string>> name;

            /**
             * @brief Description value of the version.
             */
            std::optional<const rfl::Ref<std::string>> description;
    };

    /**
     * @brief Operator `<<` overload.
     * @param stream Stream to push.
     * @param version Version to append to the stream.
     * @returns The given stream with the given version appended.
     */
    std::ostream &operator<<(std::ostream &stream, const version_t &version);

    /**
     * @brief Operator `>>` overload.
     * @param stream Stream from which to extract a version.
     * @param version Version object to populate.
     * @returns The given stream after extracting the version.
     */
    std::istream &operator>>(std::istream &stream, version_t &version);

    /**
     * @brief Determine if two versions are equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is equal to the right.
     */
    bool operator==(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are not equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is not equal to the right.
     */
    bool operator!=(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are less than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than the right.
     */
    bool operator<(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are less than or equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than or equal to the right.
     */
    bool operator<=(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are greater than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than the right.
     */
    bool operator>(const version_t &left, const version_t &right) noexcept;

    /**
     * @brief Determine if two versions are greater than or equal each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than or equal to the right.
     */
    bool operator>=(const version_t &left, const version_t &right) noexcept;

}

#endif // VERSION_HPP