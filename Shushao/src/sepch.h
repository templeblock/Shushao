#ifndef SE_GCH
#define SE_GCH

// std
#include <stdlib.h>
#include <windows.h>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <typeinfo>
#include <vector>
#include <cmath>

// boost
#define BOOST_ALL_NO_LIB
#include <boost/predef.h>

#if BOOST_COMP_GNUC
#include <dirent.h>
#endif

// GLM
#include <glm/glm.hpp>
#define GLM_FORCE_SWIZZLE
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/spline.hpp>
#include <glm/gtx/transform.hpp>

#endif