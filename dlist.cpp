#include <cstddef> 
#include <iostream>


using namespace std;

template<class ItemType>
struct NodeType
{
	ItemType info;
	NodeType<ItemType>*	next;
	NodeType<ItemType>*	back;
};

template <class ItemType>		
DList<ItemType>::DList()		// Class constructor
{
    length = 0;
    head = NULL;
}

template <class ItemType>
DList<ItemType>::~DList   ()		
{
	// Post: List is empty; All items have been deallocated.

}

template <class ItemType>
void DList<ItemType>::makeEmpty()
{
    // Post: List is empty; all items have been deallocated.
	NodeType<ItemType>* tempCur= new NodeType<ItemType>;
	NodeType<ItemType>* tempNxt= new NodeType<ItemType>;
	tempCur=head;
	
	if(tempCur==NULL)
	{
		cout << "Your list is already empty" << endl; 
	}
	
	else

		while(tempCur!=NULL)
		{
			tempNxt=tempCur->next;
			delete tempCur;
			tempCur=tempNxt;
		}
		delete tempNxt;
		delete tempCur;
		head=NULL;
		length=0;	
}

template <class ItemType>
void DList<ItemType>::deleteItem (ItemType item)	
{
	//  Pre :  item to be deleted is passed in via parameter 
        // Post :  item is deleted if it exists in list 
	NodeType<ItemType>* tempFound= new NodeType<ItemType>;
	if(inList(item)==true)
	{
		tempFound=location(item);
		deleteLocation(tempFound);
	}





}

template <class ItemType>
bool DList<ItemType>::inList (ItemType item) const
{
	//  Pre :  item to be located is passed in via parameter 
        // Post :  function returns bool value if item is found 
	bool check=false;
	NodeType<ItemType>* findItem=new NodeType<ItemType>;
	findItem=head;

	if(head==NULL)
		return false;	
	else
		while(findItem!=NULL && check!=true)
		{
			if(findItem-> info==item)
				check=true;
			else
				findItem=findItem->next;
		}		
	
	return check;

}

template <class ItemType>
bool DList<ItemType>::isEmpty() const		
{
	// Post : function returns true if list is empty, false otherwise
	if(head==NULL)
		return true;
	else
		return false;
}

template <class ItemType>
void DList<ItemType>::print() const	
{
	// Pre  : List is not empty 
	// Post : Items in List have been printed to screen
	NodeType<ItemType>* printPtr= new NodeType<ItemType>;
	printPtr=head;

		cout << "Your list: " << " ";
		while (printPtr!=NULL)
		{
			cout << printPtr -> info << " ";
			printPtr=printPtr->next;
		}
		cout << " " << endl;
}
	
template <class ItemType>
void DList<ItemType>::insertHead(ItemType item)	
{
	//  Pre : item to be inserted is passed in via parameter
        // Post : item is inserted at head of list;  Former first node is 
        //        linked back to this new one via double link;
        //        Length incremented;  Special case handled if list is empty 
        NodeType<ItemType>* newNode = new NodeType<ItemType>;
	newNode->info = item;
	newNode->back = NULL;
	if(head!=NULL)
	{
		head -> back=newNode;
		newNode-> next=head;
	}
	else
		newNode->next=NULL;

	head=newNode;
	length++;	
}

template <class ItemType>
void DList<ItemType>::appendTail(ItemType item)
{
	//  Pre :  item to be inserted is passed in via parameter
        // Post :  item is added to tail of list; Former last node
        //         is linked to this new one via double link 
	NodeType<ItemType>* newNode = new NodeType<ItemType>;
	NodeType<ItemType>* temp= new NodeType<ItemType>;
	temp=head;
	newNode->info=item;
	newNode->next=NULL;
	if(head==NULL)
	{
		newNode->back=NULL;
		head=newNode;
	}
	else
	{	
		newNode->back=last();  
		newNode->back->next=newNode;	
	}

	length++;
}

template <class ItemType>
int DList<ItemType>::lengthIs() const	
{
	// Post : Function returns current length of list  
	return length;
}

template <class ItemType>
NodeType<ItemType>* DList<ItemType>::location(ItemType item) const	
{
	//  Pre : item to be located is passed in via parameter 
        // Post : function returns address of item being searched for --
        //        if not found, NULL is returned  


	NodeType<ItemType>* tempLoc= new NodeType<ItemType>;
	tempLoc=head;

	if(head==NULL)
		return NULL;
	if (head->info==item)
		return head;
	else
		while(tempLoc->next!=NULL)
		{
			if(tempLoc->info==item)
				return tempLoc;
			else
				tempLoc=tempLoc->next;
		}
	
	if(last()->info==item)
		return last();
	//return NULL		
}

template <class ItemType>
NodeType<ItemType>* DList<ItemType>::last() const	
{
	// Post : Function returns location of current last item in list
	NodeType<ItemType>* tempLast= new NodeType<ItemType>;
	tempLast=head;
	
	if(head==NULL)
	{
		cout << "Your list is empty" << endl;
		return NULL;	
	}
	else
		while(tempLast->next!=NULL)
			tempLast=tempLast->next;
		return tempLast;
}

template <class ItemType>
void DList<ItemType>::deleteLocation (NodeType<ItemType>* delPtr)	
{

	//  Pre : Item to be deleted exits in list and its location
        //        is passed in via parameter
                   
	// Post : Location passed in is removed from list;  Length
        //        is decremented, and location is deallocated 

        // Special Cases Handled for Deleting Only One Item in List,
        // The Head Item of List, and the Tail Item of List
	
			//deletes if there is only one node in the list
			if((delPtr->back==NULL) && (delPtr->next==NULL))
			{
				head=NULL;	
			}
			
			//deletes from the head of the list
			else if((delPtr->back==NULL) && (delPtr->next!=NULL))  
			{
				delPtr->next->back=NULL;
				head=delPtr->next;
			}

			//deletes the tail of the list
			else if (delPtr->next==NULL)  
			{
				delPtr->back->next=NULL;
			
			}
			//This deletes from the middle of the list or if a node is connected to another node from back and front
			else{ 
				delPtr->back->next=delPtr->next;
				delPtr->next->back=delPtr->back;
			     }
			delete delPtr;
			length--;
	
}			
	
			
		
	


