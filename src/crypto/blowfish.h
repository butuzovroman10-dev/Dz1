#ifndef BLOWFISH_H
#define BLOWFISH_H

#include <vector>
#include <cstdint>
#include <chrono>

class Blowfish {
private:
    uint32_t P[18];
    uint32_t S[4][256];
    
    void generateSubkeys(const std::vector<uint8_t>& key);
    uint32_t F(uint32_t x);
    void encryptBlock(uint32_t& left, uint32_t& right);
    void decryptBlock(uint32_t& left, uint32_t& right);
    
public:
    Blowfish();
    void setKey(const std::vector<uint8_t>& key);
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data);
    double measureEncryptionDelay(const std::vector<uint8_t>& data);
};

#endif