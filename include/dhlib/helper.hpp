
#ifndef DH_HELPER_HPP__
#define DH_HELPER_HPP__

#include <iostream>
#include <sstream>

#ifdef __WIN32__
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif


namespace dhlib {


/// \brief ignore case compare functor
template<typename CharT>
struct ignore_case_equal
{
    bool operator()(CharT lhs, CharT rhs)
    {
        return tolower(lhs)==tolower(rhs);
    }
};


/// get file extension name
inline
std::string get_extension(std::string const& filename)
{
    return filename.substr(filename.find_last_of('.')+1);
}

inline
size_t get_file_size(std::string const& filename)
{
#ifdef __WIN32__
    WIN32_FILE_ATTRIBUTE_DATA fad;
    ::GetFileAttributesEx( filename.c_str(), ::GetFileExInfoStandard, &fad );
    return fad.nFileSizeLow ;
#else
    struct stat fs;
    if ( stat(filename.c_str(),&fs) == -1 )
    {
        return 0;
    }
    return fs.st_size;
#endif
}


} // namespace dhnet

#endif // DH_HELPER_HPP__
