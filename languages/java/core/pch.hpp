//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __PCH_H__

#pragma once

#define COMPANY uilab

#define BEGIN_NAMESPACE(__x__) namespace COMPANY { ## namespace __x__ {
#define END_NAMESPACE   }; };

#define USINGNAMESPACE(__x__) using namespace COMPANY::##x;

typedef unsigned char byte;

#ifdef WIN32
static const char* INT64_FORMAT = "%I64d";
#else
static const char* INT64_FORMAT = "%lld";
#endif




#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <cstddef>

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>

#include <exception>
#include <stdexcept>

#include <memory>
#include <new>
#include <scoped_allocator>

#include <typeindex>
#include <type_traits>

#include <algorithm>
#include <functional>
#include <utility>
#include <limits>
#include <numeric>
#include <ratio>
#include <locale>
#include <random>

#include <string>
//#include <string_view>
#include <regex>


#include <array>
#include <valarray>
#include <bitset>
#include <tuple>
//#include <any>
//#include <variant>
//#include <optional>
#include <list>
#include <forward_list>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <cfenv>
#include <codecvt>

#include <chrono>

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <thread>
#include <future>
//#include <execution>

#endif // __PCH_H__
