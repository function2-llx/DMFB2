#pragma once

struct DropletData {
	int identifier;
	int type;
	int mixingTime;
	int detectingTime;
};

extern DropletData* dropletData;