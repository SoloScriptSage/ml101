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

void DataHandler::read_feature_vector(string path) {
	uint32_t header[4]; // MAGIC | NUM IMAGES| ROWSIZE | COLSIZE
	unsigned char bytes[4];
	
	FILE* file = fopen(path.c_str(), "rb");
	if (file) {
		for (int i = 0; i < 4; ++i) {
			if (fread(bytes, sizeof(bytes), 1, file)) {
				header[i] = count_to_little_endian(bytes);
			}
		}
		printf("Done getting file header\n");

		int image_size = header[2] * header[3];
		for (int i = 0; i < header[1]; ++i) {
			Data* data = new Data();
			uint8_t pixel;
			for (int j = 0; j < image_size; ++j) {
				if (fread(&pixel, sizeof(pixel), 1, file)) {
					data->append_to_feature_vector(pixel);
				}
				else {
					printf("Error reading pixel\n");
					exit(1);
				}
			}
			data_array->push_back(data);
		}
		printf("Successfully read and stored %lu feature vectors\n", data_array->size());
	}
	else {
		printf("Error opening file\n");
		exit(1);
	}
}

void DataHandler::read_feature_labels(string path) {

}

void DataHandler::split_data();
void DataHandler::count_classes();

uint32_t DataHandler::count_to_little_endian(const unsigned char* bytes);

vector<Data*>* DataHandler::get_training_data();
vector<Data*>* DataHandler::get_test_data();
vector<Data*>* DataHandler::get_validation_data();