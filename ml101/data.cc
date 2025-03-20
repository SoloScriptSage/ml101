#include "data.hpp"

void Data::set_feature_vector(vector<uint8_t> *vect) {
	feature_vector = vect;
}

void Data::append_to_feature_vector(uint8_t val) {
	feature_vector->push_back(val);
}

void Data::set_label(uint8_t val) {
	label = val;
}

void Data::set_enum_label(int val) {
	enum_label = val;
}

int Data::get_feaure_vector_size() {
	return feature_vector->size();
}

uint8_t Data::get_label(int) {
	return label;
}

int Data::get_enum_label() {
	return enum_label;
}

vector<uint8_t>* Data::get_feature_vector() {
	return feature_vector;
}