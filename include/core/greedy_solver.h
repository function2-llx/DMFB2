#include "core/DMFB.h"

class GreedySolver : public DMFB {
  

  public:
    virtual std::vector<const State*> get_route(const State*) const override;
};