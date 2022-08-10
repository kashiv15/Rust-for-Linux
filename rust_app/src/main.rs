use std::time::Instant;

mod test_linked_list;
use crate::test_linked_list::TestLinkedList;

mod test_vec;
use crate::test_vec::TestVec;

fn main() {
    println!("");

    let n_elements = 20;

    let mut test_ll: TestLinkedList = TestLinkedList::new();
    test_ll.test_all(n_elements);
    
    let mut test_vec:TestVec = TestVec::new();
    test_vec.test_all(n_elements);
    
    let now = Instant::now();
    
    let elapsed_time = now.elapsed();

    println!("Running slow_function() took {} micro seconds.", elapsed_time.as_micros());
}
