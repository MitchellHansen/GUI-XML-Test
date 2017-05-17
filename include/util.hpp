#pragma once
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>
#include <bitset>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <imgui/imgui.h>

const double PI = 3.141592653589793238463;
const float  PI_F = 3.14159265358979f;

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
    }
    return elems;
}

inline float elap_time() {
    static std::chrono::time_point<std::chrono::system_clock> start;
    static bool started = false;

    if (!started) {
        start = std::chrono::system_clock::now();
        started = true;
    }

    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = now - start;
    return static_cast<float>(elapsed_time.count());
}

struct fpsCounter {
public:
	fpsCounter() {};

	void frame(double delta_time) {

		// Apply 100 units of smoothing
		if (frame_count == 100) {
			frame_count = 0;
			fps_average = 0;
		}
		frame_count++;
		fps_average += (delta_time - fps_average) / frame_count;
	}

	void draw() {

		if (arr_pos == 200)
			arr_pos = 0;

		fps_array[arr_pos] = static_cast<float>(1.0 / fps_average);
		arr_pos++;

		ImGui::Begin("Performance");
		ImGui::PlotLines("FPS", fps_array, 200, 0, std::to_string(1.0 / fps_average).c_str(), 0.0f, 150.0f, ImVec2(200, 80));
		ImGui::End();
	}

private:

	float fps_array[200]{60};
	int arr_pos = 0;

	int frame_count = 0;
	double fps_average = 0;

};


inline sf::Vector3f sphereToCart(sf::Vector2f i) {

	auto r = sf::Vector3f(
		(1 * sin(i.y) * cos(i.x)),
		(1 * sin(i.y) * sin(i.x)),
		(1 * cos(i.y))
	);
	return r;
};

inline sf::Vector3f sphereToCart(sf::Vector3f i) {

	auto r = sf::Vector3f(
		(i.x * sin(i.z) * cos(i.y)),
		(i.x * sin(i.z) * sin(i.y)),
		(i.x * cos(i.z))
	);
	return r;
};


inline sf::Vector3f cartToSphere(sf::Vector3f in) {

	auto r = sf::Vector3f(
		sqrt(in.x * in.x + in.y * in.y + in.z * in.z),
		atan(in.y / in.x),
		atan(sqrt(in.x * in.x + in.y * in.y) / in.z)
	);
	return r;
};

inline sf::Vector2f cartToNormalizedSphere(sf::Vector3f in) {

	auto r = sf::Vector2f(
		atan2(sqrt(in.x * in.x + in.y * in.y), in.z), 
		atan2(in.y, in.x)
	);
	return r;
}

inline sf::Vector3f fixOrigin(sf::Vector3f base, sf::Vector3f head) {
	return head - base;
}

inline sf::Vector3f normalize(sf::Vector3f in) {

	float multiplier = sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
	auto r = sf::Vector3f(
		in.x / multiplier,
		in.y / multiplier,
		in.z / multiplier
	);
	return r;
}


inline float dotProduct(sf::Vector3f a, sf::Vector3f b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float magnitude(sf::Vector3f in){
	return sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
}

inline float angleBetweenVectors(sf::Vector3f a, sf::Vector3f b){
	return acos(dotProduct(a, b) / (magnitude(a) * magnitude(b)));
}

inline float distanceBetweenPoints(sf::Vector3f a, sf::Vector3f b) {
	return sqrt(dotProduct(a, b));
}

inline float degreesToRadians(float in) {
	return static_cast<float>(in * PI / 180.0f);
}

inline float radiansToDegrees(float in) {
	return static_cast<float>(in * 180.0f / PI);
}

inline std::string readFile(std::string file_name){

	std::ifstream input_file(file_name);

	if (!input_file.is_open()){
		std::cout << file_name << " could not be opened" << std::endl;
		return "";
	}

	std::stringstream buf;
	buf << input_file.rdbuf();
	input_file.close();
	return buf.str();
}

inline void dumpLog(std::stringstream* ss, std::string file_name) {
	
	std::ofstream log_file;
	log_file.open(file_name);

	log_file << ss->str();
	log_file.close();
}

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount _mm_popcnt_u32
#  define __builtin_popcountll _mm_popcnt_u64
#endif

inline int count_bits_int32(int32_t v) {
	
	return static_cast<int>(__builtin_popcount(v));
}

inline int count_bits_int64(int64_t v) {

	return static_cast<int>(__builtin_popcountll(v));
}

inline void setBit(int position, char* c) {
	*c |= (uint64_t)1 << position;
}

inline void flipBit(int position, char* c) {
	*c ^= (uint64_t)1 << position;
}

inline int getBit(int position, char* c) {
	return (*c >> position) & (uint64_t)1;
}

inline void setBit(int position, uint64_t* c) {
	*c |= (uint64_t)1 << position;
}

inline void flipBit(int position, uint64_t* c) {
	*c ^= (uint64_t)1 << position;
}

inline int getBit(int position, uint64_t* c) {
	return (*c >> position) & (uint64_t)1;
}
