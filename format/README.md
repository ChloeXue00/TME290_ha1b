# OpenDLV code foramtting

OpenDLV microservices in the `core` and `community` groups needs to follow the
code guidelines represented by the `.clang-format` file in this repository.

## Vim

To integrate the formatting rules into Vim, please use the `tovimrc.sh` script
to generate settings for [vim-clang-format](https://github.com/rhysd/vim-clang-format)
to put in `.vimrc`.

## Visual Studio Code

Install the extension `Clang-Format. In the Extension Settings, set the path to
the `.clang-format` file in this repository for Assume Filename (not tested).
