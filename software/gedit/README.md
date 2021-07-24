1 mkdir -p ~/.local/share/gedit/plugins
2 将clangformat.plugin, clangformat.py这两个文件拷贝到上面的目录下面
3 打开gedit, Preferences->Plugins, 勾选clang-format plugin选项.
4 安装clang-format, 确保/usr/bin下面有clang-format, 如下所示:
mcl@mcl:/usr/bin$ ll clang-format*
lrwxrwxrwx 1 root root   16 3月  26 13:55 clang-format -> clang-format-3.9*
lrwxrwxrwx 1 root root   32 12月 27  2017 clang-format-3.9 -> ../lib/llvm-3.9/bin/clang-format*
-rwxr-xr-x 1 root root 4447 12月 27  2017 clang-format-diff-3.9*
5 安装python3
6 然后就可以了，gedit中的快捷键为CTRL+SHIFT+F
