
#ifndef SIMPLEHTMLPARSER_H
#define SIMPLEHTMLPARSER_H

#include "openhttp.h"

class SimpleHTMLParser {
	// URL read in anchor

	int urlAnchorLength;
	char urlAnchor[MaxURLLength];
	
 public:
	SimpleHTMLParser();
	bool match(char **b, const char *m);
	
	bool parse(char * buffer, int n);

	// Calbacks
	virtual void onContentFound(char c);
	virtual void onAnchorFound(char * url);
};

#endif

