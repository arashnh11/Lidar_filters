#include<string>

// This class will provide the body of different testcases for the purpose of evaluation
class testcase{
private:
	string name;
public:
	vector<vector<float> > data;
	vector<float> scan;
	vector<float> med_out;
	vector<float> range_out;
	testcase(string name):name(name){}
	string get_name(){
		return name;
	}
	void set_name(string input){
		name = input;
	}
	~testcase(){}
};

// This struct will provide the different test functions for the purpose of evaluation
struct Test{
	void check_FD(filter &L, testcase &T); // check the correctness of filtered range and temporal median
	void check_temp(filter &L, size_t D); // check the correctness of large temporal scans
	vector<testcase> Build_FD(); // generates a group of known testcases
	void print(vector<float> &scan); // print the scan measurements
	void rand_update(filter &L, size_t N, size_t D); // runs a sample of temporal scans and shows the filtered outputs
	void tests();
};

// check the correctness of filtered range and temporal median
void Test::check_FD(filter &L, testcase &T){
	if (L.med_update(T.data) == T.med_out &&
		L.range_update(T.scan) == T.range_out){
		cout << T.get_name() << ": Passed \n";
	}
	else
	{
		cerr << T.get_name() << ": Failed \n";
	}
}

// check the correctness of large temporal scans
void Test::check_temp(filter &L, size_t D){
	testcase* temp = new testcase(" ");
	temp->scan.resize(1e3, 0.1);
	temp->med_out = temp->scan;
	temp->range_out = temp->scan;
	for (size_t i = 0; i < D; i++){
		L.add_scan(temp->scan, temp->data);
		string str = "1000 measurements T";
		string index = to_string(i);
		str += index;
		temp->set_name(str);
		if (i == D - 1) check_FD(L, *temp);
	}
	delete temp;
}

// generates a group of known testcases
vector<testcase> Test::Build_FD(){
	filter L;
	vector<testcase> FD_scans;
	testcase triv0("Empty case");
	triv0.scan = {};
	triv0.med_out = {};
	triv0.range_out = {};
	L.add_scan(triv0.scan, triv0.data);

	testcase triv1("1e30 reading values");
	triv1.scan = {-1e30, 60, 1e30};
	triv1.med_out = {-1e30, 60, 1e30};
	triv1.range_out = {0.03, 50, 50};
	L.add_scan(triv1.scan, triv1.data);

	testcase triv2("1000 measurements per scan");
	triv2.scan.resize(1e3, 0.1);
	triv2.med_out = triv2.scan;
	triv2.range_out = triv2.scan;
	L.add_scan(triv2.scan, triv2.data);

	FD_scans.push_back(triv0);
	FD_scans.push_back(triv1);
	FD_scans.push_back(triv2);

	testcase T0("Problem example T0 update");
	T0.scan = {0., 1., 2., 1., 3.};
	T0.med_out = {0, 1., 2., 1., 3.};
	T0.range_out = {0.03, 1., 2., 1., 3.};

	testcase T1("Problem example T1 update");
	T1.scan = {1., 5., 7., 1., 3.};
	T1.med_out = {0.5, 3., 4.5, 1., 3.};
	T1.range_out = {1., 5., 7., 1., 3.};

	testcase T2("Problem example T2 update");
	T2.scan = {2., 3., 4., 1., 0.};
	T2.med_out = {1., 3., 4., 1., 3.};
	T2.range_out = {2., 3., 4., 1., 0.03};

	testcase T3("Problem example T3 update");
	T3.scan = {3., 3., 3., 1., 3.};
	T3.med_out = {1.5, 3., 3.5, 1., 3.};
	T3.range_out = {3., 3., 3., 1., 3.};

	testcase T4("Problem example T4 update");
	T4.scan = {10., 2., 4., 0., 0.};
	T4.med_out = {2., 3., 4., 1., 3.};
	T4.range_out = {10., 2., 4., 0.03, 0.03};

	for (size_t step = 0; step < 5; step++){
		if (step == 0) {
			L.add_scan(T0.scan, T0.data);
			FD_scans.push_back(T0);
		}
		else if (step == 1) {
			L.add_scan(T1.scan, T0.data);
			T1.data = T0.data;
			FD_scans.push_back(T1);
		}
		else if (step == 2) {
			L.add_scan(T2.scan, T1.data);
			T2.data = T1.data;
			FD_scans.push_back(T2);
		}
		else if (step == 3) {
			L.add_scan(T3.scan, T2.data);
			T3.data = T2.data;
			FD_scans.push_back(T3);
		}
		else if (step == 4) {
			L.add_scan(T4.scan, T3.data);
			T4.data = T3.data;
			FD_scans.push_back(T4);
		}
	}
	return FD_scans;
}

// print the scan measurements
void Test::print(vector<float> &scan){
	for (auto i: scan) cout << i << "\t";
	cout << endl;
	return;
}

// runs a sample of temporal scans and shows the filtered outputs
void Test::rand_update(filter &L, size_t N, size_t D){
	testcase* temp = new testcase(" ");
	string str = "Update filter N = ";
	str = str + to_string(N) + ", D = " + to_string(D);
	temp->set_name(str);
	cout << temp->get_name() << endl;
	for (size_t i = 0; i < D; i++){
		temp->scan = L.gen_scan(N);
		L.add_scan(temp->scan, temp->data);
		temp->med_out = L.med_update(temp->data);
		temp->range_out = L.range_update(temp->scan);
		cout << "T" << i << " scan: \t\t";
		print(temp->scan);
		cout << "T" << i << " Median update: \t";
		print(temp->med_out);
		cout << "T" << i << " Range update: \t";
		print(temp->range_out);
		cout << endl;
	}
	delete temp;
	return;
}
void Test::tests(){
	filter L;
	Test test;
	try {
		test.Build_FD();
	}
	catch(bad_alloc &e){
		cerr << "Error: Out of memory - Please reduce the size of the largest variable" << endl;
		return;
	}
	vector<testcase> FD_scans = test.Build_FD();
	for (size_t i = 0; i < FD_scans.size(); i++){
		test.check_FD(L, FD_scans[i]);
	}
	test.check_temp(L, 1000);
	test.rand_update(L, 10, 20);
	return;
}
