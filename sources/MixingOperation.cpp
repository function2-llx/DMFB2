#include <cassert>
#include "MixingOperation.h"

using namespace std;

MixingOperation::MixingOperation() {}

istream& operator >> (istream& is, MixingOperation& mixingOperation)
{
	is >> mixingOperation.rows >> mixingOperation.columns >> mixingOperation.duration;
	for (int i = 0; i < 2; i++) {
		is >> mixingOperation.inputIdentifier[i];
		mixingOperation.inputIdentifier[i]--;
	}
	int i1 = mixingOperation.inputIdentifier[0], i2 = mixingOperation.inputIdentifier[1];
	mixPair[i1][i2] = mixPair[i2][i1] = true;
	mixingData[i1][i2] = mixingData[i2][i1] = mixingOperation;
	return is;
}

ostream& operator << (ostream& os, const MixingOperation& mixingOperation)
{
    os << "mixing operation" << endl;
	os << "corresponding mixer identifier: " << mixingOperation.mixerIdentifer << endl;
    return os;
}

bool MixingOperation::match(int identifier1, int identifier2)
{
    assert(this->inputIdentifier[0] < this->inputIdentifier[1]);
    if (identifier1 > identifier2) swap(identifier1, identifier2);
    return this->inputIdentifier[0] == identifier1 && this->inputIdentifier[1] == identifier2;
}

vector<MixingOperation*> allMixingOperations;
bool* toBeMixed;
bool** mixPair;
MixingOperation** mixingData;