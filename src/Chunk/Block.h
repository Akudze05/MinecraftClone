//
// Created by Asg on 15.06.2025.
//

#ifndef BLOCK_H
#define BLOCK_H

/*
 * List block types
 * 1 - stone
 * 2 - dirt
 * 3 - grass
 */

class Block {
public:
	int blocktype; // type block
	float x, y, z; // position in chunk
	float wx, wy, wz; // position in world
};



#endif //BLOCK_H
