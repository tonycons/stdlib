#pragma GCC diagnostic ignored "-Weverything"  // idgaf

/*
 Implementation of chkstk: https://github.com/skeeto/w64devkit/blob/master/src/libchkstk.S
*/
extern "C" void __chkstk() {
    asm volatile(R""(

	pushq       %rax
	pushq       %rcx
	movq        %gs:(0x10), %rcx	// rcx = stack low address
	negq        %rax		        // rax = frame low address
	addq        %rsp, %rax		    
	jb          1f			        // frame low address overflow?
	xorl        %eax, %eax		    // overflowed: frame low address = null
0:	subq        $0x1000, %rcx	    // extend stack into guard page
	testl       %eax, (%rcx)	    // commit page (two instruction bytes)
1:	cmpq        %rax, %rcx
	ja          0b
	popq        %rcx
	popq        %rax
)"");
}
