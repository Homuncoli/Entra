* In programming no one-solution fits all, always go for the best solution for your current problem
* most important:
	* performance?: can be needlessly complicated
	* development time: use something already there, or really simple
	* usability: use something you know, 
	* etc...

* Inheritance Model
	* uses OOP inheritance
		* works out of the box
	* slow
	* massive dependency tree
	* doublicate code

* Component Model
	* game object as container for components
	* still uses inheritance
		* all components from base component
		* each component has update method
			* but not every component needs to do something
		* too slow of really high performance systems
			* physics, rendering, collision
		* what if one "system" requires two components
	* piece game object together
	* hard to optimize
	* easy to use wrong
	* code spread all around code base
	* used in Unity, Unreal
		* Unreal even uses inheritance / virtual functions, but is not because it of it

* Entity Component System
	* utilizes a manager
	* components
	    * **have no functionality**^1
		* store data
	* systems
	    * **systems have no state**^1
		* one system does only one thing
		* transform data
	* entities
	    * **entities are essentially ids**
	* Advantages
		* these system cares about these N components
	* Disadvantages
		* terrible if history of values is important

* Implementation
	* explaination #2, code #3
	* Entities
	    * **id**
	     	* id with wrapper class
		* index
	     	* fast, but too much wasted memory in sparse arrays
		* class
	* Systems
		* all systems inherit from one base
		* can be stored as **array of pointers** or as individual objects in the manager
        * variadic template of components as mask
        * keeps track of the component with OnEntityCreated/Destroyed
	* Components
		* should just be an struct
		* need unique identifier
            * maybe manual static is ok
            * maybe use type_info
            * or address of static function
            * or some weird macros
		* owned by manager
            * on the heap with PoolAllocator to ensure continues block


* Data-Oriented Design
	* optimize for the transformation of data
	* sorting alogos, search queries, etc...
	* --> pack data tightly to avoid cache misses

* Data-Driven Design
	* creating systems that are controlled by data

* References
	* [1]: Tim, Overwatch lead dev, GDC talk, about ECS and networking stuff