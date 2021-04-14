#include "AppDelegate.h"



static Application* s_pInstance = NULL;


/** ����һЩ���� */
Vector4 lightPos(2.0f, 5.0f, 0.0f, 1.0f);

#define MAX_SHADOW_CASTER_POINTS 4  /**< �����ڵ��嶥����� */
bool displayVolume = false;         /**< ������Ӱ���־ */  
float extendAmount = 5.0f;          /**< �������� */

/** �ڵ��嶥������ */
Vector4 ShadowObject[MAX_SHADOW_CASTER_POINTS];


Application* Application::create()
{
	if (s_pInstance)
		return s_pInstance;

	AppDelegate* app = new AppDelegate();
	s_pInstance = app;
	return reinterpret_cast<Application*>(app);
}

Application* Application::getInstance()
{
	return s_pInstance;
}

void Application::destroy()
{
	SAVE_FREE(s_pInstance);
}



AppDelegate::AppDelegate()
	: Application()
{
	_fps = 0;
}

bool AppDelegate::init()
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);						
	glClearDepth(1.0f);											
	glEnable(GL_DEPTH_TEST);

	/** ���ù�Դ���� */
	float ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
   float diffuseLight[] = {1.0, 1.0, 1.0, 1.0}; 
   float specularLight[] = {1.0, 1.0, 1.0, 1.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	initGLExt();

	resizeDraw(true);

	glEnable(GL_TEXTURE_2D);
	
	if(!_ground.loadTexture("res/ground.tga"))
	{
		MessageBox(NULL,"��������ʧ��!","����",MB_OK);
		exit(-1);
	}


	if(!_font.initFont())
	{
		Log("��ʼ������ʧ��!");
		return false;
	}

	ShadowObject[0] = Vector4(-1.0f, 2.5f, -1.0f);
   ShadowObject[1] = Vector4(-1.0f, 2.5f,  1.0f);
   ShadowObject[2] = Vector4(1.0f, 2.5f,  1.0f);
   ShadowObject[3] = Vector4(1.0f, 2.5f, -1.0f);

	return true;
}

void AppDelegate::unInit()
{
}


void AppDelegate::update(DWORD milliseconds)
{
	/** ���ƹ�Դλ���ƶ� */
	if(isPressed(VK_UP))
		lightPos.z -= 0.01f;
	if(isPressed(VK_DOWN))
		lightPos.z += 0.01f;

	if(isPressed(VK_LEFT))
		lightPos.x -= 0.01f;

    if(isPressed(VK_RIGHT))
		lightPos.x += 0.01f;
   
	/** �ո���л��Ƿ������Ӱ�� */
	if(isPressed(VK_SPACE))
	   displayVolume = !displayVolume;
}


void AppDelegate::caculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	     /**< ������ʾ֡�� */	
    static float lastTime			= 0.0f;	     /**< ��¼�ϴ�ʱ�� */						
    float currentTime = GetTickCount() * 0.001f; /**< ��õ�ǰʱ�� */	 			

	framesPerSecond++;                           /**< ��ʾ֡������1 */
    /** ���ʱ������1.0�� */
	if( currentTime - lastTime > 1.0f )          
    {
		
	    lastTime = currentTime;                   /**< ���浱ǰʱ�� */
		_fps = framesPerSecond;                  /**< ��ǰ֡������_Fps */
        framesPerSecond = 0;                      /**< ��֡������ */                    
    }
}

/** ���������Ϣ */
void  AppDelegate::printText()
{
	char string[128];                               /**< ���ڱ�������ַ��� */
	glPushAttrib(GL_CURRENT_BIT);                   /**< ����������ɫ������Ϣ */
	glColor3f(0.0f,1.0f,0.0f);                      /**< ����������ɫ */
	
	/** ���֡�� */
	caculateFrameRate();                               /**< ����֡�� */
    sprintf(string,"FPS:%3.0f", _fps);                 /**< �ַ�����ֵ */
	_font.printText(string, -5.0f,3.0f);              /**< ����ַ��� */
	glPopAttrib();
}

