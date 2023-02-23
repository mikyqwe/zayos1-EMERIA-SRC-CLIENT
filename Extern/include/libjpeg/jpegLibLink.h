#ifndef _JPEGLIBLINK_H_
#define _JPEGLIBLINK_H_

#ifdef _WIN32

#ifdef _DEBUG
	#pragma comment(lib, "libjpeg-debug.lib")
#else
	#pragma comment(lib, "libjpeg.lib")
#endif

#endif

#endif /* !_JPEGLIBLINK_H_ */
