/*
_____________________
|                                        |
|    We’re almost done!!!   |
|____________________|

*/



/*driver*/
/******************************************************************************
cs2123p5Driver.c by Susu’s subjects (Andrew Hutton, Eddie Aviles, Jose Hernandez, Marian Murra Rodriguez)
Purpose:
    This program manages a Price Menu for the Klunker Car Company  using a
    binary tree to represent the information.  It uses input commands to
    manage the tree, print its contents, and print a quote.
Command Parameters:
    n/a
Input:
    DEFINE OPTION szId szSubordinateToId szTitle
        where szId is the ID of this new node,
              szSubordinateToId is the ID of the node that is its
                  parent (via a pChild pointer),
              szTitle is the title for this option (e.g., "Model").
        Assuming the node with szSubordinateToId exists, this inserts a
        new node in the tree.  If a child already exists, follow that child's
        sibling chain until you can insert it at a pointer that would be NULL.
        Note:  if we are inserting at the root, the szSubordinateToId will be "ROOT".
        Print warnings (don't terminate) if the szId already exists or if the
        szSubordinateToId doesn't exist.

    DEFINE VALUE szId szOptionId cCostInd dCost szTitle
        where szId is the ID of this new node,
              szOptionId is the ID of the option which is its parent
                         (via a pChild pointer),
              cCostInd is a character (not a string) representing whether it has a cost,
              dCost is the cost which may be positive or negative,
              szTitle is the title for this option value (e.g., "Racing White").
        Assuming the option node with szOptionId exists, this inserts a new node
        in the tree.  If a child already exists for that Option, follow that child's
        sibling chain until you can insert it at a pointer that would be NULL.
        Print warnings (don't terminate) if the szId already exists or if the
        szOptionId doesn't exist.  If the node containing the szOptionId isn't
        an OPTION, print a warning.

    PRINT ALL
        prints the entire tree in a pretty print style (see sample output).

    PRINT ONE szId
        prints one item
    QUOTE BEGIN
        starts a quote selection.  determineQuote isn't called.
    QUOTE OPTION iLevel szOptionId iSelection
    …
    QUOTE END
        invokes determineQUote passing the quote selction information.
        Example:
        QUOTE BEGIN
        QUOTE OPTION 0 MODEL 1	// selected BASE
        QUOTE OPTION 1 ENGINE_BASE 1	// selected 1.8-liter 4 Cyl Automatic
        QUOTE OPTION 1 COLOR_BASE 2	// selected BLUE
        QUOTE OPTION 1 AUDIO_BASE 2	// selected 8-TRK
        QUOTE OPTION 0 WARRANTY 3	// selected 70k/5yr
        QUOTE END
        It would print a quote:
        MODEL     BASE				        17000
        ENGINE    1.8-liter 4 Cyl Automatic 	0
        COLOR     Blue                         50
        AUDIO     8-Track                    -100
        WARRANTY  70k/5yr                     900
        Total                               17850

    DELETE szId
        This causes the specified node to be deleted
        from the tree.  As a result, its parent should no longer reference it.
        It and its descendants must be freed.  Do not delete its immediate siblings.
        For example, deleting the base should remove it from its parent's value chain,
        but should not cause PLUS and OY to be deleted.
        Print a warning if the szId doesn't already exist.

    *	a comment in the data.  It is only used to explain the data.

Results:
    The commands cause a binary tree to be modified, printed or used to get
    a prie quote.
    Each command is also printed by this simple driver.
Returns:
    0 - normal
    ??
Notes:
    1. This file contains a simple driver and some useful functions.
    2. Project groups will create the rest of the driver in this same file.
    3. Individuals (people not on teams) will use a .o file for the rest of the driver.
    4. Quote Selections can have a maximum of MAX_QUOTE_ITEM  entries.
*******************************************************************************/

// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"

/*
 *This is a temporary function for testing.
 * This function may be deleted
 */
