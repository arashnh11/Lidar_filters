# Lidar_filters
LIDAR Filter
========

LIDAR Filter will provide you with useful functions to reduce noise in the data coming from a LIDAR sensor
attached to your robot. LIDAR filter provides two filters entitled as range and temporal median.

To use this software:

    To use range filter, you should define an array of sensor measurements as a one-dimensional vector.
    Example:
    
    // Range filter
    filter L1; // define the filter
    vector<float> scan1 = {0., 4., 7., 9.}; // Current scan measurement
    vector<float> output1 = L1.range_update(scan1); // Range filtered scan

    To use temporal median filter, you should define an array of scan measurments as a vector
    and the two-dimensional accumulated data up to current scan.
    Example:
	
    // Temporal median filter
	filter L2;
    vector<float> scan2 = {10., 4., -7., 3.};
    vector<vector<float> > data; // Collected data up to current scan
	L2.add_scan(scan2, data); // Add new scan to the data
	vector<float> output2 = L2.med_update(data); // Temporal median of the current and previous scans

Tests
--------
Testcases are located inside the Test directory which includes:
	-- Empty scan
	-- Problem sample 
	-- Scans with very large values up to 1e30
	-- Large number of measurements up to 1000
	-- Large number of scans (1000) with large number of measurements (1000)
	-- Update with 20 number of random scans each with 10 measurements

To run all testcases, simply compile and execute run.cpp located inside the Run directory.
	
Installation
--------
    Copy filters.h from src directory from Test directory file located inside src folder into your main running code. Then add the file as a header:
    #include "filters.h"
