#pragma once

#include "saxonc/XdmItem.h"
class SaxonCHelper
{
public:

	static SaxonCHelper* getInstance();

	static SaxonProcessor* getSaxonProcessor();

	Xslt30Processor* getXslt30Processor();

protected:
	SaxonCHelper();
	~SaxonCHelper();

	static SaxonCHelper* instance;

	static SaxonProcessor* saxonProc;

	static Xslt30Processor* xslt30Processor;
};
