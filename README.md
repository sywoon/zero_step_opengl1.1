# ����֮opengl1.1
ѧϰ<<OpenGL��Ϸ���>>�����еļ�¼���ĵá�

## ���빤��
 vs2015 clang


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
    2 �޸�LogԴ��
      #ifdef COMPILE_CL
		printf("%s", pszBuf);
      #else
		char* pszAnsi = NULL;
		if (utf82ansi(pszBuf, &pszAnsi))
		{
			printf("%s", pszAnsi);
			free(pszAnsi);
		} else {
			printf("%s", pszBuf);
		}
      #endif
      
   ȱ�ݣ�����exe���ܻ��� cl���������log��ʽ��ansi
       ��clang�����log��ʽ��utf-8��ŷ���
```





