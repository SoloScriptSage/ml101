#include "data_handler.hpp"
#include <fstream>
#include <algorithm>
#include <random>

void DataHandler::read_feature_vector(const std::string& path) {
    uint32_t header[4]; // MAGIC | NUM IMAGES | ROWSIZE | COLSIZE
    unsigned char bytes[4];

    FILE* file = fopen(path.c_str(), "rb");
    if (!file) {
        perror("Error opening feature vector file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 4; ++i) {
        if (fread(bytes, sizeof(bytes), 1, file)) {
            header[i] = convert_to_little_endian(bytes);
        }
    }

    std::cout << "Done getting feature vector file header\n";
    int image_size = header[2] * header[3];

    for (int i = 0; i < header[1]; ++i) {
        auto data = std::make_unique<Data>();
        uint8_t pixel;

        for (int j = 0; j < image_size; ++j) {
            if (fread(&pixel, sizeof(pixel), 1, file)) {
                data->append_to_feature_vector(pixel);
            }
            else {
                std::cerr << "Error reading pixel\n";
                exit(EXIT_FAILURE);
            }
        }

        data_array.push_back(std::move(data));
    }

    fclose(file);
    std::cout << "Successfully read and stored " << data_array.size() << " feature vectors\n";
}

void DataHandler::read_feature_labels(const std::string& path) {
    uint32_t header[2]; // MAGIC | NUM IMAGES
    unsigned char bytes[4];

    FILE* file = fopen(path.c_str(), "rb");
    if (!file) {
        perror("Error opening label file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 2; ++i) {
        if (fread(bytes, sizeof(bytes), 1, file)) {
            header[i] = convert_to_little_endian(bytes);
        }
    }

    std::cout << "Done getting label file header\n";

    for (size_t i = 0; i < data_array.size(); ++i) {
        uint8_t label;
        if (fread(&label, sizeof(label), 1, file)) {
            data_array[i]->set_label(label);
        }
        else {
            std::cerr << "Error reading label\n";
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    std::cout << "Successfully read and stored " << data_array.size() << " labels\n";
}

void DataHandler::split_data() {
    std::vector<Data*> shuffled_data;
    for (auto& data : data_array) {
        shuffled_data.push_back(data.get());
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled_data.begin(), shuffled_data.end(), g);

    int train_size = shuffled_data.size() * TRAIN_SET_PERCENT;
    int test_size = shuffled_data.size() * TEST_SET_PERCENT;

    training_data.assign(shuffled_data.begin(), shuffled_data.begin() + train_size);
    testing_data.assign(shuffled_data.begin() + train_size, shuffled_data.begin() + train_size + test_size);
    validation_data.assign(shuffled_data.begin() + train_size + test_size, shuffled_data.end());

    std::cout << "Data split into training (" << training_data.size()
        << "), testing (" << testing_data.size()
        << "), and validation (" << validation_data.size() << ") sets.\n";
}

uint32_t DataHandler::convert_to_little_endian(const unsigned char* bytes) {
    return (static_cast<uint32_t>(bytes[0]) << 24) |
        (static_cast<uint32_t>(bytes[1]) << 16) |
        (static_cast<uint32_t>(bytes[2]) << 8) |
        (static_cast<uint32_t>(bytes[3]));
}

const std::vector<Data*>& DataHandler::get_training_data() const {
    return training_data;
}

const std::vector<Data*>& DataHandler::get_test_data() const {
    return testing_data;
}

const std::vector<Data*>& DataHandler::get_validation_data() const {
    return validation_data;
}

 
void DataHandler::count_classes() {
    int count = 0;
    for (size_t i = 0; i < data_array.size(); ++i) {
        uint8_t label = data_array[i]->get_label();

        if (class_map.find(label) == class_map.end()) {
            class_map[label] = count++;
        }
    }

    num_classes = count;
}

int main() {
    DataHandler* dataHandler = new DataHandler();
    dataHandler->read_feature_vector(".../FILE_NAME");
    dataHandler->read_feature_labels(".../FILE_NAME");
    dataHandler->split_data();
    dataHandler->count_classes();
}