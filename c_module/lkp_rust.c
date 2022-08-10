#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hashtable.h>
#include <linux/rbtree.h>
#include <linux/xarray.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/vmalloc.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/********************************** Module Setup **********************************/

static int num_elements;
static int num_tries;

struct time_data {
    unsigned long long fill_time;
    unsigned long long find_time;
    unsigned long long inc_time;
};

static struct time_data list_time;
static struct time_data hash_time;
static struct time_data tree_time;
static struct time_data xarray_time;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tom Tase");
MODULE_DESCRIPTION("LKP Final Project");

module_param(num_elements, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(num_elements, "Number of elements to store in the data structures");

module_param(num_tries, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(num_tries, "Number of test trials to perform");

/****************************** Init Data Structures ******************************/
static LIST_HEAD(mylist);

#define MYHASH_BITS 5
static DEFINE_HASHTABLE(myhash, MYHASH_BITS);

struct rb_root mytree = RB_ROOT;

DEFINE_XARRAY(my_xarray);
static unsigned long num_items_x = 0;

/******************************** Init Data Entries ********************************/

struct entry {
    int val;
    struct list_head list;
    struct hlist_node hash;
    struct rb_node node;
};

/****************************** Fuction Prototypes ******************************/

static int file_show(struct seq_file * m, void * v);
static int file_open(struct inode * inode, struct file * file);
static void test_insert(struct seq_file * m, int elem);
static void test_find(struct seq_file * m, int elem);
static void test_incrament(struct seq_file * m, int elem);
static void cleanup(void);

/******************************** Proc File Output *******************************/

static const struct proc_ops lkp_rust_ops = {
	.proc_open = file_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int file_show(struct seq_file * m, void * v) {

    int i;

    for(i = 0; i < num_tries; i++) {

        test_insert(m, num_elements);
        test_find(m, num_elements);
        test_incrament(m, num_elements);
        cleanup();

    }

    seq_printf(m, "Avg time for Linked list insert for %i elements took %llu ticks\n", num_elements, list_time.fill_time / num_tries);
    seq_printf(m, "Avg time for Hash Table insert for %i elements took %llu ticks\n", num_elements, hash_time.fill_time / num_tries);
    seq_printf(m, "Avg time for RB Tree insert for %i elements took %llu ticks\n", num_elements, tree_time.fill_time / num_tries);
    seq_printf(m, "Avg time for XArray insert for %i elements took %llu ticks\n", num_elements, xarray_time.fill_time / num_tries);
    
    seq_printf(m, "Avg time for Linked list find for %i elements took %llu ticks\n", num_elements, list_time.find_time / num_tries);
    seq_printf(m, "Avg time for Hash Table find for %i elements took %llu ticks\n", num_elements, hash_time.find_time / num_tries);
    seq_printf(m, "Avg time for RB Tree find for %i elements took %llu ticks\n", num_elements, tree_time.find_time / num_tries);
    seq_printf(m, "Avg time for XArray find for %i elements took %llu ticks\n", num_elements, xarray_time.find_time / num_tries);
    
    seq_printf(m, "Avg time for Linked list increment for %i elements took %llu ticks\n", num_elements, list_time.inc_time / num_tries);
    seq_printf(m, "Avg time for Hash Table increment for %i elements took %llu ticks\n", num_elements, hash_time.inc_time / num_tries);
    seq_printf(m, "Avg time for RB Tree increment for %i elements took %llu ticks\n", num_elements, tree_time.inc_time / num_tries);
    seq_printf(m, "Avg time for XArray increment for %i elements took %llu ticks\n", num_elements, xarray_time.inc_time / num_tries);

    return 0;
}

static int file_open(struct inode * inode, struct file * file) {
    return single_open(file, file_show, NULL);
}

/****************************** Linked List ******************************/

static int fill_linked_list(int size) {

    int i;

    for(i = 0; i < size; i++) {

        struct entry * new_list_entry = vmalloc(sizeof(new_list_entry));

        if(new_list_entry == NULL) {
		    return -ENOMEM;
	    }

        new_list_entry -> val = i;

        list_add_tail(&(new_list_entry -> list), &mylist);
    }

    return 0;

}

static void find_in_list(int val) {

    struct entry * cur;

    list_for_each_entry(cur, &mylist, list) {
		if(cur -> val == val) {
            printk(KERN_INFO "Found %i", val);
            return;
        }
	}

}

static void loop_and_increment_hash_table(void) {
    
    struct entry * cur;

    list_for_each_entry(cur, &mylist, list) {
		cur -> val ++;
	}

}

static void destroy_linked_list_and_free(void)
{

	struct entry * current_entry, * next;

	list_for_each_entry_safe(current_entry, next, &mylist, list) {
		list_del(&(current_entry -> list));
		vfree(current_entry);
	}
}

/****************************** Hash Table *******************************/

static int fill_hash_table(int size) {

    int i;

    for(i = 0; i < size; i++) {

        struct entry * new_hash_entry = vmalloc(sizeof(new_hash_entry));

        if(new_hash_entry == NULL) {
		    return -ENOMEM;
	    }

        new_hash_entry -> val = i;

        hash_add(myhash, &new_hash_entry -> hash, (long)i);
    }

    return 0;
}

static void find_in_hash_table(int val) {

    struct entry * cur;
	unsigned bkt;

    hash_for_each(myhash, bkt, cur, hash) {
		if(cur -> val == val) {
            printk(KERN_INFO "Found %i", val);
            return;
        }
	}

}

static void loop_and_increment_list(void) {
    
    struct entry * cur;
	unsigned bkt;

    hash_for_each(myhash, bkt, cur, hash) {
		cur -> val ++;
	}

}

static void destroy_hash_table_and_free(void)
{

	struct entry * cur;
	struct hlist_node * node;
	unsigned bkt;

	hash_for_each_safe(myhash, bkt, node, cur, hash) {
		hash_del(&(cur -> hash));
		vfree(cur);
	}

}

/******************************** RB Tree ********************************/

void rb_tree_insert(struct rb_root *root, struct entry *data)
{

	struct rb_node **new = &(root -> rb_node), *parent = NULL;
	struct entry *tree_entry;

	while(*new) {

		parent = *new;
		tree_entry = rb_entry(parent, struct entry, node);
		if(data -> val < tree_entry -> val) {
			new = &parent -> rb_left;
		} else {
			new = &parent -> rb_right;
		}
	}

	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);

}

struct entry * search(struct rb_root * root, int val) {

	struct rb_node *node = root -> rb_node;
	struct entry * entry;

	while(node) {

		entry = rb_entry(node, struct entry, node);
		
		if(entry -> val != val) {
			if(val < entry -> val) {
				node = node -> rb_left;
			} else {
				node = node -> rb_right;
			}
		}
		else {

			return entry;
		}

	}

	return NULL;

}

static int fill_rb_tree(int size) {

    int i;

    for(i = 0; i < size; i++) {

        struct entry * new_rbtree_entry = vmalloc(sizeof(new_rbtree_entry));

        if(new_rbtree_entry == NULL) {
		    return -ENOMEM;
	    }

        new_rbtree_entry -> val = i;

        rb_tree_insert(&mytree, new_rbtree_entry);
    }

    return 0;
}

static void find_in_rb_tree(int val) {

    struct entry * rval;

    rval = search(&mytree, val);

    printk(KERN_INFO "Found %i", rval -> val);

}

static void loop_and_incrament_tree(struct rb_root * root) {

    struct rb_node *tmp;
	struct entry *entry;

	for(tmp = rb_first(root); tmp; tmp = rb_next(tmp)) {
		
		entry = rb_entry(tmp, struct entry, node);
        entry -> val ++;

    }

}

void rb_tree_destroy_and_free(struct rb_root * root) {

	struct rb_node *tmp;
	struct entry *entry;

	for(tmp = rb_first(root); tmp; tmp = rb_next(tmp)) {
		
		entry = rb_entry(tmp, struct entry, node);
		rb_erase(&entry->node, root);
		vfree(entry);
	}
}

/********************************* Xarray ********************************/

static int fill_xarray(int size) {

    int i;

    for(i = 0; i < size; i++) {

        xa_store(&my_xarray, num_items_x, xa_mk_value(i), GFP_KERNEL);
	    num_items_x ++;
    }

    return 0;
}

static void find_in_xarray(int val) {

    void * entry;
	unsigned long index = 0;

    xa_for_each(&my_xarray, index, entry) {
		if((int)xa_to_value(entry) == val) {
            printk(KERN_INFO "Found %i", (int)xa_to_value(entry));
            return;
        }
	}
}

static void loop_and_incrament_xarray(void) {

    void * entry;
	unsigned long index = 0;

    xa_for_each(&my_xarray, index, entry) {
		//(int)xa_to_value(entry) ++;

        *entry ++;
	}
}

static void destroy_xarray_and_free(void) {

	void * entry;
	unsigned long index = 0;

	xa_for_each(&my_xarray, index, entry) {
		xa_erase(&my_xarray, index);
	}
}

/**************************** Test Functions ****************************/

static void test_insert(struct seq_file * m, int elem) {

    unsigned long long start_time, end_time;

    printk(KERN_INFO "Testing insert\n");

    start_time = rdtsc_ordered();
    fill_linked_list(elem);
    end_time = rdtsc_ordered();
    list_time.fill_time += (end_time - start_time);
    // seq_printf(m, "Linked list insert for %i elements took %llu ticks\n", elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    fill_hash_table(elem);
    end_time = rdtsc_ordered();
    hash_time.fill_time += (end_time - start_time);
    // seq_printf(m, "Hash table insert for %i elements took %llu ticks\n", elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    fill_rb_tree(elem);
    end_time = rdtsc_ordered();
    tree_time.fill_time += (end_time - start_time);
    // seq_printf(m, "RB Tree insert for %i elements took %llu ticks\n", elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    fill_xarray(elem);
    end_time = rdtsc_ordered();
    xarray_time.fill_time += (end_time - start_time);
    // seq_printf(m, "XArray insert for %i elements took %llu ticks\n", elem, (end_time - start_time));

}

static void test_find(struct seq_file * m, int elem) {

    unsigned long long start_time, end_time;

    printk(KERN_INFO "Testing find\n");

    start_time = rdtsc_ordered();
    find_in_list(elem/2);
    end_time = rdtsc_ordered();
    list_time.find_time += (end_time - start_time);
    // seq_printf(m, "%i found in list of %i elements took %llu ticks\n", elem/2, elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    find_in_hash_table(elem/2);
    end_time = rdtsc_ordered();
    hash_time.find_time += (end_time - start_time);
    // seq_printf(m, "%i found in table of %i elements took %llu ticks\n", elem/2, elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    find_in_rb_tree(elem/2);
    end_time = rdtsc_ordered();
    tree_time.find_time += (end_time - start_time);
    // seq_printf(m, "%i found in rbtree of %i elements took %llu ticks\n", elem/2, elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    find_in_xarray(elem/2);
    end_time = rdtsc_ordered();
    xarray_time.find_time += (end_time - start_time);
    // seq_printf(m, "%i found in xarray of %i elements took %llu ticks\n", elem/2, elem, (end_time - start_time));
    
}

static void test_incrament(struct seq_file * m, int elem) {

    unsigned long long start_time, end_time;

    printk(KERN_INFO "Testing find\n");

    start_time = rdtsc_ordered();
    loop_and_increment_list();
    end_time = rdtsc_ordered();
    list_time.inc_time += (end_time - start_time);
    // seq_printf(m, "Linked list imcrament for %i elements took %llu ticks\n", elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    loop_and_increment_hash_table();
    end_time = rdtsc_ordered();
    hash_time.inc_time += (end_time - start_time);
    // seq_printf(m, "Hash table incrament for %i elements took %llu ticks\n", elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    loop_and_incrament_tree(&mytree);
    end_time = rdtsc_ordered();
    tree_time.inc_time += (end_time - start_time);
    // seq_printf(m, "RB Tree incrament for %i elements took %llu ticks\n", elem, (end_time - start_time));

    start_time = rdtsc_ordered();
    loop_and_incrament_xarray();
    end_time = rdtsc_ordered();
    xarray_time.inc_time += (end_time - start_time);
    // seq_printf(m, "XArray incrament for %i elements took %llu ticks\n", elem, (end_time - start_time));

}

static void cleanup(void) {

    destroy_linked_list_and_free();
    destroy_hash_table_and_free();
    destroy_xarray_and_free();
    rb_tree_destroy_and_free(&mytree);
}

/********************************* Parse ********************************/

static int __init lkp_rust_init(void)
{
	int err = 0;

	if (!num_elements) {
		printk(KERN_INFO "Missing \'num_elements\' parameter, exiting\n");
		return -1;
	}

    if (!num_tries) {
		printk(KERN_INFO "Missing \'num_tries\' parameter, exiting\n");
		return -1;
	}	

	proc_create("lkp_rust", 0, NULL, &lkp_rust_ops);

	return err;
}

static void __exit lkp_rust_exit(void)
{
	remove_proc_entry("lkp_rust", NULL);
    printk(KERN_INFO "Exiting lkp_rust module\n");
	return;
}

module_init(lkp_rust_init);

module_exit(lkp_rust_exit);