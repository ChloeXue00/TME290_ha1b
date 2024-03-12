# OpenDLV code formatting

OpenDLV microservices in the `core` and `community` groups needs to follow the
code guidelines represented by the `opendlv-clangformat` file in this repository.
The file should NOT be part of each OpenDLV microservice repository, but should
rather be used as a external file located on your system.

In OpenDLV Desktop, this is already prepared, and the below instructions are not
needed.

To download the file, run:

```bash
wget -O ${HOME}/.opendlv-clangformat https://git.opendlv.org/core/opendlv-template-microservice/-/raw/main/format/opendlv-clangformat
```

In order to automatically format a source code file make sure to first install
`clang-format`.

Ubuntu:
```bash
sudo apt install clang-format
```

Arch Linux:
```bash
sudo pacman -S clang
```

To then format a file, run the following command:
```bash
clang-format --style=file:${HOME}/.opendlv-clangformat some-file.cpp
```

The above command will only print the formatted output on the terminal, to
also save the file, run the below. IMPORTANT: Always commit changes before
running the formatter to avoid overwriting and loosing work.
```bash
clang-format --style=file:${HOME}/.opendlv-clangformat some-file.cpp > some-file.cpp
```

It has been found that earlier versions of `clang-format` does not give the
ideal result, and only version 17 or later seems to honor most settings. If your
system does not provide the correct version
(check with `clang-format --version`), please use Docker as explain below when
preparing to push towards `core` or `community`.

## Format using Docker

Prepare the Dockerized formatter by running:
```bash
docker build -t opendlv-tool-codeformat .
```

Then, run it using the following command:
```bash
cat some-file.cpp | docker run -i --rm opendlv-tool-codeformat
```

When saving the result, instead run it as below. IMPORTANT: Always commit
changes before running the formatter to avoid overwriting and loosing work.
```bash
cat some-file.cpp | docker run -i --rm opendlv-tool-codeformat > some-file.cpp
```

## Editor integration

It may be desirable to continuously keep the formatting up to standards by
integrating `clang-format` into your editor of choice. Here follows instructions
for different common editors.

### Vim

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

### Visual Studio Code

Install the extension
[Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format).
In the extension settings, set the path to the format file in the `style`
field, according to `file:${HOME}/.opendlv-clangformat`.
