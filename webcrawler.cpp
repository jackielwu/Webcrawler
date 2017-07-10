
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
		findTitle(buffer, n);
		//parse(buffer, n);
		
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
	FILE *file;
	int n;
	file = fopen(urlFileName,"w");
	for (int i=0;i<_tailURL;i++) {
		fprintf(file,"%d %s\n", i, _urlArray[i]._url);
		printf("%s\n", _urlArray[i]._description);
		fprintf(file,"%s\n\n",_urlArray[i]._description);
	}
	fclose(file);
	return;
}
void WebCrawler::writeWordFile(const char *wordFileName)
{
	/*ofstream file;
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
	return;*/
}

void WebCrawler::onContentFound(char c) {
  //default gethttp
  //printf("%c", c);
  if(c>='a'&&c<='z' || c>='A'&&c<='Z'|| c==' '||c=='.') {
  	if(temp.length() < 500) {
  		temp += c;
  	}
  }
  else {
  	temp+='\0';
  	printf("%s\n",temp.c_str());
  	_urlArray[_headURL]._description = strdup(temp.c_str());
  	temp.clear();
  }
  //findWord(c);
}

void WebCrawler::onAnchorFound(char * url)
{
  if(_tailURL < _maxUrls)
  {
  	if(url[0] != '#')
  	{
  		std::string u(url);
  		if(u.find("http") == 0) {
  			printf("%d %s\n",_tailURL, url);
  			_urlArray[_tailURL]._url = strdup(url);
  			_urlToUrlRecord->insertItem((const char*)url, _tailURL);
  			_tailURL++;
  		}
  		/*else if(u.find("http") == string::npos) {
  			//u.insert(0, "/");//handle separation for relative links
  			u.insert(0, _urlArray[0]._url);
  			_urlArray[_tailURL++]._url = (char *)u.c_str();
  			_urlToUrlRecord->insertItem((const char*)u.c_str(), _tailURL);
  			//printf("%s\n", _urlArray[_tailURL-1]._url);
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

void WebCrawler::findTitle(char *buffer, int n)
{
	int titleLength;
	char title[MaxURLLength];
	enum { START, TAG, TITLE} state;

	state = START;
	
	char * bufferEnd = buffer + n;
	char * b = buffer;
	bool lastCharSpace = false;
	while (b < bufferEnd) {
		//printf("<%c,%d,%d>", *b, *b,state);
		switch (state) {
		case START: {
			if (match(&b,"<title>")) {
				state = TITLE;
			}
			else if	(match(&b,"<")) {
				state = TAG;
			}
			else {
				b++;
			}
			break;
		}
		case TITLE: {
			if (match(&b,"<")) {
				// Found ending "
				state = TAG;
				title[titleLength] = '\0';
				//onAnchorFound(urlAnchor);
				printf("%d %s\n",_headURL, title);
				_urlArray[_headURL]._description = strdup(title);
				//printf("\n");
			}
			else {
				if ( titleLength < MaxURLLength-1) {
					title[titleLength] = *b;
					titleLength++;
				}
				//printf("%c", *b, *b);
				b++;
			}
			break;
				
		}
		case TAG: {
			if (match(&b, ">")) {
				state = START;
			}
			else {
				b++;
			}
			break;
		}
		default:;
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
	  maxURL = 10;
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

