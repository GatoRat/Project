// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>

namespace fs = std::filesystem;

/** Rename backup files.
    @param pathname Full pathname of the backup file.
    @param maxBackups The maximum number of backups.
    @remarks The current files are renamed in the form:
        \<parent pathname\>.\<number\>[.\<ext\>] with '0' filled to the size
        of the maximum digits and with with the lowest number being the most recent
        file. (The current file will be assigned 1 for the number.)

        For example, if pathname is "C:/Temp/Config.json", after the first call to
        this function with maxBackups set to 99, the result would be:
        "C:/Temp/Config.01.json".
 */
void RenameBackupFiles(const fs::path& pathname, uint16_t maxBackups = 9);
