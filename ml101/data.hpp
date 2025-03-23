#ifndef __DATA_H
#define __DATA_H

#include <vector>
#include <cstdint>
#include <memory>

class Data {
    std::vector<uint8_t> feature_vector;
    uint8_t label;
    int enum_label;

public:
    Data() = default;
    ~Data() = default;

    void set_feature_vector(const std::vector<uint8_t>& vect);
    void append_to_feature_vector(uint8_t val);
    void set_label(uint8_t val);
    void set_enum_label(int val);

    int get_feature_vector_size() const;
    uint8_t get_label() const;
    int get_enum_label() const;

    const std::vector<uint8_t>& get_feature_vector() const;
};

#endif