/*
void printNode(NodeT *pRoot, char *szId){
	NodeT *p = findId(pRoot, szId);
	printf("\n\n\nIn printNode:\n\n\n");
	printf("p->element.szId = %s\n",p->element.szId);
	printf("p->element.szTitle = %s\n",p->element.szTitle);
	printf("p->element.cNodeType = %c\n",p->element.cNodeType);
	printf("p->element.cCostInd = %c\n",p->element.cCostInd);
	printf("p->element.dCost = %.2lf\n",p->element.dCost);
}
*/

/******************************findId********************************
NodeT *findId(NodeT *p, char szId[])
Purpose:
	Searches the file for the specified name to return the location.
Parameters:
	I NodeT *p		Sets a node.
	I char szID		Name to find in the file.
Notes:
Return Value:
************************************************************************/

NodeT *findId(NodeT *p, char szId[])
{
	NodeT *pFound = NULL;
	if (p == NULL)
		return NULL;
	if (strcmp(szId, p->element.szId) == 0)
	{
		pFound = p;
		/*
		printf("p->element.szId = %s\n",p->element.szId);
		printf("p->element.szTitle = %s\n",p->element.szTitle);
		printf("p->element.cNodeType = %c\n",p->element.cNodeType);
		printf("p->element.cCostInd = %c\n",p->element.cCostInd);
		printf("p->element.dCost = %lf\n",p->element.dCost);
		*/
		return pFound;
	}
	else
	{
		if(p->pChild != NULL)
			pFound = findId(p->pChild, szId);
		if(pFound != NULL)
			return pFound;
		if (p->pSibling != NULL)
			pFound = findId(p->pSibling, szId);
		if(pFound != NULL)
			return pFound;
	}
	return NULL;
}

/******************************findParent********************************
NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pkid)
Purpose:
	Scans through the nodes and determines the parent node.
Parameters:
	I NodeT *p		Sets a node
	I NodeT *pParent	Sets the parent node
	I NodeT *pkid		Sets the kid node
Notes:
Return Value:
************************************************************************/


NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pkid)
{
	if (p == NULL)
		return NULL;
	if (strcmp(p->element.szId, pkid->element.szId) == 0)
	{
		return pParent;
	}
	else
	{
		if (p->pSibling != NULL)
		{
			pParent = findParent(pParent, p->pSibling, pkid);
			if(pParent != NULL)
			{
				return pParent;
			}
		}
		if(p->pChild != NULL)
		{
			pParent = p;
			pParent = findParent(pParent, p->pChild, pkid);
			if(pParent != NULL)
			{
				return pParent;
			}
		}
	}
	return NULL;
}

/******************************allocNodeT********************************
NodeT *allocNodeT(Element element)
Purpose:
	Allocates memory for elements to create space for the nodes.
Parameters:
	I Element element		Element(s) that are created.
Notes:
Return Value:
************************************************************************/

NodeT *allocNodeT(Element element)
{
	NodeT *pNew;
	pNew = (NodeT *)malloc(sizeof(NodeT));
	if (pNew == NULL)
		ErrExit(ERR_ALGORITHM, "No available memory for queue");
	pNew->element = element;
	pNew->pSibling = NULL;
	pNew->pChild = NULL;
	return pNew;
}

/******************************newElement********************************
Element *newElement(void)
Purpose:
	Allocates memory for new elements
Parameters:
	I Element *newElement	Space for newly created element.
Notes:
Return Value:
************************************************************************/

Element *newElement(void)
{
	Element *element = (Element *)malloc(sizeof(Element));
	element->cCostInd = 'N';
	element->dCost = 0.0;
	return element;
}


/******************************freeSubTree********************************
void freeSubTree(NodeT *p)
Purpose:
	Frees up the memory created by the created tree.
Parameters:
	I NoteT *p		Sets a node.
Notes:
Return Value:
************************************************************************/

void freeSubTree(NodeT *p){
	if(p != NULL)
	{
		freeSubTree(p->pSibling);
		freeSubTree(p->pChild);
		free(p);
	}
	return;
}

/*************************insertPriceMenu***************************
void insertPriceMenu(Tree tree, Element element, char *szOptionId)
Purpose:
	Creates memory for a tree. Builds a tree whose nodes contain the prices of the menu.
Parameters:
	I Tree tree			Sets the tree.
	I Element element		Sets the elements.
	I char *szOptionId		Text of price in menu.
Notes:
Return Value:
************************************************************************/

