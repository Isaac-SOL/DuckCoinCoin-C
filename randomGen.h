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
#include "blockchain.h"
#include "util.h"

/**
 * creation transaction aleatoire
 * @return une transaction aleatoire
 */
Transaction random_trans();

//Transaction *random_tl();
/**
 * creation block aleatoire
 * @return un bloc aleatoire
 */
Block *random_block();

/**
 * creation transaction aleatoire
 * @return une blockchain aleatoire
 */
Blockchain *random_blockchain();

#endif /* RANDOMGEN_H_ */
