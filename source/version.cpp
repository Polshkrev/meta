#include <version.hpp>

#include <sstream> // std::stringstream

#include <stdexcept>

namespace
{
    /**
     * @brief Generic print function for a version.
     * @param stream Stream to which to append a version string.
     * @param version Version to append to the given stream.
     * @param newline If true is passed in, a newline is appended.
     */
    void _version_print(std::ostream &stream, const polutils::version_t &version, bool newline) noexcept
    {
        if (version.name.has_value())
        {
            stream << (*version.name.value()) << ": ";
        }

        stream << static_cast<std::uint16_t>(version.major.value_or(0)) << "." << static_cast<std::uint16_t>(version.minor.value_or(0)) << "." << static_cast<std::uint16_t>(version.patch.value_or(0));

        if (version.description.has_value())
        {
            stream << " - " << *(version.description.value());
        }

        if (newline)
        {
            stream << "\n";
        }
    }
}

namespace polutils
{
    /**
     * @brief Release the version object.
     * @exception If the major value has reached the maximum `uint8_t`, a `ValueError` is thrown.
     */
    void version_t::release(void)
    {
        if (major == UINT8_MAX)
        {
            throw std::overflow_error("The major version has reached the maximum allowed by type.");
        }
        else if (!major.has_value())
        {
            throw std::runtime_error("No major value has been set for the version.");
        }
        (major.value()) = (major.value() + 1);
        minor = 0;
        patch = 0;
    }

    /**
     * @brief Update the version object.
     * @exception If the minor value has reached the maximum `uint8_t`, a `ValueError` is thrown.
     */
    void version_t::update(void)
    {
        if (minor == UINT8_MAX)
        {
            throw std::overflow_error("The minor version has reached the maximum allowed by type.");
        }
        else if (!minor.has_value())
        {
            throw std::runtime_error("No minor value has been set for the version.");
        }
        (minor.value()) = (minor.value() + 1);
        patch = 0;
    }

    /**
     * @brief Patch the version object.
     * @exception If the patch value has reached the maximum `uint8_t`, a `ValueError` is thrown.
     */
    void version_t::fix(void)
    {
        if (patch == UINT8_MAX)
        {
            throw std::overflow_error("The patch version has reached the maximum allowed by type.");
        }
        else if (!patch.has_value())
        {
            throw std::runtime_error("No patch value has been set for the version.");
        }
        (patch.value()) = (patch.value() + 1);
    }

    /**
     * @brief Publish a new version. Sets the given version's major release number to one.
     * @exception If the given version object is evaluated to already be published, a `ValueError` is thrown.
     */
    void version_t::publish(void)
    {
        if (is_public())
        {
            throw std::logic_error("Version is already public.");
        }
        major = 1;
        minor = 0;
        patch = 0;
    }

    /**
     * @brief Compare the major release of a given version object.
     * @param major Major release number to compare.
     * @returns False if the given version object's major version is not greater than or equal to the given major parametre, else true.
     */
    bool version_t::compare_major(std::uint8_t major) const noexcept
    {
        return this->major >= major;
    }

    /**
     * @brief Compare the minor release of a given version object.
     * @param minor Minor release number to compare.
     * @returns False if the given version object's minor version is not greater than or equal to the given minor parametre, else true.
     */
    bool version_t::compare_minor(std::uint8_t minor) const noexcept
    {
        return this->minor >= minor;
    }

    /**
     * @brief Compare the patch release of a given version object.
     * @param patch Patch release number to compare.
     * @returns False if the given version object's patch version is not greater than or equal to the given patch parametre, else true.
     */
    bool version_t::compare_patch(std::uint8_t patch) const noexcept
    {
        return this->patch >= patch;
    }

    /**
     * @brief Compare one given version object to another.
     * @param version Version object to compare.
     * @returns False if all of one given version object's properties are not greater than or equal to each other, else true.
     */
    bool version_t::compare(const version_t &version) const noexcept
    {
        return *this >= version;
    }

