#include <forward_list>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

class Cargo {
protected:
  string uldtype;
  string abbrev;
  string uldid;
  int aircraft;
  double weight;
  string dest;

public:
  /// Default constructor
  Cargo() {
    cout << "Default Cargo data" << endl;
    uldtype = "Pallet";
    abbrev = "MJK";
    uldid = "JKJ12345LO";
    aircraft = 700;
    weight = 1200.0;
    dest = "SJC";
  }

  /// Full constructor
  Cargo(const string uldtype, const string abbrev, const string uldid,
        const int aircraft, const int weight, const string dest) {
    this->uldtype = uldtype;
    this->abbrev = abbrev;
    this->uldid = uldid;
    this->aircraft = aircraft;
    this->weight = weight;
    this->dest = dest;
  }

  // Copy constructor
  Cargo(const Cargo &data) {
    cout << "Copy constructor called" << endl;
    uldtype = data.uldtype;
    abbrev = data.abbrev;
    uldid = data.uldid;
    aircraft = data.aircraft;
    weight = data.weight;
    dest = data.dest;
  }

  /// Destructor
  virtual ~Cargo() {}
  // SETTERS/ MUTATORS
  void setuldtype(string uldtype1) { uldtype = uldtype1; }
  void setabbrev(string abbrev1) { abbrev = abbrev1; }
  void setuldid(string uldid1) { uldid = uldid1; }
  void setaircraft(int aircraft1) { aircraft = aircraft1; }
  void setweight(double weight1) { weight = weight1; }
  void setdestination(string dest1) { dest = dest1; }

  // GETTERS/ ACCESSORS
  string getuldtype() const { return uldtype; }
  string getabbrev() const { return abbrev; }
  string getuldid() const { return uldid; }
  int getaircraft() const { return aircraft; }
  double getweight() const { return weight; }
  string getdestination() const { return dest; }

  // Pure virtual function to be overridden in child classes
  virtual void maxWeight(double &, double, string) = 0;
};

// BOEING737 CLASS
class Boeing737 : public Cargo {

public:
  static const double maxLoad737;
  // Constructors
  Boeing737() : Cargo("", "", "", 737, 0, "") {}
  Boeing737(const string &uldtype, const string &abbrev, const string &uldid,
            const int craft, const double weight, const string &dest) {
    this->uldtype = uldtype;
    this->abbrev = abbrev;
    this->uldid = uldid;
    this->aircraft = craft;
    this->weight = weight;
    this->dest = dest;
  }

  void maxWeight(double &, double, string) override;
};
const double Boeing737::maxLoad737 = 46000.0;

// MAX WEIGHT CHECK 737
void Boeing737::maxWeight(double &totalwt737, double wt, string id) {
  totalwt737 += wt;
  if (totalwt737 > Boeing737::maxLoad737) {
    cout << "737 weight trigger\n";
    totalwt737 -= wt;
    throw runtime_error(id + " too heavy for 737, removed\n\n");
  }
}

// LOAD 737
void load737(forward_list<Boeing737> &cargoList737, const string type,
             const string abrv, const string id, const int craft,
             const double wt, const string dest, double &totwt737) {
  Boeing737 unitObj737(type, abrv, id, craft, wt, dest);
  unitObj737.maxWeight(totwt737, wt, id);
  cargoList737.push_front(unitObj737); // push front valid 737 obj to ll
}

// BOEING767 CLASS
class Boeing767 : public Cargo {
public:
  static const double maxLoad767;
  static unordered_set<string> existingUldids; // Store existing uldids
  // Constructors
  Boeing767() : Cargo("", "", "", 767, 0, "") {}
  Boeing767(const string &uldtype, const string &abbrev, const string &uldid,
            const int craft, const double weight, const string &dest)
      : Cargo(uldtype, abbrev, uldid, craft, weight, dest) {}

  void maxWeight(double &, double, string) override;
};
const double Boeing767::maxLoad767 = 116000.0;

// CHECK MAX WEIGHT 767
void Boeing767::maxWeight(double &totalwt767, double wt, string id) {
  totalwt767 += wt;
  if (totalwt767 > Boeing767::maxLoad767) {
    cout << "767 weight trigger\n";
    totalwt767 -= wt;
    throw runtime_error(id + " too heavy for 767, removed\n\n");
  }
}
unordered_set<string> Boeing767::existingUldids;

