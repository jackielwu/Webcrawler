
#include "webcrawler.h"

// Add your implementation here
WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots)
{
	_maxUrls = maxUrls;
	_urlArray = new URLRecord[_maxUrls];
	//printf("%d\n", _maxUrls);
	//printf("%lu\n", sizeof(_urlArray));
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList *>();
	_headURL = 0;
	_tailURL = 0;
	tempLength=0;
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
		//printf("crawl\n");
		//Fetch the next URL is _headURL
		int n;
		char * buffer = fetchHTML(_urlArray[_headURL]._url, &n);
		//printf("%s\n",buffer);
		_headURL++;
		//If doc is not txt/html
		if(!isHTML(buffer))
		{
			continue;
		}
		if(strlen(buffer)<=3) {
			//printf("redirect\n");
			_urlArray[_headURL-1]._description = strdup("Redirect.");
			continue;
		}
		//isHTML(buffer);
		//Get first 500 char of doc w/o tags
		//Add to description to URLRecord
		findTitle(buffer, n);
		
		//printf("%s\n",_urlArray[_headURL-1]._url);
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
	//printf("%d\n", _tailURL);
	FILE *file;
	int n;
	file = fopen(urlFileName,"w");
	for (int i=0;i<_tailURL;i++) {
		fprintf(file,"%d %s\n", i, _urlArray[i]._url);
		//printf("%s\n", _urlArray[i]._description);
		fprintf(file,"%s\n\n",_urlArray[i]._description);
	}
	fclose(file);
	return;
}
void WebCrawler::writeWordFile(const char *wordFileName)
{
	FILE *file;
	int n;
	file = fopen(wordFileName,"w");
	HashTableTemplateIterator<URLRecordList *> iterator(_wordToURLRecordList);
	const char *key;
	URLRecordList *e;
	while(iterator.next(key,e)) {
		//printf("\n%s",key);
		fprintf(file,"%s", key);
		do {
			//printf(" %d",e->_urlRecordIndex);
			fprintf(file," %d", e->_urlRecordIndex);
			e=e->_next;
		}
		while(e != NULL);
		fprintf(file,"\n");
	}
	fclose(file);
	return;
}

void WebCrawler::onContentFound(char c) {
  //default gethttp
  //printf("%c", c);
  if(c>='a'&&c<='z' || c>='A'&&c<='Z'|| c==' ') {
  	findWord(c);
  }
  else {
  	tempLength=0;
  }
}

void WebCrawler::onAnchorFound(char * url)
{
  if(_tailURL < _maxUrls)
  {
  	if(url[0] != '#')
  	{
  		std::string u(url);
  		//printf("%d %s\n",_tailURL, url);
  		//printf("%s\n",_urlArray[_headURL-1]._url);
  		if(u.find("http") == 0) {
  			_urlArray[_tailURL]._url = strdup(url);
  			_urlToUrlRecord->insertItem((const char*)url, _tailURL);
  			_tailURL++;
  		}
  		else if(u.find("ftp")==0){;}
  		else if(u.find("mailto")==0){;}
  		else if(u.find("//")==0) {
  			u.insert(0,"http:");
  			_urlArray[_tailURL]._url = strdup(u.c_str());
  			_urlToUrlRecord->insertItem((const char*)u.c_str(), _tailURL);
  			_tailURL++;
  			//printf("%s\n", _urlArray[_tailURL-1]._url);
  		}
  		else if(u.find("/")==0) {
  			//printf("/\n");
  			u.insert(0,_urlArray[_headURL-1]._url);
  			_urlArray[_tailURL]._url = strdup(u.c_str());
  			_urlToUrlRecord->insertItem((const char*)u.c_str(), _tailURL);
  			_tailURL++;
  			//printf("%s\n", _urlArray[_headURL-1]._url);
  			//printf("%s\n", _urlArray[_tailURL-1]._url);
  		}
  		else if(u.find("../")==0) {
  			//printf("../\n");
  			std::string root(_urlArray[_headURL-1]._url);
  			//printf("%s\n",root.c_str());
  			int index = root.find_last_of("/");
  			root.erase(index,root.length()-index);
  			//printf("%s\n",root.c_str());
  			u.erase(0,2);
  			u.insert(0,root);
  			//printf("%s\n",u.c_str());
  			_urlArray[_tailURL]._url = strdup(u.c_str());
  			_urlToUrlRecord->insertItem((const char*)u.c_str(), _tailURL);
  			_tailURL++;
  		}
  		else {
  			//printf("%s\n",_urlArray[_headURL-1]._url);
  			//u.insert(0, "/");//handle separation for relative links
  			u.insert(0, _urlArray[_headURL-1]._url);
  			_urlArray[_tailURL]._url = strdup(u.c_str());
  			_urlToUrlRecord->insertItem((const char*)u.c_str(), _tailURL);
  			_tailURL++;
  			//printf("%s\n", _urlArray[_headURL-1]._url);
  			//printf("%s\n", _urlArray[_tailURL-1]._url);
  		}
  	}
	}
}

