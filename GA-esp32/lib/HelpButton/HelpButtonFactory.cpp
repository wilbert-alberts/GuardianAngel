/*
 * HelpButtonFactory.cpp
 *
 *  Created on: 5 dec. 2021
 *      Author: wilbert
 */

#include <HelpButton.hpp>
#include <PeriodicTask.hpp>
#include <ValueProvider.hpp>
#include <memory>


namespace HelpButtonFactory {
std::shared_ptr<IButton> create(ValueProvider vp) {
	return std::shared_ptr<IButton>(new HelpButton(vp));
}

}

