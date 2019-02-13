#ifndef DFMB_H
#define DFMB_H

#include <set>
#include <map>
#include "core/State.h"

class DMFB {
  private:
		std::map<int, int> typeMap;
		int nDroplets, nSinks, nTypes, nDispensers;
		int rows, columns;
		int* boundary[4];
		int** detector;

		DMFB(const DMFB&);
		DMFB& operator = (const DMFB&);
		
		void print(std::ostream&, int);
		void placeSink(int sinkCount);
		void placeDispenser(int dispenserCount);
		void placeDetector(int detectorCount);
		bool dfs(const State*);

	public:
		DMFB();
		~DMFB();

		void loadSequencingGraph();
		void loadModuleLibrary();
		void loadDesignObejective();

		int getDropletNumber() const;

		void solve();
		void printPlace(std::ostream&);

		void set_placement();

		std::vector<const State*> get_route(const State* state) const;
};

extern int *leastTime;
extern DMFB *DMFBsolver;
extern std::vector<int> type;

#endif	//DFMB_H