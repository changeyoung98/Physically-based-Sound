# Physically-based-Sound

## 环境搭建
### 预处理
该部分代码参考项目 [SoundSynth](https://github.com/ztzhang/SoundSynth)以及内置的模态声项目[Modal Sound](https://github.com/cxzheng/ModalSound/tree/master/ModalSound)

根据项目的环境搭建指示安装依赖。在我的尝试中，Ubuntu 16.04 LTS虚拟机可以顺利跑通ModalSound项目，但是按照SoundSynth的指令构建modalSound模块出现了问题。于是我使用ModalSound的环境依赖，根据SoundSynth代码替换了原始modalsound的代码，可以build出所需要的可执行文件。

在这里，我的项目只使用了extmat这一可执行文件。所以项目使用只需构建ModalSound模块。如果项目后续进一步优化可以参考上述两个项目其他部分的源码。

当构建出extmat程序后，我们执行以下命令来获取.ev文件。
```sh
~/work/SoundSynth/modal_sound/build/bin$./extmat -f obj-1 -y 3.5E+9 -p 0.34 -m -k -g -s -d 1 | tee -a 'precalc.log'
# obj-1为模型名称，所在路径下应有obj-1.obj，obj-1.tet，
# -y 后面的3.5E+9是物体对应材质的杨氏模量
# -p 后面的0.34 是物体对应材质的泊松比
# 上述均为变量，可以根据需求替换
```
运行结果如下：
```sh
Load mesh [obj-1.tet] ... INFO: Read tetrahedron mesh: 3571 free vertices, 0 fixed vertices
INFO: Read 13576 tets
 [OK]
Initialize mesh ... Update surface: 4368 triangles
 [OK]
```
可以得到相应的刚性矩阵和质量矩阵，保存在spm文件中。
下面使用matlab进行矩阵计算
```sh
~/work/SoundSynth/modal_sound/build/bin$ sh /usr/local/MATLAB/R2018a/bin/matlab -nodisplay -nodesktop -nosplash -r "addpath('~/work/SoundSynth/file_generator');ev_generator60('obj-1', 60); quit"| tee -a 'mat.log'
# sh后面是matlab执行程序所在路径
# addpath里面是SoundSynth项目里提供的矩阵计算matlab程序所在路径，用到的是ev_generator60,这些都不需要更改，里面的obj-1可以更换为要处理的模型名称。
```
运行结果：
```sh
                            < M A T L A B (R) >
                  Copyright 1984-2018 The MathWorks, Inc.
                   R2018a (9.4.0.813654) 64-bit (glnxa64)
                             February 23, 2018

 
要开始，请键入以下项之一: helpwin、helpdesk 或 demo。
有关产品信息，请访问 www.mathworks.com。
 
reading obj-1.stiff.spm
 size: 10713 x 10713
reading finishedreading obj-1.mass.spm
 size: 10713 x 10713
reading finishedEigen Value calculated.s
```
这样就得到了.ev文件。

### 实时合成
这部分我们离开Linux虚拟机，在Windows环境下进行。
VS项目的环境依赖如下（根据自己安装依赖的位置调整）：
```
#项目属性 → C/C++ → 附加包含目录：
D:\Users\SFML-2.5.1\include
D:\Users\eigen3
D:\Users\OpenAL\include

# 项目属性 → 链接器 → 附加库目录：
D:\Users\SFML-2.5.1\lib
D:\Users\OpenAL\libs\Win64
D:\Users\OpenAL\libs\Win32
```
主函数在Source.cpp，调整生成目标为exe时，注释掉`# define _DLLExport __declspec (dllexport)` 和 `*extern "C" int _DLLExport USound(int obj, int mat, double force)`可以调试程序。反之，调整生成目标文dll时，保留上述两部分，即可生成dll。

### Unity部分
基本上没有环境依赖。VS合成的DLL放入Plugins文件夹下，并且注意startcpp中的dll名称对应，以及入口函数名称对应。
