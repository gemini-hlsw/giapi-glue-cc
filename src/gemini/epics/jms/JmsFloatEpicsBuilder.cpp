/*
 * JmsFloatEpicsBuilder.cpp
 *
 *  Created on: Mar 31, 2009
 *      Author: anunez
 */

#include "JmsFloatEpicsBuilder.h"
#include <gemini/epics/EpicsStatusItemImpl.h>

namespace giapi {

JmsFloatEpicsBuilder::JmsFloatEpicsBuilder(BytesMessage *bm) :
	JmsEpicsBuilder(bm) {

}

JmsFloatEpicsBuilder::~JmsFloatEpicsBuilder() {

}

pEpicsStatusItem JmsFloatEpicsBuilder::getEpicsStatusItem() {

	int size = _nElements * sizeof(float);
	char * data = (char *) malloc(size);
	for (int i = 0; i < _nElements; i++) {
		float val = _message->readFloat();
		memcpy(data + i * sizeof(float), &val, sizeof(float));
	}
	pEpicsStatusItem item = EpicsStatusItemImpl::create(_name, type::FLOAT,
			_nElements, data, size);
	//destroy the local memory
	delete data;
	return item;
}

}