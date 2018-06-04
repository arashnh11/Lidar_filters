#include "../src/filters.h"
#include "../Test/tests.h"
#include "../Docs/docs.h"
int main(){
	disc(); // Short summary of the software
	// Run all testcases
	Test CHECK;
	CHECK.tests();

	// Range filter
    filter L1; // define the filter
    vector<float> scan1 = {0., 4., 7., 9.}; // Current scan measurement
    vector<float> output1 = L1.range_update(scan1); // Range filtered scan

    // Temporal median filter
	filter L2;
    vector<float> scan2 = {10., 4., -7., 3.};
    vector<vector<float> > data; // Collected data up to current scan
	L2.add_scan(scan2, data); // Add new scan to the data
	vector<float> output2 = L2.med_update(data); // Temporal median of the current and previous scans
	return 0;
}
