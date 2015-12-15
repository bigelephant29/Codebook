syn on
set nu et si hl cul ts=4 sw=4 bs=2
map <F5> <ESC>:!./%<<CR>
map <F9> <ESC>:w<CR>:!g++ -O2 -Wall -std=c++0x % -o %<<CR>
map <c-F5> <ESC>:!./%< < %<.in<CR>
com INPUT sp %<.in

