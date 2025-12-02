#include "../crypto/blowfish.h"
#include <iostream>
#include <cassert>
#include <vector>

void testBlowfishBasic() {
    std::cout << "\n=== Testing Blowfish Basic Functionality ===" << std::endl;
    
    Blowfish blowfish;
    
    // Тестовый ключ (16 байт = 128 бит)
    std::vector<uint8_t> key = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87
    };
    
    blowfish.setKey(key);
    
    // Тестовые данные
    std::vector<uint8_t> plaintext = {
        'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'
    };
    
    std::cout << "Original text: ";
    for (uint8_t c : plaintext) std::cout << c;
    std::cout << std::endl;
    
    // Шифрование
    auto ciphertext = blowfish.encrypt(plaintext);
    std::cout << "Ciphertext size: " << ciphertext.size() << " bytes" << std::endl;
    
    // Дешифрование
    auto decrypted = blowfish.decrypt(ciphertext);
    
    std::cout << "Decrypted text: ";
    for (uint8_t c : decrypted) std::cout << c;
    std::cout << std::endl;
    
    // Проверка
    assert(plaintext.size() == decrypted.size());
    for (size_t i = 0; i < plaintext.size(); ++i) {
        assert(plaintext[i] == decrypted[i]);
    }
    
    std::cout << "✓ Basic encryption/decryption test passed" << std::endl;
}

void testBlowfishPerformance() {
    std::cout << "\n=== Testing Blowfish Performance ===" << std::endl;
    
    Blowfish blowfish;
    std::vector<uint8_t> key(16, 0x42); // 16-байтовый ключ
    
    blowfish.setKey(key);
    
    // Тестирование различных размеров пакетов
    std::vector<int> packet_sizes = {64, 128, 256, 512, 1024, 2048};
    
    std::cout << "Packet Size (bytes)\tEncryption Delay (ms)\tRequirement Met" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    
    for (int size : packet_sizes) {
        std::vector<uint8_t> packet(size);
        for (int i = 0; i < size; ++i) {
            packet[i] = i % 256;
        }
        
        double delay = blowfish.measureEncryptionDelay(packet);
        
        std::cout << size << "\t\t\t" << delay << "\t\t\t";
        if (delay < 1.0) {
            std::cout << "✓ PASS" << std::endl;
        } else {
            std::cout << "✗ FAIL" << std::endl;
        }
        
        // Дополнительная проверка целостности
        auto encrypted = blowfish.encrypt(packet);
        auto decrypted = blowfish.decrypt(encrypted);
        
        assert(packet.size() == decrypted.size());
        for (size_t i = 0; i < packet.size(); ++i) {
            assert(packet[i] == decrypted[i]);
        }
    }
}

void testBlowfishEdgeCases() {
    std::cout << "\n=== Testing Blowfish Edge Cases ===" << std::endl;
    
    Blowfish blowfish;
    std::vector<uint8_t> key = {0x00}; // Минимальный ключ
    
    blowfish.setKey(key);
    
    // Тест 1: Пустые данные
    std::vector<uint8_t> empty_data;
    auto encrypted_empty = blowfish.encrypt(empty_data);
    auto decrypted_empty = blowfish.decrypt(encrypted_empty);
    assert(empty_data == decrypted_empty);
    std::cout << "✓ Empty data test passed" << std::endl;
    
    // Тест 2: Один байт
    std::vector<uint8_t> single_byte = {0x41};
    auto encrypted_single = blowfish.encrypt(single_byte);
    auto decrypted_single = blowfish.decrypt(encrypted_single);
    assert(single_byte == decrypted_single);
    std::cout << "✓ Single byte test passed" << std::endl;
    
    // Тест 3: Точный размер блока (8 байт)
    std::vector<uint8_t> exact_block = {1, 2, 3, 4, 5, 6, 7, 8};
    auto encrypted_exact = blowfish.encrypt(exact_block);
    auto decrypted_exact = blowfish.decrypt(encrypted_exact);
    assert(exact_block == decrypted_exact);
    std::cout << "✓ Exact block size test passed" << std::endl;
    
    // Тест 4: Большие данные
    std::vector<uint8_t> large_data(10000);
    for (size_t i = 0; i < large_data.size(); ++i) {
        large_data[i] = i % 256;
    }
    auto encrypted_large = blowfish.encrypt(large_data);
    auto decrypted_large = blowfish.decrypt(encrypted_large);
    assert(large_data == decrypted_large);
    std::cout << "✓ Large data test passed" << std::endl;
}

void runAllCryptoTests() {
    std::cout << "Running Blowfish Cryptography Tests..." << std::endl;
    
    try {
        testBlowfishBasic();
        testBlowfishPerformance();
        testBlowfishEdgeCases();
        
        std::cout << "\n=========================================" << std::endl;
        std::cout << "All cryptography tests passed successfully!" << std::endl;
        std::cout << "=========================================" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
    }
}