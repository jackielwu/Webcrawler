
#include "webcrawler.h"

// Add your implementation here
WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots)
{
	_maxUrls = maxUrls;
	_urlArray = new URLRecord[_maxUrls];
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList *>();
	_headURL = 0;
	_tailURL = 0;
	for(int i=0;i<nurlRoots;i++)
	{
	  URLRecord *e = new URLRecord;
	  e->_url = (char *) urlRoots[i];
	  e->_description = new char[500];
	  //e->_i = 0;
	  _urlArray[_tailURL++] = *e;
	}
}

void WebCrawler::crawl()
{
	while(_headURL < _tailURL)
	{
		printf("crawl\n");
		//Fetch the next URL is _headURL
		int n;
		char * buffer = fetchHTML(_urlArray[_headURL]._url, &n);
		_headURL++;
		//If doc is not txt/html
		if(buffer == NULL)
		{
			continue;
		}
		//Get first 500 char of doc w/o tags
		//Add to description to URLRecord
		parse(buffer, n);
		
		//Find all hyperlinks of doc and add them to _urlArray and _urlToUrlRecord if not already to _urlToUrlRecord
		//Only insert up to _maxURL entries
		
		//for(word: doc)
		{
      //Add index of URL to URLRecordList in _wordToURLRecordList table
      //if the URL is not already there		
		}
	}
}

void WebCrawler::writeURLFile(const char * urlFileName)
{
	printf("%d\n", _tailURL);
	ofstream file;
	file.open(urlFileName);
	//Iterator
	if(file.is_open()) {
		for (int i=0;i<_tailURL;i++) {
		  file << i << " " << _urlArray[i]._url << "\n";
		  file << _urlArray[i]._description << "\n";
		  file << "\n"; 
		}
		file.close();
	}
	else cout << "Unable to open file\n";
	return;
}
void WebCrawler::writeWordFile(const char *wordFileName)
{
	ofstream file;
	file.open(wordFileName);
	//Iterator
	if(file.is_open()) {
		HashTableTemplateIterator<URLRecordList *> iterator(_wordToURLRecordList);
		const char * key;
		URLRecordList *e;
		while (iterator.next(key, e)) {
		  file << key;
		  while(e->_next != NULL) {
		  	file << " " << e->_urlRecordIndex;
			}
			file << "\n";
		}
		file.close();
	}
	else cout << "Unable to open file\n";
	return;
}

void WebCrawler::onContentFound(char c) {
  //default gethttp
  //printf("%c", c);
  /*if(_urlArray[_headURL]._i <500) {
    _urlArray[_headURL]._description[_urlArray[_headURL]._i++] = c;
  }*/
  if(temp.length() < 500) {
  	temp += c;
  }
  else {
  	printf("%s\n", temp.c_str());
  	_urlArray[_headURL]._description = strdup(temp.c_str());
  	temp.clear();
  }
  //findWord(c);
}

void WebCrawler::onAnchorFound(char * url)
{
	printf("%s\n", url);
  if(_tailURL < _maxUrls)
  {
  	if(url[0] != '#')
  	{
  		string u(url);
  		if(u.find("http") == 0) {
  			_urlArray[_tailURL]._url = strdup(url);
  			_urlToUrlRecord->insertItem((const char*)url, _tailURL);
  			_tailURL++;
  		}
  		/*else if(u.find("http") == string::npos) {
  			u.insert(0, "/");//handle separation for relative links
  			u.insert(0, _urlArray[_headURL]._url);
  			_urlArray[_tailURL++]._url = (char *)u.c_str();
  			printf("%s\n", _urlArray[_tailURL-1]._url);
  		}*/
  	}
	}
}

void WebCrawler::findWord(char c)
{
   
  if(c != ' ') {
    //put in string
    temp += c;
  }
  else {
    //put string into word
    const char *word = temp.c_str();
    URLRecordList *e;
    URLRecordList *n = new URLRecordList;
		n->_urlRecordIndex = _headURL;
    if(_wordToURLRecordList->find(word, &e)) {
    	if(e->_urlRecordIndex != _headURL) {
    		e->_next = n;
    		n->_next = NULL;
    	}
    }
    else {
    	_wordToURLRecordList->insertItem(word, n);
		}
	}
}

int main( int argc, char ** argv )
{
  const char * option;
  char ** url;
  int maxURL;
  int nURL;
  
  if ( argc == 1) {
	  printUsage();
	  exit(1);	  
  }
  else if (argc == 2) {
	  option = "";
	  url = &argv[1];
  }
  else { 
	  // There is an option
	  option = argv[1];
	  maxURL = atoi(argv[2]);
	  url = &argv[3];
	  nURL = argc-2;
  }
  
  // Process the arguments
  if ( !strcmp(option,"-h") ) {
	  printUsage();
	  exit(1);
  }
  
  if ( !strcmp(option,"") ) {
	  // default 1000 maxURLs
	  maxURL = 100;
	  nURL = 1;
	  WebCrawler wc(maxURL, nURL, (const char **)url);
	  wc.crawl();
	  wc.writeURLFile("url.txt");
	  //wc.writeWordFile("word.txt");
	}

  if ( *argv == NULL ) {
    // Print usage also if no URL after the arguments
    printUsage();
    exit(1);
  }
}

void printUsage()
{
  fprintf( stderr, "  Usage: webcrawl [-u <maxurls>] url-list\n");
  fprintf( stderr, "  Example: webcrawl https://www.cs.purdue.edu/\n");
}