void insertPriceMenu(Tree tree, Element element, char *szOptionId)
{
	NodeT *pNew = allocNodeT(element);
	if(tree->pRoot != NULL)
	{
		NodeT *pParent = findId(tree->pRoot, szOptionId);
		if(pParent != NULL)
		{
			//printf("goes through the p != NULL \n");
			if(pParent->pChild != NULL)
			{
				if(pParent->pChild->pSibling == NULL)
				{
					//first sibling
					pParent->pChild->pSibling = pNew;
				}
				else
				{
					//not first sibling
					Element tmpElement;
					NodeT *tmpSibling = allocNodeT(tmpElement);
					tmpSibling->pSibling = pParent->pChild->pSibling;
					while(tmpSibling->pSibling != NULL)
					{
						tmpSibling = tmpSibling->pSibling;
					}
					tmpSibling->pSibling = pNew;
				}
				//printf("pParent->pChild->pSibling->element.szId == %s;\n",pParent->pChild->pSibling->element.szId);
			}
			else
			{
				if(element.cNodeType == 'V' && pParent->element.cNodeType != 'O')
				{
					printf("Warning: OPTION VALUE must have an OPTION as its parent, found '%s'\n", pParent->element.szId);
					return;
				}
				pParent->pChild = pNew;
			}
		}
		else{
			printf("Error: The specified parent ID (%s) was not found for the value \"%s\"\n",szOptionId,pNew->element.szId);
		}
		/*
		pNew = allocNodeT(p->element);
		tree->pRoot->pChild = pNew;
		printf("new root child node has been inserted\n");
		*/
	}
	else
	{
		tree->pRoot = pNew;
	}
}


/******************** newTree **************************************
  Tree newTree()
Purpose:
    Allocates memory for a TreeImp and initializes it.
Parameters:
    n/a
Notes:
    - Checks for malloc memory allocation error.
Returns:
    Returns a Tree.  Note that a Tree is simply a pointer to
    a TreeImp.
**************************************************************************/
Tree newTree()
{
	Tree tree = (Tree)malloc(sizeof(TreeImp));
	if (tree == NULL)
		ErrExit(ERR_ALGORITHM, "malloc allocation error for TreeImp");
	Element *pElement = newElement();
	strcpy(pElement->szId, "ROOT");
	strcpy(pElement->szTitle, "root");
	tree->pRoot = allocNodeT(*pElement);
	return tree;
}

/******************** newQuoteSelection **************************************
  QuoteSelection newQuoteSelection()
Purpose:
    Allocates memory for a QuoteSelectionImp and initializes it.
Parameters:
    n/a
Notes:
    - Checks for malloc memory allocation error.
Returns:
    Returns a QuoteSelection.  Note that a QuoteSelection is simply a pointer to
    a QuoteSelectionImp.
**************************************************************************/
QuoteSelection newQuoteSelection()
{
	QuoteSelection  quote= (QuoteSelection)malloc(sizeof(QuoteSelectionImp));
	if (quote == NULL)
		ErrExit(ERR_ALGORITHM, "malloc allocation error for QuoteSelectionImp");
	quote->iQuoteItemCnt = 0;
	return quote;
}

