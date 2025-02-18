#ifndef _INC_MYFILE
#define _INC_MYFILE

#include <winsock2.h>

struct file {
    HFILE h;
};

#include "mytxt.h"
#include <io.h>

/**
 * @brief Open a file by name
 *
 * Opens a file by name with read/write and returns a pointer to the file structure.
 *
 * @param name The name of the file to open
 * @return A pointer to the file structure
 */
file *open(LPCSTR name);

/**
 * @brief Open a file by name with flags
 *
 * Opens a file by name with the specified flags and returns a pointer to the file structure.
 *
 * @param name The name of the file to open
 * @param flags The flags to use when opening the file
 * @return A pointer to the file structure
 */
file *open2(LPCSTR name, unsigned long flags);

/**
 * @brief Open a file by txt structure
 *
 * Opens a file by name specified in the txt structure and returns a pointer to the file structure.
 *
 * @param t The txt structure containing the name of the file to open
 * @return A pointer to the file structure
 */
file *open(txt *t);

/**
 * @brief Open a file by txt structure
 *
 * Opens a file by name specified in the txt structure and returns a pointer to the file structure.
 *
 * @param t The txt structure containing the name of the file to open
 * @return A pointer to the file structure
 */
file *open2(txt *t, unsigned long flags);

/**
 * @brief Get data from a file
 *
 * Reads a specified number of bytes from a file and stores them in the destination offset.
 *
 * @param filePtr A pointer to the file structure
 * @param dest The destination offset to store the data
 * @param bytesToRead The number of bytes to read
 */
void get(file *filePtr, void *dest, long bytesToRead);

/**
 * @brief Load data from a file into a destination buffer
 *
 * Loads data from a file into a destination buffer.
 *
 * @param name The name of the file to open
 * @param dest The destination buffer where the data will be read into
 * @param bytesToRead The number of bytes to read from the file
 */
void loadFromFile(LPCSTR name, void *dest, long bytesToRead);

/**
 * @brief Load data from a file into a destination buffer and return the file structure
 *
 * Loads data from a file into a destination buffer and returns the file structure.
 *
 * @param name The name of the file to open
 * @param dest The destination buffer where the data will be read into
 * @param bytesToRead The number of bytes to read from the file
 * @return A pointer to the file structure
 */
file *loadFromFileAndReturn(LPCSTR name, void *dest, long bytesToRead);

/**
 * @brief Loads data from a file into a destination buffer.
 *
 * Allocates memory for the destination buffer and loads data from a file into the buffer.
 *
 * @param name The name of the file to open.
 * @param dest The destination buffer where the data will be read into.
 * @param bytesToRead The number of bytes to read from the file.
 */
template <typename T>
 void loadFromFileMalloc(LPCSTR name, T *dest, long bytesToRead);

/**
 * @brief Seek to a position in a file
 *
 * Seeks to a specified position in a file.
 *
 * @param filePtr A pointer to the file structure
 * @param bytesToSeek The position in the file to reach
 */
void seek(file *filePtr, long bytesToSeek);

/**
 * @brief Close a file
 *
 * @param filePtr A pointer to the file structure
 */
void close(file *filePtr);

/**
 * @brief Write data into a file
 *
 * Writes a specified number of bytes from the source into a file.
 *
 * @param filePtr A pointer to the file structure
 * @param src The source offset to read the data from
 * @param bytesToWrite The number of bytes to write
 */
void put(file *filePtr, void *src, long bytesToWrite);

/**
 * @brief Seek to the beginning of a file
 *
 * @param filePtr A pointer to the file structure
 * @return The current position in the file
 */
long seek(file *filePtr);

/**
 * @brief Get the length of a file
 *
 * Gets the length of a file.
 *
 * @param filePtr A pointer to the file structure
 * @return The length of the file
 */
long fileLen(file *filePtr);

/**
 * @brief Get the length of a file
 *
 * Gets the length of a file. \n\n
 * <b>Deprecated</b> in favor of fileLen - will remove after testing.
 *
 * @param filePtr A pointer to the file structure
 * @return The length of the file
 */
long lof(file *filePtr);

/**
 * @brief Load a file into memory
 *
 * Loads a file into memory and returns a pointer to the data.
 *
 * @param name The name of the file to load
 * @return A pointer to the data
 */
void *loadfile(LPCSTR name);

/**
 * @brief Wait for a file to be available
 *
 * Waits for a file to be available.
 *
 * @param name The name of the file to wait for
 */
void waitforfile(LPCSTR name);

/**
 * @brief Delete a file
 *
 * Deletes a file.
 *
 * @param name The name of the file to delete
 */
void deletefile(LPCSTR name);

#endif
