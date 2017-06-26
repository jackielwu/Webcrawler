
#include "webcrawler.h"

// Add your implementation here
WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots)
{
	_maxUrls = maxUrls;
	_urlArray = new URLRecord*[_maxUrls];
	
}

void WebCrawler::crawl()
{
	while(_headURL < _tailURL)
	{
		//Fetach the next URL is _headURL
		_headURL++;
		//If doc is not txt/html
		if(!)
		{
			;
		}
		//Get first 500 char of doc w/o tags
		//Add to description to URLRecord

		//Find all hyperlinks of doc and add them to _urlArray and _urlToUrlRecord if not already to _urlToUrlRecord
		//Only insert up to _maxURL entries
		
		for(word: doc)
		{
      //Add index of URL to URLRecordList in _wordToURLRecordList table
      //if the URL is not already there		
		}
	}
}

void WebCrawler::writeURLFile(const char * urlFileName)
{

}
void WebCrawler::writeWordFile(const char *wordFilename)
{

}
