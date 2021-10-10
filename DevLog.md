# Octover 10, 2021 (Sunday) - Day 11
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