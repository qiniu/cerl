/*
@encoding utf8
@ns cerl
@*/

#define CERL_VERBOSE
#define CERL_DIAGNOST

#define NS_CERL			cerl
#define NS_CERL_BEGIN	namespace cerl {
#define NS_CERL_END		}

#define cerl_call		winx_call
#define cerl_callback	WINAPI

#define CERL_ASSERT(e)
#define CERL_TRACE
#define CERL_TRACE_IF(e, y)

/** 正常调试信息。
@example
	CERL_TR("Send", ("Send %d bytes\n", bytes));
@*/
#define CERL_TR(f, y)

/** 详细日志信息。
@example
	CERL_VLOG("Send", ("Send %d bytes\n", bytes));
@*/
#define CERL_VLOG(f, y)

/** 正常日志信息。
@example
	CERL_LOG("Send", ("Send %d bytes\n", bytes));
@*/
#define CERL_LOG(f, y)

/** 异常情况，但可容忍（逻辑考虑了这种异常情况并已经进行处理）。
@example
	CERL_WARN("Send", ("Send %d bytes\n", bytes));
@*/
#define CERL_WARN(f, y)

/** 异常情况，但可容忍（逻辑考虑了这种异常情况并已经进行处理）。
@example
	CERL_ERROR("Recv", ("Recv failed: %d\n", errno));
@*/
#define CERL_ERROR(f, y)

// -------------------------------------------------------------------------

typedef String;

// -------------------------------------------------------------------------
