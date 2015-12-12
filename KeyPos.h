/**
	* @file KeyPos.h
	* Header file for the key position class
	* @authors Jeff Witthuhn, Ross Kostron, Subodh Bhattarai, Andrew Morris, Alex Gatzke

*/
#include <fstream>
#ifndef KEYPOS
#define KEYPOS
#include <iostream>
using namespace std;

/** \class KeyPos
	\breif The KeyPos class

	A class to hold keys and positions with overloaded operators for comparisons, input and output
*/
template<class obj>
class KeyPos{
	public: 
	obj key; /**< the key */
	int pos; /**< the position */

	/**
		The getKey method
		\return The Key
	*/
	obj getKey() const{
		return key;
	}
	
	/**
		The getPos method
		\return The Position
	*/
	int getPos() const{
		return pos;
	}
	
	/**
		The = method
		\return nothing
	*/
	void operator=(const KeyPos<obj> &kp )
      { 
         key = kp.getKey();
         pos = kp.getPos();
      }
	 
	 /**
		The >= method
		\return true if >=, false if <
	*/
    bool operator>=(const KeyPos<obj> &kp ) const
      { 
        return getKey()>=kp.getKey();
      }
	  
	 /**
		The > method
		\return true if >, false if <=
	*/
    bool operator>(const KeyPos<obj> &kp ) const 
      { 
        return getKey()>kp.getKey();
      }
	  
	/**
		The <= method
		\return true if <=, false if >
	*/
    bool operator<=(const KeyPos<obj> &kp )const 
      { 
        return getKey()<=kp.getKey();
      }
	  
	/**
		The < method
		\return true if <, false if >=
	*/
    bool operator<(const KeyPos<obj> &kp ) const
      { 
        return getKey()<kp.getKey();
      }
	  
	/**
		The friend function for output
		\return output of key and pos
	*/
    friend ostream &operator<<( ostream &output, const KeyPos<obj> &kp ){ 
         output << kp.getPos()<<" "<< kp.getKey();
         return output;            
      }

	/**
		The friend function for input
		\return input of key and pos
	*/
    friend istream &operator>>( istream  &input, KeyPos<obj> &kp )
      { 
         input >> kp.pos >> kp.key;
         return input;            
      }
      private:
};
#endif