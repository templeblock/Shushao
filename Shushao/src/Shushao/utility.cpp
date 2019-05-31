#include "sepch.h"

#include "Debug.h"
#include "Utility.h"

namespace se {

	namespace util {

		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
		//random_device rd; // obtain a random number from hardware
		//mt19937 mt(rd()); // seed the generator
		std::mt19937_64 mt(ss);

		int random(int min, int max) {
			std::uniform_int_distribution<int> distr(min, max);
			return distr(mt);
		}

		float random(float min, float max) {
			std::uniform_real_distribution<float> distr(min, max);
			return distr(mt);
		}

		const std::type_info& ty(void* obj) {
			return typeid(obj);
		}

		bool happens(float probability) {
			if (probability == 1.0f) return true;
			float tr = random(0.0f, 1.0f);
			//Debug::Log << "tr: " << tr << " probability: " << probability << std::endl;
			return tr <= probability;
		}

		std::string basename(std::string filename) {
			std::string n = filename.substr(filename.find_last_of("/\\") + 1);
			return n.substr(0, n.find_last_of("."));
		}

		std::string zerofill(int number, int lenght) {
			std::ostringstream out;
			std::string sNumber = toString(number);

			for (unsigned int i = 0; i < lenght - sNumber.size(); i++) {
				out << "0";
			}

			out << sNumber;

			return out.str();
		}

		/// NB: modificare anche versione template in header
		std::string classtitle(std::string type) {
			std::regex pat("N2se|P6");
			type = std::regex_replace(type, pat, "");

			pat = "E$";
			type = std::regex_replace(type, pat, "");

			std::string title;
			bool inName = false;
			char prevc;
			for (char& c : type) {
				if (isdigit(c) && !inName) continue;
				if ((isupper(c) || isdigit(c)) && inName && !(c == 'D' && prevc == '2')) title += " ";
				title += (inName ? c : toupper(c));
				inName = true;
				prevc = c;
			}

			return title;
		}

		glm::vec3 toEulerAngles(const glm::quat& q) {
			float roll, pitch, yaw;

			// roll (x-axis rotation)
			double sinr = 2.0f * (q.w * q.x + q.y * q.z);
			double cosr = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
			roll = (float)atan2(sinr, cosr);

			// pitch (y-axis rotation)
			double sinp = +2.0f * (q.w * q.y - q.z * q.x);
			if (fabs(sinp) >= 1)
				pitch = (float)copysign(M_PI / 2, sinp);  // use 90 degrees if out of range
			else
				pitch = (float)asin(sinp);

			// yaw (z-axis rotation)
			double siny = +2.0f * (q.w * q.z + q.x * q.y);
			double cosy = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
			yaw = (float)atan2(siny, cosy);

			return { roll, pitch, yaw };
		}

		std::vector<std::string> split(std::string text, char delim) {
			std::vector<std::string> elements;
			std::stringstream stream(text);
			std::string item;
			while (getline(stream, item, delim)) {
				item.erase(0, item.find_first_not_of(" \n\r\t"));
				item.erase(item.find_last_not_of(" \n\r\t") + 1);
				elements.push_back(item);
			}
			return elements;
		}

		std::vector<std::wstring> wsplit(std::wstring text, wchar_t delim) {
			std::vector<std::wstring> elements;
			std::wstringstream stream(text);
			std::wstring item;
			while (getline(stream, item, delim)) {
				item.erase(0, item.find_first_not_of(L" \n\r\t"));
				item.erase(item.find_last_not_of(L" \n\r\t") + 1);
				elements.push_back(item);
			}
			return elements;
		}

		// std::vector<std::string> filesindir(std::string path) {
		// 	std::vector<std::string> files;
		// 	DIR* dir;
		// 	struct dirent* ent;
		// 	if ((dir = opendir(path.c_str())) != NULL) {
		// 		while ((ent = readdir(dir)) != NULL) {
		// 			files.push_back(ent->d_name);
		// 		}
		// 		closedir(dir);
		// 	}
		// 	return files;
		// }

	}  // namespace util

}  // namespace se