/*************************define***************************
void define(Tree tree, QuoteSelection quote, char szInputBuffer[])
Purpose:

Parameters:
	I Tree tree			Sets the tree.
	I QuoteSelection quote	Allocates memory for a quote in QuoteSelectionImp.
I char szInputBuffer
Notes:
Return Value:
************************************************************************/
void define(Tree tree, QuoteSelection quote, char szInputBuffer[])
{
	char definition[MAX_TOKEN_SIZE + 1];
	Element *element = newElement();
	
	szInputBuffer = getToken(szInputBuffer, definition, MAX_TOKEN_SIZE);
	if(strcmp(definition, "OPTION") == 0)
	{
		element->cNodeType = 'O';
		
		char szId[MAX_ID_SIZE  + 1];
		szInputBuffer = getToken(szInputBuffer, szId, MAX_ID_SIZE );
		strcpy(element->szId, szId);
		
		char szSubordinateToId[MAX_ID_SIZE  + 1];
		szInputBuffer = getToken(szInputBuffer, szSubordinateToId, MAX_ID_SIZE );
		
		char szTitle[MAX_TOKEN_SIZE + 1];
		szInputBuffer = getToken(szInputBuffer, szTitle, MAX_TOKEN_SIZE);
		
		strcpy(element->szTitle, szTitle);
		
		insertPriceMenu(tree, *element, szSubordinateToId);
	}
	if(strcmp(definition, "VALUE") == 0)
	{
		element->cNodeType = 'V';
		
		char szID[MAX_ID_SIZE+1];
		szInputBuffer = getToken(szInputBuffer, szID, MAX_TOKEN_SIZE + 1);
		strcpy(element->szId, szID);
		
		char szSubordinateToId[MAX_ID_SIZE  + 1];
		szInputBuffer = getToken(szInputBuffer, szSubordinateToId, MAX_ID_SIZE );
		
		char cCostInd[MAX_TOKEN_SIZE+1];
		szInputBuffer = getToken(szInputBuffer, cCostInd, MAX_TOKEN_SIZE + 1);
		if(strcmp(cCostInd, "Y") == 0)
		{
			element->cCostInd = 'Y';
			char sCost[MAX_TOKEN_SIZE+1];
			szInputBuffer = getToken(szInputBuffer, sCost, MAX_TOKEN_SIZE + 1);
			element->dCost = atof(sCost);
			
			//char szTitle[MAX_TOKEN_SIZE+1];
			//szInputBuffer = getToken(szInputBuffer, szTitle, MAX_TOKEN_SIZE + 1);
			
			strcpy(element->szTitle, szInputBuffer);
			element->szTitle[strlen(element->szTitle) -2] = '\0';
		}
		else
		{
			printf("Error: Value %s has no cost\n",element->szId);
		}
		insertPriceMenu(tree, *element, szSubordinateToId);
	}

}


/*************************printOne***************************
void printOne(Tree tree, char szId[])
Purpose:
	Prints the
Parameters:
	I Tree tree		Sets the tree.
	I char szId		Text of the identification of parts.
Notes:
Return Value:
************************************************************************/

void printOne(Tree tree, char szId[])
{
	NodeT *p = findId(tree->pRoot, szId);
	if(p != NULL)
	{
	printf("cNodeType: %c\nszId: %s\nszTitle: %s\ncCostInd: %c\ndCost: %.2lf\n",p->element.cNodeType,p->element.szId,p->element.szTitle,p->element.cCostInd,p->element.dCost);
	}
	else
	{
		printf("Error: node \"%s\" not found\n",szId);
	}
	return;
}

/*************************printQuote***************************
void printQuote(QuoteSelection quote, int indent)
Purpose:
	Prints the quote.
Parameters:
	I QuoteSelection quote
	I int indent
Notes:
Return Value:
************************************************************************/


void printQuote(Tree tree, QuoteSelection quote)
{
	int i;
	int tab;
	double dTotalCost = 0.0;
	NodeT *pOption;
	NodeT *pValue;
	for(i = 0; i < quote->iQuoteItemCnt; i++)
	{
		for(tab = 0; tab < quote->quoteItemM[i].iLevel; tab++)
		{
			printf("\t\t");
		}
		pOption = findId(tree->pRoot, quote->quoteItemM[i].szOptionId);
		pValue = pOption->pChild;
		if(pValue == NULL)
		{
			printf("Error: quote option has no child value\n");
			return;
		}
		for(tab = 1; tab < quote->quoteItemM[i].iSelection; tab++)
		{
			if(pValue != NULL)
				pValue = pValue->pSibling;
		}
		if(pValue == NULL)
		{
			printf("Error: trying to reference a nonexistent sibling in printQuote\n");
			return;
		}
		
		printf("%s\n", pOption->element.szTitle);
		for(tab = 0; tab < quote->quoteItemM[i].iLevel; tab++)
		{
			printf("\t\t");
		}
		printf("\t%s", pValue->element.szTitle);
		tab *= 2;
		tab += strlen(pValue->element.szTitle)/8;
		while(tab < 8)
		{
			printf("\t");
			tab++;
		}
		printf("%.2lf\n", quote->quoteItemM[i].dCost);
		dTotalCost += quote->quoteItemM[i].dCost;
	}
	printf("\tTotal is\t\t\t\t\t%.2lf\n",dTotalCost);
	return;
}
/************************printPriceMenu***************************
void printPriceMenu(NodeT *p, int indent)
Purpose:
	Prints the prices listed in the menu.
Parameters:
	I NodeT *p		Sets a node.
I int indent
Notes:
Return Value:
************************************************************************/

