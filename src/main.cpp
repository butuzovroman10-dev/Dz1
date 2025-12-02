#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <map>
#include <random>
#include "ml/knn_classifier.h"
#include "crypto/blowfish.h"

// Простые демонстрационные тесты
void runSimpleKNNTests() {
    std::cout << "\n=== Simple KNN Tests ===" << std::endl;
    
    KNNClassifier knn;
    
    // Простые данные для классификации
    std::vector<std::vector<double>> train_data = {
        {1.0, 1.0}, {1.5, 2.0}, {2.0, 1.0}, {3.0, 3.0},  // Класс A
        {8.0, 8.0}, {9.0, 9.0}, {10.0, 8.0}, {8.0, 10.0} // Класс B
    };
    
    std::vector<std::string> train_labels = {"A", "A", "A", "A", "B", "B", "B", "B"};
    
    std::vector<std::vector<double>> test_data = {
        {2.0, 2.0},  // Должен быть A
        {9.0, 9.0},  // Должен быть B
        {5.0, 5.0}   // Граничный случай
    };
    
    knn.fit(train_data, train_labels);
    
    std::cout << "Test samples:" << std::endl;
    for (size_t i = 0; i < test_data.size(); ++i) {
        for (int k : {1, 3, 5}) {
            std::string prediction = knn.predict(test_data[i], k);
            std::cout << "Point [" << test_data[i][0] << ", " << test_data[i][1] 
                      << "] with k=" << k << ": " << prediction << std::endl;
        }
        std::cout << std::endl;
    }
}

void runSimpleBlowfishTests() {
    std::cout << "\n=== Simple Blowfish Tests ===" << std::endl;
    
    Blowfish blowfish;
    
    // Тестовый ключ
    std::vector<uint8_t> key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    blowfish.setKey(key);
    
    // Тестовые сообщения разных размеров
    std::vector<std::string> test_messages = {
        "Hello",
        "Network packet",
        "This is a longer test message for encryption",
        "Short",
        "A"
    };
    
    for (const auto& message : test_messages) {
        std::vector<uint8_t> plaintext(message.begin(), message.end());
        
        std::cout << "\nOriginal: \"" << message << "\" (" 
                  << plaintext.size() << " bytes)" << std::endl;
        
        // Шифрование
        auto start = std::chrono::high_resolution_clock::now();
        auto ciphertext = blowfish.encrypt(plaintext);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> encrypt_time = end - start;
        
        // Дешифрование
        start = std::chrono::high_resolution_clock::now();
        auto decrypted = blowfish.decrypt(ciphertext);
        end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> decrypt_time = end - start;
        
        std::string recovered(decrypted.begin(), decrypted.end());
        
        std::cout << "Encrypted size: " << ciphertext.size() << " bytes" << std::endl;
        std::cout << "Encryption time: " << encrypt_time.count() << " ms" << std::endl;
        std::cout << "Decryption time: " << decrypt_time.count() << " ms" << std::endl;
        std::cout << "Total time: " << (encrypt_time + decrypt_time).count() << " ms" << std::endl;
        std::cout << "Decrypted: \"" << recovered << "\"" << std::endl;
        std::cout << "Success: " << (message == recovered ? "YES" : "NO") << std::endl;
    }
}

void performanceTestKNN() {
    std::cout << "\n=== KNN Performance Test ===" << std::endl;
    
    std::ofstream report("knn_performance.csv");
    report << "Size,F1_Score,Time_ms\n";
    
    std::vector<int> sizes = {100, 500, 1000, 2000, 5000};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 100.0);
    std::uniform_int_distribution<> label_dist(0, 1);
    
    for (int size : sizes) {
        KNNClassifier knn;
        
        // Генерация данных
        std::vector<std::vector<double>> train_data;
        std::vector<std::string> train_labels;
        
        for (int i = 0; i < size; ++i) {
            std::vector<double> sample(10); // 10 признаков
            for (int j = 0; j < 10; ++j) {
                sample[j] = dist(gen);
            }
            train_data.push_back(sample);
            train_labels.push_back(label_dist(gen) ? "Attack" : "Normal");
        }
        
        // Тестовая выборка (20%)
        int test_size = size / 5;
        std::vector<std::vector<double>> test_data;
        std::vector<std::string> test_labels;
        
        for (int i = 0; i < test_size; ++i) {
            std::vector<double> sample(10);
            for (int j = 0; j < 10; ++j) {
                sample[j] = dist(gen);
            }
            test_data.push_back(sample);
            test_labels.push_back(label_dist(gen) ? "Attack" : "Normal");
        }
        
        // Обучение и тестирование
        auto start = std::chrono::high_resolution_clock::now();
        knn.fit(train_data, train_labels);
        double f1 = knn.calculateF1Score(test_data, test_labels, 5);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration<double, std::milli>(end - start);
        
        report << size << "," << f1 << "," << duration.count() << "\n";
        
        std::cout << "Size: " << size 
                  << ", F1 Score: " << std::fixed << std::setprecision(3) << f1
                  << ", Time: " << duration.count() << " ms" << std::endl;
    }
    
    report.close();
    std::cout << "Report saved to knn_performance.csv" << std::endl;
}

