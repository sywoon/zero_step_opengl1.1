# ����֮opengl1.1
ѧϰ<<OpenGL��Ϸ���>>�����еļ�¼���ĵá�

## ���빤��
 vs2015 [clang8.0.0 32λ](http://releases.llvm.org/download.html#8.0.0) [winsdk1809 10.0.17763.0](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive)


## utf-8��������
vs��utf-8����֧�ֲ��Ǻܺã���cl����ʱ���Դ���/source-charset:utf-8��֧��
```
  ����
    ����cl��exe �������ʱ����Ĭ�Ͼ���ansi�Ķ���Դ���ʽutf-8
    ���Ʊ���ʱ���Զ�ת����  ���½��������clang�г�ͻ
    
  Ϊ�˽�Դ�����ͨ�õ�utf-8���룬��Ӱ��Log�Ϳ���̨���ĵ���ʾ��
  ���������
    1 cl�ж������ĺ��������õ��Ǹ�������
      cl *.cpp /source-charset:utf-8 /D COMPILE_CL
    2 �޸�LogԴ��  ��ͳһ���log�ļ���ʽΪansi
        bool bFromUtf8 = false;
        char* pszAnsi = NULL;

    #ifdef COMPILE_CL
        bFromUtf8 = false;
    #else
        if (utf82ansi(pszBuf, &pszAnsi))
        {
            bFromUtf8 = true;
        }
    #endif
```




















