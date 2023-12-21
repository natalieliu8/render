//
//  main.cpp
//  render
//
//  Created by natalie liu on 11/12/23.
//
//IMPORTANT TODO: THE LAST ONE OF STRING CAN'T HAVE SPACES (?!' ETC) ONLY @P@, @P@, @P@
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <streambuf>
#include <cassert>
using namespace std;

const int maxLineLength = 181; //has to include '\0'
int render(int lineLength, istream& inf, ostream& outf);
int trackdashes(char* temporary, char dash, int pos);
void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if ( ! dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if ( ! oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}

int main() {
               
                /*ofstream outfile("results.txt");   // outfile is a name of our choosing.
                    if ( ! outfile )           // Did the creation fail?
                    {
                        cerr << "Error: Cannot create results.txt!" << endl;
                    }
                string input;
                cout<< "Input what you want to write in the file" << endl;
                getline(cin,input);
                outfile << input << endl;
                
                ifstream infile("results.txt");
                if (!infile)
                {
                    cerr << "Cannot open " << "results.txt" << "!" << endl;
                }
                cout << "Enter maximum line length: ";
                int len;
                cin >> len;
                cin.ignore(10000, '\n');
                int returnCode = render(len, infile, cout);
                cout << "Return code is " << returnCode << endl;*/
    /*ifstream infile("test.txt");
    int rc = render(250, infile, cout);
    cout  << "Return code is " << rc << endl;*/
    testRender(7, "This\n\t\tis a\ntest\n", "This is\na test\n", 0);
            testRender(8, "  This is a test  \n", "This is\na test\n", 0);
            testRender(6, "Testing it\n", "Testin\ng it\n", 1);
            testRender(-5, "irrelevant", "irrelevant", 2);
    testRender(10, "--------------------", "----------\n----------\n", 0);
    testRender(6, "bruno-is-a-dog", "bruno-\nis-a-\ndog\n", 0);
    testRender(9,"-testing-out----dashes-is-sofun","-testing-\nout----\ndashes-\nis-sofun\n",0);
    testRender(3,"so--called", "so-\n-\ncal\nled\n",1);
    testRender(6,"@P@ @P@ @P@ @P@ @P@", "",0);
   testRender(15,"hello! i am currently testing double spaces in punctuation!", "hello!  i am\ncurrently\ntesting double\nspaces in\npunctuation!\n",0);
    testRender(15,"continuous functions will this process as one or two breaks","continuous\nfunctions will\nthis process as\none or two\nbreaks\n",0);
    testRender(15,"continuous functions @P@ @P@ will this process as one or two breaks","continuous\nfunctions\n\nwill this\nprocess as one\nor two breaks\n",0);
    testRender(15,"@P@ cannot be at the beginning","cannot be at\nthe beginning\n",0);
    testRender(15,"cannot be at the end @P@", "cannot be at\nthe end\n",0);
    testRender(9, "word-word", "word-word\n",0);
    testRender(15, "@P@ @P@ @P@\nhello","hello\n",0);
    testRender(40, "testing! there should be two spaces: or not","testing!  there should be two spaces:\nor not\n",0);
    testRender(0,"hello!","",2);
    testRender(6,"swan's nest","swan's\nnest\n",0);
    testRender(5,"swan's nest","swan'\ns\nnest\n",1);
    testRender(4,"so--called","so--\ncall\ned\n",1);
    testRender(6,"harry-adieu-queue-","harry-\nadieu-\nqueue-\n",0);
    testRender(5,"harry-adieu-queue-","harry\n-\nadieu\n-\nqueue\n-\n",1);
    testRender(20,"@P@-@P@","@P@-@P@\n",0);
    testRender(10, "love toeatapplesandbananas", "love\ntoeatapple\nsandbanana\ns\n", 1);
            cerr << "Tests complete" << endl;
        
}

int render(int lineLength, istream& inf, ostream& outf){
    if (lineLength < 1){
        return 2;
    }
    
    char temporary[maxLineLength]; //this is going to be how i read a line at a time
    
    int printposition = 0;

    int wordlength; //these two variables are the RAW words, without spaces/dashes or anything added
    char word[maxLineLength];
    int wordcount = 0; //to deal with the case if there are multiple @P@'s in a row, only input
    
    bool initialparagraph = true;
    int continuousbreaks = 0;
    bool needabreak = false;
    bool punctuationspace = false;
    
    char processedword[maxLineLength];
    int processedwordlength;
    
    int dashposition;
    bool dashexistence;
    int portionindex;
    char wordportion[maxLineLength];
    
    bool toolarge = false; //this identifies whether or not I'll have a case where the word cannot be broken up, and thus have to return 1 but still output smth
    
    
    while(inf.getline(temporary, maxLineLength)){ //this outer loop gets me each input line
        //cerr << "the line of interest is: " << temporary << endl;
        //cerr << "the length of the line of interest is: " << strlen(temporary) << endl;
        if (temporary[0] == '\0' && initialparagraph == true){
            return 0;
        }
        if (temporary[0] == '\0' ){
            continue;
        }
        
        
        for (int position = 0; position < strlen(temporary);){// this loop is for each word

            //if the line starts out with spaces
            if (isspace(temporary[position])){
                position++;
                continue;
            }
            //separate the words
            wordlength = 1;
            while(position+wordlength < strlen(temporary)){ //find the wordlength of the raw word
                if(!isspace(temporary[position+wordlength])){
                    wordlength++;
                    //cerr << "the wordlength at this time is: " << wordlength << endl;
                }
                else{
                    break;
                }
            }
            
            strncpy(word,&temporary[position],wordlength);
            word[wordlength] = '\0'; //make sure that the array ends so it doesn't have remaining characters from previous, longer words
            //cerr << "[raw word is " << word << "]" << endl;
            position = position + wordlength;
            
           
            if (strcmp(word,"@P@") != 0){
                wordcount++;
            }
            
            if (strcmp(word,"@P@") == 0){ //IMPORTANT: WHAT HAPPENS IF THERE'S A @P@ @P@ @P@? ALSO @P@ CANNOT BE THE FIRST OR LAST ONE
                if (initialparagraph == false && continuousbreaks == 0){
                    needabreak = true; //flag this and output later after the next word is processed, thus that the final @P@ won't return everything
                }
                continuousbreaks++;
                continue;
            }
            
            //add wordcount
            continuousbreaks = 0;
            initialparagraph = false;
            
            if (needabreak == true){ //this is where we output the previous @P@
                //cerr << "this break happens when continuousbreaks is: " << continuousbreaks;
                outf << endl << endl;
                printposition = 0;
                needabreak = false;
                punctuationspace = false;
            }
            

            //now it's time to start outputting the words: i'm going to print out each word at a time
            //there are 4 cases for words: one is that the entire word happily fits on the line. the other three are involved if the word does NOT fit on the line. let's first tackle when the entire word fits on the line
            
            //cerr << "[the print position is "  << printposition << "]";
            portionindex = 0;
            for(;;){
                //first, i need to add my spaces to the processed word: processedword is basically the raw word w/ spaces added to account for when space is at the start or the beginning
                if (printposition > 0){ //starting not in the middle of a line!
                    if (punctuationspace == true){ //what if there's a ?,!,:
                        processedword[0] = ' ';
                        processedword[1] = ' ';
                        strcpy(&processedword[2],&word[portionindex]);
                        punctuationspace = false;
                        processedwordlength = wordlength - portionindex + 2;
                    }
                    else{ //this is simply if it's not at the beginning
                        processedword[0] = ' ';
                        strcpy(&processedword[1], &word[portionindex]);
                        //cerr << "[this is the processed word index: " << portionindex << "]" << endl;
                        processedwordlength = wordlength - portionindex + 1;
                    }
                    
                }
                else{
                    strcpy(processedword, &word[portionindex]);
                    processedwordlength = wordlength - portionindex;
                }
                //cerr << "[this is the processed word: " << processedword << "]" << endl;
                
                //now that i've gotten my spaces, it's time to figure out whether or not the words fit onto the line. let's start w/ first determining whether or not the word fits on the line
                
                
                if (printposition + processedwordlength <= lineLength){
                    outf << processedword;
                    printposition = printposition + processedwordlength;
                    break;
                }
                else{
                    dashexistence = false;
                    
                    for (int n = strlen(processedword) - 1;(dashposition = trackdashes(processedword, '-', n)) >= 0 ;){
                        //cerr << "this for loop has been entered" << endl;
                        
                        if ((printposition + dashposition + 1) <= lineLength){
                            strncpy(wordportion, processedword, dashposition+1);
                            wordportion[dashposition+1] = '\0';
                            dashexistence = true;
                            //cerr << "wordportion: " << wordportion << endl;
                            break;
                        }
                        else{
                            n = dashposition - 1;
                        }
                    }
                    
                    if (dashexistence == true){ //this is the case where a dash will successfully break up a word on a line
                        //cerr << "!!!HELLO" << endl;
                        outf << wordportion;
                        portionindex = strlen(wordportion) + portionindex;
                        printposition = strlen(wordportion) + printposition;
                    }
                    else{
                        if (printposition != 0){ //otherwise, we just switch lines
                            outf << endl;
                            printposition = 0;
                        }
                        else if (strlen(word) > lineLength && dashexistence == false){ //this is when the wordlength is just too long
                            toolarge = true;
                            strncpy(wordportion, processedword, lineLength);
                            wordportion[lineLength] = '\0';
                            outf << wordportion << endl;
                            portionindex += strlen(wordportion);
                            printposition = 0;
                        }
                        else {
                            strncpy(wordportion, processedword, lineLength);
                            wordportion[lineLength] = '\0';
                            outf << wordportion << endl;
                            portionindex += strlen(wordportion);
                            printposition = 0;
                        }
                    }
                }
            }
                
                if (word[wordlength-1] == '.'||word[wordlength-1] == '?'||word[wordlength-1] == '!'||word[wordlength-1] == ':'){
                    punctuationspace = true; //this is to combat the case where the ! is at the end of the output line, and thus cannot have any spaces behind it: if we raise a flag and only output the space when we process the next word, that avoids this
                }
            }
        }
    
    
    if (wordcount != 0){
        outf <<endl;} //this can only happen if it's not just a line w/ @P@ @P@ @P@
    if (toolarge == true){
        return 1;
    }
    return 0;

}

int trackdashes(char* temporary, char dash, int pos){
    int i;
    for (i = pos; i >= 0; i --) {
        if (temporary[i] == dash)
            return(i);
    }
    return (-1); //if there are no dashes
}




