#include "data_handler.hpp"



DataHandler::DataHandler() {
	data_array = new vector<Data*>();
	testing_data = new vector<Data*>();
	training_data = new vector<Data*>();
	validation_data = new vector<Data*>();
}
DataHandler::~DataHandler() {
	// FREE DYNAMIC MEMORY
}

void DataHandler::read_feature_vector(string path);
void DataHandler::read_feature_labels(string path);

void DataHandler::split_data();
void DataHandler::count_classes();

uint32_t DataHandler::count_to_little_endian(const unsigned char* bytes);

vector<Data*>* DataHandler::get_training_data();
vector<Data*>* DataHandler::get_test_data();
vector<Data*>* DataHandler::get_validation_data();