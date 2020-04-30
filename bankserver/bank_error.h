#pragma once
#include "pch.h"

typedef enum session_error {};
typedef enum transaction_error {};
typedef enum session_pool_error {};
typedef enum listener_error {};
typedef enum db_query_error {};

typedef enum class session_result { success, failed, pending, unknown };
typedef enum class transaction_result { success, failed, pending, unknown };
typedef enum class session_pool_result { success, failed, pending, unknown };
typedef enum class listener_result { success, failed, pending, unknown };
typedef enum class db_query_result { success, failed, pending, unknown };

typedef enum class action_status { success, failed, pending, unknown };
typedef enum class result_type {
	session,
	transaction,
	sesion_pool,
	listener,
	db_query
};


struct bank_result {
	string what;
	action_status result;
	result_type type; 	
};