void printPriceMenu(NodeT *p, int indent)
{
	if(p != NULL){
		int i;
		for(i = 0; i < indent; i++)
		{
			printf("\t");
		}
		printf("%s", p->element.szTitle);
		i += strlen(p->element.szTitle)/8;
		while(i < 8)
		{
			printf("\t");
			i++;
		}
		printf("%.2lf\n", p->element.dCost);
		printPriceMenu(p->pChild, (indent+1));
		printPriceMenu(p->pSibling, indent);
	}
	return;
}

/*************************determineQuote***************************
QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
Purpose:

Parameters:
	I Tree tree
	I QuoteSelection quoteSelection
Notes:
Return Value:
************************************************************************/
QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
{
	/*
	char *option;
	sscanf(szInputBuffer, "%s ", option);
	
	char *iLevel;
	szInputBuffer = getToken(szInputBuffer, iLevel, MAX_TOKEN_SIZE + 1);
	
	char *szOptionId;
	szInputBuffer = getToken(szInputBuffer, szOptionId, MAX_TOKEN_SIZE + 1);
	
	char *iSelection;
	szInputBuffer = getToken(szInputBuffer, iSelection, MAX_TOKEN_SIZE + 1);
	
	quote->quoteItemM[quote->iQuoteItemCnt].iLevel = atoi(iLevel);
	strcpy(quote->quoteItemM[quote->iQuoteItemCnt].szOptionId, szOptionId);
	quote->quoteItemM[quote->iQuoteItemCnt].iSelection = atoi(iSelection);
	NodeT *p = findId(tree->pRoot, szOptionId);
	quote->quoteItemM[quote->iQuoteItemCnt].dCost = p->element.dCost;
	quote->iQuoteItemCnt++;
	*/
	QuoteResult quoteResult;
	quoteResult.dTotalCost = 0.0;
	quoteResult.returnCode = QUOTE_NORMAL;
	return quoteResult;
}

