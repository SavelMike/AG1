#ifndef __FLIB_H_
#define __FLIB_H__

#ifdef __cplusplus
	#include <cstdio>
	#include <cstdlib>
	#include <cstdint>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdint.h>
#endif

/*!
 * File opening mode.
 */
typedef enum Mode {
	READ, WRITE
} FLIB_Mode;


/*!
 * Prepare inner structure for the file handling.
 *
 * (don't include call to this function in your
 *  submission, only use for your local testing)
 */
void flib_init_files ( uint16_t number_of_files );

/*!
 * Tear down inner structure for the file handling.
 *
 * (don't include call to this function in your
 *  submission, only use for your local testing)
 */
void flib_free_files ( );

/*!
 * Open file.
 *
 * Opens the file whose ID is specified in the parameter file_ID. The operations 
 * that are allowed on the stream and how these are performed are defined by the
 * mode parameter.
 * 
 * \param[in] file_ID Identification of the file to be opened.
 * \param[in] mode File access mode. It can be:
 * +-------------------------------------------------------+
 * | READ  | Open file for input operations. The file must | 
 * |       | exists, i.e., be previously opened in WRITE   |
 * |       | mode.                                         |
 * +-------------------------------------------------------+
 * | WRITE | Open it for output operations. If a file with |
 * |       | the same ID already exists its contents are   |
 * |       | discarded and the file is treated as a new    |
 * |       | empty file.                                   |
 * +-------------------------------------------------------+
 */
void flib_open ( uint16_t file_ID, FLIB_Mode mode );

/*!
 * Close file.
 *
 * Closes previously opened file specified in the parameter file_ID. All internal
 * pointers and structured are disassociated and freed.
 *
 * \param[in] file_ID Identification of the file to be closed.
 */
void flib_close ( uint16_t file_ID );

/*!
 * Read block of numbers from file.
 *
 * Reads an block of count numbers from the file specified in the parameter file_ID
 * and stores them in the block of memory specified by buffer.
 *
 * \param[in] file_ID Identification of the file to be read from.
 * \param[out] buffer Pointer to a block of memory with a size of at least 
 *                    (count*4) bytes.
 * \param[in] count Number of numbers to be read from the file.
 *
 * \return The total number of numbers successfully read. If this number differs
 * from the count parameter the end-of-file was reached while reading.
 */
int64_t flib_read ( uint16_t file_ID, int32_t * buffer, int64_t count );

/*!
 * Write block of data to file.
 *
 * Writes an block of count numbers from the block of memory pointed by buffer 
 * to the current position in the file specified in the parameter file_ID.
 *
 * \param[in] file_ID Identification of the file to be write into.
 * \param[in] buffer Pointer to a block of memory with a size of at least
 *                   (count * 4) bytes containing numbers to be written.
 * \param[in] count Number of numbers to be written.
 *
 * \return The total number of numbers sucessfully written.
 */
int64_t flib_write ( uint16_t file_ID, int32_t * buffer, int64_t count );

/*!
 * Remove file.
 *
 * Deletes the file specified in the parameter file_ID.
 *
 * \param[in] file_ID Identification of the file to be deleted.
 */
void flib_remove ( uint16_t file_ID );

#endif
