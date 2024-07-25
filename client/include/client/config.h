
/**
 * @file config.h
 * 
 * @copyright This file's a part of the "mediantazy" project and is distributed
 * and licensed under "mediantazy gplv1" license.
 * 
 * @author joba14
 * 
 * @date 2024-07-25
 */

#ifndef __server__include__server__config_h__
#define __server__include__server__config_h__

#include "common/types.h"

typedef struct
{
	const char_t* address;
	uint16_t port;
} client_config_s;

client_config_s client_config_from_cli(int32_t* const argc, const char_t*** const argv);

#endif
