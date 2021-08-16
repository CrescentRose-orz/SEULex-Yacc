# SEULex-Yacc
## 项目简介
- 本项目是18级编译原理课程设计项目，组员为薛江天、廖盼和殷春锁。
- 本项目共由SexLex和SeuYacc组成，分别对应编译器的词法分析和语法分析部分
- 通过gcc的make工具编译，makefile为编译Lex buildYacc为编译Yacc
- make 编译lex与yacc，可执行程序分别位于src/SeuLex/bin/Lex 与src/SeuYacc/bin/Yacc
- make Lex，make Yacc 分别单独编译Lex与Yacc
- 2021.8.10 Lex修复了\t \f 制表符等空白字符支持的错误
- 2021.8.16 Lex 开始加入lex直接支持词法分析


Tips:
    对于自定义词法，可以通过dot编译生成的visualNFA，查看生成的NFA，以检查正确性，DFA不推荐（边太多）。