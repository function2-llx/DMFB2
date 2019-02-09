#include "Check.h"
#include "Global.h"
#include <set>
#include <map>

int check(State* curState, WashState* curwash){
	std::vector<const Droplet*> curdroplet = curState->getDroplets();
	std::vector<const Droplet*> predroplet = curState->decision->getDroplets();
	std::vector<const Washer*> curWashers = curwash->getWashers();
	std::vector<const Washer*> preWashers = curwash->decision->getWashers();
	
	std::vector<const Droplet*>* drops[2] = {&curdroplet, &predroplet};
	std::vector<const Washer*>* washers[2] = {&curWashers, &preWashers};
	
	
	{//check droplet & droplet
		std::set<int> indentifiers;
		std::map<int, Point> position[2];
		for(int i = 0; i < 2; i++)
			for(auto di = drops[i]->begin(); di != drops[i]->end(); di++){
				indentifiers.insert((*di)->getIdentifier());
				position[i][(*di)->getIdentifier()] = (*di)->getPosition();
			}
		//check moving
		
		for(auto i = indentifiers.begin(); i !=indentifiers.end(); i++){
			auto curPosition = position[0].find(*i), prePosition = position[1].find(*i);
			if(curPosition == position[0].end() || prePosition == position[1].end()){
				continue;
			} else if(absSum(curPosition->second - prePosition->second) > 1) {
				return 1;
			}
		}
		
		//check fluidic constrains
		for(auto i = indentifiers.begin(); i !=indentifiers.end(); i++)
			for(auto j = indentifiers.begin(); j != indentifiers.end(); j++)
				if(*i!=*j && Global::mixingResult[*i][*j] != -1){
					auto pre_i = position[0].find(*i), cur_i = position[1].find(*i);
					auto pre_j = position[0].find(*j), cur_j = position[1].find(*j);
					
					int exist_i = 0, exist_j = 0;
					if(pre_i != position[0].end())
						exist_i |= 1;
					if(cur_i != position[1].end())
						exist_i |= 2;
					if(pre_j != position[0].end())
						exist_j |= 1;
					if(cur_j != position[1].end())
						exist_j |= 2;
					
					if(exist_i != exist_j){//they don't exist at the same time
						continue;
					} else {//a fool method
						if(exist_i & 1){
							if( (exist_j & 1) && adjacent(pre_i->second, pre_j->second) ){
								return 2;
							}
							if( (exist_j & 2) && adjacent(pre_i->second, cur_j->second) ){
								return 2;
							}
						}
						if(exist_i & 2){
							if( (exist_j & 1) && adjacent(cur_i->second, pre_j->second) ){
								return 2;
							}
							if( (exist_j & 2) && adjacent(cur_i->second, cur_j->second) ){
								return 2;
							}
						}
					}
					
				}
		
	}
	
	{//check droplet & washers
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 2;j++)
				for(auto di = drops[i]->begin(); di != drops[i]->end(); di++)
					for(auto wi = washers[j]->begin(); wi != washers[j]->end(); wi++)
						if(adjacent((*di)->getPosition(), (*wi)->getPosition()))
							return 3;
	}
	
	/*{//check washers & washers
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 2; j++){
				assert(washers[i].size == washers[j].size());
				int size = washers[i].size();
				for(int wi = 0; wi < size; wi++)
					for(int wj = 0; wj < size; wj++)
						if(wi != wj && adjacent(washers[i][wi].getPoint(), washers[j][wj].getPoint()))
							return false;
			}
	}*/ //they didn't give every washer a ID, so let it go :)
	
	return 0;
	
}
