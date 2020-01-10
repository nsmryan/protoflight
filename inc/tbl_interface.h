/**
 * @file tbl_interface.h
 *
 * @author Noah Ryan
 *
 * This file contains interface definitions for the Table module.
 *
 * The Table module is responsible for retrieving configuration
 * tables from persistent memory on initialization and providing
 * this data to other modules.
 * After initialization the Table module can provide or
 * update the most recent version of a table from RAM or persistent
 * memory or from RAM.
 */
#ifndef __TBL_INTERFACE_H__
#define __TBL_INTERFACE_H__

#include "tbl_definitions.h"


/**
 * @brief tbl_update_table
 *
 * This function updates the Table module's version of a
 * table. This will either update the table in persistent memory
 * (location == TBL_LOCATION_STORED) or in RAM
 * (location == TBL_LOCATION_RAM).
 *
 * @param[in] table_id - identifier for the table to update
 * @param[in] location - the table location to update
 * @param[in] table_buffer - the buffer to place the table
 * @param[in] table_size - the table_bufer size in bytes
 *
 * @return A TBL_RESULT_ENUM which indicates that table_buffer
 * was used to update the table, or an error code indicating the
 * source of the error.
 * 
 */
TBL_RESULT_ENUM tbl_update_table(TBL_TABLE_ID_ENUM table_id,
                                 TBL_LOCATION_ENUM location,
                                 uint8_t *table_buffer,
                                 uint32_t table_size);

/**
 * @brief tbl_retrieve_table
 *
 * This function retrieves a table (given by the table id)
 * from a given location (RAM or stored in persistent memory).
 * The table is copied into the given table buffer (table_buffer).
 *
 * @param[in] table_id - identifier for the table to update.
 * @param[in] location - the table location to update.
 * @param[out] table_buffer - the buffer to update with the table
 *             data.
 * @param[in] table_size - the table_bufer size in bytes.
 *
 * @return Either success (TBL_RESULT_OKAY) indicating that
 * table_buffer contains a copy of the table, or an error
 * code indicating the cause of the error. If an error
 * is returned then table_buffer is not modified.
 */
TBL_RESULT_ENUM tbl_retrieve_table(TBL_TABLE_ID_ENUM table_id,
                                   TBL_LOCATION_ENUM location,
                                   uint8_t *table_buffer,
                                   uint32_t table_size);

/**
 * @brief tbl_get_status
 *
 * This function provides the Table module's status.
 * No status is returned- if a null pointer is provided, it
 * will do nothing.
 */
void tbl_get_status(TBL_Status *status);

#endif // ndef __TBL_INTERFACE_H__ */
