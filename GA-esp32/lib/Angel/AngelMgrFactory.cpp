/*
 * AngelMgrFactory.cpp
 *
 *  Created on: 17 nov. 2021
 *      Author: wilbert
 */

#include <AngelMgrImpl.hpp>
#include <memory>

class AngelMgr;

namespace AngelMgrFactory {

std::shared_ptr<AngelMgr> create() {
	return std::shared_ptr<AngelMgr>(new AngelMgrImpl());
}


}

