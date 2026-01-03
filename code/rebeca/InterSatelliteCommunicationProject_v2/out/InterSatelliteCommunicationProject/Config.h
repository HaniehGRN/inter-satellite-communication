

#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "Types.h"

#define SAFE_MODE
#define PROGRESS_REPORT


#if (defined DEBUG || defined EXPORT_STATE_SPACE)
	#define ENABLE_STATE_ID
#endif



#define NEW_STATE 1



class StackOverflowException : public Exception {
public:
	StackOverflowException(string message) : Exception(message) {}
	~StackOverflowException() throw() {}
};



#endif