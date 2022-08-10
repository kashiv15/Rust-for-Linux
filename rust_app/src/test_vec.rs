use std::vec::Vec;

pub struct TestVec {
	container: Vec<i8>
}

impl TestVec {
	pub fn new() -> TestVec {
		return TestVec{ container: Vec::<i8>::new() };
	}

	// Fills the Vec with a specified number of elements that all equal some value
	// Starts by clearing the Vec
	// size - number of element to create
	// val - value to initialize each element to
	// this method uses push() to fill the Vec
	pub fn fill(&mut self, size: usize, val: i8) {
		// 1.) --- Clear container ---
		self.container.clear();

		let mut i: usize = 0;
		
		// 2.) --- Fill container ---
		while i < size {
			self.container.push(val);

			i += 1;
		}
	}

	// Searches Vec for a specified element
	// Performs a linear search till it finds the 1st matching element.
	// returns the index of that element.
	pub fn find(&mut self, val: i8) -> usize {
		let mut i: usize = 0;

		while i < self.container.len() && self.container[i] != val {
			i += 1;
		}

		return i;
	}

	pub fn sort(&mut self) {
		// Iterate through vector
		// unsorted is the index of the 1st element of the unsorted region of Vec. 
		for unsorted in 0..self.container.len() {
			// Find least element.
			// Search from unsorted to end of vec and find the index of least element
			let mut least: usize = unsorted;
			for i in unsorted..self.container.len() {
				if self.container[i] < self.container[least] {
					least = i;
				}
			} 

			// Swap least element with 1st unsorted element.
			let temp = self.container[unsorted];
			self.container[unsorted] = self.container[least];
			self.container[least] = temp;
		}
	}
	
	pub fn increment(&mut self, val: i8) {
		for i in 0..self.container.len() {
			self.container[i] += val;
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
		*self = TestVec::new();

		self.fill(size, 100);
		self.increment(2);
		self.sort();
		self.find(104);
		self.print();
	}
}