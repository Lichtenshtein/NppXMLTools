#include "SaxonCHelper.h"

SaxonCHelper* SaxonCHelper::instance = nullptr;
SaxonProcessor* SaxonCHelper::saxonProc = nullptr;
Xslt30Processor* SaxonCHelper::xslt30Processor = nullptr;

SaxonCHelper::SaxonCHelper()
{
}

SaxonCHelper::~SaxonCHelper()
{
	if (saxonProc != nullptr) {
		delete saxonProc;
	}
	if (xslt30Processor != nullptr) {
		delete xslt30Processor;
	}
	if (instance != nullptr) {
		delete instance;
	}
}

SaxonCHelper* SaxonCHelper::getInstance()
{
	if (instance == nullptr) {
		instance = new SaxonCHelper();
	}
	return instance;
}

SaxonProcessor* SaxonCHelper::getSaxonProcessor()
{
	if (saxonProc == nullptr) {
		saxonProc = new SaxonProcessor();
	}
	return saxonProc;
}

Xslt30Processor* SaxonCHelper::getXslt30Processor()
{
	return getSaxonProcessor()->newXslt30Processor();
}
