/*
 * GSMFactory.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_GSM_GSMFACTORY_HPP_
#define LIB_GSM_GSMFACTORY_HPP_

#include <memory>

class GSM;

namespace GSMFactory {
std::shared_ptr<GSM> create();
}

#endif /* LIB_GSM_GSMFACTORY_HPP_ */
