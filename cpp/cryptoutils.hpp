#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>

typedef struct {
    uint8_t* word;
    uint8_t key_char;
    float score = 0.0;
} WordScoring;

// Methods

std::vector<uint8_t> hex_to_byte(std::string hex_string);

std::string bytes_to_hex(const uint8_t* bytes, uint size);

std::string byte_to_b64(const uint8_t* bytes, uint size);

std::vector<uint8_t> b64_to_bytes(std::string const& encoded_string);

std::vector<uint8_t> fixed_xor(std::vector<uint8_t> buffer1, std::vector<uint8_t> buffer2);

float score_message(std::vector<uint8_t> message);

void single_char_xor(std::vector<uint8_t> encrypted, WordScoring* result);

std::vector<uint8_t> repeating_key_xor(std::vector<uint8_t> plain_text, std::vector<uint8_t> key);

uint hamming_distance(std::vector<uint8_t> v1, std::vector<uint8_t> v2);

bool comp(const std::pair<uint, float> &a, const std::pair<uint, float> &b);

std::vector<std::pair<uint, float>> get_keysizes_distances(std::vector<uint8_t> content, uint min_keysize, uint max_keysize);

std::vector<uint8_t> get_best_key(std::vector<uint8_t> content, std::vector<std::pair<uint, float>> keysizes_vector, uint sample_size);