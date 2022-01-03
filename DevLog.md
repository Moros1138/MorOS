# January 3, 2022 (Monday)
First, HAPPY NEW YEAR!!!

I've decided that I wanted to make a game more than I felt like implementing the C++ STL. The consequences of that decision is the abandonment of the idea of using the old::PixelGameEngine API to drive the creation of the game. I believe I have sufficient functionality developed to make a game, as is.

I still intend to write the game routines in C++ because I rather like namespaces and classes, just a few of the nice things C++ gives me without having to reinvent the wheel too much. 

A note about C++ and STL. I'll be leaving my STL work in the repo in case I decide to use what I've got working, so with the project. I just don't want the focus of this particular project to be that of reinventing the C++ STL wheel.

# December 10, 2021 (Friday) - Hell if I know!
Sooo.. everything we've accomplished so far. Assume that I have scrapped it all and started over. While some things may look similar, the entire system, step by step has been painstakingly reimplemented in an attempt to understand what's going on!

A few things I want to focus on.

First, I decided to cheat and link libgcc into my kernel which gives me 64-bit math functions in 32-bit protected mode.

I've decided that the core kernel functions would be written in C, not C++. Not that I think C++ is a bad idea or anything but I found the temptation to use C++ features would distract me from getting the work done.

Implemented a rudimentary set of memory allocation (heap) functionality. ``malloc``, ``free``, ``realloc``, and ``free`` are now available through the kernel after the heap is initialized. The heap is initialized IMMEDIATELY following the initialization of the CPUs GDT/IDT and interrupt services!

Implemented a rudimentary keyboard and mouse interrupt handler. A short round of testing proved that I was going to need some sort of queue to make handling events across the kernel less like putting my balls in a vice!

Once I had an event queue implemented, I spent some more time on the keyboard and mouse interrupt handlers to make them utilize the event system. The event system got some tweaks to make everything play nicely as well.

Finally, the reason I took the break, I hit a brick wall concerning the touchpad on my laptop. It wouldn't work. I spent a great deal of time learning that I would need to write a completely separate and custom driver to support the touchpad because it doesn't actually use the PS/2 protocol. I've decided to put that on the back burner because I'm less interested in getting a touchpad working anyways. I just thought it would be really cool to show off a mouse **working** on a **REAL MACHINE**.

That brings us to now. The next step is to bring in the olc::PixelGameEngine and write my custom ``platform`` and ``renderer`` for it that utilizes everything MorOS can currently provide AND to see what else I need to provide!

I imagine I will have to implement more than a few C++ STL. I've already implemented the ``std::vector``. It's probably not the best, but who cares!

That's all for now.



# October 17, 2021 (Sunday) - Day 18
So I wanted to really really learn more about what was actually happening with the VGA. I found some more examples and started to get a much better idea of what was going on.

Anyways, I've also detected vertical retrace, which allows me to swap buffers in a much cleaner way. Got a working(ish) mouse. It's a little awkward in the way it moves and I'm not sure yet why that is, but I'll work on that later.

Next on my agenda is tackling the keyboard, add a few more features to that.



# October 13, 2021 (Wednesday) - Day 14
TWO WEEKS!! And I got to my first keyboard!

So, a few things I've done since I last made an entry... In my quest for VGA 13h mode. I learned that I had failed to implement a 16-bit port writing. Got that out of the way and then I turned to some very interesting documentation to get me into a screen that I could make graphics on!

