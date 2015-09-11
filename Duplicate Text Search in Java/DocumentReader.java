
import java.util.ArrayList;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;


/**********************************************************
 *  The DocumentReader classs includes utilities for extracting words 
 *  from text documents and returning them in an ArrayList. <br>
 *  
 * @author Nathan Sprague
 * @version V2.0, 04/2012
 
 **********************************************************/
public class DocumentReader 
{

    /*********************************************************
     * This method opens a plain-text file and reads its contents
     *  into an ArrayList.  All punctuation and whitespace is
     *  discarded.  All words are converted to lower-case. 
     *  The words in the ArrayList are stored in the order
     *  they appear in the document and include repetitions.
     * 
     *  @param fileName   the name of a plain-text file 
     *  @throws FileNotFoundException 
     *  
     *  @returns  an arraylist containing the ordered list of words
     ********************************************************/
    public static ArrayList<String> readDocument(String fileName) 
                                      throws FileNotFoundException
    {
        Scanner scanner; 
        ArrayList<String> words = new ArrayList<String>();

        scanner = new Scanner(new File(fileName));
        scanner.useDelimiter("[\\W]"); //all non-word characters
        while (scanner.hasNext()) {
            String cur = scanner.next();
            if (!cur.equals(""))  
            {
                words.add(cur.toLowerCase());
            }
        }

        return words;
    }

}


