/**
 * original: 07/04/2018
 * remade: 19/04/2018
 * @author Nicolas
 *
 */
#ifndef RANDOMGEN_H_
#define RANDOMGEN_H_

#include "sha256/sha256.h"
#include "deque.h"
#include "transaction.h"
#include "util.h"



Transaction *random_trans();

//Transaction *random_tl();

Block *random_block();

Blockchain *random_blockchain();
