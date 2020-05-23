// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

/// <summary>
/// Reads text from a file using the fastest method for C++.
/// </summary>
/// <param name="inStream">The stream to read.</param>
/// <returns>The text read. This may be empty.</returns>
/// <exception>May throw exceptions associated with streams.</exception>
std::string FastReadTextFileCpp(const std::ifstream& inStream);

/// <summary>
/// Reads text from a file using the fastest method for C++.
/// </summary>
/// <param name="pathname">The file to read.</param>
/// <returns>The text read. This may be empty.</returns>
/// <exception>May throw exceptions associated with streams.</exception>
std::string FastReadTextFileCpp(const fs::path& pathname);

/// <summary>
/// Writes a file using the fastest method for for C++.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="str">The string to write.</param>
/// <returns><b>true</b> if successful, else <b>false</b>.</returns>
/// <remarks>
/// This always tries to create a new file. If the file exists and is writable,
/// this will overwrite the existing file.
/// </remarks>
/// <exception>May throw exceptions associated with streams.</exception>
bool FastWriteTextFileCpp(const fs::path& pathname, std::string_view str);

/// <summary>
/// Reads text from a file using the fastest method for C.
/// </summary>
/// <param name="pathname">The file to read.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).
///     If less than zero, the actual file size will be used.</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <returns>The text read. This may be empty.</returns>
/// <remarks>
/// This uses the StdioFile class and opens the file with "rt".
/// </remarks>
/// <exception>May throw exceptions associated with StdioFile.</exception>
std::string FastReadTextFile(const fs::path& pathname, int64_t maxBytesToRead = -1, int64_t* pFileSize = nullptr);

/// <summary>
/// Reads text from a file using the fastest method for C.
/// </summary>
/// <param name="data">The string to receive the data.</param>
/// <param name="pathname">The file to read.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).
///     If less than zero, the actual file size will be used.</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <returns>The length of the text read. This may be zero.</returns>
/// <remarks>
/// This uses the StdioFile class and opens the file with "rt".
/// </remarks>
/// <exception>May throw exceptions associated with StdioFile.</exception>
size_t FastReadTextFile(std::string& data, const fs::path& pathname, int64_t maxBytesToRead = -1, int64_t* pFileSize = nullptr);

/// <summary>
/// Writes a file using the fastest method for for C.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="pStr">The string to write.</param>
/// <returns><b>true</b> if the write succeeded, else <b>false</b>.</returns>
/// <remarks>
/// This always tries to create a new file. If the file exists and is writable,
/// this will overwrite the existing file.
///
/// This uses the StdioFile class and opens the file with "wt".
/// </remarks>
/// <exception>May throw exceptions associated with StdioFile.</exception>
bool FastWriteTextFile(const fs::path& pathname, const char* pStr);

