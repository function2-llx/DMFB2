#pragma once

struct DropletData {
	int id, fa_id = -1, peer_id = -1;
	int type;
	int mixingTime = 0;
	int detectingTime = 0;
    int output_sink = -1;
};