Found a function that did [all the heavy lifting here](http://bos.asmhackers.net/docs/vga_without_bios/docs/mode%2013h%20without%20using%20bios.htm). But because I wasn't planning to support multiple mode changes, I felt like it would be ok to simply refactor the code down to exactly what was required for me to switch to 13h without BIOS interrupts (which as you know by now, I lack in protected mode).

Anyways, after I refactored it down to exactly what I needed for 320x200 in 256 colors, I decided that simply copying someone else's code wasn't really helping me learn anything useful. So I spent most of the last couple days reading through [VGA Hardware](https://wiki.osdev.org/VGA_Hardware) on the OS Dev Wiki, as well as the resources at [FreeVGA on OSDever](http://www.osdever.net/FreeVGA/vga/vga.htm). If you need to do stuff with VGA... These are excellent places to start!

I also took some time to tweak the timer. I decided that I wanted to have 1 tick for every millisecond (ish) .. this makes it super easy to calculate timing later on when I start porting PGE.

Oh, did I mention I got my keyboard working? It is definitely **NOT** a fully implemented keyboard driver at this point. But I'm getting closer and closer to bringing PGE over. Getting excited!




# October 10, 2021 (Sunday) - Day 11
I feel like I've accomplished alot today! I have implemented and refactored the GDT and IDT. Implemented ISR and IRQ and have a working timer! Now I'm going to move on to getting user input from the keyboard and mouse! Wish me luck!



# October 9, 2021 (Saturday) - Still Day 10
OMFG... I'm simultaneously relieved and upset right now! After all this time banging my head against the brick wall trying to get this kernel to have interrupts with C++. It turns out the problem wasn't with the code, it was with the build command in the Makefile.

Apparently optimizing is BAD! ``-O2`` was the difference between code that worked and code that broke.




# October 9, 2021 (Saturday) - Day 10
Lots of things have happened over the last few days. I got started on implementing interrupts only to hit a bit of a brick wall. For reasons I have yet to identify, when I started implementing interrupts it would cause my kernel to boot loop. This was frustrating, to say the least. Then I found a tutorial that does everything I want to do, except in C instead of C++. I spent some time following that tutorial and really getting a better understanding of what was going on. I learned a few things about my previous attempt with C++..

* I fundamentally misunderstood GDT and how to implement it.
* I fundamentally misunderstood IDT, ISR, the handlers, and how to implement it.
* Finally, C++ is a complete pain in the ass when it has to interact with a C ABI!!

Anyways, armed with my newly acquired knowledge I have thrown much of the project out to make way for a new (better?) approach and hopefully this time I will be successful and able to move on to the next phase. Fingers crossed?!



# October 3, 2021 (Sunday) - Still Day 4 
Totally learned how to embed assets into the kernel image AND how to access it from in the C/C++ code. This basically negates my need to have any sort of memory manager. I may stick a pin in the memory manager and shift my focus back to getting GDT and IDT going so I can start responding to interrupts. 



# October 3, 2021 (Sunday) - Day 4
Turns out the ``printf`` is a bigger beast than I had thought and it definitely turned into one of those wheels I don't feel like re-inventing (at least for now). So to put a pin on it until some future time I have chosen to use a drop-in ``printf`` implementation [I found on github. It's pretty awesome and works like a charm](https://github.com/mpaland/printf)!

Now I can shift my focus back to memory!



# October 2, 2021 (Saturday) - Day 3
In my quest for memory management, I learned that my abilty to represent variables on the screen was woefully inadequate. So I spent alot of my time today learning how to use multiboot's memory stuff to get a better understanding of how to know that I'm accessing memory that is both in existence and availble for my use (I don't want to presume that because I'm the kernel, I own all.. apparently that's not true... so much for my all powerful god-like powers!).

Anyways, as I got started with the memory management I started to realize that I REALLY REALLY REALLY want ``printf`` which I don't have currently. So been beefing up my terminal output routines.




# October 1, 2021 (Friday) - Day 2
Ok, I don't want to sound like I'm whinging.. but.. omg, no bios interrupts! I knew there had to be some curve ball this was going the throw at me. Now I have to figure out how to get my video and keyboard! ... grumble grumble ...

Reading about how to interact with the hardware. Turns out I've been relying entirely on BIOS for all the things I was doing and thinking was doing things "on my own"... wake up call from hell!




# September 30, 2021 (Thursday) - Still Day 1
While making my "clear the screen and hello" kernel, I began to tweak the makefile to allow me to build the kernel into a bootable ISO image to make it easier for others to build it and see it for themselves. However, I ran into a little issue with grub-mkrescue, first I forgot to install xorriso. Totally my fault, the guide says it at least twice and I somehow managed to gloss over it. But grub-mkrescue kept giving me an error about `mformat`, which I had never heard of. Thanks to a google search and some helpful answers on the stack that overflows, I discovered that I needed to install the package `mtools` for grub-mkrescue to work. Who knew? Installing things makes things work!

Anyway.. NEXT!


# September 30, 2021 (Thursday) - Day 1
After doing a short practice to understand a few of the basic principles, I decided to take some friendly advice and log my progress. So I have decided to start over from the beginning and document my progress. I will not be discussing in great detail what I learned or why I chose a particular methodology. Instead I will allow the code changes to speak about what progress has been made and when I feel there's something interesting I should add, I'll make an entry into this log.

Today I have spent most of my time preparing the repo in a way that will be conducive to showcasing a work-in-progress project. I think I got something that'll work out nicely, but I'll probably tweak things as I go.