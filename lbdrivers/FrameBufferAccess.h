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

protected:
//	bool refreshInProgress = false;

public:
	//FrameBufferAccess(){}
	explicit FrameBufferAccess(uint32_t maxColumns, uint32_t maxRows, uint8_t *buffer)
	: FrameBuffer(maxColumns, maxRows, buffer) {}

	virtual ~FrameBufferAccess(){};

	virtual uint8_t * getBuffer()const { return FrameBuffer::getBuffer();}	// tylko przeslania widocznosc
	//virtual FrameBufferAccess * getFrameBufferAccess() { return this;}
	virtual bool isRefreshNeeded(){ return FrameBuffer::isRefreshNeeded(); }// tylko przeslania widocznosc
	virtual void inline refreshAcknowledge(){
		refreshNeed = false;		// wlasnie sie rozpoczyna resresh, wiec będzie odświerzone juz niebawem
//		refreshInProgress = true;	// jedziemy z pisaniem
	}

//	virtual void inline refreshStop(){
//		refreshInProgress = false;
//	}

//	virtual bool isRefershInProgress(){
//		return refreshInProgress;
//	}
//	virtual void refreshStop(){ FrameBuffer::refreshStop(); }
//	virtual void refreshStart(){ FrameBuffer::refreshAcknowledge(); }
	//virtual void setBackLight(bool enable) = 0;
	virtual bool isBackLightOn(){ return backLightIndicator; }
};

#endif /* FRAMEBUFFERACCESS_H_ */
