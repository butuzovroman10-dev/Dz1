#include "knn_classifier.h"
#include <iostream>
#include <unordered_map>

KNNClassifier::KNNClassifier() : n_features(0) {}

double KNNClassifier::calculateDistance(const std::vector<double>& a, 
                                      const std::vector<double>& b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

void KNNClassifier::fit(const std::vector<std::vector<double>>& data, 
                       const std::vector<std::string>& labels) {
    training_data = data;
    training_labels = labels;
    if (!data.empty()) {
        n_features = data[0].size();
    }
}

std::string KNNClassifier::predict(const std::vector<double>& sample, int k) {
    std::vector<DistanceLabel> distances;
    
    for (size_t i = 0; i < training_data.size(); ++i) {
        double dist = calculateDistance(sample, training_data[i]);
        distances.push_back({dist, training_labels[i]});
    }
    
    std::sort(distances.begin(), distances.end());
    
    std::unordered_map<std::string, int> label_count;
    for (int i = 0; i < k && i < static_cast<int>(distances.size()); ++i) {
        label_count[distances[i].label]++;
    }
    
    std::string best_label;
    int max_count = 0;
    for (const auto& pair : label_count) {
        if (pair.second > max_count) {
            max_count = pair.second;
            best_label = pair.first;
        }
    }
    
    return best_label;
}

std::vector<std::string> KNNClassifier::predictBatch(
    const std::vector<std::vector<double>>& samples, int k) {
    std::vector<std::string> predictions;
    for (const auto& sample : samples) {
        predictions.push_back(predict(sample, k));
    }
    return predictions;
}

double KNNClassifier::calculateF1Score(const std::vector<std::vector<double>>& test_data,
                                     const std::vector<std::string>& test_labels,
                                     int k) {
    auto predictions = predictBatch(test_data, k);
    
    std::unordered_map<std::string, int> true_positives;
    std::unordered_map<std::string, int> false_positives;
    std::unordered_map<std::string, int> false_negatives;
    
    std::unordered_map<std::string, bool> unique_labels;
    for (const auto& label : training_labels) {
        unique_labels[label] = true;
    }
    
    for (size_t i = 0; i < predictions.size(); ++i) {
        const std::string& predicted = predictions[i];
        const std::string& actual = test_labels[i];
        
        if (predicted == actual) {
            true_positives[actual]++;
        } else {
            false_positives[predicted]++;
            false_negatives[actual]++;
        }
    }
    
    double macro_f1 = 0.0;
    int label_count = 0;
    
    for (const auto& label_pair : unique_labels) {
        const std::string& label = label_pair.first;
        
        int tp = true_positives[label];
        int fp = false_positives[label];
        int fn = false_negatives[label];
        
        double precision = (tp + fp > 0) ? static_cast<double>(tp) / (tp + fp) : 0.0;
        double recall = (tp + fn > 0) ? static_cast<double>(tp) / (tp + fn) : 0.0;
        
        double f1 = (precision + recall > 0) ? 
                   2 * precision * recall / (precision + recall) : 0.0;
        
        macro_f1 += f1;
        label_count++;
    }
    
    return (label_count > 0) ? macro_f1 / label_count : 0.0;
}