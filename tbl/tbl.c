/**
 * @file tbl.c
 *
 * @author Noah Ryan
 *
 * This file contains implementations of function for the Table module.
 */
#include "fsw_definitions.h"

#include "tbl_definitions.h"
#include "tbl_interface.h"


TBL_State gvTBL_state = {0}


FSW_RESULT_ENUM tbl_initialize(void)
{
	// NOTE read tables from flash.
	// Use table definitions for names and where to load them.
}

TBL_RESULT_ENUM tbl_retrieve_table(TBL_TABLE_ID_ENUM table_id,
				   TBL_LOCATION_ENUM location,
		                   uint8_t *table_buffer,
			           uint32_t table_size)
{
	TBL_RESULT_ENUM tbl_result = TBL_RESULT_OKAY;

	if (table_buffer == NULL)
	{
		tbl_result = TBL_RESULT_NULL_POINTER;
	}

	if (tbl_result == TBL_RESULT_OKAY)
	{
		switch (location)
		{
			case TBL_LOCATION_INVALID:
				tbl_result = TBL_RESULT_INVALID_ARGUMENT;
				break;

			case TBL_LOCATION_STORED:
				// NOTE provide stored copy
				break;

			case TBL_LOCATION_RAM:
				// NOTE provide RAM copy
				break;

			default:
				tbl_result = TBL_RESULT_INVALID_ARGUMENT;
				break;
		}
	}
	// NOTE probably need hardware to implement stored memory tables

	return tbl_result;
}

TBL_RESULT_ENUM tbl_update_table(TBL_TABLE_ID_ENUM table_id,
				 TBL_LOCATION_ENUM location,
		                 uint8_t *table_buffer,
			         uint32_t table_size)
{
	TBL_RESULT_ENUM tbl_result = TBL_RESULT_OKAY;

	// NOTE probably need hardware to implement stored memory tables

	return tbl_result;
}

void tbl_get_status(TBL_Status *status)
{
	if (status != NULL)
	{
		*status = gvTBL_status;
	}
}
