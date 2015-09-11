""" Implementation of the Stack ADT using a Python list.

(c) 2011 John Wiley & Sons, Data Structures and Algorithms Using
Python, by Rance D. Necaise.

Modified by Nathan Sprague to confrom to JMU CS240 coding conventions.
Also modified for compatibility with Python 2.x.
"""

class Stack(object):
    """ Implementation of the Stack ADT using a Python list."""

    def __init__(self):
        """ Creates an empty stack."""
        self._theItems = list()

    def isEmpty(self):
        """ Returns True if the stack is empty or False otherwise. """
        return len(self) == 0

    def __len__ (self):
        """ Returns the number of items in the stack. """
        return len(self._theItems)

    def peek(self):
        """ Returns the top item on the stack without removing it. """
        assert not self.isEmpty(), "Cannot peek at an empty stack"
        return self._theItems[-1]

    def pop(self):
        """ Removes and returns the top item on the stack. """
        assert not self.isEmpty(), "Cannot pop from an empty stack"
        return self._theItems.pop()

    def push(self, item):
        """ Push an item onto the top of the stack. """
        self._theItems.append(item)
