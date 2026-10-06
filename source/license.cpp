#include <license.hpp>

#include <fstream> // std::ifstream
#include <sstream> // std::ostringstream

namespace
{
    /**
     * @brief Read a file of a given string path.
     * @param filename Name of the file to read.
     * @returns A static string representation of the contents of the given file.
     * @exception If the given file can not be read, `std::runtime_error` is throw.
     */
    static std::string __read_file(const std::string &filename)
    {
        std::ifstream input(filename);
        if (!input)
        {
            throw std::runtime_error("Could not open file: " + filename);
        }
        std::ostringstream output;
        output << input.rdbuf();
        return output.str();
    }
}

namespace meta
{
    /**
     * @brief Read the license file.
     * @returns The contents of the license file as a string.
     * @exception If the license file can not be read, `std::runtime_error` is throw.
     */
    std::string license_t::read(void) const
    {
        return __read_file(*path);
    }
}