// LOAD 767
void load767(forward_list<Boeing767> &cargoList767, const string type,
             const string abrv, const string id, const int craft,
             const double wt, const string dest, double &totwt767) {

  // Check if uldid already exists in existingUldids
  if (Boeing767::existingUldids.find(id) != Boeing767::existingUldids.end()) {
    cout << "Unit with uldid " << id << " already exists. Skipping." << endl;
    return;
  }

  Boeing767 unitObj767(type, abrv, id, craft, wt, dest);
  unitObj767.maxWeight(totwt767, wt, id);
  cargoList767.push_front(unitObj767); // push front valid 767 obj to ll

  // Add the uldid to the existingUldids set
  Boeing767::existingUldids.insert(id);
}
void checkPlane(int plane) {
  if (!(plane == 737 || plane == 767)) {
    throw runtime_error(to_string(plane) + " bad airplane type\n\n");
  }
}
void checkType(string type) {
  if (!(type == "Container" || type == "Pallet")) {
    throw runtime_error(type + " not Container or Pallet, rejected load\n\n");
  }
}
void checkAbbrev(int air, string abbrev) {
  if (air == 737) {
    if (!(abbrev == "AYF" || abbrev == "AYK" || abbrev == "AAA" ||
          abbrev == "AYY" || abbrev == "PAG" || abbrev == "PMC" ||
          abbrev == "PLA")) {
      throw runtime_error(abbrev + " bad type for 737\n\n");
    }
  }
  if (air == 767) {
    if (!(abbrev == "AKE" || abbrev == "APE" || abbrev == "AKC" ||
          abbrev == "AQP" || abbrev == "AQF" || abbrev == "AAP" ||
          abbrev == "P1P" || abbrev == "P6P")) {
      throw runtime_error(abbrev + " bad type for 767\n\n");
    }
  }
}
// Input function
void input(forward_list<Boeing737> &cList737,
           forward_list<Boeing767> &cList767) {

  string uldtype, abbrev, uldid, dest;
  int aircraft;
  double weight;

  ifstream inputFile;
  double totwt737 = 0;
  double totwt767 = 0;

  inputFile.open("lab6/lab6data.txt");
  if (!inputFile.is_open()) {
    cerr << "Could not open file lab6.txt." << endl;
    exit(EXIT_FAILURE);
  }

  while (inputFile.peek() != EOF) {
    while (inputFile.peek() == ' ') {
      inputFile.get(); // Read and ignore leading whitespace
    }
    try {
      inputFile >> uldtype >> abbrev >> uldid >> aircraft >> weight >> dest;
      checkType(uldtype);
      checkPlane(aircraft);

      if (aircraft == 737) {
        checkAbbrev(aircraft, abbrev);
        load737(cList737, uldtype, abbrev, uldid, aircraft, weight, dest,
                totwt737);
      }
      if (aircraft == 767) {
        checkAbbrev(aircraft, abbrev);
        load767(cList767, uldtype, abbrev, uldid, aircraft, weight, dest,
                totwt767);
      }
    } // end of try
    catch (runtime_error &excpt) {
      cout << excpt.what();
    }
  } // end of while
  inputFile.close();
  cout << "\n\nplane 737 total weight: " << totwt737 << " pounds" << endl;
  cout << "plane 767 total weight: " << totwt767 << " pounds"
       << "\n\n"
       << endl;
}

void output(forward_list<Boeing737> &cargoList737,
            forward_list<Boeing767> &cargoList767) {
  cargoList737.reverse();
  cargoList767.reverse();
  
  cout << setw(30) << left << "Boeing 737 Units:" << endl;
  cout << setw(20) << left << "Unit Number" << setw(20) << left
       << "Unit Load Type" << setw(25) << left << "Unit Load Abbreviation"
       << setw(20) << left << "Unit Identifier" << setw(20) << left
       << "Aircraft Type" << setw(25) << left << "Unit Weight" << setw(20)
       << left << "Destination Code" << endl;
  cout
      << "---------------------------------------------------------------------"
         "--------------------------------------------------------------------"
      << endl;

  int unitNumber737 = 1;
  for (const auto &unit : cargoList737) {
    cout << setw(20) << left << unitNumber737++ << setw(20) << left
         << unit.getuldtype() << setw(25) << left << unit.getabbrev()
         << setw(20) << left << unit.getuldid() << setw(20) << left
         << unit.getaircraft() << setw(25) << fixed << setprecision(2)
         << unit.getweight() << " pounds" << setw(20) << left
         << unit.getdestination() << endl;
  }

  cout << endl;
  cout << setw(30) << left << "Boeing 767 Units:" << endl;
  cout << setw(20) << left << "Unit Number" << setw(20) << left
       << "Unit Load Type" << setw(25) << left << "Unit Load Abbreviation"
       << setw(20) << left << "Unit Identifier" << setw(20) << left
       << "Aircraft Type" << setw(25) << left << "Unit Weight" << setw(20)
       << left << "Destination Code" << endl;
  cout
      << "---------------------------------------------------------------------"
         "--------------------------------------------------------------------"
      << endl;

  int unitNumber767 = 1;
  for (const auto &unit : cargoList767) {
    cout << setw(20) << left << unitNumber767++ << setw(20) << left
         << unit.getuldtype() << setw(25) << left << unit.getabbrev()
         << setw(20) << left << unit.getuldid() << setw(20) << left
         << unit.getaircraft() << setw(25) << fixed << setprecision(2)
         << unit.getweight() << " pounds" << setw(20) << left
         << unit.getdestination() << endl;
  }
}

// Main function
int main() {
  forward_list<Boeing737> cargoList737;
  forward_list<Boeing767> cargoList767;

  input(cargoList737, cargoList767);
  output(cargoList737, cargoList767);

  return 0;
}
