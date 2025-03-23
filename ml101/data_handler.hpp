#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <unordered_set>
#include <memory>

#include "data.hpp"

class DataHandler {
    std::vector<std::unique_ptr<Data>> data_array;
    std::vector<Data*> training_data;
    std::vector<Data*> testing_data;
    std::vector<Data*> validation_data;

    int num_classes = 0;
    int feature_vector_size = 0;
    std::map<uint8_t, int> class_map;

    const double TRAIN_SET_PERCENT = 0.75;
    const double TEST_SET_PERCENT = 0.20;
    const double VALIDATION_SET_PERCENT = 0.05;

public:
    DataHandler() = default;
    ~DataHandler() = default;

    void read_feature_vector(const std::string& path);
    void read_feature_labels(const std::string& path);
    void split_data();
    void count_classes();

    uint32_t convert_to_little_endian(const unsigned char* bytes);

    const std::vector<Data*>& get_training_data() const;
    const std::vector<Data*>& get_test_data() const;
    const std::vector<Data*>& get_validation_data() const;
};

#endif
