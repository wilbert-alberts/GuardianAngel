/*
 * HelpButton.hpp
 *
 *  Created on: 5 dec. 2021
 *      Author: wilbert
 */

#ifndef LIB_HELPBUTTON_HELPBUTTON_HPP_
#define LIB_HELPBUTTON_HELPBUTTON_HPP_

#include <Debouncer.hpp>
#include <IButton.hpp>
#include <ITicking.hpp>
#include <ValueProvider.hpp>
#include <memory>
#include <vector>

class PeriodicTask;

class HelpButton: public IButton, public ITicking {
public:
	HelpButton();
	virtual ~HelpButton();
	virtual PeriodicTask* createTask();
	virtual void addListener(std::shared_ptr<IBtnListener> listener);
	virtual void delListener(std::shared_ptr<IBtnListener> listener);
	virtual void tick();

private:
	void notifyListeners();

	std::vector<std::shared_ptr<IBtnListener>> listeners;
	Debouncer activityDebouncer;
};

namespace HelpButtonFactory {
std::shared_ptr<IButton> create();
}

#endif /* LIB_HELPBUTTON_HELPBUTTON_HPP_ */
