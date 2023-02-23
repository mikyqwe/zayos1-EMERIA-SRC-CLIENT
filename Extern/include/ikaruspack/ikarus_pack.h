#pragma once
//* including standards headers
#include <map>
#include <string>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <atomic>
#include <functional>
#include <chrono>
#include <iostream>
#include <codecvt>
#include <locale>
#include <thread>
#include <mutex>
#include <atomic>
#include <ppl.h>

//extern dep.
#include <cryptopp/cryptlib.h>
#include <cryptopp/filters.h>
#include <cryptopp/default.h>
#include <cryptopp/hex.h>
#include <cryptopp/aes.h>

//ikex
#include <ikex/bench_mark.h>
#include <ikex/format.h>

//* including windows headers
#include <Windows.h>

//* new changes
#define __IMPROVE_IMPORT_ARCHIVE__
#define __USE_PARALLEL_FOR__ 
#define __USE_NEW_EASY_ENCRYPT__
#define __USE_SHORTER_DICTFILE_SIZE__
//#define __USE_DEBUG_ALLOC__

//* declaring prototypes
namespace ikaruspack {
	std::wstring utils_to_wstring(const std::string& st);
	std::string  utils_to_string(const std::wstring& st);
}

//* including library headers
#include "dynamic_buffer.h"
#include "key.h"
#include "archive.h"
#include "mapped_archive.h"
#include "utils.h"

//#define ENABLE_TESTS

