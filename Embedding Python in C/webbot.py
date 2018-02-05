'''
Written by Christian Phillips 9/27/16
Program goes to main page, extracts relevent links, then visits each linked page and extracts html information.
runtime: between 10 and 15 min...
'''


'''import important libraries'''
from bs4 import BeautifulSoup
import mechanize
import time
import urllib
import string
import sys


'''grab speech from given html link, write speech to file'''
def downloadSpeech(html):
    '''this does the actual HTML downloading'''
    browser = mechanize.Browser()
    read = browser.open(html)
    htmlstr = read.read()

    '''CHUNCK RM - remove header and footer information'''
    htmlCRM = ""
    sIndex = htmlstr.find("</header>")
    eIndex = htmlstr.find("<footer>")
    for n in range(sIndex, eIndex):
        htmlCRM += htmlstr[n]

    '''GRAB Title'''
    title = ""
    author = ""
    transcript = ""
    sIndex = htmlCRM.find("<h1")
    sIndex = htmlCRM.find(">", sIndex)+1
    eIndex = htmlCRM.find("<", sIndex)
    for n in range(sIndex, eIndex):
        title += htmlCRM[n]

    '''GRAB Author'''
    sIndex = htmlCRM.find("<h2", eIndex)
    sIndex = htmlCRM.find(">", sIndex)+1
    eIndex = htmlCRM.find("<", sIndex)
    for n in range(sIndex, eIndex):
        author += htmlCRM[n]

    '''GRAB Transcript, ignore everything in <...> '''
    sIndex = htmlCRM.find("<h2>Transcript</h2>",eIndex)
    sIndex = sIndex + 18
    eIndex = htmlCRM.find("</div>", sIndex)
    ignore = False
    for n in range(sIndex, eIndex):
        if(htmlCRM[n] == "<"):
            ignore = True
        elif(htmlCRM[n] == ">"):
            ignore = False

        if(ignore == False and htmlCRM[n] != ">"):
            transcript += htmlCRM[n]

    '''SPECIAL CASE replace &#39; with '  '''
    transcript = transcript.replace("&#39;", "'")
    transcript = transcript.replace("&nbsp;", "")
    transcript = transcript.replace("&rsquo;", "'")
    transcript = transcript.replace("&mdash;", ": ")

    '''SPECIAL CASE &#'''
    title = title.replace("&#8221", "")
    title = title.replace("&#8220", "'")
    title = title.replace("&#8217", "'")
    title = title.replace("&#8212", " ")

    '''check to make sure doc name is legal
        replace all illegal characters'''
    title = author + " " + title
    title = title.replace(":", "")
    title = title.replace("/", "")
    title = title.replace("\\", "")
    title = title.replace("?", "")
    title = title.replace("|", "")
    title = title.replace("*", "")
    title = title.replace(";", "")
    title = title.replace("'", "")
    

    '''SPECIAL CASE: remove &--(stuff)--; '''
    skip = False
    tempTitle = ""
    for n in range(len(title)):
        if(title[n] == "&"):
            skip = True
        elif(skip):
            if(title[n] == ";"):
                skip = False
        else:
            tempTitle += title[n];

    title = tempTitle

    '''Make sure length of Title is Manageable'''
    tempTitle = ""
    if(len(title) > 150):
        for n in range(0, 100):
            newTitle += title[n]
        title = newTitle

    '''If no transcript, don't write transcript to file'''
    if len(transcript) < 30:
        return title

    title += ".txt";
    ftmp = open("PresidentSpeeches/"+title, 'w')
    ftmp.write(transcript)
    ftmp.close

    print title
    return title
'''End DOWNLOAD SPEECH'''




def downloadFiles(numFiles):
    base = "http://millercenter.org"
    start = base + "/president/speeches"
    br = mechanize.Browser()
    r = br.open(start)
    html = r.read()


    '''edit out useless info'''
    sIndex = html.find("<!--Begin list of speeches -->")
    print "Start Index: " + str(sIndex)
    eIndex = html.find("<!--close listing-->")
    print "End Index:   " + str(eIndex)


    print "HomePage Edit"
    htmlEdit = ""
    for n in range(sIndex, eIndex):
        htmlEdit += html[n]
    print "HomePage Edit Complete"


    print "Beautifying Soup"

    '''Find and Extract Links'''
    soup = BeautifulSoup(htmlEdit, "lxml")


    print "Extracting Links"
    prev = cur = ""

    '''for use in main program, contains a list of all files generated'''
    fileList = open("FILELIST1.txt", 'w')

    '''increment for number of files requested'''
    ctr = 0;
    for link in soup.find_all('a'):
        cur = linkText = str(link.get('href'))
        if(len(linkText) > 10):
            if((prev == cur) != True):
                linkGet = base + linkText
                filename = downloadSpeech(linkGet)

                if(len(filename) > 10):
                	fileList.write(filename+"\n")
                        ctr = ctr+1
        prev = cur
        if(ctr == numFiles):
            break;

    fileList.close()

    print "finished"
'''END DOWNLOAD_FILES'''


'''///////////////////////////MAIN FUNCTION:: ENTER HERE////////////////////////////////'''
def main():
    numFiles = int(raw_input("How many files do you want to download? (-1 for all)\n>>> "))
    if numFiles != 0:
        downloadFiles(numFiles)
    print "exiting webbot"

if __name__ == "__main__":
    main()
