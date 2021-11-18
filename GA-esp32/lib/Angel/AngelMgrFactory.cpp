/*
 * AngelMgrFactory.cpp
 *
 *  Created on: 17 nov. 2021
 *      Author: wilbert
 */

#include <memory>

#include "AngelMgrImpl.hpp"
#include "AngelMgrTaskImpl.hpp"

class AngelMgr;

namespace AngelMgrFactory {
std::shared_ptr<AngelMgr> create() {
	return std::shared_ptr<AngelMgr>(new AngelMgrImpl());
}
std::shared_ptr<AngelMgr> createTask() {
	return std::shared_ptr<AngelMgr>(new AngelMgrTaskImpl());

}

}

