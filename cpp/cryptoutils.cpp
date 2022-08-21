#include "cryptoutils.hpp"


// Constants
static const std::map<uint8_t, float> score_per_char = {
    {'a', 8.4966}, {'b', 2.0720}, {'c', 4.5388}, {'d', 3.3844}, {'e',11.1607}, {'f', 1.8121}, 
    {'g', 2.4705}, {'h', 3.0034}, {'i', 7.5448}, {'j', 0.1965}, {'k', 1.1016}, {'l', 5.4893}, 
    {'m', 3.0129}, {'n', 6.6544}, {'o', 7.1635}, {'p', 3.1671}, {'q', 0.1962}, {'r', 7.5809}, 
    {'s', 5.7351}, {'t', 6.9509}, {'u', 3.6308}, {'v', 1.0074}, {'w', 1.2899}, {'x', 0.2902}, 
    {'y', 1.7779}, {'z', 0.2722}
};

static const std::map<std::string, float> score_per_syllables = {
    {"al", 0.93}, {"an", 2.17}, {"ar", 1.06}, {"as", 1.09}, {"at", 1.17}, {"ea", 0.84}, {"ed", 1.29},
    {"en", 1.37}, {"er", 2.11}, {"es", 1.00}, {"ha", 1.17}, {"he", 3.65}, {"hi", 1.07}, {"in", 2.10},
    {"is", 0.99}, {"it", 1.24}, {"le", 0.95}, {"me", 0.83}, {"nd", 1.62}, {"ne", 0.75}, {"ng", 0.99},
    {"nt", 0.77}, {"on", 1.36}, {"or", 1.09}, {"ou", 1.41}, {"re", 1.64}, {"se", 0.85}, {"st", 0.96},
    {"te", 1.00}, {"th", 3.99}, {"ti", 0.92}, {"to", 1.24}, {"ve", 1.11}, {"wa", 0.84}
};

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(uint8_t c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

// Methods

std::vector<uint8_t> hex_to_byte(std::string hex_string) {    
    size_t lenght = hex_string.size();
    std::vector<uint8_t> data;

    for (size_t i = 0; i < lenght; i+=2)
    {
        int8_t c = hex_string[i], c2 = hex_string[i+1];
        int8_t value1 = 0, value2 = 0;

        if      (c >= '0' && c <= '9')   value1 = (c - '0');
        else if (c >= 'A' && c <= 'F')   value1 = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')   value1 = (10 + (c - 'a'));

        if      (c2 >= '0' && c2 <= '9') value2 = (c2 - '0');
        else if (c2 >= 'A' && c2 <= 'F') value2 = (10 + (c2 - 'A'));
        else if (c2 >= 'a' && c2 <= 'f') value2 = (10 + (c2 - 'a'));

        uint8_t final = 0;

        final += value1 << (((i + 1) % 2) * 4);
        final += value2 << ((((i+1) + 1) % 2) * 4);
        data.push_back(final);
    }
    return data;    
}

std::string bytes_to_hex(const uint8_t* bytes, uint size) {
    std::stringstream string_stream;

    string_stream << std::hex << std::setfill('0');

    for (uint i = 0; i < size; i++) {
        string_stream << std::hex << std::setfill('0');
        string_stream << std::setw(2)<<static_cast<int>(bytes[i]);
    }

    return string_stream.str();
}

std::string byte_to_b64(const uint8_t* bytes, uint size) {
    std::string ret;
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    while (size--) {
        char_array_3[i++] = *(bytes++);
        if (i == 3) {
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for(i = 0; (i <4) ; i++)
            ret += base64_chars[char_array_4[i]];
        i = 0;
    }
  }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::vector<uint8_t> b64_to_bytes(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  uint8_t char_array_4[4], char_array_3[3];
  std::vector<uint8_t> ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
          ret.push_back(char_array_3[i]);
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
  }

  return ret;
}

std::vector<uint8_t> fixed_xor(std::vector<uint8_t> buffer1, std::vector<uint8_t> buffer2) {
    assert(buffer1.size() == buffer2.size());
    std::vector<uint8_t> returnable;
    
    for (size_t i = 0; i < buffer1.size(); i++)
        returnable.push_back(buffer1.at(i) ^ buffer2.at(i));
    
    return returnable;
}

float score_message(std::vector<uint8_t> message) {
    float score = 0;
    for (uint8_t character : message) {
        if (score_per_char.find(character) != score_per_char.end())
            score += score_per_char.at(character);
    }
    for (uint i = 0; i < message.size() - 1; i++) {
        std::string syllable;
        syllable.push_back(message.at(i));
        syllable.push_back(message.at(i+1));
        if (score_per_syllables.find(syllable) != score_per_syllables.end())
            score += score_per_syllables.at(syllable) * 10;
    }
    return score;
}

void single_char_xor(std::vector<uint8_t> encrypted, WordScoring* result) {
    std::vector<uint8_t> key;
    uint s = 0;
    uint8_t changed = 0;

    for (int i = 0; i < 256; i++) {
        for(s = 0; s < encrypted.size(); s++)
            key.push_back((uint8_t)i);

        std::vector<uint8_t> message = fixed_xor(key, encrypted);
        float score = score_message(message);

        if (result->score < score) {
            changed = 1;
            result->key_char = i;
            result->score = score;
        }

        key.clear();
    }
    if (changed) {
        for(s = 0; s < encrypted.size(); s++)
            key.push_back(result->key_char);
        std::vector<uint8_t> message = fixed_xor(key, encrypted);
        memcpy(result->word, message.data(), message.size());
    }
}

std::vector<uint8_t> repeating_key_xor(std::vector<uint8_t> plain_text, std::vector<uint8_t> key) {
    uint size = key.size();
    
    uint position = 0;
    while (key.size() != plain_text.size())
    {
        key.push_back(key.at(position));
        position = (position + 1) % size;
    }

    return fixed_xor(plain_text, key);
}

uint hamming_distance(std::vector<uint8_t> v1, std::vector<uint8_t> v2) {
    uint hammering_distance = 0;
    std::vector<uint8_t> difference = fixed_xor(v1, v2);
    for (uint8_t byte : difference) {
        uint8_t tmp = byte;
        while (tmp != 0)
        {
            hammering_distance += tmp % 2;
            tmp = tmp / 2;
        }
    }
    return hammering_distance;
}

bool comp(const std::pair<uint, float> &a, const std::pair<uint, float> &b) { 
    return (a.second < b.second); 
} 

std::vector<std::pair<uint, float>> get_keysizes_distances(std::vector<uint8_t> content, uint min_keysize, uint max_keysize) {
    std::map<uint, float> keysizes_distances;
    // ranges with the keysize
    std::vector<uint8_t> range1(content.begin(), content.begin() + min_keysize), 
                         range2((content.begin() + min_keysize), (content.begin() + 2*min_keysize));

    for(uint keysize = min_keysize; keysize < max_keysize + 1; keysize++) {
        
        if (keysize != min_keysize) {
            range1.push_back(range2.at(0)); 
            range2.erase(range2.begin());
            range2.push_back(content.at((2*keysize) - 1));
            range2.push_back(content.at(2*keysize));
        }
        float normalized_distance = ((float)hamming_distance(range1, range2)) / keysize;
        keysizes_distances.insert({keysize, normalized_distance});
    }

    // Make a sorted vector with the map
    std::vector<std::pair<uint, float>> keysizes_vector;
    for (auto& it : keysizes_distances) keysizes_vector.push_back(it);
    std::sort(keysizes_vector.begin(), keysizes_vector.end(), comp);

    return keysizes_vector;
}

std::vector<uint8_t> get_best_key(std::vector<uint8_t> content, std::vector<std::pair<uint, float>> keysizes_vector, uint sample_size) {
    std::vector<uint8_t> best_key;
    float best_score = -1;

    for (uint i = 0; i < keysizes_vector.size() && i < sample_size; i++)
    {
        uint keysize = keysizes_vector.at(i).first;

        std::vector<std::vector<uint8_t>> blocks;
        for (uint j = 0; j < keysize; j++) {
            std::vector<uint8_t> block;
            for (uint k = j; k < content.size(); k += keysize) {
                block.push_back(content.at(k));
            }
            blocks.push_back(block);
        }
        std::vector<uint8_t> key;
        WordScoring word_scoring;
        float score = 0;
        word_scoring.word = new uint8_t[blocks.at(0).size()];
        for (std::vector<uint8_t> block : blocks) {
            single_char_xor(block, &word_scoring);
            key.push_back(word_scoring.key_char);
            score += word_scoring.score;
            word_scoring.score = -1;
        }
        delete word_scoring.word; 
        if (score > best_score) {
            best_key = key; best_score = score;
        }
    }
    return best_key;
}