constexpr uint32_t FastReadAll   = UINT32_MAX; //-V2517
constexpr uint32_t FastReadNone  = 0;   // Verfies if file can be open for reading and optionally fills in size.
constexpr uint32_t FastWriteNone = 0;   // Creates a zero length file.

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="data">The vector to receive the data.</param>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="bytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add on first resize.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The length of the data read.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably should be
/// limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastReadFile(std::vector<uint8_t>& data, const fs::path& pathname, uint32_t bytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="data">The vector to receive the data.</param>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="bytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add when resizing the vector.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The length of the data read.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable and/or to obtain the file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastReadFile(std::vector<char>& data, const fs::path& pathname, uint32_t bytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add when resizing the vector.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The data read. This may be empty.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable and/or to obtain the file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
std::vector<uint8_t> FastReadFile(const fs::path& pathname, uint32_t maxBytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add when resizing the vector.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The data read. This may be empty.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable and/or to obtain the file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
std::vector<char> FastReadFile2(const fs::path& pathname, uint32_t maxBytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="fileMustExist">If file doesn't exist; if <b>true</b> an exception is
///     thrown, else data is emptied and zero is returned. In the latter case, if
///     pFileSize is valid, it will be set to -1.</param>
/// <param name="data">The vector to receive the data.</param>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add when resizing the string.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The length of the data read.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable and/or to obtain the file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
//uint32_t FastReadFileStr(bool fileMustExist, std::string& data, const fs::path& pathname, uint32_t maxBytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="data">The vector to receive the data.</param>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add when resizing the string.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The length of the data read.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable and/or to obtain the file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastReadFileStr(std::string& data, const fs::path& pathname, uint32_t maxBytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);
uint32_t FastReadFileStr_noExOnOpen(std::string& data, const fs::path& pathname, uint32_t maxBytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="fileMustExist">If file doesn't exist; if <b>true</b> an exception is
///     thrown, else an empty string is returned. In the latter case, if pFileSize is
///     valid, it will be set to -1.</param>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add when resizing the string.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The data read. This may be empty.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable and/or to obtain the file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
std::string FastReadFileStr(bool fileMustExist, const fs::path& pathname, uint32_t maxBytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="extraSize">Extra bytes to add when resizing the string.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The data read. This may be empty.</returns>
/// <remarks>
/// If <paramref name="bytesToRead"/> is zero, the file will be opened, the
/// optional <paramref name="pFileSize"/> obtained, and closed. This can be
/// used to verify that a file is readable and/or to obtain the file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
std::string FastReadFileStr(const fs::path& pathname, uint32_t maxBytesToRead = FastReadAll, int64_t* pFileSize = nullptr, uint32_t extraSize = 0, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="fileMustExist">If file doesn't exist; if <b>true</b> an exception is
///     thrown, else zero is returned. In the latter case, if pFileSize is valid,
///     it will be set to -1.</param>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="pBuffer">The buffer which will receive the data. May be nullptr.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The length of the data read.</returns>
/// <remarks>
/// If <paramref name="pBuffer"/> is nullptr or <paramref name="bytesToRead"/> is zero,
/// the file will be opened, the optional <paramref name="pFileSize"/> obtained, and
/// closed. This can be used to verify that a file is readable and/or to obtain the
/// file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastReadFile(bool fileMustExist, const fs::path& pathname, void* pBuffer, uint32_t maxBytesToRead, int64_t* pFileSize = nullptr, bool forceReadOnly = false);

/// <summary>
/// Reads a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="pBuffer">The buffer which will receive the data. May be nullptr.</param>
/// <param name="maxBytesToRead">The maximum bytes to read. May be zero (0).</param>
/// <param name="pFileSize">An optional pointer to receive the file size.</param>
/// <param name="forceReadOnly">If <b>true</b> and file fails to open with
///     with read share permissions, and attempt to open the file with read/write
///     share permissions will <b>not</b> be attempted.</param>
/// <returns>The length of the data read.</returns>
/// <remarks>
/// If <paramref name="pBuffer"/> is nullptr or <paramref name="bytesToRead"/> is zero,
/// the file will be opened, the optional <paramref name="pFileSize"/> obtained, and
/// closed. This can be used to verify that a file is readable and/or to obtain the
/// file size.
///
/// This function is intentionally limited to read UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastReadFile(const fs::path& pathname, void* pBuffer, uint32_t maxBytesToRead, int64_t* pFileSize = nullptr, bool forceReadOnly = false);

/// <summary>
/// Writes a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="pBuffer">The buffer which contains the data to write. May be nullptr.</param>
/// <param name="bytesToWrite">The number of bytes to write. May be zero (0).</param>
/// <returns>The length of the data written.</returns>
/// <remarks>
/// This always creates a new file. If the file exists and is writable, this will
/// overwrite the existing file.
///
/// If <paramref name="pBuffer"/> is nullptr or <paramref name="bytesToWrite"/>
/// is zero, the file will be created and then closed.
///
/// This function is intentionally limited to write UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastWriteFile(const fs::path& pathname, const void* pBuffer, uint32_t bytesToWrite);

/// <summary>
/// Writes a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="data">The data to write.</param>
/// <returns>The length of the data written.</returns>
/// <remarks>
/// This always creates a new file. If the file exists and is writable, this will
/// overwrite the existing file.
///
/// If <paramref name="data"/> is empty, the file will be created and then closed.
///
/// This function is intentionally limited to write UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastWriteFile(const fs::path& pathname, const std::vector<uint8_t>& data);

/// <summary>
/// Writes a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="str">The string to write.</param>
/// <returns>The length of the data written.</returns>
/// <remarks>
/// This always creates a new file. If the file exists and is writable, this will
/// overwrite the existing file.
///
/// If <paramref name="str"/> is empty, the file will be created and then closed.
///
/// This function is intentionally limited to write UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastWriteFile(const fs::path& pathname, std::string_view str);

/// <summary>
/// Writes a file using the fastest method for this operating system.
/// </summary>
/// <param name="pathname">The pathname of the file.</param>
/// <param name="pStr">The string to write.</param>
/// <param name="charsToWrite">The maximum chars to write. If greater than the actual
///     string length, the actual length will be written.</param>
/// <returns>The length of the data written.</returns>
/// <remarks>
/// This always creates a new file. If the file exists and is writable, this will
/// overwrite the existing file.
///
/// If <paramref name="pStr"/> is nullptr or <paramref name="charsToWrite"/>
/// is zero, the file will be created and then closed.
///
/// This function is intentionally limited to write UINT32_MAX bytes and arguably
/// should be limited to a value less than that.
/// </remarks>
/// <exception cref="XceptionFile">Thrown on any file operation error.</exception>
uint32_t FastWriteFileStr(const fs::path& pathname, const char* pStr, uint32_t charsToWrite = UINT32_MAX); //-V2517
