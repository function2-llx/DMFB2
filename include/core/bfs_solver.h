#include "core/DMFB.h"

class BfsSolver : public DMFB {
  public:
    std::vector<const State*> get_route(const State* init) const;
};