use std::collections::LinkedList;

pub struct TestLinkedList {
	container: LinkedList<i8>
}

impl TestLinkedList {
	pub fn new() -> TestLinkedList {
		return TestLinkedList{ container: LinkedList::<i8>::new() }
	}

	// Fills the LinkedList with a specified number of elements that all equal some value
	// Starts by clearing the LinkedList
	// size - number of element to create
	// val - value to initialize each element to
	// this method uses push() to fill the LinkedList
	pub fn fill(&mut self, size: usize, val: i8) {
		// 1.) --- Clear container ---
		self.container.clear();

		// 2.) --- Fill Container ---
		for _i in 0..size {
			self.container.push_back(val);
		}
	}

	// Searches LinkedList for a specified element
	// Performs a linear search till it finds the 1st matching element.
	// Either returns ?pointer? to that element or null if it was not found.
	pub fn find(&mut self, val: i8) -> i8 {
		for element in self.container.iter() {
			if *element == val {
				return *element;
			}
		}

		return 0;
	}

//	pub fn sort(&mut self) {
//		// Index of 1st element of the unsorted region of Vec. 
//		//           V
//		// [ s s s s u u u u u ]
//		let mut unsorted: usize;
//
//		// Iterate through vector
//		for unsorted in (0..self.container.len()) {
//			// Find least element.
//			// Search from unsorted to end of vec and find the index of least element
//			let mut least: usize = unsorted;
//			for i in (unsorted..self.container.len()) {
//				if self.container[i] < self.container[least] {
//					least = i;
//				}
//			} 
//
//			// Swap least element with 1st unsorted element.
//			let temp = self.container[unsorted];
//			self.container[unsorted] = self.container[least];
//			self.container[least] = temp;
//		}
//	}
	
	pub fn increment(&mut self, val: i8) {
		let iter = self.container.iter_mut();

		for element in iter {
			*element += val;
		}
	}

	// Make self constant. "No mut"
	pub fn print(&mut self) {
		for &element in &self.container {
			print!("{} ", element);
		}

		println!("")
	}

	pub fn test_all(&mut self, size: usize) {
		*self = TestLinkedList::new();

		self.fill(size, 100);
		self.increment(2);
		self.find(104);
		self.print();
	}
}