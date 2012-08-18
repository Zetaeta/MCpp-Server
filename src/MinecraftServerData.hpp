/*
 * MinecraftServerData.hpp
 *
 *  Created on: 4 Aug 2012
 *      Author: daniel
 */

#ifndef MINECRAFTSERVERDATA_HPP_
#define MINECRAFTSERVERDATA_HPP_

#include <thread>
#include "network/NetworkServer.hpp"

struct MinecraftServerData {
    NetworkServer *networkServer;
    bool shutdown;

    MinecraftServerData() 
        :shutdown(false) {}
};


#endif /* MINECRAFTSERVERDATA_HPP_ */