void WebCrawler::findWord(char c)
{
  
  if(c != ' ') {
    //put in string
    //printf("%c",c);
    temp[tempLength++]=c;
  }
  else {
    //put string into word
    if(tempLength==0)
    	return;
    temp[tempLength]='\0';
    const char *word = strdup(temp);
    tempLength=0;
    //printf("%s\n",word);
    URLRecordList *e;
    URLRecordList *n;
    if(_wordToURLRecordList->find(word, &e)) {
    		//printf("%s\n",_wordToURLRecordList->_key);
    	n=e;
    	while(n != NULL) {
    		if(n->_urlRecordIndex == _headURL-1) {
    			return;
    		}
				n=n->_next;
    	}
    	n = new URLRecordList;
    	n->_urlRecordIndex = _headURL-1;
    	n->_next = e;
    	_wordToURLRecordList->insertItem(word,n);
    }
    else {
    	n = new URLRecordList;
    	n->_urlRecordIndex = _headURL-1;
    	n->_next=NULL;
    	_wordToURLRecordList->insertItem(word, n);
		}
	}
}

void WebCrawler::findTitle(char *buffer, int n)
{
	int titleLength=0;
	char title[MaxURLLength];
	enum { START, TAG, TITLE, END} state;

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
			if (match(&b,"</title>")) {
				// Found ending "
				state = END;
				//printf(".%s.%d\n",title,titleLength);
				if(titleLength>0){
					title[titleLength] = '\0';
					//onAnchorFound(urlAnchor);
					//printf("%d %s\n",_headURL-1, title);
					int i=0;
					for(;i<titleLength;i++) {
						if(title[i]<' ' || title[i] > '~') {
							title[i]=' ';
						}
					}
					//printf("break\n");
					_urlArray[_headURL-1]._description = strdup(title);
					//printf("\n");
				}
				else {
					_urlArray[_headURL-1]._description = strdup("No Title.");
				}
				//printf(".%s.%d\n",title,titleLength);
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
		case END: {
			return;
		}
		default:;
		}
		
	}
}

int main( int argc, char ** argv )
{
  const char * option = "";
  char ** url;
  int maxURL;
  int nURL;
  
  if ( argc == 1) {
	  printUsage();
	  exit(1);	  
  }
  else if (strcmp(argv[1], "-u") == 0){ 
	  // There is an option
	  option = argv[1];
	  maxURL = atoi(argv[2]);
	  url = &argv[3];
	  nURL = argc-3;
  }
  else {
  	option = "";
	  url = &argv[1];
  }
  
  // Process the arguments
  if ( !strcmp(option,"-u") ) {
  	WebCrawler wc(maxURL, nURL, (const char **)url);
	  wc.crawl();
	  wc.writeURLFile("url.txt");
	  wc.writeWordFile("word.txt");
	}
  
  if ( !strcmp(option,"") ) {
	  // default 1000 maxURLs
	  maxURL = 1000;
	  nURL = argc -1;
	  WebCrawler wc(maxURL, nURL, (const char **)url);
	  wc.crawl();
	  wc.writeURLFile("url.txt");
	  wc.writeWordFile("word.txt");
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
bool WebCrawler::isHTML(char * buffer)
{
	if(buffer == NULL){
		return false;
	}
	if(buffer[0]=='%') {
		_urlArray[_headURL-1]._description = strdup("PDF.");
		return false;
	}
	return true;
}

