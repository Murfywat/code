""" Testing Methods for the Vector class. 


@author: Nathan Sprague
@version: 10/3/12
"""
import unittest
from t_array import Array
from Vector import Vector

BIG_VEC_SIZE = 50

class VectorTester(unittest.TestCase):
    """Test class for testing Vector methods.  """

    def setUp(self):
        """ Create several vectors that can be used by any test. """
        Array.visible = False
        self.emptyVec = Vector()
        self.oneVec = Vector()
        self.oneVec.append(10)
        
        # Create two equal large vectors containing the numbers
        # 0-BIG_VEC_SIZE-1 We will call methods on bigVec. cleanBigVec
        # will be used for comparisons... to make sure that bigVec is
        # not altered by methods that should not change it.

        self.bigVec = Vector()
        self.cleanBigVec = Vector()
        for i in range(BIG_VEC_SIZE):
            self.bigVec.append(i)
            self.cleanBigVec.append(i)
            
        self.duplicatesVec = Vector()
        self.duplicatesVec.append("a")
        self.duplicatesVec.append("b")
        self.duplicatesVec.append("b")
        self.duplicatesVec.append("c")


    #--------------------------------------------------------
    #TESTING METHODS FOR LENGTH (and APPEND)
    #--------------------------------------------------------         
    def testLengthEmpty(self):
        self.assertEquals(len(Vector()), 0, 
                          "Empty Vector should have length 0.")

    def testLengthAfterOneAppend(self):
        self.assertEquals(len(self.oneVec), 1)
        
    def testLengthAfterManyAppends(self):   
        self.assertEquals(len(self.bigVec), BIG_VEC_SIZE)

    def testCorrectFirstItemAfterAppends(self):
        self.assertEquals(self.bigVec[0], 0)
        
    def testCorrectLastItemAfterAppends(self):
        self.assertEquals(self.bigVec[BIG_VEC_SIZE-1], BIG_VEC_SIZE-1)

    
    #--------------------------------------------------------
    #TESTING METHODS FOR EQUAL AND NOT EQUAL
    #--------------------------------------------------------         
    def testEmptyVecsEqual(self):
        self.assertEqual(self.emptyVec, Vector())

    def testEmptyVecsNotEqual(self):
        self.assertFalse(self.emptyVec != Vector())
        
    def testEqualNonVec(self):
        self.assertFalse(self.emptyVec == [])
        
    def testNotEqualNonVec(self):
        self.assertNotEqual(self.emptyVec, [])

    def testNotEqualsOutOfOrder(self):
        vec1 = Vector()
        vec1.append("a")
        vec1.append("b")
        vec2 = Vector()
        vec2.append("b")
        vec2.append("a")
        self.assertNotEqual(vec1, vec2)

    def testEqualsOutOfOrder(self):
        vec1 = Vector()
        vec1.append("a")
        vec1.append("b")
        vec2 = Vector()
        vec2.append("b")
        vec2.append("a")
        self.assertFalse(vec1 == vec2)
        
    def testEqualsDifferentSizes(self):
        vec = Vector()
        for i in range(BIG_VEC_SIZE-1):
            vec.append(i)
        self.assertFalse(vec == self.bigVec)  

    def testNotEqualsDifferentSizes(self):
        vec = Vector()
        for i in range(BIG_VEC_SIZE-1):
            vec.append(i)
        self.assertNotEqual(vec, self.bigVec)  

    def testEqualsTrue(self):
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testNotEqualsTrue(self):
        self.assertFalse(self.bigVec !=  self.cleanBigVec)

    def testEqualsSame(self):
        self.assertEqual(self.bigVec,  self.bigVec)

    def testNotEqualsSame(self):
        self.assertFalse(self.bigVec !=  self.bigVec)
        
    #--------------------------------------------------------
    #TESTING METHODS FOR CONTAINS
    #--------------------------------------------------------         
    def testContainsOnEmpty(self):
        self.assertFalse(10 in self.emptyVec)

    def testContainsFalse(self):
        self.assertFalse("a" in self.bigVec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testContainsAtStart(self):
        self.assertTrue(0 in self.bigVec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testContainsAtEnd(self):
        self.assertTrue(BIG_VEC_SIZE - 1 in self.bigVec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testContainsMiddle(self):
        self.assertTrue(5 in self.bigVec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    
        
    #--------------------------------------------------------
    #TESTING METHODS FOR GETITEM
    #--------------------------------------------------------     
    def testGetItemEmptyVec(self):
        self.assertRaises(IndexError, self.emptyVec.__getitem__, 0)
        
    def testGetItemIndexTooSmall(self):
        self.assertRaises(IndexError, self.bigVec.__getitem__,
                          -BIG_VEC_SIZE - 1)
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testGetItemIndexTooBig(self):
        self.assertRaises(IndexError, self.bigVec.__getitem__, BIG_VEC_SIZE)
        self.assertEqual(self.bigVec, self.cleanBigVec)
     
    def testGetItemNegativeIndexStart(self):
        self.assertEqual(self.bigVec[-BIG_VEC_SIZE],  0)
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testGetItemNegativeIndexEnd(self):
        self.assertEqual(self.bigVec[-1],  BIG_VEC_SIZE - 1)
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    #(tests for append check correct first and last items.)
    
    #--------------------------------------------------------
    #TESTING METHODS FOR SETITEM
    #--------------------------------------------------------     
        
    def testSetItemEmptyVec(self):
        self.assertRaises(IndexError, self.emptyVec.__setitem__, 0, "a")
           
    def testSetIndexTooSmall(self):
        self.assertRaises(IndexError, self.bigVec.__setitem__,
                          -BIG_VEC_SIZE - 1, "a")
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testSetIndexTooBig(self):
        self.assertRaises(IndexError, self.bigVec.__setitem__,
                          BIG_VEC_SIZE, "a")
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testSetFirstItem(self):
        self.bigVec[0] = "a"
        self.assertEqual(self.bigVec[0], "a")
        self.assertEqual(len(self.bigVec), BIG_VEC_SIZE)
        
    def testSetLastItem(self):
        self.bigVec[BIG_VEC_SIZE - 1 ] = "a"
        self.assertEqual(self.bigVec[BIG_VEC_SIZE - 1], "a") 
        self.assertEqual(len(self.bigVec), BIG_VEC_SIZE)
        
    def testSetNegativeIndexStart(self):
        self.bigVec[-BIG_VEC_SIZE] = "a"
        self.assertEqual(self.bigVec[-BIG_VEC_SIZE],  "a")
        self.assertEqual(len(self.bigVec), BIG_VEC_SIZE)
    
    def testSetNegativeIndexEnd(self):
        self.bigVec[-1] = "a"
        self.assertEquals(self.bigVec[-1],  "a")
        self.assertEqual(len(self.bigVec), BIG_VEC_SIZE)
        

    #--------------------------------------------------------
    #TESTING METHODS FOR STRING
    #--------------------------------------------------------   

    def testStringEmpty(self):
        self.assertEqual(str(self.emptyVec), "[]")

    def testStringOneVal(self):
        self.assertEqual(str(self.oneVec), "[10]")

    def testStringBigVec(self):
        bigList = range(BIG_VEC_SIZE)
        self.assertEqual(str(self.bigVec), str(bigList))

    #--------------------------------------------------------
    #TESTING METHODS FOR GETSLICE
    #--------------------------------------------------------  
    def testSliceFirstIndexTooSmall(self):
        self.assertRaises(IndexError, self.bigVec.getSlice,
                           -BIG_VEC_SIZE - 1, 1)
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testSliceFirstIndexTooBig(self):
        self.assertRaises(IndexError, self.bigVec.getSlice,
                           BIG_VEC_SIZE, 1)
        self.assertEqual(self.bigVec, self.cleanBigVec)
       
    def testSliceSecondIndexTooSmall(self):
        self.assertRaises(IndexError, self.bigVec.getSlice,
                           -BIG_VEC_SIZE - 1, 1)
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testSliceSecondIndexTooBig(self):
        self.assertRaises(IndexError, self.bigVec.getSlice,
                           BIG_VEC_SIZE + 1, 1)
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testSliceOutOfOrder(self):
        self.assertEquals(self.bigVec.getSlice(2, 1), Vector())
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testSliceOutOfOrderNegative(self):
        self.assertEquals(self.bigVec.getSlice(-1, -2), Vector())
        self.assertEqual(self.bigVec, self.cleanBigVec)
            
    def testSliceSizeOneStart(self):
        vec = Vector()
        vec.append(0)
        self.assertEquals(self.bigVec.getSlice(0, 1), vec)
        self.assertEqual(self.bigVec, self.cleanBigVec)       
        
    def testSliceSizeOneStartNegative(self):
        vec = Vector()
        vec.append(0)
        self.assertEquals(self.bigVec.getSlice(-BIG_VEC_SIZE,-BIG_VEC_SIZE+1),
                          vec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testSliceSizeOneEnd(self):
        vec = Vector()
        vec.append(BIG_VEC_SIZE -1)
        self.assertEquals(self.bigVec.getSlice(BIG_VEC_SIZE-1, BIG_VEC_SIZE),
                          vec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testBigSliceMiddle(self):
        vec = Vector()
        for i in range(10, BIG_VEC_SIZE - 10):
            vec.append(i)
        self.assertEquals(self.bigVec.getSlice(10, BIG_VEC_SIZE - 10), vec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testBigSliceMiddleNegative(self):
        vec = Vector()
        for i in range(10, BIG_VEC_SIZE - 10):
            vec.append(i)
        self.assertEquals(self.bigVec.getSlice(10 - BIG_VEC_SIZE, -10), vec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    #--------------------------------------------------------
    #TESTING METHODS FOR ITERATOR
    #--------------------------------------------------------     
    def testIteratorBigVec(self):
        i = 0
        for item in self.bigVec:
            #make sure the correct values are retrieved
            self.assertEquals(i, item)
            i += 1
        #Make sure the right number of steps took place.
        self.assertEqual(i, BIG_VEC_SIZE)
        self.assertEqual(self.bigVec, self.cleanBigVec)
   
    def testIteratorBigVecDirectly(self):
        i = 0
        it = iter(self.bigVec)
        for item in it:
            self.assertEquals(i, item)
            i += 1
        self.assertEqual(i, BIG_VEC_SIZE)
        self.assertEqual(self.bigVec, self.cleanBigVec)
            
    def testIteratorEmptyVec(self):
        for _ in self.emptyVec:
            #Should never get here.  Raise an exception if we do.
            self.assertTrue(False)
            
    #--------------------------------------------------------
    #TESTING METHODS FOR EXTEND
    #--------------------------------------------------------     
    def testExtendByEmpty(self):
        vec = Vector()
        for i in range(BIG_VEC_SIZE):
            vec.append(i)
        self.bigVec.extend(Vector())
        self.assertEqual(self.bigVec, vec)
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testExtendEmpty(self):
        vec = Vector()
        vec.extend(self.bigVec)
        self.assertEqual(self.bigVec, vec)
            
    def testExtendEmptyByEmpty(self):
        vec = Vector()
        vec.extend(Vector())
        self.assertEqual(vec, Vector())
    
    def testExtendBySelf(self):
        self.assertRaises(NotImplementedError,self.bigVec.extend, self.bigVec )
        self.assertEqual(self.bigVec, self.cleanBigVec)
        
    def testExtendGood(self):
        vec = Vector()
        for i in range(BIG_VEC_SIZE):
            vec.append(i)
        vec.append(10)
        
        self.bigVec.extend(self.oneVec)
        self.assertEqual(self.bigVec, vec)

    def testExtendByNonVector(self):
        vec1 = Vector()
        vec2 = Vector()
        vec1.extend(["a", "b"])
        vec2.append("a")
        vec2.append("b")
        self.assertEqual(vec1, vec2)
    
    #--------------------------------------------------------
    #TESTING METHODS FOR INSERT
    #--------------------------------------------------------
    def testInsertIndexTooSmall(self):
        self.assertRaises(IndexError, self.bigVec.insert,
                          -BIG_VEC_SIZE - 1, "a")
        self.assertEqual(self.bigVec, self.cleanBigVec)
    
    def testInsertIndexTooBig(self):
        self.assertRaises(IndexError, self.bigVec.insert,
                          BIG_VEC_SIZE + 1, "a")
        self.assertEqual(self.bigVec, self.cleanBigVec)
         
    def testInsertAtStart(self):
        self.bigVec.insert(0, "a")
        vec = Vector()
        vec.append("a")
        for i in range(BIG_VEC_SIZE):
            vec.append(i)
        self.assertEqual(self.bigVec, vec)
        
    def testInsertAtStartNegative(self):
        self.bigVec.insert(-BIG_VEC_SIZE, "a")
        vec = Vector()
        vec.append("a")
        for i in range(BIG_VEC_SIZE):
            vec.append(i)
        self.assertEqual(self.bigVec, vec)
        
    def testInsertAtEnd(self):
        self.bigVec.insert(BIG_VEC_SIZE, "a")
        vec = Vector()
        for i in range(BIG_VEC_SIZE):
            vec.append(i)
        vec.append("a")
        self.assertEqual(self.bigVec, vec)
        
    def testInsertMiddle(self):
        vec1 = Vector()
        vec1.append("a")
        vec1.append("b")
        vec1.append("c")
        vec2 = Vector()
        vec2.append("a")
        vec2.append("c")
        vec2.insert(1,"b")
        self.assertEqual(vec1, vec2)
        
    #--------------------------------------------------------
    #TESTING METHODS FOR REMOVE
    #--------------------------------------------------------    
    def testRemoveEmpty(self):
        self.assertRaises(ValueError, self.emptyVec.remove, "a")
        self.assertEqual(len(self.emptyVec), 0)
    
    def testRemoveNotIn(self):
        self.assertRaises(ValueError, self.bigVec.remove, "a")
        self.assertEqual(self.bigVec, self.cleanBigVec)
 
    def testRemoveLast(self):
        self.oneVec.remove(10)
        self.assertEqual(self.oneVec, Vector())
        
    def testRemoveAtStart(self):
        vec = Vector()
        for i in range(1,BIG_VEC_SIZE):
            vec.append(i)
            
        self.bigVec.remove(0)
        vec.remove(0)
        self.assertEqual(vec, self.bigVec)
        
        
    def testRemoveAtEnd(self):
        vec = Vector()
        for i in range(BIG_VEC_SIZE-1):
            vec.append(i)
        
        self.bigVec.remove(BIG_VEC_SIZE-1)
        self.assertEqual(vec, self.bigVec)
        
    def testManyRemoves(self):
        vec = Vector()
        for i in range(50, BIG_VEC_SIZE):
            vec.append(i)
            
        for i in range(50):
            self.bigVec.remove(i)
        self.assertEquals(vec, self.bigVec)
        
        
    def testRemoveWithDuplicates(self):   
        vec = Vector()
        vec.append("a")
        vec.append("b")
        vec.append("c")
        self.duplicatesVec.remove("b")
        self.assertEquals(vec, self.duplicatesVec)
        
    #--------------------------------------------------------
    #TESTING METHODS FOR INDEX
    #--------------------------------------------------------      
        
    def testIndexEmpty(self):
        self.assertRaises(ValueError, self.emptyVec.index, "a")
        self.assertEqual(len(self.emptyVec), 0)
        
    def testIndexNotIn(self):
        self.assertRaises(ValueError, self.bigVec.index, "a")
        self.assertEqual(len(self.emptyVec), 0)
        
    def testIndexAtStart(self):
        self.assertEqual(self.bigVec.index(0), 0)
        
    def testIndexAtEnd(self):
        self.assertEqual(self.bigVec.index(BIG_VEC_SIZE-1), BIG_VEC_SIZE -1)
        
    def testIndexDuplicate(self):
        #should return position of first match
        self.assertEqual(self.duplicatesVec.index("b"), 1)
    
    #--------------------------------------------------------
    #TESTING METHODS FOR COUNT
    #--------------------------------------------------------      
               
    #NOT YET IMPLEMENTED!

    #--------------------------------------------------------
    #TESTING METHODS FOR POP
    #--------------------------------------------------------     
    
    def testPop(self):

        testV = Vector()
        for i in range(BIG_VEC_SIZE):
            testV.append(i)

       
        testV.pop(5)
        self.bigVec.pop(5)


        for i in range(10,20):
            testV.pop(i)
            self.bigVec.pop(i)


        self.assertRaises(IndexError, testV.pop, -7)
        self.assertRaises(IndexError, testV.pop, BIG_VEC_SIZE + 5)
        self.assertEqual(testV, self.bigVec)
        
            
        
        

    #--------------------------------------------------------
    #TESTING METHODS FOR REVERSE
    #--------------------------------------------------------      
        
    def testReverse(self):
        testV = Vector()
    

if __name__ == "__main__":
    unittest.main()
