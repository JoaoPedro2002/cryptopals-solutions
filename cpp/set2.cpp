#include "aes.hpp"
#include "cryptoutils.hpp"

void challenge9() {
    std::cout << "Challenge 9" << std::endl;
    std::string s = "YELLOW SUBMARINE";
    std::vector<uint8_t> v(s.begin(), s.end());

    uint block_size = 20;
    v = pkcs7_padding(v, block_size);
    assert(v.size() == block_size);

    std::cout << v.size() << " == " << block_size << std::endl;
}

void challenge10(){
    std::cout << "Challenge 10" << std::endl;
    std::ifstream File("files/10.txt");
    std::string line;
    std::stringstream file_stream;
    while (std::getline(File, line)) file_stream << line;

    std::vector<uint8_t> buffer = b64_to_bytes(file_stream.str());
    buffer = pkcs7_padding(buffer, AES_BLOCKLEN);

    const uint8_t key[] = "YELLOW SUBMARINE";
    const uint8_t iv[]  = { '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' }; 
    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, buffer.data(), buffer.size());

    buffer.push_back(0);
    std::cout << buffer.data() << std::endl;

}

void challenge11(){
    std::cout << "Challenge 11" << std::endl;

}

void challenge12(){
    std::cout << "Challenge 12" << std::endl;

    
}

void challenge13(){
    std::cout << "Challenge 13" << std::endl;

}

void challenge14(){
    std::cout << "Challenge 14" << std::endl;

}

void challenge15(){
    std::cout << "Challenge 15" << std::endl;

}

void challenge16(){
    std::cout << "Challenge 15" << std::endl;

}

int main() {    
    // challenge9 (); std::cout << std::endl;
    challenge10(); std::cout << std::endl;
    challenge11(); std::cout << std::endl;
    challenge12(); std::cout << std::endl;
    challenge13(); std::cout << std::endl;
    challenge14(); std::cout << std::endl;
    challenge15(); std::cout << std::endl;
    challenge16(); std::cout << std::endl;
    
    return 0;
}