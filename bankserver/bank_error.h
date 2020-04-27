#pragma once
#include "pch.h"

namespace bank_error {
	typedef enum session_error {};
	typedef enum transaction_error {};
	typedef enum session_pool_error {};
	typedef enum listener_error {};
	typedef enum db_query_error {};
}

namespace bank_result {
	typedef enum session_result { success, failed, pending, unknown };
	typedef enum transaction_result { success, failed, pending, unknown };
	typedef enum session_pool_result { success, failed, pending, unknown };
	typedef enum listener_result { success, failed, pending, unknown };
	typedef enum db_query_result { success, failed, pending, unknown };
}
