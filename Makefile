
all: git-commit gethttp web-crawler

web-crawler: webcrawler.cpp HashTableTemplate.h SimpleHTMLParser.cpp openhttp.cpp
	g++ -o webcrawl -g webcrawler.cpp openhttp.cpp SimpleHTMLParser.cpp  HashTableTemplate.h -lcurl

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp -lcurl

git-commit:
	git add Makefile *.h *.cpp README .local.git.out >> .local.git.out
	git commit -a -m "Commit web-crawler" >> .local.git.out

clean:
	rm -f *.o gethttp core
