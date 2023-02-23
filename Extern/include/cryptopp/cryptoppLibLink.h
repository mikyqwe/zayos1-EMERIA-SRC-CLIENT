#ifndef _CRYPTOPPLIBLINK_H_
#define _CRYPTOPPLIBLINK_H_
#ifdef _WIN32
	#ifdef _DEBUG
		#pragma comment( lib, "cryptlib-Debug.lib" )
	#else
		#pragma comment( lib, "cryptlib-Release.lib" )
	#endif
#endif

#endif /* !_CRYPTOPPLIBLINK_H_ */
