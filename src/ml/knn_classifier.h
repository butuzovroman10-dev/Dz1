#ifndef KNN_CLASSIFIER_H
#define KNN_CLASSIFIER_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <queue>

class KNNClassifier {
private:
    std::vector<std::vector<double>> training_data;
    std::vector<std::string> training_labels;
    int n_features;
    
    struct DistanceLabel {
        double distance;
        std::string label;
        
        bool operator<(const DistanceLabel& other) const {
            return distance < other.distance;
        }
    };
    
    double calculateDistance(const std::vector<double>& a, const std::vector<double>& b);
    
public:
    KNNClassifier();
    void fit(const std::vector<std::vector<double>>& data, 
             const std::vector<std::string>& labels);
    std::string predict(const std::vector<double>& sample, int k);
    std::vector<std::string> predictBatch(const std::vector<std::vector<double>>& samples, int k);
    double calculateF1Score(const std::vector<std::vector<double>>& test_data,
                           const std::vector<std::string>& test_labels,
                           int k);
};

#endif