    /**
     * @brief Determine if the version is public.
     * @returns True if the version's major value is greater than or equal to one, else false.
     */
    bool version_t::is_public(void) const noexcept
    {
        return compare_major(1);
    }

    /**
     * @brief Obtain a string representation of the version object.
     * @returns A string representing a semantic version.
     */
    std::string version_t::_string(void) const noexcept
    {
        std::stringstream result = std::stringstream();
        _version_print(result, *this, false);
        return result.str();
    }

    /**
     * @brief Obtain a string representation of the version object.
     * @returns A string representing a semantic version.
     */
    const char *version_t::to_string(void) const noexcept
    {
        std::string string = _string();
        return string.c_str();
    }

    /**
     * @brief Operator `<<` overload.
     * @param stream Stream to push.
     * @param version Version to append to the stream.
     * @returns The given stream with the given version appended.
     */
    std::ostream &operator<<(std::ostream &stream, const version_t &version)
    {
        stream << version.to_string();
        return stream;
    }

    /**
     * @brief Operator `>>` overload.
     * @param stream Stream from which to extract a version.
     * @param version Version object to populate.
     * @returns The given stream after extracting the version.
     */
    std::istream &operator>>(std::istream &stream, version_t &version)
    {
        unsigned int major;
        unsigned int minor;
        unsigned int patch;
        char separator;

        if (!(stream >> major)) return stream;

        else if (!(stream >> separator) || separator != '.')
        {
            stream.setstate(std::ios::failbit);
            return stream;
        }

        else if (!(stream >> minor)) return stream;

        else if (!(stream >> separator) || separator != '.')
        {
            stream.setstate(std::ios::failbit);
            return stream;
        }
        else if (!(stream >> patch)) return stream;

        else if (major > UINT8_MAX || minor > UINT8_MAX || patch > UINT8_MAX)
        {
            stream.setstate(std::ios::failbit);
            return stream;
        }

        version.major = major;
        version.minor = minor;
        version.patch = patch;

        return stream;
    }

    /**
     * @brief Determine if two versions are equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is equal to the right.
     */
    bool operator==(const version_t &left, const version_t &right) noexcept
    {
        return left.major == right.major && left.minor == right.minor && left.patch == right.patch;
    }

    /**
     * @brief Determine if two versions are not equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is not equal to the right.
     */
    bool operator!=(const version_t &left, const version_t &right) noexcept
    {
        return !(left == right);
    }

    /**
     * @brief Determine if two versions are less than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than the right.
     */
    bool operator<(const version_t &left, const version_t &right) noexcept
    {
        if (left.major != right.major) return left.major < right.major;

        else if (left.minor != right.minor) return left.minor < right.minor;

        return left.patch < right.patch;
    }

    /**
     * @brief Determine if two versions are less than or equal to each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is less than or equal to the right.
     */
    bool operator<=(const version_t &left, const version_t &right) noexcept
    {
        if (left.major != right.major) return left.major <= right.major;

        else if (left.minor != right.minor) return left.minor <= right.minor;

        return left.patch <= right.patch;
    }

    /**
     * @brief Determine if two versions are greater than each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than the right.
     */
    bool operator>(const version_t &left, const version_t &right) noexcept
    {
        if (left.major != right.major) return left.major > right.major;

        else if (left.minor != right.minor) return left.minor > right.minor;

        return left.patch > right.patch;
    }

    /**
     * @brief Determine if two versions are greater than or equal each other.
     * @param left Operand to compare.
     * @param right Operand to compare.
     * @returns True if the given left version is greater than or equal to the right.
     */
    bool operator>=(const version_t &left, const version_t &right) noexcept
    {
        if (left.major != right.major) return left.major >= right.major;

        else if (left.minor != right.minor) return left.minor >= right.minor;

        return left.patch >= right.patch;
    }
}