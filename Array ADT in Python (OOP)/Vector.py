"""
Murphy Cooke
PA2
V1

I received no outside help for this PA other than using my textbook for the course
"""

from t_array import Array

class Vector(object):

    def __init__(self):
        self._index = 0
        self._size = 0
        self._capacity = 2
        self._vector = Array(self._capacity)



    def __len__(self):
        return self._size

    def __contains__(self, item):
        for i in range(len(self)):
            if item is self[i]:
                return True
            else:
                pass

    def __eq__(self, v):
        if type(self) is type(v) and len(self) is len(v):
            for i in range(len(self)):
                if self[i] is not v[i]:
                    return False
                else:
                    return True
        elif not any(self) or not any(v):
            if not any(self) and not any(v):
                return True
            else:
                return False
        else:
            return False


    def __ne__(self, v):
        return (not(self == v))

    def __getitem__(self, indx):

        if (indx >= self._index):
            raise IndexError()
        elif indx < 0:
            newNum = indx + len(self)
            tempItem = self._vector[newNum]
            return tempItem
        else:
            tempItem = self._vector[indx]
            return tempItem

    
    def __setitem__(self, indx, item):

        if (indx > self._index):
            raise IndexError()
        elif indx < 0:
            newNum = indx + len(self)
            tempItem = self._vector[newNum]
            return tempItem
        else:
            if self._vector[indx] == None:
                self._vector[indx] = item
                self._size = self._size + 1
            else:
                self._vector[indx] = item
            

    def __str__(self):
        string = "["
        for i in range(self._size):
            if i == self._size - 1:
                string += str(self._vector[i]) + ""                    
            elif i < self._size:            
                string += str(self._vector[i]) + ", "

        string += "]"

        return string


    def getSlice(self, indx1, indx2):
        if indx1 > indx2 - 1:
            return Vector()
        elif indx1 > self._index or (indx2 - 1) > self._index:
            raise IndexError()
        elif indx1 < 0:
            indx1 = indx1 + (self._index)
            newVec = Vector()

            for i in range((indx2 - 1) - indx1):
                newVec[i + (indx1 - 1)] = self[i + (indx1 -1)]
            return newVec

            
        else:
            newVec = Vector()

            for i in range((indx2 - 1) - indx1):
                newVec[i + (indx1 -1)] = self[i + (indx1 -1)]
            return newVec
            

    def __iter__(self):
        return VectorIter(self)

    def append(self, item):

        if self._size < self._capacity:
            self._vector[self._index] = item
            
        else:
            double = self._capacity * 2
            self._capacity = double
            v1 = Array(double)
            for i in range(len(self)):
                obj = self._vector[i]
                v1[i] = obj
            Array.free(self._vector)
            self._vector = v1
            self._vector[self._index] = item

        self._index = (self._index + 1)
        self._size = (self._size + 1)
            

    def extend(self, items):
        if items is not self:
            for i in range(len(items)):   
                self.append(items[i])
        else:
            raise NotImplementedError()

    def insert(self, indx, item):

        if indx <= self._index and indx >= 0:
            self._vector[indx] = item
        elif indx < 0:
            indx = len(self) + indx
        else:
            raise IndexError()
    def remove(self, item):
        for i in range(len(self)):
            if self._vector[i] is item:
                self._vector[i] = None
                break
    def pop(self, indx = -1):

        if indx is not -1:
        
            if self._vector is None or indx >= self._index:
                raise IndexError()
            else:
                temp = self._vector[indx]
                self._vector[indx] = None
                return temp
        else:
            temp = self._vector[len(self) - 1]
            self._vector[len(self) - 1] = None

    def index(self, item):
        temp = -1
        for x in range(len(self)):
            if self[x] is item and temp is -1:
                temp = x
                
        return temp
    
    def count(self, item):

        times = 0
        
        for x in range(len(self)):
            if item == self[x]:
                times += 1
                
        return times

    def reverse(self):
        v1 = self._vector
        v1.reverse()
        self._vector = v1

    def __del__(self):
        Array.free(self._vector)
        
    def negativeConvert(self, num):
        newNum = num + len(self)
        return newNum

class VectorIter:

    def __init__(self, varray):
        self._arrayRef = varray
        self._curNdx = 0

    def __iter__(self):
        return self

    def next(self):
        if self._curNdx < len(self._arrayRef):
            entry = self._arrayRef[self._curNdx]
            self._curNdx += 1
            return entry
        else:
            raise StopIteration


def main():
    v = Vector()
    for i in range(8):
        v.append(i)

    v.pop()
    v.pop(7)
    v.__setitem__(4, 12)
    v = "goodbye!"



if __name__ == "__main__":
    main()
