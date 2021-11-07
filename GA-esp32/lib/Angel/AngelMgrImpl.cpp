/*
 * AngelMgrImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <Angel.hpp>
#include <AngelFactory.hpp>
#include <AngelMgr.hpp>
#include <GSM.hpp>
#include <Message.hpp>
#include <PeriodicTask.hpp>
#include <Config.hpp>
#include <LoadableAngel.hpp>
#include <SaveableAngel.hpp>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

class AngelMgrImpl: public AngelMgr, public PeriodicTask {
public:
	AngelMgrImpl();
	virtual ~AngelMgrImpl() {
	}

	virtual void setWatchDog(std::shared_ptr<WatchDog> wd);
	virtual void setHelpRequestDetector(std::shared_ptr<ActivityDetector> hr);
	virtual void setGSM(std::shared_ptr<GSM> gsm);

	virtual void setConfigProvider(std::shared_ptr<Config> cfg);


	virtual void tick();

	void processMessage(std::shared_ptr<Message> msg);
	void subscribeAngel(const std::string &phonenr, const std::string &start,
			const std::string &end, bool save);
	void unsubscribeAngel(const std::string &phonenr, const std::string &start,
			const std::string &end, bool save);

private:
	std::shared_ptr<WatchDog> wd;
	std::shared_ptr<ActivityDetector> hr;
	std::shared_ptr<GSM> gsm;
	std::shared_ptr<Config> cfg;

	std::vector<std::shared_ptr<Angel>> angels;

	void loadConfig();
	void saveConfig();
	void saveNrAngels();
	int loadNrAngels();
};

namespace AngelMgrFactory {
std::shared_ptr<AngelMgr> create() {
	return std::shared_ptr<AngelMgr>(new AngelMgrImpl());
}

}
AngelMgrImpl::AngelMgrImpl() :
		PeriodicTask("AngelMgr", 60 * 1000, 4000) {

}

void AngelMgrImpl::setWatchDog(std::shared_ptr<WatchDog> _wd) {
	wd = _wd;
}

void AngelMgrImpl::setHelpRequestDetector(
		std::shared_ptr<ActivityDetector> _hr) {
	hr = _hr;
}

void AngelMgrImpl::setGSM(std::shared_ptr<GSM> _gsm) {
	gsm = _gsm;
}

void AngelMgrImpl::tick() {
	auto msgs = gsm->getMessageIDs();
	std::for_each(msgs.begin(), msgs.end(), [&](MessageID mid) {
		auto msg = gsm->getMessage(mid);
		if (msg->isValid()) {
			processMessage(msg);
		}
		gsm->delMessage(msg->getMessageID());
	});
}

void AngelMgrImpl::processMessage(std::shared_ptr<Message> msg) {
	if (msg->getAction() == "subscribe") {
		subscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd(), true);
	}
	if (msg->getAction() == "unsubscribe") {
		unsubscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd(), true);
	}
}

void AngelMgrImpl::subscribeAngel(const std::string &phonenr,
		const std::string &start, const std::string &end, bool save) {
	auto angel = std::find_if(angels.begin(), angels.end(),
			[&](std::shared_ptr<Angel> a) {
				return a->getPhoneNr() == phonenr;
			});
	if (angel == angels.end()) {
		auto newAngel = AngelFactory::create(phonenr, wd, hr, gsm);
		angels.push_back(newAngel);
		angel = angels.end();
		angel--;
	}

	(*angel)->addInterval(start, end);
	if (save)
	saveConfig();
}


void AngelMgrImpl::unsubscribeAngel(const std::string &phonenr,
		const std::string &start, const std::string &end, bool save) {
	auto angel = std::find_if(angels.begin(), angels.end(),
			[&](std::shared_ptr<Angel> a) {
				return a->getPhoneNr() == phonenr;
			});
	if (angel != angels.end()) {
		if (start != "" || end != "")
			(*angel)->delInterval(start, end);
		else
			angels.erase(angel);
	}
	if(save)
	saveConfig();
}

void AngelMgrImpl::setConfigProvider(
		std::shared_ptr<Config> _cfg) {
	cfg = _cfg;
	loadConfig();
}

void AngelMgrImpl::loadConfig() {
	// TODO: check how to deal with angels that managed to subscribe before
	// loading the config.
	if (cfg != nullptr) {
		int nrAngels = loadNrAngels();
		for (auto idx=0; idx<nrAngels; idx++) {
			LoadableAngel loadAngel(cfg, idx);
			int nrIntervals = loadAngel.getNrIntervals();
			for (int iv=0; iv< nrIntervals; iv++) {
				subscribeAngel(loadAngel.getPhoneNr(), loadAngel.getStart(iv), loadAngel.getEnd(iv), false);
			}
		}
	}
}

void AngelMgrImpl::saveConfig() {
	if (cfg != nullptr) {
		saveNrAngels();
		for (size_t idx=0; idx<angels.size(); idx++) {
			SaveableAngel saveAngel(angels[idx], cfg, idx);
			saveAngel.save();
		}
	}
}

void AngelMgrImpl::saveNrAngels() {
	cfg->putProperty("nrAngels", std::to_string(angels.size()));
}

int AngelMgrImpl::loadNrAngels() {
	return std::stoi(*cfg->getProperty("nrAngels"));
}
