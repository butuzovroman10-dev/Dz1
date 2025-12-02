#include "../ml/knn_classifier.h"
#include "../ml/data_processor.h"
#include <iostream>
#include <vector>

void testKNN() {
    std::cout << "Testing KNN Classifier..." << std::endl;
    
    // Создание тестовых данных
    std::vector<std::vector<double>> train_data = {
        {1.0, 2.0}, {2.0, 3.0}, {3.0, 1.0}, {4.0, 2.0},
        {6.0, 5.0}, {7.0, 7.0}, {8.0, 6.0}, {9.0, 8.0}
    };
    
    std::vector<std::string> train_labels = {
        "A", "A", "A", "A", "B", "B", "B", "B"
    };
    
    std::vector<std::vector<double>> test_data = {
        {1.5, 2.5}, {7.5, 7.5}
    };
    
    std::vector<std::string> test_labels = {
        "A", "B"
    };
    
    KNNClassifier knn;
    knn.fit(train_data, train_labels);
    
    // Тестирование предсказания
    auto predictions = knn.predictBatch(test_data, 3);
    
    std::cout << "Predictions: ";
    for (const auto& pred : predictions) {
        std::cout << pred << " ";
    }
    std::cout << std::endl;
    
    // Тестирование F1-score
    double f1 = knn.calculateF1Score(test_data, test_labels, 3);
    std::cout << "F1-Score: " << f1 << std::endl;
}