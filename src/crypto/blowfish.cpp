#include "blowfish.h"
#include <cstring>
#include <iostream>

// Константы инициализации P-блока и S-блоков
const uint32_t INIT_P[18] = {
    0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344,
    0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
    0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c,
    0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
    0x9216d5d9, 0x8979fb1b
};

const uint32_t INIT_S[4][256] = {
    // S-блоки (упрощенно - в реальной реализации должны быть полные)
    // Здесь приведен сокращенный пример
    { /* S[0] */ },
    { /* S[1] */ },
    { /* S[2] */ },
    { /* S[3] */ }
};

Blowfish::Blowfish() {
    // Инициализация P-блока
    std::memcpy(P, INIT_P, sizeof(INIT_P));
    // Инициализация S-блоков
    std::memcpy(S, INIT_S, sizeof(INIT_S));
}

void Blowfish::generateSubkeys(const std::vector<uint8_t>& key) {
    // Упрощенная генерация подключей
    // В реальной реализации здесь должен быть полный алгоритм генерации
    for (int i = 0, j = 0; i < 18; ++i) {
        uint32_t data = 0;
        for (int k = 0; k < 4; ++k) {
            data = (data << 8) | key[j];
            j = (j + 1) % key.size();
        }
        P[i] ^= data;
    }
    
    // Дальнейшая обработка P и S блоков
    // (опущено для краткости)
}

uint32_t Blowfish::F(uint32_t x) {
    uint16_t a = (x >> 24) & 0xFF;
    uint16_t b = (x >> 16) & 0xFF;
    uint16_t c = (x >> 8) & 0xFF;
    uint16_t d = x & 0xFF;
    
    // Упрощенная функция F
    return ((S[0][a] + S[1][b]) ^ S[2][c]) + S[3][d];
}

void Blowfish::encryptBlock(uint32_t& left, uint32_t& right) {
    for (int i = 0; i < 16; ++i) {
        left ^= P[i];
        right ^= F(left);
        std::swap(left, right);
    }
    std::swap(left, right);
    right ^= P[16];
    left ^= P[17];
}

void Blowfish::decryptBlock(uint32_t& left, uint32_t& right) {
    for (int i = 17; i > 1; --i) {
        left ^= P[i];
        right ^= F(left);
        std::swap(left, right);
    }
    std::swap(left, right);
    right ^= P[1];
    left ^= P[0];
}

void Blowfish::setKey(const std::vector<uint8_t>& key) {
    generateSubkeys(key);
}

std::vector<uint8_t> Blowfish::encrypt(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result = data;
    
    // Дополнение до размера блока (8 байт)
    size_t padding = 8 - (result.size() % 8);
    if (padding != 8) {
        result.insert(result.end(), padding, static_cast<uint8_t>(padding));
    }
    
    // Шифрование блоков
    for (size_t i = 0; i < result.size(); i += 8) {
        uint32_t left = (result[i] << 24) | (result[i+1] << 16) | 
                       (result[i+2] << 8) | result[i+3];
        uint32_t right = (result[i+4] << 24) | (result[i+5] << 16) | 
                        (result[i+6] << 8) | result[i+7];
        
        encryptBlock(left, right);
        
        result[i] = (left >> 24) & 0xFF;
        result[i+1] = (left >> 16) & 0xFF;
        result[i+2] = (left >> 8) & 0xFF;
        result[i+3] = left & 0xFF;
        result[i+4] = (right >> 24) & 0xFF;
        result[i+5] = (right >> 16) & 0xFF;
        result[i+6] = (right >> 8) & 0xFF;
        result[i+7] = right & 0xFF;
    }
    
    return result;
}

std::vector<uint8_t> Blowfish::decrypt(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> result = data;
    
    // Дешифрование блоков
    for (size_t i = 0; i < result.size(); i += 8) {
        uint32_t left = (result[i] << 24) | (result[i+1] << 16) | 
                       (result[i+2] << 8) | result[i+3];
        uint32_t right = (result[i+4] << 24) | (result[i+5] << 16) | 
                        (result[i+6] << 8) | result[i+7];
        
        decryptBlock(left, right);
        
        result[i] = (left >> 24) & 0xFF;
        result[i+1] = (left >> 16) & 0xFF;
        result[i+2] = (left >> 8) & 0xFF;
        result[i+3] = left & 0xFF;
        result[i+4] = (right >> 24) & 0xFF;
        result[i+5] = (right >> 16) & 0xFF;
        result[i+6] = (right >> 8) & 0xFF;
        result[i+7] = right & 0xFF;
    }
    
    // Удаление дополнения
    if (!result.empty()) {
        size_t padding = result.back();
        if (padding <= 8) {
            result.resize(result.size() - padding);
        }
    }
    
    return result;
}

double Blowfish::measureEncryptionDelay(const std::vector<uint8_t>& data) {
    auto start = std::chrono::high_resolution_clock::now();
    encrypt(data);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}