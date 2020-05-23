// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

namespace TestHelper
{
    class BaseTmpDir;

    /** Creates a temporary directory on construction and deletes it, and all of
        its contents upon destruction.
     */
    class TmpDir
    {
        friend class BaseTmpDir;
    private:
        /** The full path of the directory. */
        fs::path _fullpath;

        /** Does the actual creation.
            @param parent The parent TmpDir.
            @param pDirName The name of the directory. If NULL or empty, a random
                name will be used with the prefix "~dir.".
            @throws std::exception If \p pDirName is invalid, contains path
                delimiters or path creation otherwise fails.
         */
        void create(const TmpDir& parent, const char* pDirName);

        /** TmpDir constructor used \b ONLY by BaseTmpDir.
            @param fullpath The name of the base temp directory.
         */
        explicit TmpDir(const fs::path& _fullpath) :
            _fullpath(_fullpath)
        {
        }

    public:
        /** TmpDir constructor.
            @param pDirName The name of the directory. If NULL or empty, a random
                name will be used with the prefix "~dir.". This directory will be
                created relative to the base temp directory. It will NOT create
                multiple directories.
            @remarks For simplicity, this class does no reference counting.
                This instance should be destroyed \i before parent, otherwise
                the resulting behavior is undefined.
            @throws std::exception If \p pDirName is invalid, contains path
                delimiters or path creation otherwise fails.
         */
        explicit TmpDir(const char* pDirName);

        /** TmpDir constructor taking a parent.
            @param parent The parent TmpDir.
            @param pDirName The name of the directory. If NULL or empty, a random
                name will be used with the prefix "~dir.".
            @remarks For simplicity, this class does no reference counting.
                This instance should be destroyed \i before parent, otherwise
                the resulting behavior is undefined.
            @throws std::runtime_error If /p pDirName is NULL, invalid or
                contains path delimiters.
         */
        TmpDir(const TmpDir& parent, const char* pDirName);

        /** TmpDir copy constructor (deleted).
         */
        TmpDir(const TmpDir&) = delete;

        /** TmpDir destructor.
            @remarks ALL contents of the directory will be removed. If any files
                or directories are still open this may fail without notice. If
                the directory has already been removed, nothing will happen.
         */
        virtual ~TmpDir();

        /** Removes the directory and clears the fullpath.
            This is useful when doing testing of invalid paths.
            @remarks ALL contents of the directory will be removed. If any files
                or directories are still open this may fail without notice. If
                the directory has already been removed, nothing will happen.
         */
        virtual void remove();

        /** TmpDir assignment operator (deleted).
         */
        void operator=(const TmpDir&) = delete;

        // Note: A fs::path operator was not included so it's clear in the test code
        //       what is being done.

        /** Returns the full path.
            @returns The full path.
         */
        const fs::path& fullpath() const
        {
            return _fullpath;
        }
    };

    const TmpDir& GetTmpDir();
}
