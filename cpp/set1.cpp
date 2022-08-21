#include "aes.hpp"
#include "cryptoutils.hpp"

// Challenges

void challenge1() {
    std::cout << "Challenge 1" << std::endl;
    const std::string b16 = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    
    std::vector<uint8_t> bytes = hex_to_byte(b16);
    std::cout << "Bytes:   " << bytes.data() << std::endl;

    std::string b64 = byte_to_b64(bytes.data(), bytes.size());
    std::cout << "Base 64: " << b64 << std::endl;

    assert(b64 == "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t");
}

void challenge2() {
    std::cout << "Challenge 2" << std::endl;

    std::string hex_buf1 = "1c0111001f010100061a024b53535009181c";
    std::string hex_buf2 = "686974207468652062756c6c277320657965";

    std::vector<uint8_t> xored = fixed_xor(hex_to_byte(hex_buf1), hex_to_byte(hex_buf2));

    std::cout << "Xored:     " << +xored.data() << std::endl;

    std::string hex_xored = bytes_to_hex(xored.data(), xored.size());

    std::cout << "Hex xored: " << hex_xored << std::endl;

    assert(hex_xored == "746865206b696420646f6e277420706c6179");
}

void challenge3() {
    std::cout << "Challenge 3" << std::endl;

    std::string hex_encrypted = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    std::vector<uint8_t> encrypted = hex_to_byte(hex_encrypted);

    WordScoring result;
    result.word = new uint8_t[encrypted.size() + 1];
    single_char_xor(encrypted, &result);

    result.word[encrypted.size()] = 0;
    std::cout << "key:   " << result.key_char << std::endl
              << "score: " << result.score    << std::endl
              << "word:  " << result.word     << std::endl;
    delete result.word;
}

void challenge4() {
    std::cout << "Challenge 4" << std::endl;

    WordScoring result;
    uint8_t first_line = 1;
    std::string line; 
    std::ifstream File("files/4.txt");
    while (std::getline(File, line))
    {
        std::vector<uint8_t> encrypted = hex_to_byte(line);
        if (first_line) {
            result.word = new uint8_t[encrypted.size() + 1];
            first_line = 0;
        }

        single_char_xor(encrypted, &result);
        result.word[encrypted.size()] = 0;
    }
    File.close();

    std::cout << "key:   " << result.key_char << std::endl
              << "score: " << result.score    << std::endl
              << "word:  " << result.word     << std::endl;
    delete result.word;
}

void challenge5() {
    std::cout << "Challenge 5" << std::endl;

    std::string plain_text = "Burning 'em, if you ain't quick and nimble\n" 
    "I go crazy when I hear a cymbal ", 
                key_text   = "ICE";
    std::vector<uint8_t> vector(plain_text.begin(), plain_text.end()),
                         key_vector(key_text.begin(), key_text.end());
    std::vector<uint8_t> encryped = repeating_key_xor(vector, key_vector);

    std::string hex_encrypted = bytes_to_hex(encryped.data(), encryped.size());
    hex_encrypted.pop_back(); hex_encrypted.pop_back();

    vector.push_back(0); hex_encrypted.push_back(0);
    std::cout << "plain text: " << vector.data() << std::endl
              << "encrypted:  " << hex_encrypted << std::endl
              << "expected:   " << "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f" << std::endl;
}

void challenge6() {
    std::cout << "Challenge 6" << std::endl;

    uint min_keysize = 2, max_keysize = 40;
    
    // Read the file and put its contents in a vector
    std::ifstream File("files/6.txt");
    std::string line;
    std::stringstream file_stream;
    while (std::getline(File, line)) file_stream << line; 

    std::vector<uint8_t> file_contents = b64_to_bytes(file_stream.str());

    // Find the keylen with the smallest distance
    std::vector<std::pair<uint, float>> keysizes_vector = get_keysizes_distances(file_contents, min_keysize, max_keysize);

    // Iterate over the smallest values 
    uint8_t sample_size = 30;
    std::vector<uint8_t> best_key = get_best_key(file_contents, keysizes_vector, sample_size);
    std::vector<uint8_t> decrypted = repeating_key_xor(file_contents, best_key);

    best_key.push_back(0);
    decrypted.push_back(0);
    std::cout << "key: "<< best_key.data() << std::endl;
    std::cout << decrypted.data();

    File.close();
}

void challenge7() {
    std::cout << "Challenge 7" << std::endl;
    uint8_t key[] = "YELLOW SUBMARINE";
    AES_ctx ctx;

    // Read the file and put its contents in a vector
    std::ifstream File("files/7.txt");
    std::string line;
    std::stringstream file_stream;
    while (std::getline(File, line)) file_stream << line;
    std::vector<uint8_t> file_contents = b64_to_bytes(file_stream.str());

    std::vector<uint8_t> final_v;
    AES_init_ctx(&ctx, key);
    for (size_t i=0; i < file_contents.size(); i+= AES_BLOCKLEN) {
        std::vector<uint8_t> v(file_contents.begin() + i, file_contents.begin() + i + AES_BLOCKLEN);
        AES_ECB_decrypt(&ctx, v.data());
        final_v.insert(final_v.end(), v.begin(), v.end());
    }
    final_v.push_back(0);
    std::cout << final_v.data();

    File.close();

}

void challenge8() {
    std::cout << "Challenge 8" << std::endl;

    // Read the file
    std::ifstream File("files/8.txt");
    std::string line;

    std::pair<std::string, int> best = {"", 0};
    while (std::getline(File, line)) {
        std::map<std::string, int> repetitions;
        using pair_type = decltype(repetitions)::value_type;

        std::vector<uint8_t> line_bytes = hex_to_byte(line);
        for (size_t i=0; i < line_bytes.size(); i+=AES_BLOCKLEN) {
            std::string line_fragment(line_bytes.begin() + i, line_bytes.begin() + i + AES_BLOCKLEN);
            if (repetitions.find(line_fragment) != repetitions.end()) {
                repetitions[line_fragment]++;
            } else {
                repetitions[line_fragment] = 1;
            }
        }
        auto pr = std::max_element(
            std::begin(repetitions), std::end(repetitions),
            [] (const pair_type & p1, const pair_type & p2) {
                return p1.second < p2.second;
            }   
        );
        
        if (pr->second > best.second) {
            best.first = line; best.second = pr->second;
        }
    }

    std::cout << "best line:   " << best.first << 
              "\n repetitions: " << best.second << std::endl;

}

int main() {    
    challenge1(); std::cout << std::endl;
    challenge2(); std::cout << std::endl;
    challenge3(); std::cout << std::endl;
    challenge4(); std::cout << std::endl;
    challenge5(); std::cout << std::endl;
    challenge6(); std::cout << std::endl;
    challenge7(); std::cout << std::endl;
    challenge8(); std::cout << std::endl;
    
    return 0;
}