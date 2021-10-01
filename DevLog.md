# October 1, 2021 (Friday) - Day 2
Ok, I don't want to sound like I'm whinging.. but.. omg, no bios interrupts! I knew there had to be some curve ball this was going the throw at me. Now I have to figure out how to get my video and keyboard! ... grumble grumble ...

Reading about how to interact with the hardware. Turns out I've been relying entirely on BIOS for all the things I was doing and thinking was doing things "on my own"... wake up call from hell!




# September 30, 2021 (Thursday) - Still Day 1
While making my "clear the screen and hello" kernel, I began to tweak the makefile to allow me to build the kernel into a bootable ISO image to make it easier for others to build it and see it for themselves. However, I ran into a little issue with grub-mkrescue, first I forgot to install xorriso. Totally my fault, the guide says it at least twice and I somehow managed to gloss over it. But grub-mkrescue kept giving me an error about `mformat`, which I had never heard of. Thanks to a google search and some helpful answers on the stack that overflows, I discovered that I needed to install the package `mtools` for grub-mkrescue to work. Who knew? Installing things makes things work!

Anyway.. NEXT!


# September 30, 2021 (Thursday) - Day 1
After doing a short practice to understand a few of the basic principles, I decided to take some friendly advice and log my progress. So I have decided to start over from the beginning and document my progress. I will not be discussing in great detail what I learned or why I chose a particular methodology. Instead I will allow the code changes to speak about what progress has been made and when I feel there's something interesting I should add, I'll make an entry into this log.

Today I have spent most of my time preparing the repo in a way that will be conducive to showcasing a work-in-progress project. I think I got something that'll work out nicely, but I'll probably tweak things as I go.