x86_64机器上为编译出32位执行文件

gcc -m32 -c main.c -o main.o

nasm -f elf entry.asm -o entry.o

ld -m elf_i386 -s main.o entry.o -o app.out