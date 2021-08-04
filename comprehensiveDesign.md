```plantuml
digraph G{
    rankdir=TB
    compound = true              
    "libstdc++标准库实现"[style = "filled" color = "grey"]
    "glib标准库实现"[style = "filled" color = "grey"]
    subgraph clusterCompiler{
        rankdir=TB;
        compound = true;  
        label = "编译器"
        style = "dashed";
        rank = same{glib,C_source}
        glib[style = "filled" color = "grey"]
        glib ->C_source [style = "dotted" label="需要OS支持posix"]
        "libstdc++"[style = "filled" color = "grey"]
        "libstdc++" ->CPP_source [style = "dotted" label="需要glib"]
        C_source -> lexer;
        C_source[style = "filled"color = "black" fontcolor = "white"]
        CPP_source[style = "filled"color = "grey" ]
        subgraph cluster_Front{
            node[shape = "rectangle" style = "filled"color = "black" fontcolor = "white"]lexer,parser,Inter;
            rankdir=LR
            label = "编译器前端"
            style = "dashed";
            lexer[
                shape = "rectangle"
                style = " filled"
                color = "black" 
                fontcolor = "white"
                label = "词法分析"
            ]
            parser[
                label = "语法分析"
            ]
            Inter[
                label = "中间代码生成"
            ]
            lexer->parser[label=" token"]
            parser->Inter[label=" AST"]
        }
        subgraph cluster_IR{
            label = "中间代码"
            IR[label = "LLVM IR code" ]
        }
        Inter -> IR[lhead = cluster_Front ltail = cluster_IR weight = 100]
        IR_Input -> IR
        CPP_source -> Clang
        Clang -> IR
        IR->IR_Output[weight = 100]
        IR->optimizer[weight = 20]
        subgraph clusterBack{
            style = "dashed";
            label = "编译器后端"
            optimizer->IR_Output
            IR_Output[label = "LLVM IR code"]
            IR_Output->translate
            translate ->asmer   
            translate[ shape = "rectangle" label = "IR生成汇编" style = "filled"color = "black" fontcolor = "white"]
            optimizer[style = "filled"color = "grey" shape = "rectangle"]
            asmer[shape = "rectangle" label ="汇编器" style = "filled"color = "black" fontcolor = "white"]
            linker[style = "filled"color = "grey" shape = "rectangle"]
            oFile
            elfFile[label ="bin纯二进制文件"]
            execFile

        }
        linker ->elfFile [style = "invis"]
        asmer -> elfFile [weight = 100 label="对os无依赖，单文件，无需linker"]
        asmer -> oFile
        oFile ->linker [weight = 100]
        linker -> execFile [weight = 100 label="单文件"]
        oFile ->linker [weight = 10,label="多文件"]       
        "libstdc++标准库实现"->linker[style = "dotted"]
        "glib标准库实现"->linker[style = "dotted"]
    }

    elfFile -> coeinput
    elfFile -> loadInput
    execFile->loadInput
    subgraph clusterHardware{ 
        style = "dashed";
        label = "硬件"
        node[shape = "rectangle"] cpu,simulator;
        coeinput[ style = "filled"color = "black" fontcolor = "white"]
        loadInput[ style = "filled"color = "grey"]
        coeinput -> cpu[label = "coe文件直接初始化cpu主存,使用物理地址，首地址为0（bios在非0处，初始化完成后跳转至地址0"]
        loadInput -> simulator
        loadInput -> cpu[label = "串口下载、存储器载入(OS)"];
        rank = same{cpu,simulator}
        cpu[label=
            <
                <table>
                    <tr>
                        <td bgcolor="black" align="center" colspan="2">
                            <font color="white">RISCV CPU Based on fpga </font>
                        </td>
                    </tr>
                    <tr>
                        <td align="left" bgcolor="black">
                            <font color="white">&#40;0&#41; 双核流水CPU（顺序/乱序)</font>
                        </td>
                        <td align="left" bgcolor="grey">
                            <font color="black">&#40;1&#41; AI coprocessor(or gpgpu)</font>
                        </td>
                    </tr>
                    
                </table>
            >
            ]

        simulator[label=
            <
                <table border="0" cellborder="0" cellpadding="3" bgcolor="white">
                    <tr>
                        <td bgcolor="grey" align="center" colspan="2">
                            CPU simulator: Based on Qemu with RISCV
                        </td>
                    </tr>
                </table>
            >]
        cpu -> simulator[dir = "both"]
    }


    subgraph clusterOS{

        label = "OS"
    }

    subgraph clusterApplication{
        label = "应用"
    }
    clusterCompiler -> clusterHardware




}
```
</br>
#综合课设-软件篇</br>
##汇编器</br>
#####·两次扫描</br>
第一次完成展开缩写指令，对每行指令、伪指令地址标定，构建标号表（所有label->地址的映射关系）</br>
第二次进行正常的词法分析、语法分析，通过语法制导翻译生成机器码。</br>

任务：</br>
·仿照补充讲义中MIPS汇编的词法、语法定义构建RISCV词法、语法规范定义。按lex、yacc规则编写.l,.y文件。
·可以参照龙书，c99.l 及其yacc描述。词法规则参见龙书中lex的正则（向前看符'\'不可用)



2021.8.3

编译器前端：
clang前端 ast节点类型，模仿其写一个ast，正在读

汇编器、链接器：
.o 可重定向文件
elf文件结构
段定义
符号表等等
需要os加载器来实现重定向

glibc 危

方向1
可以考虑做个extend c，放弃c++，手动实现部分常用库函数
做个文件、线程操作的小系统（xv6）

方向2
用c写编译器，以实现自举（在os中可以编译编译器）