void performanceTestBlowfish() {
    std::cout << "\n=== Blowfish Performance Test ===" << std::endl;
    
    std::ofstream report("blowfish_performance.csv");
    report << "PacketSize,EncryptionTime_ms,DecryptionTime_ms,TotalTime_ms\n";
    
    Blowfish blowfish;
    std::vector<uint8_t> key(16, 0x42);
    blowfish.setKey(key);
    
    std::vector<int> packet_sizes = {64, 128, 256, 512, 1024, 2048};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> byte_dist(0, 255);
    
    for (int size : packet_sizes) {
        // Генерация случайного пакета
        std::vector<uint8_t> packet(size);
        for (int i = 0; i < size; ++i) {
            packet[i] = static_cast<uint8_t>(byte_dist(gen));
        }
        
        // Измерение времени шифрования
        auto start_enc = std::chrono::high_resolution_clock::now();
        auto encrypted = blowfish.encrypt(packet);
        auto end_enc = std::chrono::high_resolution_clock::now();
        
        // Измерение времени дешифрования
        auto start_dec = std::chrono::high_resolution_clock::now();
        auto decrypted = blowfish.decrypt(encrypted);
        auto end_dec = std::chrono::high_resolution_clock::now();
        
        auto enc_time = std::chrono::duration<double, std::milli>(end_enc - start_enc);
        auto dec_time = std::chrono::duration<double, std::milli>(end_dec - start_dec);
        auto total_time = enc_time + dec_time;
        
        report << size << "," << enc_time.count() << "," 
               << dec_time.count() << "," << total_time.count() << "\n";
        
        std::cout << "Packet: " << size << " bytes, "
                  << "Enc: " << enc_time.count() << " ms, "
                  << "Dec: " << dec_time.count() << " ms, "
                  << "Total: " << total_time.count() << " ms" 
                  << " (Requirement: " << (total_time.count() < 1.0 ? "PASS" : "FAIL") << ")" 
                  << std::endl;
        
        // Проверка целостности
        bool success = (packet == decrypted);
        if (!success) {
            std::cout << "WARNING: Decryption failed for packet size " << size << std::endl;
        }
    }
    
    report.close();
    std::cout << "Report saved to blowfish_performance.csv" << std::endl;
}

int main(int argc, char* argv[]) {
    std::cout << "================================================" << std::endl;
    std::cout << "   Network Security Analysis System" << std::endl;
    std::cout << "   Variant 6: KNN + Blowfish" << std::endl;
    std::cout << "================================================" << std::endl;
    
    srand(time(nullptr)); // Инициализация генератора случайных чисел
    
    if (argc > 1) {
        std::string command = argv[1];
        
        if (command == "--simple") {
            runSimpleKNNTests();
            runSimpleBlowfishTests();
        } else if (command == "--performance") {
            performanceTestKNN();
            performanceTestBlowfish();
        } else if (command == "--all") {
            runSimpleKNNTests();
            runSimpleBlowfishTests();
            performanceTestKNN();
            performanceTestBlowfish();
        } else if (command == "--help") {
            std::cout << "\nUsage: " << argv[0] << " [option]\n";
            std::cout << "Options:\n";
            std::cout << "  --simple       Run simple demonstration tests\n";
            std::cout << "  --performance  Run performance tests with reports\n";
            std::cout << "  --all          Run all tests\n";
            std::cout << "  --help         Show this help message\n";
            std::cout << "  (no args)      Run demonstration\n";
        }
    } else {
        // Запуск демонстрации по умолчанию
        std::cout << "\nRunning demonstration...\n";
        runSimpleKNNTests();
        runSimpleBlowfishTests();
        
        std::cout << "\n================================================" << std::endl;
        std::cout << "For more tests:\n";
        std::cout << "  " << argv[0] << " --simple       (basic functionality)\n";
        std::cout << "  " << argv[0] << " --performance  (with CSV reports)\n";
        std::cout << "  " << argv[0] << " --all          (all tests)\n";
        std::cout << "================================================" << std::endl;
    }
    
    return 0;
}
