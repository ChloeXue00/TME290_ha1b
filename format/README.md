# OpenDLV code foramtting

OpenDLV microservices in the `core` and `community` groups needs to follow the
code guidelines represented by the `opendlv-clangformat` file in this repository.
The file should NOT be part of each OpenDLV microservice repository, but should
rather be used as a external file located on your system.

To download the file, run:

```bash
$ wget -O ${HOME}/.opendlv-clangformat https://git.opendlv.org/core/opendlv-template-microservice/-/raw/main/format/opendlv-clangformat
```

## Vim

To integrate the formatting rules into Vim, please install `clang-format` and
[vim-clang-format](https://github.com/rhysd/vim-clang-format) and add the
following into your `.vimrc`:
```
let g:clang_format#extra_args = "--style=file:${HOME}/.opendlv-clangformat"

autocmd FileType c,cpp,hpp,objc nnoremap <buffer><Leader>cf :<C-u>ClangFormat<CR>
autocmd FileType c,cpp,hpp,objc vnoremap <buffer><Leader>cf :ClangFormat<CR>
nmap <Leader>C :ClangFormatAutoToggle<CR>
```

Files can now be formatted using the `<Leader>+c+f` key combination. The leader
key is by default `\`. Automatic formatting can be activated using `<Leader>+C`.

## Visual Studio Code

Install the extension
[Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format).
In the extension settings, set the path to the format file in the `style`
field, according to `file:${HOME}/.opendlv-clangformat`.
