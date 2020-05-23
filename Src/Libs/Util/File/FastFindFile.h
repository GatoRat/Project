// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <functional>
//#include <regex>
#include "../Detect/DetectOperatingSystem.h"

namespace fs = std::filesystem;

#ifdef _WIN32

#include "../System/_lean_windows_.h"

/** The find data
    \remarks For _WIN32, this is derived from WIN32_FIND_DATA. That definition is below:
    \code
    struct WIN32_FIND_DATA
    {
        DWORD    dwFileAttributes;
        FILETIME ftCreationTime;
        FILETIME ftLastAccessTime;
        FILETIME ftLastWriteTime;
        DWORD    nFileSizeHigh;
        DWORD    nFileSizeLow;
        DWORD    dwReserved0;
        DWORD    dwReserved1;
        TCHAR    cFileName[MAX_PATH];
        TCHAR    cAlternateFileName[16];
    };
    \endcode
    \remarks Do not use cAlternateFileName from the above. If is now generally empty.
    \remarks Do not use ftLastAccessTime since it is unreliable.
    \remarks The FILE_ATTRIBUTE_NORMAL attribute is unreliable.
*/
struct FastFindData : public WIN32_FIND_DATA
{
    /** Indicates whether the found filename is only dots (periods).
        \param  pStr A pointer to the string to check.
        \returns \b true if \p pStr is only dots, else \b false.
    */
    static bool IsDots(const wchar_t* pStr);

    /** Indicates whether the filename is only dots (periods).
        \returns \b true if the filename is only dots, else \b false.
    */
    bool isDots() const;

    /** Indicates whether the object is a file.
        \returns \b true if the object is a file, else \b false.
    */
    bool isFile() const;

    /** Indicates whether the found object is a directory.
        \returns \b true if the object is a directory, else \b false.
    */
    bool isDir() const;

    /** Indicates whether the object is hidden.
        \returns \b true if the object is hidden, else \b false.
    */
    bool isHidden() const;

    /** Indicates whether the object is read only.
        \returns \b true if the object is read only, else \b false.
    */
    bool isReadOnly() const;

    /** Indicates whether this is a system object.
        \returns \b true if this is a system object, else \b false.
    */
    bool isSystem() const;

    /** Indicates whether the object has device, system, hidden, temporary or offline
        attributes set.
        \returns \b true if the object has device, system, hidden, temporary or offline
            attributes set.
    */
    bool anyExtendedAttr() const;

    /** Returns the creation time of the object.
        \remarks The number of 100-nanosecond intervals since January 1, 1601 (UTC).
        \returns The creation time of the object.
    */
    uint64_t getCreationTime() const;

    /** Returns the last write time of the object.
        \remarks The number of 100-nanosecond intervals since January 1, 1601 (UTC).
        \returns The last write time of the object.
    */
    uint64_t getLastWriteTime() const;

    /** Returns the length of the object, in bytes.
        \returns The length of the object, in bytes.
    */
    uint64_t getLength() const;

    /** Returns the filename as a filesystem::path.
        \returns The filename as a filesystem::path.
    */
    fs::path getFilename() const;
};

/** Performs local file system searches.
    @remarks
        This class is a thin layer over the Win32 \b FindFirstFile and \FindNextFile
        functions. All features and limitations of those functions apply to this class.
    \par
       \b FastFindFile is derived from \b FastFindData which is, in turn derived
        from \b WIN32_FIND_DATA. When a search is successful, the resulting data may
        be accessed directly or through helper functions.
    \par
        This exists since it offers more control, returns more data and is 200 times
        faster than filesystem::directory_iterator when retrieving the additional
        information.
*/
class FastFindFile final : public FastFindData
{
protected:
    HANDLE _hFind = INVALID_HANDLE_VALUE;

public:
    FastFindFile() = default;
    FastFindFile(const FastFindFile&) = delete;
    void operator=(const FastFindFile&) = delete;
    ~FastFindFile();

    /** Closes the search handle. */
    void close() noexcept;

    /** Searches a directory for a file or subdirectory which matches a specified name.
        \param pMask A pointer to a null-terminated string that specifies a valid path and
            file name, that can contain wildcard characters (* and ?).
        \returns \b true if the search succeeds, else \b false.
        \bremarks
            The \b FindFirst method opens a search handle and returns information about the
            first found object which matches the specified pattern.
        \par
            After the search handle is created, use the \b findNext method to search for other
            objects that match the same pattern. When the search handle is no longer needed,
            close it by using the close member function.
        \par
            \b FastFindFile is derived from \b FastFindData which is, in turn derived
            from \b WIN32_FIND_DATA. When a search is successful, the resulting data may
            be accessed directly or through helper functions.
        \see findNext, ::FindFirstFileEx, ::FindNextFile
    */
    bool findFirst(const wchar_t* pMask, bool limitToDirs = false, bool useLargeFetch = true);

    /** Continues the file search from a previous successful call to \b findFirst.
        \returns \b true if the search succeeds, else \b false.
        \see findFirst, ::FindNextFile, ::FindFirstFileEx
    */
    bool findNext();

    /** Indicates whether the find handle is valid
        \returns \b true if the handle is valid, else \b false.
    */
    bool isValid() const;

public:
    enum Types
    {
        All,
        OnlyFiles,
        OnlyDirs,
    };

    /** Function to process the object which was found.
        \param basePath The directory in which the find was performed.
        \param findData The find data.
        \remarks DoCallback \b true KeepFinding, else \b false.
    */
    typedef std::function<bool(const fs::path& basePath, const FastFindData& findData)> FindCallback;

    static bool Find(const fs::path& basePath, const fs::path& mask, Types types, const FindCallback& findCallback);

    /** Performs a recursive search.
        \param pRegex The regular expression to use with files. This does not check
            directories. The regex expression equivalent of <tt>"*.ext"</tt> is
            <tt>"^.+\.ext$"</tt>. (In C++ code, remember to escape the backslash.)
        \param findCallback The function to call for each found file.
        \returns \b true if find operation ended naturally, else \b false.
    */
    //static bool FindRecursive(const fs::path& basePath, const std::wregex* pRegex, const FindCallback& findCallback);

    static bool FindRecursive(const fs::path& basePath, const FindCallback& findCallback);
};

#endif