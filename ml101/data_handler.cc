#define _CRT_SECURE_NO_WARNINGS  // Place this at the very top

#include "data_handler.hpp"
#include <fstream>
#include <algorithm>
#include <random>

using namespace std;

DataHandler::DataHandler() {
    data_array = new vector<Data*>;
    testing_data = new vector<Data*>;
    training_data = new vector<Data*>;
    validation_data = new vector<Data*>;    
}

DataHandler::~DataHandler() {
    // Free allocated data
}
void DataHandler::read_feature_vector(string path) {
    uint32_t header[4]; // MAGIC | NUM IMAGES | ROWSIZE | COLSIZE
    unsigned char bytes[4];

    FILE* file = fopen(path.c_str(), "r");    
    if(file){
        for (int i = 0; i < 4; ++i) {
            if (fread(bytes, sizeof(bytes), 1, file)) {
                header[i] = convert_to_little_endian(bytes);
            }
        }

        printf("Done getting feature input file header\n");
        int image_size = header[2] * header[3];

        for (int i = 0; i < header[1]; ++i) {
            Data *data = new Data();
            uint8_t element[1];

            for (int j = 0; j < image_size; ++j) {
                if (fread(element, sizeof(element), 1, file)) {
                    data->append_to_feature_vector(element[0]);
                }
                else {
                    printf("Error reading pixel\n");
                    exit(1);
                }
            }
            data_array->push_back(data);
        }

        printf("Successfully read and stored %lu feature vectors.\n", data_array->size());
    }else {
        printf("Couldn't find a file.\n");
        exit(1);
    }
}

void DataHandler::read_feature_labels(string path) {
    uint32_t header[2]; // MAGIC | NUM IMAGES | ROWSIZE | COLSIZE
    unsigned char bytes[2];

    FILE* file = fopen(path.c_str(), "r");    
    if(file){
        for (int i = 0; i < 2; ++i) {
            if (fread(bytes, sizeof(bytes), 1, file)) {
                header[i] = convert_to_little_endian(bytes);
            }
        }

        printf("Done getting label file header\n");

        for (int i = 0; i < header[1]; ++i) {
            uint8_t element[1];
            if (fread(element, sizeof(element), 1, file)) {
                data_array->at(i)->set_label(element[0]);  
            }
            else {
                printf("Error reading pixel\n");
                exit(1);
            }
        }
        printf("Successfully read and stored label.\n", data_array->size());
    }else {
        printf("Couldn't find a file.\n");
        exit(1);
    }
}


void DataHandler::split_data() {
    unordered_set<int> used_indexes;
    int train_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int valid_size = data_array->size() * VALIDATION_SET_PERCENT;

    // Training Data
    int count  = 0;
    while(count < train_size) {
        int rand_index = rand() & data_array->size();
        if(used_indexes.find(rand_index) == used_indexes.end()) {
            training_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }

    // Test Data
    count  = 0;
    while(count < test_size) {
        int rand_index = rand() & data_array->size();
        if(used_indexes.find(rand_index) == used_indexes.end()) {
            testing_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }

    // Validation Data
    count  = 0;
    while(count < test_size) {
        int rand_index = rand() & data_array->size();
        if(used_indexes.find(rand_index) == used_indexes.end()) {
            validation_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            count++;
        }
    }

    printf("Training Data Size: %lu.\n", training_data->size());
    printf("Testing Data Size: %lu.\n", testing_data->size());
    printf("Validaton Data Size: %lu.\n", validation_data->size());
}

uint32_t DataHandler::convert_to_little_endian(const unsigned char* bytes) {
    return (uint32_t) ( 
        (bytes[0] << 24) |
        (bytes[1] << 16) | 
        (bytes[2] << 8) | 
        (bytes[3])
    );
}

vector<Data*> *DataHandler::get_training_data() {
    return training_data;
}

vector<Data*> *DataHandler::get_test_data() {
    return testing_data;
}

vector<Data*> *DataHandler::get_validation_data() {
    return validation_data;
}

 
void DataHandler::count_classes() {
    int count = 0;
    for (unsigned i = 0; i < data_array->size(); ++i) {
        if(class_map.find(data_array->at(i)->get_label()) == class_map.end()) {
            class_map[data_array->at(i)->get_label()] = count;
            data_array->at(i)->set_enum_label(count);
            count++;
        }
    }

    num_classes = count;
    printf("Successfully Extracted %d Unique Classes.\n", num_classes);
}

int main() {
    DataHandler* dataHandler = new DataHandler();
    dataHandler->read_feature_vector("../fashion/train-images-idx3-ubyte");
    dataHandler->read_feature_labels("../fashion/train-labels-idx1-ubyte");
    dataHandler->split_data();
    dataHandler->count_classes();
    delete dataHandler;  // Free memory
}