/*************************quote***************************
void quote(Tree tree, QuoteSelection quote, char szInputBuffer[])
Purpose:
	Prints the quote.
Parameters:
	I Tree tree
	I QuoteSelection quote
	I int szInputBuffer
Notes:
Return Value:
************************************************************************/
void quote(Tree tree, QuoteSelection quote, char szInputBuffer[])
{
	char option[MAX_ID_SIZE+1];
	szInputBuffer = getToken(szInputBuffer, option, MAX_TOKEN_SIZE + 1);
	
	if(strcmp(option, "BEGIN") == 0)
	{
		//quote = newQuoteSelection();
		//do nothing here
	}
	else if(strcmp(option, "OPTION") == 0)
	{
		char iLevel[MAX_TOKEN_SIZE];
		szInputBuffer = getToken(szInputBuffer, iLevel, MAX_TOKEN_SIZE + 1);
		
		char szOptionId[MAX_TOKEN_SIZE];
		szInputBuffer = getToken(szInputBuffer, szOptionId, MAX_TOKEN_SIZE + 1);
		
		char iSelection[MAX_TOKEN_SIZE];
		szInputBuffer = getToken(szInputBuffer, iSelection, MAX_TOKEN_SIZE + 1);
		
		quote->quoteItemM[quote->iQuoteItemCnt].iLevel = atoi(iLevel);
		strcpy(quote->quoteItemM[quote->iQuoteItemCnt].szOptionId, szOptionId);
		quote->quoteItemM[quote->iQuoteItemCnt].iSelection = atoi(iSelection);
		
		//printNode(tree->pRoot, quote->quoteItemM[quote->iQuoteItemCnt].szOptionId);
		
		NodeT *p = findId(tree->pRoot, szOptionId);
		if(p == NULL)
		{
			printf("Error: Quote option id not found\n");
			return;
		}
		
		p = p->pChild;
		if(p == NULL)
		{
			printf("Error: quote option has no child value\n");
			return;
		}
		int i;
		for(i = 1; i < quote->quoteItemM[quote->iQuoteItemCnt].iSelection; i++)
		{
			if(p != NULL)
				p = p->pSibling;
		}
		if(p == NULL)
		{
			printf("Error: trying to reference a nonexistent sibling\n");
			//printf("Error: trying to reference a nonexistent sibling of node %s\n",p->element.szId);
			return;
		}
		quote->quoteItemM[quote->iQuoteItemCnt].dCost = p->element.dCost;
		quote->iQuoteItemCnt++;
	}
	else if(strcmp(option, "END") == 0)
	{
		printQuote(tree, quote);
		//QuoteResult result = determineQuote(tree, quote);
	}
	else
		printf("Error: unrecognized quote command \"%s\"\n", option);
}

/*************************freeTree********************************
void freeTree(Tree tree)
Purpose:
	Frees the tree calling the freeSubTree function.
Parameters:
	I	Tree tree		Tree that will be freed
Notes:
	N/A
Return value:
	void function
*****************************************************************/
void freeTree(Tree tree){
	if(tree->pRoot != NULL){
		freeSubTree(tree->pRoot);
		free(tree);
	}
}

void deleteItem(Tree tree, char szId[])
{
	NodeT *p = findId(tree->pRoot, szId);
	NodeT *tmpNode = findParent(NULL, tree->pRoot, p);
	if(tmpNode != NULL)
	{
		if(strcmp(tmpNode->pChild->element.szId, szId) == 0)
		{
			tmpNode->pChild = p->pSibling;
			printf("Success: node \"%s\" deleted\n",szId);
			freeSubTree(p->pChild);
			free(p);
			return;
		}
		else
		{
			tmpNode = tmpNode->pChild;
			while(tmpNode->pSibling != NULL && strcmp(tmpNode->pSibling->element.szId, p->element.szId) != 0)
			{
				tmpNode = tmpNode->pSibling;
			}
			if(tmpNode->pSibling == NULL)
				printf("Error: item \"%s\" not deleted because it could not be found\n",szId);
			if(tmpNode == NULL)
			{
				printf("Error: node \"%s\" not deleted\n",szId);
				return;
			}
			tmpNode->pSibling = p->pSibling;
			freeSubTree(p->pChild);
			free(p);
			return;
		}
	}
	else
	{
		printf("Segmentation fault: 11\nJust kidding!!!!!\n");
	}
}

/*************************delete***************************
void delete(Tree tree, QuoteSelection quote, char szInputBuffer[])
Purpose:
	Frees up memory of the tree.
Parameters:
	I Tree tree
	I QuoteSelection quote
	I char szInputBuffer
Notes:
	Does not to be more interesting...
Return Value:
************************************************************************/
void delete(Tree tree, QuoteSelection quote, char *szInputBuffer)
{
	
	char szId[MAX_TOKEN_SIZE +  1];
	szInputBuffer = getToken(szInputBuffer, szId, MAX_TOKEN_SIZE + 1);
	deleteItem(tree, szId);
}

/*************************print***************************
void print(Tree tree, char *szInputBuffer)
Purpose:
	Prints.
Parameters:
	I Tree tree
I char *szInputBuffer
Notes:
Return Value:
************************************************************************/

