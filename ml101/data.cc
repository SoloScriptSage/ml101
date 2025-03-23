#include "data.hpp"

void Data::set_feature_vector(const std::vector<uint8_t>& vect) {
    feature_vector = vect;  // Deep copy to avoid memory issues
}

void Data::append_to_feature_vector(uint8_t val) {
    feature_vector.push_back(val);
}

void Data::set_label(uint8_t val) {
    label = val;
}

void Data::set_enum_label(int val) {
    enum_label = val;
}

int Data::get_feature_vector_size() const {
    return feature_vector.size();
}

uint8_t Data::get_label() const {
    return label;
}

int Data::get_enum_label() const {
    return enum_label;
}

const std::vector<uint8_t>& Data::get_feature_vector() const {
    return feature_vector;
}