void AppDelegate::render()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  	glTranslatef(0.0f, -2.0f, -10.0f);
	
	/** ���Ƶ��� */
	glDisable(GL_LIGHTING);
	glPushMatrix();

	   /** ָ������ */
	   glBindTexture(GL_TEXTURE_2D, _ground.getID());

       ///�����ı���
      glBegin(GL_QUADS);
		 glNormal3f(0.0f, 1.0f, 0.0f); /**< ָ������ */
         glTexCoord2f(0.0, 0.0); glVertex3f(15.0f, -0.01f, 15.0f);
         glTexCoord2f(1.0, 0.0); glVertex3f(-15.0f, -0.01f, 15.0f);
         glTexCoord2f(1.0, 1.0); glVertex3f(-15.0f, -0.01f, -15.0f);
         glTexCoord2f(0.0, 1.0); glVertex3f(15.0f, -0.01f, -15.0f);
      glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
	/** ����Բ�� */
	glPushMatrix();
        glDisable(GL_TEXTURE_2D);
		glTranslatef(-2.0f, 0.8f, -0.2f);
		auxSolidTorus(0.2f, 0.4f);
	glPopMatrix();

   /** ���������� */
	glPushMatrix();
		glTranslatef(0.0f, 0.8f, -0.2f );
		auxSolidCube(1.0f);
	glPopMatrix();

   /** �������� */
	glPushMatrix();
		glTranslatef(2.0f, 0.8f, -0.2f);
		auxSolidSphere(0.7f);
    glPopMatrix();

   /** �����ڵ��� */
   glPushMatrix();
     glBegin(GL_QUADS);
  			glNormal3f(0, 1, 0);
        	glVertex3f(ShadowObject[0].x, ShadowObject[0].y, ShadowObject[0].z);
        	glVertex3f(ShadowObject[1].x, ShadowObject[1].y, ShadowObject[1].z);
			glVertex3f(ShadowObject[2].x, ShadowObject[2].y, ShadowObject[2].z);
			glVertex3f(ShadowObject[3].x, ShadowObject[3].y, ShadowObject[3].z);
	glEnd();
  glPopMatrix();


   /** ���ƹ�Դ */
	glDisable(GL_LIGHTING);
	glPushMatrix();

	    /** ���ù�Դλ�� */
		float lp[4];		
		
		lp[0] = lightPos.x; 
		lp[1] = lightPos.y;
		lp[2] = lightPos.z; 
		lp[3] = lightPos.w;
		glLightfv(GL_LIGHT0, GL_POSITION, lp);
	
		/** ��ʼ���� */
		glTranslatef(lightPos.x,lightPos.y, lightPos.z);
		glColor3f(1.0f, 1.0f, 0.5f);
		auxSolidSphere(0.1);
		glColor3f(1.0f,1.0f,1.0f);  
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

}

/** �������� */
void AppDelegate::drawSilhouette(Vector4 lightPos, float Extend)
{
	Vector4 extended;     
   int A, B;

   /** �رչ��� */
   glDisable(GL_LIGHTING);

   /** ������ɫ */
   glColor3f(0.5f, 0.7f, 0.5f);

   /** ��ÿ�����㶼���� */
   for(int i = 0; i < MAX_SHADOW_CASTER_POINTS; i++)
      {
          
          A = i; B = i + 1;

          /** �߽��� */
          if(B >= MAX_SHADOW_CASTER_POINTS) B = 0;

         /** �����ı��� */
         glBegin(GL_QUADS);

            /** ԭʼ����A */
            glVertex3f(ShadowObject[A].x, ShadowObject[A].y, ShadowObject[A].z);

            /** ����A�����ĵ� */
            extended.extendVertexPos(ShadowObject[A], lightPos, Extend);
            glVertex3f(extended.x, extended.y, extended.z);

            /** ����B�����ĵ� */
            extended.extendVertexPos(ShadowObject[B], lightPos, Extend);
            glVertex3f(extended.x, extended.y, extended.z);

            /** ԭʼ����B */
            glVertex3f(ShadowObject[B].x, ShadowObject[B].y, ShadowObject[B].z);

         glEnd();
      }

   glEnable(GL_LIGHTING);

}


void AppDelegate::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);			
	glLoadIdentity();
    
	/** �ر���ɫ����д�� */
	 glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	 
	 /** ��Ⱦ���� */
	 render();
	

	/** ָ��ģ�滺����� */
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);   
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0, 0);

	/** ������Ӱ��,ģ��ֵ��1 */   
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glCullFace(GL_BACK);
	drawSilhouette(lightPos, extendAmount);

	/** �ٴλ�����Ӱ��,ģ��ֵ��1 */
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	glCullFace(GL_FRONT);
	drawSilhouette(lightPos, extendAmount);

	///�ָ���ǰ������
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	/** ������Ӱ���� */
    glDisable(GL_LIGHT0);
	glStencilFunc(GL_EQUAL, 1, 1);
	render();

	/** ������Ӱ����Ĳ��� */
    glEnable(GL_LIGHT0);
	glStencilFunc(GL_EQUAL, 0, 1);
	render();

	/** �ر�ģ����� */
	glDepthFunc(GL_LESS);
	glDisable(GL_STENCIL_TEST);

    /** ������Ӱ�� */
	if(displayVolume == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);   /**< ָ���߿�ģʽ */
		drawSilhouette(lightPos, extendAmount);      /**< ������Ӱ�� */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   /**< �ָ����ģʽ */
	}

	/** �����Ļ��Ϣ */ 
	printText();
		
	glFlush();	
}