void print(Tree tree, char *szInputBuffer)
{
	char printType[MAX_TOKEN_SIZE + 1];
	szInputBuffer = getToken(szInputBuffer, printType, MAX_TOKEN_SIZE);
	if(strcmp(printType, "ALL") == 0)
	{
		printPriceMenu(tree->pRoot, 0);
	}
	else if(strcmp(printType, "ONE") == 0)
	{
		char szId[MAX_TOKEN_SIZE +  1];
		szInputBuffer = getToken(szInputBuffer, szId, MAX_TOKEN_SIZE + 1);
		printOne(tree, szId);
	}
	else
	{
		printf("Error: unrecognized print type: %s\n",printType);
	}
}

/*************************processCommand***************************
void processCommand(Tree tree, QuoteSelection quoteSelection, char szInputBuffer[])
Purpose:

Parameters:
	I Tree tree
I QuoteSelection quoteSelection
I char szInputBuffer
Notes:
Return Value:
************************************************************************/

void processCommand(Tree tree, QuoteSelection quoteSelection, char szInputBuffer[])
{
	char command[MAX_TOKEN_SIZE];
	command[0] = '\0';
	szInputBuffer = getToken(szInputBuffer, command, MAX_TOKEN_SIZE + 1);
	if(strcmp(command, "DEFINE") == 0)
	{
		define(tree, quoteSelection, szInputBuffer);
	}
	else if(strcmp(command, "PRINT") == 0)
	{
		print(tree, szInputBuffer);
	}
	else if(strcmp(command, "QUOTE") == 0)
	{
		quote(tree, quoteSelection, szInputBuffer);
	}
	else if(strcmp(command, "DELETE") == 0)
	{
		delete(tree, quoteSelection, szInputBuffer);
	}
	else
	{
		printf("Unrecognized command: %s\n", command);
	}
}

int main(void)
{
	Tree tree = newTree();                         // Binary tree
	char szInputBuffer[MAX_LINE_SIZE + 1];          // input text line
	char *pszToken = "";

	// Variables for Quote
	QuoteSelection quoteSelection = newQuoteSelection();

	// Read command lines until EOF

	while (fgets(szInputBuffer, MAX_LINE_SIZE, stdin) != NULL)
	{
		printf("%s", szInputBuffer);

		// If the line is just a comment or empty, ignore it
		if (szInputBuffer[0] == '*' || szInputBuffer[0] == '\0')
			continue;                               // Command is a comment so skip it

		processCommand(tree, quoteSelection, szInputBuffer);
	}
	freeTree(tree);
	free(quoteSelection);
	fclose(stdin);
	printf("Execution complete, program exiting.\n");
	return 0;
}

/***  U T I L I T Y functions ***/

