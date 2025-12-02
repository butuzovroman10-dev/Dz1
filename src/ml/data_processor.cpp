#include "data_processor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>

DataProcessor::NetworkTrafficData DataProcessor::loadFromCSV(const std::string& filename) {
    NetworkTrafficData result;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return result;
    }
    
    std::string line;
    bool is_first_line = true;
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        
        if (is_first_line) {
            // Первая строка - заголовки
            for (size_t i = 0; i < row.size() - 1; ++i) {
                result.feature_names.push_back(row[i]);
            }
            is_first_line = false;
        } else {
            // Данные
            std::vector<double> features;
            
            // Преобразование всех столбцов, кроме последнего, в числа
            for (size_t i = 0; i < row.size() - 1; ++i) {
                try {
                    double value = std::stod(row[i]);
                    features.push_back(value);
                } catch (...) {
                    features.push_back(0.0);
                }
            }
            
            result.features.push_back(features);
            
            // Последний столбец - метка
            std::string label = row.back();
            result.labels.push_back(label);
            
            // Обновление кодирования меток
            if (result.label_encoding.find(label) == result.label_encoding.end()) {
                int code = result.label_encoding.size();
                result.label_encoding[label] = code;
            }
        }
    }
    
    file.close();
    std::cout << "Loaded " << result.features.size() << " samples with " 
              << result.feature_names.size() << " features" << std::endl;
    
    return result;
}

void DataProcessor::normalizeFeatures(std::vector<std::vector<double>>& features) {
    if (features.empty()) return;
    
    size_t n_features = features[0].size();
    std::vector<double> mins(n_features, std::numeric_limits<double>::max());
    std::vector<double> maxs(n_features, std::numeric_limits<double>::lowest());
    
    // Нахождение минимумов и максимумов
    for (const auto& sample : features) {
        for (size_t i = 0; i < n_features; ++i) {
            if (sample[i] < mins[i]) mins[i] = sample[i];
            if (sample[i] > maxs[i]) maxs[i] = sample[i];
        }
    }
    
    // Нормализация
    for (auto& sample : features) {
        for (size_t i = 0; i < n_features; ++i) {
            double range = maxs[i] - mins[i];
            if (range > 0) {
                sample[i] = (sample[i] - mins[i]) / range;
            } else {
                sample[i] = 0.0;
            }
        }
    }
}

void DataProcessor::splitData(const NetworkTrafficData& data,
                             double train_ratio,
                             NetworkTrafficData& train_data,
                             NetworkTrafficData& test_data) {
    if (data.features.empty()) return;
    
    // Создание индексов для перемешивания
    std::vector<size_t> indices(data.features.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }
    
    // Перемешивание индексов
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    // Разделение данных
    size_t train_size = static_cast<size_t>(data.features.size() * train_ratio);
    
    train_data.feature_names = data.feature_names;
    test_data.feature_names = data.feature_names;
    train_data.label_encoding = data.label_encoding;
    test_data.label_encoding = data.label_encoding;
    
    for (size_t i = 0; i < indices.size(); ++i) {
        size_t idx = indices[i];
        
        if (i < train_size) {
            train_data.features.push_back(data.features[idx]);
            train_data.labels.push_back(data.labels[idx]);
        } else {
            test_data.features.push_back(data.features[idx]);
            test_data.labels.push_back(data.labels[idx]);
        }
    }
    
    std::cout << "Split data: " << train_data.features.size() << " training, "
              << test_data.features.size() << " test samples" << std::endl;
}