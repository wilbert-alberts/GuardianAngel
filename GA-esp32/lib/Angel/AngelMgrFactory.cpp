/*
 * AngelMgrFactory.cpp
 *
 *  Created on: 17 nov. 2021
 *      Author: wilbert
 */

#include <AngelMgrImpl.hpp>
#include <PeriodicTask.hpp>
#include <memory>

class AngelMgr;

namespace AngelMgrFactory {

std::shared_ptr<AngelMgr> create() {
	return std::shared_ptr<AngelMgr>(new AngelMgrImpl());
}

PeriodicTask* createTask() {
	auto am = std::shared_ptr<AngelMgrImpl>(new AngelMgrImpl());
	auto r = new PeriodicTask("AngelMgr", am, 1000, 4000);
	return r;

}

}

