/*!
 * @file     mcl_json_util.h
 * @brief    Json utility module interface header file.
 *
 * This module is used for json handling like json parsing and json generating.
 *
 * @copyright Copyright (C) 2019 Siemens Aktiengesellschaft.\n
 *            All rights reserved.
 */

#ifndef MCL_JSON_UTIL_H_
#define MCL_JSON_UTIL_H_

#include "mcl_core/mcl_core_common.h"

#ifdef  __cplusplus
extern "C"
{
#endif

/**
 * Handle for json.
 */
typedef void mcl_json_t;

/**
 * Json type definitions.
 */
typedef enum E_MCL_JSON_TYPE
{
    MCL_JSON_ARRAY,  //!< Json array.
    MCL_JSON_OBJECT, //!< Json object.
} E_MCL_JSON_TYPE;

/**
 * This function initializes json library.
 */
extern MCL_CORE_EXPORT void mcl_json_util_library_initialize(void);

/**
 * This function initializes the given @p root json.
 *
 * @param [in] json_type Type of the json.
 * @param [out] root Root json.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_initialize(E_MCL_JSON_TYPE json_type, mcl_json_t **root);

/**
 * This function creates an array in @p root.
 *
 * @param [in] root Root json object.
 * @param [in] array_name Name of the array which is going to be created in @p root.
 * @param [out] json_array Json array which is going to be created in @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root, @p array_name or @p json_array is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p array_name is already used in @p root as object name.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_start_array(mcl_json_t *root, const char *array_name, mcl_json_t **json_array);

/**
 * This function gets the item at given index from @p array.
 *
 * @param [in] array Array json object.
 * @param [in] index Index of the item to get from @p array.
 * @param [out] item Item requested.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p array or @p item is NULL.</li>
 * <li>#MCL_INVALID_PARAMETER if provided @p index is out of range.</li>
 * <li>#MCL_JSON_TYPE_MISMATCH in case @p array is not a json array.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_get_array_item(mcl_json_t *array, int index, mcl_json_t **item);

/**
 * This function returns the size of @p array.
 *
 * @param [in]  array Array json object.
 * @param [out] size Size of @p array.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p array or @p size is NULL.</li>
 * <li>#MCL_JSON_TYPE_MISMATCH in case @p array is not a json array.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_get_array_size(mcl_json_t *array, mcl_size_t *size);

/**
 * This function creates an object in @p root.
 *
 * @param [in] root Root json object.
 * @param [in] object_name Name of the object which is going to be created in @p root.
 * @param [out] json_object Json object which is going to be created in @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root, @p object_name or @p json_object is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p object_name is already used in @p root as object name.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_start_object(mcl_json_t *root, const char *object_name, mcl_json_t **json_object);

/**
 * This function adds string to @p root which can be object or array.
 *
 * @note if @p root is array, @p object_name must be null.
 *
 * @param [in] root Root json. It can be object or array.
 * @param [in] object_name Name of the name/value pair which is going to be added to @p root.
 * @param [in] object_value Value of the name/value pair which is going to be added to @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root or @p object_value is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p object_name is already used in @p root as object name.</li>
 * <li>#MCL_INVALID_PARAMETER if @p root type is array and @p object_name is not null, or @p root type is object and @p object_name is null.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_add_string(mcl_json_t *root, const char *object_name, const char *object_value);

/**
 * This function adds integer number to @p root which can be object or array.
 *
 * @note if @p root is array, @p object_name must be null.
 *
 * @param [in] root Root json. It can be object or array.
 * @param [in] object_name Name of the name/value pair which is going to be added to @p root.
 * @param [in] number Value of the name/value pair which is going to be added to @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p object_name is already used in @p root as object name.</li>
 * <li>#MCL_INVALID_PARAMETER if @p root type is array and @p object_name is not null, or @p root type is object and @p object_name is null.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_add_uint(mcl_json_t *root, const char *object_name, const mcl_size_t number);

/**
 * This function adds double number to @p root which can be object or array.
 *
 * @param [in] root Root json. It can be object or array.
 * @param [in] object_name Name of the name/value pair which is going to be added to @p root.
 * @param [in] number Value of the name/value pair which is going to be added to @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p object_name is already used in @p root as object name.</li>
 * <li>#MCL_INVALID_PARAMETER if @p root type is array and @p object_name is not null, or @p root type is object and @p object_name is null.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_add_double(mcl_json_t *root, const char *object_name, const double number);

/**
 * This function adds @p bool_value to @p root which can be object or array.
 *
 * @param [in] root Root json. It can be object or array.
 * @param [in] object_name Name of the bool value which is going to be added to @p root.
 * @param [in] bool_value Bool value to be added to @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p object_name is already used in @p root as object name.</li>
 * <li>#MCL_INVALID_PARAMETER if @p root type is array and @p object_name is not null, or @p root type is object and @p object_name is null.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_add_bool(mcl_json_t *root, const char *object_name, const mcl_bool_t bool_value);

/**
 * This function adds null to @p root which can be object or array.
 *
 * @param [in] root Root json. It can be object or array.
 * @param [in] object_name Name of the null value which is going to be added to @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p object_name is already used in @p root as object name.</li>
 * <li>#MCL_INVALID_PARAMETER if @p root type is array and @p object_name is not null, or @p root type is object and @p object_name is null.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_add_null(mcl_json_t *root, const char *object_name);

/**
 * This function adds @p object to @p root.
 *
 * @param [in] root Root json object.
 * @param [in] object_name Name of @p object which is going to be going to be added to @p root.
 * @param [in] object @p object which is going to be added to @p root.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root, @p object_name or @p object is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NAME_DUPLICATION if the same @p object_name is already used in @p root as object name.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_add_object(mcl_json_t *root, const char *object_name, mcl_json_t *object);

/**
 * This function adds @p object to @p root array.
 *
 * @param [in] root Root json array.
 * @param [in] object @p object which is going to be added to @p root array.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root, @p object_name or @p object is NULL.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_add_item_to_array(mcl_json_t *root, mcl_json_t *object);

/**
 * This function gives the value of @p json_child object, when the @p child_name in @p json_parent object is given.
 *
 * @param [in] json_parent Root json object of @p json_child.
 * @param [in] child_name Name of the @p json_child object.
 * @param [out] json_child The json object which is going to be received.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p json_parent, @p child_name or @p json_child is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_NON_EXISTING_CHILD if @p json_child doesn't exist in @p json_parent.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_get_object_item(mcl_json_t *json_parent, const char *child_name, mcl_json_t **json_child);

/**
 * This function checks whether root object has child object or not.
 *
 * @param [in] root Json object which is going to be checked whether it has child or not.
 * @param [out] result This bool value is going to be made true, if @ root has child. Otherwise it is false.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root or @p result is NULL.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_has_child(mcl_json_t *root, mcl_bool_t *result);

/**
 * This function gets the integer value of a given json object.
 * Keep in mind that if the actual value of the json object is of type double,
 * the function will truncate double value to integer and return MCL_OK.
 *
 * @param [in] json Json object to get the value.
 * @param [out] number_value Value of @p json.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p json or @p number_value is NULL.</li>
 * <li>#MCL_JSON_TYPE_MISMATCH in case the requested value type is not integer or double.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_get_number_value(mcl_json_t *json, mcl_int32_t *number_value);

/**
 * This function gets the double value of a given json object.
 *
 * @param [in] json Json object to get the value.
 * @param [out] double_value Value of @p json.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p json or @p double_value is NULL.</li>
 * <li>#MCL_JSON_TYPE_MISMATCH in case the requested value type is not double.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_get_double_value(mcl_json_t *json, double *double_value);

/**
 * This function gets the boolean value of a given json object.
 *
 * @param [in] json Json object to get the value.
 * @param [out] bool_value Boolean value of @p json.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p json or @p bool_value is NULL.</li>
 * <li>#MCL_JSON_TYPE_MISMATCH in case the requested value type is not boolean.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_get_bool_value(mcl_json_t *json, mcl_bool_t *bool_value);

/**
 * This function gets the string value of a given json object.
 *
 * @param [in] json Json object to get the value.
 * @param [out] string_value String value of @p json. New memory space will be allocated for this parameter. Ownership passed to caller. Caller must free the space.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p json or @p string_value is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * <li>#MCL_JSON_TYPE_MISMATCH in case the requested value type is not string.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_get_string(mcl_json_t *json, char **string_value);

/**
 * This function gives the string of @p root in json format.
 *
 * @param [in] root Root json object.
 * @param [out] json_string The string of @p root in json format. New memory space will be allocated for this parameter. Ownership passed to caller. Caller must free the space.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p root or @p json_string is NULL.</li>
 * <li>#MCL_FAIL either the given json is invalid or memory can not be allocated for root.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_to_string(mcl_json_t *root, char **json_string);

/**
 * This function parses the given string to the given json object.
 *
 * @param [in] json_string String in json format. 
 * @param [in] buffer_size Size of the buffer.If @p json_string is null terminated, @p size must be zero.
 *             Otherwise @p size value must be size of @p json_string.
 * @param [out] root @p json_string is going to be parsed to this root object.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p json_string or @p root is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY If memory cannot be allocated for root.</li>
 * <li>#MCL_INVALID_PARAMETER If json does not end within @p buffer size.</li>
 * <li>#MCL_FAIL If given json has invalid format.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_parse(const char *json_string, mcl_size_t buffer_size, mcl_json_t **root);

/**
 * This function duplicates @p source_json as @p duplicated_json.
 *
 * @param [in] source_json Json object to be duplicated.
 * @param [out] duplicated_json Duplicated json from @p source_json.
 * @return
 * <ul>
 * <li>#MCL_OK in case of success.</li>
 * <li>#MCL_TRIGGERED_WITH_NULL if provided @p source_json or @p duplicated_json is NULL.</li>
 * <li>#MCL_OUT_OF_MEMORY in case there is not enough memory in the system to proceed.</li>
 * </ul>
 */
extern MCL_CORE_EXPORT mcl_error_t mcl_json_util_duplicate(const mcl_json_t *source_json, mcl_json_t **duplicated_json);

/**
 * This function destroys @p root.
 *
 * @note If @p root is destroyed, all child json objects in @p root are also destroyed.
 *
 * @param [out] root Root json object.
 */
extern MCL_CORE_EXPORT void mcl_json_util_destroy(mcl_json_t **root);

#ifdef  __cplusplus
}
#endif

#endif //MCL_JSON_UTIL_H_
