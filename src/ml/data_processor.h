#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <vector>
#include <string>
#include <map>

class DataProcessor {
public:
    struct NetworkTrafficData {
        std::vector<std::vector<double>> features;
        std::vector<std::string> labels;
        std::vector<std::string> feature_names;
        std::map<std::string, int> label_encoding;
    };
    
    NetworkTrafficData loadFromCSV(const std::string& filename);
    void normalizeFeatures(std::vector<std::vector<double>>& features);
    void splitData(const NetworkTrafficData& data,
                   double train_ratio,
                   NetworkTrafficData& train_data,
                   NetworkTrafficData& test_data);
};

#endif