   import java.util.*;
   import java.io.*;
/**********************************************************
 * Searches for duplicates within the provided text file. 
 *  
 * @author Murphy Cooke
 *
 **********************************************************/
   public class DuplicateSearch
   {
      public static void main (String[] args) throws FileNotFoundException
      {
      
         String file1 = "";
         String tester = "";
         String file2 = "";
         int size = 3;
         ArrayList<String> theOutput = new ArrayList<String>();
      	
         if(args.length > 0 && args.length < 4)
         {
            file1 = args[0];
            file2 = args[1];
         
            if(args.length > 2)
            {
               size = Integer.parseInt(args[2]);	
            }
         }
      	
         theOutput.addAll(findCopy(file1, file2, size));
         System.out.print(theOutput.toString());
         System.out.println("done");
        
      
      }
   	
   	
      public static ArrayList<String> findCopy(String file, String filetwo, int k) throws FileNotFoundException
      {
      	
      	
         ArrayList<String> totalOutput = new ArrayList<String>();
      	
         String finalOutput = "";
         ArrayList<String> longestCopy = new ArrayList<String>();
         int locA = -1;
         int locB = -1;
         int currentIndex = 0;
         int wordControl = k;
         int wordControlTwo = 0;
         int tempLoc = 0;
         int tempLocTwo = 0;
         boolean chain = true;
      	
         try{
         
            ArrayList<String> words1 = DocumentReader.readDocument(file);
            ArrayList<String> words2initial = new ArrayList<String>(DocumentReader.readDocument(filetwo));
            HashMap<ArrayList<String>, ArrayList<Integer>> words2 = new HashMap<ArrayList<String>, ArrayList<Integer>>();
         	
            for(int i = 0; i < words2initial.size(); i++)
            {
               ArrayList<String> tempList = new ArrayList<String>(); 
            	
            
               for(int j = currentIndex; j < wordControl; j++)
               {
                  if(wordControl < words2initial.size())
                  {
                  
                     tempList.add(words2initial.get(j));
                      
                  }
               	
               	
               }
            
            
               currentIndex++;
               wordControl = currentIndex + k;
            
            	
               if(words2.containsKey(tempList))
               {
               	
                  ArrayList<Integer> tempIndex = new ArrayList<Integer>(words2.get(tempList));
               	
                  for(int j = wordControlTwo; j < currentIndex + (k - 1); j++)
                  {
                     tempIndex.add(j);
                  } 
                  wordControlTwo = currentIndex;
               	
                
                  words2.put(tempList, tempIndex);	
                 
               }
               else
               {
               	
                  ArrayList<Integer> tempIndex = new ArrayList<Integer>();
               	
                  for(int j = wordControlTwo; j < currentIndex + (k - 1); j++)
                  {
                     tempIndex.add(j);
                  
                  } 
                  wordControlTwo = currentIndex;
               
                  words2.put(tempList, tempIndex);	
                 
               }
            	
            
            	
            }
         
            wordControl = 0; 
            currentIndex = 0;
         	
            ArrayList<String> tempCopy = new ArrayList<String>();
            ArrayList<String> tempChain = new ArrayList<String>();
            ArrayList<String> tempChainTwo = new ArrayList<String>();
         	
            for(int i = 0; i < words1.size() - (k + 1); i++)
            {
               ArrayList<String> tempWordList = new ArrayList<String>(); 
            	 
            	
               for(int j = currentIndex; j < currentIndex + k; j++)
               {
                  if(wordControl < words1.size())
                  {
                     tempWordList.add(words1.get(j));
                  }
               }
            	
               currentIndex++;
               wordControl = currentIndex + k;
            	
               if(words2.containsKey(tempWordList))
               {
                  tempChain.clear();
               	
               	
                  chain = true;
               	
                  tempLoc = words2.get(tempWordList).get(0);
                  tempLocTwo = i;
                  
                  
                  while(chain)
                  {
                  	
                  	
                     tempChainTwo.clear();
                  	
                     for(int e = currentIndex; e < currentIndex + k; e++)
                     {
                        if(wordControl < words1.size())
                        {
                           tempChainTwo.add(words1.get(e));
                        }
                     }
                  
                  	
                     if(words2.containsKey(tempChainTwo))
                     {
                        currentIndex++;
                 
                     	
                        for(int f = 1; f < tempWordList.size(); f++)
                        {
                           tempChainTwo.add(tempWordList.get(f));
									
                        }
                        if(currentIndex < words1.size() - k)
                        {
                           tempChainTwo.add(words1.get(currentIndex));
                        }
                        if(tempChainTwo.size() > longestCopy.size())
                        {
                           longestCopy = tempChainTwo;
                        }
                     }
                     else
                     {
                        chain = false;
                     }
                  }
               
               	
               }
            
               wordControl = currentIndex + k;
            	
            }
         	
         	
         	
                          
         }
            catch(FileNotFoundException e)
            {
               System.out.println("Please enter proper file names");
            }
      	
      	
      	
      	
         return longestCopy;
      }
   }