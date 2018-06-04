#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
using namespace std;
typedef unsigned int size_t;

// This class will provide a group of filters for LIDAR measurements

class filter{
private:
	float min = 0.03;
	float max = 50.0;
public:
	filter(){}
	vector<float> range_update(vector<float> &scan); // Range filter
	vector<float> med_update(vector<vector<float> > &data); // Temporal median filter
	vector<float> gen_scan(size_t N); // Random scan generator
	void add_scan(vector<float> &scan,
									 vector< vector<float> > &data); // Scans accumulation
	float median(vector<float> &input); // Find median in a sorted array
	~filter(){}
};

// This function will provide a filtered scan based on the given minimum maximum value for measurements
vector<float> filter::range_update(vector<float> &scan){
	float min = 0.03;
	float max = 50.;
	vector<float> range_out(scan.size(), min);
	for (size_t i = 0; i < scan.size(); i++){
		if (scan[i] < min) range_out[i] = min;
		else if (scan[i] > max) range_out[i] = max;
		else range_out[i] = scan[i];
	}
	return range_out;
}

// This function will calculate the median of a sorted array
float filter::median(vector<float> &input){
	unsigned int N = input.size();
	if (N % 2 == 0){
		return 0.5 * (input[N/2 - 1] + input[N/2]);
	}
	else {
		return input[N/2];
	}
}

// This function will provide a filtered scan based on the median of current and previous measurements
vector<float> filter::med_update(vector<vector<float> > &data){
	size_t N = data.size();
	vector<float> med_out(N, 0);
	for (size_t i = 0; i < N; i++){
		med_out[i] = median(data[i]);
	}
	return med_out;
}

// This function will generate random scan measurements for the test purposes
vector<float> filter::gen_scan(size_t N){
	vector<float> input(N, 0);
	for (size_t i = 0; i < N; i++){
		input[i] = rand() % 10;
	}
	return input;
}

// This function will accumulate current scan to the previous scans
void filter::add_scan(vector<float> &scan, vector< vector<float> > &data){
	size_t N = scan.size();
	size_t D = data.size();
	if (D == 0){
		for (size_t i = 0; i < N; i++){
			data.push_back(vector<float>());
			data[i].push_back(scan[i]);
		}
	}
	else {
		for (size_t i = 0; i < N; i++){
			vector<float>::iterator it = lower_bound(data[i].begin(), data[i].end(), scan[i]);
			data[i].insert(it,scan[i]);
		}
	}
	return;
}
