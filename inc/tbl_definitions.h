/**
 * @file tbl_definitions.h
 *
 * @author Noah Ryan
 *
 * This file contains definitions for the Table module.
 */


/**
 * This definition provides the maximum table name size in
 * bytes
 */
#define TBL_MAX_NAME_SIZE_BYTES 20


/**
 * This definition is the result type for the Table module,
 * indicating the success (TBL_RESULT_OKAY) of a function,
 * or providing an error code describing the cause of the error.
 */
typedef enum
{
	TBL_RESULT_INVALID          = 0, /*<< Invalid result value */
	TBL_RESULT_OKAY             = 1, /*<< Successful result */
	TBL_RESULT_NULL_POINTER     = 2, /*<< Null pointer detected */
	TBL_RESULT_INVALID_ARGUMENT = 3, /*<< Invalid argument provided to a function */
	TBL_RESULT_ENUM_RESULTS /*<< Number of result values */
} TBL_RESULT_ENUM;

/**
 * This enum enumerates the tables within the flight software.
 * Each table is identified with a unique number within this enum.
 */
typedef enum
{
	TBL_TABLE_ID_INVALID = 0,
	TBL_TABLE_ID_NUM_TABLES
} TBL_TABLE_ID_ENUM;

/**
 * This enum provides the possible locations of a table.
 * These can be either stored in persistent memory, or
 * stored in RAM.
 */
typedef enum
{
	TBL_LOCATION_INVALID  = 0, /* Invalid location value */
	TBL_LOCATION_STORED   = 1, /* Stored in persistent memory */
	TBL_LOCATION_RAM      = 2, /* Stored in RAM */
	TBL_LOCATION_NUM_LOCATIONS /* Number of locations */
} TBL_LOCATION_ENUM;

/**
 * This struct provides the status of the Table module.
 */
typedef struct
{
	uint16_t tables_read_ram;
	uint16_t tables_read_stored;
	uint16_t tables_updated_ram;
	uint16_t tables_updated_stored;
	uint16_t table_read_errors;
} TBL_Status;

/**
 * This struct contains the definition of a table used when
 * saving or loading tables.
 */
typedef struct
{
	char *name;
	uint32_t size;       /*<< The size of the table in bytes */
	uint8_t *table_ptr;  /*<< A pointer to the table data in RAM */
} TBL_TableDefinition;

{
/**
 * This struct is the state for the Table module.
 */
typedef struct
{
	TBL_TableDefinition definitions[TBL_TABLE_ID_NUM_TABLES];
	TBL_Status status;
} TBL_State;

