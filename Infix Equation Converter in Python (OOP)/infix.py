"""
Murphy Cooke.
Version 1
10/28/2012

A simple infix equation converter using Stacks and Recursion

I received no outside help on this project. 
"""    
from pyliststack import Stack
   
class BadExpressionError(Exception):
    pass
        
def evalInfixStack(expression):
    """ Evaluate a fully parenthesized infix expression using a stack.

    This function evaluates fully parenthesized  expressions involving
    single digit integers and the operators addition (+), subtraction (-), 
    multiplication (*), integer division (/), and modular division (%).
    
    A BadExpressionError will be raised if the expression contains unrecognized
    characters, or is not well formed.  Spaces and tab characters are ignored.
    
    
    Arguments: expression - a string containing a mathematical expression.
    
    Returns: an integer containing the value of the expression. 
    """ 

    tempchar1 = ""
    
    variables = Stack()
    
    tempchar2 = expression.split()
    characters = tempchar1.join(tempchar2)
    print characters

    for i in characters:

        if not realNum(i) and i not in "+-%*/()":
            
            raise BadExpressionError()
        else:
            
            if realNum(i) or i in "+-%*/":
                variables.push(i)

            if i is ')':
                var2 = variables.pop()
                op = variables.pop()
                var1 = variables.pop()
            
                value = evaluate(op, var1, var2)
                variables.push(value)

    
          
               
    return variables.peek()
        


def evalInfixRecursion(expression):
    """ Evaluate a fully parenthesized infix expression using recursion.

    This function evaluates fully parenthesized  expressions involving
    single digit integers and the operators addition (+), subtraction (-), 
    multiplication (*), integer division (/), and modular division (%).
    
    A BadExpressionError will be raised if the expression contains unrecognized
    characters, or is not well formed.  Spaces and tab characters are ignored.
    
    
    Arguments: expression - a string containing a mathematical expression.
    
    Returns: an integer containing the value of the expression. 
    """
    tempexpression = expression.replace(' ', '')
    strIter = iter(tempexpression)
    value = evalInfixRecursionInner(strIter)

    return value

def evalInfixRecursionInner(iterator):
    """
    Recursive function for solving basic infix equations. 
    """
    character = iterator.next()

    numbers = "012346789"
    operat = "+-*/%"


    if character in numbers or character in operat or character in "()":
        
        if character in numbers:
            return int(character)
        else: 
            try:
                var1 = evalInfixRecursionInner(iterator)
                operator = iterator.next()        
                var2 = evalInfixRecursionInner(iterator)
                endparenth = iterator.next() 
                return evaluate(operator, var1, var2)
            except StopIteration:
                pass
    else:
        raise BadExpressionError()

def evaluate(operator, var1, var2):
    """
    A simple method for evaluating basic equations
    """

    variable1 = int(var1)
    variable2 = int(var2)
    if operator == "/":
        return variable1 / variable2
    elif operator == "*":
        return variable1 * variable2
    elif operator == "+":
        return variable1 + variable2
    elif operator == "%":
        return variable1 % variable2
    else:
        return variable1 - variable2

def realNum(item):
    """
    A simple method for evaluating whether or not an object is an integer
    """
    try:
        int(item)
        return True
    except ValueError:
        return False
def main():
    """ Prompt a user for mathematical expressions and print results twice: 
    once evaluated using a stack, and once evaluated using recursion. 
    """
    
    expression = raw_input("Please Enter an Expression")
    val = evalInfixRecursion(expression)
    val2 = evalInfixStack(expression)

    print(val)
    print(val2)

  
   
if __name__ == "__main__":
    main()
