/*
 *  hdIBContainer.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef _HD_IB_CONTAINER_H_
#define _HD_IB_CONTAINER_H_

#include "hdIBProxy.h"

class hdIBContainer : public hdIBProxy
{
private:
	hdTypedefList<hdIBProxy *, 32>* m_proxyItems;
};


#endif