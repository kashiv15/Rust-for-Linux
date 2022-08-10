#![no_std]

extern crate alloc;

use alloc::borrow::ToOwned;
use alloc::string::String;

use linux_kernel_module::println;

struct HelloWorldModule {
    message: String,
}

impl linux_kernel_module::KernelModule for HelloWorldModule {
    fn init() -> linux_kernel_module::KernelResult<Self> {
        println!("Hello kernel module!");
        Ok(HelloWorldModule {
            message: "Inside module!".to_owned(),
        })
    }
}

impl Drop for HelloWorldModule {
    fn drop(&mut self) {
        println!("My message is {}", self.message);
        println!("Exit module!");
    }
}

linux_kernel_module::kernel_module!(
    HelloWorldModule,
    author: b"modified by kashi for Linux Kernel project",
    description: b"A simple kernel module",
    license: b"GPL"
);
