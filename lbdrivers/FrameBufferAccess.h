/*
 * FrameBufferAccess.h
 *
 *  Created on: 24 paź 2019
 *      Author: lechu
 */

#ifndef FRAMEBUFFERACCESS_H_
#define FRAMEBUFFERACCESS_H_

#include "FrameBuffer.h"

class FrameBufferAccess: public FrameBuffer {

public:
	//FrameBufferAccess(){}
	explicit FrameBufferAccess(uint32_t maxColumns, uint32_t maxRows, uint8_t *buffer)
	: FrameBuffer(maxColumns, maxRows, buffer) {}

	virtual ~FrameBufferAccess(){};

	virtual uint8_t * getBuffer()const { return FrameBuffer::getBuffer();}	// tylko przeslania widocznosc
	//virtual FrameBufferAccess * getFrameBufferAccess() { return this;}
	virtual bool isRefreshNeeded(){ return FrameBuffer::isRefreshNeeded(); }// tylko przeslania widocznosc
	virtual void refreshStop(){ FrameBuffer::refreshStop(); }
	virtual void refreshStart(){ FrameBuffer::refreshStart(); }
};

#endif /* FRAMEBUFFERACCESS_H_ */
