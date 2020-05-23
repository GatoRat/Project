// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include "TestHelper.TmpDir.h"

namespace fs = std::filesystem;

namespace TestHelper
{
    /** Creates a temporary file on construction and deletes it upon destruction.
     */
    class TmpFile
    {
    public:
        /** TmpFile constructor which creates the file using a passed fstream.
           @param parent The parent TmpDir.
           @param pFileName The name of the file.
           @param file The file stream to use to open the file. close() will \b not
                be called on this stream. However, the stream must be closed before
                this instance is destructed, else the file will \b not be deleted.
           @param mode The mode for opening the file.
           @remarks For simplicity, this class does no reference counting.
                This instance should be destroyed \i before parent, otherwise
                the resulting behavior is undefined.
           @throws std::invalid_argument If \p pFileName is NULL or empty.
         */
        TmpFile(const TmpDir& parent, const char* pFileName, std::fstream* pFile = nullptr,
            std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);

        /** TmpFile constructor which creates the file and optionally writes its contents.
           @param parent The parent TmpDir.
           @param pFileName The name of the file.
           @param pData The data to write. If NULL, nothing will be written.
           @param dataLen The length of the data to write. If zero, nothing will be written.
           @remarks For simplicity, this class does no reference counting.
                This instance should be destroyed \i before parent, otherwise
                the resulting behavior is undefined.
           @throws std::invalid_argument If \p pFileName is NULL or empty.
           @throws std::ofstream::failure If create and/or write fails.
         */
        TmpFile(const TmpDir& parent, const char* pFileName, const char* pData, std::streamsize dataLen);

        /** TmpDir copy constructor (deleted).
         */
        TmpFile(const TmpFile&) = delete;

        /** TmpDir destructor.
           @remarks ALL contents of the directory will be removed. If any files
                or directories are still open this may fail without notice. If
                the directory has already been removed, nothing will happen.
         */
        virtual ~TmpFile();

        /** Removes the file and clears the pathname.
            This is useful when doing testing of invalid paths.
           @remarks If the file is still open this may fail without notice. If
                the file has already been removed, nothing will happen.
         */
        virtual void remove();

        /** TmpFile assignment operator (deleted).
         */
        void operator=(const TmpFile&) = delete;

        /** Returns the full path name of the file.
           @returns The full path name of the file.
         */
        const fs::path& pathname() const
        {
            return _pathname;
        }

    private:
        /** Creates the file.
           @param parent The parent TmpDir.
           @param pFileName The name of the file.
           @param pFile A pointer to the optional fstream to use to open the file.
           @param pFile The mode for opening the file.
           @remarks For simplicity, this class does no reference counting.
                This instance should be destroyed \i before parent, otherwise
                the resulting behavior is undefined.
           @throws std::invalid_argument If \p pFileName is NULL or empty.
        */
        void create(const TmpDir& parent, const char* pFileName, std::fstream* pFile, std::ios_base::openmode mode);

    private:
        fs::path _pathname;
    };
}