/******************** ErrExit **************************************
  void ErrExit(int iexitRC, char szFmt[], ... )
Purpose:
    Prints an error message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of
    arguments after szFmt varies dependent on the format codes in
    szFmt.
    It also exits the program with the specified exit return code.
Parameters:
    I   int iexitRC             Exit return code for the program
    I   char szFmt[]            This contains the message to be printed
                                and format codes (just like printf) for
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt.
Notes:
    - Prints "ERROR: " followed by the formatted error message specified
      in szFmt.
    - Prints the file path and file name of the program having the error.
      This is the file that contains this routine.
    - Requires including <stdarg.h>
Returns:
    Returns a program exit return code:  the value of iexitRC.
**************************************************************************/
void ErrExit(int iexitRC, char szFmt[], ... )
{
    va_list args;               // This is the standard C variable argument list type
    va_start(args, szFmt);      // This tells the compiler where the variable arguments
                                // begins.  They begin after szFmt.
    printf("ERROR: ");
    vprintf(szFmt, args);       // vprintf receives a printf format string and  a
                                // va_list argument
    va_end(args);               // let the C environment know we are finished with the
                                // va_list argument
    printf("\n");
    exit(iexitRC);
}
/******************** warning **************************************
  void warning(char szFmt[], ... )
Purpose:
    Prints an warning message defined by the printf-like szFmt and the
    corresponding arguments to that function.  The number of
    arguments after szFmt varies dependent on the format codes in
    szFmt.
Parameters:
    I   char szFmt[]            This contains the message to be printed
                                and format codes (just like printf) for
                                values that we want to print.
    I   ...                     A variable-number of additional arguments
                                which correspond to what is needed
                                by the format codes in szFmt.
Notes:
    - Prints "Warning: " followed by the formatted warning message specified
      in szFmt.
    - Usually after using the warning function, your code should return.
    - Requires including <stdarg.h>
Returns:
    n/a
**************************************************************************/
void warning(char szFmt[], ... )
{
    va_list args;               // This is the standard C variable argument list type
    va_start(args, szFmt);      // This tells the compiler where the variable arguments
                                // begins.  They begin after szFmt.
    printf("Warning: ");
    vprintf(szFmt, args);       // vprintf receives a printf format string and  a
                                // va_list argument
    va_end(args);               // let the C environment know we are finished with the
                                // va_list argument
    printf("\n");
}
/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    In general, this routine optionally prints error messages and diagnostics.
    It also prints usage information.

    If this is an argument error (iArg >= 0), it prints a formatted message
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits
    with ERR_COMMAND_LINE.

    If this is a usage error (but not specific to the argument), it prints
    the specific message and its supplemental diagnostic information.  It
    also shows the usage and exist with ERR_COMMAND_LINE.

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript or control:
                                    > 0 - command argument subscript
                                    0 - USAGE_ONLY - show usage only
                                    -1 - USAGE_ERR - show message and usage
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    switch (iArg)
    {
        case USAGE_ERR:
            fprintf(stderr, "Error: %s %s\n"
                , pszMessage
                , pszDiagnosticInfo);
            break;
        case USAGE_ONLY:
            break;
        default:
            fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
                , iArg
                , pszMessage
                , pszDiagnosticInfo);
    }
    // print the usage information for any type of command line error
    fprintf(stderr, "p5 \n");
    if (iArg == USAGE_ONLY)
        exit(USAGE_ONLY);
    else
        exit(ERR_COMMAND_LINE);
}

/******************** getToken **************************************
char * getToken (char *pszInputTxt, char szToken[], int iTokenSize)
Purpose:
   Examines the input text to return the next token.  It also
   returns the position in the text after that token.  This function
   does not skip over white space, but it assumes the input uses
   spaces to separate tokens.
Parameters:
   I   char *pszInputTxt       input buffer to be parsed
   O   char szToken[]          Returned token.
   I   int iTokenSize          The size of the token variable.  This is used
                               to prevent overwriting memory.  The size
                               should be the memory size minus 1 (for
                               the zero byte).
Returns:
   Functionally:
       Pointer to the next character following the delimiter after the token.
       NULL - no token found.
   szToken parm - the returned token.  If not found, it will be an
       empty string.
Notes:
   - If the token is larger than iTokenSize, we return a truncated value.
   - If a token isn't found, szToken is set to an empty string
   - This function does not skip over white space occurring prior to the token.
**************************************************************************/
char * getToken(char *pszInputTxt, char szToken[], int iTokenSize)
{
   int iDelimPos;                      // found position of delim
   int iCopy;                          // number of characters to copy
   char szDelims[20] = " \n\r";        // delimiters
   szToken[0] = '\0';
   // check for NULL pointer
   if (pszInputTxt == NULL)
       ErrExit(ERR_ALGORITHM
               , "getToken passed a NULL pointer");

   // Check for no token if at zero byte
   if (*pszInputTxt == '\0')
       return NULL;

   // get the position of the first delim
   iDelimPos = strcspn(pszInputTxt, szDelims);

   // if the delim position is at the first character, return no token.
   if (iDelimPos == 0)
       return NULL;

   // see if we have more characters than target token, if so, trunc
   if (iDelimPos > iTokenSize)
       iCopy = iTokenSize;             // truncated size
   else
       iCopy = iDelimPos;

   // copy the token into the target token variable
   memcpy(szToken, pszInputTxt, iCopy);
   szToken[iCopy] = '\0';              // null terminate

   // advance the position
   pszInputTxt += iDelimPos;
   if (*pszInputTxt == '\0')
       return pszInputTxt;
   else
       return pszInputTxt + 1;
}